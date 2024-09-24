#include "HttpErrorCode.h"

EHttpErrorCode ConvertToHttpErrorCode(int32 ResponseCode)
{
    switch (ResponseCode)
    {
    case 200: return EHttpErrorCode::OK;
    case 201: return EHttpErrorCode::Created;

    case 400: return EHttpErrorCode::BadRequest;
    case 401: return EHttpErrorCode::Unauthorized;
    case 402: return EHttpErrorCode::PaymentRequired;
    case 403: return EHttpErrorCode::Forbidden;
    case 404: return EHttpErrorCode::NotFound;
    case 422: return EHttpErrorCode::UnprocessableContent;
    case 429: return EHttpErrorCode::TooManyRequests;

    case 500: return EHttpErrorCode::InternalServerError;
    case 503: return EHttpErrorCode::ServiceUnavailable;
    default: return EHttpErrorCode::None;
    }
}