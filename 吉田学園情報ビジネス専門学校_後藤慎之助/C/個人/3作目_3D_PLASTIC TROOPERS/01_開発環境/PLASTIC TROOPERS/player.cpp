//=====================================================
//
// 3D�Q�[���̂ЂȌ`�̏���[player.h]
// Author:�㓡�T�V��
//
//=====================================================

//=====================================================
// �C���N���[�h�t�@�C��
//=====================================================
#include "player.h"
#include "camera.h"
#include "bullet.h"
#include "input.h"
#include "shadow.h"
#include "controller.h"
#include "block.h"
#include "wall.h"
#include "enemy.h"
#include "collision.h"
#include "item.h"
#include "campaign.h"
#include "effect.h"
#include "effect2.h"
#include "enemy.h"
#include "sound.h"
#include "boss.h"

//=====================================================
// �O���[�o���ϐ���`
//=====================================================
PLAYER g_Player;        //�v���C���[�̏��
int g_ShadowIndex;
float g_fAngle;         //�X�e�B�b�N�̊p�x�L���p
int g_nCntCharge;       //�`���[�W���Ԃ��J�E���g
bool g_bChange;         //���펝���ւ��p
bool g_bReachBoss;      //�{�X�ɂ��ǂ蒅�������ǂ���

int g_FrameCount/* = 0*/;				// ���݂̃t���[����

int g_PlayKeySetlndex/* = 0*/;			// �Đ����̃L�[�Z�b�g�C���f�b�N�X

int g_PlayerCurrentAnimation/* = MOTION_IDLE*/;	// �Đ����̃A�j���[�V����

int g_nFreme/* = 0*/;					// �t���[����

int g_nCntKey/* = 1*/;					// �L�[��

int g_nChangeFlame/* = 20*/;			// ���[�V�����̃t���[��

MOTIONSTATE g_MotionState;			// ���[�V�����̍\����
MOTIONSTATE g_MotionOldState;		// 1F�O�̃��[�V�����̍\����

D3DXVECTOR3 g_NumPos[MAX_PARTS];		// pos�̍���
D3DXVECTOR3 g_NumRot[MAX_PARTS];		// rot�̍���
D3DXVECTOR3 g_NumRotDest[MAX_PARTS];	// rot�̍���

ANIMATION g_aPMotion[MOTION_MAX];	// ���[�V�����̏��
                                    // ���f���p�[�c�̃f�[�^
ModelParts g_aPlayer[MAX_PARTS] =
{
    { "MODEL/00_body.x" },
    { "MODEL/01_head.x" },
    { "MODEL/02_armR.x" },
    { "MODEL/03_handR.x" },
    { "MODEL/04_armL.x" },
    { "MODEL/05_handL.x" },
    { "MODEL/06_legR.x" },
    { "MODEL/07_footR.x" },
    { "MODEL/08_legL.x" },
    { "MODEL/09_footL.x" },
    { "MODEL/10_weapon.x" },
    { "MODEL/11_weapon2.x" },
};

//=====================================================
// �v���C���[����������
//=====================================================
void InitPlayer(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�O���[�o���ϐ��̏�����
    g_MotionState = MOTION_IDLE;
    g_fAngle = 0.0f;
    g_nCntCharge = 0;
    g_Player.bChanceAttack = false;
    g_bChange = false;
    g_bReachBoss = false;

    g_FrameCount = 0;				// ���݂̃t���[����

    g_PlayKeySetlndex = 0;			// �Đ����̃L�[�Z�b�g�C���f�b�N�X

    g_PlayerCurrentAnimation = MOTION_IDLE;	// �Đ����̃A�j���[�V����

    g_nFreme = 0;					// �t���[����

    g_nCntKey = 1;					// �L�[��

    g_nChangeFlame = 20;			// ���[�V�����̃t���[��

    //�\���̂̏�����
    g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, -3500.0f);		// �����ʒu
    g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ʒu
    g_Player.rot = D3DXVECTOR3(0.0f, 1800.0f, 0.0f);		// ��������
    g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ړ���
    g_Player.bDisp = true;
    g_Player.bJump = false;
    g_Player.bSecondJump = false;
    g_Player.bSkyDash = false;
    g_Player.bLand = false;
    g_Player.bGuard = false;
    g_Player.bBullet = true;
    g_Player.state = PLAYERSTATE_NORMAL;
    g_Player.nCounterState = 0;
    g_Player.nCntSkyDash = 0;
    g_Player.nAttackTime = 0;
    g_Player.nCntEXAttackCD = 0;
    g_Player.fLife = PLAYER_LIFE;	//���@�̗̑�
    g_Player.fEnergy = PLAYER_ENERGY;	//���@�̃G�i�W�[
    g_Player.fOldEnergy = PLAYER_ENERGY;	//���@��1F�G�i�W�[
    g_Player.fChanceMeter = 0.0f;           //�K�E�Q�[�W
    g_Player.nChance = 0;

    //�e�̐ݒ�
    //g_ShadowIndex = SetShadow(g_Player.pos, g_Player.rot);

    LoadPlayer();
    LoadAnimation();

    // ���f���p�[�c�̏�����
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        //		g_Player.modelParts[nCntParts].pos = g_aPlayer[nCntParts].pos;			// �����ʒu
        //		g_Player.modelParts[nCntParts].rot = g_aPlayer[nCntParts].rot;			// �����p�x

        g_Player.modelParts[nCntParts].nldxModelParent = 0;
    }
    // �t�@�C���̓ǂݍ���
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        g_Player.modelParts[nCntParts].nldxModelParent = g_aPlayer[nCntParts].nParents;

        // X�t�@�C���̓ǂݍ���
        D3DXLoadMeshFromX(g_aPlayer[nCntParts].pFileName,
            D3DXMESH_SYSTEMMEM,
            pDevice,
            NULL,
            &g_Player.modelParts[nCntParts].pBuffMat,
            NULL,
            &g_Player.modelParts[nCntParts].nNumMat,
            &g_Player.modelParts[nCntParts].pMesh);

    }
}

//=====================================================
// �v���C���[�I������
//=====================================================
void UninitPlayer(void)
{
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        // ���b�V���̔j��
        if (g_Player.modelParts[nCntParts].pMesh != NULL)
        {
            g_Player.modelParts[nCntParts].pMesh->Release();
            g_Player.modelParts[nCntParts].pMesh = NULL;
        }
        // �}�e���A���̔j��
        if (g_Player.modelParts[nCntParts].pBuffMat != NULL)
        {
            g_Player.modelParts[nCntParts].pBuffMat->Release();
            g_Player.modelParts[nCntParts].pBuffMat = NULL;
        }
    }
}

