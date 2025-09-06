// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ParagonGASTarget : TargetRules
{
	public ParagonGASTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		// CppStandard = CppStandardVersion.Cpp20;
		// ExtraModuleNames.Add("ParagonGAS");
		ExtraModuleNames.AddRange(new string[] { "ParagonGAS" });
	}
}
