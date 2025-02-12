// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonImporterCommands.h"

#define LOCTEXT_NAMESPACE "FCommonImporterModule"

void FCommonImporterCommands::RegisterCommands()
{
	UI_COMMAND(CommonImporter, "CommonImporter", "Bring up CommonImporter window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
