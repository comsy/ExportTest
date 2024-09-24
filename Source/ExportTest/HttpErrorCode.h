#pragma once

#include "CoreMinimal.h"
#include "HttpErrorCode.generated.h"

UENUM(BlueprintType)
enum class EHttpErrorCode : uint8
{
    None = 0 UMETA(DisplayName = "None") ,
    OK = 1 UMETA(DisplayName = "OK") , // 200
    Created = 2 UMETA(DisplayName = "Created") , // 201

    BadRequest = 11 UMETA(DisplayName = "Bad Request") , // 400
    Unauthorized = 12 UMETA(DisplayName = "Unauthorized") , // 401
    PaymentRequired = 13 UMETA(DisplayName = "Unauthorized") , // 402
    Forbidden = 14 UMETA(DisplayName = "Forbidden") , // 403
    NotFound = 15 UMETA(DisplayName = "Not Found") , // 404
    UnprocessableContent = 16 UMETA(DisplayName = "Unprocessable Content") , // 422
    TooManyRequests = 17 UMETA(DisplayName = "Too Many Requests") , // 429
    
    InternalServerError = 21 UMETA(DisplayName = "Internal Server Error") , // 500
    ServiceUnavailable = 22 UMETA(DisplayName = "Service Unavailable") // 503
};

// 함수 선언
EHttpErrorCode ConvertToHttpErrorCode(int32 ResponseCode);