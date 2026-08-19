#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "GenericTeamAgentInterface.h"
#include "ModularCombatCharacterBase.generated.h"

// 前方宣言
class UCombatRelationComponent;

 /*
 * @class AModularCombatCharacterBase
 *
 * @brief CombatRelationシステムの使用例を示すキャラクタークラス。
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
