#include "Entity/MWEntity.h"
#include "MWLogChannels.h"

void UMWEntity::Initialize()
{

}

void UMWEntity::Uninitialize()
{
	UnloadActor();
}

UClass* UMWEntity::GetActorClass() const
{
	return nullptr;
}

void UMWEntity::OnActorLoaded(AActor* InLoadedActor)
{
	OwnActor = InLoadedActor;
}

void UMWEntity::UnloadActor()
{
	if (OwnActor.IsValid())
	{
		OwnActor->Destroy();

		OwnActor = nullptr;
	}
}

TObjectPtr<AActor> UMWEntity::LoadActor(bool bDeferConstruction)
{
	UClass* actorClass = GetActorClass();

	if (nullptr == actorClass)
	{
		UE_LOG(LogMWEntity, Warning, TEXT("Entity(%s) has not a actor class."), *GetName());

		return nullptr;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParams.bDeferConstruction = bDeferConstruction;

	UWorld* world = GetWorld();
//	if (world == nullptr)
//	{
//#if WITH_EDITOR
//		world = GEditor->GetEditorWorldContext().World();
//#endif
//	}

	if (!world) 
	{
		UE_LOG(LogMWEntity, Warning, TEXT("Entity(%s) is not in a world."), *GetName());

		return nullptr;
	}

	AActor* newActor = world->SpawnActor<AActor>(actorClass, Location, Rotation, spawnParams);

	if (newActor)
	{
		OnActorLoaded(newActor);
	}

	return newActor;
}

void UMWEntity::SetLoadedActor(AActor* InLoadedActor)
{
	if (InLoadedActor != nullptr)
	{
		OnActorLoaded(InLoadedActor);
	}
}

void UMWEntity::SetSpawnData(const FObjectId& Id, const FVector& InRoc, const FRotator& InRot)
{
	SetId(Id);
	SetLocation(InRoc);
	SetRotation(InRot);
}

void UMWEntity::SetLocation(const FVector& InLoc)
{
	Location = InLoc;

	OnLocationUpdated();
}

FVector UMWEntity::GetLocation() const
{
	return OwnActor.IsValid() ? OwnActor->GetActorLocation() : Location;
}

FVector UMWEntity::GetFloorLocation() const
{
	return GetLocation();
}

void UMWEntity::SetRotation(const FRotator& InRot)
{
	Rotation = InRot;

	OnRotationUpdated();
}

FRotator UMWEntity::GetRotation() const
{
	return OwnActor.IsValid() ? OwnActor->GetActorRotation() : Rotation;
}

void UMWEntity::OnLocationUpdated()
{
	if (OwnActor.IsValid())
	{
		OwnActor->SetActorLocation(Location);
	}
}

void UMWEntity::OnRotationUpdated()
{
	if (OwnActor.IsValid())
	{
		OwnActor->SetActorRotation(Rotation);
	}
}
