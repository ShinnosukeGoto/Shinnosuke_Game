//=====================================================
//
// 3D�Q�[���̂ЂȌ`�̏���[boss.h]
// Author:�㓡�T�V��
//
//=====================================================

//=====================================================
// �C���N���[�h�t�@�C��
//=====================================================
#include "boss.h"
#include "camera.h"
#include "sound.h"
#include "effect2.h"
#include "collision.h"
#include "campaign.h"
#include "bullet.h"
#include "player.h"
#include "camera.h"
#include "score.h"
#include "time.h"

//=====================================================
// �O���[�o���ϐ���`
//=====================================================
BOSS g_Boss;        //�{�X�̏��
int g_ShadowIndexBossBoss;
int g_RandomAttackBoss = 1;				//�e�������_���ɑłƂ��Ɏg�p
int g_nTimeScore;				//�c�莞�ԂɑΉ������X�R�A

int g_FrameCountBoss/* = 0*/;				// ���݂̃t���[����

int g_PlayKeySetIndexBoss/* = 0*/;			// �Đ����̃L�[�Z�b�g�C���f�b�N�X

int g_BossCurrentAnimation/* = MOTION_IDLE*/;	// �Đ����̃A�j���[�V����

int g_nFremeBoss/* = 0*/;					// �t���[����

int g_nCntKeyBoss/* = 1*/;					// �L�[��

int g_nChangeFlameBoss/* = 20*/;			// ���[�V�����̃t���[��

MOTIONSTATE_BOSS g_MotionStateBoss;			// ���[�V�����̍\����
MOTIONSTATE_BOSS g_MotionOldStateBoss;		// 1F�O�̃��[�V�����̍\����

D3DXVECTOR3 g_NumPosBoss[MAX_BOSS_PARTS];		// pos�̍���
D3DXVECTOR3 g_NumRotBoss[MAX_BOSS_PARTS];		// rot�̍���
D3DXVECTOR3 g_NumRotBossDest[MAX_BOSS_PARTS];	// rot�̍���

ANIMATION_BOSS g_aBossMotion[MOTION_MAX_BOSS];	// ���[�V�����̏��
                                    // ���f���p�[�c�̃f�[�^
ModelParts_BOSS g_aBoss[MAX_BOSS_PARTS] =
{
    { "MODEL/00_karada_mae.x" },
    { "MODEL/01_karada_usiro.x" },
    { "MODEL/02_atama.x" },
    { "MODEL/03_L_foot.x" },
    { "MODEL/04_L_sune.x" },
    { "MODEL/05_L_leg.x" },
    { "MODEL/06_R_foot.x" },
    { "MODEL/07_R_sune.x" },
    { "MODEL/08_R_leg.x" },
    { "MODEL/09_L_ude.x" },
    { "MODEL/10_R_ude.x" },
    { "MODEL/11_L_wing.x" },
    { "MODEL/12_R_wing.x" },
    { "MODEL/13_tail.x" },
};

