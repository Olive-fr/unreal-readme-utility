/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */

#include "ReadmeAssetEditorModule.h"

#include "AssetTools/ReadmeEditorCommands.h"
#include "Styles/ReadmeAssetEditorStyle.h"

#define LOCTEXT_NAMESPACE "FReadmeAssetEditorModule"
//#include "ReadmeAssetEditorPrivatePCH.h"


void FReadmeAssetEditorModule::StartupModule()
{
	Style = MakeShareable(new FReadmeAssetEditorStyle());

	FReadmeEditorCommands::Register();

	RegisterAssetTools();
	RegisterMenuExtensions();
}

void FReadmeAssetEditorModule::ShutdownModule()
{
	UnregisterAssetTools();
	UnregisterMenuExtensions();

	Style.Reset();
}

void FReadmeAssetEditorModule::RegisterAssetTools()

{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	RegisterAssetTypeAction(AssetTools, MakeShareable(new FReadmeAssetActions(Style.ToSharedRef())));
}

void FReadmeAssetEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	RegisteredAssetTypeActions.Add(Action);
}

void FReadmeAssetEditorModule::RegisterMenuExtensions()
{
	MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
	ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);
}

IMPLEMENT_MODULE(FReadmeAssetEditorModule, ReadmeAssetEditor);


#undef LOCTEXT_NAMESPACE
