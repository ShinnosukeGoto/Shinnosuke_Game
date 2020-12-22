//=====================================================
//
// �ŋ��̘b�����̏���[boss.h](�X�g�[���[���[�h�̃{�X)
// Author:�㓡�T�V��
//
//=====================================================

//=====================================================
// �C���N���[�h�t�@�C��
//=====================================================
#include "boss.h"
#include "camera.h"
#include "sound.h"
#include "effect.h"
#include "collision.h"
#include "game.h"
#include "bullet.h"
#include "player.h"
#include "camera.h"
#include "score.h"
#include "time.h"
#include "shadow.h"
#include "block.h"
#include "fire.h"
#include "ui.h"
#include "setting.h"

//=====================================================
// �O���[�o���ϐ���`
//=====================================================
BOSS g_Boss;        //�{�X�̏��

int g_FrameCountBoss;				// ���݂̃t���[����

int g_PlayKeySetIndexBoss;			// �Đ����̃L�[�Z�b�g�C���f�b�N�X

int g_BossCurrentAnimation;	// �Đ����̃A�j���[�V����

int g_nFremeBoss;					// �t���[����

int g_nCntKeyBoss;					// �L�[��

int g_nChangeFlameBoss;			// ���[�V�����̃t���[��

MOTIONSTATE_BOSS g_MotionOldStateBoss;		// 1F�O�̃��[�V�����̍\����

D3DXVECTOR3 g_NumPosBoss[MAX_BOSS_PARTS];		// pos�̍���
D3DXVECTOR3 g_NumRotBoss[MAX_BOSS_PARTS];		// rot�̍���
D3DXVECTOR3 g_NumRotBossDest[MAX_BOSS_PARTS];	// rot�̍���

ANIMATION_BOSS g_aBossMotion[MOTION_MAX_BOSS];	// ���[�V�����̏��
                                                // ���f���p�[�c�̃f�[�^
ModelParts_BOSS g_aBoss[MAX_BOSS_PARTS] =
{
    { "MODEL/boss/00_body.x" },
    { "MODEL/boss/01_head.x" },
    { "MODEL/boss/02_armR.x" },
    { "MODEL/boss/03_handR.x" },
    { "MODEL/boss/04_armL.x" },
    { "MODEL/boss/05_handL.x" },
    { "MODEL/boss/06_legR.x" },
    { "MODEL/boss/07_footR.x" },
    { "MODEL/boss/08_legL.x" },
    { "MODEL/boss/09_footL.x" },
};

int g_nTimeScore;				        //�c�莞�ԂɑΉ������X�R�A

//=====================================================
// �{�X����������
// Author:�㓡�T�V��
//=====================================================
void InitBoss(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�O���[�o���ϐ��̏�����
    g_nTimeScore = 0;
    g_FrameCountBoss = 0;				// ���݂̃t���[����
    g_PlayKeySetIndexBoss = 0;			// �Đ����̃L�[�Z�b�g�C���f�b�N�X
    g_BossCurrentAnimation = MOTION_IDLE_BOSS;	// �Đ����̃A�j���[�V����
    g_nFremeBoss = 0;					// �t���[����
    g_nCntKeyBoss = 1;					// �L�[��
    g_nChangeFlameBoss = 20;			// ���[�V�����̃t���[��

    //�\���̂̏�����
    g_Boss.pos = BOSS_FIRST_POS;		// �����ʒu
    g_Boss.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ʒu
    g_Boss.size = BOSS_SIZE;
    g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��������
    g_Boss.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��������
    g_Boss.rotMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��������
    g_Boss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ړ���
    g_Boss.bUse = true;
    g_Boss.bStun = false;
    g_Boss.MotionState = MOTION_IDLE_BOSS;
    g_Boss.state = BOSSSTATE_STAY;
    g_Boss.nCounterRigid = 0;
    g_Boss.nStayTime = NORMAL_STAY_TIME;
    g_Boss.nAttackTime = 0;
    g_Boss.nChanceTime = 0;
    g_Boss.nCntFlinch = 0;
    g_Boss.nStunTime = FLINCH_TIME;
    g_Boss.fDistance = 9999.0f;
    g_Boss.fLife = BOSS_LIFE;	//���@�̗̑�

    g_Boss.nShadow = SetShadow(g_Boss.pos, g_Boss.rot, g_Boss.size.x,
        g_Boss.pos.y, D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_ALPHA_MAX));//�e�̐ݒ�
    g_Boss.nCntInvincibleFrame = 0;
    g_Boss.bTransparence = false;

    g_Boss.rot.y = D3DXToRadian(90);

    LoadBoss();
    LoadBossAnimation();

    // ���f���p�[�c�̏�����
    for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
    {
        g_Boss.modelParts[nCntParts].nldxModelParent = 0;
    }
    // �t�@�C���̓ǂݍ���
    for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
    {
        g_Boss.modelParts[nCntParts].nldxModelParent = g_aBoss[nCntParts].nParents;

        // X�t�@�C���̓ǂݍ���
        D3DXLoadMeshFromX(g_aBoss[nCntParts].pFileName,
            D3DXMESH_SYSTEMMEM,
            pDevice,
            NULL,
            &g_Boss.modelParts[nCntParts].pBuffMat,
            NULL,
            &g_Boss.modelParts[nCntParts].nNumMat,
            &g_Boss.modelParts[nCntParts].pMesh);

    }
}

//=====================================================
// �{�X�I������
// Author:�㓡�T�V��
//=====================================================
void UninitBoss(void)
{
    for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
    {
        // ���b�V���̔j��
        if (g_Boss.modelParts[nCntParts].pMesh != NULL)
        {
            g_Boss.modelParts[nCntParts].pMesh->Release();
            g_Boss.modelParts[nCntParts].pMesh = NULL;
        }
        // �}�e���A���̔j��
        if (g_Boss.modelParts[nCntParts].pBuffMat != NULL)
        {
            g_Boss.modelParts[nCntParts].pBuffMat->Release();
            g_Boss.modelParts[nCntParts].pBuffMat = NULL;
        }
    }
}

