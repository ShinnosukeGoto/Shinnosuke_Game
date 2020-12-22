//=============================================================================
//
// �ŋ��̘b�����̏���[enemy.cpp]�i�Q�[�����ɏo�Ă���G���G�j
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "player.h"
#include "camera.h"
#include "shadow.h"
#include "enemy.h"
#include "bullet.h"
#include "score.h"
#include "effect.h"
#include "collision.h"
#include "block.h"
#include "item.h"
#include "sound.h"
#include "game.h"
#include "fire.h"
#include "bomb.h"

//=============================================================================
// �O���[�o���ϐ���`
//=============================================================================
LPD3DXMESH g_pMeshEnemy[ENEMYTYPE_MAX] = {};		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatEnemy[ENEMYTYPE_MAX] = {};   // �}�e���A�����ւ̃|�C���^
DWORD g_nNumMatEnemy[ENEMYTYPE_MAX] = {};			// �}�e���A�����̐�

ENEMY g_aEnemy[MAX_ENEMY];	        //�G�l�~�[�̏��
int g_RandomAttack = 1;				//�e�������_���ɑłƂ��Ɏg�p

//=============================================================================
// �G�l�~�[����������
// Author:�㓡�T�V��
//=============================================================================
void InitEnemy(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    // X�t�@�C���̓ǂݍ���
    //�L���m��
    D3DXLoadMeshFromX("MODEL/enemy000.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_CANNON],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_CANNON],
        &g_pMeshEnemy[ENEMYTYPE_CANNON]);

    //�A���u����
    D3DXLoadMeshFromX("MODEL/enemy001.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_UMBRELLA],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_UMBRELLA],
        &g_pMeshEnemy[ENEMYTYPE_UMBRELLA]);

    //�S�[�X�g
    D3DXLoadMeshFromX("MODEL/enemy002.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_GHOST],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_GHOST],
        &g_pMeshEnemy[ENEMYTYPE_GHOST]);

    //�c�`�m�R
    D3DXLoadMeshFromX("MODEL/enemy003.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_SNAKE],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_SNAKE],
        &g_pMeshEnemy[ENEMYTYPE_SNAKE]);

    //���b�h�S�[�X�g
    D3DXLoadMeshFromX("MODEL/enemy004.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_RED_GHOST],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_RED_GHOST],
        &g_pMeshEnemy[ENEMYTYPE_RED_GHOST]);

    //�G�̏��̏�����
    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEnemy[nCntEnemy].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEnemy[nCntEnemy].rotMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEnemy[nCntEnemy].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
        g_aEnemy[nCntEnemy].type = ENEMYTYPE_CANNON;
        g_aEnemy[nCntEnemy].bUse = false;
        g_aEnemy[nCntEnemy].fLife = 0.0f;
        g_aEnemy[nCntEnemy].nAttackTime = 0;
        g_aEnemy[nCntEnemy].nShadow = 0;
        g_aEnemy[nCntEnemy].nCntInvincibleFrame = 0;
        g_aEnemy[nCntEnemy].bTransparence = false;
        g_aEnemy[nCntEnemy].fDistance = 0.0f;
        g_aEnemy[nCntEnemy].bFindPlayer = false;
        g_aEnemy[nCntEnemy].nCntInvincibleFrame = 0;
    }
}

//=============================================================================
// �G�l�~�[�I������
// Author:�㓡�T�V��
//=============================================================================
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

