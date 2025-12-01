// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DSGame : ModuleRules
{
	public DSGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new[] 
		{
			"Core", 
			"CoreUObject", 
			"Engine",  
			"InputCore", 
			"EnhancedInput",
			"GameplayTags",
			"DeveloperSettings",
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});
	}
}
