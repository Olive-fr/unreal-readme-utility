
#include "ReadmeAssetEditorModule.h"

#include "Styles/ReadmeAssetEditorStyle.h"

#define LOCTEXT_NAMESPACE "FReadmeAssetEditorModule"
//#include "ReadmeAssetEditorPrivatePCH.h"


void FReadmeAssetEditorModule::StartupModule()
{
	Style = MakeShareable(new FReadmeAssetEditorStyle());

	//		FReadmeAssetEditorCommands::Register();

	RegisterAssetTools();
	RegisterMenuExtensions();
}

void FReadmeAssetEditorModule::ShutdownModule()
{
	UnregisterAssetTools();
	UnregisterMenuExtensions();
	
}

IMPLEMENT_MODULE(FReadmeAssetEditorModule, ReadmeAssetEditor);


#undef LOCTEXT_NAMESPACE