//=====================================================
// �v���C���[�X�V����
//=====================================================
void UpdatePlayer(void)
{
    //�R���g���[���[�̏����擾����
    DIJOYSTATE2 Controller = GetController();
    //�J�����̏����擾����
    CAMERA*pCamera = GetCamera();
    //�G�̏����擾����
    ENEMY*pEnemy;
    BOSS*pBoss;

    //�v���C���[�̏��
    switch (g_Player.state)
    {
    case PLAYERSTATE_NORMAL:	//�ʏ�̏��
        break;

    case PLAYERSTATE_DAMAGE:	//�_���[�W���
        g_Player.nCounterState--;

        if (g_Player.nCounterState <= 0)
        {
            g_Player.state = PLAYERSTATE_NORMAL;

        }
        break;
    }

    //�v���C���[���������Ă���Ȃ�X�V
    if (g_Player.bDisp == true)
    {
        // �v���C���[�������Ă��Ȃ��Ƃ�
        D3DXVECTOR3 lengthCheckVec = g_Player.move;
        lengthCheckVec.y = 0;
        if (D3DXVec3Length(&lengthCheckVec) <= 1.5f && g_Player.bJump == false)
        {
            g_MotionState = MOTION_IDLE;    //���[�V�������A�C�h���ɂ���
        }

        //1F�O�̈ʒu���L��
        g_Player.posOld = g_Player.pos;

        //1F�O�̃G�i�W�[���L��
        g_Player.fOldEnergy = g_Player.fEnergy;

        //�d��
        g_Player.move.y -= GRAVITY;

        //�m�b�N�o�b�N�𐧌�
        if (g_Player.move.x != 0.0f || g_Player.move.z != 0.0f)
        {
            g_Player.move.x *= 0.8f;
            g_Player.move.z *= 0.8f;
        }

        //�U�����Ă��Ȃ��Ƃ�
        if (g_Player.nAttackTime == 0)
        {

            //�W�����v
            if (g_Player.bSkyDash == false && g_Player.bJump == false && GetJoypadTrigger(BUTTON_A))
            {
                g_Player.move.y = 0;
                g_Player.move.y += VALUE_JUMP;
                g_Player.bJump = true;
                g_Player.bSecondJump = true;
                g_MotionState = MOTION_JUMP;    //���[�V�������W�����v�ɂ���
            }

            //2�i�ڃW�����v
            else if (g_Player.bSecondJump == true && GetJoypadTrigger(BUTTON_A))
            {
                g_Player.move.y = 0;
                g_Player.move.y += VALUE_JUMP;
                g_Player.bSecondJump = false;
                g_MotionState = MOTION_JUMP;    //���[�V�������W�����v�ɂ���
            }
        }

        //�ʒu�X�V
        g_Player.pos += g_Player.move;

        //�n�ʂɂ���
        if (g_Player.pos.y < 0)
        {
            g_Player.move.y = 0;	//�d�͂�؂�
            g_Player.pos.y = 0;	//�߂肱�܂Ȃ��悤�ɂ���
            //g_Player.rot.x = 0;	//�����𒼂�
            g_Player.bJump = false;
            g_Player.bSecondJump = false;
            g_Player.bSkyDash = false;
            g_Player.nCntSkyDash = 0;

            //�U�����Ă��Ȃ��Ƃ�
            if (g_Player.nAttackTime == 0)
            {

                //�K�[�h�����Ă���
                if (GetJoypadPress(BUTTON_L1) || GetJoypadPress(BUTTON_L2))
                {
                    g_Player.bGuard = true;
                }
                else
                {
                    g_Player.bGuard = false;
                }

                if (Controller.lY != 0 || Controller.lX != 0)
                {
                    float fAngle = atan2(Controller.lX, Controller.lY*-1);//�X�e�B�b�N�̊p�x�����߂�
                    float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//�X�e�B�b�N�̊p�x�����߂�

                    //�X�e�B�b�N�ƃJ�����̌������L�^����
                    g_fAngle = pCamera->rot.y + fAngle;

                    //�K�[�h���̕���
                    if (g_Player.bGuard == true)
                    {
                        g_Player.pos.x += sinf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                        g_Player.pos.z += cosf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                    }

                    //�K�[�h���Ă��Ȃ�
                    else
                    {
                        //�_�b�V��
                        if (GetJoypadPress(BUTTON_B) && g_Player.fEnergy != 0.0f)
                        {
                            g_Player.fEnergy -= ENERGY_DASH;
                            g_Player.pos.x += sinf(pCamera->rot.y + fAngle) *VALUE_DASH;
                            g_Player.pos.z += cosf(pCamera->rot.y + fAngle) *VALUE_DASH;
                            g_MotionState = MOTION_WALK;//���[�V����������ɂ���
                        }

                        //����
                        else
                        {
                            g_Player.pos.x += sinf((pCamera->rot.y + fAngle))*VALUE_MOVE;
                            g_Player.pos.z += cosf((pCamera->rot.y + fAngle))*VALUE_MOVE;
                            g_MotionState = MOTION_WALK;//���[�V����������ɂ���
                            //SetEffect2(g_Player.pos, g_Player.pos, 0.0f,EFFECT2TYPE_RUN);
                        }

                    }
                    //�L�����̌�����ς���
                    g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
                }
                else
                {
                    //�_�b�V��
                    if (GetJoypadPress(BUTTON_B) && g_Player.bGuard == false && g_Player.fEnergy != 0.0f)
                    {
                        g_Player.fEnergy -= ENERGY_DASH;
                        g_Player.pos.x += sinf(g_fAngle) *VALUE_DASH;
                        g_Player.pos.z += cosf(g_fAngle) *VALUE_DASH;
                        g_MotionState = MOTION_WALK;//���[�V����������ɂ���
                    }
                }
            }
        }

        //�n�ʂ���ɂ���
        else
        {
            g_Player.bJump = true;
            g_Player.bGuard = false;   //�󒆂̓K�[�h�ł��Ȃ�

            //�U�����Ă��Ȃ��Ƃ�
            if (g_Player.nAttackTime == 0)
            {

                //�X�e�B�b�N���|��Ă���
                if (Controller.lY != 0 || Controller.lX != 0)
                {
                    //�󒆃_�b�V�����́A�����]���s��
                    if (g_Player.bSkyDash == false)
                    {
                        float fAngle = atan2(Controller.lX, Controller.lY*-1);//�X�e�B�b�N�̊p�x�����߂�
                        float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//�X�e�B�b�N�̊p�x�����߂�

                        //�X�e�B�b�N�ƃJ�����̌������L�^����
                        g_fAngle = pCamera->rot.y + fAngle;

                        //�u���b�N�̏�
                        if (g_Player.bLand == true)
                        {
                            //�K�[�h�����Ă���
                            if (GetJoypadPress(BUTTON_L1) || GetJoypadPress(BUTTON_L2))
                            {
                                g_Player.bGuard = true;
                            }
                            else
                            {
                                g_Player.bGuard = false;
                            }

                            //�K�[�h���̕���
                            if (g_Player.bGuard == true)
                            {
                                g_Player.pos.x += sinf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                                g_Player.pos.z += cosf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                            }

                            //�K�[�h���Ă��Ȃ�
                            else
                            {
                                //�_�b�V��
                                if (GetJoypadPress(BUTTON_B) && g_Player.fEnergy != 0.0f)
                                {
                                    g_Player.bSkyDash = true;
                                }

                                //�u���b�N�����
                                else
                                {
                                    g_Player.pos.x += sinf(g_fAngle) *VALUE_MOVE;
                                    g_Player.pos.z += cosf(g_fAngle) *VALUE_MOVE;
                                    g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
                                    g_MotionState = MOTION_WALK;//���[�V����������ɂ���
                                }
                            }
                        }

                        //��
                        else
                        {
                            //�󒆃_�b�V��
                            if (GetJoypadPress(BUTTON_B) && g_Player.fEnergy != 0.0f)
                            {
                                g_Player.bSkyDash = true;
                            }

                            //�󒆕���
                            else
                            {
                                g_Player.pos.x += sinf(g_fAngle) *VALUE_MOVE;
                                g_Player.pos.z += cosf(g_fAngle) *VALUE_MOVE;
                                g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
                            }
                        }
                    }

                    //�󒆃_�b�V���̈ړ�
                    if (g_Player.bSkyDash == true)
                    {
                        g_Player.nCntSkyDash++;    //�󒆈ړ��̎��Ԃ𐔂���

                        //�W�����v�ŁA�󒆃_�b�V���L�����Z��
                        if (GetJoypadTrigger(BUTTON_A))
                        {
                            g_Player.nCntSkyDash = SKY_DUSH_TIME;
                        }

                        //�󒆃_�b�V���̃t���[����
                        if (g_Player.nCntSkyDash > 0 && g_Player.nCntSkyDash < SKY_DUSH_TIME)
                        {
                            g_Player.fEnergy -= ENERGY_DASH;
                            //��i�ڂ̃W�����v���̋󒆃_�b�V��
                            if (g_Player.bSecondJump == true)
                            {
                                g_Player.move.y = GRAVITY;  //�󒆂ɂƂǂ܂�悤�ɂ���
                            }
                            g_Player.pos.x += sinf(g_fAngle) *VALUE_DASH;
                            g_Player.pos.z += cosf(g_fAngle) *VALUE_DASH;

                            //�u���b�N�̏�ɂ���Ƃ�
                            if (g_Player.bLand == true)
                            {
                                g_MotionState = MOTION_WALK;//���[�V����������ɂ���
                                float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//�X�e�B�b�N�̊p�x�����߂�
                                //�L�����̌�����ς���
                                g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
                            }
                            else
                            {
                                g_MotionState = MOTION_SKYDASH;//���[�V�������󒆃_�b�V���ɂ���
                            }
                        }

                        //�󒆃_�b�V���̃t���[��������ȏ�ɂȂ�����
                        else if (g_Player.nCntSkyDash >= SKY_DUSH_TIME)
                        {
                            g_MotionState = MOTION_WALK;//���[�V����������ɂ���
                            g_Player.nCntSkyDash = SKY_DUSH_TIME;
                            g_Player.pos.x += sinf(g_fAngle) *VALUE_AFTER_SKYDASH;
                            g_Player.pos.z += cosf(g_fAngle) *VALUE_AFTER_SKYDASH;
                        }
                    }
                }

                //�X�e�B�b�N���|��Ă��Ȃ�
                else
                {
                    //�_�b�V��
                    if (g_Player.bSkyDash == false && GetJoypadPress(BUTTON_B) && g_Player.bGuard == false && g_Player.fEnergy != 0.0f)
                    {
                        g_Player.bSkyDash = true;
                    }

                    //�󒆃_�b�V���̈ړ�
                    if (g_Player.bSkyDash == true)
                    {
                        g_Player.nCntSkyDash++;    //�󒆈ړ��̎��Ԃ𐔂���

                        //�W�����v�ŁA�󒆃_�b�V���L�����Z��
                        if (GetJoypadTrigger(BUTTON_A))
                        {
                            g_Player.nCntSkyDash = SKY_DUSH_TIME;
                        }

                        //�󒆃_�b�V���̃t���[����
                        if (g_Player.nCntSkyDash > 0 && g_Player.nCntSkyDash < SKY_DUSH_TIME)
                        {
                            g_Player.fEnergy -= ENERGY_DASH;
                            //��i�ڂ̃W�����v���̋󒆃_�b�V��
                            if (g_Player.bSecondJump == true)
                            {
                                g_Player.move.y = GRAVITY;  //�󒆂ɂƂǂ܂�悤�ɂ���
                            }
                            g_Player.pos.x += sinf(g_fAngle) *VALUE_DASH;
                            g_Player.pos.z += cosf(g_fAngle) *VALUE_DASH;


                            //�u���b�N�̏�ɂ���Ƃ�
                            if (g_Player.bLand == true)
                            {
                                g_MotionState = MOTION_WALK;//���[�V����������ɂ���
                            }
                            else
                            {
                                g_MotionState = MOTION_SKYDASH;//���[�V�������󒆃_�b�V���ɂ���
                            }
                        }

                        //�󒆃_�b�V���̃t���[��������ȏ�ɂȂ�����
                        else if (g_Player.nCntSkyDash >= SKY_DUSH_TIME)
                        {
                            g_Player.nCntSkyDash = SKY_DUSH_TIME;
                            g_Player.pos.x += sinf(g_fAngle) *VALUE_AFTER_SKYDASH;
                            g_Player.pos.z += cosf(g_fAngle) *VALUE_AFTER_SKYDASH;
                            g_MotionState = MOTION_WALK;//���[�V����������ɂ���
                        }
                    }
                }
            }
        }

        //�K�[�h
        if (g_Player.bGuard == true && g_Player.nAttackTime == 0)
        {
            g_MotionState = MOTION_GUARD;//���[�V�������K�[�h�ɂ���

            //�ʏ탂�[�h
            if (pCamera->bBossMode == false)
            {
                pEnemy = GetEnemy();
                for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
                {
                    //��ԋ߂��G�Ɍ���
                    if (pEnemy->nRank == 1)
                    {

                        float fPposx = g_Player.pos.x, fPposz = g_Player.pos.z;	//���@�̍��W
                        float fEposx = pEnemy->pos.x, fEposz = pEnemy->pos.z;		//�G�̍��W
                        float fAngle;											//�p�x

                        fAngle = atan2f((fPposx - fEposx), (fPposz - fEposz));	//�p�x�����߂�

                                                                                //���b�N�I�������G�̂ق�������
                        if (pCamera->bLockOn == true)
                        {
                            g_Player.rot.y = fAngle;
                        }
                    }
                }
            }

            //�{�X���[�h
            else
            {
                pBoss = GetBoss();
                float fPposx = g_Player.pos.x, fPposz = g_Player.pos.z;	//���@�̍��W
                float fBposx = pBoss->pos.x, fBposz = pBoss->pos.z;		//�G�̍��W
                float fAngle;											//�p�x

                fAngle = atan2f((fPposx - fBposx), (fPposz - fBposz));	//�p�x�����߂�

                //���b�N�I�������G�̂ق�������
                if (pCamera->bLockOn == true)
                {
                    g_Player.rot.y = fAngle;
                }
            }

        }

        //�U��
        if (GetJoypadTrigger(BUTTON_X) && g_Player.nAttackTime == 0 && g_Player.fEnergy != 0.0f)
        {
            if (g_Player.bBullet == true)
            {
                //�e�U���̃J�E���^�[
                g_Player.nAttackTime = 40;
            }
            else
            {
                //���U���̃J�E���^�[
                g_Player.nAttackTime = 45;
            }
            

            //�K�[�h����������
            if (g_Player.bGuard == true)
            {
                g_Player.bGuard = false;
            }

            //�J���������b�N�I��������
            pCamera->bLockOn = true;
        }

        //����؂�ւ�
        if (GetJoypadTrigger(BUTTON_Y) && g_Player.nAttackTime == 0 && g_Player.fEnergy != 0.0f)
        {
            //�U���̃J�E���^�[
            g_Player.nAttackTime = 30;

            //�K�[�h����������
            if (g_Player.bGuard == true)
            {
                g_Player.bGuard = false;
            }

            //����`�F���W�̃t���O
            g_bChange = true;
        }

        //�K�E�U��
        if (GetJoypadTrigger(BUTTON_R1) && g_Player.nAttackTime == 0 
            && g_Player.fEnergy > 0.0f && g_Player.nChance > 0 && g_Player.nCntEXAttackCD == 0||
            GetJoypadTrigger(BUTTON_R2) && g_Player.nAttackTime == 0 
            && g_Player.fEnergy > 0.0f && g_Player.nChance > 0 && g_Player.nCntEXAttackCD == 0)
        {
            //�U���̃J�E���^�[
            g_Player.nAttackTime = VALUE_EXATTACK_FRAME;

            //�K�[�h����������
            if (g_Player.bGuard == true)
            {
                g_Player.bGuard = false;
            }

            //�`�����X�l3�̂Ƃ��A���[�^�[��0�ɂ���
            if (g_Player.nChance == 3)
            {
                g_Player.fChanceMeter = 0.0f;
            }

            //�`�����X�l��1����
            g_Player.nChance--;

            //�K�E�Z�̃N�[���^�C��
            g_Player.nCntEXAttackCD = EXATTACK_CD;

            //�K�E���ɂ���
            g_Player.bChanceAttack = true;

            //�J���������b�N�I��������
            pCamera->bLockOn = true;
        }

        //�U�����̃J�E���^�[
        if (g_Player.nAttackTime > 0)
        {
            //�U�����Ԃ��J�E���g
            g_Player.nAttackTime--;

            //���펝���ւ�
            if (g_bChange == true)
            {
                g_MotionState = MOTION_GUARD;//���[�V�����𕐊펝���ւ��ɂ���

                if (g_Player.nAttackTime == 15)
                {
                    //���]
                    g_Player.bBullet = !g_Player.bBullet;
                }

                //����`�F���W������
                else if (g_Player.nAttackTime == 0)
                {
                    g_bChange = false;
                }
            }

            //�U��
            else
            {
                //�K�E�U��
                if (g_Player.bChanceAttack == true)
                {

                    //�e�̕K�E��
                    if (g_Player.bBullet == true)
                    {
                        g_MotionState = MOTION_BULLET;//���[�V�������e�U���ɂ���

                                                      //�K�[�h���Ɠ����ʁA������
                                                      //�X�e�B�b�N���|��Ă���
                        if (Controller.lY != 0 || Controller.lX != 0)
                        {
                            float fAngle = atan2(Controller.lX, Controller.lY*-1);//�X�e�B�b�N�̊p�x�����߂�
                            float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//�X�e�B�b�N�̊p�x�����߂�

                                                                                        //�X�e�B�b�N�ƃJ�����̌������L�^����
                            g_fAngle = pCamera->rot.y + fAngle;

                            g_Player.pos.x += sinf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                            g_Player.pos.z += cosf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                            g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
                        }

                        //���̃G�t�F�N�g
                        if (g_Player.nAttackTime > 60)
                        {
                            float fMoveX = 0.0f;    //Y�̈ړ���
                            float fMoveY = 0.0f;    //Y�̈ړ���
                            float fMoveZ = 0.0f;    //Y�̈ړ���
                            int nPlusMinusY = 0;

                            for (int nCnt = 0; nCnt < 4; nCnt++)
                            {
                                nPlusMinusY = int(rand() % 2 + 1);

                                if (nPlusMinusY == 1)
                                {
                                    nPlusMinusY = 1;
                                }
                                else
                                {
                                    nPlusMinusY = -1;
                                }
                                //�������̈ړ�
                                fMoveX = float(rand() % 800 + 300) / 100.0f;
                                //�c�����̈ړ�
                                fMoveY = float(rand() % 300 + 0) / 100.0f;
                                //�������̈ړ�
                                fMoveZ = float(rand() % 800 + 300) / 100.0f;

                                D3DXVECTOR3 move = D3DXVECTOR3(sinf(g_fAngle) *fMoveX, fMoveY*nPlusMinusY, cosf(g_fAngle) *fMoveZ);
                                SetEffect2(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + 80.0f, g_Player.modelParts[10].mtxWorld._43)
                                    , move, EFFECT2_SIZE_HEAL, EFFECT2_INIT_COLOR_HEAL, EFFECT2TYPE_FIRE); //�X�g�[���\�[�h�G�t�F�N�g��
                            }
                        }

                        //�`�����X�A�^�b�N������
                        else if (g_Player.nAttackTime == 0)
                        {
                            g_Player.bChanceAttack = false;
                        }
                    }

                    //���̕K�E��
                    else
                    {
                        g_MotionState = MOTION_SWORD2;//���[�V������K�E�ɂ���

                        //�ʏ탂�[�h
                        if (pCamera->bBossMode == false)
                        {
                            //�G���擾
                            pEnemy = GetEnemy();

                            for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
                            {
                                //��ԋ߂��G�Ɍ���
                                if (pEnemy->nRank == 1)
                                {

                                    float fPposx = g_Player.pos.x, fPposz = g_Player.pos.z;	//���@�̍��W
                                    float fEposx = pEnemy->pos.x, fEposz = pEnemy->pos.z;	//�G�̍��W
                                    float fAngle;											//�p�x

                                    float fAdjustment;  //�����̒���

                                    float fDistanceY;   //Y���̍�

                                    fAngle = atan2f((fPposx - fEposx), (fPposz - fEposz));	//�p�x�����߂�

                                                                                            //��������
                                    if (g_Player.pos.y > pEnemy->pos.y)
                                    {
                                        fAdjustment = -1.0f;

                                        //Y��
                                        fDistanceY = g_Player.pos.y - pEnemy->pos.y;
                                    }
                                    else if (g_Player.pos.y == pEnemy->pos.y)
                                    {
                                        fAdjustment = 0.0f;

                                        fDistanceY = 0.0f;
                                    }
                                    else if (g_Player.pos.y < pEnemy->pos.y)
                                    {
                                        fAdjustment = 1.0f;

                                        //Y��
                                        fDistanceY = pEnemy->pos.y - g_Player.pos.y;
                                    }

                                    //�����蔻��̔���
                                    if (g_Player.nAttackTime == 165)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 150)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 135)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 120)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 105)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 90)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 75)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 60)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 45)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 30)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 15)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }

                                }

                            }
                        }

                        //�{�X���[�h
                        else
                        {
                            pBoss = GetBoss();
                            float fPposx = g_Player.pos.x, fPposz = g_Player.pos.z;	//���@�̍��W
                            float fBposx = pBoss->pos.x, fBposz = pBoss->pos.z;		//�G�̍��W
                            float fAngle;											//�p�x

                            fAngle = atan2f((fPposx - fBposx), (fPposz - fBposz));	//�p�x�����߂�

                            float fAdjustment;  //�����̒���

                            float fDistanceY;   //Y���̍�


                            //��������
                            if (g_Player.pos.y > pBoss->pos.y + BOSS_HEIGHT)
                            {
                                fAdjustment = -1.0f;

                                //Y��
                                fDistanceY = g_Player.pos.y - pBoss->pos.y + BOSS_HEIGHT;
                            }
                            else if (g_Player.pos.y == pBoss->pos.y + BOSS_HEIGHT)
                            {
                                fAdjustment = 0.0f;

                                fDistanceY = 0.0f;
                            }
                            else if (g_Player.pos.y < pBoss->pos.y + BOSS_HEIGHT)
                            {
                                fAdjustment = 1.0f;

                                //Y��
                                fDistanceY = pBoss->pos.y + BOSS_HEIGHT - g_Player.pos.y;
                            }

                            //�����蔻��̔���
                            if (g_Player.nAttackTime == 165)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 150)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 135)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 120)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 105)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 90)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 75)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 60)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 45)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 30)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 15)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                        }
                        

                        //�K�[�h���Ɠ����ʁA������
                        //�X�e�B�b�N���|��Ă���
                        if (Controller.lY != 0 || Controller.lX != 0)
                        {
                            float fAngle = atan2(Controller.lX, Controller.lY*-1);//�X�e�B�b�N�̊p�x�����߂�
                            float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//�X�e�B�b�N�̊p�x�����߂�

                                                                                        //�X�e�B�b�N�ƃJ�����̌������L�^����
                            g_fAngle = pCamera->rot.y + fAngle;

                            g_Player.pos.x += sinf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                            g_Player.pos.z += cosf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                        }

                        //�W�����v
                        if (g_Player.bSkyDash == false && g_Player.bJump == false && GetJoypadTrigger(BUTTON_A))
                        {
                            g_Player.move.y = 0;
                            g_Player.move.y += VALUE_JUMP;
                            g_Player.bJump = true;
                            g_Player.bSecondJump = true;
                        }

                        //���f����������]������
                        g_Player.rot.y -= 0.4f;
                        if (g_Player.rot.y <= -3600.0f)
                        {
                            g_Player.rot.y = 0.0f;
                        }

                        //�`�����X�A�^�b�N������
                        if (g_Player.nAttackTime == 0)
                        {
                            g_Player.bChanceAttack = false;
                        }
                    }

                }

                //�ʏ�U��
                else
                {
                    //�ʏ�U���́A�G�i�W�[�������
                    //�e�̎�
                    if (g_Player.bBullet == true)
                    {
                        g_MotionState = MOTION_BULLET;//���[�V�������e�U���ɂ���
                        g_Player.fEnergy -= ENERGY_BULLET_ATTACK;
                    }

                    //���̎�
                    else
                    {
                        g_MotionState = MOTION_SWORD1;//���[�V���������U���ɂ���
                        g_Player.fEnergy -= ENERGY_SWORD_ATTACK;
                    }

                    
                    //�K�[�h���Ɠ����ʁA�e�������Ă���Ԃ�������
                    if (g_Player.bBullet == true)
                    {
                        //�X�e�B�b�N���|��Ă���
                        if (Controller.lY != 0 || Controller.lX != 0)
                        {
                            float fAngle = atan2(Controller.lX, Controller.lY*-1);//�X�e�B�b�N�̊p�x�����߂�
                            float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//�X�e�B�b�N�̊p�x�����߂�

                                                                                        //�X�e�B�b�N�ƃJ�����̌������L�^����
                            g_fAngle = pCamera->rot.y + fAngle;

                            g_Player.pos.x += sinf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                            g_Player.pos.z += cosf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                            g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
                        }
                    }

                    //����U�肩�����ԁA������
                    else
                    {
                        if (g_Player.nAttackTime > 20)
                        {
                            //�K�[�h���Ɠ����ʁA������
                            //�X�e�B�b�N���|��Ă���
                            if (Controller.lY != 0 || Controller.lX != 0)
                            {
                                float fAngle = atan2(Controller.lX, Controller.lY*-1);//�X�e�B�b�N�̊p�x�����߂�
                                float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//�X�e�B�b�N�̊p�x�����߂�

                                                                                            //�X�e�B�b�N�ƃJ�����̌������L�^����
                                g_fAngle = pCamera->rot.y + fAngle;

                                g_Player.pos.x += sinf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                                g_Player.pos.z += cosf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                                g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
                            }
                        }

                    }

                    //�ʏ탂�[�h
                    if (pCamera->bBossMode == false)
                    {
                        //�G���擾
                        pEnemy = GetEnemy();
                        for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
                        {
                            //��ԋ߂��G�Ɍ���
                            if (pEnemy->nRank == 1)
                            {

                                float fPposx = g_Player.pos.x, fPposz = g_Player.pos.z;	//���@�̍��W
                                float fEposx = pEnemy->pos.x, fEposz = pEnemy->pos.z;	//�G�̍��W
                                float fAngle;											//�p�x

                                float fAdjustment;  //�����̒���

                                float fDistanceY;   //Y���̍�

                                fAngle = atan2f((fPposx - fEposx), (fPposz - fEposz));	//�p�x�����߂�

                                //���b�N�I�������G�̂ق�������
                                g_Player.rot.y = fAngle;


                                //��������
                                if (g_Player.pos.y > pEnemy->pos.y)
                                {
                                    fAdjustment = -1.0f;

                                    //Y��
                                    fDistanceY = g_Player.pos.y - pEnemy->pos.y;
                                }
                                else if (g_Player.pos.y == pEnemy->pos.y)
                                {
                                    fAdjustment = 0.0f;

                                    fDistanceY = 0.0f;
                                }
                                else if (g_Player.pos.y < pEnemy->pos.y)
                                {
                                    fAdjustment = 1.0f;

                                    //Y��
                                    fDistanceY = pEnemy->pos.y - g_Player.pos.y;
                                }

                                //�e�U��
                                if (g_Player.bBullet == true)
                                {
                                    if (g_Player.nAttackTime == 32)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                            D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                                , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                        PlaySound(SOUND_LABEL_SE_SHOT);
                                    }
                                    else if (g_Player.nAttackTime == 28)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                            D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                                , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                        PlaySound(SOUND_LABEL_SE_SHOT);
                                    }
                                    else if (g_Player.nAttackTime == 24)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                            D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                                , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                        PlaySound(SOUND_LABEL_SE_SHOT);
                                    }
                                    else if (g_Player.nAttackTime == 20)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                            D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                                , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                        PlaySound(SOUND_LABEL_SE_SHOT);
                                    }
                                }

                                //���U��
                                else
                                {
                                    if (g_Player.nAttackTime == 18)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(g_Player.rot.y)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(g_Player.rot.y)*SWORD_SPEED), 8, SWORD_RIGID, SWORD_DAMAGE, BULLETTYPE_PLAYER);
                                    }
                                }
                            }
                        }
                    }

                    //�{�X���[�h
                    else
                    {
                        pBoss = GetBoss();
                        float fPposx = g_Player.pos.x, fPposz = g_Player.pos.z;	//���@�̍��W
                        float fBposx = pBoss->pos.x, fBposz = pBoss->pos.z;		//�G�̍��W
                        float fAngle;											//�p�x

                        fAngle = atan2f((fPposx - fBposx), (fPposz - fBposz));	//�p�x�����߂�

                        float fAdjustment;  //�����̒���

                        float fDistanceY;   //Y���̍�

                        //���b�N�I�������G�̂ق�������
                        g_Player.rot.y = fAngle;

                                            //��������
                        if (g_Player.pos.y > pBoss->pos.y + BOSS_HEIGHT)
                        {
                            fAdjustment = -1.0f;

                            //Y��
                            fDistanceY = g_Player.pos.y - pBoss->pos.y + BOSS_HEIGHT;
                        }
                        else if (g_Player.pos.y == pBoss->pos.y + BOSS_HEIGHT)
                        {
                            fAdjustment = 0.0f;

                            fDistanceY = 0.0f;
                        }
                        else if (g_Player.pos.y < pBoss->pos.y + BOSS_HEIGHT)
                        {
                            fAdjustment = 1.0f;

                            //Y��
                            fDistanceY = pBoss->pos.y + BOSS_HEIGHT - g_Player.pos.y;
                        }

                        //�e�U��
                        if (g_Player.bBullet == true)
                        {
                            if (g_Player.nAttackTime == 32)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                    D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                        , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                PlaySound(SOUND_LABEL_SE_SHOT);
                            }
                            else if (g_Player.nAttackTime == 28)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                    D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                        , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                PlaySound(SOUND_LABEL_SE_SHOT);
                            }
                            else if (g_Player.nAttackTime == 24)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                    D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                        , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                PlaySound(SOUND_LABEL_SE_SHOT);
                            }
                            else if (g_Player.nAttackTime == 20)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                    D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                        , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                PlaySound(SOUND_LABEL_SE_SHOT);
                            }
                        }

                        //���U��
                        else
                        {
                            if (g_Player.nAttackTime == 18)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(g_Player.rot.y)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(g_Player.rot.y)*SWORD_SPEED), 8, SWORD_RIGID, SWORD_DAMAGE, BULLETTYPE_PLAYER);
                            }
                        }
                    }
                }
            }

        }

        //�K�E�Z�̃N�[���^�C��
        if (g_Player.nCntEXAttackCD > 0)
        {
            g_Player.nCntEXAttackCD--;
        }

        //�u���b�N�̏�ɂ���Ƃ�
        //�u���b�N�̏�ɂ̂��Ă��Ȃ��Ƃ���else�����邽�߁A���ʉ����Ă��Ȃ�
        if (LandBlock(&g_Player.pos, &g_Player.posOld, D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH)) == true)
        {
            g_Player.move.y = 0;
            g_Player.bJump = false;
            g_Player.bSecondJump = true;
            g_Player.bSkyDash = false;
            g_Player.nCntSkyDash = 0;
            g_Player.bLand = true;

            //�U�����Ă��Ȃ��Ƃ�
            if (g_Player.nAttackTime == 0)
            {

                //�K�[�h�����Ă���
                if (GetJoypadPress(BUTTON_L1) || GetJoypadPress(BUTTON_L2))
                {
                    g_Player.bGuard = true;
                }
                else
                {
                    g_Player.bGuard = false;
                }
            }
        }
        else
        {
            g_Player.bLand = false;
        }

        //�e�̓����蔻��
        CollisionBulletPlayer(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT * 2, PLAYER_WIDTH));

        //�G�Ƃ̓����蔻��
        CollisionEnemy(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT * 2, PLAYER_WIDTH));

        //�{�X�Ƃ̓����蔻��
        CollisionBoss(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT * 2, PLAYER_WIDTH));

        //�A�C�e���Ƃ̓����蔻��
        CollisionItem(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT * 2, PLAYER_WIDTH));

        //�G�t�F�N�g�Ƃ̓����蔻��
        CollisionEffectPlayer(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT * 2, PLAYER_WIDTH));

        //�e������
        //SetPositionShadow(g_ShadowIndex, D3DXVECTOR3(g_Player.pos.x, SHADOW_HEIGHT, g_Player.pos.z));

        //�ړ�����
        //��
        if (g_Player.pos.x < -WALL_SIZE+ PLAYER_WIDTH)
        {
            g_Player.pos.x = -WALL_SIZE + PLAYER_WIDTH;
        }
        //�E
        if (g_Player.pos.x > WALL_SIZE - PLAYER_WIDTH)
        {
            g_Player.pos.x = WALL_SIZE - PLAYER_WIDTH;
        }

        //���
        if (g_Player.pos.z < -WALL_SIZE * 4 + PLAYER_WIDTH)
        {
            g_Player.pos.z = -WALL_SIZE * 4 + PLAYER_WIDTH;
        }
        //��ԉ�
        if (g_Player.pos.z > WALL_SIZE * 4 - PLAYER_WIDTH)
        {
            g_Player.pos.z = WALL_SIZE * 4 - PLAYER_WIDTH;
        }

        //�t�F�[�Y���Ƃ̕�
        WALL*pWall = GetWall();
        for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
        {
            if (pWall->bUse == true)
            {
                switch (pWall->type)
                {
                case WALLTYPE_RED1:
                    if (g_Player.pos.z > -WALL_SIZE * 2 - PLAYER_WIDTH)
                    {
                        g_Player.pos.z = -WALL_SIZE * 2 - PLAYER_WIDTH;
                    }
                    break;

                case WALLTYPE_RED2:
                    if (g_Player.pos.z > 0.0f - PLAYER_WIDTH)
                    {
                        g_Player.pos.z = 0.0f - PLAYER_WIDTH;
                    }
                    break;

                case WALLTYPE_RED3:
                    if (g_Player.pos.z > WALL_SIZE * 2 - PLAYER_WIDTH)
                    {
                        g_Player.pos.z = WALL_SIZE * 2 - PLAYER_WIDTH;
                    }
                    break;
                }
            }
        }

        //�{�X�ɂ��ǂ蒅������A�J�������{�X�Ƀ��b�N�I��
        if (g_bReachBoss == false)
        {
            if (g_Player.pos.z > REACH_BOSS_Z)
            {
                pCamera->state = CAMERASTATE_BOSS;
                g_bReachBoss = true;
            }
        }
            
        //���S����
        if (g_Player.fLife <= 0.0f)
        {
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
                SetEffect2(g_Player.pos, move, EFFECT2_SIZE_EXPLOSION, EFFECT2_INIT_COLOR_EXPLOSION2, EFFECT2TYPE_EXPLOSION); //�����G�t�F�N�g
            }

            g_Player.bDisp = false;    //���@�𖢎g�p��

            PlaySound(SOUND_LABEL_SE_EXPLOSION2);

            SetCampaignState(CAMPAIGNSTATE_GAMEOVER);    //�Q�[���I�[�o�[��ʂ�     
        }

        //���f���̃A�j���[�V����
        UpdatePlayerMotion();

    }
}

