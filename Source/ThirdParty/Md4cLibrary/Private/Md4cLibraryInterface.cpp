/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */

#include "Md4cLibraryInterface.h"
#include "HAL/Platform.h"


#include <string>


FMd4cInterface::FMd4cInterface()
{
}

FMd4cInterface::~FMd4cInterface()
{
}


void HtmlParserReassembler(const MD_CHAR* Text, MD_SIZE size, void* userdata)
{
	FMd4cInterface* Md4CInterface = static_cast<FMd4cInterface*>(userdata);
	if (size > 0 && size < std::strlen(Text))
	{
		std::string str(Text);
		Md4CInterface->MarkdownParserResult.append(str.substr(0, size));
	}
	else
	{
		Md4CInterface->MarkdownParserResult.append(Text);
	}
	return;
}

void FMd4cInterface::ParseMarkdownAsHtml(FString& InMarkdown, FString& Result)
{
	const std::string ConvertedTextToMarkdownParser{TCHAR_TO_UTF8(*InMarkdown)};
	
	MarkdownParserResult = "";
	
	// This function written in c, sources can be found here: https://github.com/mity/md4c
	// will fill the result by a callback function, HtmlParserReassembler in our case, in the TempResult variable
	if (const int MarkdownParserResultCode = md_html(ConvertedTextToMarkdownParser.c_str(), 
			ConvertedTextToMarkdownParser.length(), HtmlParserReassembler,//HtmlParserReassembler,
			this,
			0,
			MD_HTML_FLAG_XHTML);
		MarkdownParserResultCode == 1)
	{
		return ;
	}

	Result = FString(UTF8_TO_TCHAR(MarkdownParserResult.c_str()));
}