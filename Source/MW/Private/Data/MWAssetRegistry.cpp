// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Data/MWAssetRegistry.h"
#include "Define/MWDefineCommon.h"
#include "MWLogChannels.h"
#include "MWGameSingleton.h"
#include "Util/MWEnumUtil.h"

#include "Engine/ObjectLibrary.h"
#include "Engine/DataTable.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Texture.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimBlueprint.h"
#include "Materials/MaterialInterface.h"
#include "PaperSprite.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

UMWAssetRegistry* UMWAssetRegistry::Get()
{
	return MWSINGLETON->GetDataAsset();
}

void UMWAssetRegistry::Initialize()
{
#if WITH_EDITOR
	CreateLibrarys();
#endif
}

UClass* UMWAssetRegistry::GetPathAndBaseClass(const EDataAssetType& DataAssetType, FString& DataPath)
{
	switch (DataAssetType)
	{
	case UMWAssetRegistry::EDataAssetType::Widget:
		DataPath = TEXT("/Game/UI/");					
		return UObject::StaticClass();

	case UMWAssetRegistry::EDataAssetType::Texture:
		DataPath = TEXT("/Game/Gfx/image/UIImages/Textures");
		return UTexture::StaticClass();

	case UMWAssetRegistry::EDataAssetType::Sprite:
		DataPath = TEXT("/Game/Gfx/image/UIImages/Textures");
		return UPaperSprite::StaticClass();

	case UMWAssetRegistry::EDataAssetType::AnimSchemaAsset:
		DataPath = TEXT("/Game/Asset/Character");
		return UObject::StaticClass();
		//return UMWAnimSchemaAsset::StaticClass();

	case UMWAssetRegistry::EDataAssetType::AnimAsset:
		DataPath = TEXT("/Game/Asset/Character");
		return UObject::StaticClass();
		//return UMWAnimAsset::StaticClass();

	case UMWAssetRegistry::EDataAssetType::StaticMesh:
		DataPath = TEXT("/Game/Asset/Character");
		return UStaticMesh::StaticClass();

	case UMWAssetRegistry::EDataAssetType::SkeletalMesh:
		DataPath = TEXT("/Game/Asset/Character");
		return USkeletalMesh::StaticClass();

	case UMWAssetRegistry::EDataAssetType::AnimSequence:
		DataPath = TEXT("/Game/Asset/Character");
		return UAnimSequence::StaticClass();

	case UMWAssetRegistry::EDataAssetType::AnimMontage:
		DataPath = TEXT("/Game/Asset/Character");
		return UAnimMontage::StaticClass();

	case UMWAssetRegistry::EDataAssetType::AnimationBP:
		DataPath = TEXT("/Game/Asset/Character");
		return UAnimBlueprint::StaticClass();

	case UMWAssetRegistry::EDataAssetType::DataTable:
		DataPath = TEXT("/Game/Data/DataTable");
		return UDataTable::StaticClass();

	case UMWAssetRegistry::EDataAssetType::PropStaticMesh:
		DataPath = TEXT("/Game/Asset/BG");
		return UStaticMesh::StaticClass();

	case UMWAssetRegistry::EDataAssetType::PropSkeletalMesh:
		DataPath = TEXT("/Game/Asset/BG");
		return USkeletalMesh::StaticClass();

#if WITH_EDITOR
	case UMWAssetRegistry::EDataAssetType::Map:
		DataPath = TEXT("/Game/World");
		return UWorld::StaticClass();
#endif

	case UMWAssetRegistry::EDataAssetType::WidgetStyle:		
		DataPath = TEXT("/Game/GamePlay/Data/DataAsset/Theme");
		return UDataAsset::StaticClass();

	case UMWAssetRegistry::EDataAssetType::Environment:
		DataPath = TEXT("/Game/GamePlay/Data/DataAsset/Environment");
		return UDataAsset::StaticClass();
	default:
		break;
	}
	return UObject::StaticClass();
}

#if WITH_EDITOR