//=====================================================
// �{�X�X�V����
// Author:�㓡�T�V��
//=====================================================
void UpdateBoss(void)
{
    //�{�X���������Ă���Ȃ�
    if (g_Boss.bUse == true)
    {
        //���G����
        if (g_Boss.nCntInvincibleFrame > 0)
        {
            g_Boss.nCntInvincibleFrame--;                     //���G���Ԃ𐔂���
            g_Boss.bTransparence = !g_Boss.bTransparence;   //���G���́A�_�ł�����
        }
        else
        {
            g_Boss.bTransparence = false; //����������
        }

        //1F�O�̈ʒu���L��
        g_Boss.posOld = g_Boss.pos;

        //���@���擾����
        PLAYER *pPlayer = GetPlayer();
        CAMERA *pCamera = GetCamera();

        //�U����ړ��Ɋւ���ϐ�
        float fPposx = pPlayer->pos.x, fPposz = pPlayer->pos.z;	//���@�̍��W
        float fEposx = g_Boss.pos.x, fEposz = g_Boss.pos.z;		//�G�̍��W
        float fAngle = atan2f((fEposx - fPposx), (fEposz - fPposz));	//�p�x�����߂�
        float fAngle2 = atan2f((fPposx - fEposx), (fPposz - fEposz));	//�p�x�����߂�
        int nRandAttack = 0;        //�����_���ȍU���ɂ���

        //�ʒu���ړ��ʂƕR�Â�
        g_Boss.pos += g_Boss.move;

        //�{�X��Ȃ�
        if (pCamera->bBossBattle == true)
        {
            //���@�ƃ{�X�̋���
            g_Boss.fDistance = sqrtf(
                powf((pPlayer->pos.x - g_Boss.pos.x), 2) +
                powf((pPlayer->pos.z - g_Boss.pos.z), 2));

            //��Ԃɂ��ꍇ����
            switch (g_Boss.state)
            {
                //========================
                //�����擾�ҋ@
                //========================
            case BOSSSTATE_STAY:

                //�ړ��ʂ��Ȃ���
                g_Boss.move.x = 0.0f;
                g_Boss.move.z = 0.0f;

                //�X�^�����Ă��Ȃ��Ȃ�
                if (g_Boss.bStun == false)
                {
                    //�ҋ@���Ԃ��J�E���g
                    if (g_Boss.nStayTime > 0)
                    {
                        g_Boss.nStayTime--;
                    }

                    //���@�̂ق������X�Ɍ���悤�ɂ���
                    g_Boss.rotDest.y = fAngle;

                    // ��]�̐���
                    if (g_Boss.rot.y > D3DX_PI)
                    {
                        g_Boss.rot.y -= D3DX_PI * 2;
                    }
                    if (g_Boss.rot.y < -D3DX_PI)
                    {
                        g_Boss.rot.y += D3DX_PI * 2;
                    }

                    g_Boss.rotMin.y = g_Boss.rotDest.y - g_Boss.rot.y;

                    // �ړI�̊p�x�̉�]�𐧌�
                    if (g_Boss.rotMin.y > D3DX_PI)
                    {
                        g_Boss.rotDest.y -= D3DX_PI * 2;
                    }
                    if (g_Boss.rotMin.y < -D3DX_PI)
                    {
                        g_Boss.rotDest.y += D3DX_PI * 2;
                    }

                    // ��]�̐���
                    if (g_Boss.rot.y > D3DX_PI)
                    {
                        g_Boss.rot.y -= D3DX_PI * 2;
                    }
                    if (g_Boss.rot.y < -D3DX_PI)
                    {
                        g_Boss.rot.y += D3DX_PI * 2;
                    }

                    if (fabsf(g_Boss.rotMin.y) < 0)
                    {
                        g_Boss.rotMin.y = g_Boss.rotDest.y;
                    }
                    else
                    {
                        //�c��̗͂ɂ���āA�U�����������ς���
                        if (g_Boss.fLife > BOSS_ANGER_LIFE)
                        {
                            g_Boss.rot.y += (g_Boss.rotDest.y - g_Boss.rot.y) * NORMAL_ROT_SPEED;
                        }
                        else if (g_Boss.fLife <= BOSS_ANGER_LIFE && g_Boss.fLife > BOSS_EXANGER_LIFE)
                        {
                            g_Boss.rot.y += (g_Boss.rotDest.y - g_Boss.rot.y) * ANGER_ROT_SPEED;
                        }
                        else if (g_Boss.fLife <= BOSS_EXANGER_LIFE)
                        {
                            g_Boss.rot.y += (g_Boss.rotDest.y - g_Boss.rot.y) * EXANGER_ROT_SPEED;
                        }
                    }

                }
                //�X�^��������
                else
                {
                    //�c��̗͂ɂ���āA�����擾�ҋ@���Ԃ�ݒ�
                    if (g_Boss.fLife > BOSS_ANGER_LIFE)
                    {
                        g_Boss.nStayTime = NORMAL_STAY_TIME;
                    }
                    else if (g_Boss.fLife <= BOSS_ANGER_LIFE && g_Boss.fLife > BOSS_EXANGER_LIFE)
                    {
                        g_Boss.nStayTime = ANGER_STAY_TIME;
                    }
                    else if (g_Boss.fLife <= BOSS_EXANGER_LIFE)
                    {
                        g_Boss.nStayTime = EXANGER_STAY_TIME;
                    }
                }

                //���̍s��(�U��)�ɐ؂�ւ���
                if (g_Boss.nStayTime == 0)
                {
                    //������������
                    nRandAttack = rand() % 100 + 1;

                    //�ߋ����̎�
                    if (g_Boss.fDistance < BOSS_CONSIDER_SHORT)
                    {
                        //5%
                        if (nRandAttack <= 5)
                        {
                            //�u���X
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_BREATH;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_BREATH;
                        }

                        //35%
                        else if (nRandAttack > 5 && nRandAttack <= 40)
                        {
                            //�ʏ펞
                            if (g_Boss.fLife > BOSS_EXANGER_LIFE)
                            {
                                //�^�b�N��
                                //�U���ɐ؂�ւ���
                                g_Boss.state = BOSSSTATE_ATTACK_TACKLE;
                                //�U���t���[����ݒ�
                                g_Boss.nAttackTime = ATTACK_TIME_TACKLE;
                            }

                            //���V��
                            else
                            {
                                //�W�����v
                                //�U���ɐ؂�ւ���
                                g_Boss.state = BOSSSTATE_ATTACK_JUMP;
                                //�U���t���[����ݒ�
                                g_Boss.nAttackTime = ATTACK_TIME_JUMP;
                            }

                        }

                        //60��
                        else if (nRandAttack > 40)
                        {
                            //�ʏ펞
                            if (g_Boss.fLife > BOSS_EXANGER_LIFE)
                            {
                                //�W�����v
                                //�U���ɐ؂�ւ���
                                g_Boss.state = BOSSSTATE_ATTACK_JUMP;
                                //�U���t���[����ݒ�
                                g_Boss.nAttackTime = ATTACK_TIME_JUMP;
                            }

                            //���V��
                            else
                            {
                                //�^�b�N��
                                //�U���ɐ؂�ւ���
                                g_Boss.state = BOSSSTATE_ATTACK_TACKLE;
                                //�U���t���[����ݒ�
                                g_Boss.nAttackTime = ATTACK_TIME_TACKLE;
                            }

                        }
                    }

                    //�������̎�
                    else if (g_Boss.fDistance >= BOSS_CONSIDER_SHORT && g_Boss.fDistance < BOSS_CONSIDER_MEDIUM)
                    {
                        //20%
                        if (nRandAttack <= 20)
                        {
                            //�W�����v
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_JUMP;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_JUMP;
                        }

                        //30%
                        else if (nRandAttack > 20 && nRandAttack <= 50)
                        {
                            //�u���X
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_BREATH;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_BREATH;
                        }

                        //50��
                        else if (nRandAttack > 50)
                        {
                            //�^�b�N��
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_TACKLE;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_TACKLE;
                        }
                    }

                    //�������̎�
                    else if (g_Boss.fDistance >= BOSS_CONSIDER_MEDIUM)
                    {
                        //5%
                        if (nRandAttack <= 5)
                        {
                            //�W�����v
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_JUMP;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_JUMP;
                        }

                        //40%
                        else if (nRandAttack > 5 && nRandAttack <= 45)
                        {
                            //�^�b�N��
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_TACKLE;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_TACKLE;
                        }

                        //55��
                        else if (nRandAttack > 45)
                        {
                            //�u���X
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_BREATH;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_BREATH;
                        }
                    }

                }

                g_Boss.MotionState = MOTION_IDLE_BOSS;   //�ҋ@

                break;

                //========================
                //�ːi�U����
                //========================
            case BOSSSTATE_ATTACK_TACKLE:

                //�X�^�����Ă��Ȃ��Ȃ�
                if (g_Boss.bStun == false)
                {
                    //�v���C���[���������Ă���Ȃ�
                    if (pPlayer->bDisp == true)
                    {
                        //�c��̗͂ɂ���āA�X�s�[�h��ς���
                        if (g_Boss.fLife > BOSS_ANGER_LIFE)
                        {
                            //�^�b�N���̃X�s�[�h���ŏ���1F�̂ݎ擾
                            if (g_Boss.nAttackTime == ATTACK_TIME_TACKLE)
                            {
                                //���@��ǂ�������
                                g_Boss.move.x = -sinf(g_Boss.rot.y)*NORMAL_TACKLE_SPEED;
                                g_Boss.move.z = -cosf(g_Boss.rot.y)*NORMAL_TACKLE_SPEED;
                            }
                        }
                        //�{��/���V���A�Ǐ]������
                        else
                        {
                            //�{�莞
                            if (g_Boss.fLife > BOSS_EXANGER_LIFE)
                            {
                                //�^�b�N���̃X�s�[�h���ŏ���1F�̂ݎ擾
                                if (g_Boss.nAttackTime == ATTACK_TIME_TACKLE)
                                {
                                    //���@��Ǐ]
                                    g_Boss.move.x = -sinf(g_Boss.rot.y)*ANGER_TACKLE_SPEED;
                                    g_Boss.move.z = -cosf(g_Boss.rot.y)*ANGER_TACKLE_SPEED;
                                }
                            }
                            //���V��
                            else
                            {
                                //�^�b�N���̃X�s�[�h���ŏ���1F�̂ݎ擾
                                if (g_Boss.nAttackTime == ATTACK_TIME_TACKLE)
                                {
                                    //���@��ǂ�������
                                    g_Boss.move.x = -sinf(g_Boss.rot.y)*EXANGER_TACKLE_SPEED;
                                    g_Boss.move.z = -cosf(g_Boss.rot.y)*EXANGER_TACKLE_SPEED;
                                }
                            }
                        }
                    }

                    //�U�����̃J�E���^�[
                    if (g_Boss.nAttackTime > 0)
                    {
                        g_Boss.nAttackTime--;
                    }
                }

                //�X�^��������
                else
                {
                    //�ړ��ʂ��Ȃ���
                    g_Boss.move.x = 0.0f;
                    g_Boss.move.z = 0.0f;

                    //�X�^�������ɁA���̍s���������邽�߂�1�ɂ���
                    g_Boss.nAttackTime = 1;
                }

                //���̍s���ɐ؂�ւ���
                if (g_Boss.nAttackTime == 0)
                {
                    //�{�莞
                    if (g_Boss.fLife < BOSS_ANGER_LIFE)
                    {
                        //�������߂��Ȃ�A�W�����v�ɔh��
                        if (g_Boss.fDistance < BOSS_CONSIDER_SHORT)
                        {
                            //�W�����v
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_JUMP;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_JUMP;
                        }

                        //�����������Ȃ�A�����Œ�ҋ@
                        else
                        {
                            //�����Œ�ҋ@�ɂ���
                            g_Boss.state = BOSSSTATE_CHANCE;

                            //�����Œ�ҋ@���Ԃ�ݒ�
                            g_Boss.nChanceTime = CHANCE_TIME_TACKLE;
                        }
                    }

                    //�ʏ펞
                    else
                    {
                        //�����Œ�ҋ@�ɂ���
                        g_Boss.state = BOSSSTATE_CHANCE;

                        //�����Œ�ҋ@���Ԃ�ݒ�
                        g_Boss.nChanceTime = CHANCE_TIME_TACKLE;
                    }

                }

                g_Boss.MotionState = MOTION_WALK_BOSS;   //����

                break;

                //========================
                //�W�����v�U����
                //========================
            case BOSSSTATE_ATTACK_JUMP:

                //�ړ��ʂ��Ȃ���
                g_Boss.move.x = 0.0f;
                g_Boss.move.z = 0.0f;

                //�X�^�����Ă��Ȃ��Ȃ�
                if (g_Boss.bStun == false)
                {
                    //�W�����v�ʂ��ŏ���1F�̂ݎ擾
                    if (g_Boss.nAttackTime == ATTACK_TIME_JUMP)
                    {
                        PlaySound(SOUND_LABEL_SE_JUMP);
                        //�W�����v
                        g_Boss.pos.y = 1.0f;
                        g_Boss.move.y = VALUE_BOSS_JUMP;
                    }
                    //������
                    else if (g_Boss.nAttackTime == 90)
                    {
                        StopSound(SOUND_LABEL_SE_JUMP);
                        PlaySound(SOUND_LABEL_SE_FALL);
                    }
                    //����������������
                    else if (g_Boss.nAttackTime < 90)
                    {
                        //�d�͂����߂�
                        g_Boss.move.y -= GRAVITY * 2;
                    }

                    //���n�����u�ԂɁA�߂��̃v���C���[���X�^��������
                    if (g_Boss.nAttackTime == 30)
                    {
                        StopSound(SOUND_LABEL_SE_FALL);
                        PlaySound(SOUND_LABEL_SE_GROUND);

                        //�ʏ펞
                        if (g_Boss.fLife > BOSS_ANGER_LIFE)
                        {
                            //�n�Ȃ炵�G�t�F�N�g(�O��)
                            //�G�t�F�N�g�֘A�̕ϐ�
                            float fAngle = 0.0f;    //�p�x
                            float fSpeedY = 0.0f;   //����
                            float fRadius = RANGE_STUN_XZ; //�G�t�F�N�g���a
                            for (int nCnt = 0; nCnt < COUNT_EFFECT_ITEM; nCnt++)
                            {
                                fSpeedY = float(rand() % 800 + 300) / 100.0f;
                                //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                                fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                                D3DXVECTOR3 pos = D3DXVECTOR3(g_Boss.pos.x - sinf(fAngle)*fRadius, g_Boss.pos.y
                                    , g_Boss.pos.z - cosf(fAngle)*fRadius);
                                D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                                SetEffect(pos, move, EFFECT_SIZE_EXPLODE_PLAYER, EFFECTTYPE_EXPLOSION); //�n�Ȃ炵
                            }

                            //�n�Ȃ炵�G�t�F�N�g(����)
                            fRadius = RANGE_STUN_XZ / 3 * 2; //�G�t�F�N�g���a
                            for (int nCnt = 0; nCnt < COUNT_EFFECT_ITEM; nCnt++)
                            {
                                fSpeedY = float(rand() % 800 + 300) / 100.0f;
                                //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                                fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                                D3DXVECTOR3 pos = D3DXVECTOR3(g_Boss.pos.x - sinf(fAngle)*fRadius, g_Boss.pos.y
                                    , g_Boss.pos.z - cosf(fAngle)*fRadius);
                                D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                                SetEffect(pos, move, EFFECT_SIZE_EXPLODE_PLAYER, EFFECTTYPE_EXPLOSION); //�n�Ȃ炵
                            }

                            //�n�Ȃ炵�Ŏ��@���X�^��������
                            if (g_Boss.fDistance < RANGE_STUN_XZ && g_Boss.pos.y <= RANGE_STUN_Y - pPlayer->pos.y)
                            {
                                pPlayer->bStun = true;
                            }
                        }

                        //�{�莞
                        else
                        {
                            //�n�Ȃ炵�G�t�F�N�g(�O��)
                            //�G�t�F�N�g�֘A�̕ϐ�
                            float fAngle = 0.0f;    //�p�x
                            float fSpeedY = 0.0f;   //����
                            float fRadius = ANGER_RANGE_STUN_XZ; //�G�t�F�N�g���a
                            for (int nCnt = 0; nCnt < COUNT_EFFECT_ITEM; nCnt++)
                            {
                                fSpeedY = float(rand() % 800 + 300) / 100.0f;
                                //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                                fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                                D3DXVECTOR3 pos = D3DXVECTOR3(g_Boss.pos.x - sinf(fAngle)*fRadius, g_Boss.pos.y
                                    , g_Boss.pos.z - cosf(fAngle)*fRadius);
                                D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                                SetEffect(pos, move, EFFECT_SIZE_EXPLODE_PLAYER, EFFECTTYPE_EXPLOSION); //�n�Ȃ炵
                            }

                            //�n�Ȃ炵�G�t�F�N�g(����)
                            fRadius = ANGER_RANGE_STUN_XZ / 3 * 2; //�G�t�F�N�g���a
                            for (int nCnt = 0; nCnt < COUNT_EFFECT_ITEM; nCnt++)
                            {
                                fSpeedY = float(rand() % 800 + 300) / 100.0f;
                                //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                                fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                                D3DXVECTOR3 pos = D3DXVECTOR3(g_Boss.pos.x - sinf(fAngle)*fRadius, g_Boss.pos.y
                                    , g_Boss.pos.z - cosf(fAngle)*fRadius);
                                D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                                SetEffect(pos, move, EFFECT_SIZE_EXPLODE_PLAYER, EFFECTTYPE_EXPLOSION); //�n�Ȃ炵
                            }

                            //�n�Ȃ炵�Ŏ��@���X�^��������
                            if (g_Boss.fDistance < ANGER_RANGE_STUN_XZ && g_Boss.pos.y <= RANGE_STUN_Y - pPlayer->pos.y)
                            {
                                pPlayer->bStun = true;
                            }
                        }
                    }

                    //�U�����̃J�E���^�[
                    if (g_Boss.nAttackTime > 0)
                    {
                        g_Boss.nAttackTime--;
                    }
                }

                //�X�^��������
                else
                {
                    //�X�^�������ɁA���̍s���������邽�߂�1�ɂ���
                    g_Boss.nAttackTime = 1;
                }

                //���̍s��(�����Œ�ҋ@)�ɐ؂�ւ���
                if (g_Boss.nAttackTime == 0)
                {
                    //�����Œ�ҋ@�ɂ���
                    g_Boss.state = BOSSSTATE_CHANCE;

                    //�����Œ�ҋ@���Ԃ�ݒ�
                    g_Boss.nChanceTime = CHANCE_TIME_JUMP;
                }

                g_Boss.MotionState = MOTION_JUMP_BOSS;   //�W�����v

                break;

                //========================
                //�u���X�U����
                //========================
            case BOSSSTATE_ATTACK_BREATH:

                //�ړ��ʂ��Ȃ���
                g_Boss.move.x = 0.0f;
                g_Boss.move.z = 0.0f;

                //�X�^�����Ă��Ȃ��Ȃ�
                if (g_Boss.bStun == false)
                {
                    //���߂Ă���ԁA���@������悤�ɂ���
                    if (g_Boss.nAttackTime > BREATH_MOMENT)
                    {
                        //���@�̂ق������X�Ɍ���悤�ɂ���
                        g_Boss.rotDest.y = fAngle;

                        // ��]�̐���
                        if (g_Boss.rot.y > D3DX_PI)
                        {
                            g_Boss.rot.y -= D3DX_PI * 2;
                        }
                        if (g_Boss.rot.y < -D3DX_PI)
                        {
                            g_Boss.rot.y += D3DX_PI * 2;
                        }

                        g_Boss.rotMin.y = g_Boss.rotDest.y - g_Boss.rot.y;

                        // �ړI�̊p�x�̉�]�𐧌�
                        if (g_Boss.rotMin.y > D3DX_PI)
                        {
                            g_Boss.rotDest.y -= D3DX_PI * 2;
                        }
                        if (g_Boss.rotMin.y < -D3DX_PI)
                        {
                            g_Boss.rotDest.y += D3DX_PI * 2;
                        }

                        // ��]�̐���
                        if (g_Boss.rot.y > D3DX_PI)
                        {
                            g_Boss.rot.y -= D3DX_PI * 2;
                        }
                        if (g_Boss.rot.y < -D3DX_PI)
                        {
                            g_Boss.rot.y += D3DX_PI * 2;
                        }

                        if (fabsf(g_Boss.rotMin.y) < 0)
                        {
                            g_Boss.rotMin.y = g_Boss.rotDest.y;
                        }
                        else
                        {
                            //�c��̗͂ɂ���āA�U�����������ς���
                            if (g_Boss.fLife > BOSS_ANGER_LIFE)
                            {
                                g_Boss.rot.y += (g_Boss.rotDest.y - g_Boss.rot.y) * NORMAL_ROT_SPEED;
                            }
                            else if (g_Boss.fLife <= BOSS_ANGER_LIFE && g_Boss.fLife > BOSS_EXANGER_LIFE)
                            {
                                g_Boss.rot.y += (g_Boss.rotDest.y - g_Boss.rot.y) * ANGER_ROT_SPEED;
                            }
                            else if (g_Boss.fLife <= BOSS_EXANGER_LIFE)
                            {
                                g_Boss.rot.y += (g_Boss.rotDest.y - g_Boss.rot.y) * EXANGER_ROT_SPEED;
                            }
                        }
                    }

                    //�e�𔭎�
                    else if (g_Boss.nAttackTime == BREATH_MOMENT)
                    {

                        //�ʏ펞
                        if (g_Boss.fLife > BOSS_ANGER_LIFE)
                        {
                            SetBullet(D3DXVECTOR3(g_Boss.modelParts[1].mtxWorld._41, g_Boss.pos.y + BOSS_HEIGHT
                                , g_Boss.modelParts[1].mtxWorld._43), BULLET_SIZE_BOSS,
                                D3DXVECTOR3(-sinf(g_Boss.rot.y)*BULLET_SPEED_BOSS, 0.0f
                                    , -cosf(g_Boss.rot.y)*BULLET_SPEED_BOSS), BULLET_LIFE_BOSS, BULLET_DAMAGE_BOSS, BULLETTYPE_BOSS);
                        }

                        //�{�莞
                        else
                        {
                            SetBullet(D3DXVECTOR3(g_Boss.modelParts[1].mtxWorld._41, g_Boss.pos.y + BOSS_HEIGHT
                                , g_Boss.modelParts[1].mtxWorld._43), BULLET_SIZE_BOSS,
                                D3DXVECTOR3(-sinf(g_Boss.rot.y)*BULLET_SPEED_BOSS + 1.5f, 0.0f
                                    , -cosf(g_Boss.rot.y)*BULLET_SPEED_BOSS + 1.5f), BULLET_LIFE_BOSS, BULLET_DAMAGE_BOSS, BULLETTYPE_BOSS);

                            SetBullet(D3DXVECTOR3(g_Boss.modelParts[1].mtxWorld._41, g_Boss.pos.y + BOSS_HEIGHT
                                , g_Boss.modelParts[1].mtxWorld._43), BULLET_SIZE_BOSS,
                                D3DXVECTOR3(-sinf(g_Boss.rot.y - (float)D3DXToRadian(22.5))*BULLET_SPEED_BOSS + 1.5f, 0.0f
                                    , -cosf(g_Boss.rot.y - (float)D3DXToRadian(22.5))*BULLET_SPEED_BOSS + 1.5f), BULLET_LIFE_BOSS, BULLET_DAMAGE_BOSS, BULLETTYPE_BOSS);

                            SetBullet(D3DXVECTOR3(g_Boss.modelParts[1].mtxWorld._41, g_Boss.pos.y + BOSS_HEIGHT
                                , g_Boss.modelParts[1].mtxWorld._43), BULLET_SIZE_BOSS,
                                D3DXVECTOR3(-sinf(g_Boss.rot.y + (float)D3DXToRadian(22.5))*BULLET_SPEED_BOSS + 1.5f, 0.0f
                                    , -cosf(g_Boss.rot.y + (float)D3DXToRadian(22.5))*BULLET_SPEED_BOSS + 1.5f), BULLET_LIFE_BOSS, BULLET_DAMAGE_BOSS, BULLETTYPE_BOSS);
                        }
                    }

                    //���ˉ�
                    else if (g_Boss.nAttackTime == BREATH_MOMENT - 5)
                    {
                        PlaySound(SOUND_LABEL_SE_SHOT); //�V���b�g��
                    }

                    //�U�����̃J�E���^�[
                    if (g_Boss.nAttackTime > 0)
                    {
                        g_Boss.nAttackTime--;
                    }
                }

                //�X�^��������
                else
                {
                    //�X�^�������ɁA���̍s���������邽�߂�1�ɂ���
                    g_Boss.nAttackTime = 1;
                }

                //���̍s��(�����Œ�ҋ@)�ɐ؂�ւ���
                if (g_Boss.nAttackTime == 0)
                {
                    //�ړ��ʂ��Ȃ���
                    g_Boss.move.x = 0.0f;
                    g_Boss.move.z = 0.0f;

                    //�����Œ�ҋ@�ɂ���
                    g_Boss.state = BOSSSTATE_CHANCE;

                    //�����Œ�ҋ@���Ԃ�ݒ�
                    g_Boss.nChanceTime = CHANCE_TIME_BREATH;
                }

                g_Boss.MotionState = MOTION_BREATH_BOSS;   //�e�U��

                break;

                //========================
                //�����Œ�ҋ@
                //========================
            case BOSSSTATE_CHANCE:

                //�ړ��ʂ��Ȃ���
                g_Boss.move.x = 0.0f;
                g_Boss.move.z = 0.0f;

                //�X�^�����Ă��Ȃ��Ȃ�
                if (g_Boss.bStun == false)
                {
                    //�����Œ�ҋ@�̃J�E���^�[
                    if (g_Boss.nChanceTime > 0)
                    {
                        g_Boss.nChanceTime--;
                    }
                }

                //���̍s��(�����擾�ҋ@)�ɐ؂�ւ���
                if (g_Boss.nChanceTime == 0)
                {
                    //�����擾�ҋ@�ɂ���
                    g_Boss.state = BOSSSTATE_STAY;

                    //�c��̗͂ɂ���āA�����擾�ҋ@���Ԃ�ݒ�
                    if (g_Boss.fLife > BOSS_ANGER_LIFE)
                    {
                        g_Boss.nStayTime = NORMAL_STAY_TIME;
                    }
                    else if (g_Boss.fLife <= BOSS_ANGER_LIFE && g_Boss.fLife > BOSS_EXANGER_LIFE)
                    {
                        g_Boss.nStayTime = ANGER_STAY_TIME;
                    }
                    else if (g_Boss.fLife <= BOSS_EXANGER_LIFE)
                    {
                        g_Boss.nStayTime = EXANGER_STAY_TIME;
                    }
                }

                g_Boss.MotionState = MOTION_IDLE_BOSS;   //�ҋ@

                break;
            }
            
            //�����Y�̉��̓����蔻��
            if (HitMomoFire(&g_Boss.pos, &g_Boss.posOld, g_Boss.size) == true)
            {
                //���̋����ɂ���āA�^����_���[�W�Ƌ��ݒl��ς���
                switch (pPlayer->nFire)
                {
                case PLAYER_FIRST_FIRE:
                    HitBoss(BOMB_FLINCH, BOMB_DAMAGE);
                    break;

                case PLAYER_FIRST_FIRE + 1:
                    HitBoss(BOMB_FLINCH + 10, BOMB_DAMAGE + 5.0f);
                    break;

                case PLAYER_FIRST_FIRE + 2:
                    HitBoss(BOMB_FLINCH + 20, BOMB_DAMAGE + 10.0f);
                    break;

                case PLAYER_FIRST_FIRE + 3:
                    HitBoss(BOMB_FLINCH + 30, BOMB_DAMAGE + 15.0f);
                    break;

                case PLAYER_FIRST_FIRE + 4:
                    HitBoss(BOMB_FLINCH * 2, BOMB_DAMAGE * 2);
                    break;
                }
            }

            //����/�X�^���̏���
            if (g_Boss.bStun == false && g_Boss.nCounterRigid > BOSS_REACH_FLINCH)
            {
                g_Boss.bStun = true;
                g_Boss.nCntFlinch++;        //���݉񐔂����Z
            }

            //����/�X�^����
            else if (g_Boss.bStun == true)
            {
                //�X�^���̃G�t�F�N�g
                if (g_Boss.nStunTime == STUN_TIME)
                {
                    SetEffect(D3DXVECTOR3(g_Boss.modelParts[1].mtxWorld._41, BOSS_HEIGHT * 5, g_Boss.modelParts[1].mtxWorld._43)
                        , D3DXVECTOR3(0.0f,0.0f,0.0f), EFFECT_SIZE_STUN, EFFECTTYPE_BOSS_STUN); //�{�X�X�^���G�t�F�N�g
                }

                g_Boss.nCounterRigid = 0;    //����/�X�^���܂ł̒l�����Z�b�g
                g_Boss.nStunTime--;          //����/�X�^������

                //����
                if (g_Boss.nCntFlinch == 1)
                {
                    g_Boss.MotionState = MOTION_FLINCH_BOSS;   //����

                    //�ジ���肷��
                    g_Boss.move.x = sinf(g_Boss.rot.y)*FLINCH_MOVE;
                    g_Boss.move.z = cosf(g_Boss.rot.y)*FLINCH_MOVE;

                    //���݉���
                    if (g_Boss.nStunTime < 0)
                    {
                        g_Boss.nStunTime = FLINCH_TIME; //���͋���
                        g_Boss.bStun = false;
                    }
                }

                //����(���X�^��)
                else if (g_Boss.nCntFlinch == 2)
                {
                    g_Boss.MotionState = MOTION_FLINCH_BOSS;   //����

                    //�ジ���肷��
                    g_Boss.move.x = sinf(g_Boss.rot.y)*FLINCH_MOVE;
                    g_Boss.move.z = cosf(g_Boss.rot.y)*FLINCH_MOVE;

                    //���݉���
                    if (g_Boss.nStunTime < 0)
                    {
                        g_Boss.nStunTime = STUN_TIME; //���̓X�^��
                        g_Boss.bStun = false;
                    }
                }

                //�X�^��(���͋���)
                else if (g_Boss.nCntFlinch == 3)
                {
                    g_Boss.MotionState = MOTION_STUN_BOSS;   //�X�^��

                    //�ړ��ʂ��Ȃ���
                    g_Boss.move.x = 0.0f;
                    g_Boss.move.z = 0.0f;

                    //���݉���
                    if (g_Boss.nStunTime < 0)
                    {
                        g_Boss.nStunTime = FLINCH_TIME; //���͋���
                        g_Boss.bStun = false;
                        g_Boss.nCntFlinch = 0;  //���݉񐔂����Z�b�g
                    }
                }
            }

            //�d��
            g_Boss.move.y -= GRAVITY;
            if (g_Boss.pos.y < 0)
            {
                g_Boss.pos.y = 0;
                g_Boss.move.y = 0;
            }

            //�u���b�N�̓����蔻��
            CollisionBlock(&g_Boss.pos, &g_Boss.posOld, g_Boss.size);

            //�e��ݒ�
            SetPositionShadow(g_Boss.nShadow
                , D3DXVECTOR3(g_Boss.pos.x, 0, g_Boss.pos.z), g_Boss.pos.y);

            //���S����(�v���C���[���������Ă���Ȃ�)
            if (g_Boss.fLife <= 0.0f && pPlayer->bDisp == true)
            {
                //�c��̕b���𐔂���
                g_nTimeScore = g_nTimeMinute * 60 + g_nTimeSecond;

                //��Փx�ɂ���āA����X�R�A��ς���
                SETTING *pSetting = GetSetting();
                //�m�[�}��
                if (pSetting->nLevel == SETTINGLEVEL_NORMAL)
                {
                    ScoreCalculation((int)pPlayer->fLife * 30 + g_nTimeScore * 100 + 10000);
                }
                //�S
                else
                {
                    ScoreCalculation(g_nTimeScore * 100 + 67500);
                }

                //�e�̍폜
                SHADOW *pShadow = GetShadow();//�e�̏����擾
                pShadow[g_Boss.nShadow].use = false;

                g_Boss.bUse = false;    //���@�𖢎g�p��

                PlaySound(SOUND_LABEL_SE_BOSS_EXPLOSION);   //�{�X�����ꂽ���̐�

                //�����G�t�F�N�g
                float fSpeedX = 0.0f;   //����X
                float fSpeedY = 0.0f;   //����Y
                float fSpeedZ = 0.0f;   //����Z
                int nPlusMinusX = 0;    //����X
                int nPlusMinusZ = 0;    //����Z
                for (int nCnt = 0; nCnt < COUNT_EFFECT_EXPLODE_PLAYER * 2; nCnt++)
                {
                    fSpeedX = float(rand() % 500) / 100.0f;
                    fSpeedY = float(rand() % 800 + 300) / 100.0f;
                    fSpeedZ = float(rand() % 500) / 100.0f;
                    nPlusMinusX = int(rand() % 2 + 1);
                    nPlusMinusZ = int(rand() % 2 + 1);
                    if (nPlusMinusX == 1)
                    {
                        nPlusMinusX = 1;
                    }
                    else
                    {
                        nPlusMinusX = -1;
                    }

                    if (nPlusMinusZ == 1)
                    {
                        nPlusMinusZ = 1;
                    }
                    else
                    {
                        nPlusMinusZ = -1;
                    }

                    D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX*nPlusMinusX, fSpeedY, fSpeedZ*nPlusMinusZ);
                    SetEffect(g_Boss.pos, move, EFFECT_SIZE_EXPLODE_PLAYER, EFFECTTYPE_EXPLOSION); //�����G�t�F�N�g
                }

                //������
                g_Boss.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_Boss.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_Boss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_Boss.fDistance = 9999.9f;

                PlaySound(SOUND_LABEL_SE_FINISH);       //�t�B�j�b�V���̌��ʉ�

                //�Q�[���N���A�̔w�i���Ăяo�����߂̐ݒ�
                GAME *pGame = GetGame();
                pGame->winner = SELECT_BOSS;

                SetGameState(GAMESTATE_FINSIH_GAME);    //�Q�[���N���A 
            }

            //�{���ԂŁA�I�[�����܂Ƃ�
            if (g_Boss.fLife <= BOSS_ANGER_LIFE)
            {
                float fSpeedY = float(rand() % 800 + 300) / 100.0f;
                //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                float fRadius = float(rand() % 17500 + 0) / 100.0f;
                D3DXVECTOR3 pos = D3DXVECTOR3(g_Boss.pos.x - sinf(fAngle)*fRadius, g_Boss.pos.y + GRID_SIZE * 2, g_Boss.pos.z - cosf(fAngle)*fRadius);
                D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                SetEffect(pos, move, EFFECT_SIZE_EVIL, EFFECTTYPE_EVIL); //�E�ӂ̔g��
            }

        }
        //���f���̃A�j���[�V����
        UpdateBossMotion();
    }
}

