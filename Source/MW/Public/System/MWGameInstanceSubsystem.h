#pragma once

// Include Header
#include "Subsystems/GameInstanceSubsystem.h"
#include "MWGameInstanceSubsystem.generated.h"

// Forward Declare
class UMWBattleSystem;
class UMWEntityManager;
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

	UMWEntityManager* GetEntityManager()
	{
		return EntityManager;
	}

	UMWBattleSystem* GetBattleManager()
	{
		return BattleSystem;
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
	TObjectPtr<UMWEntityManager> EntityManager;

	UPROPERTY()
	TObjectPtr<UMWBattleSystem> BattleSystem;

	UPROPERTY()
	TObjectPtr<UMWUIManager> UIManager;

	UPROPERTY()
	TObjectPtr<UMWSaveGameManager> SaveGameManager;
};