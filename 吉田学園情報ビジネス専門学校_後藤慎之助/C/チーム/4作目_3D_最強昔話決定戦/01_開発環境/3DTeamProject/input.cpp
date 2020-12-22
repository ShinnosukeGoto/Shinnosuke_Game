//=============================================================================
//
// 最強昔話決定戦 [input.cpp]の処理 (キーボード操作)
// Author :後藤慎之助
//
//=============================================================================
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define NUM_KEY_MAX (256)//キーの最大数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECTINPUT8 g_pInput = NULL;	//DirectInputへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	//入力デバイス

BYTE g_aKeyState[NUM_KEY_MAX];	//キーボードの入力情報ワーク
BYTE g_aKeyState2[NUM_KEY_MAX];	//キーボードの入力情報ワーク
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];	//キーボードのトリガー情報
BYTE g_aKeyStateRelease[NUM_KEY_MAX];	//キーボードのリリース情報

//=============================================================================
// キーボード初期化処理
// Author:後藤慎之助
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
    //DirectInputオブジェクトの作成
    if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
    {
        return E_FAIL;
    }

    //入力デバイス(キーボード)の作成
    if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
    {
        return E_FAIL;
    }

    //データフォーマットを作成
    if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
    {
        return E_FAIL;
    }

    //協調モードを設定(フォアグラウンド&非排他モード)
    if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
    {
        return E_FAIL;
    }

    //キーボードのアクセス権を獲得
    g_pDevKeyboard->Acquire();
    return S_OK;
}

//=============================================================================
// キーボード終了処理
// Author:後藤慎之助
//=============================================================================
void UninitKeyboard(void)
{
    //入力デバイス(キーボード)の開放
    if (g_pDevKeyboard != NULL)
    {
        //キーボードのアクセス権を開放(入力制御終了)
        g_pDevKeyboard->Unacquire();
        g_pDevKeyboard->Release();
        g_pDevKeyboard = NULL;
    }

    //Directinputオブジェクトの開放
    if (g_pInput != NULL)
    {
        g_pInput->Release();
        g_pInput = NULL;
    }
}

//=============================================================================
// キーボード更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateKeyboard(void)
{
    BYTE aKeyState[NUM_KEY_MAX]; //キーボードのトリガー入力情報
    BYTE aKeyState2[NUM_KEY_MAX]; //キーボードのリリース入力情報
    int nCntKey;

    //デバイスからデータを取得（トリガー）
    if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
    {
        for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
        {
            //トリガー
            g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];

            //キープレス情報を保存
            g_aKeyState[nCntKey] = aKeyState[nCntKey];
        }
    }

    //デバイスからデータを取得（リリース）
    if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState2), aKeyState2)))
    {
        for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
        {
            //リリース
            g_aKeyStateRelease[nCntKey] = (g_aKeyState2[nCntKey] ^ aKeyState2[nCntKey])&~aKeyState2[nCntKey];

            //キープレス情報を保存
            g_aKeyState2[nCntKey] = aKeyState2[nCntKey];
        }
    }

    //キーボードの更新処理
    else
    {
        //キーボードへのアクセス権を取得
        g_pDevKeyboard->Acquire();
    }
}

//==============================================================================
// キーボードのプレス状況を取得する処理
// Author:後藤慎之助
//==============================================================================
bool GetKeyboardPress(int nkey)
{
    return(g_aKeyState[nkey] & 0x80) ? true : false;
}

//==============================================================================
// トリガー処理
// Author:後藤慎之助
//==============================================================================
bool GetKeyboardTrigger(int nkey)
{
    return(g_aKeyStateTrigger[nkey] & 0x80) ? true : false;
}

//==============================================================================
// リリース処理
// Author:後藤慎之助
//==============================================================================
bool GetKeyboardRelease(int nkey)
{
    return(g_aKeyStateRelease[nkey] & 0x80) ? true : false;
}