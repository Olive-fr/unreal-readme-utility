/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */

#include "ReadmeEditorCommands.h"

#include "Framework/Commands/InputChord.h"
#include "Framework/Commands/UICommandInfo.h"

#define LOCTEXT_NAMESPACE "ReadmeEditorCommands"
void FReadmeEditorCommands::RegisterCommands()
{
	UI_COMMAND(RedChannel, "Red", "Toggles the red channel", EUserInterfaceActionType::ToggleButton, FInputChord());
}
#undef LOCTEXT_NAMESPACE
