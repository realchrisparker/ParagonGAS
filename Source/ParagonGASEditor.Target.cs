// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ParagonGASEditorTarget : TargetRules
{
	public ParagonGASEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		// CppStandard = CppStandardVersion.Cpp20;
		// ExtraModuleNames.Add("ParagonGAS");
		ExtraModuleNames.AddRange(new string[] { "ParagonGAS" });
	}
}
