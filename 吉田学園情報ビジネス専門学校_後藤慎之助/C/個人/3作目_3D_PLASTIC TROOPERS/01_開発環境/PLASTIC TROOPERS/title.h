//=============================================================================
//
// PLASTIC TROOPERS�̏��� [title.h] (�^�C�g��)
// Author:�㓡�T�V��
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_
#define _CRT_SECURE_NO_WARNINGS_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define PRESS_ENTER_X (460)//�摜�̃T�C�Y�i�c�j
#define PRESS_ENTER_Y (80)//�摜�̃T�C�Y�i���j

#define NORMAL_HARD_X (260)//�摜�̃T�C�Y�i�c�j
#define NORMAL_HARD_Y (70)//�摜�̃T�C�Y�i���j

#define TITLE_UI (8)	//�^�C�g���̕\����

//=============================================================================
//�\���̒�`
//=============================================================================
typedef enum
{
	LEVEL_NORMAL = 0,	//�m�[�}�����x��
	LEVEL_HARD,			//�n�[�h���x��
	LEVEL_MAX			//��Փx�̍ő吔
}LEVEL;	//�L�����y�[���̓�Փx

typedef enum
{
	SURVIVALTIME_1m = 0,	//1��
	SURVIVALTIME_3m,		//3��
	SURVIVALTIME_INFINITY,	//������
	SURVIVALTIME_MAX		//�T�o�C�o�����Ԃ̍ő吔
}SURVIVALTIME;	//�T�o�C�o������

//=============================================================================
//�v���g�^�C�v�錾
//=============================================================================
void InitTitle(void);
void UpdateTitle(void);
void UninitTitle(void);
void DrawTitle(void);
MODE GetModeTitle(void);            //���[�h�I���̎擾
LEVEL GetLevelTitle(void);          //�L�����y�[����Փx�̎擾
SURVIVALTIME GetSurvivalTitle(void);//�T�o�C�o�����Ԃ̎擾

#endif