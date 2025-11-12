#include "System/MWGameInstanceSubsystem.h"
#include "MWGameInstance.h"
#include "Gameplay/MWPartyManager.h"
#include "Entity/MWEntityManager.h"
#include "Gameplay/Battle/MWBattleSystem.h"
#include "UI/MWUIManager.h"
#include "Data/MWRuntimeDataManager.h"

#define INIT_MANAGER(VarName, ClassType)\
	VarName = NewObject<ClassType>(this);\
	VarName->Initialize();

#define DEINIT_MANAGER(VarName)\
	if (IsValid(VarName))\
	{\
		VarName->Deinitialize();\
	}

void UMWGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	INIT_MANAGER(PartyManager, UMWPartyManager);
	INIT_MANAGER(EntityManager, UMWEntityManager);
	INIT_MANAGER(BattleSystem, UMWBattleSystem);
	INIT_MANAGER(UIManager, UMWUIManager);
	INIT_MANAGER(RuntimeDataManager, UMWRuntimeDataManager);
}

void UMWGameInstanceSubsystem::Deinitialize()
{
	DEINIT_MANAGER(PartyManager);
	DEINIT_MANAGER(EntityManager);
	DEINIT_MANAGER(BattleSystem);
	DEINIT_MANAGER(UIManager);
	DEINIT_MANAGER(RuntimeDataManager);
}

UMWGameInstanceSubsystem* UMWGameInstanceSubsystem::Get(const UObject* WorldContext)
{
	if (UMWGameInstance* GameInstacne = UMWGameInstance::Get(WorldContext))
	{
		return GameInstacne->GetSubsystem<UMWGameInstanceSubsystem>();
	}

	return nullptr;
}
