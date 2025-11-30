#pragma once

// Include Header
#include "Character/MWCharacter.h"
#include "MWBattleUnitAvatar.generated.h"

// Forward Declare
class UMWAbilitySet;
class UMWBattleUnitComponent;

// Define

/*
 * @class AMWBattleUnitAvatar
 * 
 * @brief Avatar actor for battle unit. It will not shown in battle, only used to carry necessary data.
 *        戦闘ユニットのアバター、戦闘に表示されず、必要なデータを保持するために使用される。
 *
 * @note
 */
UCLASS()
class MW_API AMWBattleUnitAvatar : public AMWCharacter
{
	GENERATED_BODY()
	
public:
	AMWBattleUnitAvatar(const FObjectInitializer& ObjectInitializer);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle")
	TObjectPtr<UMWBattleUnitComponent> BattleUnitComponent;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};