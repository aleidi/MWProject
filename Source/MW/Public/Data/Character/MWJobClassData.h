#pragma once

#include "Data/MWPrimaryAsset.h"
#include "MWJobClassData.generated.h"

//UCLASS()
//class UMWJobSkillData : public UDataAsset
//{
//	GENERATED_BODY()
//
//};

UCLASS()
class UMWJobClassData : public UMWPrimaryAsset
{
	GENERATED_BODY()

public:
	UMWJobClassData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Id = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	//UMWJobSkillData Skill;
};