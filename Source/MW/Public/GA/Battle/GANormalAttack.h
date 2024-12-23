#pragma once

#include "CoreMinimal.h"
#include "GA/MWGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "GANormalAttack.generated.h"

class UAnimSequence;

USTRUCT(BlueprintType)
struct FNormalAttackData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequence* Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Cooldown;
};

UCLASS(Abstract, Blueprintable, ClassGroup = GA, meta = (Category = "Battle"))
class UNormalAttackDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FNormalAttackData> DataList;
};

UCLASS()
class MW_API UGANormalAttack : public UMWGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnEndAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditAnywhere, Category="Data")
	TSubclassOf<UPrimaryDataAsset> Data;
};