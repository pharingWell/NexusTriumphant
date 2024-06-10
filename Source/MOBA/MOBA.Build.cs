// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MOBA : ModuleRules
{
	public MOBA(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameplayAbilities", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });
    }
}
