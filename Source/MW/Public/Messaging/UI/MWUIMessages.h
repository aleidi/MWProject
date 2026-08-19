#pragma once

#include "GameplayTagContainer.h"
#include "MWUIMessages.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FMWCharacterStatusData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FText PlayerName;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentHealth = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxHealth = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 Level = 1;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTexture2D> Portrait = nullptr;
};

USTRUCT(BlueprintType)
struct FMWCharacterStatusMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 CharacterId = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite)
	FMWCharacterStatusData MainCharacter;

	UPROPERTY(BlueprintReadWrite)
	FMWCharacterStatusData SubCharacter;
};

USTRUCT(BlueprintType)
struct FMWSkillChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bLeftBar = true;

	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	FText SkillName;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTexture2D> SkillIcon = nullptr;

	UPROPERTY(BlueprintReadWrite)
	int32 RemainUse = 0;

	UPROPERTY(BlueprintReadWrite)
	float CurrentRecoveryAmount = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float MaxRecoveryAmount = 0.0f;
};

USTRUCT(BlueprintType)
struct FMWSkillEquippedMessage
{
	GENERATED_BODY()

	// 0=上、1=左、2=下、3=右
	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 SkillId = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct FMWSkillUsedMessage
{
	GENERATED_BODY()

	// 0=上、1=左、2=下、3=右
	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex = 0;

	// 任意。有効な場合はスロットのバインド確認に使用できます。
	UPROPERTY(BlueprintReadWrite)
	int32 SkillId = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite)
	int32 RemainUse = 0;

	UPROPERTY(BlueprintReadWrite)
	float CurrentRecoveryAmount = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float MaxRecoveryAmount = 0.0f;
};
