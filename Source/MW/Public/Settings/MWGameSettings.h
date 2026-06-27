#pragma once

#include "Engine/DeveloperSettings.h"
#include "MWGameSettings.generated.h"

class UDataTable;

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="MW Game Settings"))
class MW_API UMWGameSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UPROPERTY(Config, EditAnywhere, Category="Data|Character")
    TSoftObjectPtr<UDataTable> CharacterTableAsset;

    UPROPERTY(Config, EditAnywhere, Category="Data|Skill")
    TSoftObjectPtr<UDataTable> SkillTableAsset;
};