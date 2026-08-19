#pragma once

// ヘッダーをインクルード
#include "Define/MWDefineGameplay.h"
#include "GameFramework/SaveGame.h"
#include "MWSaveGame.generated.h"

// 前方宣言

// 定義
USTRUCT(BlueprintType)
struct FMWSaveMetaData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	int32 SaveId = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	FString Slot = TEXT("");

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	int32 SaveVersion = 0;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	float PlayTime = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	FDateTime SaveDateTime;
};

USTRUCT(BlueprintType)
struct FMWCharacterSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	int32 CharacterId = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	int32 Level = 0;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	int32 Experience = 0;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	TArray<int32> LearnedSkills;

	// 装備中スキルと対応するスロットIndex
	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	TMap<int32, int32> EquippedSkills;
};

USTRUCT(BlueprintType)
struct FMWPartyData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	int32 LeaderCharacterId = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	TArray<FMWCharacterSaveData> PartyMembers;
};

/*
 * @class UMWSaveGame
 * 
 * @brief 
 *
 * @note
 */
UCLASS()
class UMWSaveGame : public USaveGame
{
	GENERATED_BODY()
	
	// セーブデータ処理
public:
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void ChangeData();

public:
	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	FMWSaveMetaData MetaData;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	FMWPartyData PartyData;

	// クエスト
	// マップ地域
	// インベントリ
};