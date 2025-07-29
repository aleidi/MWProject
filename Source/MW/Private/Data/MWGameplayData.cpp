// Copyright Epic Games, Inc. All Rights Reserved.

#include "Data/MWGameplayData.h"
#include "System/MWAssetManager.h"
#include "Data/MWMasterData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MWGameplayData)

UMWGameplayData::UMWGameplayData()
{
}

const UMWGameplayData& UMWGameplayData::Get()
{
	return *UMWAssetManager::Get().GetMasterData().GameplayData;
}
