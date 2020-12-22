//=============================================================================
//
// �ŋ��̘b����� [input.cpp]�̏��� (�L�[�{�[�h����)
// Author :�㓡�T�V��
//
//=============================================================================
#include "input.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define NUM_KEY_MAX (256)//�L�[�̍ő吔

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECTINPUT8 g_pInput = NULL;	//DirectInput�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	//���̓f�o�C�X

BYTE g_aKeyState[NUM_KEY_MAX];	//�L�[�{�[�h�̓��͏�񃏁[�N
BYTE g_aKeyState2[NUM_KEY_MAX];	//�L�[�{�[�h�̓��͏�񃏁[�N
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];	//�L�[�{�[�h�̃g���K�[���
BYTE g_aKeyStateRelease[NUM_KEY_MAX];	//�L�[�{�[�h�̃����[�X���

//=============================================================================
// �L�[�{�[�h����������
// Author:�㓡�T�V��
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
    //DirectInput�I�u�W�F�N�g�̍쐬
    if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
    {
        return E_FAIL;
    }

    //���̓f�o�C�X(�L�[�{�[�h)�̍쐬
    if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
    {
        return E_FAIL;
    }

    //�f�[�^�t�H�[�}�b�g���쐬
    if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
    {
        return E_FAIL;
    }

    //�������[�h��ݒ�(�t�H�A�O���E���h&��r�����[�h)
    if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
    {
        return E_FAIL;
    }

    //�L�[�{�[�h�̃A�N�Z�X�����l��
    g_pDevKeyboard->Acquire();
    return S_OK;
}

//=============================================================================
// �L�[�{�[�h�I������
// Author:�㓡�T�V��
//=============================================================================
void UninitKeyboard(void)
{
    //���̓f�o�C�X(�L�[�{�[�h)�̊J��
    if (g_pDevKeyboard != NULL)
    {
        //�L�[�{�[�h�̃A�N�Z�X�����J��(���͐���I��)
        g_pDevKeyboard->Unacquire();
        g_pDevKeyboard->Release();
        g_pDevKeyboard = NULL;
    }

    //Directinput�I�u�W�F�N�g�̊J��
    if (g_pInput != NULL)
    {
        g_pInput->Release();
        g_pInput = NULL;
    }
}

//=============================================================================
// �L�[�{�[�h�X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateKeyboard(void)
{
    BYTE aKeyState[NUM_KEY_MAX]; //�L�[�{�[�h�̃g���K�[���͏��
    BYTE aKeyState2[NUM_KEY_MAX]; //�L�[�{�[�h�̃����[�X���͏��
    int nCntKey;

    //�f�o�C�X����f�[�^���擾�i�g���K�[�j
    if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
    {
        for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
        {
            //�g���K�[
            g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];

            //�L�[�v���X����ۑ�
            g_aKeyState[nCntKey] = aKeyState[nCntKey];
        }
    }

    //�f�o�C�X����f�[�^���擾�i�����[�X�j
    if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState2), aKeyState2)))
    {
        for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
        {
            //�����[�X
            g_aKeyStateRelease[nCntKey] = (g_aKeyState2[nCntKey] ^ aKeyState2[nCntKey])&~aKeyState2[nCntKey];

            //�L�[�v���X����ۑ�
            g_aKeyState2[nCntKey] = aKeyState2[nCntKey];
        }
    }

    //�L�[�{�[�h�̍X�V����
    else
    {
        //�L�[�{�[�h�ւ̃A�N�Z�X�����擾
        g_pDevKeyboard->Acquire();
    }
}

//==============================================================================
// �L�[�{�[�h�̃v���X�󋵂��擾���鏈��
// Author:�㓡�T�V��
//==============================================================================
bool GetKeyboardPress(int nkey)
{
    return(g_aKeyState[nkey] & 0x80) ? true : false;
}

//==============================================================================
// �g���K�[����
// Author:�㓡�T�V��
//==============================================================================
bool GetKeyboardTrigger(int nkey)
{
    return(g_aKeyStateTrigger[nkey] & 0x80) ? true : false;
}

//==============================================================================
// �����[�X����
// Author:�㓡�T�V��
//==============================================================================
bool GetKeyboardRelease(int nkey)
{
    return(g_aKeyStateRelease[nkey] & 0x80) ? true : false;
}