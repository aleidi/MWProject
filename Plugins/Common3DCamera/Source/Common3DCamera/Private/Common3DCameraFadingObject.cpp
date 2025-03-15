// Copyright 2021 Alexander Shumeyko. All Rights Reserved.

#include "Common3DCameraFadingObject.h"

#include "Common3DCameraComponent.h"
#include "Common3DCameraModeDataAsset.h"
#include "Algo/ForEach.h"
#include "Components/MeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

FC3DFadingActorInfo::FC3DFadingActorInfo(AActor* InActor, bool bInFadeOut, float InFadeOutTime, float InFadeValue)
    : Actor(InActor)
    , bFadeOut(bInFadeOut)
    , FadeOutTime(InFadeOutTime)
    , FadeValue(InFadeValue)
{
}

bool FC3DFadingActorInfo::operator==(const FC3DFadingActorInfo& other) const
{
	return Actor == other.Actor;
}

void UC3DCameraFadingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	static FName fade = FName("Fade");
	if (!MaterialFadeParamNames.Contains(fade))	MaterialFadeParamNames.Emplace(FName("Fade"));
	
	//获取当前的cameramode
	if (bEnableFade)
	{
		//自我Fade
		if (bFadeSelfIfCollision)
		{
			SelfFade();
		}

		if (MaterialFadeParamNames.Num() != 0)
		{
			TArray<AActor*> newFadingActors = FilterCollidedActors(FindCollidedActors());
			UpdateFadingActorsList(newFadingActors);
			ApplyFade(DeltaTime);
		}
	}
	//If after disable fading left proccessed actors
	else if (!bEnableFade)
	{
		UpdateFadingActorsList({});
		ApplyFade(DeltaTime);
	}
}

void UC3DCameraFadingObject::Validate(bool bWithInterpolation)
{
	Super::Validate(bWithInterpolation);
}

void UC3DCameraFadingObject::OnEnterCameraMode(bool bWithInterpolation)
{
	Super::OnEnterCameraMode(bWithInterpolation);

	if (!bEnableFade)
	{
		for (auto& fadingActor : FadingActors)
		{
			fadingActor.bFadeOut = false;
			if (!bWithInterpolation)
			{
				fadingActor.FadeValue = MaterialFadeMaxValue;
			}
		}
	}

	// 将owner上附加的actor从ManualFadeActor中移除
	if (!bSelfFadeAttachedActors)
	{
		TArray<AActor*> attachedToOwnerActors;
		GetOwningActor()->GetAttachedActors(attachedToOwnerActors);
		for (auto actor : attachedToOwnerActors)
		{
			RemoveManualFadeActor(actor);
		}
	}

	// 
	if (!bWithInterpolation)
	{
		ApplyFade(0.f);
		UpdateFadingActorsList({});
	}
}

void UC3DCameraFadingObject::AddIgnoredActor(AActor* Actor)
{
	if (Actor == nullptr)
	{
		return;
	}

	IgnoredActors.Add(Actor);

	FC3DFadingActorInfo* fadingActorInfo = FadingActors.FindByPredicate([Actor](const FC3DFadingActorInfo& FadingActorInfo) {
		return FadingActorInfo.Actor.Get() == Actor;
	});
	if (fadingActorInfo != nullptr)
	{
		fadingActorInfo->bFadeOut = false;
	}
}

void UC3DCameraFadingObject::RemoveIgnoredActor(AActor* Actor)
{
	IgnoredActors.Remove(Actor);
}

void UC3DCameraFadingObject::ClearIgnoredActors()
{
	IgnoredActors.Empty();
}

void UC3DCameraFadingObject::AddManualFadeActor(AActor* Actor, bool bWithCustomTime /* = false*/, float CustomFadeOutTime /* = 0.f*/)
{
	if (Actor == nullptr)
	{
		return;
	}

	ManualFadingActors.Add(Actor);

	float fadeOutTime = bWithCustomTime ? CustomFadeOutTime : FadeOutTime;

	FC3DFadingActorInfo* fadingActorInfo = FadingActors.FindByPredicate([Actor](const FC3DFadingActorInfo& FadingActorInfo) {
		return FadingActorInfo.Actor.Get() == Actor;
	});

	if (fadingActorInfo != nullptr)
	{
		fadingActorInfo->FadeOutTime = fadeOutTime;
	}
	else
	{
		FadingActors.Add(FC3DFadingActorInfo(Actor, true, fadeOutTime));
	}
}

