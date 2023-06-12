/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */
#pragma once

#include "CoreMinimal.h"
#include "UObject/UnrealType.h"
#include "ReadmeAsset.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnTextChangedSignature)

/**
 * Implements an asset that can be used to store arbitrary text, such as notes
 * or documentation.
 */
UCLASS(BlueprintType, hidecategories=(Object))
class READMEASSETEDITOR_API UReadmeAsset
	: public UObject
{
	GENERATED_BODY()

public:
	FOnTextChangedSignature OnTextChanged;

	void SetText(FText NewText)
	{
		Text = NewText;
		OnTextChanged.Broadcast();
	}
	
	/** Holds the stored text. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="ReadmeAsset")
	FText Text = FText::FromString("# Documentation");
};
