#pragma once

// Include Header
#include "Subsystems/GameInstanceSubsystem.h"
#include "MWGameInstanceSubsystem.generated.h"

// Forward Declare
class UMWCharacterDataManager;
class UMWSaveGameManager;
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

	UMWUIManager* GetUIManager()
	{
		return UIManager;
	}

	UMWSaveGameManager* GetSaveGameManager()
	{
		return SaveGameManager;
	}

private:
	UPROPERTY()
	TObjectPtr<UMWCharacterDataManager> CharacterDataManager = nullptr;

	UPROPERTY()
	TObjectPtr<UMWUIManager> UIManager = nullptr;

	UPROPERTY()
	TObjectPtr<UMWSaveGameManager> SaveGameManager = nullptr;
};