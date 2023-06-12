/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */

#pragma once
#include <string>

#include "Md4cLibraryInterface.h"
#include "ReadmeAsset.h"
#include "WebBrowser/Public/IWebBrowserWindow.h"
 	

#include "Delegates/DelegateSignatureImpl.inl"
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
	SReadmeViewerAssetEditor(): ReadmeAsset(nullptr) {};
	
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
	FString GetCssHtmlBlock();
	//static void HtmlParserReassembler(const MD_CHAR* text, MD_SIZE size, void* userdata);
	void ParseTextMarkdown();
	FString RegexReplace(FString target, FString pattern, FString substitution, bool ignoreCase, bool multiline);

private:

	/** Holds the editable text box widget. */
	TSharedPtr<SWebBrowser> WebBrowser;
	// IWebBrowserWindow sdfd;
	
	/** Pointer to the text asset that is being edited. */
	UReadmeAsset* ReadmeAsset;

	
	TDelegate<void(void)> ReadmeTextChangedDelegate;
	
	FMd4cInterface Md4cModuleInstance;
};