//=====================================================
// �v���C���[�`�揈��
//=====================================================
void DrawPlayer(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^
    D3DXMATRIX mtxRot, mtxTrans, mtxParent;
    D3DMATERIAL9 matDef;		// ���݂̃}�e���A���ێ��p
    D3DXMATERIAL* pMat;			// �}�e���A���f�[�^�ւ̃|�C���^

    //�v���C���[�����݂��Ă���Ȃ�`��
    if (g_Player.bDisp == true)
    {
        // ���[���h�}�g���b�N�X�̏�����
        D3DXMatrixIdentity(&g_Player.mtxWorld);

        // �����𔽉f
        D3DXMatrixRotationYawPitchRoll(&mtxRot,
            g_Player.rot.y,
            g_Player.rot.x,
            g_Player.rot.z);
        D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

        // �ʒu�𔽉f
        D3DXMatrixTranslation(&mtxTrans,
            g_Player.pos.x,
            g_Player.pos.y,
            g_Player.pos.z);
        D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

        // �p�[�c�̐ݒ�
        for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
        {
            // ���[���h�}�g���b�N�X�̏�����
            D3DXMatrixIdentity(&g_Player.modelParts[nCntParts].mtxWorld);

            // �����𔽉f
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_aPlayer[nCntParts].rot.y,
                g_aPlayer[nCntParts].rot.x,
                g_aPlayer[nCntParts].rot.z);

            D3DXMatrixMultiply(
                &g_Player.modelParts[nCntParts].mtxWorld,
                &g_Player.modelParts[nCntParts].mtxWorld, &mtxRot);

            // �ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans,
                g_aPlayer[nCntParts].pos.x,
                g_aPlayer[nCntParts].pos.y,
                g_aPlayer[nCntParts].pos.z);
            D3DXMatrixMultiply(
                &g_Player.modelParts[nCntParts].mtxWorld,
                &g_Player.modelParts[nCntParts].mtxWorld, &mtxTrans);

            // �e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, NULL);

            // ���݂̃}�e���A�����擾����
            pDevice->GetMaterial(&matDef);

            // �}�e���A���f�[�^�ւ̃|�C���^���擾
            pMat = (D3DXMATERIAL*)g_Player.modelParts[nCntParts].pBuffMat->GetBufferPointer();

            // �e���f���̃C���f�b�N�X�� - 1�̎�
            if (g_Player.modelParts[nCntParts].nldxModelParent == -1)
            {
                mtxParent = g_Player.mtxWorld;		// �v���C���[�̃}�g���N�X
            }
            else
            {
                mtxParent = g_Player.modelParts[g_Player.modelParts[nCntParts].nldxModelParent].mtxWorld;
            }
            // �e���f���̃}�g���b�N�X�̔���
            D3DXMatrixMultiply(
                &g_Player.modelParts[nCntParts].mtxWorld,
                &g_Player.modelParts[nCntParts].mtxWorld, &mtxParent);

            // ���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_Player.modelParts[nCntParts].mtxWorld);

            for (int nCntMat = 0; nCntMat < (int)g_Player.modelParts[nCntParts].nNumMat; nCntMat++)
            {
                // �}�e���A���̐ݒ�
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                
                if (nCntParts == 10)
                {
                    // ���f���p�[�c
                    if (g_Player.bBullet == true)
                    {
                        g_Player.modelParts[nCntParts].pMesh->DrawSubset(nCntMat);
                    }
                }
                
                else if (nCntParts == 11)
                {
                    // ���f���p�[�c
                    if (g_Player.bBullet == false)
                    {
                        g_Player.modelParts[nCntParts].pMesh->DrawSubset(nCntMat);
                    }
                }

                else
                {
                    g_Player.modelParts[nCntParts].pMesh->DrawSubset(nCntMat);
                }
            }

            // �ێ����Ă����}�e���A����߂�
            pDevice->SetMaterial(&matDef);
        }
    }
}