//=====================================================
// �{�X�`�揈��
// Author:�㓡�T�V��
//=====================================================
void DrawBoss(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^
    D3DXMATRIX mtxRot, mtxTrans, mtxParent;
    D3DMATERIAL9 matDef;		// ���݂̃}�e���A���ێ��p
    D3DXMATERIAL* pMat;			// �}�e���A���f�[�^�ւ̃|�C���^

    //�{�X�����݂��Ă��āA�����łȂ��Ȃ�`��
    if (g_Boss.bUse == true && g_Boss.bTransparence == false)
    {
        // ���[���h�}�g���b�N�X�̏�����
        D3DXMatrixIdentity(&g_Boss.mtxWorld);

        // �����𔽉f
        D3DXMatrixRotationYawPitchRoll(&mtxRot,
            g_Boss.rot.y,
            g_Boss.rot.x,
            g_Boss.rot.z);
        D3DXMatrixMultiply(&g_Boss.mtxWorld, &g_Boss.mtxWorld, &mtxRot);

        // �ʒu�𔽉f
        D3DXMatrixTranslation(&mtxTrans,
            g_Boss.pos.x,
            g_Boss.pos.y,
            g_Boss.pos.z);
        D3DXMatrixMultiply(&g_Boss.mtxWorld, &g_Boss.mtxWorld, &mtxTrans);

        // �p�[�c�̐ݒ�
        for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
        {
            // ���[���h�}�g���b�N�X�̏�����
            D3DXMatrixIdentity(&g_Boss.modelParts[nCntParts].mtxWorld);

            // �����𔽉f
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_aBoss[nCntParts].rot.y,
                g_aBoss[nCntParts].rot.x,
                g_aBoss[nCntParts].rot.z);

            D3DXMatrixMultiply(
                &g_Boss.modelParts[nCntParts].mtxWorld,
                &g_Boss.modelParts[nCntParts].mtxWorld, &mtxRot);

            // �ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans,
                g_aBoss[nCntParts].pos.x,
                g_aBoss[nCntParts].pos.y,
                g_aBoss[nCntParts].pos.z);
            D3DXMatrixMultiply(
                &g_Boss.modelParts[nCntParts].mtxWorld,
                &g_Boss.modelParts[nCntParts].mtxWorld, &mtxTrans);

            // �e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, NULL);

            // ���݂̃}�e���A�����擾����
            pDevice->GetMaterial(&matDef);

            // �}�e���A���f�[�^�ւ̃|�C���^���擾
            pMat = (D3DXMATERIAL*)g_Boss.modelParts[nCntParts].pBuffMat->GetBufferPointer();

            // �e���f���̃C���f�b�N�X�� - 1�̎�
            if (g_Boss.modelParts[nCntParts].nldxModelParent == -1)
            {
                mtxParent = g_Boss.mtxWorld;		// �{�X�̃}�g���N�X
            }
            else
            {
                mtxParent = g_Boss.modelParts[g_Boss.modelParts[nCntParts].nldxModelParent].mtxWorld;
            }
            // �e���f���̃}�g���b�N�X�̔���
            D3DXMatrixMultiply(
                &g_Boss.modelParts[nCntParts].mtxWorld,
                &g_Boss.modelParts[nCntParts].mtxWorld, &mtxParent);

            // ���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_Boss.modelParts[nCntParts].mtxWorld);

            for (int nCntMat = 0; nCntMat < (int)g_Boss.modelParts[nCntParts].nNumMat; nCntMat++)
            {
                // �}�e���A���̐ݒ�
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                g_Boss.modelParts[nCntParts].pMesh->DrawSubset(nCntMat);
            }

            // �ێ����Ă����}�e���A����߂�
            pDevice->SetMaterial(&matDef);
        }
    }
}

