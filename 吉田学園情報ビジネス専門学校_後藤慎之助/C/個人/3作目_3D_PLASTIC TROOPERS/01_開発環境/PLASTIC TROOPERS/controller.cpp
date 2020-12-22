//=============================================================================
// 
//PLASTIC TROOPERS�̏���[controller.cpp](�R���g���[���[�ł̑���j
// Author : �㓡�T�V��
//
//=============================================================================
#include "controller.h"
#include "input.h"

//*****************************************************************************
// Joypad�p�ݒ�l
//*****************************************************************************
#define DEADZONE		(2500)			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX		(10000)			// �L���͈͂̍ő�l
#define RANGE_MIN		(-10000)			// �L���͈͂̍ŏ��l
#define	MAX_CONTROLLER	(4)				//�Ȃ��R���g���[���[�̍ő吔 

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
    LPDIRECTINPUTDEVICE8	pDIJoypad = NULL;	// �R���g���[���[�f�o�C�X
    DIJOYSTATE2 State;							//�R���g���[���[�̃v���X���
    DIJOYSTATE2 Trigger;						//�R���g���[���[�̃g���K�[���
    DIJOYSTATE2 Release;						//�R���g���[���[�̃v���X���
}CONTROLLER;

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECTINPUT8			pDInput = NULL;		// DirectInput�I�u�W�F�N�g
LPDIRECTINPUTDEVICE8	pDIJoypad = NULL;	// �R���g���[���[�f�o�C�X
DIJOYSTATE2 g_ControllerState;				//�R���g���[���[�̃v���X���
DIJOYSTATE2 g_ControllerStateTrigger;		//�R���g���[���[�̃g���K�[���
DIJOYSTATE2 g_ControllerStateRelease;		//�R���g���[���[�̃v���X���
CONTROLLER g_Controller[MAX_CONTROLLER];

//=============================================================================
//	�W���C�X�e�B�b�N�E�f�o�C�X��񋓁i�R�[���o�b�N�֐��j
//=============================================================================
HRESULT CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/)
{
    HRESULT hr;

    hr = pDInput->CreateDevice(lpddi->guidInstance, &g_Controller[0].pDIJoypad, NULL);
    if (FAILED(hr))
        return DIENUM_CONTINUE;	// ���̃f�o�C�X���

    return DIENUM_CONTINUE;	// �f�o�C�X�̗񋓂𒆎~

}

//=============================================================================
// Joypad�p�I�u�W�F�N�g�쐬
//=============================================================================
HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd)
{
    HRESULT	hr;

    //Direct�I�u�W�F�N�g�̍쐬
    if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL)))
    {
        return E_FAIL;
    }

    // �R���g���[���[��T��
    pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, NULL, DIEDFL_ATTACHEDONLY);

    if (!g_Controller[0].pDIJoypad)
        return false;

    for (int nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
    {//�{�^���̏�����
        g_Controller[0].Trigger.rgbButtons[nCntButton] = '\0';
    }

    // �R���g���[���[�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
    hr = g_Controller[0].pDIJoypad->SetDataFormat(&c_dfDIJoystick2);
    if (FAILED(hr))
        return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

                      // ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
    hr = g_Controller[0].pDIJoypad->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    if (FAILED(hr))
        return false; // ���[�h�̐ݒ�Ɏ��s


    // ���̒l�͈̔͂�ݒ�
    // X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
    // (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
    // ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�́A�֌W�Ȃ�)
    DIPROPRANGE diprg;
    ZeroMemory(&diprg, sizeof(diprg));
    diprg.diph.dwSize = sizeof(diprg);
    diprg.diph.dwHeaderSize = sizeof(diprg.diph);
    diprg.diph.dwHow = DIPH_BYOFFSET;
    diprg.lMin = RANGE_MIN;
    diprg.lMax = RANGE_MAX;

    //���X�e�B�b�N
    // X���͈̔͂�ݒ�
    diprg.diph.dwObj = DIJOFS_X;
    g_Controller[0].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
    // Y���͈̔͂�ݒ�
    diprg.diph.dwObj = DIJOFS_Y;
    g_Controller[0].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

    //�E�X�e�B�b�N
    // Z���͈̔͂�ݒ�
    diprg.diph.dwObj = DIJOFS_Z;
    g_Controller[0].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
    // Z��]�͈̔͂�ݒ�
    diprg.diph.dwObj = DIJOFS_RZ;
    g_Controller[0].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);


    // �e�����ƂɁA�����̃]�[���l��ݒ肷��B
    // �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
    // �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
    DIPROPDWORD	dipdw;
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
    dipdw.diph.dwHow = DIPH_BYOFFSET;
    dipdw.dwData = DEADZONE;

    //���X�e�B�b�N
    //X���̖����]�[����ݒ�
    dipdw.diph.dwObj = DIJOFS_X;
    g_Controller[0].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
    //Y���̖����]�[����ݒ�
    dipdw.diph.dwObj = DIJOFS_Y;
    g_Controller[0].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

    //�E�X�e�B�b�N
    //Z���̖����]�[����ݒ�
    dipdw.diph.dwObj = DIJOFS_Z;
    g_Controller[0].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
    //Z��]�̖����]�[����ݒ�
    dipdw.diph.dwObj = DIJOFS_RZ;
    g_Controller[0].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);


    //�R���g���[���[���͐���J�n
    g_Controller[0].pDIJoypad->Acquire();

    return S_OK;

}

