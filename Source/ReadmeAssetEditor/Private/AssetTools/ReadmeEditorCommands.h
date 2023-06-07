#pragma once

#include "Framework/Commands/Commands.h"
#include "Internationalization/Internationalization.h"
#include "Styling/AppStyle.h"
#include "Templates/SharedPointer.h"
#include "UObject/NameTypes.h"
#include "UObject/UnrealNames.h"

class FUICommandInfo;

class FReadmeEditorCommands: public TCommands<FReadmeEditorCommands>
{
public:

	/**
	 * Default constructor.
	 */
	FReadmeEditorCommands( ) 
		: TCommands<FReadmeEditorCommands>("ReadmeEditor", NSLOCTEXT("Contexts", "ReadmeEditor", "Readme Editor"), NAME_None, FAppStyle::GetAppStyleSetName())
	{ }

public:

	// TCommands interface

	virtual void RegisterCommands() override;
	
public:

	/** Toggles the red channel */
	TSharedPtr<FUICommandInfo> RedChannel;
};
