// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ReadmeAsset.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"


/**
 * Implements the UReadmeAsset asset editor widget.
 */
class SReadmeAssetEditor
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SReadmeAssetEditor) { }
	SLATE_END_ARGS()

public:

	/** Virtual destructor. */
	virtual ~SReadmeAssetEditor();

	/**
	 * Construct this widget
	 *
	 * @param InArgs The declaration data for this widget.
	 * @param InReadmeAsset The UReadmeAsset asset to edit.
	 * @param InStyleSet The style set to use.
	 */
	void Construct(const FArguments& InArgs, UReadmeAsset* InReadmeAsset, const TSharedRef<ISlateStyle>& InStyle);

	void AddReferencedObjects( FReferenceCollector& Collector );

	// float MIN_UPDATE_TIME = 1;
	// float LastUpdateTime = 0;
private:

	/** Callback for text changes in the editable text box. */
	void HandleEditableTextBoxTextChanged(const FText& NewText);

	/** Callback for committed text in the editable text box. */
	void HandleEditableTextBoxTextCommitted(const FText& Comment, ETextCommit::Type CommitType);

	/** Callback for property changes in the text asset. */
	void HandleReadmeAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent);

private:

	/** Holds the editable text box widget. */
	TSharedPtr<SMultiLineEditableTextBox> EditableTextBox;

	/** Pointer to the text asset that is being edited. */
	UReadmeAsset* ReadmeAsset;
};
