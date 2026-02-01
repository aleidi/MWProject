#pragma once

// Include Header
#include "Animation/Notify/MWAnimNotify.h"
#include "Data/GameplayAbility/ChargeableSkillData.h"
#include "AN_InterruptibleWindowCheck.generated.h"

// Forward Declare

// Define

/*
 * @class UAN_InterruptibleWindowCheck
 * 
 * @brief Check input charge stage and set comboable gameplay tag on ability system component.
 *
 * @note This notify checks if the character has a specific charge stage tag,
 *       and if so, removes the uninterruptible tag to allow combo attacks.
 */
UCLASS()
class UAN_InterruptibleWindowCheck : public UMWAnimNotify
{
	GENERATED_BODY()
	
public:
	//UAN_InterruptibleWindowCheck();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo Window")
	TArray<EMWInputChargeStage> InputChargeStageToCheck;

private:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};