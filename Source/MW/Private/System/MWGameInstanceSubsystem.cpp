#include "System/MWGameInstanceSubsystem.h"
#include "MWGameInstance.h"
#include "Gameplay/MWPartyManager.h"
#include "Entity/MWEntityManager.h"
#include "Gameplay/Battle/MWBattleSystem.h"
#include "UI/MWUIManager.h"

#define DEINIT_MANAGER(VarName)\
	if (IsValid(VarName))\
	{\
		VarName->Deinitialize();\
	}

void UMWGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	PartyManager		= NewObject<UMWPartyManager>(this);
	EntityManager	= NewObject<UMWEntityManager>(this);
	BattleSystem		= NewObject<UMWBattleSystem>(this);
	UIManager			= NewObject<UMWUIManager>(this);

	PartyManager->Initialize();
	EntityManager->Initialize();
	BattleSystem->Initialize();
	UIManager->Initialize();
}

void UMWGameInstanceSubsystem::Deinitialize()
{
	DEINIT_MANAGER(PartyManager);
	DEINIT_MANAGER(EntityManager);
	DEINIT_MANAGER(BattleSystem);
	DEINIT_MANAGER(UIManager);
}

UMWGameInstanceSubsystem* UMWGameInstanceSubsystem::Get(const UObject* WorldContext)
{
	if (UMWGameInstance* GameInstacne = UMWGameInstance::Get(WorldContext))
	{
		return GameInstacne->GetSubsystem<UMWGameInstanceSubsystem>();
	}

	return nullptr;
}