//=============================================================================
// �{�X�̎擾
// Author:�㓡�T�V��
//=============================================================================
BOSS*GetBoss(void)
{
    return &g_Boss;
}

//=============================================================================
// �{�X�̃_���[�W�ݒ�
// Author:�㓡�T�V��
//=============================================================================
void HitBoss(int nCntFlinch, float fDamage)
{
    //���G���ԂłȂ��Ȃ�
    if (g_Boss.nCntInvincibleFrame == 0)
    {
        GAME *pGame = GetGame();

        //�Q�[�����I�������A�_���[�W���󂯂Ȃ�
        if (pGame->state == GAMESTATE_NORMAL)
        {
            //�c��̗͂ɂ���āA�X�^���l�̏㏸��ݒ�
            if (g_Boss.fLife > BOSS_ANGER_LIFE)
            {
                g_Boss.nCounterRigid += nCntFlinch; //�X�^���l��������
            }
            //�c��̗͔�������A�X�^���l�������肸�炭�Ȃ�
            else
            {
                g_Boss.nCounterRigid += nCntFlinch / 2; //�X�^���l��������
            }

            //�c��̗͂ɂ���āA�󂯂�_���[�W��ݒ�
            if (g_Boss.fLife > BOSS_EXANGER_LIFE)
            {
                //�_���[�W�v�Z
                g_Boss.fLife -= nCntFlinch;
            }
            //�c��̗�4����1����A�d������
            else
            {
                //�_���[�W�v�Z
                g_Boss.fLife -= nCntFlinch / 2;
            }

            //���G���Ԃ�����
            g_Boss.nCntInvincibleFrame = ENEMY_INVINCIBLE_FRAME;
        }
    }
}

