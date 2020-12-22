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
#include "shadow.h"
#include "enemy.h"
#include "bullet.h"
#include "score.h"
#include "campaign.h"
#include "effect.h"
#include "effect2.h"
#include "collision.h"
#include "wall.h"
#include "block.h"
#include "meshfield.h"
#include "item.h"
#include "enemylife.h"
#include "sound.h"

//=====================================================
// �O���[�o���ϐ���`
//=====================================================
LPD3DXMESH g_pMeshEnemy[ENEMYTYPE_MAX] = {};		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatEnemy[ENEMYTYPE_MAX] = {};// �}�e���A�����ւ̃|�C���^
DWORD g_nNumMatEnemy[ENEMYTYPE_MAX] = {};			// �}�e���A�����̐�

ENEMY g_Enemy[MAX_ENEMY];	        //�G�l�~�[�̏��
int g_ShadowIndexEnemy;
int g_RandomAttack = 1;				//�e�������_���ɑłƂ��Ɏg�p

int g_nCntPhase1;                   //��1�t�F�[�Y
int g_nCntPhase2;                   //��2�t�F�[�Y
int g_nCntPhase3;                   //��3�t�F�[�Y
int g_nCntPhase4;                   //��4�t�F�[�Y

//=====================================================
// ���f������������
//=====================================================
void InitEnemy(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    // X�t�@�C���̓ǂݍ���
    //�L���m��
    D3DXLoadMeshFromX("MODEL/enemy000.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_000],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_000],
        &g_pMeshEnemy[ENEMYTYPE_000]);

    //�X�p�C�_�[
    D3DXLoadMeshFromX("MODEL/enemy001.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_001],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_001],
        &g_pMeshEnemy[ENEMYTYPE_001]);

    //�o�^�t���C
    D3DXLoadMeshFromX("MODEL/enemy002.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_002],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_002],
        &g_pMeshEnemy[ENEMYTYPE_002]);

    //�S�[���f���X�p�C�_�[
    D3DXLoadMeshFromX("MODEL/enemy003.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_003],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_003],
        &g_pMeshEnemy[ENEMYTYPE_003]);

    //�W�����v�{�b�N�X
    D3DXLoadMeshFromX("MODEL/enemy004.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_004],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_004],
        &g_pMeshEnemy[ENEMYTYPE_004]);

    //�t�B�b�V��
    D3DXLoadMeshFromX("MODEL/enemy005.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_005],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_005],
        &g_pMeshEnemy[ENEMYTYPE_005]);

    //�O���[�o���ϐ��̏�����
    g_nCntPhase1 = 1;   //��1�t�F�[�Y�̃L�[�́A�L���m��
    g_nCntPhase2 = 1;   //��2�t�F�[�Y�̃L�[�́A�o�^�t���C
    g_nCntPhase3 = 2;   //��3�t�F�[�Y�̃L�[�́A�t�B�b�V���~2

    //�G�̏��̏�����
    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        g_Enemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].posMemory = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].rotMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
        g_Enemy[nCntEnemy].type = ENEMYTYPE_000;
        g_Enemy[nCntEnemy].nCounterState = 0;
        g_Enemy[nCntEnemy].nPosX = 0;
        g_Enemy[nCntEnemy].nPosZ = 0;
        g_Enemy[nCntEnemy].nCounterAnim = 0;
        g_Enemy[nCntEnemy].nPatternAnim = 0;
        g_Enemy[nCntEnemy].bUse = false;
        g_Enemy[nCntEnemy].fLife = 0;
        g_Enemy[nCntEnemy].nAttackTime = 0;
        g_Enemy[nCntEnemy].fDistance = 9999.9f;
        g_Enemy[nCntEnemy].nRank = 0;
        g_Enemy[nCntEnemy].bFollow = false;
    }

    //SetShadow(g_aEnemy.pos/* + ENEMYSHADOW_POSITION*/, g_aEnemy.rot);
    //g_ShadowIndex = SetShadow(g_aEnemy.pos, g_aEnemy.rot);
}

//=====================================================
// �G�l�~�[�I������
//=====================================================
void UninitEnemy(void)
{
    // ���b�V���̔j��
    for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
    {
        if (g_pMeshEnemy[nCntEnemy] != NULL)
        {
            g_pMeshEnemy[nCntEnemy]->Release();
            g_pMeshEnemy[nCntEnemy] = NULL;
        }
        // �}�e���A���̔j��
        if (g_pBuffMatEnemy[nCntEnemy] != NULL)
        {
            g_pBuffMatEnemy[nCntEnemy]->Release();
            g_pBuffMatEnemy[nCntEnemy] = NULL;
        }
    }
}

