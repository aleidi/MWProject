#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCommonImporter, Log, All);

namespace CommonImporter
{
	/* 文字列中の "\\" をすべて "/" に変換する。 */
	void FormatPathStr(FString& InOutStr);

	/* ファイルシステムパスからアセットパスを取得する。 */
	FString ConvertToAssetPath(const FString& FileSystemPath);
}