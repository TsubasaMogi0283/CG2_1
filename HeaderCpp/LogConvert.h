#pragma once
#include <cstdint>
#include <string>
#include <format>



#include <dxgidebug.h>
#include <dxcapi.h>



#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

void Log(const std::string& message);

//string->wstring
std::wstring ConvertString(const std::string& str);

//wstring->string
std::string ConvertString(const std::wstring& str);
