// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MWEditor : ModuleRules
{
	public MWEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{

        });

		PrivateDependencyModuleNames.AddRange(new string[] 
		{
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UnrealEd",
            "BlueprintGraph",
            "Blutility",
		});

		// Slate UIを使用する場合はコメントを解除
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// オンライン機能を使用する場合はコメントを解除
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// OnlineSubsystemSteamを使用する場合はuprojectのPluginsへ追加し、Enabledをtrueに設定
	}
}