//=====================================================
// �{�X�̏Փ˔���̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool CollisionBoss(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, bool bPlayer)
{
    bool bCollision = false;  //�Փ˂������ǂ���

    // �{�X���g��ꂽ��
    if (g_Boss.bUse == true)
    {
        //�����蔻����v�Z
        if (RectangleCollision(pPos, pPosOld, size, &g_Boss.pos, g_Boss.size) == true)
        {
            bCollision = true;    //�Փ�

            //�v���C���[�Ȃ�
            if (bPlayer == true)
            {
                //�m�b�N�o�b�N
                PLAYER *pPlayer = GetPlayer();
                if (pPlayer->nCntInvincibleFrame == 0)  //���G�łȂ��Ȃ�
                {
                    float fKnockback = VALUE_KNOCKBACK;
                    pPlayer->move.x = pPlayer->pos.x - g_Boss.posOld.x;
                    pPlayer->move.z = pPlayer->pos.z - g_Boss.posOld.z;
                    D3DXVec3Normalize(&pPlayer->move, &pPlayer->move);  //�x�N�g���𐳋K��
                    pPlayer->move.x = pPlayer->move.x*fKnockback;
                    pPlayer->move.z = pPlayer->move.z*fKnockback;
                }
            }
        }
    }

    return bCollision;    //�Փ˂������ǂ�����Ԃ�
}

