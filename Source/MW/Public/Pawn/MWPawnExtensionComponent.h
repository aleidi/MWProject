#pragma once

#include "Pawn/MWPawnComponent.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "MWPawnExtensionComponent.generated.h"

class UDataAsset;
class UMWAbilitySystemComponent;
class FMWTargetSelector;
struct FMWAbilitySet_GrantedHandles;
struct FMWFoundActorInfo;

/* UMWPawnExtensionComponent
* 
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

#pragma region Ability
public:
	void InitializeAbilitySystem(UMWAbilitySystemComponent* InASC, AActor* OnwnerActor);
	void UninitializeAbilitySystem();

public:
	/** Delegate fired when our pawn becomes the ability system's avatar actor */
	FSimpleMulticastDelegate OnAbilitySystemInitialized;

	/** Delegate fired when our pawn is removed as the ability system's avatar actor */
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;

protected:
	UPROPERTY(Transient)
	TObjectPtr<UMWAbilitySystemComponent> AbilitySystemComponent = nullptr;
#pragma endregion

public:
	int32 GetPawnDataId() const;

	void SetPawnDataId(int32 NewId);

protected:
	UPROPERTY()
	int32 PawnDataId = INDEX_NONE;
};