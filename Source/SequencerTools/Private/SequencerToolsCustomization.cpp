#include "SequencerTools/Private/SequencerToolsPrivatePCH.h"
#include "SequencerToolsCustomization.h"
#include "PropertyEditing.h"

#define LOCTEXT_NAMESPACE "EditorTool"

void FSequencerToolsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TSet<UClass*> Classes;

	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
	DetailBuilder.GetObjectsBeingCustomized(ObjectsBeingCustomized);

	for (auto WeakObject : ObjectsBeingCustomized)
	{
		if (UObject* Instance = WeakObject.Get())
		{
			Classes.Add(Instance->GetClass());
		}
	}

	IDetailCategoryBuilder& Parameters = DetailBuilder.EditCategory("Parameters");

	for (UClass* Class : Classes)
	{
		for (TFieldIterator<UFunction> FuncIt(Class); FuncIt; ++FuncIt)
		{
			UFunction* Function = *FuncIt;
			if (Function->HasAnyFunctionFlags(FUNC_Exec) && (Function->NumParms == 0))
			{
				const FString FunctionName = Function->GetName();
				const FText ButtonCaption = FText::FromString(FunctionName);
				const FText FilteringString = FText::FromString(FunctionName);
				const TAttribute <FText> ToolTip = FText::Format(LOCTEXT("ToolTipText", "{0}"), Function->GetToolTipText());
				if (FunctionName == "CaptureRange" || FunctionName == "ResetCapture")
				{
					Parameters.AddCustomRow(FilteringString)
						.ValueContent()
						[
							SNew(SButton)
							.Text(ButtonCaption)
						.ToolTipText(ToolTip)
						.OnClicked(FOnClicked::CreateStatic(&FSequencerToolsCustomization::ExecuteToolCommand, &DetailBuilder, Function))
						];
				}
			}
		}
	}
}

TSharedRef<IDetailCustomization> FSequencerToolsCustomization::MakeInstance()
{
	return MakeShareable(new FSequencerToolsCustomization);
}

FReply FSequencerToolsCustomization::ExecuteToolCommand(IDetailLayoutBuilder* DetailBuilder, UFunction* MethodToExecute)
{
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
	DetailBuilder->GetObjectsBeingCustomized(ObjectsBeingCustomized);

	for (auto WeakObject : ObjectsBeingCustomized)
	{
		if (UObject* Instance = WeakObject.Get())
		{
			Instance->CallFunctionByNameWithArguments(*MethodToExecute->GetName(), *GLog, nullptr, true);
		}
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE