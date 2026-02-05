#pragma once

// Include Header
#include "Animation/Notify/MWAnimNotify.h"
#include "GameplayTagContainer.h"
#include "AN_DamageCheck.generated.h"

// Forward Declare

// Define

/*
 * @class UAN_DamageCheck
 * 
 * @brief Animation Notify for triggering damage calculation at specific timestamps in skill animations
 *
 * @note Used in chargeable skill system to apply damage to previously captured targets
 */
UCLASS()
class UAN_DamageCheck : public UMWAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

protected:
	/** Damage event identifier tag (e.g., "Damage.Skill.Slash.First") */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (Categories = "Damage"))
	FGameplayTag DamageEventTag;

	/** Damage multiplier for this specific hit timing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DamageMultiplier = 1.0f;

	/** Hit index for multi-hit combo sequences (0 = first hit, 1 = second hit, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0", UIMin = "0"))
	int32 HitIndex = 0;
};