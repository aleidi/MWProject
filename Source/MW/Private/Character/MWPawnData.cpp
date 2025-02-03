// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/MWPawnData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MWPawnData)

UMWPawnData::UMWPawnData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PawnClass = nullptr;
	InputConfig = nullptr;
	//DefaultCameraMode = nullptr;
}

FPrimaryAssetId UMWPawnData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FPrimaryAssetType(TEXT("PawnData")), FName(*GetNameSafe(this)));
}