//=============================================================================
// �v���C���[�̎擾
//=============================================================================
PLAYER*GetPlayer(void)
{
    return &g_Player;
}

//=============================================================================
// �v���C���[�̃_���[�W�ݒ�
//=============================================================================
void HitPlayer(int nCntRigid, float fDamage)
{
    //PlaySound(SOUND_LABEL_SE_EXPLOSION);	//������

    //�_���[�W��ԂɑJ��
    g_Player.state = PLAYERSTATE_DAMAGE;
    g_Player.nCounterState = nCntRigid; //�q�b�g�d��

    //�_���[�W�v�Z
    //�K�[�h�����Ă���
    if (g_Player.bGuard == true)
    {
        g_Player.fLife -= fDamage/2;
    }
    //�K�[�h���Ă��Ȃ�
    else
    {
        g_Player.fLife -= fDamage;
    }
    
    //�K�E�Q�[�W�㏸
    g_Player.fChanceMeter += TAKE_DAMAGE_CHARGE;
}

//=============================================================================
// �A�j���[�V�������[�h
//=============================================================================
void LoadAnimation(void)
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
    pFile = fopen("motion.txt", "r");

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
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPMotion[nCntMotion].nLoop);
                        }

                        if (strcmp(cHeadText, "NUM_KEY") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPMotion[nCntMotion].nNum_Key);
                        }

                        //			if (strcmp(cHeadText, "COLLISION") == 0)
                        //			{
                        //				sscanf(cReedText, "%s %s %d %f %f %f %f %d %d", &cDie, &cDie, &g_PlayerCollision.nPartsInfo, &g_PlayerCollision.CollisionPos.x, &g_PlayerCollision.CollisionPos.y, &g_PlayerCollision.CollisionPos.z, &g_PlayerCollision.fLength, &g_PlayerCollision.nFirstFlame[nCntMotion], &g_PlayerCollision.nLastFlame);
                        //			}
                        if (strcmp(cHeadText, "KEYSET") == 0)
                        {

                            while (strcmp(cHeadText, "END_KEYSET") != 0)
                            {

                                fgets(cReedText, sizeof(cReedText), pFile);
                                sscanf(cReedText, "%s", &cHeadText);

                                if (strcmp(cHeadText, "FRAME") == 0)
                                {
                                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPMotion[nCntMotion].KeyInfo[nCntKey].nFrame);
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
                                                &g_aPMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].x,
                                                &g_aPMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].y,
                                                &g_aPMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].z);
                                        }

                                        if (strcmp(cHeadText, "ROT") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &g_aPMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].x,
                                                &g_aPMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].y,
                                                &g_aPMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].z);
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
void LoadPlayer(void)
{
    // �t�@�C���|�C���g
    FILE *pFile;

    // �ϐ��錾
    int  nCntLoad = 0;
    char cReedText[128];	// �����Ƃ��ēǂݎ��p
    char cHeadText[256];	//
    char cDie[128];

    // �t�@�C���J
    pFile = fopen("player.txt", "r");

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
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer[nCntLoad].nData);
                        }

                        if (strcmp(cHeadText, "PARENT") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer[nCntLoad].nParents);
                        }

                        if (strcmp(cHeadText, "POS") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                &g_aPlayer[nCntLoad].posOrigin.x,
                                &g_aPlayer[nCntLoad].posOrigin.y,
                                &g_aPlayer[nCntLoad].posOrigin.z);

                            g_aPlayer[nCntLoad].pos = g_aPlayer[nCntLoad].posOrigin;
                        }

                        if (strcmp(cHeadText, "ROT") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                &g_aPlayer[nCntLoad].rot.x,
                                &g_aPlayer[nCntLoad].rot.y,
                                &g_aPlayer[nCntLoad].rot.z);
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
void UpdatePlayerMotion(void)
{
    //���[�V�������ς�����Ƃ�
    if (g_MotionOldState != g_MotionState) {
        g_nFreme = 0;
        g_nCntKey = 0;
    }
    for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
    {
        if (g_nFreme == 0)
        {
            g_NumPos[nCntMotion].x = ((g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].pos[nCntMotion].x -
                (g_aPlayer[nCntMotion].pos.x - g_aPlayer[nCntMotion].posOrigin.x)) / float(g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].nFrame));

            g_NumPos[nCntMotion].y = ((g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].pos[nCntMotion].y -
                (g_aPlayer[nCntMotion].pos.y - g_aPlayer[nCntMotion].posOrigin.y)) / float(g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].nFrame));

            g_NumPos[nCntMotion].z = ((g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].pos[nCntMotion].z -
                (g_aPlayer[nCntMotion].pos.z - g_aPlayer[nCntMotion].posOrigin.z)) / float(g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].nFrame));

            g_NumRotDest[nCntMotion] =
                g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].rot[nCntMotion] - g_aPlayer[nCntMotion].rot;

            g_NumRot[nCntMotion] = (g_NumRotDest[nCntMotion] / float(g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].nFrame));
        }

        g_aPlayer[nCntMotion].pos += g_NumPos[nCntMotion];
        g_aPlayer[nCntMotion].rot += g_NumRot[nCntMotion];
    }
    g_MotionOldState = g_MotionState;
    // ���[�v����Ƃ�
    if (g_nFreme >= g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].nFrame && g_aPMotion[g_MotionState].nLoop == 1)
    {
        g_nFreme = 0;
        g_nCntKey++;

        if (g_nCntKey >= g_aPMotion[g_MotionState].nNum_Key)
        {
            g_nCntKey = 0;
        }

    }
    // ���[�v���Ȃ��Ƃ�
    else if (g_nFreme >= g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].nFrame && g_aPMotion[g_MotionState].nLoop == 0)
    {
        g_nFreme = 0;
        g_nCntKey++;
        if (g_nCntKey >= g_aPMotion[g_MotionState].nNum_Key)
        {
            for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
            {
                g_NumPos[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_NumRot[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
            g_MotionState = MOTION_IDLE;
            g_nCntKey = 0;
        }
    }
    else
    {
        g_nFreme++;
    }
}