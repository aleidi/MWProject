#pragma once

#include "Data/Character//MWCharacterDataTypes.h"
#include "Data/MWPrimaryAsset.h"
#include "MWCharacterData.generated.h"

class UMWAbilitySet;

UCLASS()
class UMWCharacterData : public UMWPrimaryAsset
{
	GENERATED_BODY()

public:
	UMWCharacterData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Id = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Appearance")
	FMWCharacterAppearanceData DefaultAppearance;

	// TODO: Other appearance when changing outfit

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	FMWCharacterAnimData DefaultAnimation;

	// TODO: AbilitySet
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TArray<TSoftObjectPtr<UMWAbilitySet>> BaseAbilitySets;
};