#pragma once
#include <string>
#include <cstdint>
#include <format>



#include <dxgidebug.h>
#include <dxcapi.h>



void Log(const std::string& message);



//>string->wstring
std::wstring ConvertString(const std::string& str);

//wstring->string
std::string ConvertString(const std::wstring& str);
	