void UMWAssetRegistry::CreateLibrarys()
{	
	PathArray.Reserve((int32)EDataAssetType::DataAssetTypeMax);
	PathArray.Init(FMWSoftPath(), (int32)EDataAssetType::DataAssetTypeMax);

	for (int32 dataAssetType = (int32)EDataAssetType::Widget; dataAssetType != (int32)EDataAssetType::DataAssetTypeMax; ++dataAssetType)
	{
		FString dataPath;
		UClass* inBaseClass = GetPathAndBaseClass((EDataAssetType)dataAssetType, dataPath);

		// When cooking, only the worlds registered in the region mapping table are packaged instead of the entire Content/World/.
		if (IsRunningCommandlet() && inBaseClass == UWorld::StaticClass())
		{
			continue;
		}

		TMap<FName, FSoftObjectPath>& dataPathMap = PathArray[dataAssetType].PathMap;

		UObjectLibrary* ObjectLib = UObjectLibrary::CreateLibrary(inBaseClass, false, GIsEditor);		
		ObjectLib->LoadAssetDataFromPath(dataPath);

		TArray<FAssetData> assetList;
		ObjectLib->GetAssetDataList(assetList);
				
		for (FAssetData& data : assetList)
		{
			FName key = data.AssetName;
			
			if (dataPathMap.Find(key) == nullptr)
			{
				dataPathMap.Add(key, data.GetSoftObjectPath());
			}
			else if (dataAssetType != (int32)EDataAssetType::InValidType)
			{
				const EDataAssetType tempValue = (EDataAssetType)dataAssetType;

				MW_LOG_WARNING(TEXT("%s DataType is Duplicated - name : %s, path : %s"),
					*ENUM_TO_STRING(EDataAssetType, tempValue),
					*data.AssetName.ToString(), *data.GetObjectPathString());
			}
		}
	}
}

void UMWAssetRegistry::Reload()
{
	PathArray.Empty();

	CreateLibrarys();
}

void UMWAssetRegistry::BeginCacheForCookedPlatformData(const ITargetPlatform* TargetPlatform)
{
	Super::BeginCacheForCookedPlatformData(TargetPlatform);

	CreateLibrarys();
}

const FMWSoftPath* UMWAssetRegistry::GetSelectedTypePathData(EDataAssetType InDataAssetType, const FString& SelectedFolderPath, TArray<FMWSoftPath>& OutPathArray)
{
	FString dataPath;
	EDataAssetType dataAssetType = (EDataAssetType)InDataAssetType;
	UClass* inBaseClass = GetPathAndBaseClass(dataAssetType, dataPath);
	dataPath = SelectedFolderPath;

	int32 index = (int32)InDataAssetType;
	TMap<FName, FSoftObjectPath>& dataPathMap = OutPathArray[index].PathMap;

	UObjectLibrary* objectLib = UObjectLibrary::CreateLibrary(inBaseClass, false, GIsEditor);
	objectLib->LoadAssetDataFromPath(dataPath);

	TArray<FAssetData> assetList;
	objectLib->GetAssetDataList(assetList);
	for (FAssetData& data : assetList)
	{
		FName Key = data.AssetName;
		if (dataPathMap.Find(Key) == nullptr)
		{
			dataPathMap.Add(Key, data.GetSoftObjectPath());
		}
		else
		{
			const EDataAssetType tempValue = (EDataAssetType)dataAssetType;

			MW_LOG_WARNING(TEXT("%s DataType is Duplicated - name : %s, path : %s"),
				*ENUM_TO_STRING(EDataAssetType, tempValue),
				*data.AssetName.ToString(), *data.GetObjectPathString());
		}
	}	

	if (OutPathArray.IsValidIndex(index))
	{
		return &OutPathArray[index];
	}

	return nullptr;
}
#endif

FSoftClassPath UMWAssetRegistry::GetSceneUIPath(EMWSceneUIType SceneUIType)
{
	FString uiName = STRING_FORMAT(TEXT("WBP_%s"), *ENUM_TO_STRING(EMWSceneUIType, SceneUIType));

	return GetClassPath(EDataAssetType::Widget, uiName);
}

FSoftClassPath UMWAssetRegistry::GetUIPath(EMWWindowUIType UIType)
{
	FString uiName = STRING_FORMAT(TEXT("WBP_%s"), *ENUM_TO_STRING(EMWWindowUIType, UIType));

	return GetClassPath(EDataAssetType::Widget, uiName);
}

FSoftClassPath UMWAssetRegistry::GetPopupUIPath(EMWPopupUIType UIType)
{
	FString uiName = STRING_FORMAT(TEXT("WBP_%s"), *ENUM_TO_STRING(EMWPopupUIType, UIType));

	return GetClassPath(EDataAssetType::Widget, uiName);
}

FSoftObjectPath UMWAssetRegistry::GetAnimMontagePathByAniName(const FString& AnimationName)
{
	FString animMontageName = STRING_FORMAT(TEXT("AM_%s"), *AnimationName);

	return GetObjectPath(EDataAssetType::AnimMontage, animMontageName);
}

