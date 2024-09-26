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
    // ���� ��ΰ� �����ϴ��� Ȯ��
    if (!FPaths::FileExists(ImageFilePath))
    {
        UE_LOG(LogTemp , Error , TEXT("������ �������� �ʽ��ϴ�: %s") , *ImageFilePath);
        return false;
    }

    // ���� ��η� ��ȯ
    FString AbsolutePath = FPaths::ConvertRelativePathToFull(ImageFilePath);

    // FString�� LPCWSTR�� ��ȯ
    LPCWSTR FilePath = *AbsolutePath;

    // SHELLEXECUTEINFO ����ü ����
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

    // ����Ʈ ��� ����
    if (!ShellExecuteExW(&ShExecInfo))
    {
        UE_LOG(LogTemp , Error , TEXT("����Ʈ ����� �������� ���߽��ϴ�."));
        return false;
    }
    // ����Ʈ �۾��� �Ϸ�� ������ ��� (�ɼ�)
    WaitForSingleObject(ShExecInfo.hProcess , INFINITE);
    CloseHandle(ShExecInfo.hProcess);
    return true;
#else
    UE_LOG(LogTemp , Warning , TEXT("�� ����� �����쿡���� �����˴ϴ�."));
    return false;
#endif
}