//=============================================================================
// �G�l�~�[�X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateEnemy(void)
{
    //���@���擾����
    PLAYER *pPlayer = GetPlayer();

    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        //�g�p���Ȃ�
        if (g_aEnemy[nCntEnemy].bUse == true)
        {
            //���G����
            if (g_aEnemy[nCntEnemy].nCntInvincibleFrame > 0)
            {
                g_aEnemy[nCntEnemy].nCntInvincibleFrame--;                     //���G���Ԃ𐔂���
                g_aEnemy[nCntEnemy].bTransparence = !g_aEnemy[nCntEnemy].bTransparence;   //���G���́A�_�ł�����
            }
            else
            {
                g_aEnemy[nCntEnemy].bTransparence = false; //����������
            }

            //1F�O�̈ʒu���L��
            g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

            //�U����ړ��Ɋւ���ϐ�
            float fPposx = pPlayer->pos.x, fPposz = pPlayer->pos.z;	                        //���@�̍��W
            float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposz = g_aEnemy[nCntEnemy].pos.z;	//�G�̍��W
            float fAngle = atan2f((fEposx - fPposx), (fEposz - fPposz));	                //�p�x�����߂�
            float fAngle2 = atan2f((fPposx - fEposx), (fPposz - fEposz));	                //�p�x�����߂�

            //���@�ƓG�̋���
            g_aEnemy[nCntEnemy].fDistance = sqrtf(
                powf((pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x), 2) +
                powf((pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z), 2));

            //�G�t�F�N�g�̂��߂̕ϐ�
            float fAngleEffect = 0.0f;    //�p�x
            float fRadius = g_aEnemy[nCntEnemy].size.x; //�G�t�F�N�g���a
            float fSpeedX = 0.0f;   //����X
            float fSpeedY = 0.0f;   //����Y
            float fSpeedZ = 0.0f;   //����Z
            int nPlusMinusX = 0;
            int nPlusMinusY = 0;
            int nPlusMinusZ = 0;

            //�e�G�l�~�[
            switch (g_aEnemy[nCntEnemy].type)
            {
                //========================
                //�L���m��
                //========================
            case ENEMYTYPE_CANNON:

                //��Ԃɂ��ꍇ����
                switch (g_aEnemy[nCntEnemy].state)
                {
                    //�ʏ펞
                case ENEMYSTATE_NORMAL:

                    //�ʒu���ړ��ʂƕR�Â�
                    g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

                    //����ɓ����Ă��A�U���̃N�[���^�C�����I�������s��
                    if (g_aEnemy[nCntEnemy].fDistance < ENEMY_FIELD_OF_VISION && g_aEnemy[nCntEnemy].nAttackTime == 0)
                    {
                        //���@�̂ق������X�Ɍ���悤�ɂ���
                        g_aEnemy[nCntEnemy].rotDest.y = fAngle;

                        // ��]�̐���
                        if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                        {
                            g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                        }
                        if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                        {
                            g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                        }

                        g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y;

                        // �ړI�̊p�x�̉�]�𐧌�
                        if (g_aEnemy[nCntEnemy].rotMin.y > D3DX_PI)
                        {
                            g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2;
                        }
                        if (g_aEnemy[nCntEnemy].rotMin.y < -D3DX_PI)
                        {
                            g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2;
                        }

                        // ��]�̐���
                        if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                        {
                            g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                        }
                        if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                        {
                            g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                        }

                        if (fabsf(g_aEnemy[nCntEnemy].rotMin.y) < 0)
                        {
                            g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y;
                        }
                        else
                        {
                            g_aEnemy[nCntEnemy].rot += (g_aEnemy[nCntEnemy].rotDest - g_aEnemy[nCntEnemy].rot) * ENEMY_TURN_SPEED;
                        }

                        //�e�̔���
                        if ((rand() % (g_RandomAttack * ENEMY_ATTACK_COOL_DOWN)) == 0)
                        {
                            SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + ENEMY_HEIGHT_CANNON / 2
                                , g_aEnemy[nCntEnemy].pos.z), BULLET_SIZE_ENEMY,
                                D3DXVECTOR3(-sinf(g_aEnemy[nCntEnemy].rot.y)*BULLET_SPEED_ENEMY, 0.0f
                                    , -cosf(g_aEnemy[nCntEnemy].rot.y)*BULLET_SPEED_ENEMY), BULLET_LIFE_ENEMY, BULLET_DAMAGE_ENEMY, BULLETTYPE_ENEMY);

                            g_aEnemy[nCntEnemy].nAttackTime = ENEMY_ATTACK_COOL_DOWN;
                        }
                    }

                    break;

                    //�_���[�W��
                case ENEMYSTATE_DAMAGE:

                    if (g_aEnemy[nCntEnemy].nCntInvincibleFrame <= 0)
                    {
                        g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                break;

                //========================
                //�A���u����
                //========================
            case ENEMYTYPE_UMBRELLA:

                //��Ԃɂ��ꍇ����
                switch (g_aEnemy[nCntEnemy].state)
                {
                    //�ʏ펞
                case ENEMYSTATE_NORMAL:

                    //�ʒu���ړ��ʂƕR�Â�
                    g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

                    // �ړ����Ă������������
                    // ��]�̐���
                    if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                    }

                    g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y;

                    // �ړI�̊p�x�̉�]�𐧌�
                    if (g_aEnemy[nCntEnemy].rotMin.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rotMin.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2;
                    }

                    // ��]�̐���
                    if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                    }

                    if (fabsf(g_aEnemy[nCntEnemy].rotMin.y) < 0)
                    {
                        g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y;
                    }
                    else
                    {
                        g_aEnemy[nCntEnemy].rot += (g_aEnemy[nCntEnemy].rotDest - g_aEnemy[nCntEnemy].rot) * ENEMY_TURN_SPEED;
                    }

                    //�W�����v
                    if (g_aEnemy[nCntEnemy].pos.y == 0)
                    {
                        g_aEnemy[nCntEnemy].move.y = ENEMY_JUMP;
                    }

                    break;

                    //�_���[�W��
                case ENEMYSTATE_DAMAGE:

                    if (g_aEnemy[nCntEnemy].nCntInvincibleFrame <= 0)
                    {
                        g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                break;

                //========================
                //�S�[�X�g
                //========================
            case ENEMYTYPE_GHOST:

                //��Ԃɂ��ꍇ����
                switch (g_aEnemy[nCntEnemy].state)
                {
                    //�ʏ펞
                case ENEMYSTATE_NORMAL:

                    //�ʒu���ړ��ʂƕR�Â�
                    g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

                    // �ړ����Ă������������
                    // ��]�̐���
                    if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                    }

                    g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y;

                    // �ړI�̊p�x�̉�]�𐧌�
                    if (g_aEnemy[nCntEnemy].rotMin.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rotMin.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2;
                    }

                    // ��]�̐���
                    if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                    }

                    if (fabsf(g_aEnemy[nCntEnemy].rotMin.y) < 0)
                    {
                        g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y;
                    }
                    else
                    {
                        g_aEnemy[nCntEnemy].rot += (g_aEnemy[nCntEnemy].rotDest - g_aEnemy[nCntEnemy].rot) * ENEMY_TURN_SPEED;
                    }

                    break;

                    //�_���[�W��
                case ENEMYSTATE_DAMAGE:

                    if (g_aEnemy[nCntEnemy].nCntInvincibleFrame <= 0)
                    {
                        g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                break;

                //========================
                //���b�h�S�[�X�g
                //========================
            case ENEMYTYPE_RED_GHOST:

                //��Ԃɂ��ꍇ����
                switch (g_aEnemy[nCntEnemy].state)
                {
                    //�ʏ펞
                case ENEMYSTATE_NORMAL:

                    //�ʒu���ړ��ʂƕR�Â�
                    g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

                    // �ړ����Ă������������
                    // ��]�̐���
                    if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                    }

                    g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y;

                    // �ړI�̊p�x�̉�]�𐧌�
                    if (g_aEnemy[nCntEnemy].rotMin.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rotMin.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2;
                    }

                    // ��]�̐���
                    if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                    }

                    if (fabsf(g_aEnemy[nCntEnemy].rotMin.y) < 0)
                    {
                        g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y;
                    }
                    else
                    {
                        g_aEnemy[nCntEnemy].rot += (g_aEnemy[nCntEnemy].rotDest - g_aEnemy[nCntEnemy].rot) * ENEMY_TURN_SPEED;
                    }

                    break;

                    //�_���[�W��
                case ENEMYSTATE_DAMAGE:

                    if (g_aEnemy[nCntEnemy].nCntInvincibleFrame <= 0)
                    {
                        g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                break;

                //========================
                //�c�`�m�R
                //========================
            case ENEMYTYPE_SNAKE:

                //��Ԃɂ��ꍇ����
                switch (g_aEnemy[nCntEnemy].state)
                {
                    //�ʏ펞
                case ENEMYSTATE_NORMAL:

                    //�ʒu���ړ��ʂƕR�Â�
                    g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

                    //����ɓ�������s��
                    if (g_aEnemy[nCntEnemy].fDistance < ENEMY_FIELD_OF_VISION)
                    {
                        g_aEnemy[nCntEnemy].bFindPlayer = true;
                    }

                    break;

                    //�_���[�W��
                case ENEMYSTATE_DAMAGE:

                    if (g_aEnemy[nCntEnemy].nCntInvincibleFrame <= 0)
                    {
                        g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                break;
            }

            //�U�����̃J�E���^�[
            if (g_aEnemy[nCntEnemy].nAttackTime > 0)
            {
                g_aEnemy[nCntEnemy].nAttackTime--;
            }

            //�d�͂ƒn�ʂ̐ݒ�
            if (g_aEnemy[nCntEnemy].type != ENEMYTYPE_SNAKE)
            {
                g_aEnemy[nCntEnemy].move.y -= GRAVITY;
                if (g_aEnemy[nCntEnemy].pos.y < 0)
                {
                    g_aEnemy[nCntEnemy].pos.y = 0;
                    g_aEnemy[nCntEnemy].move.y = 0;
                }
            }
            //�c�`�m�R�͒n���܂ōs�����疢�g�p�ɂ���
            else
            {
                //�n���܂ōs������
                if (g_aEnemy[nCntEnemy].pos.y < -GRID_SIZE)
                {
                    //���g�p��
                    g_aEnemy[nCntEnemy].bUse = false;

                    //�e�̍폜
                    SHADOW *pShadow = GetShadow();//�e�̏����擾
                    pShadow[g_aEnemy[nCntEnemy].nShadow].use = false;

                    //������
                    g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                    g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                    g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                    g_aEnemy[nCntEnemy].fDistance = 9999.9f;
                }
            }

            //�c�`�m�R�����@�𔭌������ꍇ
            if (g_aEnemy[nCntEnemy].bFindPlayer == true)
            {
                g_aEnemy[nCntEnemy].rot.y += ENEMY_TURN_SPEED;
                g_aEnemy[nCntEnemy].move.y -= ENEMY_TURN_SPEED;
            }

            //�S�[�X�g�A�A���u�����A���b�h�S�[�X�g�̏ꍇ
            if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_GHOST || g_aEnemy[nCntEnemy].type == ENEMYTYPE_UMBRELLA
                || g_aEnemy[nCntEnemy].type == ENEMYTYPE_RED_GHOST)
            {
                //�u���b�N�̓����蔻��
                if (CollisionBlock(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld,
                    D3DXVECTOR3(GRID_SIZE, GRID_SIZE * 2, GRID_SIZE)) == true)
                {
                    int nRandom = 0;

                    //�^�C�v���Ƃɏꍇ����
                    switch(g_aEnemy[nCntEnemy].type)
                    {
                        //�A���u�����̏ꍇ
                    case ENEMYTYPE_UMBRELLA:
                        //�E�ړ���
                        if (g_aEnemy[nCntEnemy].move.x > 0)
                        {
                            nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_UMBRELLA_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_UMBRELLA_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_UMBRELLA_SPEED;
                            }
                        }

                        //���ړ���
                        else if (g_aEnemy[nCntEnemy].move.x < 0)
                        {
                            nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_UMBRELLA_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_UMBRELLA_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_UMBRELLA_SPEED;
                            }
                        }

                        //���ړ���
                        else if (g_aEnemy[nCntEnemy].move.z > 0)
                        {
                            nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_UMBRELLA_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_UMBRELLA_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_UMBRELLA_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                        }

                        //��O�ړ���
                        else if (g_aEnemy[nCntEnemy].move.z < 0)
                        {
                            nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_UMBRELLA_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_UMBRELLA_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_UMBRELLA_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                        }

                        break;

                        //�S�[�X�g�̏ꍇ
                    case ENEMYTYPE_GHOST:
                        //�E�ړ���
                        if (g_aEnemy[nCntEnemy].move.x > 0)
                        {
                            nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_GHOST_SPEED;
                            }
                        }

                        //���ړ���
                        else if (g_aEnemy[nCntEnemy].move.x < 0)
                        {
                            nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_GHOST_SPEED;
                            }
                        }

                        //���ړ���
                        else if (g_aEnemy[nCntEnemy].move.z > 0)
                        {
                            nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                        }

                        //��O�ړ���
                        else if (g_aEnemy[nCntEnemy].move.z < 0)
                        {
                            nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                        }

                        break;

                        //���b�h�S�[�X�g�̏ꍇ
                    case ENEMYTYPE_RED_GHOST:
                        //�E�ړ���
                        if (g_aEnemy[nCntEnemy].move.x > 0)
                        {
                            nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_RED_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_RED_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_RED_GHOST_SPEED;
                            }
                        }

                        //���ړ���
                        else if (g_aEnemy[nCntEnemy].move.x < 0)
                        {
                            nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_RED_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_RED_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_RED_GHOST_SPEED;
                            }
                        }

                        //���ړ���
                        else if (g_aEnemy[nCntEnemy].move.z > 0)
                        {
                            nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_RED_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_RED_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_RED_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                        }

                        //��O�ړ���
                        else if (g_aEnemy[nCntEnemy].move.z < 0)
                        {
                            nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_RED_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_RED_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_RED_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                        }

                        break;
                    }  
                }

                //���e�����蔲�����鎞�Ԃ̃J�E���^�[
                if (g_aEnemy[nCntEnemy].nCntDisabledBomb > 0)
                {
                    g_aEnemy[nCntEnemy].nCntDisabledBomb--;
                }

                //���e�̓����蔻��
                if (g_aEnemy[nCntEnemy].nCntDisabledBomb == 0)
                {
                    if (CollisionBomb(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld,
                        D3DXVECTOR3(GRID_SIZE, GRID_SIZE * 2, GRID_SIZE)) == true)
                    {
                        g_aEnemy[nCntEnemy].nCntDisabledBomb = ENEMY_DISABLED_BOMB_FRAME;   //���e�����蔲������t���[����

                        int nRandom = 0;

                        //�^�C�v���Ƃɏꍇ����
                        switch (g_aEnemy[nCntEnemy].type)
                        {
                            //�A���u�����̏ꍇ
                        case ENEMYTYPE_UMBRELLA:
                            //�E�ړ���
                            if (g_aEnemy[nCntEnemy].move.x > 0)
                            {
                                nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_UMBRELLA_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_UMBRELLA_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_UMBRELLA_SPEED;
                                }
                            }

                            //���ړ���
                            else if (g_aEnemy[nCntEnemy].move.x < 0)
                            {
                                nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_UMBRELLA_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_UMBRELLA_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_UMBRELLA_SPEED;
                                }
                            }

                            //���ړ���
                            else if (g_aEnemy[nCntEnemy].move.z > 0)
                            {
                                nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_UMBRELLA_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_UMBRELLA_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_UMBRELLA_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                            }

                            //��O�ړ���
                            else if (g_aEnemy[nCntEnemy].move.z < 0)
                            {
                                nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_UMBRELLA_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_UMBRELLA_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_UMBRELLA_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                            }

                            break;

                            //�S�[�X�g�̏ꍇ
                        case ENEMYTYPE_GHOST:
                            //�E�ړ���
                            if (g_aEnemy[nCntEnemy].move.x > 0)
                            {
                                nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_GHOST_SPEED;
                                }
                            }

                            //���ړ���
                            else if (g_aEnemy[nCntEnemy].move.x < 0)
                            {
                                nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_GHOST_SPEED;
                                }
                            }

                            //���ړ���
                            else if (g_aEnemy[nCntEnemy].move.z > 0)
                            {
                                nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                            }

                            //��O�ړ���
                            else if (g_aEnemy[nCntEnemy].move.z < 0)
                            {
                                nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                            }

                            break;

                            //���b�h�S�[�X�g�̏ꍇ
                        case ENEMYTYPE_RED_GHOST:
                            //�E�ړ���
                            if (g_aEnemy[nCntEnemy].move.x > 0)
                            {
                                nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_RED_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_RED_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_RED_GHOST_SPEED;
                                }
                            }

                            //���ړ���
                            else if (g_aEnemy[nCntEnemy].move.x < 0)
                            {
                                nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_RED_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_RED_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_RED_GHOST_SPEED;
                                }
                            }

                            //���ړ���
                            else if (g_aEnemy[nCntEnemy].move.z > 0)
                            {
                                nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_RED_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_RED_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_RED_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                            }

                            //��O�ړ���
                            else if (g_aEnemy[nCntEnemy].move.z < 0)
                            {
                                nRandom = rand() % 3 + 1;    //1�`3�𓾂�

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_RED_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_RED_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_RED_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                            }

                            break;
                        }
                    }
                }

                //�����̖ړI�l���A�ړ��̌����ƍ��킹��
                //�E
                if (g_aEnemy[nCntEnemy].move.x > 0.0f)
                {
                    g_aEnemy[nCntEnemy].rotDest.y = D3DXToRadian(270);
                }
                //��
                if (g_aEnemy[nCntEnemy].move.x < 0.0f)
                {
                    g_aEnemy[nCntEnemy].rotDest.y = D3DXToRadian(90);
                }
                //��
                if (g_aEnemy[nCntEnemy].move.z > 0.0f)
                {
                    g_aEnemy[nCntEnemy].rotDest.y = D3DXToRadian(180);
                }
                //��O
                if (g_aEnemy[nCntEnemy].move.z < 0.0f)
                {
                    g_aEnemy[nCntEnemy].rotDest.y = D3DXToRadian(0);
                }
            }

            //���̓����蔻��
            if (HitFire(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, g_aEnemy[nCntEnemy].size.y,
                    GRID_SIZE - COLLISION_AVOID)) == true)
            {
                HitEnemy(nCntEnemy, BOMB_DAMAGE);
            }

            //�e��ݒ�
            SetPositionShadow(g_aEnemy[nCntEnemy].nShadow
                , D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 0, g_aEnemy[nCntEnemy].pos.z), g_aEnemy[nCntEnemy].pos.y);

            //���S����
            if (g_aEnemy[nCntEnemy].fLife <= 0.0f)
            {
                //���ꂽ���̐�
                PlaySound(SOUND_LABEL_SE_EXPLOSION2);

                //��ނ��ƂɁA�X�R�A���Z
                switch (g_aEnemy[nCntEnemy].type)
                {
                case ENEMYTYPE_CANNON:
                    ScoreCalculation(ENEMY_CANNON_SCORE);
                    break;
                case ENEMYTYPE_UMBRELLA:
                    ScoreCalculation(ENEMY_UMBRELLA_SCORE);
                    break;
                case ENEMYTYPE_GHOST:
                    ScoreCalculation(ENEMY_GHOST_SCORE);
                    break;
                case ENEMYTYPE_SNAKE:
                    ScoreCalculation(ENEMY_SNAKE_SCORE);
                    break;
                case ENEMYTYPE_RED_GHOST:
                    ScoreCalculation(ENEMY_RED_GHOST_SCORE);
                    break;
                }

                //�����G�t�F�N�g
                float fSpeedX = 0.0f;   //����X
                float fSpeedY = 0.0f;   //����Y
                float fSpeedZ = 0.0f;   //����Z
                int nPlusMinusX = 0;    //����X
                int nPlusMinusZ = 0;    //����Z
                for (int nCnt = 0; nCnt < COUNT_EFFECT_EXPLODE_ITEM; nCnt++)
                {
                    fSpeedX = float(rand() % 200) / 100.0f;
                    fSpeedY = float(rand() % 800 + 300) / 100.0f;
                    fSpeedZ = float(rand() % 200) / 100.0f;
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
                    SetEffect(g_aEnemy[nCntEnemy].pos, move, EFFECT_SIZE_EXPLODE_ITEM, EFFECTTYPE_EXPLOSION);
                }

                //���g�p��
                g_aEnemy[nCntEnemy].bUse = false;

                //�e�̍폜
                SHADOW *pShadow = GetShadow();//�e�̏����擾
                pShadow[g_aEnemy[nCntEnemy].nShadow].use = false;

                //������
                g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aEnemy[nCntEnemy].fDistance = 9999.9f;
            }

        }
    }
}

