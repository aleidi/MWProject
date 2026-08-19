// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class CommonImporter : ModuleRules
{
	public CommonImporter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... 必要な公開インクルードパスをここに追加 ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... 必要な非公開インクルードパスをここに追加 ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... 静的リンクする公開依存モジュールをここに追加 ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "BlueprintEditorLibrary",
                "Blutility",
				"UMG",
                "UMGEditor"

				// ... 静的リンクする非公開依存モジュールをここに追加 ...
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... モジュールが動的に読み込む依存をここに追加 ...
			}
			);

		// サードパーティライブラリを設定
		if(Target.Platform == UnrealTargetPlatform.Win64)
		{
			// インクルードパスを追加
			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty/libxl/include_cpp"));
			
			// .lib パスを追加
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "ThirdParty/libxl/lib64/libxl.lib"));

			// .dll パスを追加し、遅延ロードを設定
			RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "ThirdParty/libxl/bin64/libxl.dll"));
            PublicDelayLoadDLLs.Add("libxl.dll");

        }
	}
}
