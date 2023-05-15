#pragma once

#include <Windows.h>

#include <cstdint>
#include <string>
#include <format>
#include <cassert>


#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")




//string->wstring
std::wstring ConvertString(const std::string& str);

//wstring->string
std::string ConvertString(const std::wstring& str);

//CompilerShader関数
IDxcBlob* CompileShader(
	//CompilerするShaderファイルへのパス
	const std::wstring& filePath,
	//Compilerに使用するProfile
	const wchar_t* profile,
	//初期化で生成したものを３つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler);

void Log(const std::string& message);

struct Vector4 {
	float x;
	float y;
	float z;
	float w;

};