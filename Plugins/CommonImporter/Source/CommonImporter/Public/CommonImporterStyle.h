// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Common.h"
#include "Styling/SlateStyle.h"

/**  */
class FCommonImporterStyle
{
public:

	static void Initialize();

	static void Shutdown();

	/** Slate レンダラーで使用中のテクスチャを再読み込みする */
	static void ReloadTextures();

	/** @return CommonImporter 用の Slate スタイルセット */
	static const ISlateStyle& Get();

	static FName GetStyleSetName();

private:

	static TSharedRef< class FSlateStyleSet > Create();

private:

	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};