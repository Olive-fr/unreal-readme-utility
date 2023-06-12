/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */

using UnrealBuildTool;
using System.IO;

public class ReadmeAssetEditor : ModuleRules
{
	public ReadmeAssetEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
			}
		);
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"WebBrowser",
				"UMG",
				"ApplicationCore",
				"Md4cLibrary"
			}
		);
		PrivateIncludePaths.AddRange(
			new string[] {
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
				"UnrealEd",
			}
		);

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"AssetTools",
				"UnrealEd",
			}
		);
	}
}
