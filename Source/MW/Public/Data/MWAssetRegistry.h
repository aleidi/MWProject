#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MWAssetRegistry.generated.h"

enum class EMWSceneUIType : uint8;
enum class EMWWindowUIType : uint8;
enum class EMWPopupUIType;

#define ASSETREGISTRY() (UMWAssetRegistry::Get())

USTRUCT()
struct FMWSoftPath
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TMap<FName, FSoftObjectPath> PathMap;
};

/**
 * UMWAssetRegistry
 *
 * Central asset path registry for the MW project.
 * Inherits from UPrimaryDataAsset and acts as a global soft-reference lookup table,
 * mapping asset names to their FSoftObjectPath / FSoftClassPath for every supported asset type.
 *
 * Usage:
 *   - Access via the ASSETREGISTRY() macro or UMWAssetRegistry::Get().
 *   - In Editor / Cook builds, call Initialize() once to populate the internal path table
 *     by scanning content directories using UObjectLibrary.
 *   - At runtime, query asset paths by name without loading the assets directly,
 *     keeping memory usage minimal until assets are explicitly loaded.
 *
 * Naming conventions used when building keys:
 *   - Widget classes  : WBP_<EnumName>
 *   - Anim Montages   : AM_<AnimationName>
 *   - Anim Schema     : ASA_Motion (fixed)
 */
UCLASS(BlueprintType, Blueprintable)
class MW_API UMWAssetRegistry : public UPrimaryDataAsset
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

	static UMWAssetRegistry* Get();

	void Initialize();

	UClass* GetPathAndBaseClass(const UMWAssetRegistry::EDataAssetType& DataAssetType, FString& DataPath);

#if WITH_EDITOR
	//Create ObjectLibrary during cooking (packaging)
	void CreateLibrarys();	
	
	virtual void BeginCacheForCookedPlatformData(const ITargetPlatform* TargetPlatform) override;

	const FMWSoftPath* GetSelectedTypePathData(UMWAssetRegistry::EDataAssetType DataAssetType, const FString& SelectedFolderPath, TArray<FMWSoftPath>& OutPathArray);
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
	FSoftClassPath GetClassPath(const UMWAssetRegistry::EDataAssetType& DataAssetType, const FString& Name);

	// Common ObjectPath
	FSoftObjectPath GetObjectPath(const UMWAssetRegistry::EDataAssetType& DataAssetType, const FString& Name, const TArray<FMWSoftPath>& InPathArray);
	FSoftObjectPath GetObjectPath(const UMWAssetRegistry::EDataAssetType& DataAssetType, const FString& Name);
	FSoftObjectPath* GetObjectPath(const UMWAssetRegistry::EDataAssetType& DataAssetType, const FName& Name);	

	const FMWSoftPath* GetMWSoftPathData(const UMWAssetRegistry::EDataAssetType& DataAssetType);

	// Collect object paths from a specific folder.
	void GatherObjectPathInFolder(const UMWAssetRegistry::EDataAssetType& DataAssetType, const FString& InFolder, TArray<FSoftObjectPath>& OutPathArray, bool bIncludeSubFolder=true);

private:
	UPROPERTY(VisibleAnywhere)
	TArray<FMWSoftPath> PathArray;
};