// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ReadmeAssetEditor : ModuleRules
{
	public ReadmeAssetEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
		);
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "AssetTools",
				// ... add other public dependencies that you statically link with here ...
			}
		);
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// "ReadmeAssetEditor/Private",
				// "ReadmeAssetEditor/Private/AssetTools",
				// "ReadmeAssetEditor/Private/Factories",
				// "ReadmeAssetEditor/Private/Styles",
				// "ReadmeAssetEditor/Private/Widgets",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"ContentBrowser",
				"Core",
				"CoreUObject",
                "DesktopWidgets",
				"EditorStyle",
				"Engine",
				"InputCore",
                "Projects",
				"Slate",
				"SlateCore",
                //"ReadmeAsset",
				"UnrealEd",
			}
		);

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"AssetTools",
				"UnrealEd",
//				"WorkspaceMenuStructure",
			}
		);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
				// "AssetTools",
				// "MainFrame",
				
				
//				"WorkspaceMenuStructure",
			}
		);
	}
}
