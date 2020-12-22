//=============================================================================
// 
// �ŋ��̘b�����̏���[controller.cpp](�R���g���[���[�ł̑���j
// Author : KUDO AYANO
//
//=============================================================================
#include "controller.h"

//*****************************************************************************
// Joypad�p�ݒ�l
//*****************************************************************************
#define DEADZONE        2500            // �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX       10000           // �L���͈͂̍ő�l
#define RANGE_MIN       -10000          // �L���͈͂̍ŏ��l
#define MAX_CONTROLLER  4               //�Ȃ��R���g���[���[�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
    LPDIRECTINPUTDEVICE8    pDIJoypad = NULL;   // �R���g���[���[�f�o�C�X
    DIJOYSTATE2 State;                          //�R���g���[���[�̃v���X���
    DIJOYSTATE2 Trigger;                        //�R���g���[���[�̃g���K�[���
    DIJOYSTATE2 Release;                        //�R���g���[���[�̃v���X���
    bool use;
}CONTROLLER;//�R���g���[���[�̏��

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECTINPUT8          pDInput = NULL;     // DirectInput�I�u�W�F�N�g
int g_nCntContoroller;                      // �ڑ����Ă���R���g���[���̐�
CONTROLLER g_Controller[MAX_CONTROLLER];    //�R���g���[���[�̏��

//=============================================================================
// [GetJoystickCallback] �W���C�X�e�B�b�N�E�f�o�C�X��񋓁i�R�[���o�b�N�֐��j
// Author:KUDO AYANO
//=============================================================================
HRESULT CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/)
{
    HRESULT hr;

    hr = pDInput->CreateDevice(lpddi->guidInstance, &g_Controller[g_nCntContoroller].pDIJoypad, NULL);
    g_nCntContoroller++;
    return DIENUM_CONTINUE; // ���̃f�o�C�X���

}

//=============================================================================
// Joypad�p�I�u�W�F�N�g�쐬
// Author:KUDO AYANO
//=============================================================================
HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd)
{
    HRESULT	hr;
    g_nCntContoroller = 0;

    //Direct�I�u�W�F�N�g�̍쐬
    if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL)))
    {
        return E_FAIL;
    }

    // �R���g���[���[��T��
    hr = pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, NULL, DIEDFL_ATTACHEDONLY);

    for (int nCntCntoroller = 0; nCntCntoroller < g_nCntContoroller; nCntCntoroller++)
    {

        if (FAILED(hr))
        {
            g_Controller[nCntCntoroller].State.rgdwPOV[nCntCntoroller] = -1;
        }
        if (!g_Controller[nCntCntoroller].pDIJoypad)
        {
            return false;
        }

        for (int nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
        {//�{�^���̏�����
            g_Controller[nCntCntoroller].Trigger.rgbButtons[nCntButton] = '\0';
        }

        //�\���L�[�@�̏�����
        for (int nCnt = 0; nCnt < 3; nCnt++)
        {
            g_Controller[nCntCntoroller].State.rgdwPOV[nCnt] = -1;
        }


        // �R���g���[���[�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
        hr = g_Controller[nCntCntoroller].pDIJoypad->SetDataFormat(&c_dfDIJoystick2);
        if (FAILED(hr))
        {
            return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s
        }

        // ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
        hr = g_Controller[nCntCntoroller].pDIJoypad->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
        if (FAILED(hr))
        {
            return false; // ���[�h�̐ݒ�Ɏ��s
        }

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
        // X���͈̔͂�ݒ�
        diprg.diph.dwObj = DIJOFS_X;
        g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
        // Y���͈̔͂�ݒ�
        diprg.diph.dwObj = DIJOFS_Y;
        g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
        // Z���͈̔͂�ݒ�
        diprg.diph.dwObj = DIJOFS_Z;
        g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
        // Z��]�͈̔͂�ݒ�
        diprg.diph.dwObj = DIJOFS_RZ;
        g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

        // �e�����ƂɁA�����̃]�[���l��ݒ肷��B
        // �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
        // �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
        DIPROPDWORD	dipdw;
        dipdw.diph.dwSize = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
        dipdw.diph.dwHow = DIPH_BYOFFSET;
        dipdw.dwData = DEADZONE;
        //X���̖����]�[����ݒ�
        dipdw.diph.dwObj = DIJOFS_X;
        g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
        //Y���̖����]�[����ݒ�
        dipdw.diph.dwObj = DIJOFS_Y;
        g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
        //Z���̖����]�[����ݒ�
        dipdw.diph.dwObj = DIJOFS_Z;
        g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
        //Z��]�̖����]�[����ݒ�
        dipdw.diph.dwObj = DIJOFS_RZ;
        g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

        //�R���g���[���[���͐���J�n
        g_Controller[nCntCntoroller].pDIJoypad->Acquire();
    }
    return S_OK;
}

