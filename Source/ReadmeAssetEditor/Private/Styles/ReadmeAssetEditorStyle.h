/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */

#pragma once

#include "Styling/SlateStyle.h"
#define TTF_FONT(RelativePath, ...) FSlateFontInfo(RootToContentDir(RelativePath, TEXT(".ttf")), __VA_ARGS__)
#define OTF_FONT(RelativePath, ...) FSlateFontInfo(RootToContentDir(RelativePath, TEXT(".otf")), __VA_ARGS__)
#include "EditorStyleSet.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"


/**
 * Implements the visual style of the text asset editor UI.
 */
class FReadmeAssetEditorStyle
	: public FSlateStyleSet
{
public:
	/** Default constructor. */
	FReadmeAssetEditorStyle()
		: FSlateStyleSet("ReadmeAssetEditorStyle")
	{
		const FVector2D Icon16x16(16.0f, 16.0f);
		const FVector2D Icon20x20(20.0f, 20.0f);
		const FVector2D Icon40x40(40.0f, 40.0f);

		const FString BaseDir = IPluginManager::Get().FindPlugin("ReadmeUtility")->GetBaseDir();
		SetContentRoot(BaseDir / TEXT("Content"));
		
		FSlateStyleRegistry::RegisterSlateStyle(*this);
	}

	/** Destructor. */
	~FReadmeAssetEditorStyle()
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*this);
	}
};

#undef TTF_FONT
#undef OTF_FONT
