#pragma once

// Include Header
#include "GenericTeamAgentInterface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CombatRelationLibrary.generated.h"

// Forward Declare
class UCombatRelationComponent;

// Define

/*
 * @class UCombatRelationLibrary
 * 
 * @brief 
 *
 * @note
 */
UCLASS()
class COMBATRELATION_API UCombatRelationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UCombatRelationComponent* FindRelationComponent(AActor* Other);

	static const UCombatRelationComponent* FindRelationComponent(const AActor* Other);

	static ETeamAttitude::Type GetTeamAttitudeTowards(const AActor* Other);
};