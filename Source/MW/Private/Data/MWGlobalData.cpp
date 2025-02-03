// Copyright Epic Games, Inc. All Rights Reserved.

#include "Data/MWGlobalData.h"
#include "System/MWAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MWGlobalData)

UMWGlobalData::UMWGlobalData()
{
}

const UMWGlobalData& UMWGlobalData::UMWGlobalData::Get()
{
	return UMWAssetManager::Get().GetGlobalData();
}
