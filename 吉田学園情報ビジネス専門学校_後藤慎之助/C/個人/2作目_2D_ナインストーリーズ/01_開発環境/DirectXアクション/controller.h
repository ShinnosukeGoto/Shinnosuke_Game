//#pragma once
////=============================================================================
////
//// �R���g���[���[���͂̏��� [controller.h]
//// Author : �㓡�T�V��
////
////=============================================================================
//#include "main.h"
////=============================================================================
//// �O���ϐ�
////=============================================================================
//extern LPDIRECTINPUT8		pDInput;
//extern LPDIRECTINPUTDEVICE8	pDIKeyboard;
//extern BYTE					diKeyState[256];
//extern LPDIRECTINPUTDEVICE8	pDIJoypad;
//extern DWORD				diJoyState;
//extern LPDIRECTINPUTDEVICE8	pDIMouse;
//extern DIMOUSESTATE			diMouseState;
////=============================================================================
////�\����/�񋓌^��`
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
//}BUTTON;//�{�^���̐�
//
//typedef enum
//{
//	CONTROLLER_KEY_JUMP,				//�W�����v������܂Ƃ߂�����
//	CONTROLLER_KEY_ATTACK,				//�U��
//	CONTROLLER_KEY_MAGIC,				//���@
//										//CONTROLLER_KEY_1,					//
//										//CONTROLLER_KEY_2,					//
//										//CONTROLLER_KEY_3,					//
//										//CONTROLLER_KEY_4,					//
//										//CONTROLLER_KEY_5,					//
//										//CONTROLLER_KEY_6,					//
//										//CONTROLLER_KEY_7,					//
//										//CONTROLLER_KEY_8,					//
//
//										CONTROLLER_CROSS_UP,				// �\���L�[ ��
//										CONTROLLER_CROSS_DOWN,				// �\���L�[ ��
//										CONTROLLER_CROSS_LEFT,				// �\���L�[ ��
//										CONTROLLER_CROSS_RIGHT,			// �\���L�[ �E
//										CONTROLLER_MAX_CONTROLLER			//�ő吔
//}CONTROLLER;//���z�R���g���[��
//
//			//-----------------------------------------------------------------------------
//			// �v���g�^�C�v�錾 function-prototypes
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
//// Joypad���p�}�N��
////-----------------------------------------------------------------------------
//// �W���C�p�b�h�̊e�{�^�����������A�R�Q�r�b�g���̎��̈ʒu�Ɋ��蓖�Ă�
//// ���̃r�b�g���I���Ȃ�΁A���̃{�^���������ꂽ���ƂɂȂ�
//
//#define BUTTON_UP     0x00000001l	//�����L�[��(dijs.IY<0)
//#define BUTTON_DOWN   0x00000002l	//�����L�[��(dijs.IY>0)
//#define BUTTON_LEFT   0x00000004l	//�����L�[��(dijs.IX<0)
//#define BUTTON_RIGHT  0x00000008l	//�����L�[�E(dijs.IX>0)
//
////#define BUTTON_X      (0)	//�w�{�^��(dijs.rgbButtons[3]&0x80)
////#define BUTTON_Y      (1)	//�x�{�^��(dijs.rgbButtons[4]&0x80)
////#define BUTTON_A      (2)	//�`�{�^��(dijs.rgbButtons[0]&0x80)
////#define BUTTON_B      (3)	//�a�{�^��(dijs.rgbButtons[1]&0x80)
////#define BUTTON_L1      (4)	//�b�{�^��(dijs.rgbButtons[2]&0x80)
////#define BUTTON_R1      (5)	//�y�{�^��(dijs.rgbButtons[5]&0x80)
////#define BUTTON_L2      (6)	//�k�{�^��(dijs.rgbButtons[6]&0x80)
////#define BUTTON_R3      (7)	//�q�{�^��(dijs.rgbButtons[7]&0x80)
////#define BUTTON_START  (11)	//�r�s�`�q�s�{�^��(dijs.rgbButtons[8]&0x80)
////#define BUTTON_M      (12)	//�l�{�^��(dijs.rgbButtons[9]&0x80)
//
//#define SAFE_RELEASE(x)	if(x){x->Release();x=NULL;} //�J���������ȒP�ɂ���
////