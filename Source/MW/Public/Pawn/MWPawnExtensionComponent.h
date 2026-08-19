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
	/** PawnがAbilitySystemのAvatarActorになった際に通知するDelegate。 */
	FSimpleMulticastDelegate OnAbilitySystemInitialized;

	/** PawnがAbilitySystemのAvatarActorから解除された際に通知するDelegate。 */
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;

protected:
	UPROPERTY(Transient)
	TObjectPtr<UMWAbilitySystemComponent> AbilitySystemComponent = nullptr;
#pragma endregion
};