//=====================================================
// �G�l�~�[�`�揈��
// Author:�㓡�T�V��
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
        //�g�p���������łȂ��Ȃ�
        if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].bTransparence == false)
        {
            // ���[���h�}�g���b�N�X�̏�����
            D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

            // �X�P�[���𔽉f
            D3DXMatrixScaling(&mtxScale,
                g_aEnemy[nCntEnemy].size.x / g_aEnemy[nCntEnemy].size.x,
                g_aEnemy[nCntEnemy].size.y / g_aEnemy[nCntEnemy].size.y,
                g_aEnemy[nCntEnemy].size.z / g_aEnemy[nCntEnemy].size.z);
            D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxScale);
            // �����𔽉f
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_aEnemy[nCntEnemy].rot.y,
                g_aEnemy[nCntEnemy].rot.x,
                g_aEnemy[nCntEnemy].rot.z);
            D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);
            // �ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans,
                g_aEnemy[nCntEnemy].pos.x,
                g_aEnemy[nCntEnemy].pos.y,
                g_aEnemy[nCntEnemy].pos.z);
            D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);


            // ���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);
            // ���݂̃}�e���A�����擾����
            pDevice->GetMaterial(&matDef);
            // �}�e���A���f�[�^�ւ̃|�C���^���擾
            pMat = (D3DXMATERIAL*)g_pBuffMatEnemy[g_aEnemy[nCntEnemy].type]->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)g_nNumMatEnemy[g_aEnemy[nCntEnemy].type]; nCntMat++)
            {
                // �}�e���A���̐ݒ�
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                // ���f���p�[�c
                g_pMeshEnemy[g_aEnemy[nCntEnemy].type]->DrawSubset(nCntMat);
            }
            // �ێ����Ă����}�e���A����߂�
            pDevice->SetMaterial(&matDef);
        }
    }
}

