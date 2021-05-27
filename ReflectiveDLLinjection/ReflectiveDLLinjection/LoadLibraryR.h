#pragma once
#include "ReflectivDLLinject.h"

	DWORD GetReflectiveLoaderOffset(VOID* lpReflectiveDllBuffer);

	HMODULE WINAPI LoadLibraryR(LPVOID lpBuffer, DWORD dwLength);

	HANDLE WINAPI LoadRemoteLibraryR(HANDLE hProcess, LPVOID lpBuffer, DWORD dwLength, LPVOID lpParameter);

