/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */

#include "SReadmeAssetEditor.h"



#define LOCTEXT_NAMESPACE "SReadmeAssetEditor"


/* SReadmeAssetEditor interface
 *****************************************************************************/

SReadmeAssetEditor::~SReadmeAssetEditor()
{
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}


void SReadmeAssetEditor::Construct(const FArguments& InArgs, UReadmeAsset* InReadmeAsset, const TSharedRef<ISlateStyle>& InStyle)
{
	ReadmeAsset = InReadmeAsset;

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SAssignNew(EditableTextBox, SMultiLineEditableTextBox)
					.OnTextChanged(this, &SReadmeAssetEditor::HandleEditableTextBoxTextChanged)
					.OnTextCommitted(this, &SReadmeAssetEditor::HandleEditableTextBoxTextCommitted)
					.Text(ReadmeAsset->Text)
			]
	];

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddSP(this, &SReadmeAssetEditor::HandleReadmeAssetPropertyChanged);
}

void SReadmeAssetEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	// Collector.
}


/* SReadmeAssetEditor callbacks
 *****************************************************************************/

void SReadmeAssetEditor::HandleEditableTextBoxTextChanged(const FText& NewText)
{
	ReadmeAsset->MarkPackageDirty();

	// double TotalSeconds = FPlatformTime::Seconds();
	// if (TotalSeconds - Last)
}


void SReadmeAssetEditor::HandleEditableTextBoxTextCommitted(const FText& Comment, ETextCommit::Type CommitType)
{
	ReadmeAsset->SetText(EditableTextBox->GetText());
}


void SReadmeAssetEditor::HandleReadmeAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (Object == ReadmeAsset)
	{
		EditableTextBox->SetText(ReadmeAsset->Text);
	}
}


#undef LOCTEXT_NAMESPACE
