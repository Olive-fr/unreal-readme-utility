// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ReadmeAssetEditorPrivatePCH.h"
#include "ReadmeAssetFactoryNew.h"


/* UReadmeAssetFactoryNew structors
 *****************************************************************************/

UReadmeAssetFactoryNew::UReadmeAssetFactoryNew(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UReadmeAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}


/* UFactory overrides
 *****************************************************************************/

UObject* UReadmeAssetFactoryNew::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UReadmeAsset>(InParent, InClass, InName, Flags);
}


bool UReadmeAssetFactoryNew::ShouldShowInNewMenu() const
{
	return true;
}
