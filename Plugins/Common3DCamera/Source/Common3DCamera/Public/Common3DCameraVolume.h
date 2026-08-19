#pragma once

#include "GameFramework/Volume.h"
#include "GameplayTagContainer.h"

#include "Common3DCameraVolume.generated.h"

UCLASS(Blueprintable)
class COMMON3DCAMERA_API AC3DCameraVolume : public AVolume
{
	GENERATED_BODY()
public:
	AC3DCameraVolume(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FGameplayTag CameraModeTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bChangeCameraModeWithInterp = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (InlineEditConditionToggle))
	bool bCheckFilterActorClassWithCamera = false;
	// 例：CharacterまたはPlayerActor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "bCheckFilterActorClassWithCamera"))
	TArray<TSubclassOf<AActor>> FilterActorClassesWithCamera;

protected:
	UFUNCTION()
	void OnVolumeBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnVolumeEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	virtual class UC3DCameraComponent* GetCameraFromActor(AActor* Actor);
};
