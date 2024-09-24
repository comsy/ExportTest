// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Http.h"
#include "HttpModule.h"
#include "HttpErrorCode.h"
#include "BFL_RelightRequester.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnRelightImageComplete, bool, bSuccess, const FString&, ResultPath);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnRelightImageFailed, EHttpErrorCode, ErrorCode, const FString&, ErrorMessage);

/**
 * 
 */
UCLASS()
class EXPORTTEST_API UBFL_RelightRequester : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "Image Processing", meta = (
        ApiKey = "YOUR_DEFAULT_API_KEY",
        SourceImagePath = "C:/IMAGE/Image.png",
        ResultImagePath = "C:/IMAGE/result.png",
        LightingImagePath = "C:/IMAGE/Image.exr",
        LightingStrength = "1.0",
        LightingRotationTheta = "90.0",
        LightingRotationPhi = "90.0",
        AutoKey = "true",
        Preview = "false"
        ))
    static void RelightImage(
        const FString& ApiKey,
        const FString& SourceImagePath,
        const FString& ResultImagePath,
        const FString& LightingImagePath,
        float LightingStrength,
        int32 LightingRotationTheta,
        int32 LightingRotationPhi,
        bool AutoKey,
        bool Preview,
        const FOnRelightImageComplete& OnComplete,
        const FOnRelightImageFailed& OnFailed);

private:
    static void OnRequestComplete(
        FHttpRequestPtr Request,
        FHttpResponsePtr Response,
        bool bSuccess,
        FString ResultFileName,
        FOnRelightImageComplete OnComplete,
        FOnRelightImageFailed OnFailed
    );

public:
    static const FString RELIGHT_IMAGE_URL;

};
