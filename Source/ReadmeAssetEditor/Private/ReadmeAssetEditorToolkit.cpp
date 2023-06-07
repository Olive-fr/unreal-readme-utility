// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ReadmeAssetEditorToolkit.h"

#include "EditorReimportHandler.h"
#include "EditorStyleSet.h"
#include "Factories.h"
#include "ReadmeAsset.h"
//#include "SDockTab.h"
#include "Widgets/SReadmeAssetEditor.h"


#define LOCTEXT_NAMESPACE "FReadmeAssetEditorToolkit"

DEFINE_LOG_CATEGORY_STATIC(LogReadmeAssetEditor, Log, All);


/* Local constants
 *****************************************************************************/

static const FName ReadmeAssetEditorAppIdentifier("ReadmeAssetEditorApp");
static const FName TextEditorTabId("TextEditor");


/* FReadmeAssetEditorToolkit structors
 *****************************************************************************/

FReadmeAssetEditorToolkit::FReadmeAssetEditorToolkit(const TSharedRef<ISlateStyle>& InStyle)
	: ReadmeAsset(nullptr)
	, Style(InStyle)
{ }


FReadmeAssetEditorToolkit::~FReadmeAssetEditorToolkit()
{
	FReimportManager::Instance()->OnPreReimport().RemoveAll(this);
	FReimportManager::Instance()->OnPostReimport().RemoveAll(this);

	GEditor->UnregisterForUndo(this);
}


/* FReadmeAssetEditorToolkit interface
 *****************************************************************************/

void FReadmeAssetEditorToolkit::Initialize(UReadmeAsset* InReadmeAsset, const EToolkitMode::Type InMode, const TSharedPtr<class IToolkitHost>& InToolkitHost)
{
	ReadmeAsset = InReadmeAsset;

	// Support undo/redo
	ReadmeAsset->SetFlags(RF_Transactional);
	GEditor->RegisterForUndo(this);

	// create tab layout
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("Standalone_ReadmeAssetEditor")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
				->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewSplitter()
						->SetOrientation(Orient_Vertical)
						->SetSizeCoefficient(0.66f)
						->Split
						(
							FTabManager::NewStack()
								->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
								->SetHideTabWell(true)
								->SetSizeCoefficient(0.1f)
								
						)
						->Split
						(
							FTabManager::NewStack()
								->AddTab(TextEditorTabId, ETabState::OpenedTab)
								->SetHideTabWell(true)
								->SetSizeCoefficient(0.9f)
						)
				)
		);

	FAssetEditorToolkit::InitAssetEditor(
		InMode,
		InToolkitHost,
		ReadmeAssetEditorAppIdentifier,
		Layout,
		true /*bCreateDefaultStandaloneMenu*/,
		true /*bCreateDefaultToolbar*/,
		InReadmeAsset);

	RegenerateMenusAndToolbars();
}


/* FAssetEditorToolkit interface
 *****************************************************************************/

FString FReadmeAssetEditorToolkit::GetDocumentationLink() const
{
	return FString(TEXT("https://github.com/ue4plugins/ReadmeAsset"));
}


void FReadmeAssetEditorToolkit::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_ReadmeAssetEditor", "Text Asset Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(TextEditorTabId, FOnSpawnTab::CreateSP(this, &FReadmeAssetEditorToolkit::HandleTabManagerSpawnTab, TextEditorTabId))
		.SetDisplayName(LOCTEXT("TextEditorTabName", "Text Editor"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));
}


void FReadmeAssetEditorToolkit::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(TextEditorTabId);
}


/* IToolkit interface
 *****************************************************************************/

FText FReadmeAssetEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Text Asset Editor");
}


FName FReadmeAssetEditorToolkit::GetToolkitFName() const
{
	return FName("ReadmeAssetEditor");
}


FLinearColor FReadmeAssetEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}


FString FReadmeAssetEditorToolkit::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "ReadmeAsset ").ToString();
}


/* FGCObject interface
 *****************************************************************************/

void FReadmeAssetEditorToolkit::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(ReadmeAsset);
}


/* FEditorUndoClient interface
*****************************************************************************/

void FReadmeAssetEditorToolkit::PostUndo(bool bSuccess)
{ }


void FReadmeAssetEditorToolkit::PostRedo(bool bSuccess)
{
	PostUndo(bSuccess);
}


/* FReadmeAssetEditorToolkit callbacks
 *****************************************************************************/

TSharedRef<SDockTab> FReadmeAssetEditorToolkit::HandleTabManagerSpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier)
{
	TSharedPtr<SWidget> TabWidget = SNullWidget::NullWidget;

	if (TabIdentifier == TextEditorTabId)
	{
		TabWidget = SNew(SReadmeAssetEditor, ReadmeAsset, Style);
	}

	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			TabWidget.ToSharedRef()
		];
}


#undef LOCTEXT_NAMESPACE
