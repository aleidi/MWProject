// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/AssetManager.h"
#include "Templates/SubclassOf.h"
#include "MWAssetManager.generated.h"

class UPrimaryDataAsset;
class UMWGameplayData;
class UMWGameplayData;

struct FMWBundles
{
	static const FName Equipped;
};


/**
 * UMWAssetManager
 *
 * 機能を拡張し、ゲーム固有型を保持するAssetManager実装です。
 * ゲーム固有のロード処理を集約するため、AssetManagerを継承して使用します。
 * DefaultEngine.iniの'AssetManagerClassName'で本クラスを指定します。
 */
UCLASS(Config = Game)
class UMWAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	UMWAssetManager();

	// AssetManagerのSingletonを返します。
	static UMWAssetManager& Get();

	// TSoftObjectPtrが参照するAssetを返します。未ロードの場合は同期ロードします。
	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// TSoftClassPtrが参照する派生クラスを返します。未ロードの場合は同期ロードします。
	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// AssetManagerがロードおよび追跡中の全AssetをLogへ出力します。
	static void DumpLoadedAssets();

protected:
	template <typename GameDataClass>
	const GameDataClass& GetOrLoadTypedGameData(const TSoftObjectPtr<GameDataClass>& DataPath)
	{
		if (TObjectPtr<UPrimaryDataAsset> const * pResult = GameDataMap.Find(GameDataClass::StaticClass()))
		{
			return *CastChecked<GameDataClass>(*pResult);
		}

		// 必要に応じてBlocking Loadします。
		return *CastChecked<const GameDataClass>(LoadGameDataOfClass(GameDataClass::StaticClass(), DataPath, GameDataClass::StaticClass()->GetFName()));
	}

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);
	static bool ShouldLogAssetLoads();

	// ロード済みAssetをメモリに保持する一覧へThread Safeに追加します。
	void AddLoadedAsset(const UObject* Asset);

	UPrimaryDataAsset* LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType);

	virtual void StartInitialLoading() override;
#if WITH_EDITOR
	virtual void PreBeginPIE(bool bStartSimulate) override;
#endif

protected:
	// ロード済みGameData
	UPROPERTY(Transient)
	TMap<TObjectPtr<UClass>, TObjectPtr<UPrimaryDataAsset>> GameDataMap;

	// ゲームのMasterData。ゲーム開始時にロードします。
	TSharedPtr<FStreamableHandle> GameplayDataHandle = nullptr;

private:
	// AssetManagerがロードおよび追跡中のAsset。
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	// ロード済みAsset一覧の変更時に使用するScopeLock。
	FCriticalSection LoadedAssetsCritical;
};


template<typename AssetType>
AssetType* UMWAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// ロード済みAsset一覧へ追加します。
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template<typename AssetType>
TSubclassOf<AssetType> UMWAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			// ロード済みAsset一覧へ追加します。
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}
