#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "GenericTeamAgentInterface.h"
#include "ModularCombatCharacterBase.generated.h"

// Forward Declare
class UCombatRelationComponent;

 /*
 * @class AModularCombatCharacterBase
 *
 * @brief This is an example character class that demonstrates the use of the combat relation system.
 *
 * @note
 */
UCLASS()
class COMBATRELATION_API AModularCombatCharacterBase : public AModularCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AModularCombatCharacterBase(const FObjectInitializer& Obj);

	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

};
