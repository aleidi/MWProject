#pragma once

#include "GameFramework/CheatManager.h"
#include "MWCheatManager.generated.h"


UCLASS()
class MW_API UMWCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override;

	UFUNCTION(exec)
	void PlayFMODSound(const FString& InEvent);

	UFUNCTION(exec)
	void StopChannelSound(bool bFadeOut = false);

	UFUNCTION(exec)
	void GetChannelVolume();

	UFUNCTION(exec)
	void SetChannelVolume(float InVolume);

	UFUNCTION(exec)
	void SetChannelMute(bool bInMute);

	UFUNCTION(exec)
	void GetFMODMemoryUsage();

	UFUNCTION(exec)
	void ReloadData();

	UFUNCTION(exec)
	void SetGlobalTimeDilation(float InTimeDilation = 1.f);

	UFUNCTION(exec)
	void TeleportTo(float InPosX, float InPosY, float InPosZ);

	UFUNCTION(exec)
	void LoadStreamLevel(const FString& MapName);

	UFUNCTION(exec)
	void UnloadStreamLevel(const FString& MapName);

	UFUNCTION(exec)
	void PrepareStreamLevel(float X, float Y, float Z);

	UFUNCTION(exec)
	void SetFollowCameraLengthMinMax(float InMin, float InMax);
	UFUNCTION(exec)
	void SetFollowCameraOffset(float X, float Y, float Z);
	UFUNCTION(exec)
	void SetFov(float InFov);

	UFUNCTION(exec)
	void PlaySequence();

	UFUNCTION(exec)
	void SpawnCharacter(int32 CharacterId);

	UFUNCTION(exec)
	void SpawnBattleUnit(int32 C1, int32 C2, int32 C3);
};
