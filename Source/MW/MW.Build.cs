// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class MW : ModuleRules
{
	public MW(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"GameplayTags",
			"GameplayAbilities",
			"GameplayTasks",
			"EnhancedInput",
			"GameFeatures",
			"ModularGameplay",
			"CommonInput",
			"UMG",
			"Slate",
			"SlateCore",
			"CommonUI",
			"Paper2D",
			"Niagara",
			"ModularGameplay",
            "ModularGameplayActors"
        });

		PrivateDependencyModuleNames.AddRange(new string[] 
		{
			"Common3DCamera"
		});

        // This adds editor module dependencies (only when building for the editor)
        if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.Add("UnrealEd");
		}
	}
}
