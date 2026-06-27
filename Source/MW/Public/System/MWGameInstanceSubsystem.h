#pragma once

// Include Header
#include "Subsystems/GameInstanceSubsystem.h"
#include "MWGameInstanceSubsystem.generated.h"

// Forward Declare
class UMWCharacterDataManager;
class UMWSaveGameManager;
class UMWSkillDataManager;
class UMWSkillInputService;
class UMWUIManager;

// Define

/*
 * @class UMWGameInstanceSubsystem
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class MW_API UMWGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	static UMWGameInstanceSubsystem* Get(const UObject* WorldContext);

	UMWCharacterDataManager* GetCharacterDataManager()
	{
		return CharacterDataManager;
	}

	UMWSaveGameManager* GetSaveGameManager()
	{
		return SaveGameManager;
	}

	UMWSkillDataManager* GetSkillDataManager()
	{
		return SkillDataManager;
	}

	UMWSkillInputService* GetSkillInputService()
	{
		return SkillInputService;
	}

	UMWUIManager* GetUIManager()
	{
		return UIManager;
	}

private:
	UPROPERTY()
	TObjectPtr<UMWCharacterDataManager> CharacterDataManager = nullptr;

	UPROPERTY()
	TObjectPtr<UMWSaveGameManager> SaveGameManager = nullptr;

	UPROPERTY()
	TObjectPtr<UMWSkillDataManager> SkillDataManager = nullptr;

	UPROPERTY()
	TObjectPtr<UMWSkillInputService> SkillInputService = nullptr;

	UPROPERTY()
	TObjectPtr<UMWUIManager> UIManager = nullptr;
};