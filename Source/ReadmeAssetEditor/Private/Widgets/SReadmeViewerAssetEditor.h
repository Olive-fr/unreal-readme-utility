// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ReadmeAsset.h"
#include "MD4C/md4c.h"
#include "WebBrowser/Public/IWebBrowserWindow.h"
#include "WebBrowser/Public/SWebBrowserView.h"


/**
 * Implements the UReadmeAsset asset editor widget.
 */
class SReadmeViewerAssetEditor
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SReadmeViewerAssetEditor) { }
	SLATE_END_ARGS()

public:

	/** Virtual destructor. */
	virtual ~SReadmeViewerAssetEditor();

	/**
	 * Construct this widget
	 *
	 * @param InArgs The declaration data for this widget.
	 * @param InReadmeAsset The UReadmeAsset asset to edit.
	 * @param InStyleSet The style set to use.
	 */
	void Construct(const FArguments& InArgs, UReadmeAsset* InReadmeAsset, const TSharedRef<ISlateStyle>& InStyle);
	static void process_output(const MD_CHAR* text, MD_SIZE size, void* userdata);
	void ParseTextMarkdownAsync(FText& NewText);
	FString RegexReplace(FString target, FString pattern, FString substitution, bool ignoreCase, bool multiline);

	std::string TempResult;

private:

	/** Holds the editable text box widget. */
	TSharedPtr<SWebBrowser> WebBrowser;
	// IWebBrowserWindow sdfd;
	
	/** Pointer to the text asset that is being edited. */
	UReadmeAsset* ReadmeAsset;
};
