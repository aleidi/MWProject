// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpatialNavigation.h"

#define LOCTEXT_NAMESPACE "FSpatialNavigationModule"

void FSpatialNavigationModule::StartupModule()
{
	// モジュールがメモリにロードされた後に実行される。正確なタイミングは.upluginファイルでモジュールごとに指定される
}

void FSpatialNavigationModule::ShutdownModule()
{
	// モジュールをクリーンアップするため、シャットダウン時に呼び出される場合がある。動的リロード対応モジュールでは、
	// モジュールをアンロードする前に呼び出される。
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSpatialNavigationModule, SpatialNavigation)
