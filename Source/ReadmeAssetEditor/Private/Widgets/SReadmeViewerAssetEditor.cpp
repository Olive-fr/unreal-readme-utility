// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SReadmeViewerAssetEditor.h"



#define LOCTEXT_NAMESPACE "SReadmeViewerAssetEditor"


/* SReadmeViewerAssetEditor interface
 *****************************************************************************/

SReadmeViewerAssetEditor::~SReadmeViewerAssetEditor()
{
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}


void SReadmeViewerAssetEditor::Construct(const FArguments& InArgs, UReadmeAsset* InReadmeAsset, const TSharedRef<ISlateStyle>& InStyle)
{
	ReadmeAsset = InReadmeAsset;

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SAssignNew(EditableTextBox, SMultiLineEditableTextBox)
					.OnTextChanged(this, &SReadmeViewerAssetEditor::HandleEditableTextBoxTextChanged)
					.OnTextCommitted(this, &SReadmeViewerAssetEditor::HandleEditableTextBoxTextCommitted)
					.Text(ReadmeAsset->Text)
			]
	];

	EditableTextBox->SetIsReadOnly(true);

	

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddSP(this, &SReadmeViewerAssetEditor::HandleReadmeAssetPropertyChanged);
}


/* SReadmeViewerAssetEditor callbacks
 *****************************************************************************/

void SReadmeViewerAssetEditor::HandleEditableTextBoxTextChanged(const FText& NewText)
{
	ReadmeAsset->MarkPackageDirty();
}


void SReadmeViewerAssetEditor::HandleEditableTextBoxTextCommitted(const FText& Comment, ETextCommit::Type CommitType)
{
	ReadmeAsset->Text = EditableTextBox->GetText();
}


void SReadmeViewerAssetEditor::HandleReadmeAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (Object == ReadmeAsset)
	{
		EditableTextBox->SetText(ReadmeAsset->Text);
	}
}


#undef LOCTEXT_NAMESPACE
