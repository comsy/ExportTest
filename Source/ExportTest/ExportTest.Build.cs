// Fill out your copyright notice in the Description page of Project Settings.

//using System.IO;
//using System;
using UnrealBuildTool;

public class ExportTest : ModuleRules
{
	public ExportTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "HTTP" , "Json", "JsonUtilities" });

        // �߰�: Windows �÷��������� ����
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicSystemLibraries.Add("gdiplus.lib");
            PublicAdditionalLibraries.Add("Comdlg32.lib");

            // �ʿ��� ��� �߰����� ��Ŭ��� ��� ����
            //PublicIncludePaths.Add(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86), "Windows Kits", "10", "Include", "um"));
        }

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
