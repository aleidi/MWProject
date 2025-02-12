// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonImporterStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FCommonImporterStyle::StyleInstance = nullptr;

void FCommonImporterStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FCommonImporterStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FCommonImporterStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("CommonImporterStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

TSharedRef< FSlateStyleSet > FCommonImporterStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("CommonImporterStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("CommonImporter")->GetBaseDir() / TEXT("Resources"));

	Style->Set("CommonImporter.OpenPluginWindow", new IMAGE_BRUSH(TEXT("Icon40"), Icon40x40));

	return Style;
}

void FCommonImporterStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FCommonImporterStyle::Get()
{
	return *StyleInstance;
}
