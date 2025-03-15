// Copyright 2021 Alexander Shumeyko. All Rights Reserved.

#include "Common3DCameraVolume.h"

#include "Common3DCameraComponent.h"
#include "Components/BrushComponent.h"

AC3DCameraVolume::AC3DCameraVolume(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
	GetBrushComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetBrushComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetBrushComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AC3DCameraVolume::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AC3DCameraVolume::OnVolumeBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AC3DCameraVolume::OnVolumeEndOverlap);
}

void AC3DCameraVolume::OnVolumeBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor != nullptr && (!bCheckFilterActorClassWithCamera || FilterActorClassesWithCamera.Find(OtherActor->GetClass())))
	{
		if (UC3DCameraComponent* cameraComponent = GetCameraFromActor(OtherActor))
		{
			cameraComponent->OnBeginOverlapCameraVolume(this);
		}
	}
}

void AC3DCameraVolume::OnVolumeEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor != nullptr && (!bCheckFilterActorClassWithCamera || FilterActorClassesWithCamera.Find(OtherActor->GetClass())))
	{
		if (UC3DCameraComponent* cameraComponent = GetCameraFromActor(OtherActor))
		{
			cameraComponent->OnEndOverlapCameraVolume(this);
		}
	}
}

UC3DCameraComponent* AC3DCameraVolume::GetCameraFromActor(AActor* Actor)
{
	return Actor->FindComponentByClass<UC3DCameraComponent>();
}