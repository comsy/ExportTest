// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "MyWebSocketLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnWebSocketConnected);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnWebSocketConnectionError , const FString& , Error);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnWebSocketClosed , int32 , StatusCode , const FString& , Reason);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnWebSocketApprovalCompleted , int32 , StatusCode , const FString& , Reason);
/**
 * 
 */
UCLASS()
class EXPORTTEST_API UMyWebSocketLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
    static TSharedPtr<IWebSocket> WebSocket;

public:
    UFUNCTION(BlueprintCallable , Category = "WebSocket")
    static void Connect(
        const FString& URL ,
        const FOnWebSocketConnected& OnConnected ,
        const FOnWebSocketConnectionError& OnConnectionError ,
        const FOnWebSocketClosed& OnClosed , 
        const FOnWebSocketApprovalCompleted& OnApprovalCompleted
    );

    UFUNCTION(BlueprintCallable , Category = "WebSocket")
    static void Approval(
        const FString& Amount , 
        const FString& Period
    );

    UFUNCTION(BlueprintCallable , Category = "WebSocket")
    static void Disconnect();

    UFUNCTION(BlueprintCallable , Category = "WebSocket")
    static void StopAgent();

private:
    static void SendMessage(const FString& Message);	
};
