#include "System/MWGameInstanceSubsystem.h"
#include "Gameplay/MWPartyManager.h"
#include "MWGameInstance.h"
#include "SaveGame/MWSaveGameManager.h"
#include "UI/MWUIManager.h"

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
	INIT_MANAGER(GameDataManager, UMWGameDataManager);
	INIT_MANAGER(PartyManager, UMWPartyManager);
	INIT_MANAGER(UIManager, UMWUIManager);
	INIT_MANAGER(SaveGameManager, UMWSaveGameManager);
}

void UMWGameInstanceSubsystem::Deinitialize()
{
	DEINIT_MANAGER(GameDataManager);
	DEINIT_MANAGER(PartyManager);
	DEINIT_MANAGER(UIManager);
	DEINIT_MANAGER(SaveGameManager);
}

UMWGameInstanceSubsystem* UMWGameInstanceSubsystem::Get(const UObject* WorldContext)
{
	if (UMWGameInstance* GameInstacne = UMWGameInstance::Get(WorldContext))
	{
		return GameInstacne->GetSubsystem<UMWGameInstanceSubsystem>();
	}

	return nullptr;
}
