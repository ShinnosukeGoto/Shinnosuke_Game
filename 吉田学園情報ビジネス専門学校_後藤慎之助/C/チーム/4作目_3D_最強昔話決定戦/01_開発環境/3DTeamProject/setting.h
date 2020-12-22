//=============================================================================
//
// �ŋ��̘b�����̏��� [setting.h] (�ݒ�I��)
// Author:�㓡�T�V��
//
//=============================================================================
#ifndef _SETTING_H_
#define _SETTING_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
//�\���̒�`
//=============================================================================

typedef enum
{
    SETTINGTYPE_PLAYER,	        //�v���C���[
    SETTINGTYPE_ROUND,	        //���E���h/��Փx�I��
    SETTINGTYPE_TIME,           //��������
    SETTINGTYPE_START_POS,		//�J�n�ʒu
    SETTINGTYPE_SUDDEN_DEATH,   //�T�h���f�X
    SETTINGTYPE_STAGE,          //�X�e�[�W
    SETTINGTYPE_OK,             //OK
    SETTINGTYPE_MAX		        //�ݒ�̎�ނ̍ő吔
}SETTINGTYPE;//�ݒ�̎��

typedef enum
{
    SETTINGSTAGE_NORMAL,	        //�m�[�}��
    SETTINGSTAGE_PLAIN,             //����
    SETTINGSTAGE_RUIN,		        //���
    SETTINGSTAGE_CROSS,		        //�N���X
    SETTINGSTAGE_RANDOM,		    //�����_��
    SETTINGSTAGE_MAX		        //�ݒ�̎�ނ̍ő吔
}SETTINGSTAGE;//�X�e�[�W�I���̎��

typedef enum
{
    SETTINGLEVEL_NORMAL,	        //�ӂ�
    SETTINGLEVEL_EXPERT,	        //�G�L�X�p�[�g
    SETTINGLEVEL_MAX		        //�ݒ�̎�ނ̍ő吔
}SETTINGLEVEL;    //�X�g�[���[�̓�Փx�I��

//�ݒ�̏��
typedef struct
{
    int nMaxPlayer;         //�v���C���[�̍ő吔
    int nMaxRound;          //��惉�E���h�̍ő吔
    int nTimeLimit;         //��������
    bool bRandomStartPos;   //�����_���ȊJ�n�ʒu���ǂ���
    bool bSuddenDeath;      //�T�h���f�X�����邩�ǂ���
    int nStage;             //�X�e�[�W�I��
    int nLevel;             //��Փx
    SETTINGTYPE type;       //����ݒ蒆��
}SETTING;

//=============================================================================
//�v���g�^�C�v�錾
//=============================================================================
void InitSetting(void);
void UpdateSetting(void);
void UninitSetting(void);
void DrawSetting(void);
SETTING *GetSetting(void);	//�ݒ�I���̎擾

void InitFirstSetting(void);    //�ݒ��ʂɓ���Ȃ��Ă��A�������������

//=============================================================================
//�}�N����`
//=============================================================================
#define SETTING_UI (32)	//�ݒ�̕\����

//�ŏ��̐ݒ�
#define FIRST_SETTING_MAX_PLAYER 4                //�v���C���[�̍ő吔
#define FIRST_SETTING_MAX_ROUND 3                 //��惉�E���h�̍ő吔
#define FIRST_SETTING_TIME_LIMIT 3                //��������
#define FIRST_SETTING_RANDOM_START_POS false      //�����_���ȊJ�n�ʒu���ǂ���
#define FIRST_SETTING_SUDDEN_DEATH true           //�T�h���f�X�����邩�ǂ���
#define FIRST_SETTING_STAGE SETTINGSTAGE_NORMAL   //�X�e�[�W�I��
#define FIRST_SETTING_LEVEL SETTINGLEVEL_NORMAL   //��Փx�I��

//�ݒ�̍��ږ��̑傫��
#define SUBJECT_SIZE_X 180.0f
#define SUBJECT_SIZE_Y 90.0f

//�ݒ�̍��ږ��̈ʒu
#define SUBJECT_PLAYER_POS D3DXVECTOR3(250.0f,135.0f,0.0f)
#define SUBJECT_ROUND_POS D3DXVECTOR3(250.0f,225.0f,0.0f)
#define SUBJECT_TIME_LIMIT_POS D3DXVECTOR3(250.0f,315.0f,0.0f)
#define SUBJECT_RANDOM_START_POS D3DXVECTOR3(250.0f,405.0f,0.0f)
#define SUBJECT_SUDDEN_DEATH_POS D3DXVECTOR3(250.0f,495.0f,0.0f)
#define SUBJECT_STAGE_POS D3DXVECTOR3(250.0f,585.0f,0.0f)

//�I�����̑傫��
#define CHOICE_SIZE_X 80.0f
#define CHOICE_SIZE_Y 40.0f

//�I�����̈ʒu
#define CHOICE_PLAYER_POS D3DXVECTOR3(1000.0f,135.0f,0.0f)
#define CHOICE_ROUND_POS D3DXVECTOR3(1000.0f,225.0f,0.0f)
#define CHOICE_TIME_LIMIT_POS D3DXVECTOR3(1000.0f,315.0f,0.0f)
#define CHOICE_RANDOM_START_POS D3DXVECTOR3(1000.0f,405.0f,0.0f)
#define CHOICE_SUDDEN_DEATH_POS D3DXVECTOR3(1000.0f,495.0f,0.0f)
#define CHOICE_STAGE_POS D3DXVECTOR3(1000.0f,585.0f,0.0f)

//OK�̑傫���ƈʒu
#define OK_SIZE_X 75.0f
#define OK_SIZE_Y 50.0f
#define OK_POS D3DXVECTOR3(SCREEN_WIDTH / 2, 675.0f, 0.0f)

//�J�[�\���֘A
#define COUNT_MOVE_CURSOR 12                                    //�J�[�\���𓮂�����܂ł̃t���[��
#define CURSOR_SIZE 40.0f                                       //�J�[�\���̑傫��
#define LEFT_CURSOR_POS D3DXVECTOR3(-120.0f,0.0f,0.0f)          //���J�[�\���̈ʒu
#define RIGHT_CURSOR_POS D3DXVECTOR3(120.0f,0.0f,0.0f)          //�E�J�[�\���̈ʒu

#endif