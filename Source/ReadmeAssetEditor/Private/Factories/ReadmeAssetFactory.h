/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */

#pragma once

#include "Factories/Factory.h"
#include "ReadmeAssetFactory.generated.h"


/**
 * Implements a factory for UReadmeAsset objects.
 */
UCLASS(hidecategories=Object)
class UReadmeAssetFactory
	: public UFactory
{
	GENERATED_UCLASS_BODY()

public:

	// UFactory Interface

	virtual UObject* FactoryCreateBinary(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn) override;
};
