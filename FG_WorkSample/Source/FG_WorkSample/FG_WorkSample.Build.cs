// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FG_WorkSample : ModuleRules
{
	public FG_WorkSample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore", 
            "EnhancedInput",
            "Niagara",
            "GameplayTasks",
            "NavigationSystem"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(new string[]
        {
            "FG_WorkSample/Public/Player",
            "FG_WorkSample/Public/Components",
            "FG_WorkSample/Public/Pickups",
            "FG_WorkSample/Public/AI",
            "FG_WorkSample/Public/AI/Tasks",
            "FG_WorkSample/Public/AI/Services",
            "FG_WorkSample/Public/"
        });
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