//=============================================================================
// �G�l�~�[�̐ݒ菈��
// Author:�㓡�T�V��
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE type)
{
    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        if (g_aEnemy[nCntEnemy].bUse == false)
        {
            //�ݒ�
            g_aEnemy[nCntEnemy].pos = pos;
            g_aEnemy[nCntEnemy].posOld = pos;
            g_aEnemy[nCntEnemy].move = move;
            g_aEnemy[nCntEnemy].type = type;

            //������
            g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
            g_aEnemy[nCntEnemy].nAttackTime = 0;
            g_aEnemy[nCntEnemy].nCntInvincibleFrame = 0;
            g_aEnemy[nCntEnemy].bTransparence = false;
            g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aEnemy[nCntEnemy].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aEnemy[nCntEnemy].rotMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aEnemy[nCntEnemy].bFindPlayer = false;
            g_aEnemy[nCntEnemy].nCntInvincibleFrame = 0;

            //��ނ��ƂɁA�傫���ƃ��C�t��ݒ�
            switch (g_aEnemy[nCntEnemy].type)
            {
            case ENEMYTYPE_CANNON:
                g_aEnemy[nCntEnemy].rot.y = D3DXToRadian(90);
                g_aEnemy[nCntEnemy].size = ENEMY_SIZE_CANNON;
                g_aEnemy[nCntEnemy].fLife = ENEMY_LIFE_CANNON;
                break;
            case ENEMYTYPE_UMBRELLA:
                g_aEnemy[nCntEnemy].size = ENEMY_SIZE_UMBRELLA;
                g_aEnemy[nCntEnemy].fLife = ENEMY_LIFE_UMBRELLA;
                break;
            case ENEMYTYPE_GHOST:
                g_aEnemy[nCntEnemy].size = ENEMY_SIZE_GHOST;
                g_aEnemy[nCntEnemy].fLife = ENEMY_LIFE_GHOST;
                break;
            case ENEMYTYPE_SNAKE:
                g_aEnemy[nCntEnemy].rot.y = D3DXToRadian(270);
                g_aEnemy[nCntEnemy].size = ENEMY_SIZE_SNAKE;
                g_aEnemy[nCntEnemy].fLife = ENEMY_LIFE_SNAKE;
                break;
            case ENEMYTYPE_RED_GHOST:
                g_aEnemy[nCntEnemy].size = ENEMY_SIZE_GHOST;
                g_aEnemy[nCntEnemy].fLife = ENEMY_LIFE_RED_GHOST;
                break;
            }

            //�g�p���ɂ���
            g_aEnemy[nCntEnemy].bUse = true;

            //�G�̉e�̐ݒ�
            g_aEnemy[nCntEnemy].nShadow = SetShadow(g_aEnemy[nCntEnemy].pos,g_aEnemy[nCntEnemy].rot,
                GRID_SIZE , g_aEnemy[nCntEnemy].pos.y, D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_ALPHA_MAX));

            break;
        }
    }
}

