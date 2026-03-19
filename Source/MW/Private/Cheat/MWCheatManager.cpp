#include "Cheat/MWCheatManager.h"
#include "Define/MWDefineDataTable.h"
#include "Define/MWDefineGameplay.h"
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
	const FMWCharacterTemplateData* data = /*DATATABLEMANAGER()->GetCharacterDataById(CharacterId)*/nullptr;

	FVector loc = UGameplayStatics::GetPlayerController(this, 0)->GetPawn()->GetActorLocation();

	//ENTITYMANAGER(this)->SpawnCharacter(CharacterId, loc);
}

void UMWCheatManager::SpawnBattleUnit(int32 C1, int32 C2, int32 C3)
{
	FVector loc = UGameplayStatics::GetPlayerController(this, 0)->GetPawn()->GetActorLocation();

	//ENTITYMANAGER(this)->SpawnBattleUnitCharacter(C1, C2, C3, loc);
}
