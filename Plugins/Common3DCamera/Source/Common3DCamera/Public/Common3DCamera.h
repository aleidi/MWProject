// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogC3D, Log, All);

class FCommon3DCameraModule : public IModuleInterface
{
public:

	/** IModuleInterfaceの実装 */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
