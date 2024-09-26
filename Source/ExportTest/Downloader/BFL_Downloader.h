// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Http.h"
#include "HttpModule.h"
#include "../HttpErrorCode.h"
#include "BFL_Downloader.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUploadImageComplete , const FString& , Hash);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnUploadImageFailed , EHttpErrorCode , ErrorCode , const FString& , ErrorMessage);

/**
 * 
 */
UCLASS()
class EXPORTTEST_API UBFL_Downloader : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable , Category = "Upload" , meta = (
        SourceImagePaths = "C:/IMAGE/Image1.png, C:/IMAGE/Image2.png, ..."
    ))
    static void UploadImage(
        const TArray<FString>& SourceImagePaths ,
        const FOnUploadImageComplete& OnComplete ,
        const FOnUploadImageFailed& OnFailed);

private:
    static void OnRequestComplete(
        FHttpRequestPtr Request ,
        FHttpResponsePtr Response ,
        bool bSuccess ,
        FOnUploadImageComplete OnComplete ,
        FOnUploadImageFailed OnFailed
    );

public:
    static const FString UPLOAD_IMAGE_URL;
	
};
