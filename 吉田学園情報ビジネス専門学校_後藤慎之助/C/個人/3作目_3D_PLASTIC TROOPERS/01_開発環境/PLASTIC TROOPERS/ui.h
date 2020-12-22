//=============================================================================
//
// PLASTIC TROOPERS [ui.h]�̏��� (�Q�[����ʂ�UI)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_
#define _CRT_SECURE_NO_WARNINGS_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_UI (32)          //1��ʂɔz�u����UI�̍ő吔
#define BAR_EXTEND (7.7f)	 //�o�[�̔w�i���L����

//=============================================================================
//�\���̒�`
//=============================================================================
typedef enum
{
    UITYPE_000 = 0,		//�X�R�A�̔w�i�i�E��j
    UITYPE_001,			//�L�����y�[��
    UITYPE_002,			//�T�o�C�o��
    UITYPE_003,			//�o�[�T�X
    UITYPE_004,			//�v���N�e�B�X
    UITYPE_005,     	//�K�E�Q�[�W�̔w�i
    UITYPE_006,         //HP/ENERGY�Q�[�W�̔w�i
    UITYPE_007,         //ON/OFF
    UITYPE_008,         //HP�̕���
    UITYPE_009,         //ENERGY�̕���
    UITYPE_010,         //HP�Q�[�W
    UITYPE_011,         //ENERGY�Q�[�W
    UITYPE_012,         //�K�E�Q�[�W
    UITYPE_013,         //�K�E�̐���
    UITYPE_014,         //��
    UITYPE_015,         //�e
    UITYPE_016,         //Y�{�^��
    UITYPE_017,         //�v���XR
    UITYPE_018,         //�X�s�j���O�u���[�h
    UITYPE_019,         //�X�v���b�h�t�@�C�A
    UITYPE_020,         //�{�X��HP�̔w�i
    UITYPE_021,         //�{�X��HP�o�[
    UITYPE_022,         //BOSS�̕���
    UITYPE_MAX			//��ނ̍ő吔
}UITYPE;	//UI�̎��

typedef struct
{
    D3DXVECTOR3 pos;	//�ʒu
    float fSizeX;		//��
    float fSizeY;       //�c
    UITYPE type;		//���
    int nCounterAnim;	//�A�j���[�V�����J�E���^�[
    int nPatternAnim;	//�A�j���[�V�����p�^�[��
    bool bUse;          //�g�p���Ă��邩�ǂ���
}UI;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitUI(void);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);

void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, UITYPE type);
#endif