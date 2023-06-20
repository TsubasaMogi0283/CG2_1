#include "Function.h"


LRESULT CALLBACK  WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
	switch (msg) {
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);

}



//使い方はこんな感じ
	//出力にちゃんと「WSTRING」が出る
	//Log(ConvertString(std::format(L"wstring\n", wstringValue)));



void Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}


//>string->wstring
std::wstring ConvertString(const std::string& str) {
	if (str.empty())
	{
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0)
	{
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

//wstring->string
std::string ConvertString(const std::wstring& str) {
	if (str.empty())
	{
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0)
	{
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}


////CompileShader関数
IDxcBlob* CompileShader(
	const std::wstring& filePath,
	const wchar_t* profile,
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler) {
	//1.hlslファイルを読む
	Log(ConvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	//2.Compileする
	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E",L"main",
		L"-T",profile,
		L"-Zi",L"-Qembed_debug",
		L"-Od",
		L"-Zpr",
	};

	//実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(&shaderSourceBuffer, arguments, _countof(arguments), includeHandler, IID_PPV_ARGS(&shaderResult));
	//コンパイルエラーではなくdxcが起動出来ないなど致命的な状況
	assert(SUCCEEDED(hr));


	//3.警告・エラーが出ていないかを確認する
	//警告・エラーが出てたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		assert(false);
	}


	//4.Compile結果を受け取って返す
	//BLOB・・・BinaryLargeOBject
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	//実行用のバイナリを返却
	return shaderBlob;


}





Vector4 ColorAdapter(unsigned int color){
    Vector4 ResultColor = {

       ((color >> 24) & 0xFF) / 255.0f, // 赤

       ((color >> 16) & 0xFF) / 255.0f, // 緑

       ((color >>8) & 0xFF) / 255.0f,  // 青

       ((color) & 0xFF) / 255.0f //透明度

    };

     return ResultColor;

}





//'CG2_1.exe' (Win32): 'C:\授業資料\CG\CG2_2023\CG2_1\CG2_1_New\x64\Debug\CG2_1.exe' が読み込まれました。シンボルが読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\ntdll.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\kernel32.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\KernelBase.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\user32.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\win32u.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\gdi32.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\gdi32full.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\msvcp_win.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\ucrtbase.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\dxgi.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\D3D12.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\msvcp140d.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\ole32.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\combase.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\vcruntime140_1d.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\vcruntime140d.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\rpcrt4.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\ucrtbased.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\ucrtbased.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\ucrtbased.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\ucrtbased.dll' がアンロードされました
//'CG2_1.exe' (Win32): 'C:\Windows\System32\ucrtbased.dll' がアンロードされました
//'CG2_1.exe' (Win32): 'C:\授業資料\CG\CG2_2023\CG2_1\CG2_1_New\x64\Debug\dxcompiler.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\oleaut32.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\advapi32.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\msvcrt.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\sechost.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\imm32.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\授業資料\CG\CG2_2023\CG2_1\CG2_1_New\x64\Debug\dxil.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\uxtheme.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\ProgramData\A-Volute\A-Volute.Nahimic\Modules\Scheduled\x64\AudioDevProps2.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\ws2_32.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\shlwapi.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\shell32.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\ProgramData\A-Volute\A-Volute.Nahimic\Modules\Scheduled\x64\NahimicOSD.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\ProgramData\A-Volute\A-Volute.Nahimic\Modules\Scheduled\x64\ProductInfo.dll' が読み込まれました。
//Error 20 (this feature has not been implemented yet) in function AVolute::GetProductInfoT::<lambda_3920e95365a48b95dd51020986e9e351>::operator ()
//Error 20 (this feature has not been implemented yet) in function AVolute::GetProductInfoT::<lambda_3920e95365a48b95dd51020986e9e351>::operator ()
//Error 20 (this feature has not been implemented yet) in function AVolute::GetProductInfoT::<lambda_3920e95365a48b95dd51020986e9e351>::operator ()
//Error 20 (this feature has not been implemented yet) in function AVolute::GetProductInfoT::<lambda_3920e95365a48b95dd51020986e9e351>::operator ()
//Error 20 (this feature has not been implemented yet) in function AVolute::GetProductInfoT::<lambda_3920e95365a48b95dd51020986e9e351>::operator ()
//Error 20 (this feature has not been implemented yet) in function AVolute::GetProductInfoT::<lambda_3920e95365a48b95dd51020986e9e351>::operator ()
//'CG2_1.exe' (Win32): 'C:\Windows\System32\msctf.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\kernel.appcore.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\bcryptprimitives.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\TextInputFramework.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\CoreMessaging.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\CoreUIComponents.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\WinTypes.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\cryptbase.dll' が読み込まれました。
//Hello,DirectX!
//'CG2_1.exe' (Win32): 'C:\Windows\System32\D3D12Core.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DXGIDebug.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\d3d12SDKLayers.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DXCore.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\ResourcePolicyClient.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\ResourcePolicyClient.dll' がアンロードされました
//'CG2_1.exe' (Win32): 'C:\Windows\System32\directxdatabasehelper.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\nvmi.inf_amd64_83f1f677a32cc9d3\nvdlistx.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\version.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\version.dll' がアンロードされました
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\nvmi.inf_amd64_83f1f677a32cc9d3\nvdlistx.dll' がアンロードされました
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\nvmi.inf_amd64_83f1f677a32cc9d3\nvdlistx.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\version.dll' が読み込まれました。
//Use Adapter:NVIDIA GeForce RTX 3060 Laptop GPU
//'CG2_1.exe' (Win32): 'C:\Windows\System32\cfgmgr32.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\nvmi.inf_amd64_83f1f677a32cc9d3\nvldumdx.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\msasn1.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\cryptnet.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\crypt32.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\drvstore.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\nvmi.inf_amd64_83f1f677a32cc9d3\nvdlistx.dll' がアンロードされました
//'CG2_1.exe' (Win32): 'C:\Windows\System32\devobj.dll' が読み込まれました。
//スレッド 0x346c はコード 0 (0x0) で終了しました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\wldp.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\wintrust.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\imagehlp.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\cryptsp.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\rsaenh.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\bcrypt.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\nvmi.inf_amd64_83f1f677a32cc9d3\nvwgf2umx.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\winmm.dll' が読み込まれました。
//スレッド 0x65b8 はコード 0 (0x0) で終了しました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\nvmi.inf_amd64_83f1f677a32cc9d3\NvCamera\NvCameraAllowlisting64.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\nvmi.inf_amd64_83f1f677a32cc9d3\NvCamera\NvCameraAllowlisting64.dll' がアンロードされました
//'CG2_1.exe' (Win32): 'C:\Windows\System32\windows.storage.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\SHCore.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\SHCore.dll' がアンロードされました
//'CG2_1.exe' (Win32): 'C:\Windows\System32\SHCore.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\nvspcap64.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\profapi.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\ntmarta.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\dxilconv.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\D3DSCache.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\userenv.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\twinapi.appcore.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\dxilconv.dll' がアンロードされました
//D3D12 MESSAGE: Device Debug Layer Startup Options: GPU-Based Validation is enabled (disabled by default). This results in new validation not possible during API calls on the CPU, by creating patched shaders that have validation added directly to the shader. However, it can slow things down a lot, especially for applications with numerous PSOs. Time to see the first render frame may take several minutes. [ INITIALIZATION MESSAGE #1016: CREATEDEVICE_DEBUG_LAYER_STARTUP_OPTIONS]
//FeatureLevel : 12.2
//Complete create D3D12Device!!!
//'CG2_1.exe' (Win32): 'C:\Windows\System32\d3d11.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\iigd_dch.inf_amd64_51f685305808e3a5\igd10iumd64.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\apphelp.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\iigd_dch.inf_amd64_51f685305808e3a5\igd10um64gen11.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\ncrypt.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\ntasn1.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\iigd_dch.inf_amd64_51f685305808e3a5\igdgmm64.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\iigd_dch.inf_amd64_51f685305808e3a5\igc64.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\dcomp.dll' が読み込まれました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\dwmapi.dll' が読み込まれました。
//Begin CompileShader,path:Object3d.VS.hlsl,profile:vs_6_0
//Compile Succeeded,path:Object3d.VS.hlsl,profile:vs_6_0
//Begin CompileShader,path:Object3d.PS.hlsl,profile:ps_6_0
//Compile Succeeded,path:Object3d.PS.hlsl,profile:ps_6_0
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\iigd_dch.inf_amd64_51f685305808e3a5\igc64.dll' がアンロードされました
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\iigd_dch.inf_amd64_51f685305808e3a5\igdgmm64.dll' がアンロードされました
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\iigd_dch.inf_amd64_51f685305808e3a5\igd10iumd64.dll' がアンロードされました
//スレッド 0x6d74 はコード 0 (0x0) で終了しました。
//スレッド 0x6d88 はコード 0 (0x0) で終了しました。
//スレッド 0x6d80 はコード 0 (0x0) で終了しました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\d3d11.dll' がアンロードされました
//スレッド 0x6d84 はコード 0 (0x0) で終了しました。
//スレッド 0x6d70 はコード 0 (0x0) で終了しました。
//スレッド 0x6d6c はコード 0 (0x0) で終了しました。
//スレッド 0x6d68 はコード 0 (0x0) で終了しました。
//スレッド 0x6d5c はコード 0 (0x0) で終了しました。
//スレッド 0x6d58 はコード 0 (0x0) で終了しました。
//スレッド 0x6d64 はコード 0 (0x0) で終了しました。
//スレッド 0x6d60 はコード 0 (0x0) で終了しました。
//スレッド 0x6d78 はコード 0 (0x0) で終了しました。
//スレッド 0x6d4c はコード 0 (0x0) で終了しました。
//'CG2_1.exe' (Win32): 'C:\Windows\System32\winmm.dll' がアンロードされました
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\nvmi.inf_amd64_83f1f677a32cc9d3\nvwgf2umx.dll' がアンロードされました
//'CG2_1.exe' (Win32): 'C:\Windows\System32\DriverStore\FileRepository\nvmi.inf_amd64_83f1f677a32cc9d3\nvldumdx.dll' がアンロードされました
//スレッド 0x6e48 はコード 0 (0x0) で終了しました。
//スレッド 0x14a0 はコード 0 (0x0) で終了しました。
//スレッド 0x58ec はコード 0 (0x0) で終了しました。
//スレッド 0x3e34 はコード 0 (0x0) で終了しました。
//スレッド 0x6e4c はコード 0 (0x0) で終了しました。
//スレッド 0x1d20 はコード 0 (0x0) で終了しました。
//プログラム '[27612] CG2_1.exe' はコード 0 (0x0) で終了しました。
