#include "Cheat/MWCheatManager.h"
#include "Data/MWDataTableManager.h"
#include "Define/MWDefineDataTable.h"
#include "Define/MWDefineGameplay.h"
#include "Entity/MWEntityManager.h"
#include "Gameplay/Battle/BattleUnit/MWBattleUnitAvatar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStatics.h"

void UMWCheatManager::BeginDestroy()
{
	Super::BeginDestroy();
}

void UMWCheatManager::PlayFMODSound(const FString& InEvent)
{
}

void UMWCheatManager::StopChannelSound(bool bFadeOut)
{
}

void UMWCheatManager::GetChannelVolume()
{
}

void UMWCheatManager::SetChannelVolume(float InVolume)
{
}

void UMWCheatManager::SetChannelMute(bool bInMute)
{
}

void UMWCheatManager::GetFMODMemoryUsage()
{
}

void UMWCheatManager::ReloadData()
{
}

void UMWCheatManager::SetGlobalTimeDilation(float InTimeDilation)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), InTimeDilation);
}

void UMWCheatManager::TeleportTo(float InPosX, float InPosY, float InPosZ)
{
}

void UMWCheatManager::LoadStreamLevel(const FString& MapName)
{
}

void UMWCheatManager::UnloadStreamLevel(const FString& MapName)
{
}

void UMWCheatManager::PrepareStreamLevel(float X, float Y, float Z)
{
}

void UMWCheatManager::SetFollowCameraLengthMinMax(float InMin, float InMax)
{
}

void UMWCheatManager::SetFollowCameraOffset(float X, float Y, float Z)
{
}

void UMWCheatManager::SetFov(float InFov)
{
}

void UMWCheatManager::PlaySequence()
{
}

void UMWCheatManager::SpawnCharacter(int32 CharacterId)
{
	const FMWCharacterData* data = DATATABLEMANAGER->GetCharacterDataById(CharacterId);

	FVector loc = UGameplayStatics::GetPlayerController(this, 0)->GetPawn()->GetActorLocation();

	if (data)
	{
		FMWCharacterResourceData resData;
		resData.VisualAnimInst = data->CharacterAnimInst.LoadSynchronous();
		resData.VisualMesh = data->CharacterMesh.LoadSynchronous();
		resData.Height = data->Height;

		ENTITYMANAGER(this)->CreateCharacterEntity(loc, FRotator::ZeroRotator, &resData);
	}
}

void UMWCheatManager::SpawnBattleUnit(int32 CharacterId)
{
	const FMWCharacterData* data = DATATABLEMANAGER->GetCharacterDataById(CharacterId);

	FVector loc = UGameplayStatics::GetPlayerController(this, 0)->GetPawn()->GetActorLocation();

	if (data)
	{
		FMWCharacterResourceData resData;
		resData.VisualAnimInst = data->CharacterAnimInst.LoadSynchronous();
		resData.VisualMesh = data->CharacterMesh.LoadSynchronous();
		resData.CharacterClass = AMWBattleUnitAvatar::StaticClass();

		ENTITYMANAGER(this)->CreateCharacterEntity(loc, FRotator::ZeroRotator, &resData);
	}
}
