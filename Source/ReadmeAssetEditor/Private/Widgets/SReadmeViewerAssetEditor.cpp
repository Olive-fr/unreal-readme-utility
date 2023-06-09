// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SReadmeViewerAssetEditor.h"

#include <regex>

#include "ReadmeAssetEditorModule.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Internationalization/Regex.h"
#include "MD4C/md4c-html.h"
#include "Misc/FileHelper.h"
#include "Styling/SlateStyle.h"
#include "Windows/LiveCoding/Private/External/LC_FixedSizeString.h"


#define LOCTEXT_NAMESPACE "SReadmeViewerAssetEditor"


/* SReadmeViewerAssetEditor interface
 *****************************************************************************/

SReadmeViewerAssetEditor::~SReadmeViewerAssetEditor()
{
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
	ReadmeTextChangedDelegate.Unbind();
	ReadmeAsset->OnTextChanged.RemoveAll(this);
}


void SReadmeViewerAssetEditor::Construct(const FArguments& InArgs, UReadmeAsset* InReadmeAsset,
                                         const TSharedRef<ISlateStyle>& InStyle)
{
	ReadmeAsset = InReadmeAsset;
	
	ReadmeTextChangedDelegate.BindSP(this, &SReadmeViewerAssetEditor::ParseTextMarkdown);
	ReadmeAsset->OnTextChanged.Add(ReadmeTextChangedDelegate);

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SAssignNew(WebBrowser, SWebBrowser)
			.ShowControls(false)
		]
	];

	ParseTextMarkdown();

	FReadmeAssetEditorModule& RichTextModule = FModuleManager::GetModuleChecked<FReadmeAssetEditorModule>(
		TEXT("ReadmeAssetEditor"));
}


FString SReadmeViewerAssetEditor::GetCssHtmlBlock()
{
	const FString CSSFilePath = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("ReadmeUtility"), TEXT("Resources"), TEXT("Styles/markdown.css"));

	FString CssBlock = "";
	FString CssContent;
	if (FFileHelper::LoadFileToString(CssContent, *CSSFilePath))
	{
		CssBlock = "<style>" + CssContent + "</style>";
	}
	return CssBlock;
}

void SReadmeViewerAssetEditor::ParseTextMarkdown()
{
	FText& NewText = ReadmeAsset->Text;
	if (NewText.IsEmpty())
	{
		return;
	}
	// To call the Markdown parser, we first need to adapt the type of our text
	const FString NewTextString = NewText.ToString();
	const std::string ConvertedTextToMarkdownParser{TCHAR_TO_UTF8(*NewTextString)};

	MarkdownParserResult = "";

	// This function written in c, sources can be found here: https://github.com/mity/md4c
	// will fill the result by a callback function, HtmlParserReassembler in our case, in the TempResult variable
	if (const int MarkdownParserResultCode = md_html(ConvertedTextToMarkdownParser.c_str(), ConvertedTextToMarkdownParser.length(), HtmlParserReassembler, this, 0, MD_HTML_FLAG_XHTML); MarkdownParserResultCode == 1)
	{
		return;
	}
	
	const FString HtmlBodyContent = FString(UTF8_TO_TCHAR(MarkdownParserResult.c_str()));

	const FString CssBlock = GetCssHtmlBlock();

	const FString HtmlPageContent = "<head><meta charset=\"UTF-8\">" + CssBlock + "</head> <html><body>" + HtmlBodyContent + "</body></html>";

	WebBrowser->LoadString(HtmlPageContent, TEXT(""));
	WebBrowser->Reload();
}


void SReadmeViewerAssetEditor::HtmlParserReassembler(const MD_CHAR* Text, MD_SIZE size, void* userdata)
{
	SReadmeViewerAssetEditor* This = static_cast<SReadmeViewerAssetEditor*>(userdata);
	if (size > 0 && size < std::strlen(Text))
	{
		std::string str(Text);
		This->MarkdownParserResult.append(str.substr(0, size));
	}
	else
	{
		This->MarkdownParserResult.append(Text);
	}
	return;
}

#undef LOCTEXT_NAMESPACE