FSoftClassPath UMWAssetRegistry::GetClassPath(const EDataAssetType& DataAssetType, const FString& Name)
{
	check(Name.IsEmpty() == false);

	FName searchKey = FName(*Name);

	FSoftObjectPath* pathData = PathArray[(int32)DataAssetType].PathMap.Find(searchKey);

	if (pathData == nullptr)
	{
		MW_LOG_WARNING(TEXT("Not Found in DataAsset PathMap. Key Name : %s"), *Name);

		return FSoftClassPath();
	}

	return FSoftClassPath(STRING_FORMAT(TEXT("%s_C"), *(pathData->GetAssetPathString())));
}

FSoftObjectPath UMWAssetRegistry::GetAnimSchemaAssetPath()
{
	static const FString ASAName = TEXT("ASA_Motion");

	return GetObjectPath(EDataAssetType::AnimSchemaAsset, ASAName);
}

FSoftObjectPath UMWAssetRegistry::GetAnimAssetPath(const FString& AnimClassName)
{
	FString animAssetName = "AnimAsset";/*UMEMotionUtil::ClassNameToAssetName(AnimClassName);*/

	return GetObjectPath(EDataAssetType::AnimAsset, animAssetName);
}

FSoftObjectPath UMWAssetRegistry::GetObjectPath(const EDataAssetType& DataAssetType, const FString& Name)
{
	check(Name.IsEmpty() == false);

	FName searchKey = FName(*Name);

	FSoftObjectPath* softObjectPath = GetObjectPath(DataAssetType, searchKey);

	return softObjectPath ? *softObjectPath : FSoftObjectPath();
}

FSoftObjectPath* UMWAssetRegistry::GetObjectPath(const EDataAssetType& DataAssetType, const FName& Name)
{
	FSoftObjectPath* pathData = PathArray[(int32)DataAssetType].PathMap.Find(Name);

	if (pathData == nullptr)
	{
		MW_LOG_WARNING(TEXT("Not Found in DataAsset PathMap. Key Name : %s"), *Name.ToString());
	}

	return pathData;
}

FSoftObjectPath UMWAssetRegistry::GetObjectPath(const EDataAssetType& DataAssetType, const FString& Name, const TArray<FMWSoftPath>& InPathArray)
{
	check(Name.IsEmpty() == false);

	FName searchKey = FName(*Name);

	if (const FSoftObjectPath* pathData = InPathArray[(int32)DataAssetType].PathMap.Find(searchKey))
	{
		return *pathData;
	}

	return FSoftObjectPath();
}

const FMWSoftPath* UMWAssetRegistry::GetMWSoftPathData(const UMWAssetRegistry::EDataAssetType& DataAssetType)
{
	int32 index = (int32)DataAssetType;
	if (PathArray.IsValidIndex(index))
	{
		return &PathArray[index];
	}

	return nullptr;
}

void UMWAssetRegistry::GatherObjectPathInFolder(const EDataAssetType& DataAssetType, const FString& InFolder, TArray<FSoftObjectPath>& OutPathArray, bool bIncludeSubFolder)
{
	static const TCHAR* SLASH	= TEXT("/");
	static const TCHAR SLASH_C	= TEXT('/');

	FString searchFolder = InFolder;
	if (!searchFolder.EndsWith(SLASH))
	{
		searchFolder += SLASH;
	}
	
	const TMap<FName, FSoftObjectPath>& allObjects = PathArray[(int32)DataAssetType].PathMap;

	for (const auto iter : allObjects)
	{
		const FSoftObjectPath& softObjectPath = iter.Value;

		FString objPath = softObjectPath.GetLongPackageName();
		int32 lastSlashIdx = INDEX_NONE;
		if ( objPath.FindLastChar(SLASH_C, lastSlashIdx) )
		{
			// Import including '/'
			objPath = objPath.Left(lastSlashIdx+1);
		}

		bool bMatch = false;

		// Include sub folders
		if (bIncludeSubFolder)
		{
			if (objPath.StartsWith(InFolder, ESearchCase::IgnoreCase))
			{
				bMatch = true;
			}
		}
		else
		{
			if(objPath.Equals(InFolder, ESearchCase::IgnoreCase))
			{
				bMatch = true;
			}
		}

		if (bMatch)
		{
			OutPathArray.Add(softObjectPath);
		}
	}
}