//=============================================================================
// �R���g���[���[�X�V����
// Author:KUDO AYANO
//=============================================================================
void UpdateJoypad(void)
{
    DIJOYSTATE2 ControllerState;
    //BYTE ControllerState[nCntCntoroller][MAX_BUTTON];
    HRESULT hr;
    int nCntButton;
    for (int nCntCntoroller = 0; nCntCntoroller < MAX_CONTROLLER; nCntCntoroller++)
    {
        //�\���L�[�̓��͂��A�����Ȃ��悤�ɂ��Ă���
        g_Controller[nCntCntoroller].State.rgdwPOV[0] = BUTTON_NEUTRAL;

        if (g_Controller[nCntCntoroller].pDIJoypad != NULL)
        {

            hr = g_Controller[nCntCntoroller].pDIJoypad->Poll();
            //if (FAILED(hr)) 
            //{
            //    hr = g_Controller[nCntCntoroller].pDIJoypad->Acquire();
            //}

            //�f�o�C�X����f�[�^���擾
            if (SUCCEEDED(hr = g_Controller[nCntCntoroller].pDIJoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
            {


                g_Controller[nCntCntoroller].State.lY = ControllerState.lY;
                g_Controller[nCntCntoroller].State.lX = ControllerState.lX;

                g_Controller[nCntCntoroller].State.lZ = ControllerState.lZ;		//�X�e�B�b�N�̉��iZ���j
                g_Controller[nCntCntoroller].State.lRz = ControllerState.lRz;		//�X�e�B�b�N�̏c�iZ��]�j


                                                                                    //�\���L�[�̏��
                for (int nCnt = 0; nCnt < 3; nCnt++)
                {
                    //�\���L�[�v���X����ۑ�
                    g_Controller[nCntCntoroller].State.rgdwPOV[nCnt] = ControllerState.rgdwPOV[nCnt];

                }


                //�{�^���̏��
                for (nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
                {
                    //�L�[�g���K�[����ۑ�
                    g_Controller[nCntCntoroller].Trigger.rgbButtons[nCntButton] = (g_Controller[nCntCntoroller].State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

                    //�L�[�����[�X����ۑ�
                    g_Controller[nCntCntoroller].Release.rgbButtons[nCntButton] = (g_Controller[nCntCntoroller].State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

                    //�L�[�v���X����ۑ�
                    g_Controller[nCntCntoroller].State.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];

                }
            }
            else
            {
                //�R���g���[���[�̃A�N�Z�X�����擾
                g_Controller[nCntCntoroller].pDIJoypad->Acquire();
            }
        }
    }
}

//=============================================================================
// [GetJoypadPress] �{�^���̃v���X��Ԃ��擾
// Author:KUDO AYANO
//
// ����
//  [in] nContllre : �R���g���[���̔ԍ�
//  [in] nButton   : �{�^���̔ԍ�
//
//=============================================================================
bool GetJoypadPress(int nController, int nButton)
{
    return(g_Controller[nController].State.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// �{�^���̃g���K�[��Ԃ��擾
// Author:KUDO AYANO
//
// ����
//  [in] nContllre : �R���g���[���̔ԍ�
//  [in] nButton   : �{�^���̔ԍ�
//
//=============================================================================
bool GetJoypadTrigger(int nController, int nButton)
{
    return (g_Controller[nController].Trigger.rgbButtons[nButton] & 0x80) ? true : false;;
}

//=============================================================================
// �{�^���̃����[�X��Ԃ��擾
// Author:KUDO AYANO
//
// ����
//  [in] nContllre : �R���g���[���̔ԍ�
//  [in] nButton   : �{�^���̔ԍ�
//
//=============================================================================
bool GetJoypadRelease(int nController, int nButton)
{
    return(g_Controller[nController].Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// �R���g���[���̏�Ԃ��擾
// Author:KUDO AYANO
//=============================================================================
DIJOYSTATE2 GetController(int nContllre)
{
    return g_Controller[nContllre].State;
}

//=============================================================================
// �R���g���[���[�I������
// Author:KUDO AYANO
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