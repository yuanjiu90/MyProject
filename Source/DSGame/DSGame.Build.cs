// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DSGame : ModuleRules
{
	public DSGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"GameplayTags"
		});
	}
}
