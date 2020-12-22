//=============================================================================
//
// �i�C���X�g�[���[�Y�̏��� [sound.h] (�T�E���h)
// Author:�㓡�T�V��
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C���̍\���̒�`
//*****************************************************************************
typedef enum
{
    SOUND_LABEL_BGM000 = 0,		// BGM0 �^�C�g��
    SOUND_LABEL_BGM001,			// BGM1 �Q�[��
    SOUND_LABEL_BGM002,			// BGM2 �n�b�J�[
    SOUND_LABEL_BGM003,			// BGM3 ���U���g
    SOUND_LABEL_BGM004,			// BGM4 �{�X
    SOUND_LABEL_SE_ENTER,		// �G���^�[��
    SOUND_LABEL_SE_JUMP,		// �W�����v��/�I����
    SOUND_LABEL_SE_EXPLOSION2,	// ��̉�
    SOUND_LABEL_SE_SWORD,		// ���A���̉�
    SOUND_LABEL_SE_SHOT,		// �|�̉�
    SOUND_LABEL_SE_EXPLOSION,	// �q�b�g��
    SOUND_LABEL_SE_STAR,		// ����-��
    SOUND_LABEL_SE_ITEM,		// �A�C�e���擾/�񕜉�/���@��
    SOUND_LABEL_SE_VOICE,		// �{�X���j��
    SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif