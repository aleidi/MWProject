#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MWDataAsset.generated.h"

enum class EMWSceneUIType : uint8;
enum class EMWWindowUIType : uint8;
enum class EMWPopupUIType;

#define DATAASSET (UMWDataAsset::Get())

USTRUCT()
struct FMWSoftPath
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TMap<FName, FSoftObjectPath> PathMap;
};

UCLASS(BlueprintType, Blueprintable)
class MW_API UMWDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	enum class EDataAssetType : int32
	{
		InValidType = -1,

		Widget = 0,
		Texture,
		Sprite,
		StaticMesh,
		SkeletalMesh,
		AnimSequence,
		AnimMontage,
		AnimationBP,
		DataTable,
		AnimSchemaAsset,
		AnimAsset,
		PropStaticMesh,
		PropSkeletalMesh,
		Map,				// Level, World
		WidgetStyle,		// Widget theme style
		Environment,		// Environment lighting, celestial bodies, weather
		DataAssetTypeMax,
	};

	static UMWDataAsset* Get();

	void Initialize();

	UClass* GetPathAndBaseClass(const UMWDataAsset::EDataAssetType& DataAssetType, FString& DataPath);

#if WITH_EDITOR
	//Create ObjectLibrary during cooking (packaging)
	void CreateLibrarys();	
	
	virtual void BeginCacheForCookedPlatformData(const ITargetPlatform* TargetPlatform) override;

	const FMWSoftPath* GetSelectedTypePathData(UMWDataAsset::EDataAssetType DataAssetType, const FString& SelectedFolderPath, TArray<FMWSoftPath>& OutPathArray);
	void Reload();
#endif
	
	// UI, Widget
	FSoftClassPath GetSceneUIPath(EMWSceneUIType SceneUIType);	
	FSoftClassPath GetUIPath(EMWWindowUIType UIType);	
	FSoftClassPath GetPopupUIPath(EMWPopupUIType UIType);

	// AnimMontage
	FSoftObjectPath GetAnimMontagePathByAniName(const FString& AnimationName);

	// Anim Schema Asset
	FSoftObjectPath GetAnimSchemaAssetPath();

	// Anim Asset
	FSoftObjectPath GetAnimAssetPath(const FString& AnimClassName);

	// Common ClassPath
	FSoftClassPath GetClassPath(const UMWDataAsset::EDataAssetType& DataAssetType, const FString& Name);

	// Common ObjectPath
	FSoftObjectPath GetObjectPath(const UMWDataAsset::EDataAssetType& DataAssetType, const FString& Name, const TArray<FMWSoftPath>& InPathArray);
	FSoftObjectPath GetObjectPath(const UMWDataAsset::EDataAssetType& DataAssetType, const FString& Name);
	FSoftObjectPath* GetObjectPath(const UMWDataAsset::EDataAssetType& DataAssetType, const FName& Name);	

	const FMWSoftPath* GetMWSoftPathData(const UMWDataAsset::EDataAssetType& DataAssetType);

	// Collect object paths from a specific folder.
	void GatherObjectPathInFolder(const UMWDataAsset::EDataAssetType& DataAssetType, const FString& InFolder, TArray<FSoftObjectPath>& OutPathArray, bool bIncludeSubFolder=true);

private:
	UPROPERTY(VisibleAnywhere)
	TArray<FMWSoftPath> PathArray;
};