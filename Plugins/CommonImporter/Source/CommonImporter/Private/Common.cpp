#include "Common.h"

DEFINE_LOG_CATEGORY(LogCommonImporter);

void CommonImporter::FormatPathStr(FString& InOutStr)
{
	InOutStr = InOutStr.Replace(TEXT("\\"), TEXT("/"));
}

FString CommonImporter::ConvertToAssetPath(const FString& FileSystemPath)
{
    FString projContDir = FPaths::ProjectContentDir();

    FormatPathStr(projContDir);

    if (FileSystemPath.StartsWith(projContDir)) 
    {
        const FString baseFileName = FPaths::GetBaseFilename(FileSystemPath);

        // asset format : /Game/.../filename.filename
        FString relativePath = FPaths::GetPath(FileSystemPath) / FString::Printf(TEXT("%s.%s"), *baseFileName, *baseFileName);
        relativePath = relativePath.RightChop(projContDir.Len());
        return FString::Printf(TEXT("/Game/%s"), *relativePath);
    }

    return FString();
}
