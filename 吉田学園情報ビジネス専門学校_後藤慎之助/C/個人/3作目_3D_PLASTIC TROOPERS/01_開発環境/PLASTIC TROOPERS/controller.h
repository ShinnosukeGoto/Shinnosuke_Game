//=============================================================================
//
// PLASTIC TROOPERSの処理[controller.h](コントローラーでの操作）
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#define _CRT_SECURE_NO_WARNINGS_

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
// 外部変数
//=============================================================================
extern LPDIRECTINPUT8		pDInput;
extern LPDIRECTINPUTDEVICE8	pDIKeyboard;
extern BYTE					diKeyState[256];
extern LPDIRECTINPUTDEVICE8	pDIJoypad;
extern DWORD				diJoyState;
extern LPDIRECTINPUTDEVICE8	pDIMouse;
extern DIMOUSESTATE			diMouseState;

//=============================================================================
//マクロ定義
//=============================================================================
#define BUTTON_UP     (0)	    //十字キー上
#define BUTTON_DOWN   (18000)	//十字キー下
#define BUTTON_LEFT   (27000)	//十字キー左
#define BUTTON_RIGHT  (9000)	//十字キー右

#define CROSSZONE (4500)        //十字キーの許容範囲
//
//#define SAFE_RELEASE(x)	if(x){x->Release();x=NULL;} //開放処理を簡単にする
//

//=============================================================================
//構造体/列挙型定義
//=============================================================================
typedef enum
{
    BUTTON_X = 0,
    BUTTON_Y,
    BUTTON_A,
    BUTTON_B,
    BUTTON_L1,
    BUTTON_R1,
    BUTTON_L2,
    BUTTON_R2,
    BUTTON_L3,	//
    BUTTON_R3,
    BUTTON_BACK,		//
    BUTTON_START,		//
    BUTTON_Guide,
    MAX_BUTTON
}BUTTON;//ボタンの数

//-----------------------------------------------------------------------------
// プロトタイプ宣言 function-prototypes
//-----------------------------------------------------------------------------
bool InitDInput(HINSTANCE);
HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd);
void UpdateJoypad(void);
void ReleaseDInput(void);
bool GetJoypadPress(int nButton);
bool GetJoypadTrigger(int nButton);
bool GetJoypadRelease(int nButton);
DIJOYSTATE2 GetController(void);

#endif