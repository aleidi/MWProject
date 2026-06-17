#include "Character/MWAppearanceComponent.h"

#include "Character/MWCharacter.h"
#include "Character/MWCharacterAnimInstance.h"
#include "Data/Character/MWCharacterDataManager.h"
#include "Data/Character/MWCharacterPrimaryData.h"
#include "Pawn/MWPawnExtensionComponent.h"

void UMWAppearanceComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* owner = GetPawn<APawn>();
	if (!owner)
	{
		return;
	}

	UMWPawnExtensionComponent* pawnExtComp = owner->FindComponentByClass<UMWPawnExtensionComponent>();
	if (!pawnExtComp)
	{
		return;
	}

	UMWCharacterDataManager* dataMgr = UMWCharacterDataManager::Get(this);
	if (!dataMgr)
	{
		return;
	}

	CachedCharacterDataId = pawnExtComp->GetPawnDataId();

	if (UMWCharacterPrimaryData* loaded = dataMgr->GetLoadedCharacterPrimaryData(CachedCharacterDataId))
	{
		ApplyAppearance(loaded);
		return;
	}

	TWeakObjectPtr<UMWAppearanceComponent> weakThis(this);
	const int32 dataId = CachedCharacterDataId;

	dataMgr->AsyncLoadCharacterBundles(
		dataId,
		{ FName(TEXT("Spawn")) },
		FStreamableDelegate::CreateLambda([weakThis, dataId]()
		{
			UMWAppearanceComponent* self = weakThis.Get();
			if (!IsValid(self))
			{
				return;
			}

			UMWCharacterDataManager* mgr = UMWCharacterDataManager::Get(self);
			if (!mgr)
			{
				return;
			}

			if (UMWCharacterPrimaryData* pd = mgr->GetLoadedCharacterPrimaryData(dataId))
			{
				self->ApplyAppearance(pd);
			}
		}));
}

void UMWAppearanceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UMWAppearanceComponent::ApplyAppearance(UMWCharacterPrimaryData* PrimaryData)
{
	if (!PrimaryData)
	{
		return;
	}

	AMWCharacter* character = GetPawn<AMWCharacter>();
	if (!character)
	{
		return;
	}

	USkeletalMeshComponent* visualMesh = character->GetVisualMesh();
	if (!visualMesh)
	{
		return;
	}

	if (USkeletalMesh* mesh = PrimaryData->DefaultAppearance.Mesh_Body.Get())
	{
		visualMesh->SetSkeletalMesh(mesh);
	}

	if (UClass* animClass = PrimaryData->DefaultAnimation.AnimInstance.Get())
	{
		visualMesh->SetAnimInstanceClass(animClass);
	}
}