// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ReadmeAssetEditorPrivatePCH.h"
#include "ContentBrowserModule.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"


/* FReadmeAssetActions constructors
 *****************************************************************************/

FReadmeAssetActions::FReadmeAssetActions(const TSharedRef<ISlateStyle>& InStyle)
	: Style(InStyle)
{ }


/* FAssetTypeActions_Base overrides
 *****************************************************************************/

bool FReadmeAssetActions::CanFilter()
{
	return true;
}


void FReadmeAssetActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);

	auto ReadmeAssets = GetTypedWeakObjectPtrs<UReadmeAsset>(InObjects);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ReadmeAsset_ReverseText", "Reverse Text"),
		LOCTEXT("ReadmeAsset_ReverseTextToolTip", "Reverse the text stored in the selected text asset(s)."),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([=]{
				for (auto& ReadmeAsset : ReadmeAssets)
				{
					if (ReadmeAsset.IsValid() && !ReadmeAsset->Text.IsEmpty())
					{
						ReadmeAsset->Text = FText::FromString(ReadmeAsset->Text.ToString().Reverse());
						ReadmeAsset->PostEditChange();
					}
				}
			}),
			FCanExecuteAction::CreateLambda([=] {
				for (auto& ReadmeAsset : ReadmeAssets)
				{
					if (ReadmeAsset.IsValid() && !ReadmeAsset->Text.IsEmpty())
					{
						return true;
					}
				}
				return false;
			})
		)
	);
}


uint32 FReadmeAssetActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}


FText FReadmeAssetActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_ReadmeAsset", "Text Asset");
}


UClass* FReadmeAssetActions::GetSupportedClass() const
{
	return UReadmeAsset::StaticClass();
}


FColor FReadmeAssetActions::GetTypeColor() const
{
	return FColor::White;
}


bool FReadmeAssetActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}


void FReadmeAssetActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid()
		? EToolkitMode::WorldCentric
		: EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		auto ReadmeAsset = Cast<UReadmeAsset>(*ObjIt);

		if (ReadmeAsset != nullptr)
		{
			TSharedRef<FReadmeAssetEditorToolkit> EditorToolkit = MakeShareable(new FReadmeAssetEditorToolkit(Style));
			EditorToolkit->Initialize(ReadmeAsset, Mode, EditWithinLevelEditor);
		}
	}
}


#undef LOCTEXT_NAMESPACE
