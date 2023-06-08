// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SReadmeViewerAssetEditor.h"

#include <regex>

#include "ReadmeAssetEditorModule.h"
#include "Internationalization/Regex.h"
#include "Styling/SlateStyle.h"
#include "Widgets/Text/SRichTextBlock.h"


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
		TextBox->SetText(TransformText(ReadmeAsset->Text));
	});

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SAssignNew(TextBox, SRichTextBlock)
					.Text(TransformText(ReadmeAsset->Text))
			]
	];


	
	FReadmeAssetEditorModule& RichTextModule = FModuleManager::GetModuleChecked<FReadmeAssetEditorModule>( TEXT( "ReadmeAssetEditor" ) );
	
	// Create the decorator style set


	TextBox->SetDecoratorStyleSet(RichTextModule.Style.Get());

	// FTextBlockStyleSet& StyleSet = FTextBlockStyleSet::GetDefault();
	//
	// // Style 1
	// FTextBlockStyle& Style1 = StyleSet.GetOrCreateStyle(TEXT("Style1"));
	// Style1.SetFont(FSlateFontInfo("Arial", 16));
	// Style1.SetColorAndOpacity(FLinearColor::Red);
	//
	// // Style 2
	// FTextBlockStyle& Style2 = StyleSet.GetOrCreateStyle(TEXT("Style2"));
	// Style2.SetFont(FSlateFontInfo("Verdana", 18));
	// Style2.SetColorAndOpacity(FLinearColor::Blue);
	//
	// TextBox->SetSt
}

FText SReadmeViewerAssetEditor::TransformText(FText& NewText)
{
	if (NewText.IsEmpty())
	{
		return FText();
	}
	FString TextString = NewText.ToString();
	// ^             #  Represents beginning of a line
	// [a-z]         #  Alphabetic character
	// .*            #  Any character 0 or more times
	// [a-z]         #  Alphabetic character
	// $             #  End of a line
	// i             #  Case-insensitive match
	// g             #  Global
	// m             #  Multiline

	TextString = RegexReplace(TextString, "^#### (.*)", "<Title4>$1</>", false, false);
	TextString = RegexReplace(TextString, "^### (.*)", "<Title3>$1</>", false, false);
	TextString = RegexReplace(TextString, "^## (.*)", "<Title2>$1</>", false, false);
	TextString = RegexReplace(TextString, "^# (.*)", "<Title1>$1</>", false, false);

	// Italic and blod

	// citation 

	// paragraph

	// List unordered - , + , *    ordered 1. , 1)
	//   Nested list: ident with two spaces more minimum

	// 3 spaces or more add a tab

	// Escape \

	// Links <link>, [blablabla](link), [bleblabla][x] .....   [x]:link

	// Images ![blabla](link), ![blabla][1] ....  [1]: link "title"

	// Code inline `code`, multiline 4spaces indent, multiline ``` code ```

	return FText::FromString(TextString);
}

FString SReadmeViewerAssetEditor::RegexReplace(FString target, FString pattern, FString substitution, bool ignoreCase, bool multiline = false) {
	std::string s { TCHAR_TO_UTF8(*target) };
	std::string ns { TCHAR_TO_UTF8(*substitution) };
	// The initialization is to have empty flag
	std::regex::flag_type flag = std::regex_constants::ECMAScript & std::regex_constants::icase ;
	bool useFlag = ignoreCase || multiline;
	if (useFlag)
	{
		if (ignoreCase)
		{
			flag = flag | std::regex_constants::icase;
		}
		if (multiline)
		{
			flag = flag | std::regex_constants::extended;
		}
	}
	std::regex p { useFlag ? std::regex { TCHAR_TO_UTF8(*pattern), flag } : std::regex { TCHAR_TO_UTF8(*pattern) } };
	
	std::string resultString { std::regex_replace(s, p, ns) };
	
	return  FString { UTF8_TO_TCHAR(resultString.c_str()) };
}


#undef LOCTEXT_NAMESPACE