//=====================================================
// �G�l�~�[�X�V����
//=====================================================
void UpdateEnemy(void)
{
    PLAYER*pPlayer;

    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        //�����N������
        g_Enemy[nCntEnemy].nRank = 0;

        //�g�p���Ȃ�
        if (g_Enemy[nCntEnemy].bUse == true)
        {
            //1F�O�̈ʒu���L��
            g_Enemy[nCntEnemy].posOld = g_Enemy[nCntEnemy].pos;

            //���@���擾����
            pPlayer = GetPlayer();

            //�U����ړ��Ɋւ���ϐ�
            float fPposx = pPlayer->pos.x, fPposz = pPlayer->pos.z;	//���@�̍��W
            float fEposx = g_Enemy[nCntEnemy].pos.x, fEposz = g_Enemy[nCntEnemy].pos.z;		//�G�̍��W
            float fAngle = atan2f((fEposx - fPposx), (fEposz - fPposz));	//�p�x�����߂�											//�p�x
            float fAngle2 = atan2f((fPposx - fEposx), (fPposz - fEposz));	//�p�x�����߂�
            float fAngle3 = atan2f((fEposx - g_Enemy[nCntEnemy].posMemory.x), (fEposz - g_Enemy[nCntEnemy].posMemory.z));	//�p�x�����߂�
            float fAdjustment = 0.0f;  //��������
            float fDistanceY = 0.0f;   //���@�ƓG�̍����̍�

            //��������
            if (pPlayer->pos.y < g_Enemy[nCntEnemy].pos.y)
            {
                fAdjustment = -1.0f;
                fDistanceY = g_Enemy[nCntEnemy].pos.y - pPlayer->pos.y;
            }
            else if (pPlayer->pos.y == g_Enemy[nCntEnemy].pos.y)
            {
                fAdjustment = 0.0f;
                fDistanceY = 0.0f;
            }
            else if (pPlayer->pos.y > g_Enemy[nCntEnemy].pos.y)
            {
                fAdjustment = 1.0f;
                fDistanceY = pPlayer->pos.y - g_Enemy[nCntEnemy].pos.y;
            }

            //���@�ƓG�̋���
            g_Enemy[nCntEnemy].fDistance = sqrtf(
                powf((pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x), 2) +
                powf((pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z), 2));

            //�G�t�F�N�g�̂��߂̕ϐ�
            float fAngleEffect = 0.0f;    //�p�x
            float fRadius = g_Enemy[nCntEnemy].size.x; //�G�t�F�N�g���a
            float fSpeedX = 0.0f;   //����X
            float fSpeedY = 0.0f;   //����Y
            float fSpeedZ = 0.0f;   //����Z
            int nPlusMinusX = 0;
            int nPlusMinusY = 0;
            int nPlusMinusZ = 0;

            //�e�G�l�~�[
            switch (g_Enemy[nCntEnemy].type)
            {
                //========================
                //�L���m��
                //========================
            case ENEMYTYPE_000:

                //��Ԃɂ��ꍇ����
                switch (g_Enemy[nCntEnemy].state)
                {
                    //�ʏ펞
                case ENEMYSTATE_NORMAL:

                    //�ʒu���ړ��ʂƕR�Â�
                    g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

                    //����ɓ�������s��
                    if (g_Enemy[nCntEnemy].fDistance < 1000.0f)
                    {
                        //�N�[���^�C�����I����Ă���U���\
                        if (g_Enemy[nCntEnemy].nAttackTime == 0)
                        {
                            //�e�̔���
                            if ((rand() % (g_RandomAttack * 90)) == 0)
                            {
                                SetBullet(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + ENEMY_HEIGHT_CANNON / 2
                                    , g_Enemy[nCntEnemy].pos.z), BULLET_SIZE_ENEMY,
                                    D3DXVECTOR3(-sinf(g_Enemy[nCntEnemy].rot.y)*BULLET_SPEED_ENEMY, 0.0f
                                        , -cosf(g_Enemy[nCntEnemy].rot.y)*BULLET_SPEED_ENEMY), 40, BULLET_RIGID, BULLET_DAMAGE_ENEMY, BULLETTYPE_ENEMY);

                                g_Enemy[nCntEnemy].nAttackTime = 20;
                            }
                        }
                    }

                    break;

                    //�_���[�W��
                case ENEMYSTATE_DAMAGE:
                    g_Enemy[nCntEnemy].nCounterState--;

                    //�ł��グ��
                    g_Enemy[nCntEnemy].pos.y += g_Enemy[nCntEnemy].move.y;

                    if (g_Enemy[nCntEnemy].nCounterState <= 0)
                    {
                        g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                //���@�̂ق������X�Ɍ���悤�ɂ���
                g_Enemy[nCntEnemy].rotDest.y = fAngle;

                // ��]�̐���
                if (g_Enemy[nCntEnemy].rot.y > D3DX_PI)
                {
                    g_Enemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                }
                if (g_Enemy[nCntEnemy].rot.y < -D3DX_PI)
                {
                    g_Enemy[nCntEnemy].rot.y += D3DX_PI * 2;
                }

                g_Enemy[nCntEnemy].rotMin.y = g_Enemy[nCntEnemy].rotDest.y - g_Enemy[nCntEnemy].rot.y;
                
                // �ړI�̊p�x�̉�]�𐧌�
                if (g_Enemy[nCntEnemy].rotMin.y > D3DX_PI)
                {
                    g_Enemy[nCntEnemy].rotDest.y -= D3DX_PI * 2;
                }
                if (g_Enemy[nCntEnemy].rotMin.y < -D3DX_PI)
                {
                    g_Enemy[nCntEnemy].rotDest.y += D3DX_PI * 2;
                }

                // ��]�̐���
                if (g_Enemy[nCntEnemy].rot.y > D3DX_PI)
                {
                    g_Enemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                }
                if (g_Enemy[nCntEnemy].rot.y < -D3DX_PI)
                {
                    g_Enemy[nCntEnemy].rot.y += D3DX_PI * 2;
                }

                if (fabsf(g_Enemy[nCntEnemy].rotMin.y) < 0)
                {
                    g_Enemy[nCntEnemy].rotMin.y = g_Enemy[nCntEnemy].rotDest.y;
                }
                else
                {
                    g_Enemy[nCntEnemy].rot += (g_Enemy[nCntEnemy].rotDest - g_Enemy[nCntEnemy].rot) * NORMAL_ROT_SPEED;
                }

                break;
                //========================
                //�X�p�C�_�[
                //========================
            case ENEMYTYPE_001:

                //��Ԃɂ��ꍇ����
                switch (g_Enemy[nCntEnemy].state)
                {
                    //�ʏ펞
                case ENEMYSTATE_NORMAL:

                    //�ʒu���ړ��ʂƕR�Â�
                    g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

                    //�X�p�C�_�[�̈ړ�
                    g_Enemy[nCntEnemy].nPosX += 1;

                    //�X�p�C�_�[�̈ړ��͈�
                    if (g_Enemy[nCntEnemy].nPosX > 50)
                    {
                        g_Enemy[nCntEnemy].move.x *= -1;
                        g_Enemy[nCntEnemy].nPosX *= -1;
                    }

                    break;

                    //�_���[�W��
                case ENEMYSTATE_DAMAGE:
                    g_Enemy[nCntEnemy].nCounterState--;

                    //�ł��グ��
                    g_Enemy[nCntEnemy].pos.y += g_Enemy[nCntEnemy].move.y;

                    if (g_Enemy[nCntEnemy].nCounterState <= 0)
                    {
                        g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                //�ړ��̌����ɂ��A�����]��
                if (g_Enemy[nCntEnemy].move.x < 0)
                {
                    g_Enemy[nCntEnemy].rot.y = 900; //�E����
                }
                else if (g_Enemy[nCntEnemy].move.x > 0)
                {
                    g_Enemy[nCntEnemy].rot.y = 2700;//������
                }

                break;
                //========================
                //�o�^�t���C
                //========================
            case ENEMYTYPE_002:

                //��Ԃɂ��ꍇ����
                switch (g_Enemy[nCntEnemy].state)
                {
                    //�ʏ펞
                case ENEMYSTATE_NORMAL:

                    //�ʒu���ړ��ʂƕR�Â�
                    g_Enemy[nCntEnemy].pos.x += g_Enemy[nCntEnemy].move.x;
                    g_Enemy[nCntEnemy].pos.z += g_Enemy[nCntEnemy].move.z;

                    //�o�^�t���C�̈ړ�
                    g_Enemy[nCntEnemy].nPosX += 1;
                    g_Enemy[nCntEnemy].nPosZ += 1;

                    //�o�^�t���C�̈ړ��͈�
                    //��
                    if (g_Enemy[nCntEnemy].nPosX > 60)
                    {
                        g_Enemy[nCntEnemy].move.x *= -1;
                        g_Enemy[nCntEnemy].nPosX *= -1;
                    }
                    //�c
                    if (g_Enemy[nCntEnemy].nPosZ > 80)
                    {
                        g_Enemy[nCntEnemy].move.z *= -1;
                        g_Enemy[nCntEnemy].nPosZ *= -1;
                    }

                    //�N�[���^�C�����I����Ă���U���\
                    if (g_Enemy[nCntEnemy].nAttackTime == 0)
                    {
                        //�e�̔���
                        if ((rand() % (g_RandomAttack * 90)) == 0)
                        {
                            SetBullet(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + ENEMY_HEIGHT_BUTTERFLY / 2, g_Enemy[nCntEnemy].pos.z), BULLET_SIZE_ENEMY,
                                D3DXVECTOR3(-sinf(fAngle)*BULLET_SPEED_ENEMY, fDistanceY / BULLET_SPEED_ENEMY * fAdjustment
                                    , -cosf(fAngle)*BULLET_SPEED_ENEMY), 40,BULLET_RIGID, BULLET_DAMAGE_ENEMY, BULLETTYPE_ENEMY);

                            g_Enemy[nCntEnemy].nAttackTime = 20;
                        }
                    }

                    break;

                    //�_���[�W��
                case ENEMYSTATE_DAMAGE:
                    g_Enemy[nCntEnemy].nCounterState--;

                    if (g_Enemy[nCntEnemy].nCounterState <= 0)
                    {
                        g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                //���@�̂ق�������悤�ɂ���
                g_Enemy[nCntEnemy].rot.y = fAngle;

                break;

                //========================
                //�S�[���f���X�p�C�_�[
                //========================
            case ENEMYTYPE_003:

                //��Ԃɂ��ꍇ����
                switch (g_Enemy[nCntEnemy].state)
                {
                    //�ʏ펞
                case ENEMYSTATE_NORMAL:

                    //�ʒu���ړ��ʂƕR�Â�
                    g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

                    //����ɓ������瓦���o��
                    if (g_Enemy[nCntEnemy].fDistance < 500.0f)
                    {
                        g_Enemy[nCntEnemy].move.x = sinf(fAngle)*6.0f;
                        g_Enemy[nCntEnemy].move.z = cosf(fAngle)*6.0f;
                        //���@�Ƌt������悤�ɂ���
                        g_Enemy[nCntEnemy].rot.y = fAngle2;
                    }
                    else
                    {
                        //��x�����o������A������悤�ɂ���
                        if (g_Enemy[nCntEnemy].move.x == 0)
                        {
                            //���@�̂ق�������悤�ɂ���
                            g_Enemy[nCntEnemy].rot.y = fAngle;
                        }
                        else
                        {
                            //���@�Ƌt������悤�ɂ���
                            g_Enemy[nCntEnemy].rot.y = fAngle2;
                        }
                    }

                    break;

                    //�_���[�W��
                case ENEMYSTATE_DAMAGE:
                    g_Enemy[nCntEnemy].nCounterState--;

                    //�ł��グ��
                    g_Enemy[nCntEnemy].pos.y += g_Enemy[nCntEnemy].move.y;

                    if (g_Enemy[nCntEnemy].nCounterState <= 0)
                    {
                        g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                //�u���b�N�ɑ΂��铖���蔻��
                EnemyHitBlock(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld,
                g_Enemy[nCntEnemy].size);

                //���b�V���t�B�[���h�͈̔͊O�ɍs������A����
                //��
                if (g_Enemy[nCntEnemy].pos.x < -FIELD_WIDTH_SIZE + g_Enemy[nCntEnemy].size.x)
                {
                    g_Enemy[nCntEnemy].bUse = false;
                }
                //�E
                if (g_Enemy[nCntEnemy].pos.x > FIELD_WIDTH_SIZE - g_Enemy[nCntEnemy].size.x)
                {
                    g_Enemy[nCntEnemy].bUse = false;
                }
                //���
                if (g_Enemy[nCntEnemy].pos.z < -FIELD_WIDTH_SIZE + g_Enemy[nCntEnemy].size.z)
                {
                    g_Enemy[nCntEnemy].bUse = false;
                }
                //�O
                if (g_Enemy[nCntEnemy].pos.z > FIELD_WIDTH_SIZE + g_Enemy[nCntEnemy].size.z)
                {
                    g_Enemy[nCntEnemy].bUse = false;
                }

                ////�L���L��
                //for (int nCnt = 0; nCnt < 1; nCnt++)
                //{
                //    fSpeedX = float(rand() % 200 + 100) / 100.0f;
                //    fSpeedY = float(rand() % 250 + 50) / 100.0f;
                //    fSpeedZ = float(rand() % 200 + 100) / 100.0f;
                //    nPlusMinusX = int(rand() % 2 + 1);
                //    nPlusMinusY = int(rand() % 2 + 1);
                //    nPlusMinusZ = int(rand() % 2 + 1);
                //    if (nPlusMinusX == 1)
                //    {
                //        nPlusMinusX = 1;
                //    }
                //    else
                //    {
                //        nPlusMinusX = -1;
                //    }

                //    if (nPlusMinusY == 1)
                //    {
                //        nPlusMinusY = 1;
                //    }
                //    else
                //    {
                //        nPlusMinusY = 1;
                //    }

                //    if (nPlusMinusZ == 1)
                //    {
                //        nPlusMinusZ = 1;
                //    }
                //    else
                //    {
                //        nPlusMinusZ = -1;
                //    }

                //    //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                //    fAngleEffect = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                //    D3DXVECTOR3 pos = D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x - sinf(fAngleEffect)*fRadius, g_Enemy[nCntEnemy].pos.y,
                //        g_Enemy[nCntEnemy].pos.z - cosf(fAngleEffect)*fRadius);

                //    D3DXVECTOR3 move = D3DXVECTOR3(/*fSpeedX*nPlusMinusX*/0.0f, fSpeedY*nPlusMinusY, /*fSpeedZ*nPlusMinusZ*/0.0f);
                //    SetEffect2(pos, move, EFFECT2_SIZE_HEAL, EFFECT2_INIT_COLOR_EXPLOSION, EFFECT2TYPE_RARE);
                //}

                break;
                //========================
                //�W�����v�{�b�N�X
                //========================
            case ENEMYTYPE_004:

                //��Ԃɂ��ꍇ����
                switch (g_Enemy[nCntEnemy].state)
                {
                    //�ʏ펞
                case ENEMYSTATE_NORMAL:

                    //�ʒu���ړ��ʂƕR�Â�
                    g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

                    //�W�����v
                    if (g_Enemy[nCntEnemy].pos.y == 0)
                    {
                        g_Enemy[nCntEnemy].move.y += VALUE_JUMP;
                    }

                    //�W�����v�{�b�N�X�̈ړ�
                    g_Enemy[nCntEnemy].nPosX += 1;

                    //�W�����v�{�b�N�X�̈ړ��͈�
                    if (g_Enemy[nCntEnemy].nPosX > 160)
                    {
                        g_Enemy[nCntEnemy].move.x *= -1;
                        g_Enemy[nCntEnemy].nPosX *= -1;
                    }

                    break;

                    //�_���[�W��
                case ENEMYSTATE_DAMAGE:
                    g_Enemy[nCntEnemy].nCounterState--;

                    //�ł��グ��
                    g_Enemy[nCntEnemy].pos.y += g_Enemy[nCntEnemy].move.y;

                    if (g_Enemy[nCntEnemy].nCounterState <= 0)
                    {
                        g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                //�ړ��̌����ɂ��A�����]��
                if (g_Enemy[nCntEnemy].move.x < 0)
                {
                    g_Enemy[nCntEnemy].rot.y = 900; //�E����
                }
                else if (g_Enemy[nCntEnemy].move.x > 0)
                {
                    g_Enemy[nCntEnemy].rot.y = 2700;//������
                }

                break;
                //========================
                //�t�B�b�V��
                //========================
            case ENEMYTYPE_005:

                //��Ԃɂ��ꍇ����
                switch (g_Enemy[nCntEnemy].state)
                {
                    //�ʏ펞
                case ENEMYSTATE_NORMAL:

                    //�ʒu���ړ��ʂƕR�Â�
                    g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

                    //����ɓ�������ǂ�������
                    if (g_Enemy[nCntEnemy].fDistance < 700.0f)
                    {
                        g_Enemy[nCntEnemy].bFollow = true;
                    }
                    //���삩��o����
                    else
                    {
                        ////���̏ꏊ�ɋA��
                        //if (sqrtf(
                        //    powf((g_Enemy[nCntEnemy].pos.x - g_Enemy[nCntEnemy].posMemory.x), 2) +
                        //    powf((pPlayer->pos.z - g_Enemy[nCntEnemy].posMemory.z), 2))>50.0f)
                        //{
                        //    if (g_Enemy[nCntEnemy].move.x != 0.0f)
                        //    {
                        //        g_Enemy[nCntEnemy].move.x = -sinf(fAngle3)*5.0f;
                        //        g_Enemy[nCntEnemy].move.z = -cosf(fAngle3)*5.0f;
                        //        g_Enemy[nCntEnemy].rot.y = fAngle3;
                        //    }
                        //}
                        //else
                        //{
                            g_Enemy[nCntEnemy].bFollow = false;
                            //g_Enemy[nCntEnemy].rot.y = 0;
                        /*}*/
                    }

                    //����O����U�����󂯂Ă��A�����o��
                    if (g_Enemy[nCntEnemy].fLife != ENEMY_LIFE_FISH)
                    {
                        g_Enemy[nCntEnemy].bFollow = true;
                    }

                    //�Ǐ]�̈ړ���
                    if (g_Enemy[nCntEnemy].bFollow == true && pPlayer->bDisp == true)   //�v���C���[���j��̂��邮����Ȃ���
                    {
                        //�v���C���[��Y���������ƁA�Ǐ]�����񂾂�x��������
                        if (pPlayer->pos.y > 0)
                        {
                            g_Enemy[nCntEnemy].move.x *= 0.99f;
                            g_Enemy[nCntEnemy].move.z *= 0.99f;
                        }
                        else
                        {
                            g_Enemy[nCntEnemy].move.x = -sinf(fAngle)*5.0f;
                            g_Enemy[nCntEnemy].move.z = -cosf(fAngle)*5.0f;
                        }
                    }
                    //�Ǐ]���Ă��Ȃ��Ƃ��̈ړ���
                    else
                    {
                        g_Enemy[nCntEnemy].move.x = 0.0f;
                        g_Enemy[nCntEnemy].move.z = 0.0f;
                    }

                    //�v���C���[���������Ă���Ȃ�A
                    if (pPlayer->bDisp == true)
                    {
                        //���@������悤�ɂ���
                        g_Enemy[nCntEnemy].rot.y = fAngle;
                    }

                    break;

                    //�_���[�W��
                case ENEMYSTATE_DAMAGE:
                    g_Enemy[nCntEnemy].nCounterState--;

                    //�ł��グ�p
                    g_Enemy[nCntEnemy].pos.y += g_Enemy[nCntEnemy].move.y;

                    //�Ǐ]��݂点��
                    g_Enemy[nCntEnemy].pos.x += g_Enemy[nCntEnemy].move.x * 0.5f;
                    g_Enemy[nCntEnemy].pos.z += g_Enemy[nCntEnemy].move.z * 0.5f;

                    if (g_Enemy[nCntEnemy].nCounterState <= 0)
                    {
                        g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                //�u���b�N�ɑ΂��铖���蔻��
                EnemyHitBlock(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld,
                    g_Enemy[nCntEnemy].size);

                //����
                //for (int nCnt = 0; nCnt < 1; nCnt++)
                //{
                //    fSpeedX = float(rand() % 200 + 100) / 100.0f;
                //    fSpeedY = float(rand() % 250 + 50) / 100.0f;
                //    fSpeedZ = float(rand() % 200 + 100) / 100.0f;
                //    nPlusMinusX = int(rand() % 2 + 1);
                //    nPlusMinusY = int(rand() % 2 + 1);
                //    nPlusMinusZ = int(rand() % 2 + 1);
                //    if (nPlusMinusX == 1)
                //    {
                //        nPlusMinusX = 1;
                //    }
                //    else
                //    {
                //        nPlusMinusX = -1;
                //    }

                //    if (nPlusMinusY == 1)
                //    {
                //        nPlusMinusY = 1;
                //    }
                //    else
                //    {
                //        nPlusMinusY = 1;
                //    }

                //    if (nPlusMinusZ == 1)
                //    {
                //        nPlusMinusZ = 1;
                //    }
                //    else
                //    {
                //        nPlusMinusZ = -1;
                //    }

                //    //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                //    fAngleEffect = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                //    D3DXVECTOR3 pos = D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x - sinf(fAngleEffect)*fRadius, g_Enemy[nCntEnemy].pos.y,
                //        g_Enemy[nCntEnemy].pos.z - cosf(fAngleEffect)*fRadius);

                //    D3DXVECTOR3 move = D3DXVECTOR3(/*fSpeedX*nPlusMinusX*/0.0f, fSpeedY*nPlusMinusY, /*fSpeedZ*nPlusMinusZ*/0.0f);
                //    SetEffect2(pos, move, EFFECT2_SIZE_HEAL, EFFECT2_INIT_COLOR_EXPLOSION, EFFECT2TYPE_RARE);
                //}
                
                break;
            }

            //�U�����̃J�E���^�[
            if (g_Enemy[nCntEnemy].nAttackTime > 0)
            {
                g_Enemy[nCntEnemy].nAttackTime--;
            }

            //�d�͂ƒn�ʂ̐ݒ�
            g_Enemy[nCntEnemy].move.y -= GRAVITY;
            if (g_Enemy[nCntEnemy].pos.y < 0)
            {
                g_Enemy[nCntEnemy].pos.y = 0;
                g_Enemy[nCntEnemy].move.y = 0;
            }

            //�e�̓����蔻��
            CollisionBulletEnemy(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld,
                D3DXVECTOR3(g_Enemy[nCntEnemy].size.x, g_Enemy[nCntEnemy].size.y * 2, g_Enemy[nCntEnemy].size.z) , nCntEnemy);

            //�G�t�F�N�g�̓����蔻��
            CollisionEffectEnemy(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld,
                D3DXVECTOR3(g_Enemy[nCntEnemy].size.x, g_Enemy[nCntEnemy].size.y * 2, g_Enemy[nCntEnemy].size.z), nCntEnemy);

            //�e������
            //SetPositionShadow(g_ShadowIndex, D3DXVECTOR3(g_aEnemy.pos.x, SHADOW_HEIGHT, g_aEnemy.pos.z));

            //���S����
            if (g_Enemy[nCntEnemy].fLife <= 0.0f)
            {

                //�����G�t�F�N�g
                float fSpeedX = 0.0f;   //����X
                float fSpeedY = 0.0f;   //����Y
                float fSpeedZ = 0.0f;   //����Z
                int nPlusMinusX = 0;
                int nPlusMinusZ = 0;
                for (int nCnt = 0; nCnt < 200; nCnt++)
                {
                    fSpeedX = float(rand() % 800 + 300) / 100.0f;
                    fSpeedY = float(rand() % 800 + 300) / 100.0f;
                    fSpeedZ = float(rand() % 800 + 300) / 100.0f;
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
                    SetEffect2(g_Enemy[nCntEnemy].pos, move, EFFECT2_SIZE_EXPLOSION, EFFECT2_INIT_COLOR_EXPLOSION, EFFECT2TYPE_EXPLOSION); //�����G�t�F�N�g
                }

                //�G�̎�ނɂ���ē���X�R�A��ς���
                switch (g_Enemy[nCntEnemy].type)
                {
                    //�L���m��
                case ENEMYTYPE_000:
                    ScoreCalculation(2000);
                    g_nCntPhase1--;
                    break;

                    //�X�p�C�_�[
                case ENEMYTYPE_001:
                    ScoreCalculation(1000);
                    break;

                    //�o�^�t���C
                case ENEMYTYPE_002:
                    ScoreCalculation(3000);
                    g_nCntPhase2--;
                    break;

                    //�S�[���f���X�p�C�_�[
                case ENEMYTYPE_003:
                    ScoreCalculation(10000);
                    break;

                    //�W�����v�{�b�N�X
                case ENEMYTYPE_004:
                    ScoreCalculation(5000);
                    //�n�[�g�𐶐�
                    SetItem(g_Enemy[nCntEnemy].pos, ITEM_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_000);
                    break;

                    //�t�B�b�V��
                case ENEMYTYPE_005:
                    ScoreCalculation(3500);
                    g_nCntPhase3--;
                    break;
                }

                //��1�t�F�[�Y�̕�
                if (g_nCntPhase1 == 0)
                {
                    WALL*pWall = GetWall();
                    for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
                    {
                        if (pWall->type == WALLTYPE_RED1)
                        {
                            //�g�p���Ȃ疢�g�p�ɂ���
                            if (pWall->bUse == true)
                            {
                                pWall->bUse = false;

                                //���b�N�����}�[�N
                                SetEffect(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + g_Enemy[nCntEnemy].size.y / 2
                                            , g_Enemy[nCntEnemy].pos.z), UNLOCK_SIZE,D3DXVECTOR3(0.0f, 1.0f, 0.0f), EFFECTTYPE_002);
                            }
                        }
                    }
                }

                //��2�t�F�[�Y�̕�
                if (g_nCntPhase2 == 0)
                {
                    WALL*pWall = GetWall();
                    for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
                    {
                        if (pWall->type == WALLTYPE_RED2)
                        {
                            //�g�p���Ȃ疢�g�p�ɂ���
                            if (pWall->bUse == true)
                            {
                                pWall->bUse = false;

                                //���b�N�����}�[�N
                                SetEffect(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + g_Enemy[nCntEnemy].size.y / 2
                                            , g_Enemy[nCntEnemy].pos.z), UNLOCK_SIZE, D3DXVECTOR3(0.0f, 1.0f, 0.0f), EFFECTTYPE_002);
                            }
                        }
                    }
                }

                //��3�t�F�[�Y�̕�
                if (g_nCntPhase3 == 0)
                {
                    WALL*pWall = GetWall();
                    for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
                    {
                        if (pWall->type == WALLTYPE_RED3)
                        {
                            //�g�p���Ȃ疢�g�p�ɂ���
                            if (pWall->bUse == true)
                            {
                                pWall->bUse = false;

                                //���b�N�����}�[�N
                                SetEffect(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + g_Enemy[nCntEnemy].size.y / 2
                                            , g_Enemy[nCntEnemy].pos.z), UNLOCK_SIZE, D3DXVECTOR3(0.0f, 1.0f, 0.0f), EFFECTTYPE_002);
                            }
                        }
                    }
                }

                //�o�O���
                g_Enemy[nCntEnemy].fDistance = 9999.9f;
                g_Enemy[nCntEnemy].bUse = false;

                //������
                PlaySound(SOUND_LABEL_SE_EXPLOSION2);
            }

        }
    }

    //�����N�t���̓�������
    int nNum01, nNum02;		//�������m���r����ۂɎg�p
    for (nNum01 = 0; nNum01 < MAX_ENEMY; nNum01++)
    {
        for (nNum02 = 0; nNum02 < MAX_ENEMY; nNum02++)
        {
            //��������ԋ߂����̂�Rank1�ɂȂ�悤�ɂ���
            if (g_Enemy[nNum01].fDistance >= g_Enemy[nNum02].fDistance)
            {
                g_Enemy[nNum01].nRank++;
            }
        }
    }
}

//=====================================================
// �G�l�~�[�`�揈��
//=====================================================
void DrawEnemy(void)
{
    int nCntEnemy;
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;
    D3DMATERIAL9 matDef;		// ���݂̃}�e���A���ێ��p
    D3DXMATERIAL* pMat;			// �}�e���A���f�[�^�ւ̃|�C���^


    for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        if (g_Enemy[nCntEnemy].bUse == true)
        {
            // ���[���h�}�g���b�N�X�̏�����
            D3DXMatrixIdentity(&g_Enemy[nCntEnemy].mtxWorld);

            // �X�P�[���𔽉f
            D3DXMatrixScaling(&mtxScale,
                g_Enemy[nCntEnemy].size.x / g_Enemy[nCntEnemy].size.x,
                g_Enemy[nCntEnemy].size.y / g_Enemy[nCntEnemy].size.y,
                g_Enemy[nCntEnemy].size.z / g_Enemy[nCntEnemy].size.z);
            D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorld, &g_Enemy[nCntEnemy].mtxWorld, &mtxScale);
            // �����𔽉f
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_Enemy[nCntEnemy].rot.y,
                g_Enemy[nCntEnemy].rot.x,
                g_Enemy[nCntEnemy].rot.z);
            D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorld, &g_Enemy[nCntEnemy].mtxWorld, &mtxRot);
            // �ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans,
                g_Enemy[nCntEnemy].pos.x,
                g_Enemy[nCntEnemy].pos.y,
                g_Enemy[nCntEnemy].pos.z);
            D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorld, &g_Enemy[nCntEnemy].mtxWorld, &mtxTrans);


            // ���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].mtxWorld);
            // ���݂̃}�e���A�����擾����
            pDevice->GetMaterial(&matDef);
            // �}�e���A���f�[�^�ւ̃|�C���^���擾
            pMat = (D3DXMATERIAL*)g_pBuffMatEnemy[g_Enemy[nCntEnemy].type]->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)g_nNumMatEnemy[g_Enemy[nCntEnemy].type]; nCntMat++)
            {
                // �}�e���A���̐ݒ�
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                // ���f���p�[�c
                g_pMeshEnemy[g_Enemy[nCntEnemy].type]->DrawSubset(nCntMat);
            }
            // �ێ����Ă����}�e���A����߂�
            pDevice->SetMaterial(&matDef);
        }
    }
}