//=============================================================================
// �A�j���[�V�������[�h
// Author:���� �D�l
//=============================================================================
void LoadBossAnimation(void)
{
    // �t�@�C���|�C���g
    FILE *pFile;

    // �ϐ��錾
    int  nCntLoad = 0;
    int	 nCntKey = 0;
    int	 nCntMotion = 0;

    char cReedText[1024];	// �����Ƃ��ēǂݎ��p
    char cHeadText[1024];	//
    char cDie[1024];

    // �t�@�C���J
    pFile = fopen("TXT/motionboss.txt", "r");

    // �J����
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "SCRIPT") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);
        }

        if (strcmp(cHeadText, "SCRIPT") == 0)
        {
            while (strcmp(cHeadText, "END_SCRIPT") != 0)
            {

                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "MOTIONSET") == 0)
                {

                    while (strcmp(cHeadText, "END_MOTIONSET") != 0)
                    {
                        fgets(cReedText, sizeof(cReedText), pFile);
                        sscanf(cReedText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "LOOP") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aBossMotion[nCntMotion].nLoop);
                        }

                        if (strcmp(cHeadText, "NUM_KEY") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aBossMotion[nCntMotion].nNum_Key);
                        }

                        //			if (strcmp(cHeadText, "COLLISION") == 0)
                        //			{
                        //				sscanf(cReedText, "%s %s %d %f %f %f %f %d %d", &cDie, &cDie, &g_BossCollision.nPartsInfo, &g_BossCollision.CollisionPos.x, &g_BossCollision.CollisionPos.y, &g_BossCollision.CollisionPos.z, &g_BossCollision.fLength, &g_BossCollision.nFirstFlame[nCntMotion], &g_BossCollision.nLastFlame);
                        //			}
                        if (strcmp(cHeadText, "KEYSET") == 0)
                        {

                            while (strcmp(cHeadText, "END_KEYSET") != 0)
                            {

                                fgets(cReedText, sizeof(cReedText), pFile);
                                sscanf(cReedText, "%s", &cHeadText);

                                if (strcmp(cHeadText, "FRAME") == 0)
                                {
                                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aBossMotion[nCntMotion].KeyInfo[nCntKey].nFrame);
                                }
                                if (strcmp(cHeadText, "KEY") == 0)
                                {
                                    while (strcmp(cHeadText, "END_KEY") != 0)
                                    {
                                        fgets(cReedText, sizeof(cReedText), pFile);
                                        sscanf(cReedText, "%s", &cHeadText);

                                        if (strcmp(cHeadText, "POS") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &g_aBossMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].x,
                                                &g_aBossMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].y,
                                                &g_aBossMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].z);
                                        }

                                        if (strcmp(cHeadText, "ROT") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &g_aBossMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].x,
                                                &g_aBossMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].y,
                                                &g_aBossMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].z);
                                        }
                                    }
                                    nCntLoad++;
                                }
                            }
                            nCntLoad = 0;
                            nCntKey++;
                        }
                    }
                    nCntKey = 0;
                    nCntMotion++;
                }
            }
            // �t�@�C����
            fclose(pFile);
        }
        // �J���Ȃ�
        else
        {
            printf("�J����܂���ł���\n");
        }
    }
}

