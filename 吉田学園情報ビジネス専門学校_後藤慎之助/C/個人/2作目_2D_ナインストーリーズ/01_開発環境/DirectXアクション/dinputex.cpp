//=============================================================================
//
// X-Bow Hunter [input.cpp]�̏��� (�L�[�{�[�h)
// Author :�㓡�T�V��
//
//=============================================================================
#include "dinputex.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define NUM_PAD_MAX (256)//�p�b�h�̍ő吔

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECTINPUT8       g_lpDI = NULL;	//DirectInput�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_lpDIDevice = NULL;	//���̓f�o�C�X
DIDEVCAPS            g_diDevCaps[NUM_PAD_MAX];

//=============================================================================
//�p�b�h����������
//=============================================================================
HRESULT InitPad(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_lpDI, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X(�p�b�h)�̍쐬
	if (FAILED(g_lpDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g���쐬
	if (FAILED(g_lpDIDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�(�t�H�A�O���E���h&��r�����[�h)
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

	//�p�b�h�̃A�N�Z�X�����l��
	g_lpDIDevice->Acquire();
	return S_OK;
}

//=============================================================================
// �p�b�h�I������
//=============================================================================
void UninitPad(void)
{
	//���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (g_lpDIDevice != NULL)
	{
		//�L�[�{�[�h�̃A�N�Z�X�����J��(���͐���I��)
		g_lpDIDevice->Unacquire();
		g_lpDIDevice->Release();
		g_lpDIDevice = NULL;
	}

	//Directinput�I�u�W�F�N�g�̊J��
	if (g_lpDI != NULL)
	{
		g_lpDI->Release();
		g_lpDI = NULL;
	}
}

//=============================================================================
// �p�b�h�X�V����
//=============================================================================
void UpdatePad(void)
{
	DIDEVCAPS aPadState[NUM_PAD_MAX]; //�p�b�h�̓��͏��
	//BYTE aPadState2[NUM_PAD_MAX]; //�p�b�h�̃����[�X���͏��
	int nCntPad;

	//�f�o�C�X����f�[�^���擾�i�g���K�[�j
	if (SUCCEEDED(g_lpDIDevice->GetDeviceState(sizeof(aPadState), aPadState)))
	{
		for (nCntPad = 0; nCntPad< NUM_PAD_MAX; nCntPad++)
		{
			//�L�[�v���X����ۑ�
			g_diDevCaps[nCntPad] = aPadState[nCntPad];
		}
	}

	////�f�o�C�X����f�[�^���擾�i�����[�X�j
	//if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState2), aKeyState2)))
	//{
	//	for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	//	{
	//		//�����[�X
	//		g_aKeyStateRelease[nCntKey] = (g_aKeyState2[nCntKey] ^ aKeyState2[nCntKey])&~aKeyState2[nCntKey];

	//		//�L�[�v���X����ۑ�
	//		g_aKeyState2[nCntKey] = aKeyState2[nCntKey];
	//	}
	//}

	//�p�b�h�̍X�V����
	else
	{
		//�L�[�{�[�h�ւ̃A�N�Z�X�����擾
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