//=============================================================================
//�X�V����
//=============================================================================
void UpdateJoypad(void)
{
    DIJOYSTATE2 ControllerState;
    //BYTE ControllerState[MAX_BUTTON];
    HRESULT hr;
    int nCntButton;
    //for (int nCntController = 0; nCntController < MAX_CONTROLLER; nCntController++)
    //{

    //}
    if (g_Controller[0].pDIJoypad != NULL)
    {
        hr = g_Controller[0].pDIJoypad->Poll();

        hr = g_Controller[0].pDIJoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState);

        //�f�o�C�X����f�[�^���擾
        if (SUCCEEDED(hr = g_Controller[0].pDIJoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
        {
            //���X�e�B�b�N�̏��
            //�g���K�[����ۑ�
            //g_ControllerStateTrigger.rgbButtons[nCntButton] = (g_ControllerState.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];
            //�v���X���
            g_Controller[0].State.lY = ControllerState.lY;
            g_Controller[0].State.lX = ControllerState.lX;
            g_Controller[0].State.lZ = ControllerState.lZ;
            g_Controller[0].State.lRz = ControllerState.lRz;

            //�\���L�[�̏��
            for (int nCnt = 0; nCnt < 3; nCnt++)
            {
                //�\���L�[�v���X����ۑ�
                g_Controller[0].State.rgdwPOV[nCnt] = ControllerState.rgdwPOV[nCnt];

            }


            //�{�^���̏��
            for (nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
            {
                //�L�[�g���K�[����ۑ�
                g_Controller[0].Trigger.rgbButtons[nCntButton] = (g_Controller[0].State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

                //�L�[�����[�X����ۑ�
                g_Controller[0].Release.rgbButtons[nCntButton] = (g_Controller[0].State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

                //�L�[�v���X����ۑ�
                g_Controller[0].State.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];

            }
        }
        else
        {
            //�R���g���[���[�̃A�N�Z�X�����擾
            g_Controller[0].pDIJoypad->Acquire();
        }
    }
}

//=============================================================================
//�{�^���̃v���X��Ԃ��擾
//=============================================================================
bool GetJoypadPress(int nButton)
{
    return(g_Controller[0].State.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
//�{�^���̃g���K�[��Ԃ��擾
//=============================================================================
bool GetJoypadTrigger(int nButton)
{
    return (g_Controller[0].Trigger.rgbButtons[nButton] & 0x80) ? true : false;;
}

//=============================================================================
//�{�^���̃����[�X��Ԃ��擾
//=============================================================================
bool GetJoypadRelease(int nButton)
{
    return(g_Controller[0].Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
//�R���g���[���̏�Ԃ��擾
//=============================================================================
DIJOYSTATE2 GetController(void)
{
    return g_Controller[0].State;
}

//=============================================================================
//�I������
//=============================================================================
void ReleaseDInput(void)
{
    for (int nCntDevise = 0; nCntDevise < MAX_CONTROLLER; nCntDevise++)
    {

        //���̓f�o�C�X�̊J��
        if (g_Controller[nCntDevise].pDIJoypad != NULL)
        {
            //�R���g���[���̃A�N�Z�X�����J��
            g_Controller[nCntDevise].pDIJoypad->Unacquire();
            g_Controller[nCntDevise].pDIJoypad->Release();
            g_Controller[nCntDevise].pDIJoypad = NULL;
        }
    }
    //DirectInput�I�u�W�F�N�g�̉��
    if (pDInput != NULL)
    {
        pDInput->Release();
        pDInput = NULL;
    }

}