//=============================================================================
// �G�l�~�[�̎擾
// Author:�㓡�T�V��
//=============================================================================
ENEMY*GetEnemy(void)
{
    return &g_aEnemy[0];
}

//=============================================================================
// �G�l�~�[�̃_���[�W�ݒ�
// Author:�㓡�T�V��
//=============================================================================
void HitEnemy(int nCntEnemy, float fDamage)
{
    //���G���ԂłȂ��Ȃ�
    if (g_aEnemy[nCntEnemy].nCntInvincibleFrame == 0)
    {
        GAME *pGame = GetGame();

        //�Q�[�����I�������A�_���[�W���󂯂Ȃ�
        if (pGame->state == GAMESTATE_NORMAL)
        {
            //�_���[�W��ԂɑJ��
            g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;

            //�_���[�W�v�Z
            g_aEnemy[nCntEnemy].fLife -= fDamage;

            //���G���Ԃ�����
            g_aEnemy[nCntEnemy].nCntInvincibleFrame = ENEMY_INVINCIBLE_FRAME;

            //HP������Ȃ�
            if (g_aEnemy[nCntEnemy].fLife > 0.0f)
            {
                PlaySound(SOUND_LABEL_SE_TAKE_DAMAGE_VOICE3);    //�_���[�W���󂯂����̐�
            }
        }
    }
}

