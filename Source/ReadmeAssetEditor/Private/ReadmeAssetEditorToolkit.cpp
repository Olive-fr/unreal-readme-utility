/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */
#include "ReadmeAssetEditorToolkit.h"

#include "EditorReimportHandler.h"
#include "EditorStyleSet.h"
#include "ReadmeAsset.h"
#include "Widgets/Docking/SDockTab.h"
#include <Windows.ApplicationModel.Activation.h>

#include "ReadmeAssetEditorModule.h"
#include "AssetTools/ReadmeEditorCommands.h"
#include "Compression/OodleDataCompression.h"
#include "Settings/ProjectPackagingSettings.h"
#include "Widgets/SReadmeAssetEditor.h"


#define LOCTEXT_NAMESPACE "FReadmeAssetEditorToolkit"

DEFINE_LOG_CATEGORY_STATIC(LogReadmeAssetEditor, Log, All);


/* Local constants
 *****************************************************************************/

static const FName ReadmeAssetEditorAppIdentifier("ReadmeAssetEditorApp");
static const FName ReadmeEditorTabId("ReadmeAssetEditor_Editor");
static const FName ReadmeViewerTabId("ReadmeAssetEditor_Viewer");


/* FReadmeAssetEditorToolkit structors
 *****************************************************************************/

FReadmeAssetEditorToolkit::FReadmeAssetEditorToolkit(const TSharedRef<ISlateStyle>& InStyle)
	: ReadmeAsset(nullptr)
	  , Style(InStyle)
	  , bIsEditing(false)
	  , ReadmeEditor(nullptr)
	  , ReadmeViewerEditor(nullptr)
{
}


FReadmeAssetEditorToolkit::~FReadmeAssetEditorToolkit()
{
	FReimportManager::Instance()->OnPreReimport().RemoveAll(this);
	FReimportManager::Instance()->OnPostReimport().RemoveAll(this);

	GEditor->UnregisterForUndo(this);
}

void FReadmeAssetEditorToolkit::CreateInternalWidget()
{
	ReadmeEditor = SNew(SReadmeAssetEditor, ReadmeAsset, Style);

	ReadmeViewerEditor = SNew(SReadmeViewerAssetEditor, ReadmeAsset, Style);
}


/* FReadmeAssetEditorToolkit interface
 *****************************************************************************/

void FReadmeAssetEditorToolkit::Initialize(UReadmeAsset* InReadmeAsset, const EToolkitMode::Type InMode,
                                           const TSharedPtr<class IToolkitHost>& InToolkitHost)
{
	ReadmeAsset = InReadmeAsset;

	// Support undo/redo
	ReadmeAsset->SetFlags(RF_Transactional);
	GEditor->RegisterForUndo(this);

	FReadmeEditorCommands::Register();

	const FReadmeEditorCommands& Commands = FReadmeEditorCommands::Get();

	ToolkitCommands->MapAction(
		Commands.RedChannel,
		FExecuteAction::CreateSP(this, &FReadmeAssetEditorToolkit::OnCanEdit),
		FCanExecuteAction());

	CreateInternalWidget();

	// create tab layout
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("Standalone_ReadmeAssetEditor_Layout_v3")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(ReadmeEditorTabId, ETabState::OpenedTab)
				// ->AddTab(OodleTabId, ETabState::OpenedTab)
				->SetForegroundTab(ReadmeEditorTabId)
				->SetHideTabWell(true)
				->SetSizeCoefficient(0.5f)
			)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(ReadmeViewerTabId, ETabState::OpenedTab)
				// ->AddTab(OodleTabId, ETabState::OpenedTab)
				->SetForegroundTab(ReadmeViewerTabId)
				->SetHideTabWell(true)
				->SetSizeCoefficient(0.5f)
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


	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateSP(this, &FReadmeAssetEditorToolkit::FillToolbar)
	);

	AddToolbarExtender(ToolbarExtender);

	//
	FReadmeAssetEditorModule* TextureEditorModule = &FModuleManager::LoadModuleChecked<FReadmeAssetEditorModule>(
		"ReadmeAssetEditor");
	AddToolbarExtender(
		TextureEditorModule->GetToolBarExtensibilityManager()->GetAllExtenders(
			GetToolkitCommands(), GetEditingObjects()));

	RegenerateMenusAndToolbars();

	OnCanEdit();
}

FSlateColor FReadmeAssetEditorToolkit::GetIsEditingButtonBackgroundColor() const
{
	FSlateColor Dropdown = FAppStyle::Get().GetSlateColor("Colors.Dropdown");
	return bIsEditing ? FLinearColor::Blue : FLinearColor::White;
}

FSlateColor FReadmeAssetEditorToolkit::GetIsEditingButtonForegroundColor() const
{
	FSlateColor DefaultForeground = FAppStyle::Get().GetSlateColor("Colors.Foreground");
	return bIsEditing ? FLinearColor::Black : DefaultForeground;
}

