//=============================================================================
//
// X-Bow Hunter [input.cpp]の処理 (キーボード)
// Author :後藤慎之助
//
//=============================================================================
#include "dinputex.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define NUM_PAD_MAX (256)//パッドの最大数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECTINPUT8       g_lpDI = NULL;	//DirectInputへのポインタ
LPDIRECTINPUTDEVICE8 g_lpDIDevice = NULL;	//入力デバイス
DIDEVCAPS            g_diDevCaps[NUM_PAD_MAX];

//=============================================================================
//パッド初期化処理
//=============================================================================
HRESULT InitPad(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの作成
	if (FAILED(DirectInput8Create((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_lpDI, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(パッド)の作成
	if (FAILED(g_lpDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	//データフォーマットを作成
	if (FAILED(g_lpDIDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	//協調モードを設定(フォアグラウンド&非排他モード)
	if (FAILED(g_lpDIDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
	{
		return E_FAIL;
	}

	////
	//g_diDevCaps.dwSize = sizeof(DIDEVCAPS);
	//if (FAILED(g_lpDIDevice->GetCapabilities(&g_diDevCaps)))
	//{
	//	return E_FAIL;
	//}

	////
	//if (FAILED(g_lpDIDevice->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS)))
	//{
	//	return E_FAIL;
	//}

	//パッドのアクセス権を獲得
	g_lpDIDevice->Acquire();
	return S_OK;
}

//=============================================================================
// パッド終了処理
//=============================================================================
void UninitPad(void)
{
	//入力デバイス(キーボード)の開放
	if (g_lpDIDevice != NULL)
	{
		//キーボードのアクセス権を開放(入力制御終了)
		g_lpDIDevice->Unacquire();
		g_lpDIDevice->Release();
		g_lpDIDevice = NULL;
	}

	//Directinputオブジェクトの開放
	if (g_lpDI != NULL)
	{
		g_lpDI->Release();
		g_lpDI = NULL;
	}
}

//=============================================================================
// パッド更新処理
//=============================================================================
void UpdatePad(void)
{
	DIDEVCAPS aPadState[NUM_PAD_MAX]; //パッドの入力情報
	//BYTE aPadState2[NUM_PAD_MAX]; //パッドのリリース入力情報
	int nCntPad;

	//デバイスからデータを取得（トリガー）
	if (SUCCEEDED(g_lpDIDevice->GetDeviceState(sizeof(aPadState), aPadState)))
	{
		for (nCntPad = 0; nCntPad< NUM_PAD_MAX; nCntPad++)
		{
			//キープレス情報を保存
			g_diDevCaps[nCntPad] = aPadState[nCntPad];
		}
	}

	////デバイスからデータを取得（リリース）
	//if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState2), aKeyState2)))
	//{
	//	for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	//	{
	//		//リリース
	//		g_aKeyStateRelease[nCntKey] = (g_aKeyState2[nCntKey] ^ aKeyState2[nCntKey])&~aKeyState2[nCntKey];

	//		//キープレス情報を保存
	//		g_aKeyState2[nCntKey] = aKeyState2[nCntKey];
	//	}
	//}

	//パッドの更新処理
	else
	{
		//キーボードへのアクセス権を取得
		g_lpDIDevice->Acquire();
	}
}

//------------------------------------------------------------------------------
//    Joysticks Callback.
//------------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	if (FAILED(g_lpDI->CreateDevice(pdidInstance->guidInstance, &g_lpDIDevice, NULL))) return DIENUM_CONTINUE;

	return DIENUM_STOP;
}

//------------------------------------------------------------------------------
//    Axes Callback.
//------------------------------------------------------------------------------
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - 1000;
	diprg.lMax = 0 + 1000;

	if (FAILED(g_lpDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph))) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}