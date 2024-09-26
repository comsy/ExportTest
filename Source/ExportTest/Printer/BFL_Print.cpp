// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL_Print.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <shellapi.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

bool UBFL_Print::PrintImage(const FString& ImageFilePath)
{   
#if PLATFORM_WINDOWS
    // 파일 경로가 존재하는지 확인
    if (!FPaths::FileExists(ImageFilePath))
    {
        UE_LOG(LogTemp , Error , TEXT("파일이 존재하지 않습니다: %s") , *ImageFilePath);
        return false;
    }

    // 절대 경로로 변환
    FString AbsolutePath = FPaths::ConvertRelativePathToFull(ImageFilePath);

    // FString를 LPCWSTR로 변환
    LPCWSTR FilePath = *AbsolutePath;

    // SHELLEXECUTEINFO 구조체 설정
    SHELLEXECUTEINFOW ShExecInfo = { 0 };
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = nullptr;
    ShExecInfo.lpVerb = L"print";
    ShExecInfo.lpFile = FilePath;
    ShExecInfo.lpParameters = nullptr;
    ShExecInfo.lpDirectory = nullptr;
    ShExecInfo.nShow = SW_HIDE;
    ShExecInfo.hInstApp = nullptr;

    // 프린트 명령 실행
    if (!ShellExecuteExW(&ShExecInfo))
    {
        UE_LOG(LogTemp , Error , TEXT("프린트 명령을 실행하지 못했습니다."));
        return false;
    }
    // 프린트 작업이 완료될 때까지 대기 (옵션)
    WaitForSingleObject(ShExecInfo.hProcess , INFINITE);
    CloseHandle(ShExecInfo.hProcess);
    return true;
#else
    UE_LOG(LogTemp , Warning , TEXT("이 기능은 윈도우에서만 지원됩니다."));
    return false;
#endif
}