ECheckBoxState FReadmeAssetEditorToolkit::OnGetIsEditingButtonCheckState() const
{
	return bIsEditing ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

bool FReadmeAssetEditorToolkit::IsIsEditingButtonEnabled() const
{
	return true;
}

TSharedRef<SWidget> FReadmeAssetEditorToolkit::MakeCanEditWidget()
{
	auto OnChannelCheckStateChanged = [this](ECheckBoxState NewState)
	{
		bIsEditing = !bIsEditing;
		OnCanEdit();
	};

	TSharedRef<SWidget> ChannelControl =
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		  .VAlign(VAlign_Center)
		  .Padding(2.0f)
		  .AutoWidth()
		[
			SNew(SCheckBox)
			.Style(FAppStyle::Get(), "ReadmeEditor.IsEditingButtonStyle")
			.BorderBackgroundColor(this, &FReadmeAssetEditorToolkit::GetIsEditingButtonBackgroundColor)
			.ForegroundColor(this, &FReadmeAssetEditorToolkit::GetIsEditingButtonForegroundColor)
			.OnCheckStateChanged_Lambda(OnChannelCheckStateChanged)
			.IsChecked(this, &FReadmeAssetEditorToolkit::OnGetIsEditingButtonCheckState)
			.IsEnabled(this, &FReadmeAssetEditorToolkit::IsIsEditingButtonEnabled)
			[
				SNew(STextBlock)
				.Font(FAppStyle::Get().GetFontStyle("ReadmeEditor.IsEditingButtonFont"))
				.Text(FText::FromString("Edit"))
			]
		];

	return ChannelControl;
}

void FReadmeAssetEditorToolkit::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
	TSharedRef<SWidget> ChannelControl = MakeCanEditWidget();


	ToolbarBuilder.BeginSection("ViewMode");
	{
		ToolbarBuilder.AddWidget(ChannelControl);
	}
	ToolbarBuilder.EndSection();
}

void FReadmeAssetEditorToolkit::OnCanEdit()
{
	TSharedPtr<SDockTab> DockTab = TabManager->FindExistingLiveTab(ReadmeEditorTabId);
	if (bIsEditing && !DockTab.IsValid())
	{
		//DockTab->SetVisibility(bIsEditing?EVisibility::Visible:EVisibility::Collapsed);
		//DockTab->Invalidate(EInvalidateWidgetReason::Visibility);
			TabManager->TryInvokeTab(ReadmeEditorTabId);
		
	} else if (!bIsEditing && DockTab.IsValid())
	{
			DockTab->RequestCloseTab();
	}
}

/* FAssetEditorToolkit interface
 *****************************************************************************/

FString FReadmeAssetEditorToolkit::GetDocumentationLink() const
{
	return FString(TEXT("https://github.com/Olive-fr/unreal-readme-utility"));
}

void FReadmeAssetEditorToolkit::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(
		LOCTEXT("WorkspaceMenu_ReadmeAssetEditor", "Readme Asset Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(ReadmeEditorTabId,
	                                 FOnSpawnTab::CreateSP(
		                                 this, &FReadmeAssetEditorToolkit::HandleTabManagerSpawnEditorTab))
	            .SetDisplayName(LOCTEXT("ReadmeEditorTab", "Editor"))
	            .SetGroup(WorkspaceMenuCategoryRef)
	            .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Viewports"));

	InTabManager->RegisterTabSpawner(ReadmeViewerTabId,
	                                 FOnSpawnTab::CreateSP(
		                                 this, &FReadmeAssetEditorToolkit::HandleTabManagerSpawnViewerTab))
	            .SetDisplayName(LOCTEXT("ReadmeViewerTab", "Viewer"))
	            .SetGroup(WorkspaceMenuCategoryRef)
	            .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Viewports"));
}


void FReadmeAssetEditorToolkit::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(ReadmeEditorTabId);
}


/* IToolkit interface
 *****************************************************************************/

FText FReadmeAssetEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Readme Asset Editor");
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
	// ReadmeEditor->AddR
}


/* FEditorUndoClient interface
*****************************************************************************/

void FReadmeAssetEditorToolkit::PostUndo(bool bSuccess)
{
}


void FReadmeAssetEditorToolkit::PostRedo(bool bSuccess)
{
	PostUndo(bSuccess);
}


/* FReadmeAssetEditorToolkit callbacks
 *****************************************************************************/

TSharedRef<SDockTab> FReadmeAssetEditorToolkit::HandleTabManagerSpawnEditorTab(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == ReadmeEditorTabId);

	return SNew(SDockTab)
		.Label(LOCTEXT("ReadmeEditorTab", "Editor"))
		[
			ReadmeEditor.ToSharedRef()
		];
}

TSharedRef<SDockTab> FReadmeAssetEditorToolkit::HandleTabManagerSpawnViewerTab(const FSpawnTabArgs& SpawnTabArgs)
{
	check(SpawnTabArgs.GetTabId() == ReadmeViewerTabId);

	return SNew(SDockTab)
		.Label(LOCTEXT("ReadmeViewerTab", "Viewer"))
		[
			ReadmeViewerEditor.ToSharedRef()
		];
}


#undef LOCTEXT_NAMESPACE
