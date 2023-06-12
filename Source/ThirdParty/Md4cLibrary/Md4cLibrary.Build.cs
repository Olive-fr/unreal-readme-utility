/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */

using System.Collections.Generic;
using UnrealBuildTool;
using System.IO;
using System;

public class Md4cLibrary : ModuleRules
{
	public Md4cLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
        Type = ModuleType.CPlusPlus;
        
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

        PrivateDependencyModuleNames.AddRange(
	        new string[] {
		        "Core",
		        "CoreUObject",
		        "Engine",
		        "Projects",
	        }
        );
	}
}
