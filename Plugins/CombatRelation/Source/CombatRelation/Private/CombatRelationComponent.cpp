#include "CombatRelationComponent.h"

#include "CombatRelationService.h"

UCombatRelationComponent::UCombatRelationComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatRelationComponent::BeginPlay()
{
	Super::BeginPlay();

	LastCleanupTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
}

void UCombatRelationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetWorld())
	{
		return;
	}

	const float now = GetWorld()->GetTimeSeconds();
	if (CleanupTickInterval <= 0.f || (now - LastCleanupTime) >= CleanupTickInterval)
	{
		CleanupExpiredOverrides_Internal();
		LastCleanupTime = now;
	}
}

void UCombatRelationComponent::SetActorOverride(AActor* Target, ECombatRelation InRelation, float DurationSeconds)
{
	if (!Target || Target == GetOwner())
	{
		return;
	}

	const float now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
	const float expire = (DurationSeconds > 0.f) ? (now + DurationSeconds) : -1.f;

	for (FRelationOverrideEntry& e : ActorOverrides)
	{
		if (e.Target.Get() == Target)
		{
			e.Relation = InRelation;
			e.ExpireTime = expire;
			return;
		}
	}

	FRelationOverrideEntry newE;
	newE.Target = Target;
	newE.Relation = InRelation;
	newE.ExpireTime = expire;
	ActorOverrides.Add(newE);
}

void UCombatRelationComponent::ClearActorOverride(AActor* Target)
{
	if (!Target)
	{
		return;
	}

	ActorOverrides.RemoveAll([Target](const FRelationOverrideEntry& E)
	{
		return E.Target.Get() == Target;
	});
}

void UCombatRelationComponent::SetFactionOverride(FGameplayTag OtherFaction, ECombatRelation InRelation, float DurationSeconds)
{
	if (!OtherFaction.IsValid())
	{
		return;
	}

	const float now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
	const float expire = (DurationSeconds > 0.f) ? (now + DurationSeconds) : -1.f;

	FFactionOverrideEntry& entry = FactionOverrides.FindOrAdd(OtherFaction);
	entry.Relation = InRelation;
	entry.ExpireTime = expire;
}

void UCombatRelationComponent::ClearFactionOverride(FGameplayTag OtherFaction)
{
	if (!OtherFaction.IsValid())
	{
		return;
	}

	FactionOverrides.Remove(OtherFaction);
}

void UCombatRelationComponent::ClearAllOverrides()
{
	ActorOverrides.Reset();
	FactionOverrides.Reset();
}

UCombatRelationService* UCombatRelationComponent::GetService() const
{
	if (const UWorld* world = GetWorld())
	{
		if (const UGameInstance* GI = world->GetGameInstance())
		{
			return GI->GetSubsystem<UCombatRelationService>();
		}
	}

	return nullptr;
}

ECombatRelation UCombatRelationComponent::GetRelationTo(const AActor* Other) const
{
	if (!Other || Other == GetOwner())
	{
		return ECombatRelation::Ignore;
	}

	// 1) アクターオーバーライド
	for (const FRelationOverrideEntry& entry : ActorOverrides)
	{
		if (!entry.Target.IsValid())
		{
			continue;
		}

		if (entry.Target.Get() == Other)
		{
			return entry.Relation;
		}
	}

	// 2) 相手の識別情報
	const UCombatRelationComponent* CRComp = UCombatRelationLibrary::FindRelationComponent(Other);
	if (!CRComp)
	{
		return ECombatRelation::Neutral;
	}

	// 3) パーティーの高速判定
	if (PartyId != 0 && PartyId == CRComp->PartyId)
	{
		return ECombatRelation::Friendly;
	}

	// 4) チームの高速判定
	if (TeamId != 0 && TeamId == CRComp->TeamId)
	{
		return ECombatRelation::Friendly;
	}

	// 5) 陣営オーバーライド
	if (const FFactionOverrideEntry* entry = FactionOverrides.Find(CRComp->Faction))
	{
		return entry->Relation;
	}

	// 6) 集中管理されたマトリックス
	if (UCombatRelationService* svc = GetService())
	{
		return svc->Query(Faction, CRComp->Faction);
	}

	return ECombatRelation::Neutral;
}

bool UCombatRelationComponent::IsValidTarget(const AActor* Other, ETargetPolicy Policy) const
{
	if (!Other)
	{
		return false;
	}

	if (Policy == ETargetPolicy::SelfOnly)
	{
		return Other == GetOwner();
	}

	const ECombatRelation relation = GetRelationTo(Other);
	if (relation == ECombatRelation::Ignore)
	{
		return false;
	}

	switch (Policy)
	{
	case ETargetPolicy::HostileOnly:
		return relation == ECombatRelation::Hostile;

	case ETargetPolicy::FriendlyOnly:
		return relation == ECombatRelation::Friendly;

	case ETargetPolicy::NeutralOnly:
		return relation == ECombatRelation::Neutral;

	case ETargetPolicy::AnyExceptIgnore:
		return true;

	case ETargetPolicy::PartyOnly:
		{
			const UCombatRelationComponent* CRComp = UCombatRelationLibrary::FindRelationComponent(Other);
			return CRComp && PartyId != 0 && PartyId == CRComp->PartyId;
		}

	case ETargetPolicy::FactionFriendlyOnly:
		{
			const UCombatRelationComponent* CRComp = UCombatRelationLibrary::FindRelationComponent(Other);
			if (!CRComp)
			{
				return false;
			}

			if (UCombatRelationService* svc = GetService())
			{
				return svc->Query(Faction, CRComp->Faction) == ECombatRelation::Friendly;
			}

			return false;
		}
	}

	return false;
}

void UCombatRelationComponent::CleanupExpiredOverrides_Internal()
{
	const float now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;

	// アクターオーバーライド
	ActorOverrides.RemoveAll([now](const FRelationOverrideEntry& entry)
	{
		if (!entry.Target.IsValid())
		{
			return true; // ターゲットが破棄済み
		}

		return (entry.ExpireTime > 0.f && now >= entry.ExpireTime);
	});

	// 陣営オーバーライド
	TArray<FGameplayTag> toRemove;
	for (const auto& kvp : FactionOverrides)
	{
		const FFactionOverrideEntry& entry = kvp.Value;
		if (entry.ExpireTime > 0.f && now >= entry.ExpireTime)
		{
			toRemove.Add(kvp.Key);
		}
	}

	for (const FGameplayTag& tag : toRemove)
	{
		FactionOverrides.Remove(tag);
	}
}
