#pragma once

#include "CoreMinimal.h"
#include "GA/MWGameplayAbility.h"
#include "GAAttackProcess.generated.h"

USTRUCT(BlueprintType)
struct FAttackAnim
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimSequenceBase* Anim;

	UPROPERTY(EditAnywhere, meta=(ClampMin=0.0, ClampMax=1.0))
	float ComboableRange;

	UPROPERTY(EditAnywhere, meta=(ClampMin=0.0, ClampMax=1.0))
	float PerfectComboRange;
};

UCLASS()
class MW_API UGAAttackProcess : public UMWGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FAttackAnim AnimAsset;
	// normal attack asset
	// skill attack asset

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnEndAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};