// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once


#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BORDER_BRUSH(RelativePath, ...) FSlateBorderBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define TTF_FONT(RelativePath, ...) FSlateFontInfo(RootToContentDir(RelativePath, TEXT(".ttf")), __VA_ARGS__)
#define OTF_FONT(RelativePath, ...) FSlateFontInfo(RootToContentDir(RelativePath, TEXT(".otf")), __VA_ARGS__)
#include "EditorStyleSet.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleMacros.h"
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

		// toolbar icons
		//		Set("ReadmeAssetEditor.ForwardMedia", new IMAGE_BRUSH("icon_forward_40x", Icon40x40));

		const FTextBlockStyle NormalText = FAppStyle::GetWidgetStyle<FTextBlockStyle>("NormalText");
		Set("RichTextStyle.Title1", FTextBlockStyle(NormalText)
								   .SetFont(DEFAULT_FONT("Bold", FCoreStyle::RegularTextSize))
								   .SetFontSize(80)
								   .SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f))
								   .SetHighlightColor(FLinearColor(1.0f, 1.0f, 1.0f))
								   .SetShadowOffset(FVector2D(1, 1))
								   .SetShadowColorAndOpacity(FLinearColor(0, 0, 0, 0.9f)));
		Set("Text.Title2", FTextBlockStyle(NormalText)
								   .SetFont(DEFAULT_FONT("Bold", FCoreStyle::RegularTextSize))
								   .SetFontSize(60)
								   .SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f))
								   .SetHighlightColor(FLinearColor(1.0f, 1.0f, 1.0f))
								   .SetShadowOffset(FVector2D(1, 1))
								   .SetShadowColorAndOpacity(FLinearColor(0, 0, 0, 0.9f)));
		
		Set("Title3", FTextBlockStyle(NormalText)
								   .SetFont(DEFAULT_FONT("Bold", FCoreStyle::RegularTextSize))
								   .SetFontSize(40)
								   .SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f))
								   .SetHighlightColor(FLinearColor(1.0f, 1.0f, 1.0f))
								   .SetShadowOffset(FVector2D(1, 1))
								   .SetShadowColorAndOpacity(FLinearColor(0, 0, 0, 0.9f)));

		FSlateStyleRegistry::RegisterSlateStyle(*this);
	}

	/** Destructor. */
	~FReadmeAssetEditorStyle()
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*this);
	}
};


#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
