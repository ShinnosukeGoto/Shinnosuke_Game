////=============================================================================
//// File: controller.cpp
//// Desc: DirectInput�֌W�̊֐����܂Ƃ߂�
//// Author : �㓡�T�V��
////=============================================================================
//#include "controller.h"
//#include "input.h"
////*****************************************************************************
//// Joypad�p�ݒ�l
////*****************************************************************************
//#define DEADZONE		2500			// �e����25%�𖳌��]�[���Ƃ���
//#define RANGE_MAX		1000			// �L���͈͂̍ő�l
//#define RANGE_MIN		-1000			// �L���͈͂̍ŏ��l
//#define CROSSZONE (4500)				//�\���L�[�̋��e�͈�
////*****************************************************************************
//// �O���[�o���ϐ��錾
////*****************************************************************************
//LPDIRECTINPUT8			pDInput = NULL;		// DirectInput�I�u�W�F�N�g
//LPDIRECTINPUTDEVICE8	pDIJoypad = NULL;	// �R���g���[���[�f�o�C�X
//DIJOYSTATE2 g_ControllerState;				//�R���g���[���[�̃v���X���
//DIJOYSTATE2 g_ControllerStateTrigger;		//�R���g���[���[�̃g���K�[���
//DIJOYSTATE2 g_ControllerStateRelease;		//�R���g���[���[�̃v���X���
//
//											//=============================================================================
//											//	�W���C�X�e�B�b�N�E�f�o�C�X��񋓁i�R�[���o�b�N�֐��j
//											//=============================================================================
//HRESULT CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/)
//{
//	HRESULT hr;
//
//	hr = pDInput->CreateDevice(lpddi->guidInstance, &pDIJoypad, NULL);
//	if (FAILED(hr))
//		return DIENUM_CONTINUE;	// ���̃f�o�C�X���
//
//	return DIENUM_CONTINUE;	// �f�o�C�X�̗񋓂𒆎~
//
//}
////=============================================================================
//// Joypad�p�I�u�W�F�N�g�쐬
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
//	//Direct�I�u�W�F�N�g�̍쐬
//	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL)))
//	{
//		return E_FAIL;
//	}
//
//
//	// �R���g���[���[��T��
//	pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, NULL, DIEDFL_ATTACHEDONLY);
//
//	if (!pDIJoypad)
//		return false;
//
//#if 0		//�G���R���̃R���g���[�����g���Ƃ��̓R�����g�A�E�g�O��
//
//	//���̓f�o�C�X(�R���g���[���[)�̍쐬
//	if (FAILED(pDInput->CreateDevice(GUID_Joystick, &pDIJoypad, NULL)))
//	{
//		return E_FAIL;
//	}
//#endif
//
//	// �R���g���[���[�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
//	hr = pDIJoypad->SetDataFormat(&c_dfDIJoystick2);
//	if (FAILED(hr))
//		return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s
//
//					  // ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
//	hr = pDIJoypad->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
//	if (FAILED(hr))
//		return false; // ���[�h�̐ݒ�Ɏ��s
//
//
//					  // ���̒l�͈̔͂�ݒ�
//					  // X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
//					  // (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
//					  // ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�́A�֌W�Ȃ�)
//					  //DIPROPRANGE diprg;
//					  //ZeroMemory(&diprg, sizeof(diprg));
//					  //diprg.diph.dwSize = sizeof(diprg);
//					  //diprg.diph.dwHeaderSize = sizeof(diprg.diph);
//					  //diprg.diph.dwHow = DIPH_BYOFFSET;
//					  //diprg.lMin = RANGE_MIN;
//					  //diprg.lMax = RANGE_MAX;
//					  //// X���͈̔͂�ݒ�
//					  //diprg.diph.dwObj = DIJOFS_X;
//					  //pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
//					  //// Y���͈̔͂�ݒ�
//					  //diprg.diph.dwObj = DIJOFS_Y;
//					  //pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
//
//					  //// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
//					  //// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
//					  //// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
//					  //DIPROPDWORD				dipdw;
//					  //dipdw.diph.dwSize = sizeof(DIPROPDWORD);
//					  //dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
//					  //dipdw.diph.dwHow = DIPH_BYOFFSET;
//					  //dipdw.dwData = DEADZONE;
//					  ////X���̖����]�[����ݒ�
//					  //dipdw.diph.dwObj = DIJOFS_X;
//					  //pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
//					  ////Y���̖����]�[����ݒ�
//					  //dipdw.diph.dwObj = DIJOFS_Y;
//					  //pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
//
//					  //�R���g���[���[���͐���J�n
//	pDIJoypad->Acquire();
//
//	return S_OK;
//
//}
////=============================================================================
////�X�V����
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
//		//�f�o�C�X����f�[�^���擾
//		if (SUCCEEDED(hr = pDIJoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
//		{
//			g_ControllerState.lY = ControllerState.lY;
//			g_ControllerState.lX = ControllerState.lX;
//			//�\���L�[�̏��
//			for (int nCnt = 0; nCnt < 3; nCnt++)
//			{
//				g_ControllerState.rgdwPOV[nCnt] = ControllerState.rgdwPOV[nCnt];
//
//			}
//			//�{�^���̏��
//			for (nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
//			{
//				//�L�[�g���K�[����ۑ�
//				g_ControllerStateTrigger.rgbButtons[nCntButton] = (g_ControllerState.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];
//
//				//�L�[�����[�X����ۑ�
//				g_ControllerStateRelease.rgbButtons[nCntButton] = (g_ControllerState.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];
//
//				//�L�[�v���X����ۑ�
//				g_ControllerState.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];
//
//			}
//		}
//		else
//		{
//			//�R���g���[���[�̃A�N�Z�X�����擾
//			pDIJoypad->Acquire();
//		}
//	}
//}
////=============================================================================
////�{�^���̃v���X��Ԃ��擾
////=============================================================================
//bool GetJoypadPress(int nButton)
//{
//	return(g_ControllerState.rgbButtons[nButton] & 0x80) ? true : false;
//}
////=============================================================================
////�{�^���̃g���K�[��Ԃ��擾
////=============================================================================
//bool GetJoypadTrigger(int nButton)
//{
//	bool bTrigger = (g_ControllerStateTrigger.rgbButtons[nButton] & 0x80);
//	bTrigger = (g_ControllerStateTrigger.rgbButtons[nButton] & 0x80) ? true : false;
//	return bTrigger;
//}
////=============================================================================
////�{�^���̃����[�X��Ԃ��擾
////=============================================================================
//bool GetJoypadRelease(int nButton)
//{
//	return(g_ControllerStateRelease.rgbButtons[nButton] & 0x80) ? true : false;
//}
////=============================================================================
////���z�R���g���[������
////=============================================================================
//CONTROLLER InputContoroller(void)
//{
//
//	if (GetKeyboardTrigger(DIK_SPACE) || GetJoypadTrigger(BUTTON_Y) || GetJoypadTrigger(BUTTON_B))
//	{//�W�����v�{�^���������ꂽ�Ƃ�
//		return CONTROLLER_KEY_JUMP;
//	}
//
//	if (GetKeyboardTrigger(DIK_SLASH) /*|| GetJoypadTrigger(BUTTON_Y) */|| GetJoypadTrigger(BUTTON_A))
//	{//�U���{�^���������ꂽ�Ƃ�
//		return CONTROLLER_KEY_ATTACK;
//	}
//
//	if (GetKeyboardTrigger(DIK_RETURN) /*|| GetJoypadTrigger(BUTTON_Y) || GetJoypadTrigger(BUTTON_B)*/)
//	{//���@�{�^���������ꂽ�Ƃ�
//		return CONTROLLER_KEY_MAGIC;
//	}
//
//	if (GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_UP) || g_ControllerState.rgdwPOV[0] > 0 - CROSSZONE&& g_ControllerState.rgdwPOV[0] <= 0 + CROSSZONE)
//	{//��L�[����
//		g_ControllerState.rgdwPOV[0] = 0;
//		return CONTROLLER_CROSS_UP;
//	}
//
//	if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHT) || g_ControllerState.rgdwPOV[0] > 9000 - CROSSZONE&&g_ControllerState.rgdwPOV[0] <= 9000 + CROSSZONE)
//	{//�E�L�[����
//		g_ControllerState.rgdwPOV[0] = 9000;
//		return CONTROLLER_CROSS_RIGHT;
//	}
//
//	if (GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_DOWN) || g_ControllerState.rgdwPOV[0] > 18000 - CROSSZONE&&g_ControllerState.rgdwPOV[0] <= 18000 + CROSSZONE)
//	{//���L�[����
//		g_ControllerState.rgdwPOV[0] = 18000;
//		return CONTROLLER_CROSS_DOWN;
//	}
//
//	if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFT) || g_ControllerState.rgdwPOV[0] > 27000 - CROSSZONE&& g_ControllerState.rgdwPOV[0] <= 27000 + CROSSZONE)
//	{//���L�[����
//		g_ControllerState.rgdwPOV[0] = 27000;
//		return CONTROLLER_CROSS_LEFT;
//	}
//
//	return CONTROLLER_MAX_CONTROLLER;
//}
////=============================================================================
////�I������
////=============================================================================
//void ReleaseDInput(void)
//{
//	//���̓f�o�C�X�i�L�[�{�[�h�j�̊J��
//	if (pDIJoypad != NULL)
//	{
//		//�L�[�{�[�h�̃A�N�Z�X�����J��
//		pDIJoypad->Unacquire();
//		pDIJoypad->Release();
//		pDIJoypad = NULL;
//	}
//	//DirectInput�I�u�W�F�N�g�̉��
//	if (pDInput != NULL)
//	{
//		pDInput->Release();
//		pDInput = NULL;
//	}
//
//}