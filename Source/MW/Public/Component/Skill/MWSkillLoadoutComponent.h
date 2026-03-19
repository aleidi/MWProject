#pragma once

// Include Header
#include "Pawn/MWPawnComponent.h"
#include "Component/Skill/MWSkillLoadoutTypes.h"
#include "MWSkillLoadoutComponent.generated.h"

// Forward Declare

// Define

/*
 * @class UMWSkillLoadoutComponent
 * 
 * @brief 
 *
 * @note
 */
UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class UMWSkillLoadoutComponent : public UMWPawnComponent
{
	GENERATED_BODY()
	
public:
	UMWSkillLoadoutComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	bool SetSkillInSlot(EMWSkillLoadoutSlot Slot, int32 SkillId);

private:
	void Init();

private:
	UPROPERTY()
	TArray<FMWSkillSlot> SkillSlots;
};