//=============================================================================
// �G�l�~�[�̐ݒ菈��
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos,D3DXVECTOR3 move, ENEMYTYPE type)
{
    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        if (g_Enemy[nCntEnemy].bUse == false)
        {
            //�ʒu��ݒ�
            g_Enemy[nCntEnemy].pos = pos;

            //�z�u�ʒu���L��
            g_Enemy[nCntEnemy].posMemory = pos;

            g_Enemy[nCntEnemy].move = move;

            g_Enemy[nCntEnemy].type = type;

            switch (g_Enemy[nCntEnemy].type)
            {
            case ENEMYTYPE_000:
                g_Enemy[nCntEnemy].size = ENEMY_SIZE_CANNON;
                g_Enemy[nCntEnemy].fLife = ENEMY_LIFE_CANNON;
                break;
            case ENEMYTYPE_001:
                g_Enemy[nCntEnemy].size = ENEMY_SIZE_SPIDER;
                g_Enemy[nCntEnemy].fLife = ENEMY_LIFE_SPIDER;
                break;
            case ENEMYTYPE_002:
                g_Enemy[nCntEnemy].size = ENEMY_SIZE_BUTTERFLY;
                g_Enemy[nCntEnemy].fLife = ENEMY_LIFE_BUTTERFLY;
                break;
            case ENEMYTYPE_003:
                g_Enemy[nCntEnemy].size = ENEMY_SIZE_SPIDER;
                g_Enemy[nCntEnemy].fLife = ENEMY_LIFE_GOLDENSPIDER;
                break;
            case ENEMYTYPE_004:
                g_Enemy[nCntEnemy].size = ENEMY_SIZE_JUMPBOX;
                g_Enemy[nCntEnemy].fLife = ENEMY_LIFE_JUMPBOX;
                break;
            case ENEMYTYPE_005:
                g_Enemy[nCntEnemy].size = ENEMY_SIZE_FISH;
                g_Enemy[nCntEnemy].fLife = ENEMY_LIFE_FISH;
                break;
            }

            g_Enemy[nCntEnemy].bUse = true;

            //�G��HP�o�[�̐ݒ�
            //SetEnemyLife(g_Enemy[nCntEnemy].fLife, nCntEnemy);

            break;
        }
    }
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
ENEMY*GetEnemy(void)
{
    return &g_Enemy[0];
}