void UC3DCameraFadingObject::RemoveManualFadeActor(AActor* Actor)
{
	ManualFadingActors.Remove(Actor);
}

void UC3DCameraFadingObject::ClearManualFadeActors()
{
	ManualFadingActors.Empty();
}

void UC3DCameraFadingObject::SelfFade()
{
	auto& camera = GetCamera();

	TArray<FOverlapResult> overlapResults;
    //通过球形碰撞检测自身actor和其依附的对象
	FCollisionShape collisionShape;
	collisionShape.SetSphere(SelfFadeCheckRadius);

	GetWorld()->OverlapMultiByChannel(overlapResults, camera.GetCameraLocation(), FQuat::Identity, FadeChannel, collisionShape);

	AActor* owningActor = GetOwningActor();

	auto owningActorOverlapRes = overlapResults.FindByPredicate([owningActor](auto& OverlapRes) {
		return OverlapRes.GetActor() == owningActor;
	});

	TArray<AActor*> attachedToOwnerActors;
	GetOwningActor()->GetAttachedActors(attachedToOwnerActors);

	if (owningActorOverlapRes != nullptr)
	{
		AddManualFadeActor(owningActor, bUseCustomFadeOutTimeForSelfFade, SelfFadeCustomFadeOutTime);

		if (bSelfFadeAttachedActors)
		{
			for (auto actor : attachedToOwnerActors)
			{
				AddManualFadeActor(actor, bUseCustomFadeOutTimeForSelfFade, SelfFadeCustomFadeOutTime);
			}
		}
	}
	else
	{
		RemoveManualFadeActor(owningActor);

		if (bSelfFadeAttachedActors)
		{
			for (auto actor : attachedToOwnerActors)
			{
				RemoveManualFadeActor(actor);
			}
		}
	}
}

TArray<FHitResult> UC3DCameraFadingObject::FindCollidedActors() const
{
	auto& camera = GetCamera();

	ETraceTypeQuery traceType = UCollisionProfile::Get()->ConvertToTraceType(FadeChannel);

	FVector traceStart = GetOwningActor()->GetActorLocation();
	FVector traceEnd = camera.GetCameraLocation();

	float actorCollisionHalfHeight = GetOwningActor()->GetSimpleCollisionHalfHeight();
	float actorCollisionRadius = GetOwningActor()->GetSimpleCollisionRadius();
	FVector boxHalfSize(0.f, actorCollisionRadius * 0.7f, actorCollisionHalfHeight * 0.7f * (1 - FMath::Abs(camera.GetCameraRotation().Pitch) / 90.f));

	TArray<AActor*> ignoreActors = { GetOwningActor() };

	EDrawDebugTrace::Type drawDebugTrace = camera.DebugRules.bEnableDrawFadeShapeDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	TArray<FHitResult> hitResults;
	UKismetSystemLibrary::BoxTraceMulti(GetOwningActor(), traceStart, traceEnd, boxHalfSize, FRotationMatrix::MakeFromX(camera.GetCameraLocation() - GetOwningActor()->GetActorLocation()).Rotator(), traceType, false, ignoreActors, drawDebugTrace, hitResults, true);

	return hitResults;
}

TArray<AActor*> UC3DCameraFadingObject::FilterCollidedActors(const TArray<FHitResult>& HitResults)
{
	TArray<AActor*> collidedActors;
	for (auto& hitResult : HitResults)
	{
		if (!hitResult.bBlockingHit && hitResult.GetActor())
		{
			collidedActors.Add(hitResult.GetActor());
		}
	}

	return collidedActors;
}

