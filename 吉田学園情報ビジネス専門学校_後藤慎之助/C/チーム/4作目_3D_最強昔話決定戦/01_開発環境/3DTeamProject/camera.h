//=============================================================================
//
// �ŋ��̘b�����̏��� [camera.h] (�J����)
// Author:�㓡�T�V��
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
//�}�N����`
//=============================================================================

//�J�����̈ʒu
#define CAMERA_POS_X (700.0f)
#define CAMERA_POS_Y (2000.0f)
#define CAMERA_POS_Z (400.0f)

//�J�����̒����_
#define CAMERA_ROT_X (700.0f)
#define CAMERA_ROT_Y (-400.0f)
#define CAMERA_ROT_Z (700.0f)

#define CAMERA_MOVE (0.15f) //�J�����̈ړ���

//�{�X�ɂ��ǂ蒅�����Ƃ��̃J�����̈ʒu
#define CAMERA_BOSS_REACH_X (4900.0f)
#define CAMERA_BOSS_REACH_Y (100.0f)
#define CAMERA_BOSS_REACH_Z (600.0f)

//�{�X�ɂ��ǂ蒅�������́A�{�X�𒭂߂鎞��
#define CAMERA_BOSS_TIME (180)

//=============================================================================
//�\���̒�`
//=============================================================================

//�J�����̏��
typedef enum
{
    CAMERASTATE_NORMAL = 0,	    //�ʏ�
    CAMERASTATE_STORY,		    //�X�g�[���[
    CAMERASTATE_REACH_BOSS,	    //�{�X��ɓ���Ƃ�
    CAMERASTATE_MAX			    //��Ԃ̍ő吔
}CAMERASTATE;

typedef struct
{
    D3DXVECTOR3 posV;           //�J�����̍��W
    D3DXVECTOR3 posR;           //�����_
    D3DXVECTOR3 vecU;           //������̃x�N�g��
    D3DXVECTOR3 posVDest;       //�J�����̍��W�i�ړI�l�j
    D3DXVECTOR3 posRDest;       //�J�����̒����_�i�ړI�l�j
    D3DXMATRIX mtxProjection;   //�v���W�F�N�V�����}�g���N�X
    D3DXMATRIX mtxView;         //�r���[�}�g���N�X
    D3DXVECTOR3 rot;            //�����擾�p(���[�e�[�V����,��])
    CAMERASTATE state;          //�J�����̏��
    float fDistance;            //���_-�����_�̋���
    int nCntReachBoss;          //�{�X��ɂ��ǂ蒅�������̃J�E���^�[
    bool bBossBattle;           //�{�X�킩�ǂ���
}CAMERA;//�J�����̏��

//=============================================================================
//�v���g�^�C�v�錾
//=============================================================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);	//Draw�Ɠ���
CAMERA*GetCamera(void);
#endif