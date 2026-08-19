// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Common3DCamera : ModuleRules
{
	public Common3DCamera(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... 必要なパブリックインクルードパスをここに追加 ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... 必要なその他のプライベートインクルードパスをここに追加 ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... 静的リンクするその他のパブリック依存関係をここに追加 ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "GameplayTags",
                "UMG",
				// ... 静的リンクするプライベート依存関係をここに追加 ...
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... 動的にロードするモジュールをここに追加 ...
			}
			);
	}
}
