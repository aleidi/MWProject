#include "Character/MWAppearanceComponent.h"

#include "Character/MWCharacter.h"
#include "Character/MWCharacterAnimInstance.h"
#include "Data/Character/MWCharacterData.h"
#include "Data/Character/MWCharacterDataManager.h"
#include "Engine/AssetManager.h"
#include "Pawn/MWPawnExtensionComponent.h"

void UMWAppearanceComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* owner = GetPawn<APawn>();
	UMWPawnExtensionComponent* pawnExtComp = owner ? owner->FindComponentByClass<UMWPawnExtensionComponent>() : nullptr;

	if (!pawnExtComp)
	{
		return;
	}

	int32 dataId = pawnExtComp->GetPawnDataId();
	FMWCharacterDataRow* data = GET_CHARDATAMGR(this)->GetCharacterData(dataId);
	if (!data)
	{
		return;
	}

	if (!data->DefaultAppearance.IsNull())
	{
		AppearancePrimaryAssetId = UAssetManager::Get().GetPrimaryAssetIdForPath(
			data->DefaultAppearance.ToSoftObjectPath()
		);

		if (AppearancePrimaryAssetId.IsValid())
		{
			UAssetManager::Get().LoadPrimaryAsset(
				AppearancePrimaryAssetId,
				TArray<FName>(),
				FStreamableDelegate::CreateUObject(
					this,
					&UMWAppearanceComponent::OnDefaultAppearanceLoaded,
					AppearancePrimaryAssetId)
			);
		}
	}

	if (!data->AnimInstance.IsNull())
	{
		AnimInstanceStreamableHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			data->AnimInstance.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &UMWAppearanceComponent::OnAnimInstanceClassLoaded)
		);
	}
}

void UMWAppearanceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AppearancePrimaryAssetId.IsValid())
	{
		UAssetManager::Get().UnloadPrimaryAsset(AppearancePrimaryAssetId);
		AppearancePrimaryAssetId = FPrimaryAssetId();
	}

	if (AnimInstanceStreamableHandle.IsValid())
	{
		AnimInstanceStreamableHandle->ReleaseHandle();
		AnimInstanceStreamableHandle.Reset();
	}

	Super::EndPlay(EndPlayReason);
}

void UMWAppearanceComponent::OnDefaultAppearanceLoaded(FPrimaryAssetId LoadedAssetId)
{
	UMWCharacterAppearanceData* appearanceData = Cast<UMWCharacterAppearanceData>(
		UAssetManager::Get().GetPrimaryAssetObject(LoadedAssetId)
	);

	if (!appearanceData)
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

	visualMesh->SetSkeletalMesh(appearanceData->Mesh_Body);
}

void UMWAppearanceComponent::OnAnimInstanceClassLoaded()
{
	if (!AnimInstanceStreamableHandle.IsValid())
	{
		return;
	}

	UClass* animClass = Cast<UClass>(AnimInstanceStreamableHandle->GetLoadedAsset());
	if (!animClass || !animClass->IsChildOf<UMWCharacterAnimInstance>())
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

	visualMesh->SetAnimInstanceClass(animClass);
}