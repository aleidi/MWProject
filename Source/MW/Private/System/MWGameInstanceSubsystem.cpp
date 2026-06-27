#include "System/MWGameInstanceSubsystem.h"

#include "Data/Character/MWCharacterDataManager.h"
#include "Data/Skill/MWSkillDataManager.h"
#include "Input/MWSkillInputService.h"
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
	INIT_MANAGER(CharacterDataManager, UMWCharacterDataManager);
	INIT_MANAGER(SaveGameManager, UMWSaveGameManager);
	INIT_MANAGER(SkillDataManager, UMWSkillDataManager);
	INIT_MANAGER(SkillInputService, UMWSkillInputService);
	INIT_MANAGER(UIManager, UMWUIManager);
}

void UMWGameInstanceSubsystem::Deinitialize()
{
	DEINIT_MANAGER(CharacterDataManager);
	DEINIT_MANAGER(SaveGameManager);
	DEINIT_MANAGER(SkillDataManager);
	DEINIT_MANAGER(SkillInputService);
	DEINIT_MANAGER(UIManager);
}

UMWGameInstanceSubsystem* UMWGameInstanceSubsystem::Get(const UObject* WorldContext)
{
	if (UMWGameInstance* gameInstance = UMWGameInstance::Get(WorldContext))
	{
		return gameInstance->GetSubsystem<UMWGameInstanceSubsystem>();
	}

	return nullptr;
}
