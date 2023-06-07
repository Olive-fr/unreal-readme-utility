// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Toolkits/AssetEditorToolkit.h"
#include "EditorUndoClient.h"
#include "ReadmeAsset.h"


/**
 * Implements an Editor toolkit for textures.
 */
class FReadmeAssetEditorToolkit
	: public FAssetEditorToolkit
	, public FEditorUndoClient
	, public FGCObject
{
public:

	/**
	 * Creates and initializes a new instance.
	 *
	 * @param InStyle The style set to use.
	 */
	FReadmeAssetEditorToolkit(const TSharedRef<ISlateStyle>& InStyle);

	/** Virtual destructor. */
	virtual ~FReadmeAssetEditorToolkit();

public:

	/**
	 * Initializes the editor tool kit.
	 *
	 * @param InReadmeAsset The UReadmeAsset asset to edit.
	 * @param InMode The mode to create the toolkit in.
	 * @param InToolkitHost The toolkit host.
	 */
	void Initialize(UReadmeAsset* InReadmeAsset, const EToolkitMode::Type InMode, const TSharedPtr<class IToolkitHost>& InToolkitHost);

public:

	// FAssetEditorToolkit interface

	virtual FString GetDocumentationLink() const override;
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

public:

	// IToolkit interface

	virtual FText GetBaseToolkitName() const override;
	virtual FName GetToolkitFName() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;

public:

	// FGCObject interface

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	
protected:

	// FEditorUndoClient interface

	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;

private:

	/** Callback for spawning the Properties tab. */
	TSharedRef<SDockTab> HandleTabManagerSpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier);

private:

	/** The text asset being edited. */
	UReadmeAsset* ReadmeAsset;

	/** Pointer to the style set to use for toolkits. */
	TSharedRef<ISlateStyle> Style;
};
