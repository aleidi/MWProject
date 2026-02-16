using UnrealBuildTool;

public class CombatRelation : ModuleRules
{
	public CombatRelation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"GameplayTags",
			"GameplayAbilities",
			"GameplayTasks",
			"AIModule",
			"ModularGameplay",
			"ModularGameplayActors",
			"DeveloperSettings"
		});
	}
}