//=============================================================================
// ���f�����[�h
// Author:���� �D�l
//=============================================================================
void LoadBoss(void)
{
    // �t�@�C���|�C���g
    FILE *pFile;

    // �ϐ��錾
    int  nCntLoad = 0;
    char cReedText[128];	// �����Ƃ��ēǂݎ��p
    char cHeadText[256];	//
    char cDie[128];

    // �t�@�C���J
    pFile = fopen("TXT/boss.txt", "r");

    // �J����
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "CHARACTERSET") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);
        }

        if (strcmp(cHeadText, "CHARACTERSET") == 0)
        {
            while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
            {

                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "\n") == 0)
                {
                }
                else if (strcmp(cHeadText, "PARTSSET") == 0)
                {
                    while (strcmp(cHeadText, "END_PARTSSET") != 0)
                    {

                        fgets(cReedText, sizeof(cReedText), pFile);
                        sscanf(cReedText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "INDEX") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aBoss[nCntLoad].nData);
                        }

                        if (strcmp(cHeadText, "PARENT") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aBoss[nCntLoad].nParents);
                        }

                        if (strcmp(cHeadText, "POS") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                &g_aBoss[nCntLoad].posOrigin.x,
                                &g_aBoss[nCntLoad].posOrigin.y,
                                &g_aBoss[nCntLoad].posOrigin.z);

                            g_aBoss[nCntLoad].pos = g_aBoss[nCntLoad].posOrigin;
                        }

                        if (strcmp(cHeadText, "ROT") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                &g_aBoss[nCntLoad].rot.x,
                                &g_aBoss[nCntLoad].rot.y,
                                &g_aBoss[nCntLoad].rot.z);
                        }
                    }
                    nCntLoad++;
                }
            }
        }
        // �t�@�C����
        fclose(pFile);
    }

    // �J���Ȃ�
    else
    {
        printf("�J����܂���ł���\n");
    }
}

