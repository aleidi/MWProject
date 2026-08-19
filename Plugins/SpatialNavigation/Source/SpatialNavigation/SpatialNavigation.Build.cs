// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpatialNavigation : ModuleRules
{
	public SpatialNavigation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... 必要なパブリックインクルードパスをここに追加 ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... その他の必要なプライベートインクルードパスをここに追加 ...
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
				"ProceduralMeshComponent",
				"AIModule",
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
