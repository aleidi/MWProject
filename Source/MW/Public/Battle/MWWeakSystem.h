#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "MWWeakSystem.generated.h"

UENUM(BlueprintType)
enum class EWeakState : uint8
{
	Normal,
	Weak,
	Restore,
};

USTRUCT(BlueprintType)
struct MW_API FWeakTriggerInfo
{
	GENERATED_BODY()

	// 弱点是不是被triiger了？
	// 是不是最后一个？
	// 持续多久？
	// 被触发的weaktag

	UPROPERTY()
	int32 Times;

	UPROPERTY()
	bool bLast;

	UPROPERTY()
	int32 Duration;
};

/* GameplayTag Format:
* ----------------------------
*  Battle.Weak.XXX.Trigger
* ----------------------------
*  XXX is the name of the weak
*  XXX used to check which weak is applied to character
*/
USTRUCT(BlueprintType)
struct MW_API FWeakPoint
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	FName DisplayName;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly)
	FName Name;

	/* Used to check if the weak can be trigger. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag TriggerTag;

	/* how long to keep weak state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Duration;

	/* The value is in range [0,100]. When it's 100, the weak is exposed. */
	UPROPERTY(BlueprintReadOnly)
	int32 Exposure;

	bool TryTrigger(int32 Value);

	FWeakPoint();
};

UCLASS(BlueprintType, Blueprintable)
class MW_API UWeakPointCombo : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FWeakPoint> Combo;

	/* The order of the combo now. */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Current = 0;

public:
	/* Trigger the weak combo. */
	bool TryTrigger(FGameplayTag WeakTag, int32 Value, TFunction<void(FWeakTriggerInfo&)> TriggeredCallback);

	FORCEINLINE int32 GetComboNum() const { return Combo.Num(); }

	FORCEINLINE int32 GetCurrentCombo() const { return Current; }

	FORCEINLINE bool IsValid() const { return GetComboNum() > 0; }
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MW_API UMWWeakSystem : public UObject
{
	GENERATED_BODY()

public:
	UMWWeakSystem();

	/* Set the owner of the weak system. Should be done before use.*/
	UFUNCTION(BlueprintCallable, Category = "WeakSystem")
	void SetOwner(UObject* NewOwner);

	UFUNCTION(BlueprintCallable, Category = "WeakSystem")
	bool CheckAndTriggerWeak(const FGameplayTag& NewTag, int32 Value);

	UFUNCTION(BlueprintPure, Category = "WeakSystem")
	const TArray<UWeakPointCombo*>& GetWeakPointCombos() const;

private:
	UFUNCTION()
	void TryTriggerWeak(UObject* InOwner, const FGameplayTag& NewTag, int32 Value);

	UFUNCTION()
	void OnWeakRemoved(UObject* InOwner, const FGameplayTag& RemovedTag);

	/*
	* Duration - how long weak is exposed
	*/
	UFUNCTION()
	void WeakenProcess(int32 Duration);

	UFUNCTION()
	void WeakRestore();

	UFUNCTION()
	void EnterImmune();

	UFUNCTION()
	void ExitImmune();

private:
	UPROPERTY()
	TArray<UWeakPointCombo*> WeakPointCombos;

	UPROPERTY()
	TObjectPtr<UObject> Owner;

	UPROPERTY()
	EWeakState WeakState;

	/* Save the combo used now, if owner is not in weak state, return -1. */
	UPROPERTY()
	int32 CurrentComboId = -1;

	/* The number of times the weak has been triggered */
	UPROPERTY()
	int32 TriggerTimes = 0;

	/* Be related to TriggerTimes */
	UPROPERTY()
	int32 CoolDown;

	UPROPERTY()
	FGameplayTag WeakenTag;

	UPROPERTY()
	FGameplayTag RestoreTag;

	/* If the weak combo is end, then can not be triggered. */
	UPROPERTY()
	bool bCanTrigger;

	UPROPERTY()
	FTimerHandle thWeakRestore;

	UPROPERTY()
	FTimerHandle thImmune;
};
