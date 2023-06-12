// Fill out your copyright notice in the Description page of Project Settings.
/*
 * Copyright (C) 2023 Olive, https://github.com/Olive-fr
 */
#pragma once

#include "CoreMinimal.h"
#include "md4c-html.h"
#include <string>

/**
 * 
 */
class MD4CLIBRARY_API FMd4cInterface
{
public:
	FMd4cInterface();
	~FMd4cInterface();

	
	void ParseMarkdownAsHtml(FString& InMarkdown, FString& OutResult);
	std::string MarkdownParserResult;


};
