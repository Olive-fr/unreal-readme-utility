// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SReadmeViewerAssetEditor.h"

#include <regex>

#include "ReadmeAssetEditorModule.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Internationalization/Regex.h"
#include "MD4C/md4c-html.h"
#include "Styling/SlateStyle.h"
#include "Windows/LiveCoding/Private/External/LC_FixedSizeString.h"


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

	ReadmeAsset->OnTextChanged.AddLambda([this]()
	{
		ParseTextMarkdownAsync(ReadmeAsset->Text);
	});

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

	ParseTextMarkdownAsync(ReadmeAsset->Text);

	
	FReadmeAssetEditorModule& RichTextModule = FModuleManager::GetModuleChecked<FReadmeAssetEditorModule>( TEXT( "ReadmeAssetEditor" ) );
	
	// Create the decorator style set


	// TextBox->SetContent(RichTextModule.Style.Get())
	// SetDecoratorStyleSet(RichTextModule.Style.Get());
}


void
SReadmeViewerAssetEditor::process_output(const MD_CHAR* Text, MD_SIZE size, void* userdata)
{
	SReadmeViewerAssetEditor* This = static_cast<SReadmeViewerAssetEditor*>(userdata);
	if (size > 0 && size < std::strlen(Text))
	{
		std::string str(Text);
		This->TempResult.append(str.substr(0, size));
		
	}else
	{
		This->TempResult.append(Text);
		
	}
	return;
}

void SReadmeViewerAssetEditor::ParseTextMarkdownAsync(FText& NewText)
{
	if (NewText.IsEmpty())
	{
		return ;
	}
	FString TextString = NewText.ToString();
	
	std::string s { TCHAR_TO_UTF8(*TextString) };

	TempResult = "";
	int MD_HTML = md_html(s.c_str(), s.length(), process_output,this, 0, MD_HTML_FLAG_XHTML);

	FString Contents = FString(UTF8_TO_TCHAR(TempResult.c_str()));

	Contents = "<head><meta charset=\"UTF-8\"></meta> " + Contents;

	
	WebBrowser->LoadString(Contents, TEXT(""));
	WebBrowser->Reload();
}


#undef LOCTEXT_NAMESPACE
