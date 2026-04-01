//#pragma once
//
//#include "Engine/DataTable.h"
//#include "MWGameDataTypes.generated.h"
//
//USTRUCT(BlueprintType)
//struct FMWCharacterDataRow : public FTableRowBase
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly)
//    int32 Id = -1;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	FName Name;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	FText DisplayName = FText::GetEmpty();
//
//	// ========== Appearance ==========
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Appearance")
//	TObjectPtr<UMWCharacterAppearanceData> DefaultAppearance;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Appearance")
//	TMap<int32, TSoftObjectPtr<UMWCharacterAppearanceData>> ExtraAppearance;
//
//	// ========== Animation ==========
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
//	TObjectPtr<UMWCharacterAnimData> DefaultAnimation;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
//	TObjectPtr<UMWAbilitySet> DefaultAbilitySet;
//};