//=====================================================
// �{�X����������
//=====================================================
void InitBoss(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�O���[�o���ϐ��̏�����
    g_MotionStateBoss = MOTION_IDLE_BOSS;
    g_nTimeScore = 0;

    g_FrameCountBoss = 0;				// ���݂̃t���[����

    g_PlayKeySetIndexBoss = 0;			// �Đ����̃L�[�Z�b�g�C���f�b�N�X

    g_BossCurrentAnimation = MOTION_IDLE_BOSS;	// �Đ����̃A�j���[�V����

    g_nFremeBoss = 0;					// �t���[����

    g_nCntKeyBoss = 1;					// �L�[��

    g_nChangeFlameBoss = 20;			// ���[�V�����̃t���[��

                                    //�\���̂̏�����
    g_Boss.pos = D3DXVECTOR3(0.0f, 0.0f, BOSS_FIRST_POS_Z);		// �����ʒu
    g_Boss.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ʒu
    g_Boss.size = BOSS_SIZE;
    g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��������
    g_Boss.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��������
    g_Boss.rotMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��������
    g_Boss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ړ���
    g_Boss.bUse = true;
    g_Boss.bStun = false;
    g_Boss.state = BOSSSTATE_STAY;
    g_Boss.nCounterRigid = 0;
    g_Boss.nStayTime = NORMAL_STAY_TIME;
    g_Boss.nAttackTime = 0;
    g_Boss.nChanceTime = 0;
    g_Boss.nCntFlinch = 0;
    g_Boss.nStunTime = FLINCH_TIME;
    g_Boss.fDistance = 9999.0f;
    g_Boss.fLife = BOSS_LIFE;	//���@�̗̑�

    //�e�̐ݒ�
    //g_ShadowIndexBoss = SetShadow(g_Boss.pos, g_Boss.rot);

    LoadBoss();
    LoadBossAnimation();

    // ���f���p�[�c�̏�����
    for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
    {
        //		g_Boss.modelParts[nCntParts].pos = g_aBoss[nCntParts].pos;			// �����ʒu
        //		g_Boss.modelParts[nCntParts].rot = g_aBoss[nCntParts].rot;			// �����p�x

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
//=====================================================
void UpdateBoss(void)
{
    //�{�X���������Ă���Ȃ�
    if (g_Boss.bUse == true)
    {
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
        float fAdjustment = 0.0f;  //��������
        float fDistanceY = 0.0f;   //���@�ƓG�̍����̍�
        float fKnockback = KNOCKBACK_SPIN;  //�X�s���U���̃m�b�N�o�b�N
        int nRandAttack = 0;        //�����_���ȍU���ɂ���
        float fCircle = 0.0f;    //�~��ɂ���
        float fSpeed = 0.0f;   //����
        float fRadius = 0.0f; //�G�t�F�N�g�������a

        //��������
        if (pPlayer->pos.y < g_Boss.pos.y + BOSS_HEIGHT)
        {
            fAdjustment = -1.0f;
            fDistanceY = g_Boss.pos.y + BOSS_HEIGHT - pPlayer->pos.y;
        }
        else if (pPlayer->pos.y == g_Boss.pos.y + BOSS_HEIGHT)
        {
            fAdjustment = 0.0f;
            fDistanceY = 0.0f;
        }
        else if (pPlayer->pos.y > g_Boss.pos.y + BOSS_HEIGHT)
        {
            fAdjustment = 1.0f;
            fDistanceY = pPlayer->pos.y - g_Boss.pos.y + BOSS_HEIGHT;
        }

        //�ʒu���ړ��ʂƕR�Â�
        g_Boss.pos += g_Boss.move;

        //�{�X��Ȃ�
        if (pCamera->bBossMode == true)
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
                            //�o���b�g
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_BULLET;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_BULLET;
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
                                //�X�s��
                                //�U���ɐ؂�ւ���
                                g_Boss.state = BOSSSTATE_ATTACK_SPIN;
                                //�U���t���[����ݒ�
                                g_Boss.nAttackTime = ATTACK_TIME_SPIN;
                            }

                        }

                        //60��
                        else if (nRandAttack > 40)
                        {
                            //�ʏ펞
                            if (g_Boss.fLife > BOSS_EXANGER_LIFE)
                            {
                                //�X�s��
                                //�U���ɐ؂�ւ���
                                g_Boss.state = BOSSSTATE_ATTACK_SPIN;
                                //�U���t���[����ݒ�
                                g_Boss.nAttackTime = ATTACK_TIME_SPIN;
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
                        //5%
                        if (nRandAttack <= 5)
                        {
                            //�X�s��
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_SPIN;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_SPIN;
                        }

                        //20%
                        else if (nRandAttack > 5 && nRandAttack <= 25)
                        {
                            //�o���b�g
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_BULLET;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_BULLET;
                        }

                        //75��
                        else if (nRandAttack > 25)
                        {
                            //�^�b�N��
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_TACKLE;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_TACKLE;
                        }
                    }

                    //�������̎�
                    else if(g_Boss.fDistance >= BOSS_CONSIDER_MEDIUM)
                    {
                        //5%
                        if (nRandAttack <= 5)
                        {
                            //�X�s��
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_SPIN;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_SPIN;
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
                            //�o���b�g
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_BULLET;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_BULLET;
                        }
                    }

                }

                g_MotionStateBoss = MOTION_IDLE_BOSS;   //�ҋ@

                break;

                //========================
                //�ːi�U����
                //========================
            case BOSSSTATE_ATTACK_TACKLE:

                //�X�^�����Ă��Ȃ��Ȃ�
                if (g_Boss.bStun == false)
                {
                    //�c��̗͂ɂ���āA�Ǐ]���ǂ�����ς���
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
                            //���@��Ǐ]
                            g_Boss.move.x = -sinf(fAngle)*ANGER_TACKLE_SPEED;
                            g_Boss.move.z = -cosf(fAngle)*ANGER_TACKLE_SPEED;
                        }
                        //���V��
                        else
                        {
                            //���@��ǂ�������
                            g_Boss.move.x = -sinf(g_Boss.rot.y)*EXANGER_TACKLE_SPEED;
                            g_Boss.move.z = -cosf(g_Boss.rot.y)*EXANGER_TACKLE_SPEED;
                        }

                        //���@�̂ق�������
                        g_Boss.rot.y = fAngle;
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
                        //�������߂��Ȃ�A�X�s���ɔh��
                        if (g_Boss.fDistance < RANGE_SPIN_XZ)
                        {
                            //�U���ɐ؂�ւ���
                            g_Boss.state = BOSSSTATE_ATTACK_SPIN;
                            //�U���t���[����ݒ�
                            g_Boss.nAttackTime = ATTACK_TIME_SPIN;
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

                g_MotionStateBoss = MOTION_WALK_BOSS;   //����

                break;

                //========================
                //�X�s���U����
                //========================
            case BOSSSTATE_ATTACK_SPIN:

                //�ړ��ʂ��Ȃ���
                g_Boss.move.x = 0.0f;
                g_Boss.move.z = 0.0f;

                //�X�^�����Ă��Ȃ��Ȃ�
                if (g_Boss.bStun == false)
                {
                    //���߂��I�������A���]������
                    if (g_Boss.nAttackTime < CHARGE_SPIN)
                    {
                        g_Boss.rot.y += D3DX_PI / 10.0f;    //1��]������
                    }

                    //�U�����蔭��
                    if (g_Boss.nAttackTime == 10)
                    {
                        if (g_Boss.fDistance < RANGE_SPIN_XZ && g_Boss.pos.y <= RANGE_SPIN_Y - pPlayer->pos.y)
                        {
                            //�K�E���͖��G
                            if (pPlayer->bChanceAttack == false)
                            {
                                //�ʏ펞
                                if (g_Boss.fLife > BOSS_ANGER_LIFE)
                                {
                                    HitPlayer(50, DAMAGE_SPIN);
                                }
                                //�{�莞
                                else
                                {
                                    HitPlayer(50, DAMAGE_SPIN*1.5f);
                                }     

                                //�m�b�N�o�b�N
                                pPlayer->move.x = pPlayer->pos.x - g_Boss.posOld.x;
                                pPlayer->move.z = pPlayer->pos.z - g_Boss.posOld.z;
                                D3DXVec3Normalize(&pPlayer->move, &pPlayer->move);  //�x�N�g���𐳋K��
                                pPlayer->move.x = pPlayer->move.x*fKnockback;
                                pPlayer->move.z = pPlayer->move.z*fKnockback;

                                //Y���㏸
                                pPlayer->move.y += UPPER_SPIN;    //�㏸

                                                                  //�q�b�g�G�t�F�N�g
                                float fAngle = 0.0f;    //�p�x
                                float fSpeed = 10.0f;   //����
                                CAMERA*pCamera = GetCamera();
                                for (int nCnt = 0; nCnt < 50; nCnt++)
                                {
                                    //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                                    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                                    D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->rot.y)*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->rot.y)*fSpeed);
                                    SetEffect2(pPlayer->pos, move, EFFECT2_SIZE_HIT, EFFECT2_INIT_COLOR_HIT, EFFECT2TYPE_HIT);
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
                    //�X�^�������ɁA���̍s���������邽�߂�1�ɂ���
                    g_Boss.nAttackTime = 1;
                }

                //���̍s��(�����Œ�ҋ@)�ɐ؂�ւ���
                if (g_Boss.nAttackTime == 0)
                {
                    //�����Œ�ҋ@�ɂ���
                    g_Boss.state = BOSSSTATE_CHANCE;

                    //�����Œ�ҋ@���Ԃ�ݒ�
                    g_Boss.nChanceTime = CHANCE_TIME_SPIN;
                }

                g_MotionStateBoss = MOTION_ATTACK_SPIN_BOSS;   //�X�s��

                break;

                //========================
                //�o���b�g�U����
                //========================
            case BOSSSTATE_ATTACK_BULLET:

                //�ړ��ʂ��Ȃ���
                g_Boss.move.x = 0.0f;
                g_Boss.move.z = 0.0f;

                //�X�^�����Ă��Ȃ��Ȃ�
                if (g_Boss.bStun == false)
                {
                    //���߂Ă���ԁA���@������悤�ɂ���
                    if (g_Boss.nAttackTime > BULLET_MOMENT)
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
                    else if (g_Boss.nAttackTime == BULLET_MOMENT)
                    {
                        //�ʏ펞
                        if (g_Boss.fLife > BOSS_ANGER_LIFE)
                        {
                            SetBullet(D3DXVECTOR3(g_Boss.pos.x, g_Boss.pos.y + BOSS_HEIGHT
                                , g_Boss.pos.z), BULLET_SIZE_BOSS,
                                D3DXVECTOR3(-sinf(g_Boss.rot.y)*BULLET_SPEED_BOSS, fDistanceY / BULLET_SMASH * fAdjustment
                                    , -cosf(g_Boss.rot.y)*BULLET_SPEED_BOSS), 80, BULLET_RIGID, BULLET_DAMAGE_BOSS, BULLETTYPE_BOSS);
                        }

                        //�{�莞
                        else
                        {

                            for (int nCnt = 0; nCnt < 350; nCnt++)
                            {
                                fSpeed = float(rand() % 2000 + 1000) / 100.0f;
                                //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                                fCircle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                                fRadius = float(rand() % 8000 + 0) / 100.0f;
                                D3DXVECTOR3 pos = D3DXVECTOR3(g_Boss.pos.x - sinf(fCircle + pCamera->rot.y)*fRadius
                                    , g_Boss.pos.y + BOSS_HEIGHT / 2 + cosf(fCircle)*fRadius
                                    , g_Boss.pos.z - cosf(fCircle + pCamera->rot.y)*fRadius);
                                D3DXVECTOR3 move = D3DXVECTOR3(-sinf(g_Boss.rot.y)*fSpeed, 0.0f, -cosf(g_Boss.rot.y)*fSpeed);
                                SetEffect2(pos, move, EFFECT2_SIZE_HEAL, EFFECT2_INIT_COLOR_HEAL, EFFECT2TYPE_FIRE2); //���G�t�F�N�g
                            }
                        }

                    }

                    //�ジ����
                    else if (g_Boss.nAttackTime < BULLET_MOMENT)
                    {
                        //�ジ���肷��
                        g_Boss.move.x = sinf(g_Boss.rot.y)*FLINCH_MOVE;
                        g_Boss.move.z = cosf(g_Boss.rot.y)*FLINCH_MOVE;
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
                    g_Boss.nChanceTime = CHANCE_TIME_BULLET;
                }

                g_MotionStateBoss = MOTION_ATTACK_BULLET_BOSS;   //�e�U��

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

                g_MotionStateBoss = MOTION_IDLE_BOSS;   //�ҋ@

                break;
            }

        }

        //�e�̓����蔻��
        CollisionBulletBoss(&g_Boss.pos, &g_Boss.posOld,
            D3DXVECTOR3(g_Boss.size.x, g_Boss.size.y * 2, g_Boss.size.z));

        //�e�̓����蔻��
        CollisionEffectBoss(&g_Boss.pos, &g_Boss.posOld,
            D3DXVECTOR3(g_Boss.size.x, g_Boss.size.y * 2, g_Boss.size.z));

        //����/�X�^���̏���
        if (g_Boss.bStun == false && g_Boss.nCounterRigid > BOSS_REACH_RIGID)
        {
            g_Boss.bStun = true;
            g_Boss.nCntFlinch++;        //���݉񐔂����Z
        }

        //����/�X�^����
        else if (g_Boss.bStun == true)
        {
            g_Boss.nCounterRigid = 0;    //����/�X�^���܂ł̒l�����Z�b�g
            g_Boss.nStunTime--;         //����/�X�^������

            //����
            if (g_Boss.nCntFlinch == 1)
            {
                g_MotionStateBoss = MOTION_FLINCH_BOSS;   //����

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
                g_MotionStateBoss = MOTION_FLINCH_BOSS;   //����

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
                g_MotionStateBoss = MOTION_STUN_BOSS;   //�X�^��

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

        //���S����
        if (g_Boss.fLife <= 0.0f)
        {
            g_nTimeScore = GetTime();

            ScoreCalculation((int)pPlayer->fLife * 30 + g_nTimeScore * 100 + 10000);

            //�����G�t�F�N�g
            float fSpeedX = 0.0f;   //����X
            float fSpeedY = 0.0f;   //����Y
            float fSpeedZ = 0.0f;   //����Z
            int nPlusMinusX = 0;
            int nPlusMinusY = 0;
            int nPlusMinusZ = 0;
            for (int nCnt = 0; nCnt < 200; nCnt++)
            {
                fSpeedX = float(rand() % 800 + 300) / 100.0f;
                fSpeedY = float(rand() % 800 + 300) / 100.0f;
                fSpeedZ = float(rand() % 800 + 300) / 100.0f;
                nPlusMinusX = int(rand() % 2 + 1);
                nPlusMinusY = int(rand() % 2 + 1);
                nPlusMinusZ = int(rand() % 2 + 1);
                if (nPlusMinusX == 1)
                {
                    nPlusMinusX = 1;
                }
                else
                {
                    nPlusMinusX = -1;
                }

                if (nPlusMinusY == 1)
                {
                    nPlusMinusY = 1;
                }
                else
                {
                    nPlusMinusY = 1;
                }

                if (nPlusMinusZ == 1)
                {
                    nPlusMinusZ = 1;
                }
                else
                {
                    nPlusMinusZ = -1;
                }

                D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX*nPlusMinusX, fSpeedY*nPlusMinusY, fSpeedZ*nPlusMinusZ);
                SetEffect2(g_Boss.pos, move, EFFECT2_SIZE_EXPLOSION, EFFECT2_INIT_COLOR_EXPLOSION2, EFFECT2TYPE_EXPLOSION); //�����G�t�F�N�g
            }

            g_Boss.bUse = false;    //���@�𖢎g�p��

            PlaySound(SOUND_LABEL_SE_VOICE);

            SetCampaignState(CAMPAIGNSTATE_GAMEOVER);    //�Q�[���N���A��ʂ�     
        }

        //���f���̃A�j���[�V����
        UpdateBossMotion();
    }
}

//=====================================================
// �{�X�`�揈��
//=====================================================
void DrawBoss(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^
    D3DXMATRIX mtxRot, mtxTrans, mtxParent;
    D3DMATERIAL9 matDef;		// ���݂̃}�e���A���ێ��p
    D3DXMATERIAL* pMat;			// �}�e���A���f�[�^�ւ̃|�C���^

    //�{�X�����݂��Ă���Ȃ�`��
    if (g_Boss.bUse == true)
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
//=============================================================================
BOSS*GetBoss(void)
{
    return &g_Boss;
}

//=============================================================================
// �{�X�̃_���[�W�ݒ�
//=============================================================================
void HitBoss(int nCntRigid, float fDamage)
{
    //�c��̗͂ɂ���āA�X�^���l�̏㏸��ݒ�
    if (g_Boss.fLife > BOSS_ANGER_LIFE)
    {
        g_Boss.nCounterRigid += nCntRigid; //�X�^���l��������
    }
    //�c��̗͔�������A�X�^���l�������肸�炭�Ȃ�
    else
    {
        g_Boss.nCounterRigid += nCntRigid / 2; //�X�^���l��������
    }

    //�c��̗͂ɂ���āA�󂯂�_���[�W��ݒ�
    if (g_Boss.fLife > BOSS_EXANGER_LIFE)
    {
        //�_���[�W�v�Z
        g_Boss.fLife -= fDamage;
    }
    //�c��̗�4����1����A�d������
    else
    {
        //�_���[�W�v�Z
        g_Boss.fLife -= fDamage / 2;
    }

    //�K�E�Q�[�W�㏸
    PLAYER*pPlayer = GetPlayer();
    if (pPlayer->bChanceAttack == false)    //�K�E�U�����ȊO��
    {
        //�e�̏ꍇ
        if (pPlayer->bBullet == true)
        {
            pPlayer->fChanceMeter += HIT_BULLET_CHARGE;
        }

        //���̏ꍇ
        else
        {
            pPlayer->fChanceMeter += HIT_SWORD_CHARGE;
        }
    }
}

//=====================================================
// �����蔻��̐ݒ�
//=====================================================
bool CollisionBoss(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bHit = false;  //�����������ǂ���

    //�{�X�g��ꂽ��
    if (g_Boss.bUse == true)
    {

        //�����蔻����v�Z
        if (RectangleCollision(pPos, pPosOld, size, &g_Boss.pos, g_Boss.size) == true)
        {
            PLAYER*pPlayer = GetPlayer();

            //�K�E���͖��G
            if (pPlayer->bChanceAttack == false)
            {

                HitPlayer(5, HIT_BOSS_DAMAGE);

                //�m�b�N�o�b�N
                float fKnockback = VALUE_KNOCKBACK;
                pPlayer->move.x = pPlayer->pos.x - g_Boss.posOld.x;
                pPlayer->move.z = pPlayer->pos.z - g_Boss.posOld.z;
                D3DXVec3Normalize(&pPlayer->move, &pPlayer->move);  //�x�N�g���𐳋K��
                pPlayer->move.x = pPlayer->move.x*fKnockback;
                pPlayer->move.z = pPlayer->move.z*fKnockback;

                //�q�b�g�G�t�F�N�g
                float fAngle = 0.0f;    //�p�x
                float fSpeed = 10.0f;   //����
                CAMERA*pCamera = GetCamera();
                for (int nCnt = 0; nCnt < 50; nCnt++)
                {
                    //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                    D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->rot.y)*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->rot.y)*fSpeed);
                    SetEffect2(pPlayer->pos, move, EFFECT2_SIZE_HIT, EFFECT2_INIT_COLOR_HIT, EFFECT2TYPE_HIT);
                }
            }

            bHit = true;    //��������
        }
    }

    return bHit;    //�����������ǂ�����Ԃ�
}

//=============================================================================
// �A�j���[�V�������[�h
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
    pFile = fopen("motionboss.txt", "r");

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
    pFile = fopen("boss.txt", "r");

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
//=============================================================================
void UpdateBossMotion(void)
{
    //���[�V�������ς�����Ƃ�
    if (g_MotionOldStateBoss != g_MotionStateBoss) {
        g_nFremeBoss = 0;
        g_nCntKeyBoss = 0;
    }
    for (int nCntMotion = 0; nCntMotion < MAX_BOSS_PARTS; nCntMotion++)
    {
        if (g_nFremeBoss == 0)
        {
            g_NumPosBoss[nCntMotion].x = ((g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].pos[nCntMotion].x -
                (g_aBoss[nCntMotion].pos.x - g_aBoss[nCntMotion].posOrigin.x)) / float(g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].nFrame));

            g_NumPosBoss[nCntMotion].y = ((g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].pos[nCntMotion].y -
                (g_aBoss[nCntMotion].pos.y - g_aBoss[nCntMotion].posOrigin.y)) / float(g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].nFrame));

            g_NumPosBoss[nCntMotion].z = ((g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].pos[nCntMotion].z -
                (g_aBoss[nCntMotion].pos.z - g_aBoss[nCntMotion].posOrigin.z)) / float(g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].nFrame));

            g_NumRotBossDest[nCntMotion] =
                g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].rot[nCntMotion] - g_aBoss[nCntMotion].rot;

            g_NumRotBoss[nCntMotion] = (g_NumRotBossDest[nCntMotion] / float(g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].nFrame));
        }

        g_aBoss[nCntMotion].pos += g_NumPosBoss[nCntMotion];
        g_aBoss[nCntMotion].rot += g_NumRotBoss[nCntMotion];
    }
    g_MotionOldStateBoss = g_MotionStateBoss;
    // ���[�v����Ƃ�
    if (g_nFremeBoss >= g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].nFrame && g_aBossMotion[g_MotionStateBoss].nLoop == 1)
    {
        g_nFremeBoss = 0;
        g_nCntKeyBoss++;

        if (g_nCntKeyBoss >= g_aBossMotion[g_MotionStateBoss].nNum_Key)
        {
            g_nCntKeyBoss = 0;
        }

    }
    // ���[�v���Ȃ��Ƃ�
    else if (g_nFremeBoss >= g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].nFrame && g_aBossMotion[g_MotionStateBoss].nLoop == 0)
    {
        g_nFremeBoss = 0;
        g_nCntKeyBoss++;
        if (g_nCntKeyBoss >= g_aBossMotion[g_MotionStateBoss].nNum_Key)
        {
            for (int nCntMotion = 0; nCntMotion < MAX_BOSS_PARTS; nCntMotion++)
            {
                g_NumPosBoss[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_NumRotBoss[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
            g_MotionStateBoss = MOTION_IDLE_BOSS;
            g_nCntKeyBoss = 0;
        }
    }
    else
    {
        g_nFremeBoss++;
    }
}