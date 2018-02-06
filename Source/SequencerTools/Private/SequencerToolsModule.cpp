#include "SequencerTools/Private/SequencerToolsPrivatePCH.h"
#include "SequencerToolsBaseClass.h"
#include "PropertyEditorModule.h"
#include "LevelEditor.h"
#include "SequencerToolsCustomization.h"

#define LOCTEXT_NAMESPACE "DemoTools"

class FEditorExtensionsEditorModule : public IModuleInterface
{
public:
	// IMoudleInterface interface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End of IModuleInterface interface

	static void TriggerTool(UClass* ToolClass);
	static void CreateToolListMenu(class FMenuBuilder& MenuBuilder);
	static void OnToolWindowClosed(const TSharedRef<SWindow>& Window, USequencerToolsBaseClass* Instance);

	TSharedPtr<FUICommandList> CommandList;
};

void FEditorExtensionsEditorModule::StartupModule()
{
	// Register the details customizations
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
		PropertyModule.RegisterCustomClassLayout(TEXT("SequencerToolsBaseClass"), FOnGetDetailCustomizationInstance::CreateStatic(&FSequencerToolsCustomization::MakeInstance));
		PropertyModule.NotifyCustomizationModuleChanged();
	}

	CommandList = MakeShareable(new FUICommandList);

	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));

		CommandList->Append(LevelEditorModule.GetGlobalLevelEditorActions());


		struct Local
		{
			static void AddMenuCommands(FMenuBuilder& MenuBuilder)
			{
				MenuBuilder.AddSubMenu(LOCTEXT("SequencerTools", "Faust Tools"),
					LOCTEXT("SequencerToolsTooltip", "List of tools"),
					FNewMenuDelegate::CreateStatic(&FEditorExtensionsEditorModule::CreateToolListMenu)
					);
			}
		};

		TSharedRef<FExtender> MenuExtender(new FExtender());
		MenuExtender->AddMenuExtension(
			TEXT("LevelEditor"),
			EExtensionHook::After,
			CommandList.ToSharedRef(),
			FMenuExtensionDelegate::CreateStatic(&Local::AddMenuCommands));
		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

	}
}

void FEditorExtensionsEditorModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomClassLayout("MotionVector");
}

void FEditorExtensionsEditorModule::TriggerTool(UClass* ToolClass)
{
	USequencerToolsBaseClass* ToolInstance = NewObject<USequencerToolsBaseClass>(GetTransientPackage(), ToolClass);
	ToolInstance->AddToRoot();

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	TArray<UObject*> ObjectsToView;
	ObjectsToView.Add(ToolInstance);
	TSharedRef<SWindow> Window = PropertyModule.CreateFloatingDetailsView(ObjectsToView, /*bIsLockeable=*/ false);
	Window->SetTitle(FText::FromString(ToolClass->GetName()));
	Window->SetOnWindowClosed(FOnWindowClosed::CreateStatic(&FEditorExtensionsEditorModule::OnToolWindowClosed, ToolInstance));
}

void FEditorExtensionsEditorModule::CreateToolListMenu(class FMenuBuilder& MenuBuilder)
{
	for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
	{
		UClass* Class = *ClassIt;
		if (!Class->HasAnyClassFlags(CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_Abstract))
		{
			if (Class->IsChildOf(USequencerToolsBaseClass::StaticClass()))
			{
				FString FriendlyName = Class->GetName();
				FText MenuDescription = FText::Format(LOCTEXT("ToolMenuDescription", "{0}"), FText::FromString(FriendlyName));
				FText MenuTooltip = FText::Format(LOCTEXT("ToolMenuTooltip", "Execute the {0} tool"), FText::FromString(FriendlyName));

				FUIAction Action(FExecuteAction::CreateStatic(&FEditorExtensionsEditorModule::TriggerTool, Class));

				MenuBuilder.AddMenuEntry(
					MenuDescription,
					MenuTooltip,
					FSlateIcon(),
					Action);
			}
		}
	}
}

void FEditorExtensionsEditorModule::OnToolWindowClosed(const TSharedRef<SWindow>& Window, USequencerToolsBaseClass* Instance)
{
	Instance->RemoveFromRoot();
	Instance->OnToolClosed();
}

IMPLEMENT_MODULE(FEditorExtensionsEditorModule, DemoEditorExtensionsEditor);

#undef LOCTEXT_NAMESPACE