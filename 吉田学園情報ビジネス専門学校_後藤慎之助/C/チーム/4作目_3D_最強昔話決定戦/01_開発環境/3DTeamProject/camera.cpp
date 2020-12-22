//=============================================================================
//
// �ŋ��̘b�����̏��� [camera.cpp] (�J����)
// Author : �㓡�T�V��
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "controller.h"
#include "sound.h"
#include "player.h"
#include "setting.h"
#include "controller.h"
#include "boss.h"
#include "ui.h"

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
CAMERA g_Camera;	//�J�������

//=============================================================================
// �J��������������
// Author:�㓡�T�V��
//=============================================================================
void InitCamera(void)
{
    // �J�����̈ʒu�E�����_�E�������ݒ肷��
    g_Camera.posV = D3DXVECTOR3(CAMERA_POS_X, CAMERA_POS_Y, CAMERA_POS_Z);      //�ʒu
    g_Camera.posR = D3DXVECTOR3(CAMERA_ROT_X, CAMERA_ROT_Y, CAMERA_ROT_Z);      //�����_
    g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);          //�@���x�N�g��
    g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);           //����


    //�\���̂̏�����
    g_Camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_Camera.state = CAMERASTATE_NORMAL;
    g_Camera.nCntReachBoss = 0;
    g_Camera.bBossBattle = false;

    // ����
    g_Camera.fDistance = sqrtf(
        powf((g_Camera.posV.x - g_Camera.posR.x), 2) +
        powf((g_Camera.posV.z - g_Camera.posR.z), 2));

    //�X�g�[���[���ǂ���
    SETTING* pSetting = GetSetting();
    if (pSetting->nMaxPlayer == 1)
    {
        g_Camera.state = CAMERASTATE_STORY;
    }
}

//=============================================================================
// �J�����I������
// Author:�㓡�T�V��
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// �J�����X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateCamera(void)
{
    //���@�̎擾
    PLAYER*pPlayer = GetPlayer();

    //�X�g�[���[���́A���@�ւ̒Ǐ]
    switch (g_Camera.state)
    {
    case CAMERASTATE_STORY:

        //�J�����̈ʒu�����@�ɒǏ]�����邽�߂́A�ړI�l
        g_Camera.posVDest = D3DXVECTOR3(pPlayer->pos.x - sinf((g_Camera.rot.z)) * g_Camera.fDistance, 
            g_Camera.posV.y, (pPlayer->pos.z - cosf((g_Camera.rot.z))* g_Camera.fDistance));

        //�����_�����@�ɒǏ]�����邽�߂́A�ړI�l
        g_Camera.posRDest = D3DXVECTOR3((pPlayer->pos.x + sinf(pPlayer->rot.z)), g_Camera.posR.y,
            (pPlayer->pos.z + cosf(pPlayer->rot.z)));

        //�J�����̈ʒu���A�ړI�l�ɋ߂Â���
        g_Camera.posV.x += (g_Camera.posVDest.x - g_Camera.posV.x) * CAMERA_MOVE;

        //�J�����̒����_���A�ړI�l�ɋ߂Â���
        g_Camera.posR.x += (g_Camera.posRDest.x - g_Camera.posR.x) * CAMERA_MOVE;

        break;

    case CAMERASTATE_REACH_BOSS:

        //�{�X�����̃J�E���g
        g_Camera.nCntReachBoss++;

        // �����_���{�X�̂ق��ֈړ�����
        g_Camera.posRDest = D3DXVECTOR3(BOSS_FIRST_POS_X, CAMERA_BOSS_REACH_Y + 50.0f
            , CAMERA_BOSS_REACH_Z);

        // �J�����̈ʒu���{�X�̂ق��ֈړ�����
        g_Camera.posVDest = D3DXVECTOR3(CAMERA_BOSS_REACH_X, CAMERA_BOSS_REACH_Y, CAMERA_BOSS_REACH_Z);

        // �J�����̈ʒu��ړI�l�֋߂Â���
        g_Camera.posV += (g_Camera.posVDest - g_Camera.posV) * CAMERA_MOVE;

        // �J�����̌�����ړI�l�֋߂Â���
        g_Camera.posR += (g_Camera.posRDest - g_Camera.posR) * CAMERA_MOVE;

        //1�b��ə��K�̃��[�V����
        if (g_Camera.nCntReachBoss == CAMERA_BOSS_TIME / 3)
        {
            BOSS *pBoss = GetBoss();
            pBoss->MotionState = MOTION_ROAR_BOSS;
        }

        //1.5�b��ə��K��
        else if (g_Camera.nCntReachBoss == CAMERA_BOSS_TIME / 2)
        {
            PlaySound(SOUND_LABEL_SE_ROAR);
        }

        //��������߂�ƂƂ��ɁA�J�����O�ŏ�����i�߂�
        else if (g_Camera.nCntReachBoss > CAMERA_BOSS_TIME)
        {
            //�ʒu�ƒ����_��߂�
            g_Camera.posV.y = CAMERA_POS_Y;
            g_Camera.posV.z = CAMERA_POS_Z;
            g_Camera.posR.y = CAMERA_ROT_Y;
            g_Camera.posR.z = CAMERA_ROT_Z;

            //�{�X�̃o�[
            SetUI(D3DXVECTOR3(SCREEN_WIDTH - 50.0f, SCREEN_HEIGHT / 2, 0.0f), 50.0f, 500.0f, UITYPE_020);                               //�{�X��HP�o�[�̔w�i
            SetUI(D3DXVECTOR3(SCREEN_WIDTH - 50.0f, SCREEN_HEIGHT / 2, 0.0f), 50.0f - BAR_EXTEND, 500.0f - BAR_EXTEND, UITYPE_021);     //�{�X��HP�o�[
            SetUI(D3DXVECTOR3(SCREEN_WIDTH - 50.0f, 90.0f, 0.0f), 70.0f, 50.0f, UITYPE_022);                                            //BOSS�̕���

            //�J�������X�g�[���[���[�h�̏�Ԃ�
            g_Camera.state = CAMERASTATE_STORY;
            g_Camera.nCntReachBoss = 0;

            //�{�X�Ȃ�
            PlaySound(SOUND_LABEL_BGM_SUDDEN_DEATH);

            //�{�X��J�n
            g_Camera.bBossBattle = true;
        }

        break;
    }
}

//=============================================================================
// �J�����̐ݒ�
// Author:�㓡�T�V��
//=============================================================================
void SetCamera(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���^

                                                // �v���W�F�N�V�����}�g���b�N�X�̏�����
    D3DXMatrixIdentity(&g_Camera.mtxProjection);
    // �v���W�F�N�V�����}�g���b�N�X�̍쐬
    D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
        D3DXToRadian(45.0f),
        (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
        10.0f,
        CAMERA_VIEW_RANGE);		// ����
                                // �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
    pDevice->SetTransform(D3DTS_PROJECTION,
        &g_Camera.mtxProjection);

    // �r���[�}�g���b�N�X�̏�����
    D3DXMatrixIdentity(&g_Camera.mtxView);
    // �r���[�}�g���b�N�X�̍쐬
    D3DXMatrixLookAtLH(&g_Camera.mtxView,
        &g_Camera.posV,
        &g_Camera.posR,
        &g_Camera.vecU);
    // �r���[�}�g���b�N�X�̐ݒ�
    pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);
}

//=============================================================================
// �J�����̏󋵂��擾
// Author:�㓡�T�V��
//=============================================================================
CAMERA * GetCamera(void)
{
    return &g_Camera;
}