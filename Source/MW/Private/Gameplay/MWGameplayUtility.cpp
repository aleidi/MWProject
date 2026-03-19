#include "Gameplay/MWGameplayUtility.h"

#include "Data/GameplayAbility/ChargeableSkillData.h"
#include "Define/MWDefineGameplay.h"
#include "Gameplay/MWGameplayTags.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "Interface/MWSelectableInterface.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Public/EngineUtils.h"

void UWMGameplayUtility::SearchSelectableTargets(const APlayerController* PC, TArray<FMWFoundActorInfo>& FindTargets, const TArray<AActor*>& ActorsToIgnore)
{
	if (!PC || !PC->PlayerCameraManager)
	{
		return;
	}

	FindTargets.Reset();

	for (TActorIterator<APawn> It(PC->GetWorld()); It; ++It)
	{
		AActor* target = *It;
		// filter non-selectable target and it's not on screen
		if (!target->GetClass()->ImplementsInterface(UMWSelectableInterface::StaticClass()) || !target->WasRecentlyRendered())
		{
			continue;
		}

		const FVector angleVec = target->GetActorLocation() - PC->PlayerCameraManager->GetCameraLocation();

		const float dotXY = FVector::DotProduct(angleVec.GetUnsafeNormal2D(), PC->PlayerCameraManager->GetCameraRotation().Vector().GetUnsafeNormal2D());
		const float dotZ = FVector::DotProduct(angleVec.GetUnsafeNormal(), UKismetMathLibrary::ProjectVectorOnToVector(angleVec, PC->PlayerCameraManager->GetCameraRotation().Vector()).GetUnsafeNormal());
		const float angleXY = UKismetMathLibrary::DegAcos(dotXY);
		const float angleZ = UKismetMathLibrary::DegAcos(dotZ);

		auto GetZRangeAccordingToXY = [](float XY) -> float
			{
				return 0.4167 * XY + 40.f;
			};

		if (FMath::IsWithinInclusive(angleXY, 0.f, 80.f) && FMath::IsWithinInclusive(angleZ, 0.f, GetZRangeAccordingToXY(angleXY)))
		{
			// filter ignored actor
			if(ActorsToIgnore.Contains(target)) continue;

			const bool bLeft = FVector::CrossProduct(PC->PlayerCameraManager->GetCameraRotation().Vector().GetUnsafeNormal2D(), angleVec.GetUnsafeNormal2D()).Z > 0;
			FMWFoundActorInfo info;
			info.Name = target->GetName();
			info.bLeft = bLeft;
			info.Angle = angleXY;
			FindTargets.Emplace(info);
		}
	}
}

void UWMGameplayUtility::SaveConfig(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		WorldContextObject->SaveConfig();
	}
}

void UWMGameplayUtility::LoadConfig(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		WorldContextObject->LoadConfig();
	}
}

void UWMGameplayUtility::CastAbility(AActor* Actor, const FGameplayTag& AbilityTag)
{
	if (UMWAbilitySystemComponent* mwasc = Actor->FindComponentByClass<UMWAbilitySystemComponent>())
	{
		mwasc->AbilityInputTagPressed(AbilityTag);
	}
}

FGameplayTag UWMGameplayUtility::GetInputChargeStageTag(EMWInputChargeStage ChargeStage)
{
	switch (ChargeStage)
	{
		case EMWInputChargeStage::NoCharge:
			return MWGameplayTags::Ability_InputCharge_NoCharge;
		case EMWInputChargeStage::Light:
			return MWGameplayTags::Ability_InputCharge_Light;
		case EMWInputChargeStage::Perfect:
			return MWGameplayTags::Ability_InputCharge_Perfect;
		case EMWInputChargeStage::Overcharge:
			return MWGameplayTags::Ability_InputCharge_OverCharge;
		default:
			return FGameplayTag::EmptyTag;
	}
}