//=============================================================================
// �G�l�~�[�̃_���[�W�ݒ�
//=============================================================================
void HitEnemy(int nCntEnemy, int nCntRigid, float fDamage)
{
    //PlaySound(SOUND_LABEL_SE_EXPLOSION);	//������

    //�_���[�W��ԂɑJ��
    g_Enemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
    g_Enemy[nCntEnemy].nCounterState = nCntRigid; //�q�b�g�d��

    //�_���[�W�v�Z
    g_Enemy[nCntEnemy].fLife -= fDamage;

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
bool CollisionEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bHit = false;  //�����������ǂ���

    for (int nCntEnemy = 0; nCntEnemy < MAX_BULLET; nCntEnemy++)
    {
        // �e���g��ꂽ��
        if (g_Enemy[nCntEnemy].bUse == true)
        {

            //�����蔻����v�Z
            if (RectangleCollision(pPos, pPosOld, size, &g_Enemy[nCntEnemy].pos, g_Enemy[nCntEnemy].size) == true)
            {

                PLAYER*pPlayer = GetPlayer();

                //�K�E���͖��G
                if (pPlayer->bChanceAttack == false)
                {
                    //�^�C�v���Ƃɕ�����
                    switch (g_Enemy[nCntEnemy].type)
                    {
                        //�L���m��
                    case ENEMYTYPE_000:
                        //���@�Ƀ_���[�W
                        HitPlayer(5, HIT_ENEMY_DAMAGE);
                        break;

                        //�X�p�C�_�[
                    case ENEMYTYPE_001:
                        //���@�Ƀ_���[�W
                        HitPlayer(5, HIT_ENEMY_DAMAGE);
                        break;

                        //�o�^�t���C
                    case ENEMYTYPE_002:
                        //���@�Ƀ_���[�W
                        HitPlayer(5, HIT_ENEMY_DAMAGE);
                        break;

                        //�S�[���f���X�p�C�_�[
                    case ENEMYTYPE_003:
                        //���@�Ƀ_���[�W
                        HitPlayer(5, HIT_ENEMY_DAMAGE);
                        break;

                        //�W�����v�{�b�N�X
                    case ENEMYTYPE_004:
                        //���@�Ƀ_���[�W
                        HitPlayer(5, HIT_ENEMY_DAMAGE);
                        break;

                        //�t�B�b�V��
                    case ENEMYTYPE_005:
                        //���@�Ƀ_���[�W
                        HitPlayer(5, HIT_ENEMY_DAMAGE);
                        break;
                    }

                    //�m�b�N�o�b�N
                    float fKnockback = VALUE_KNOCKBACK;
                    pPlayer->move.x = pPlayer->pos.x - g_Enemy[nCntEnemy].posOld.x;
                    pPlayer->move.z = pPlayer->pos.z - g_Enemy[nCntEnemy].posOld.z;
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
                        SetEffect2(g_Enemy[nCntEnemy].pos, move, EFFECT2_SIZE_HIT, EFFECT2_INIT_COLOR_HIT, EFFECT2TYPE_HIT);
                    }
                }

                bHit = true;    //��������
            }
        }
    }

    return bHit;    //�����������ǂ�����Ԃ�
}

//=====================================================
// �ł��グ�邽�߂̊֐�
//=====================================================
void UpperEnemy(float fUp, int nCnt)
{
    //�ł��グ��
    g_Enemy[nCnt].move.y += fUp;
}