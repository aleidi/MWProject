// Copyright Epic Games, Inc. All Rights Reserved.

#include "Common3DCamera.h"

#define LOCTEXT_NAMESPACE "FCommon3DCameraModule"

DEFINE_LOG_CATEGORY(LogC3D);

void FCommon3DCameraModule::StartupModule()
{
	// モジュールがメモリにロードされた後に実行される。正確なタイミングは.upluginのモジュール設定で指定される
}

void FCommon3DCameraModule::ShutdownModule()
{
	// シャットダウン時のモジュール解放処理として呼び出される場合がある。動的リロードをサポートするモジュールでは、
	// モジュールをアンロードする前に呼び出される。
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCommon3DCameraModule, Common3DCamera)