//=============================================================================
//
// PLASTIC TROOPERS�̏��� [camera.cpp] (�J����)
// Author : �㓡�T�V��
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "controller.h"
#include "effect.h"
#include "boss.h"
#include "sound.h"

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
CAMERA g_Camera;	//�J�������

bool g_bUnderTrigger;   //�\���L�[���̃g���K�[
bool g_bLeftTrigger;    //�\���L�[���̃g���K�[
bool g_bRightTrigger;   //�\���L�[�E�̃g���K�[
float g_fAngleCamera;   //���b�N�I�����̃J�����̌�����ς���p�x
float g_fCamera_posR_Height;  //�J�����̒����_�̍���

int g_nCntReachBoss;    //�{�X�𒍎�����J�E���^�[

//=============================================================================
// �J��������������
//=============================================================================
void InitCamera(void)
{
    // �J�����̈ʒu�E�����_�E�������ݒ肷��
    g_Camera.posV = D3DXVECTOR3(0.0f, CAMERA_POS_Y, CAMERA_POS_Z);      //�ʒu
    g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          //�����_
    g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);          //�@���x�N�g��
    g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);           //����

    //�O���[�o���ϐ��̏�����
    g_bUnderTrigger = false;
    g_bLeftTrigger = false;
    g_bRightTrigger = false;
    g_fAngleCamera = 0.0f;
    g_fCamera_posR_Height = 0.0f;
    g_nCntReachBoss = 0;

    //�\���̂̏�����
    g_Camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_Camera.state = CAMERASTATE_NORMAL;
    g_Camera.bLockOn = false;
    g_Camera.bBossMode = false;

    // ����
    g_Camera.fDistance = sqrtf(
        powf((g_Camera.posV.x - g_Camera.posR.x), 2) +
        powf((g_Camera.posV.z - g_Camera.posR.z), 2));
}

