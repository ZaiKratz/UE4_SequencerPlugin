using UnrealBuildTool;

public class SequencerTools : ModuleRules
{
	public SequencerTools(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"Slate",
				"UnrealEd",
                "Networking",
                "Sockets",
                "Core",
            "InputCore",
            "EditorStyle",
            "Slate",
            "RenderCore",
            "ShaderCore",
            "SlateCore",
            "LevelSequence"
            }
		);
		
		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"InputCore",
				"SlateCore",
				"PropertyEditor",
				"LevelEditor",
                "EditorStyle",
                "MovieScene",
                "MovieSceneTracks",
            }
		);

        PublicIncludePaths.AddRange(
               new string[] {
                    "SequencerTools/Public",
                    "Editor/UnrealEd/Public/UnrealEd",
                    "Developer/AssetTools/Public",
                    "Editor/UnrealEd/Public",
                    "Editor/UnrealEd/Classes",
                    "SequencerTools/Private",
                    "SequencerTools/Public",
                    

                   // ... add public include paths required here ...
               }
               );

        PrivateIncludePaths.AddRange(
                new string[] {
                    "SequencerTools/Private",
                    "Editor/UnrealEd/Public/UnrealEd",
                    "Editor/UnrealEd/Public",
                    "Editor/UnrealEd/Classes",
                    "Editor/UnrealEd/Private"
					// ... add other private include paths required here ...
				}
                );
    }
}