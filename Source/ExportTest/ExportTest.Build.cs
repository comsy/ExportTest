// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class ExportTest : ModuleRules
{
	public ExportTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ApplicationCore", "WebSockets" });

		PrivateDependencyModuleNames.AddRange(new string[] { "HTTP" , "Json", "JsonUtilities" });

        //PublicSystemLibraries.Add("Gdiplus.lib");
        // Ãß°¡: Windows ÇÃ·§Æû¿¡¼­¸¸ Àû¿ë
        //if (Target.Platform == UnrealTargetPlatform.Win64)
        //{

        //}

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
