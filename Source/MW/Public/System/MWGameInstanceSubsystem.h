#pragma once

// Include Header
#include "Subsystems/GameInstanceSubsystem.h"
#include "MWGameInstanceSubsystem.generated.h"

// Forward Declare
class UMWPartyManager;
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

	UMWPartyManager* GetPartyManager()
	{
		return PartyManager;
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
	TObjectPtr<UMWPartyManager> PartyManager;

	UPROPERTY()
	TObjectPtr<UMWUIManager> UIManager;

	UPROPERTY()
	TObjectPtr<UMWSaveGameManager> SaveGameManager;
};