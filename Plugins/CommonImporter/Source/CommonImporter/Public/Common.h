#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCommonImporter, Log, All);

namespace CommonImporter
{
	/* Change all the "\\" by "/" of string. */
	void FormatPathStr(FString& InOutStr);

	/* Get a asset path from it's path in system. */
	FString ConvertToAssetPath(const FString& FileSystemPath);
}