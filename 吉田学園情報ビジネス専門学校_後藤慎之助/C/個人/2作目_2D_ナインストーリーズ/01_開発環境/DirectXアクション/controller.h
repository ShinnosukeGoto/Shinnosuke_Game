//#pragma once
////=============================================================================
////
//// コントローラー入力の処理 [controller.h]
//// Author : 後藤慎之助
////
////=============================================================================
//#include "main.h"
////=============================================================================
//// 外部変数
////=============================================================================
//extern LPDIRECTINPUT8		pDInput;
//extern LPDIRECTINPUTDEVICE8	pDIKeyboard;
//extern BYTE					diKeyState[256];
//extern LPDIRECTINPUTDEVICE8	pDIJoypad;
//extern DWORD				diJoyState;
//extern LPDIRECTINPUTDEVICE8	pDIMouse;
//extern DIMOUSESTATE			diMouseState;
////=============================================================================
////構造体/列挙型定義
////=============================================================================
//typedef enum
//{
//	BUTTON_X = 0,
//	BUTTON_Y,
//	BUTTON_A,
//	BUTTON_B,
//	BUTTON_L1,
//	BUTTON_R1,
//	BUTTON_L2,
//	BUTTON_R2,
//	BUTTON_SELECT,	//
//	BUTTON_START,
//	BUTTON_L3,		//
//	BUTTON_R3,		//
//	BUTTON_M,
//	MAX_BUTTON
//}BUTTON;//ボタンの数
//
//typedef enum
//{
//	CONTROLLER_KEY_JUMP,				//ジャンプ操作をまとめたもの
//	CONTROLLER_KEY_ATTACK,				//攻撃
//	CONTROLLER_KEY_MAGIC,				//魔法
//										//CONTROLLER_KEY_1,					//
//										//CONTROLLER_KEY_2,					//
//										//CONTROLLER_KEY_3,					//
//										//CONTROLLER_KEY_4,					//
//										//CONTROLLER_KEY_5,					//
//										//CONTROLLER_KEY_6,					//
//										//CONTROLLER_KEY_7,					//
//										//CONTROLLER_KEY_8,					//
//
//										CONTROLLER_CROSS_UP,				// 十字キー 上
//										CONTROLLER_CROSS_DOWN,				// 十字キー 下
//										CONTROLLER_CROSS_LEFT,				// 十字キー 左
//										CONTROLLER_CROSS_RIGHT,			// 十字キー 右
//										CONTROLLER_MAX_CONTROLLER			//最大数
//}CONTROLLER;//仮想コントローラ
//
//			//-----------------------------------------------------------------------------
//			// プロトタイプ宣言 function-prototypes
//			//-----------------------------------------------------------------------------
//bool InitDInput(HINSTANCE);
//HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd);
//void UpdateJoypad(void);
//void ReleaseDInput(void);
//bool GetJoypadPress(int nButton);
//bool GetJoypadTrigger(int nButton);
//bool GetJoypadRelease(int nButton);
//CONTROLLER InputContoroller(void);
////-----------------------------------------------------------------------------
//// Joypad情報用マクロ
////-----------------------------------------------------------------------------
//// ジョイパッドの各ボタン押下情報を、３２ビット内の次の位置に割り当てる
//// そのビットがオンならば、そのボタンが押されたことになる
//
//#define BUTTON_UP     0x00000001l	//方向キー上(dijs.IY<0)
//#define BUTTON_DOWN   0x00000002l	//方向キー下(dijs.IY>0)
//#define BUTTON_LEFT   0x00000004l	//方向キー左(dijs.IX<0)
//#define BUTTON_RIGHT  0x00000008l	//方向キー右(dijs.IX>0)
//
////#define BUTTON_X      (0)	//Ｘボタン(dijs.rgbButtons[3]&0x80)
////#define BUTTON_Y      (1)	//Ｙボタン(dijs.rgbButtons[4]&0x80)
////#define BUTTON_A      (2)	//Ａボタン(dijs.rgbButtons[0]&0x80)
////#define BUTTON_B      (3)	//Ｂボタン(dijs.rgbButtons[1]&0x80)
////#define BUTTON_L1      (4)	//Ｃボタン(dijs.rgbButtons[2]&0x80)
////#define BUTTON_R1      (5)	//Ｚボタン(dijs.rgbButtons[5]&0x80)
////#define BUTTON_L2      (6)	//Ｌボタン(dijs.rgbButtons[6]&0x80)
////#define BUTTON_R3      (7)	//Ｒボタン(dijs.rgbButtons[7]&0x80)
////#define BUTTON_START  (11)	//ＳＴＡＲＴボタン(dijs.rgbButtons[8]&0x80)
////#define BUTTON_M      (12)	//Ｍボタン(dijs.rgbButtons[9]&0x80)
//
//#define SAFE_RELEASE(x)	if(x){x->Release();x=NULL;} //開放処理を簡単にする
////