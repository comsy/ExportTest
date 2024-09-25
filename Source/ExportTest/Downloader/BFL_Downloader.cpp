// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL_Downloader.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/FileHelper.h"
#include "../HttpErrorCode.h"
#include "Json.h"

const FString UBFL_Downloader::UPLOAD_IMAGE_URL = TEXT("https://dev-dl.klippbox.com/api/contents/upload");

void UBFL_Downloader::UploadImage(
    const FString& SourceImagePath ,
    const FOnUploadImageComplete& OnComplete ,
    const FOnUploadImageFailed& OnFailed
)
{
    FHttpModule& HttpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest , ESPMode::ThreadSafe> Request = HttpModule.CreateRequest();

    Request->SetURL(UPLOAD_IMAGE_URL);
    Request->SetVerb("POST");

    FString Boundary = FString::Printf(TEXT("---------------------------%s") , *FGuid::NewGuid().ToString());
    Request->SetHeader("Content-Type" , FString::Printf(TEXT("multipart/form-data; boundary=%s") , *Boundary));

    TArray<uint8> RequestContent;

    // Helper function to add file data
    auto AddFileData = [&RequestContent , &Boundary](const FString& Key , const FString& FilePath)
        {
            FString FileName = FPaths::GetCleanFilename(FilePath);
            FString FormElement = FString::Printf(TEXT("--%s\r\nContent-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\nContent-Type: application/octet-stream\r\n\r\n") , *Boundary , *Key , *FileName);
            RequestContent.Append((uint8*)TCHAR_TO_UTF8(*FormElement) , FormElement.Len());

            TArray<uint8> FileContent;
            if (FFileHelper::LoadFileToArray(FileContent , *FilePath))
            {
                RequestContent.Append(FileContent);
            }
            RequestContent.Append((uint8*)"\r\n" , 2);
        };

    // Add form fields
    AddFileData("upload" , SourceImagePath);

    // Add closing boundary
    FString ClosingBoundary = FString::Printf(TEXT("--%s--\r\n") , *Boundary);
    RequestContent.Append((uint8*)TCHAR_TO_UTF8(*ClosingBoundary) , ClosingBoundary.Len());

    Request->SetContent(RequestContent);

    Request->OnProcessRequestComplete().BindStatic(&UBFL_Downloader::OnRequestComplete , OnComplete , OnFailed);
    Request->ProcessRequest();
}

void UBFL_Downloader::OnRequestComplete(
    FHttpRequestPtr Request ,
    FHttpResponsePtr Response ,
    bool bSuccess ,
    FOnUploadImageComplete OnComplete ,
    FOnUploadImageFailed OnFailed
)
    {// ��û ��ü�� ������ ���
    if (!bSuccess || !Response.IsValid())
    {
        OnFailed.ExecuteIfBound(EHttpErrorCode::None , TEXT("Request failed"));
        return;
    }

    EHttpErrorCode ErrorCode = ConvertToHttpErrorCode(Response->GetResponseCode());
    if (ErrorCode != EHttpErrorCode::OK)
    {
        // ���� ó��
        FString ErrorMessage = Response->GetContentAsString();
        OnFailed.ExecuteIfBound(ErrorCode , ErrorMessage);
        return;
    }

    // Parse JSON
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    FJsonSerializer::Deserialize(Reader , JsonObject);
    if (!JsonObject.IsValid())
    {
        // ���� ó�� - TODO: �����ڵ� ���� �ʿ�.
        FString ErrorMessage = Response->GetContentAsString();
        OnFailed.ExecuteIfBound(EHttpErrorCode::InternalServerError , ErrorMessage);
        return;
    }

    // check code
    int32 Code = JsonObject->GetIntegerField("code");
    if (Code != 100)
    {
        // ���� ó�� - TODO: �����ڵ� ���� �ʿ�.
        FString ErrorMessage = Response->GetContentAsString();
        OnFailed.ExecuteIfBound(EHttpErrorCode::InternalServerError , ErrorMessage);
        return;
    }

    // Parse data
    TSharedPtr<FJsonObject> DataObject = JsonObject->GetObjectField("data");
    if (!DataObject.IsValid())
    {
        // ���� ó�� - TODO: �����ڵ� ���� �ʿ�.
        FString ErrorMessage = Response->GetContentAsString();
        OnFailed.ExecuteIfBound(EHttpErrorCode::InternalServerError , ErrorMessage);
        return;
    }

    // get hash 
    FString Hash = DataObject->GetStringField("hash");

    // Pass hash to OnComplete
    OnComplete.ExecuteIfBound(Hash);
}