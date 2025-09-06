using UnrealBuildTool;

public class ParagonGASEditor : ModuleRules
{
    public ParagonGASEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[] {
            "Core",
            "CoreUObject",
            "Engine",
            "GraphEditor",
            "PropertyEditor"
        });

        PrivateDependencyModuleNames.AddRange(new[] {
            "Slate",
            "SlateCore",
            "UnrealEd",
            "AssetTools",
            "AssetRegistry",
            "ToolMenus",
            "ContentBrowser",
            "Kismet",
            "Projects",
            "InputCore",
            "EditorSubsystem",
            "ParagonGAS",
            "KismetCompiler",
            "KismetWidgets",
            "BlueprintGraph"
        });
    }
}
