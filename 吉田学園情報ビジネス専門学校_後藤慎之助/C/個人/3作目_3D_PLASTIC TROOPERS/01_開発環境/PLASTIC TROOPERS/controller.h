//=============================================================================
//
// PLASTIC TROOPERS�̏���[controller.h](�R���g���[���[�ł̑���j
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#define _CRT_SECURE_NO_WARNINGS_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
// �O���ϐ�
//=============================================================================
extern LPDIRECTINPUT8		pDInput;
extern LPDIRECTINPUTDEVICE8	pDIKeyboard;
extern BYTE					diKeyState[256];
extern LPDIRECTINPUTDEVICE8	pDIJoypad;
extern DWORD				diJoyState;
extern LPDIRECTINPUTDEVICE8	pDIMouse;
extern DIMOUSESTATE			diMouseState;

//=============================================================================
//�}�N����`
//=============================================================================
#define BUTTON_UP     (0)	    //�\���L�[��
#define BUTTON_DOWN   (18000)	//�\���L�[��
#define BUTTON_LEFT   (27000)	//�\���L�[��
#define BUTTON_RIGHT  (9000)	//�\���L�[�E

#define CROSSZONE (4500)        //�\���L�[�̋��e�͈�
//
//#define SAFE_RELEASE(x)	if(x){x->Release();x=NULL;} //�J���������ȒP�ɂ���
//

//=============================================================================
//�\����/�񋓌^��`
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
}BUTTON;//�{�^���̐�

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾 function-prototypes
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