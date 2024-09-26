// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_Print.generated.h"

/**
 * 
 */
UCLASS()
class EXPORTTEST_API UBFL_Print : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable , Category = "Printing")
	static bool PrintImage(const FString& ImageFilePath);
};
