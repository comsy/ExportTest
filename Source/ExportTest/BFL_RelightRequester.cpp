// comsy test
// 
// BFL_RelightRequester.cpp
#include "BFL_RelightRequester.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/FileHelper.h"
#include "HttpErrorCode.h"

const FString UBFL_RelightRequester::RELIGHT_IMAGE_URL = TEXT("https://sdk.beeble.ai/v1/relight-image");

void UBFL_RelightRequester::RelightImage(
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
    const FOnRelightImageFailed& OnFailed
)
{
    FHttpModule& HttpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpModule.CreateRequest();

    Request->SetURL(RELIGHT_IMAGE_URL);
    Request->SetVerb("POST");
    Request->SetHeader("x-api-key", ApiKey);

    FString Boundary = FString::Printf(TEXT("---------------------------%s"), *FGuid::NewGuid().ToString());
    Request->SetHeader("Content-Type", FString::Printf(TEXT("multipart/form-data; boundary=%s"), *Boundary));

    TArray<uint8> RequestContent;

    // Helper function to add form data
    auto AddFormData = [&RequestContent, &Boundary](const FString& Key, const FString& Value)
    {
        FString FormElement = FString::Printf(TEXT("--%s\r\nContent-Disposition: form-data; name=\"%s\"\r\n\r\n%s\r\n"), *Boundary, *Key, *Value);
        RequestContent.Append((uint8*)TCHAR_TO_UTF8(*FormElement), FormElement.Len());
    };

    // Helper function to add file data
    auto AddFileData = [&RequestContent, &Boundary](const FString& Key, const FString& FilePath)
    {
        FString FileName = FPaths::GetCleanFilename(FilePath);
        FString FormElement = FString::Printf(TEXT("--%s\r\nContent-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\nContent-Type: application/octet-stream\r\n\r\n"), *Boundary, *Key, *FileName);
        RequestContent.Append((uint8*)TCHAR_TO_UTF8(*FormElement), FormElement.Len());

        TArray<uint8> FileContent;
        if (FFileHelper::LoadFileToArray(FileContent, *FilePath))
        {
            RequestContent.Append(FileContent);
        }
        RequestContent.Append((uint8*)"\r\n", 2);
    };

    // Add form fields
    AddFileData("source_image", SourceImagePath);
    AddFileData("lighting_image", LightingImagePath);
    AddFormData("lighting_strength", FString::SanitizeFloat(LightingStrength));
    AddFormData("lighting_rotation_theta", FString::FromInt(LightingRotationTheta));
    AddFormData("lighting_rotation_phi", FString::FromInt(LightingRotationPhi));
    AddFormData("auto_key", AutoKey ? "true" : "false");
    AddFormData("preview", Preview ? "true" : "false");

    // Add closing boundary
    FString ClosingBoundary = FString::Printf(TEXT("--%s--\r\n"), *Boundary);
    RequestContent.Append((uint8*)TCHAR_TO_UTF8(*ClosingBoundary), ClosingBoundary.Len());

    Request->SetContent(RequestContent);

    Request->OnProcessRequestComplete().BindStatic(&UBFL_RelightRequester::OnRequestComplete, ResultImagePath, OnComplete, OnFailed);
    Request->ProcessRequest();
}

void UBFL_RelightRequester::OnRequestComplete(
    FHttpRequestPtr Request,
    FHttpResponsePtr Response,
    bool bSuccess,
    FString ResultImagePath,
    FOnRelightImageComplete OnComplete,
    FOnRelightImageFailed OnFailed
)
{
    if (bSuccess && Response.IsValid())
    {
        EHttpErrorCode ErrorCode = ConvertToHttpErrorCode(Response->GetResponseCode());
        if (ErrorCode == EHttpErrorCode::OK)
        {
            TArray<uint8> ResponseContent = Response->GetContent();
            if (FFileHelper::SaveArrayToFile(ResponseContent, *ResultImagePath))
            {
                OnComplete.ExecuteIfBound(true, ResultImagePath);
                return;
            }
        }
        else
        {
            // 실패 처리
            FString ErrorMessage = Response->GetContentAsString();
            OnFailed.ExecuteIfBound(ErrorCode, ErrorMessage);
        }
    }

    // 요청 자체가 실패한 경우
    OnFailed.ExecuteIfBound(EHttpErrorCode::None, TEXT("Request failed"));
}