//=====================================================
// �G�l�~�[�̏Փ˔���̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool CollisionEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, bool bPlayer)
{
    bool bCollision = false;  //�Փ˂������ǂ���

    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        // �G���g��ꂽ��
        if (g_aEnemy[nCntEnemy].bUse == true)
        {
            //�����蔻����v�Z
            if (RectangleCollision(pPos, pPosOld, size, &g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].size) == true)
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
                        pPlayer->move.x = pPlayer->pos.x - g_aEnemy[nCntEnemy].posOld.x;
                        pPlayer->move.z = pPlayer->pos.z - g_aEnemy[nCntEnemy].posOld.z;
                        D3DXVec3Normalize(&pPlayer->move, &pPlayer->move);  //�x�N�g���𐳋K��
                        pPlayer->move.x = pPlayer->move.x*fKnockback;
                        pPlayer->move.z = pPlayer->move.z*fKnockback;
                    }
                }
            }
        }
    }

    return bCollision;    //�Փ˂������ǂ�����Ԃ�
}

//=====================================================
// �������~�߂邱�Ƃ��ł���G
// Author:�㓡�T�V��
//=====================================================
bool StopFireEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bCollision = false;  //�Փ˂������ǂ���

    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        // �G���g��ꂽ��
        if (g_aEnemy[nCntEnemy].bUse == true)
        {
            //�������~�߂邱�Ƃ��ł���G�Ȃ�
            if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_CANNON)
            {
                //�����蔻����v�Z
                if (RectangleCollision(pPos, pPosOld, size, &g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].size) == true)
                {
                    bCollision = true;    //�Փ�
                }
            }
        }
    }

    return bCollision;    //�Փ˂������ǂ�����Ԃ�
}