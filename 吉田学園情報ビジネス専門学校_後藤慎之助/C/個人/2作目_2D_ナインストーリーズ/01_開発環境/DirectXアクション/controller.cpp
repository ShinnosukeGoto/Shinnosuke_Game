////=============================================================================
//// File: controller.cpp
//// Desc: DirectInput関係の関数をまとめる
//// Author : 後藤慎之助
////=============================================================================
//#include "controller.h"
//#include "input.h"
////*****************************************************************************
//// Joypad用設定値
////*****************************************************************************
//#define DEADZONE		2500			// 各軸の25%を無効ゾーンとする
//#define RANGE_MAX		1000			// 有効範囲の最大値
//#define RANGE_MIN		-1000			// 有効範囲の最小値
//#define CROSSZONE (4500)				//十字キーの許容範囲
////*****************************************************************************
//// グローバル変数宣言
////*****************************************************************************
//LPDIRECTINPUT8			pDInput = NULL;		// DirectInputオブジェクト
//LPDIRECTINPUTDEVICE8	pDIJoypad = NULL;	// コントローラーデバイス
//DIJOYSTATE2 g_ControllerState;				//コントローラーのプレス情報
//DIJOYSTATE2 g_ControllerStateTrigger;		//コントローラーのトリガー情報
//DIJOYSTATE2 g_ControllerStateRelease;		//コントローラーのプレス情報
//
//											//=============================================================================
//											//	ジョイスティック・デバイスを列挙（コールバック関数）
//											//=============================================================================
//HRESULT CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/)
//{
//	HRESULT hr;
//
//	hr = pDInput->CreateDevice(lpddi->guidInstance, &pDIJoypad, NULL);
//	if (FAILED(hr))
//		return DIENUM_CONTINUE;	// 次のデバイスを列挙
//
//	return DIENUM_CONTINUE;	// デバイスの列挙を中止
//
//}
////=============================================================================
//// Joypad用オブジェクト作成
////=============================================================================
//HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd)
//{
//	HRESULT		hr;
//
//	for (int nCnt = 0; nCnt < MAX_BUTTON; nCnt++)
//	{
//		g_ControllerStateTrigger.rgbButtons[nCnt] = '\0';
//	}
//
//	//Directオブジェクトの作成
//	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL)))
//	{
//		return E_FAIL;
//	}
//
//
//	// コントローラーを探す
//	pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, NULL, DIEDFL_ATTACHEDONLY);
//
//	if (!pDIJoypad)
//		return false;
//
//#if 0		//エレコムのコントローラを使うときはコメントアウト外す
//
//	//入力デバイス(コントローラー)の作成
//	if (FAILED(pDInput->CreateDevice(GUID_Joystick, &pDIJoypad, NULL)))
//	{
//		return E_FAIL;
//	}
//#endif
//
//	// コントローラー用のデータ・フォーマットを設定
//	hr = pDIJoypad->SetDataFormat(&c_dfDIJoystick2);
//	if (FAILED(hr))
//		return false; // データフォーマットの設定に失敗
//
//					  // モードを設定（フォアグラウンド＆非排他モード）
//	hr = pDIJoypad->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
//	if (FAILED(hr))
//		return false; // モードの設定に失敗
//
//
//					  // 軸の値の範囲を設定
//					  // X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
//					  // (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
//					  // 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドは、関係なし)
//					  //DIPROPRANGE diprg;
//					  //ZeroMemory(&diprg, sizeof(diprg));
//					  //diprg.diph.dwSize = sizeof(diprg);
//					  //diprg.diph.dwHeaderSize = sizeof(diprg.diph);
//					  //diprg.diph.dwHow = DIPH_BYOFFSET;
//					  //diprg.lMin = RANGE_MIN;
//					  //diprg.lMax = RANGE_MAX;
//					  //// X軸の範囲を設定
//					  //diprg.diph.dwObj = DIJOFS_X;
//					  //pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
//					  //// Y軸の範囲を設定
//					  //diprg.diph.dwObj = DIJOFS_Y;
//					  //pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
//
//					  //// 各軸ごとに、無効のゾーン値を設定する。
//					  //// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
//					  //// 指定する値は、10000に対する相対値(2000なら20パーセント)。
//					  //DIPROPDWORD				dipdw;
//					  //dipdw.diph.dwSize = sizeof(DIPROPDWORD);
//					  //dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
//					  //dipdw.diph.dwHow = DIPH_BYOFFSET;
//					  //dipdw.dwData = DEADZONE;
//					  ////X軸の無効ゾーンを設定
//					  //dipdw.diph.dwObj = DIJOFS_X;
//					  //pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
//					  ////Y軸の無効ゾーンを設定
//					  //dipdw.diph.dwObj = DIJOFS_Y;
//					  //pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
//
//					  //コントローラー入力制御開始
//	pDIJoypad->Acquire();
//
//	return S_OK;
//
//}
////=============================================================================
////更新処理
////=============================================================================
//void UpdateJoypad(void)
//{
//	DIJOYSTATE2 ControllerState;
//	//BYTE ControllerState[MAX_BUTTON];
//	HRESULT hr;
//	int nCntButton;
//
//	if (pDIJoypad != NULL)
//	{
//		hr = pDIJoypad->Poll();
//
//		hr = pDIJoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState);
//
//		//デバイスからデータを取得
//		if (SUCCEEDED(hr = pDIJoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
//		{
//			g_ControllerState.lY = ControllerState.lY;
//			g_ControllerState.lX = ControllerState.lX;
//			//十字キーの情報
//			for (int nCnt = 0; nCnt < 3; nCnt++)
//			{
//				g_ControllerState.rgdwPOV[nCnt] = ControllerState.rgdwPOV[nCnt];
//
//			}
//			//ボタンの情報
//			for (nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
//			{
//				//キートリガー情報を保存
//				g_ControllerStateTrigger.rgbButtons[nCntButton] = (g_ControllerState.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];
//
//				//キーリリース情報を保存
//				g_ControllerStateRelease.rgbButtons[nCntButton] = (g_ControllerState.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];
//
//				//キープレス情報を保存
//				g_ControllerState.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];
//
//			}
//		}
//		else
//		{
//			//コントローラーのアクセス権を取得
//			pDIJoypad->Acquire();
//		}
//	}
//}
////=============================================================================
////ボタンのプレス状態を取得
////=============================================================================
//bool GetJoypadPress(int nButton)
//{
//	return(g_ControllerState.rgbButtons[nButton] & 0x80) ? true : false;
//}
////=============================================================================
////ボタンのトリガー状態を取得
////=============================================================================
//bool GetJoypadTrigger(int nButton)
//{
//	bool bTrigger = (g_ControllerStateTrigger.rgbButtons[nButton] & 0x80);
//	bTrigger = (g_ControllerStateTrigger.rgbButtons[nButton] & 0x80) ? true : false;
//	return bTrigger;
//}
////=============================================================================
////ボタンのリリース状態を取得
////=============================================================================
//bool GetJoypadRelease(int nButton)
//{
//	return(g_ControllerStateRelease.rgbButtons[nButton] & 0x80) ? true : false;
//}
////=============================================================================
////仮想コントローラ入力
////=============================================================================
//CONTROLLER InputContoroller(void)
//{
//
//	if (GetKeyboardTrigger(DIK_SPACE) || GetJoypadTrigger(BUTTON_Y) || GetJoypadTrigger(BUTTON_B))
//	{//ジャンプボタンが押されたとき
//		return CONTROLLER_KEY_JUMP;
//	}
//
//	if (GetKeyboardTrigger(DIK_SLASH) /*|| GetJoypadTrigger(BUTTON_Y) */|| GetJoypadTrigger(BUTTON_A))
//	{//攻撃ボタンが押されたとき
//		return CONTROLLER_KEY_ATTACK;
//	}
//
//	if (GetKeyboardTrigger(DIK_RETURN) /*|| GetJoypadTrigger(BUTTON_Y) || GetJoypadTrigger(BUTTON_B)*/)
//	{//魔法ボタンが押されたとき
//		return CONTROLLER_KEY_MAGIC;
//	}
//
//	if (GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_UP) || g_ControllerState.rgdwPOV[0] > 0 - CROSSZONE&& g_ControllerState.rgdwPOV[0] <= 0 + CROSSZONE)
//	{//上キー入力
//		g_ControllerState.rgdwPOV[0] = 0;
//		return CONTROLLER_CROSS_UP;
//	}
//
//	if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHT) || g_ControllerState.rgdwPOV[0] > 9000 - CROSSZONE&&g_ControllerState.rgdwPOV[0] <= 9000 + CROSSZONE)
//	{//右キー入力
//		g_ControllerState.rgdwPOV[0] = 9000;
//		return CONTROLLER_CROSS_RIGHT;
//	}
//
//	if (GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_DOWN) || g_ControllerState.rgdwPOV[0] > 18000 - CROSSZONE&&g_ControllerState.rgdwPOV[0] <= 18000 + CROSSZONE)
//	{//下キー入力
//		g_ControllerState.rgdwPOV[0] = 18000;
//		return CONTROLLER_CROSS_DOWN;
//	}
//
//	if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFT) || g_ControllerState.rgdwPOV[0] > 27000 - CROSSZONE&& g_ControllerState.rgdwPOV[0] <= 27000 + CROSSZONE)
//	{//左キー入力
//		g_ControllerState.rgdwPOV[0] = 27000;
//		return CONTROLLER_CROSS_LEFT;
//	}
//
//	return CONTROLLER_MAX_CONTROLLER;
//}
////=============================================================================
////終了処理
////=============================================================================
//void ReleaseDInput(void)
//{
//	//入力デバイス（キーボード）の開放
//	if (pDIJoypad != NULL)
//	{
//		//キーボードのアクセス権を開放
//		pDIJoypad->Unacquire();
//		pDIJoypad->Release();
//		pDIJoypad = NULL;
//	}
//	//DirectInputオブジェクトの解放
//	if (pDInput != NULL)
//	{
//		pDInput->Release();
//		pDInput = NULL;
//	}
//
//}