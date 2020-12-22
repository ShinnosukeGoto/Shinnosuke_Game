//=============================================================================
//
// �ŋ��̘b�����̏��� [sound.h] (�T�E���h)
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
    SOUND_LABEL_BGM_TITLE = 0,	    // BGM �^�C�g��
    SOUND_LABEL_BGM_MENU,		    // BGM ���j���[
    SOUND_LABEL_BGM_GAME,           // BGM �Q�[��
    SOUND_LABEL_BGM_SUDDEN_DEATH,   // BGM �T�h���f�X
    SOUND_LABEL_BGM_RESULT,			// BGM ���U���g
    SOUND_LABEL_SE_SELECT,		    // �I����
    SOUND_LABEL_SE_DECISION,        // ���艹
    SOUND_LABEL_SE_EXPLOSION,	    // ������
    SOUND_LABEL_SE_SET_BOMB, 	    // �{���ݒu��
    SOUND_LABEL_SE_MAN_VOICE,		// �j�������ꂽ���̃{�C�X
    SOUND_LABEL_SE_ITEM,	        // �A�C�e���擾��
    SOUND_LABEL_SE_START,		    // �J�n��
    SOUND_LABEL_SE_WARNING,		    // �x����
    SOUND_LABEL_SE_FINISH,		    // �����I����
    SOUND_LABEL_SE_KICK,		    // �{���L�b�N��
    SOUND_LABEL_SE_WOMAN_VOICE,		// ���������ꂽ���̃{�C�X
    SOUND_LABEL_SE_PINOCCHIO_VOICE,	// �s�m�L�I�����ꂽ���̃{�C�X
    SOUND_LABEL_SE_MAN_VOICE2,		// �j�������ꂽ���̃{�C�X2
    SOUND_LABEL_SE_START_MAN_VOICE,	    // �J�n�̊|����(�j)
    SOUND_LABEL_SE_START_MAN_VOICE2,    // �J�n�̊|����(�j2)
    SOUND_LABEL_SE_START_MAN_VOICE3,    // �J�n�̊|����(�j3)
    SOUND_LABEL_SE_START_WOMAN_VOICE,   // �J�n�̊|����(��)
    SOUND_LABEL_SE_TAKE_DAMAGE_VOICE,   // �_���[�W���󂯂����̐�
    SOUND_LABEL_SE_TAKE_DAMAGE_VOICE2,  // �_���[�W���󂯂����̐�2
    SOUND_LABEL_SE_TAKE_DAMAGE_VOICE3,  // �_���[�W���󂯂����̐�3
    SOUND_LABEL_SE_TAKE_DAMAGE_VOICE4,  // �_���[�W���󂯂����̐�4
    SOUND_LABEL_SE_EXPLOSION2,	        // �����X�^�[�̔�����
    SOUND_LABEL_SE_ROAR,	            // �{�X�̙��K
    SOUND_LABEL_SE_BOSS_EXPLOSION,	    // �{�X�̔�����
    SOUND_LABEL_SE_JUMP,	            // �W�����v��
    SOUND_LABEL_SE_FALL,	            // ������
    SOUND_LABEL_SE_GROUND,	            // �n����
    SOUND_LABEL_SE_SHOT,                // �V���b�g��
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