//// Copyright Epic Games, Inc. All Rights Reserved.
//
//#pragma once
//
//#include "GameFeatureAction.h"
//#include "UObject/SoftObjectPath.h"
//#include "GameFeatureAction_AddGameplayCuePath.generated.h"
//
///**
// * GameplayCueManagerへGameplayCueパスを追加するGameFeatureActionです。
// *
// * @see UAbilitySystemGlobals::GameplayCueNotifyPaths
// */
//UCLASS(MinimalAPI, meta = (DisplayName = "Add Gameplay Cue Path"))
//class UGameFeatureAction_AddGameplayCuePath final : public UGameFeatureAction
//{
//	GENERATED_BODY()
//
//public:
//
//	UGameFeatureAction_AddGameplayCuePath();
//
//	//~UObjectインターフェース
//#if WITH_EDITOR
//	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
//#endif
//	//~UObjectインターフェース終了
//
//	const TArray<FDirectoryPath>& GetDirectoryPathsToAdd() const { return DirectoryPathsToAdd; }
//
//private:
//	/** GameplayCueManagerへ登録するパス一覧。Game Contentディレクトリからの相対パスです。 */
//	UPROPERTY(EditAnywhere, Category = "Game Feature | Gameplay Cues", meta = (RelativeToGameContentDir, LongPackageName))
//	TArray<FDirectoryPath> DirectoryPathsToAdd;
//};