//=============================================================================
// ���[�V�����̍X�V
// Author:���� �D�l
//=============================================================================
void UpdateBossMotion(void)
{
    //���[�V�������ς�����Ƃ�
    if (g_MotionOldStateBoss != g_Boss.MotionState) {
        g_nFremeBoss = 0;
        g_nCntKeyBoss = 0;
    }
    for (int nCntMotion = 0; nCntMotion < MAX_BOSS_PARTS; nCntMotion++)
    {
        if (g_nFremeBoss == 0)
        {
            g_NumPosBoss[nCntMotion].x = ((g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].pos[nCntMotion].x -
                (g_aBoss[nCntMotion].pos.x - g_aBoss[nCntMotion].posOrigin.x)) / float(g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].nFrame));

            g_NumPosBoss[nCntMotion].y = ((g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].pos[nCntMotion].y -
                (g_aBoss[nCntMotion].pos.y - g_aBoss[nCntMotion].posOrigin.y)) / float(g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].nFrame));

            g_NumPosBoss[nCntMotion].z = ((g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].pos[nCntMotion].z -
                (g_aBoss[nCntMotion].pos.z - g_aBoss[nCntMotion].posOrigin.z)) / float(g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].nFrame));

            g_NumRotBossDest[nCntMotion] =
                g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].rot[nCntMotion] - g_aBoss[nCntMotion].rot;

            g_NumRotBoss[nCntMotion] = (g_NumRotBossDest[nCntMotion] / float(g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].nFrame));
        }

        g_aBoss[nCntMotion].pos += g_NumPosBoss[nCntMotion];
        g_aBoss[nCntMotion].rot += g_NumRotBoss[nCntMotion];
    }
    g_MotionOldStateBoss = g_Boss.MotionState;
    // ���[�v����Ƃ�
    if (g_nFremeBoss >= g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].nFrame && g_aBossMotion[g_Boss.MotionState].nLoop == 1)
    {
        g_nFremeBoss = 0;
        g_nCntKeyBoss++;

        if (g_nCntKeyBoss >= g_aBossMotion[g_Boss.MotionState].nNum_Key)
        {
            g_nCntKeyBoss = 0;
        }

    }
    // ���[�v���Ȃ��Ƃ�
    else if (g_nFremeBoss >= g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].nFrame && g_aBossMotion[g_Boss.MotionState].nLoop == 0)
    {
        g_nFremeBoss = 0;
        g_nCntKeyBoss++;
        if (g_nCntKeyBoss >= g_aBossMotion[g_Boss.MotionState].nNum_Key)
        {
            for (int nCntMotion = 0; nCntMotion < MAX_BOSS_PARTS; nCntMotion++)
            {
                g_NumPosBoss[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_NumRotBoss[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
            g_Boss.MotionState = MOTION_IDLE_BOSS;
            g_nCntKeyBoss = 0;
        }
    }
    else
    {
        g_nFremeBoss++;
    }
}