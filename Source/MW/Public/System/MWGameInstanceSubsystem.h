#pragma once

// Include Header
#include "Subsystems/GameInstanceSubsystem.h"
#include "MWGameInstanceSubsystem.generated.h"

// Forward Declare
class UMWPartyManager;
class UMWEntityManager;
class UMWBattleSystem;
class UMWUIManager;

// Macro

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

	UMWBattleSystem* GetBattleSystem()
	{
		return BattleSystem;
	}

	UMWUIManager* GetUIManager()
	{
		return UIManager;
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
};