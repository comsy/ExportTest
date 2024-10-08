// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWebSocketLibrary.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"

TSharedPtr<IWebSocket> UMyWebSocketLibrary::WebSocket = nullptr;

void UMyWebSocketLibrary::Connect(
    const FString& URL ,
    const FOnWebSocketConnected& OnConnected ,
    const FOnWebSocketConnectionError& OnConnectionError ,
    const FOnWebSocketClosed& OnClosed , 
    const FOnWebSocketApprovalCompleted& OnApprovalCompleted
)
{
    WebSocket = FWebSocketsModule::Get().CreateWebSocket(URL);

    // Using lambdas to handle WebSocket events
    WebSocket->OnConnected().AddLambda([OnConnected]() {
        UE_LOG(LogTemp , Log , TEXT("Connected to WebSocket server"));
        if (OnConnected.IsBound())
        {
            OnConnected.Execute();
        }
    });

    WebSocket->OnConnectionError().AddLambda([OnConnectionError](const FString& Error) {
        UE_LOG(LogTemp , Error , TEXT("Connection error: %s") , *Error);
        if (OnConnectionError.IsBound())
        {
            OnConnectionError.Execute(Error);
        }
    });

    WebSocket->OnClosed().AddLambda([OnClosed](int32 StatusCode , const FString& Reason , bool bWasClean) {
        UE_LOG(LogTemp , Log , TEXT("Connection closed: %s") , *Reason);
        if (OnClosed.IsBound())
        {
            OnClosed.Execute(StatusCode , Reason);
        }
    });

    WebSocket->OnMessage().AddLambda([OnApprovalCompleted](const FString& Message) {
        UE_LOG(LogTemp , Log , TEXT("Received message: %s") , *Message);
        if (OnApprovalCompleted.IsBound())
        {
            OnApprovalCompleted.Execute(0 , "");
        }
    });

    WebSocket->Connect();
}

void UMyWebSocketLibrary::Approval(
    const FString& Amount ,
    const FString& Period
)
{
    // Construct the JSON message
    TSharedPtr<FJsonObject> AgentSpec = MakeShareable(new FJsonObject());
    AgentSpec->SetStringField(TEXT("inTranCode") , TEXT("NV"));
    AgentSpec->SetStringField(TEXT("inTradeType") , TEXT("D1")); // credit approval
    AgentSpec->SetStringField(TEXT("inInstallment") , Period);
    AgentSpec->SetStringField(TEXT("inTranAmt") , Amount);

    TSharedPtr<FJsonObject> AgentApproval = MakeShareable(new FJsonObject());
    AgentApproval->SetObjectField(TEXT("KIS_ICApproval") , AgentSpec);

    FString Message;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Message);
    FJsonSerializer::Serialize(AgentApproval.ToSharedRef() , Writer);

    SendMessage(Message);
}

void UMyWebSocketLibrary::SendMessage(const FString& Message)
{
    if (WebSocket.IsValid() && WebSocket->IsConnected())
    {
        WebSocket->Send(Message);
        UE_LOG(LogTemp , Log , TEXT("Sent Message: %s") , *Message);
    }
}

void UMyWebSocketLibrary::Disconnect()
{
    if (WebSocket.IsValid() && WebSocket->IsConnected())
    {
        WebSocket->Close();
        UE_LOG(LogTemp , Log , TEXT("Disconnected from WebSocket"));
    }
}

void UMyWebSocketLibrary::StopAgent()
{
    if (WebSocket.IsValid() && WebSocket->IsConnected())
    {
        SendMessage(TEXT("{\"KIS_Agent_Stop\":{}}"));
        UE_LOG(LogTemp , Log , TEXT("Sent Stop Command"));
    }
}