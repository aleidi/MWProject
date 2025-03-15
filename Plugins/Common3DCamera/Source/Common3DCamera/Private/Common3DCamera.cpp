// Copyright Epic Games, Inc. All Rights Reserved.

#include "Common3DCamera.h"

#define LOCTEXT_NAMESPACE "FCommon3DCameraModule"

DEFINE_LOG_CATEGORY(LogC3D);

void FCommon3DCameraModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FCommon3DCameraModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCommon3DCameraModule, Common3DCamera)