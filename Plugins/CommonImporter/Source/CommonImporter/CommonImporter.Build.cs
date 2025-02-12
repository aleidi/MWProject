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
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
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

				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		// include third party
		if(Target.Platform == UnrealTargetPlatform.Win64)
		{
			// add include path
			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty/libxl/include_cpp"));
			
			// add .lib path
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "ThirdParty/libxl/lib64/libxl.lib"));

			// add .dll path and set delay load
			RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "ThirdParty/libxl/bin64/libxl.dll"));
            PublicDelayLoadDLLs.Add("libxl.dll");

        }
	}
}
