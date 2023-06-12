/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */

#include "ReadmeAssetFactory.h"

#include "ReadmeAsset.h"
#include "Misc/FileHelper.h"


/* UReadmeAssetFactory structors
 *****************************************************************************/

UReadmeAssetFactory::UReadmeAssetFactory( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	Formats.Add(FString(TEXT("txt;")) + NSLOCTEXT("UReadmeAssetFactory", "FormatTxt", "Text File").ToString());
	Formats.Add(FString(TEXT("md;")) + NSLOCTEXT("UReadmeAssetFactory", "FormatMarkdown", "Readme File").ToString());
	SupportedClass = UReadmeAsset::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
}


/* UFactory overrides
 *****************************************************************************/

UObject* UReadmeAssetFactory::FactoryCreateBinary(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn)
{
	UReadmeAsset* ReadmeAsset = nullptr;
	FString TextString;

	if (FFileHelper::LoadFileToString(TextString, *CurrentFilename))
	{
		ReadmeAsset = NewObject<UReadmeAsset>(InParent, Class, Name, Flags);
		ReadmeAsset->Text = FText::FromString(TextString);
	}

	return ReadmeAsset;
}
