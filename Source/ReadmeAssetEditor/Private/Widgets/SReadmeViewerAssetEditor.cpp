/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */

#include "SReadmeViewerAssetEditor.h"

#include "ReadmeAssetEditorModule.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Internationalization/Regex.h"
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

	FString InMarkdown = NewText.ToString();
	FString HtmlBodyContent = FString();
	Md4cModuleInstance.ParseMarkdownAsHtml(InMarkdown, HtmlBodyContent);

	const FString CssBlock = GetCssHtmlBlock();

	const FString HtmlPageContent = "<head><meta charset=\"UTF-8\">" + CssBlock + "</head> <html><body>" + HtmlBodyContent + "</body></html>";

	WebBrowser->LoadString(HtmlPageContent, TEXT(""));
	WebBrowser->Reload();
}

#undef LOCTEXT_NAMESPACE
