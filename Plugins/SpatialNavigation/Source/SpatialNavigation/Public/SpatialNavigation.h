// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSpatialNavigationModule : public IModuleInterface
{
public:

	/** IModuleInterfaceの実装 */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
