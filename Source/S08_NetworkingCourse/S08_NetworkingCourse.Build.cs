// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class S08_NetworkingCourse : ModuleRules
{
	public S08_NetworkingCourse(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "Slate", "SlateCore" });
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "OnlineSubsystem", "OnlineSubsystemSteam", "Sockets", "Networking", "NetCore" });
    }
}
