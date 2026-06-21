#include "Character/MWAppearanceComponent.h"

#include "Character/MWCharacter.h"
#include "Character/MWCharacterAnimInstance.h"
#include "Data/Character/MWCharacterAsset.h"

void UMWAppearanceComponent::BeginPlay()
{
	Super::BeginPlay();

	AMWCharacter* character = GetPawn<AMWCharacter>();
	if (!character)
	{
		return;
	}

	CharacterAssetReadyHandle = character->OnCharacterAssetReady().AddUObject(
		this,
		&UMWAppearanceComponent::HandleCharacterAssetReady);

	if (UMWCharacterAsset* loaded = character->GetCharacterAsset())
	{
		ApplyAppearance(loaded);
	}
}

void UMWAppearanceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AMWCharacter* character = GetPawn<AMWCharacter>())
	{
		if (CharacterAssetReadyHandle.IsValid())
		{
			character->OnCharacterAssetReady().Remove(CharacterAssetReadyHandle);
			CharacterAssetReadyHandle.Reset();
		}
	}

	Super::EndPlay(EndPlayReason);
}

void UMWAppearanceComponent::HandleCharacterAssetReady(UMWCharacterAsset* Asset)
{
	ApplyAppearance(Asset);
}

void UMWAppearanceComponent::ApplyAppearance(UMWCharacterAsset* Asset)
{
	if (!Asset)
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

	if (USkeletalMesh* mesh = Asset->DefaultAppearance.Mesh_Body.Get())
	{
		visualMesh->SetSkeletalMesh(mesh);
	}

	if (UClass* animClass = Asset->AnimInstance.Get())
	{
		visualMesh->SetAnimInstanceClass(animClass);
	}
}