//=============================================================================
// �J�����I������
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// �J�����X�V����
//=============================================================================
void UpdateCamera(void)
{
    DIJOYSTATE2 Controller = GetController();
    PLAYER*pPlayer = GetPlayer();
    ENEMY*pEnemy = GetEnemy();
    BOSS*pBoss = GetBoss();

    //�ʏ펞
    if (g_Camera.state == CAMERASTATE_NORMAL)
    {
        //�����_�̓]��
        if (g_Camera.bLockOn == false)  //���b�N�I�����Ă��Ȃ��Ƃ��Ɏg����
        {
            if (Controller.lZ > 0)//�E
            {
                g_Camera.rot.y += 0.05f;
            }
            if (Controller.lZ < 0)//��
            {
                g_Camera.rot.y -= 0.05f;
            }

            // �����_�����f���������Ă�������ֈړ�����
            g_Camera.posRDest = D3DXVECTOR3((pPlayer->pos.x + sinf(pPlayer->rot.y)), g_Camera.posR.y,
                (pPlayer->pos.z + cosf(pPlayer->rot.y)));
        }

        //���b�N�I��
        else
        {
            //�ʏ탂�[�h
            if (g_Camera.bBossMode == false)
            {
                for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
                {

                    //�g���Ă���Ȃ�
                    if (pEnemy->bUse == true)
                    {
                        //��ԋ߂��G�Ɍ���
                        if (pEnemy->nRank == 1)
                        {
                            //�������߂��Ȃ�
                            if (pEnemy->fDistance < CONSIDER_CLOSE)
                            {
                                float fEposx = pEnemy->pos.x, fEposz = pEnemy->pos.z;		//�G�̍��W
                                float fPposx = pPlayer->pos.x, fPposz = pPlayer->pos.z;	    //�v���C���[�̍��W

                                g_fAngleCamera = atan2f((fEposx - fPposx), (fEposz - fPposz));	//�p�x�����߂�

                                                                                                // �����_�����f���������Ă�������ֈړ�����
                                g_Camera.posRDest = D3DXVECTOR3((pPlayer->pos.x + sinf(pPlayer->rot.y)), g_Camera.posR.y
                                    , (pPlayer->pos.z + cosf(pPlayer->rot.y))); //�c�����̒���

                                                                                //�^�[�Q�b�g�}�[�N
                                EFFECT*pEffect = GetEffect();
                                for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
                                {
                                    //���g�p�Ȃ�
                                    if (pEffect->bUse == false)
                                    {
                                        SetEffect(D3DXVECTOR3(pEnemy->pos.x - sinf(g_fAngleCamera)*pEnemy->size.x, pEnemy->pos.y + pEnemy->size.y / 2, pEnemy->pos.z - cosf(g_fAngleCamera)*pEnemy->size.z), TARGET_SIZE
                                            , D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECTTYPE_000);
                                    }
                                }

                                break;
                            }
                        }
                    }

                }
            }

            //�{�X���[�h
            else
            {
                float fBposx = pBoss->pos.x, fBposz = pBoss->pos.z;		//�G�̍��W
                float fPposx = pPlayer->pos.x, fPposz = pPlayer->pos.z;	    //�v���C���[�̍��W

                g_fAngleCamera = atan2f((fBposx - fPposx), (fBposz - fPposz));	//�p�x�����߂�

                // �����_�����f���������Ă�������ֈړ�����
                g_Camera.posRDest = D3DXVECTOR3((pPlayer->pos.x + sinf(pPlayer->rot.y)), g_Camera.posR.y
                    , (pPlayer->pos.z + cosf(pPlayer->rot.y))); //�c�����̒���

                //�^�[�Q�b�g�}�[�N
                EFFECT*pEffect = GetEffect();
                for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
                {
                    //���g�p�Ȃ�
                    if (pEffect->bUse == false)
                    {
                        SetEffect(D3DXVECTOR3(pBoss->pos.x - sinf(g_fAngleCamera)*pBoss->size.x, pBoss->pos.y + pBoss->size.y / 2, pBoss->pos.z - cosf(g_fAngleCamera)*pBoss->size.z), TARGET_SIZE
                            , D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECTTYPE_000);
                    }
                }
            }

            //���b�N�I�������G�̂ق�������
            g_Camera.rot.y = g_fAngleCamera;    //�������̒���

        }

        //�J�����̈ʒu���L�����ɒǏ]������
        g_Camera.posVDest = D3DXVECTOR3(pPlayer->pos.x - sinf((g_Camera.rot.y)) * g_Camera.fDistance, g_Camera.posV.y, (pPlayer->pos.z - cosf((g_Camera.rot.y))* g_Camera.fDistance));

        // �L�����ړ����̃J�����̈ʒu
        g_Camera.posV += (g_Camera.posVDest - g_Camera.posV) * CAMERA_MOVE;

        // �L�����ړ����̃J�����̌���
        g_Camera.posR.x += (g_Camera.posRDest.x - g_Camera.posR.x) * CAMERA_MOVE;
        g_Camera.posR.z += (g_Camera.posRDest.z - g_Camera.posR.z) * CAMERA_MOVE;

        //�J�����̌������E�X�e�B�b�N�ŕς���
        if (Controller.lRz > 0)//�J�����̌�����
        {
            g_fCamera_posR_Height -= 2.5f;
        }
        else if (Controller.lRz < 0)//�J�����̌�����
        {
            g_fCamera_posR_Height += 2.5f;
        }

        //�J�����̌����̐���
        if (g_fCamera_posR_Height > 80.0f)
        {
            g_fCamera_posR_Height = 80.0f;
        }
        else if (g_fCamera_posR_Height < -30.0f)
        {
            g_fCamera_posR_Height = -30.0f;
        }

        g_Camera.posR.y = CAMERA_ROT_Y + pPlayer->pos.y + g_fCamera_posR_Height;    //�J�����̌���

                                                                                    // �W�����v�̎��̎��_�ړ�
        g_Camera.posV.y = CAMERA_POS_Y + pPlayer->pos.y;    //�J�����̈ʒu

                                                            //L1/L2�Ń��b�N�I������
        if (GetJoypadPress(BUTTON_L1) || GetJoypadPress(BUTTON_L2))
        {
            g_Camera.bLockOn = true;
        }
        else
        {
            g_Camera.bLockOn = false;
        }
    }

    //�{�X��ɂ��ǂ蒅�����Ƃ�
    else
    {
        //�{�X�����̃J�E���g
        g_nCntReachBoss++;

        float fBposx = pBoss->pos.x, fBposz = pBoss->pos.z;		//�{�X�̍��W

        g_fAngleCamera = atan2f((CAMERA_BOSS_REACH_X - fBposx), (CAMERA_BOSS_REACH_Z-fBposz));	//�p�x�����߂�

        // �����_�����f���������Ă�������ֈړ�����
        g_Camera.posRDest = D3DXVECTOR3(CAMERA_BOSS_REACH_X, CAMERA_BOSS_REACH_Y+50
            , BOSS_FIRST_POS_Z); //�c�����̒���
        
        //�J�����̈ʒu���L�����ɒǏ]������
        g_Camera.posVDest = D3DXVECTOR3(CAMERA_BOSS_REACH_X, CAMERA_BOSS_REACH_Y, CAMERA_BOSS_REACH_Z);

        // �L�����ړ����̃J�����̈ʒu
        g_Camera.posV += (g_Camera.posVDest - g_Camera.posV) * CAMERA_MOVE;

        // �L�����ړ����̃J�����̌���
        g_Camera.posR += (g_Camera.posRDest - g_Camera.posR) * CAMERA_MOVE;

        //��������߂�ƂƂ��ɁA�J�����O�ŏ�����i�߂�
        if (g_nCntReachBoss > CAMERA_BOSS_TIME)
        {
            //�Q�[�����~�߂�
            StopSound(SOUND_LABEL_BGM001);

            //�J������ʏ��Ԃ�
            g_Camera.state = CAMERASTATE_NORMAL;
            g_nCntReachBoss = 0;
            g_Camera.bBossMode = true;

            //�{�X�Ȃ�
            PlaySound(SOUND_LABEL_BGM004);

            //�G�l�~�[�����ׂĖ��g�p��
            pEnemy = GetEnemy();
            for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
            {
                if (pEnemy->bUse == true)
                {
                    pEnemy->bUse = false;
                }
            }
        }
    }
}

//=============================================================================
// �J�����̐ݒ�
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
//�J�����̏󋵂��擾
//=============================================================================
CAMERA * GetCamera(void)
{
    return &g_Camera;
}