//=============================================================================
//
// DirectX3D�̏��� [camera.h] (�J����)
// Author:�㓡�T�V��
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_
#define _CRT_SECURE_NO_WARNINGS_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define CAMERA_POS_Y (220.0f)
#define CAMERA_ROT_Y (50.0f)
#define CAMERA_POS_Z (-380.0f)
#define CAMERA_MOVE (0.15f)
#define CONSIDER_CLOSE (5000.0f)

//�{�X�ɂ��ǂ蒅�����Ƃ��̃J�����̈ʒu
#define CAMERA_BOSS_REACH_X (0.0f)
#define CAMERA_BOSS_REACH_Y (100.0f)
#define CAMERA_BOSS_REACH_Z (2500.0f)

//�{�X�ɂ��ǂ蒅�������́A�{�X�𒭂߂鎞��
#define CAMERA_BOSS_TIME (180)

//=============================================================================
//�\���̒�`
//=============================================================================
//�J�����̏��
typedef enum
{
    CAMERASTATE_NORMAL = 0,	//�ʏ�
    CAMERASTATE_BOSS,		//�{�X��ɓ���Ƃ�
    CAMERASTATE_MAX			//��Ԃ̍ő吔
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
	bool bLockOn;               //���b�N�I�����Ă��邩�ǂ���
    bool bBossMode;             //�{�X���[�h���ǂ���
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