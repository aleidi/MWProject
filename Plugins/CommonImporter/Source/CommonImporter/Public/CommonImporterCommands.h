// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CommonImporterStyle.h"

class FCommonImporterCommands : public TCommands<FCommonImporterCommands>
{
public:

	FCommonImporterCommands()
		: TCommands<FCommonImporterCommands>(TEXT("CommonImporter"), NSLOCTEXT("Contexts", "CommonImporter", "CommonImporter Plugin"), NAME_None, FCommonImporterStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > CommonImporter;
};