void UC3DCameraFadingObject::UpdateFadingActorsList(TArray<AActor*> RelevantActors)
{
	FadingActors.RemoveAll([this](const FC3DFadingActorInfo& elem) {
		return !elem.Actor.IsValid() || (!elem.bFadeOut && elem.FadeValue + KINDA_SMALL_NUMBER >= MaterialFadeMaxValue);
	});

	RelevantActors.RemoveAll([this](AActor* actor) {
		return IgnoredActors.Contains(actor);
	});

	for (auto& actor : ManualFadingActors)
	{
		RelevantActors.AddUnique(actor.Get());
	}

	// find already fading actors
	for (auto& fadingActor : FadingActors)
	{
		AActor** findedActor = RelevantActors.FindByPredicate([fadingActor](const AActor* actor) {
			return fadingActor.Actor.Get() == actor;
		});

		if (findedActor != nullptr)
		{
			fadingActor.bFadeOut = true;
		}
		else
		{
			fadingActor.bFadeOut = false;
		}
	}

	// add new actors
	for (auto& actor : RelevantActors)
	{
		FadingActors.AddUnique(FC3DFadingActorInfo(actor, true, FadeOutTime));
	}
}

void UC3DCameraFadingObject::ApplyFade(float DeltaTime)
{
	for (auto& fadedActor : FadingActors)
	{
		if (fadedActor.Actor.IsValid())
		{
			float fadeSpeed = fadedActor.bFadeOut ? -1.f * fadedActor.FadeOutTime : FadeInTime;
			fadedActor.FadeValue += 1.f / fadeSpeed * DeltaTime; // fade time

			fadedActor.FadeValue = FMath::Clamp(fadedActor.FadeValue, MaterialFadeMinValue, MaterialFadeMaxValue);

			TArray<UMeshComponent*> meshes;
			fadedActor.Actor->GetComponents(meshes);
		
			for (auto& mesh : meshes)
			{
				// get all the materials of the mesh
				const TArray<UMaterialInterface*> materialInterfaces = mesh->GetMaterials(); 
				for (auto& materialInterface : materialInterfaces)
				{
					if(materialInterface != nullptr)
					{
						TArray<FMaterialParameterInfo> materialScalarParameters;
						TArray<FGuid> materialScalarParametersIds;
						materialInterface->GetAllScalarParameterInfo(materialScalarParameters, materialScalarParametersIds);

						for (auto& materialParameterInfo : materialScalarParameters)
						{
							if (MaterialFadeParamNames.Contains(materialParameterInfo.Name))
							{
								if (const auto widgetComponent = Cast<UWidgetComponent>(mesh))
								{
									if (const auto widgetMatInstance = widgetComponent->GetMaterialInstance())
									{
										widgetMatInstance->SetScalarParameterValue(materialParameterInfo.Name, fadedActor.FadeValue);
									}
								}
								else
								{
									mesh->SetScalarParameterValueOnMaterials(materialParameterInfo.Name, fadedActor.FadeValue);
								}
							}
						}
					}
				}
			}
		}
	}
}

void UC3DCameraFadingObject::InitPropertyFromDataAsset()
{
	const auto& camera = GetCamera();
	const auto& modeSettings = camera.GetCurrentCameraMode()->ModeSettings;
	const auto& fadeSettings = modeSettings.FadeSettings;

	bEnableFade = modeSettings.bEnableFadeSettings;

	FadeInTime = fadeSettings.FadeInTime;
	FadeOutTime = fadeSettings.FadeOutTime;

	MaterialFadeMinValue = fadeSettings.MaterialFadeMinValue;
	MaterialFadeMaxValue = fadeSettings.MaterialFadeMaxValue;

	MaterialFadeParamNames = fadeSettings.MaterialFadeParamNames;

	FadeChannel = fadeSettings.FadeChannel;

	bFadeSelfIfCollision = fadeSettings.bFadeSelfIfCollision;

	bUseCustomFadeOutTimeForSelfFade = fadeSettings.bUseCustomFadeOutTimeForSelfFade;

	SelfFadeCustomFadeOutTime = fadeSettings.SelfFadeCustomFadeOutTime;

	SelfFadeCheckRadius = fadeSettings.SelfFadeCheckRadius;

	bSelfFadeAttachedActors = fadeSettings.bSelfFadeAttachedActors;
}
