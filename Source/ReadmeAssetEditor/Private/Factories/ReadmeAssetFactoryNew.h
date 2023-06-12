/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */

#pragma once

#include "Factories/Factory.h"
#include "ReadmeAssetFactoryNew.generated.h"


/**
 * Implements a factory for UReadmeAsset objects.
 */
UCLASS(hidecategories=Object)
class UReadmeAssetFactoryNew
	: public UFactory
{
	GENERATED_UCLASS_BODY()

public:

	// UFactory Interface

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool ShouldShowInNewMenu() const override;
};
