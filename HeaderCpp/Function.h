#pragma once
#include "LogConvert.h"




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





