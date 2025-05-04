// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ai_deneme : ModuleRules
{
	public ai_deneme(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
