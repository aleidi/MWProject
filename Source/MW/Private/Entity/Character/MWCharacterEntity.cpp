#include "Entity/Character/MWCharacterEntity.h"
#include "Character/MWCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Data/MWMasterData.h"
#include "Define/MWDefineGameplay.h"
#include "MWGameSingleton.h"

void UMWCharacterEntity::Initialize()
{
	Super::Initialize();

	CharacterClass = AMWCharacter::StaticClass();
}

void UMWCharacterEntity::Uninitialize()
{
	Super::Uninitialize();
}

UClass* UMWCharacterEntity::GetActorClass() const
{
	return CharacterClass;
}

void UMWCharacterEntity::OnActorLoaded(AActor* InLoadedActor)
{
	Super::OnActorLoaded(InLoadedActor);

	AMWCharacter* character = Cast<AMWCharacter>(InLoadedActor);
	if (!character)
	{
		checkf(character != nullptr, TEXT("Loaded actor is not AMWCharacter class"));

		return;
	}

	character->SetOwnerEntity(this);
	character->SetCharacterLocation(Location);

	const UMWMasterData* data = MWSINGLETON->GetMasterData();
	auto* mesh = character->GetDummyMesh();
	if (mesh)
	{
		mesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

		if (data->DummyMesh)
		{
			mesh->SetSkeletalMesh(data->DummyMesh);
			mesh->SetWorldLocation(GetFloorLocation());
		}

		if (data->DummyAnimClass)
		{
			mesh->SetAnimInstanceClass(data->DummyAnimClass);
		}
	}

	if (!ResourceDataPtr)
	{
		return;
	}

	character->GetVisualMesh()->SetSkeletalMesh(ResourceDataPtr->VisualMesh);
	character->GetVisualMesh()->SetAnimInstanceClass(ResourceDataPtr->VisualAnimInst);

	// Set right mesh location
	// 正しいメッシュの位置を設定
	if (ResourceDataPtr->Height > 0.f)
	{
		character->GetCapsuleComponent()->SetCapsuleHalfHeight(ResourceDataPtr->Height * 0.5f);
	}
}

void UMWCharacterEntity::SetCharacterData(const FMWCharacterResourceData* InData)
{
	if(InData == nullptr)
	{
		return;
	}

	ResourceDataPtr = InData;

	CharacterClass = InData->CharacterClass ? InData->CharacterClass : CharacterClass;
}

AMWCharacter* UMWCharacterEntity::GetCharacterActor()
{
	return Cast<AMWCharacter>(GetActor());
}

const AMWCharacter* UMWCharacterEntity::GetCharacterActor() const
{
	return Cast<const AMWCharacter>(GetActor());
}

FVector UMWCharacterEntity::GetLocation() const
{
	return GetCharacterActor() ? GetCharacterActor()->GetFloorLocation() : Location;
}
