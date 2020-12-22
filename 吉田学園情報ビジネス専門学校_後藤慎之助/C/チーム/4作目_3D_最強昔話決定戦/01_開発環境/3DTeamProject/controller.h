//=============================================================================
//
// �ŋ��̘b�����̏���[controller.h](�R���g���[���[�ł̑���j
// Author:KUDO AYANO
//
//=============================================================================
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

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

#define BUTTON_UP_RIGHT     (4500)	//�\���L�[��E
#define BUTTON_RIGHT_DOWN  (13500)	//�\���L�[�E��
#define BUTTON_DOWN_LEFT   (22500)	//�\���L�[����
#define BUTTON_LEFT_UP   (31500)	//�\���L�[����

#define BUTTON_NEUTRAL -1       //�������͂��Ă��Ȃ��Ƃ�



#define CROSSZONE (4500)        //�\���L�[�̋��e�͈�

//�v���C���[�̔ԍ�
#define PLAYER_1 (0)     //�v���C���[�̃R���g���[���[�ԍ� (1P)
#define PLAYER_2 (1)     //�v���C���[�̃R���g���[���[�ԍ� (2P)
#define PLAYER_3 (2)     //�v���C���[�̃R���g���[���[�ԍ� (3P)
#define PLAYER_4 (3)     //�v���C���[�̃R���g���[���[�ԍ� (4P)

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
    BUTTON_L3,
    BUTTON_R3,
    BUTTON_BACK,
    BUTTON_START,
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

bool GetJoypadPress(int nController, int nButton);
bool GetJoypadTrigger(int nController, int nButton);
bool GetJoypadRelease(int nController, int nButton);
DIJOYSTATE2 GetController(int nController);

#endif