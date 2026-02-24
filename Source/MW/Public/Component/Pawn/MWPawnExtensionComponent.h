#pragma once

#include "Component/Pawn/MWPawnComponent.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "MWPawnExtensionComponent.generated.h"

class UMWPawnData;
class UMWAbilitySystemComponent;
class FMWTargetSelector;
struct FMWAbilitySet_GrantedHandles;
struct FMWFoundActorInfo;

/* UMWPawnExtensionComponent
* 
* This class is used to extend the functionality of character so that character class can keep simple.
* eg. various kinds of character data, ability system, battle function...
*/
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class UMWPawnExtensionComponent : public UMWPawnComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintPure, Category = "Pawn|Extension")
	static UMWPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor);

#pragma region PawnSetting
protected:
	// CharacterId used to get character data from data table.
	// Id is read when BeginPlay from character's owner entity's resource data.
	// CharacterIdはデータテーブルからキャラクターデータを取得するために使用される。
	// IdはBeginPlay時にキャラクターの所有エンティティのリソースデータから読み取られる。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting")
	int32 CharacterId;
#pragma endregion

#pragma region Ability
public:
	void InitializeAbilitySystem(UMWAbilitySystemComponent* InASC, AActor* OnwnerActor);
	void UninitializeAbilitySystem();

protected:
	void GiveAbility();

public:
	/** Delegate fired when our pawn becomes the ability system's avatar actor */
	FSimpleMulticastDelegate OnAbilitySystemInitialized;

	/** Delegate fired when our pawn is removed as the ability system's avatar actor */
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;
protected:
	UPROPERTY()
	TObjectPtr<UMWAbilitySystemComponent> AbilitySystemComponent;

	/* Cache the granted abilities */
	TSharedPtr<FMWAbilitySet_GrantedHandles> AbilityGranetedHandles;
#pragma endregion

#pragma region Select
public:
	void InitializeTargetSelector(const AController* InControler);

	void SwitchToLeftTarget();
	void SwitchToRightTarget();
	void CancelSelect();
	void LockTarget();
	void UnlockTarget();
	void ForceLockIfNoTarget(const FMWFoundActorInfo& Target);
	void OnTargetNotExisted(const FMWFoundActorInfo& Target);
protected:
	TSharedPtr<FMWTargetSelector> TargetSelector;
#pragma endregion

#pragma region Battle
public:
	/* @param Tag		: used to find the skill ability and activate.
	*  @param Payload	: extra information to pass
	*/
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void CastSkill(const FGameplayTag& Tag, FGameplayEventData Payload);

	/* Add a force to move pawn a certain distance. */
	UFUNCTION(BlueprintCallable, Category = "Battle|Combat")
	void AddForce(const FVector& Dir, float Intensity);

	/* This function will change the movement mode to get the pawn unaffected by gravity. */
	UFUNCTION(BlueprintCallable, Category = "Battle|Combat")
	void AffectedByGravity(bool bAffect);
#pragma endregion
};