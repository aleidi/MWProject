#pragma once

// ヘッダーをインクルード
#include "Interface/MWManagerInterface.h"
#include "UObject/NoExportTypes.h"
#include "MWGameConfigManager.generated.h"

// 前方宣言

// 定義

/*
 * @class UMWGameConfigManager
 * 
 * @brief 
 *
 * @note
 */
UCLASS()
class UMWGameConfigManager : public UObject, public IMWManagerInterface
{
	GENERATED_BODY()
	
public:
	UMWGameConfigManager();

	virtual void Initialize() override {}
	virtual void Deinitialize() override {}

private:
	void LoaddAllConfigs();

	bool LoadCharacterConfig();

public:
	//TMap<int32, FMWCharacterConfig> CharacterConfigs;

private:
	FString ConfigPath = TEXT("");
};