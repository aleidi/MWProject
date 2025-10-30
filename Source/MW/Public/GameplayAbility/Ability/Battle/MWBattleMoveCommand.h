#pragma once

// Include Header
#include "GameplayAbility/MWGameplayAbility.h"
#include "MWBattleMoveCommand.generated.h"

// Forward Declare

// Define

/*
 * @class UMWBattleMoveCommand
 * 
 * @brief This class is the base class to process command of battle unit.
 *		  This gameplay ability should be activated by the sent event.
 *
 * @note
 */
UCLASS()
class UMWBattleMoveCommand : public UMWGameplayAbility
{
	GENERATED_BODY()

public:
	UMWBattleMoveCommand(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override {}

protected:
	// The tag to activate this ability.
	// アビリティを発動させるタグ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle")
	FGameplayTag ActivateTag;

protected:
	// Ensure AbilityTriggers matches ActivateTag after properties are initialized
	// プロパティの初期化後にAbilityTriggersがActivateTagと一致することを確保する
	virtual void PostInitProperties() override;

#if WITH_EDITOR
	// Keep AbilityTriggers in sync when ActivateTag is changed in the editor
	// エディタでActivateTagが変更されたときにAbilityTriggersを同期させ続ける
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};