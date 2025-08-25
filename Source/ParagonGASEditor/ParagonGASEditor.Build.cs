using UnrealBuildTool;

public class ParagonGASEditor : ModuleRules
{
    public ParagonGASEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "UnrealEd",       // Required for editor-only stuff
                "AssetTools",     // Required for custom asset type actions
                "Slate",
                "SlateCore",
                "Projects",
                "ParagonGAS"      // Our runtime module
            }
        );
    }
}
