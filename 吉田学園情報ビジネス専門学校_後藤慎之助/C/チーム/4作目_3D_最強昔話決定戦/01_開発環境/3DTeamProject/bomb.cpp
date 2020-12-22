//=============================================================================
//
// �ŋ��̘b�����̏��� [bomb.cpp](���e)
// Author:�㓡�T�V��
//
//=============================================================================
#include "bomb.h"
#include "player.h"
#include "player2.h"
#include "player3.h"
#include "player4.h"
#include "collision.h"
#include "stage.h"
#include "player.h"
#include "fire.h"
#include "block.h"
#include "sound.h"
#include "effect.h"
#include "item.h"
#include "controller.h"
#include "shadow.h"
#include "sound.h"
#include "setting.h"
#include "enemy.h"
#include "bullet.h"
#include "input.h"
#include "boss.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define XFILE_NEME "MODEL/bomb.x"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPD3DXMESH g_pMeshBomb = NULL;                     //���b�V�����ւ̃|�C���^
DWORD g_nNumMatBomb = 0;                           //�}�e���A�����̐�
LPD3DXBUFFER g_pBuffMatBomb = NULL;                //�}�e���A�����̃|�C���^
LPDIRECT3DTEXTURE9 *g_pTexturehBomb = NULL;        //�|���S���̃e�N�X�`���ւ̃|�C���^

BOMB g_aBomb[MAX_BOMB]; //���e�̏��

//=============================================================================
// �{������������
// Author:�㓡�T�V��
//=============================================================================
void InitBomb(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̓ǂݍ���

    //X�t�@�C���̓ǂݍ���
    HRESULT result = D3DXLoadMeshFromX
    (
        XFILE_NEME,         //x�t�@�C���̃p�X
        D3DXMESH_SYSTEMMEM, //���b�V���쐬�p�̃I�v�V�������w��
        pDevice,            //�f�o�C�X
        NULL,               //(���g�p)
        &g_pBuffMatBomb,   //�}�e���A�����
        NULL,               //(���g�p)
        &g_nNumMatBomb,    //X�t�@�C���Ɏg�p����Ă���}�e���A���̐�
        &g_pMeshBomb       //x�t�@�C���̃��b�V���f�[�^
    );

    // �}�e���A���ƃe�N�X�`���̐ݒ�
    D3DXMATERIAL    *materials = (D3DXMATERIAL*)g_pBuffMatBomb->GetBufferPointer();
    g_pTexturehBomb = new LPDIRECT3DTEXTURE9[g_nNumMatBomb];

    // �e�N�X�`���̓ǂݏo��
    for (int nCntBomb = 0; nCntBomb < (int)g_nNumMatBomb; nCntBomb++)
    {
        g_pTexturehBomb[nCntBomb] = NULL;
        if (materials[nCntBomb].pTextureFilename != NULL)
        {
            // �e�N�X�`���̓ǂݍ���
            HRESULT hr = D3DXCreateTextureFromFile(pDevice, materials[nCntBomb].pTextureFilename, &g_pTexturehBomb[nCntBomb]);
            int i = 0;
            i++;
        }
    }
    for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        g_aBomb[nCntBomb].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBomb[nCntBomb].posGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBomb[nCntBomb].posNew = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBomb[nCntBomb].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBomb[nCntBomb].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBomb[nCntBomb].size = D3DXVECTOR3(GRID_SIZE, GRID_SIZE, GRID_SIZE);
        g_aBomb[nCntBomb].sizeVisual = D3DXVECTOR3(GRID_SIZE, GRID_SIZE, GRID_SIZE);
        g_aBomb[nCntBomb].type = SELECT_MOMO;
        g_aBomb[nCntBomb].KickType = SELECT_MAX;
        g_aBomb[nCntBomb].bUse = false;
        g_aBomb[nCntBomb].bFire = false;
        g_aBomb[nCntBomb].nCntTime = TIME_BOMB;
        g_aBomb[nCntBomb].nFire = 0;
        g_aBomb[nCntBomb].nShadow = 0;//�e�̐ݒ�
        g_aBomb[nCntBomb].bIncreaseSize = false;

        g_aBomb[nCntBomb].bCheckGridUp = false;
        g_aBomb[nCntBomb].bCheckGridDown = false;
        g_aBomb[nCntBomb].bCheckGridLeft = false;
        g_aBomb[nCntBomb].bCheckGridRight = false;
    }
}

//=============================================================================
// �{���I������
// Author:�㓡�T�V��
//=============================================================================
void UninitBomb(void)
{
    //���b�V���̊J��
    if (g_pMeshBomb != NULL)
    {
        g_pMeshBomb->Release();
        g_pMeshBomb = NULL;
    }
    //�}�e���A���̔j��
    if (g_pBuffMatBomb != NULL)
    {
        g_pBuffMatBomb->Release();
        g_pBuffMatBomb = NULL;
    }
    if (g_pTexturehBomb != NULL)
    {
        delete g_pTexturehBomb;
        g_pTexturehBomb = NULL;
    }
}

//=============================================================================
// �{���X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateBomb(void)
{
    //�ݒ�̏����擾
    SETTING *pSetting = GetSetting();
    //�e�v���C���[�̎擾
    PLAYER*pPlayer = GetPlayer();
    PLAYER2*pPlayer2 = GetPlayer2();
    PLAYER3*pPlayer3 = GetPlayer3();
    PLAYER4*pPlayer4 = GetPlayer4();
    //�G�t�F�N�g�֘A�̕ϐ�
    float fAngle = 0.0f;    //�p�x
    float fSpeedY = 0.0f;   //����
    float fRadius = 0.0f;   //�G�t�F�N�g�������a

    for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        //�g�p���Ă���Ȃ�
        if (g_aBomb[nCntBomb].bUse == true)
        {
            //�����܂Ő�����
            g_aBomb[nCntBomb].nCntTime--;

            //�������̑傫����ς���
            //������
            if (g_aBomb[nCntBomb].bIncreaseSize == false)
            {
                //���j�܂ł̎c�莞�ԂŁA�{���̌����ڂ̌�������ς���
                //���₩
                if (g_aBomb[nCntBomb].nCntTime > TIME_BOMB / 3)
                {
                    g_aBomb[nCntBomb].sizeVisual.x -= 1.0f;
                    g_aBomb[nCntBomb].sizeVisual.y -= 1.0f;
                    g_aBomb[nCntBomb].sizeVisual.z -= 1.0f;
                }
                //������
                else
                {
                    g_aBomb[nCntBomb].sizeVisual.x -= 10.0f;
                    g_aBomb[nCntBomb].sizeVisual.y -= 10.0f;
                    g_aBomb[nCntBomb].sizeVisual.z -= 10.0f;
                }

                //���ʂŁA����������
                if (g_aBomb[nCntBomb].sizeVisual.x < g_aBomb[nCntBomb].size.x * 2 / 3)
                {
                    g_aBomb[nCntBomb].bIncreaseSize = true;
                }
            }
            //������
            if (g_aBomb[nCntBomb].bIncreaseSize == true)
            {
                //���j�܂ł̎c�莞�ԂŁA�{���̌����ڂ̌�������ς���
                //���₩
                if (g_aBomb[nCntBomb].nCntTime > TIME_BOMB / 3)
                {
                    g_aBomb[nCntBomb].sizeVisual.x += 1.0f;
                    g_aBomb[nCntBomb].sizeVisual.y += 1.0f;
                    g_aBomb[nCntBomb].sizeVisual.z += 1.0f;
                }
                //������
                else
                {
                    g_aBomb[nCntBomb].sizeVisual.x += 10.0f;
                    g_aBomb[nCntBomb].sizeVisual.y += 10.0f;
                    g_aBomb[nCntBomb].sizeVisual.z += 10.0f;
                }

                //���ʂŁA����������
                if (g_aBomb[nCntBomb].sizeVisual.x > g_aBomb[nCntBomb].size.x)
                {
                    g_aBomb[nCntBomb].bIncreaseSize = false;
                }
            }

            //1F�O�̈ʒu���L��
            g_aBomb[nCntBomb].posOld = g_aBomb[nCntBomb].pos;

            //�O���b�h�̒��S�̒l�𓾂�
            g_aBomb[nCntBomb].posGrid = ConversionGrid(&g_aBomb[nCntBomb].pos);

            //�{���u���[�L
            //�v���C���[1
            if (GetJoypadTrigger(PLAYER_1, BUTTON_Y) && g_aBomb[nCntBomb].type == g_aBomb[nCntBomb].KickType
                && g_aBomb[nCntBomb].KickType == SELECT_MOMO)
            {
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
            }

            //�v���C���[2
            if (GetJoypadTrigger(PLAYER_2, BUTTON_Y) && g_aBomb[nCntBomb].type == g_aBomb[nCntBomb].KickType
                && g_aBomb[nCntBomb].KickType == SELECT_URA)
            {
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
            }

            //�v���C���[3
            if (GetJoypadTrigger(PLAYER_3, BUTTON_Y) && g_aBomb[nCntBomb].type == g_aBomb[nCntBomb].KickType
                && g_aBomb[nCntBomb].KickType == SELECT_KIN)
            {
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
            }

            //�v���C���[4
            if (GetJoypadTrigger(PLAYER_4, BUTTON_Y) && g_aBomb[nCntBomb].type == g_aBomb[nCntBomb].KickType
                && g_aBomb[nCntBomb].KickType == SELECT_ISUN)
            {
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
            }

            //�u���b�N�̓����蔻��
            if (CollisionBlock(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
            {
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
            }

            //���e�̓��m�̓����蔻��
            if (CollisionBombs(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), nCntBomb) == true)
            {
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
            }

            //�X�g�[���[�Ȃ�
            if (pSetting->nMaxPlayer == 1)
            {
                //�G�l�~�[�̓����蔻��
                if (CollisionEnemy(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                    D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE), false) == true)
                {
                    g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                    g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
                }

                //�{���u���[�L
                if (GetKeyboardTrigger(DIK_BACKSPACE) && g_aBomb[nCntBomb].type == g_aBomb[nCntBomb].KickType
                    && g_aBomb[nCntBomb].KickType == SELECT_MOMO)
                {
                    g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                    g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
                }

                //�����Y�̔��e���{�X�ɓ����������A���g�p�ɂ���
                if (g_aBomb[nCntBomb].type == SELECT_MOMO)
                {
                    //�e�̓����蔻��
                    if (HitBullet(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        BreakBomb(nCntBomb);    //�{����j�󂷂�
                    }

                    //�{�X�Ƃ̓����蔻��
                    if (CollisionBoss(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE), false) == true)
                    {
                        BreakBomb(nCntBomb);    //�{����j�󂷂�
                    }
                }
            }

            //�v���C���[1�ɑ΂��铖���蔻��
            if (CollisionPlayer(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE)) == true)
            {
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
            }

            //�o�g�����C�����Ȃ�
            if (pSetting->nMaxPlayer >= 2)
            {
                //�v���C���[2�ɑ΂��铖���蔻��
                if (CollisionPlayer2(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                    D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE)) == true)
                {
                    g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                    g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
                }

                //3�l�ȏ�Ȃ�
                if (pSetting->nMaxPlayer >= 3)
                {
                    //�v���C���[3�ɑ΂��铖���蔻��
                    if (CollisionPlayer3(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE)) == true)
                    {
                        g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                        g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
                    }

                    //4�l�Ȃ�
                    if (pSetting->nMaxPlayer >= 4)
                    {
                        //�v���C���[4�ɑ΂��铖���蔻��
                        if (CollisionPlayer4(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                            D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE)) == true)
                        {
                            g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                            g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
                        }
                    }
                }
            }

            //���̓����蔻��
            if (HitFire(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].pos,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID * 2, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID * 2)) == true
                && g_aBomb[nCntBomb].bFire == false)
            {
                g_aBomb[nCntBomb].nCntTime = BOMB_TRIGGER_TIME;
                g_aBomb[nCntBomb].bFire = true;
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //�U���̎������炳�Ȃ�����
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;      //�U���̎������炳�Ȃ�����
            }

            //�㉺���E�̃}�X���m�F���A�{���L�b�N�𐧌䂷��
            g_aBomb[nCntBomb].bCheckGridUp = CheckGridUp(nCntBomb);
            g_aBomb[nCntBomb].bCheckGridDown = CheckGridDown(nCntBomb);
            g_aBomb[nCntBomb].bCheckGridLeft = CheckGridLeft(nCntBomb);
            g_aBomb[nCntBomb].bCheckGridRight = CheckGridRight(nCntBomb);

            //�ړ��ʂƈʒu��R�Â���
            g_aBomb[nCntBomb].pos += g_aBomb[nCntBomb].move;

            //�e��ݒ�
            SetPositionShadow(g_aBomb[nCntBomb].nShadow
                , D3DXVECTOR3(g_aBomb[nCntBomb].pos.x, 0, g_aBomb[nCntBomb].pos.z), 0.0f);

            //�u���b�N�̃N���b�V������ (�T�h���f�X������Ȃ�A������g�p�j
            if (pSetting->bSuddenDeath == true)
            {
                if (CrushingBlock(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                    D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE), MAX_BLOCK + 1) == true)
                {
                    g_aBomb[nCntBomb].nCntTime = -1;    //������
                }
            }

            //����
            if (g_aBomb[nCntBomb].nCntTime < 0)
            {
                //������
                PlaySound(SOUND_LABEL_SE_EXPLOSION);

                //�^��
                SetFire(g_aBomb[nCntBomb].posGrid, g_aBomb[nCntBomb].type);

                //���G�t�F�N�g
                for (int nCnt = 0; nCnt < COUNT_EFFECT_FIRE; nCnt++)
                {
                    fSpeedY = float(rand() % 800 + 300) / 100.0f;
                    //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                    fRadius = float(rand() % 5000 + 0) / 100.0f;
                    D3DXVECTOR3 pos = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x - sinf(fAngle)*fRadius, 0.0f, g_aBomb[nCntBomb].posGrid.z - cosf(fAngle)*fRadius);
                    D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                    SetEffect(pos, move, EFFECT_SIZE_FIRE, EFFECTTYPE_FIRE); //���G�t�F�N�g
                }

                //�Η͕��A�E�ɍL����
                for (int nCntExplosion = 1; nCntExplosion < g_aBomb[nCntBomb].nFire + 1; nCntExplosion++)
                {
                    //�����̍L�����\��
                    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x + GRID_SIZE * nCntExplosion
                        , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z);

                    //�������󂹂Ȃ��u���b�N�ɓ������Ă��Ȃ����ǂ�������
                    //�������Ă��Ȃ��Ȃ�A������ݒu
                    if (HitNormalBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == false)
                    {
                        //����ݒu
                        SetFire(D3DXVECTOR3(g_aBomb[nCntBomb].posNew), g_aBomb[nCntBomb].type);

                        //���G�t�F�N�g
                        for (int nCnt = 0; nCnt < COUNT_EFFECT_FIRE; nCnt++)
                        {
                            fSpeedY = float(rand() % 800 + 300) / 100.0f;
                            //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                            fRadius = float(rand() % 5000 + 0) / 100.0f;
                            D3DXVECTOR3 pos = D3DXVECTOR3(g_aBomb[nCntBomb].posNew.x - sinf(fAngle)*fRadius, 0.0f, g_aBomb[nCntBomb].posNew.z - cosf(fAngle)*fRadius);
                            D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                            SetEffect(pos, move, EFFECT_SIZE_FIRE, EFFECTTYPE_FIRE); //���G�t�F�N�g
                        }
                    }

                    //�������Ă���Ȃ�Afor���甲����
                    else
                    {
                        break;
                    }

                    //�󂹂�u���b�N�ɔ���������������A����u���Ȃ���for���甲����
                    if (HitBrokenBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //�A�C�e���ɔ������������Ă��A����u���Ȃ���for�𔲂���
                    if (HitAllTypeItem(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //���e�ɔ������������Ă��Ă��A����u���Ȃ���for�𔲂���
                    if (HitBomb(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID * 2, PLAYER_HEIGHT, GRID_SIZE - COLLISION_AVOID * 2), nCntBomb) == true)
                    {
                        break;
                    }

                    //�X�g�[���[���A�������~�߂邱�Ƃ��ł���G�ɓ������Ă��Ă��A����u���Ȃ���for�𔲂���
                    if (pSetting->nMaxPlayer == 1)
                    {
                        if (StopFireEnemy(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                            D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                        {
                            break;
                        }
                    }
                }

                //�Η͕��A���ɍL����
                for (int nCntExplosion = 1; nCntExplosion < g_aBomb[nCntBomb].nFire + 1; nCntExplosion++)
                {
                    //�����̍L�����\��
                    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x - GRID_SIZE * nCntExplosion
                        , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z);

                    //�������󂹂Ȃ��u���b�N�ɓ������Ă��Ȃ����ǂ�������
                    //�������Ă��Ȃ��Ȃ�A������ݒu
                    if (HitNormalBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == false)
                    {
                        //����ݒu
                        SetFire(D3DXVECTOR3(g_aBomb[nCntBomb].posNew), g_aBomb[nCntBomb].type);

                        //���G�t�F�N�g
                        for (int nCnt = 0; nCnt < COUNT_EFFECT_FIRE; nCnt++)
                        {
                            fSpeedY = float(rand() % 800 + 300) / 100.0f;
                            //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                            fRadius = float(rand() % 5000 + 0) / 100.0f;
                            D3DXVECTOR3 pos = D3DXVECTOR3(g_aBomb[nCntBomb].posNew.x - sinf(fAngle)*fRadius, 0.0f, g_aBomb[nCntBomb].posNew.z - cosf(fAngle)*fRadius);
                            D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                            SetEffect(pos, move, EFFECT_SIZE_FIRE, EFFECTTYPE_FIRE); //���G�t�F�N�g
                        }
                    }

                    //�������Ă���Ȃ�Afor���甲����
                    else
                    {
                        break;
                    }

                    //�󂹂�u���b�N�ɔ���������������A����u���Ȃ���for���甲����
                    if (HitBrokenBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //�A�C�e���ɔ������������Ă��A����u���Ȃ���for�𔲂���
                    if (HitAllTypeItem(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //���e�ɔ������������Ă��Ă��A����u���Ȃ���for�𔲂���
                    if (HitBomb(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID * 2, PLAYER_HEIGHT, GRID_SIZE - COLLISION_AVOID * 2), nCntBomb) == true)
                    {
                        break;
                    }

                    //�X�g�[���[���A�������~�߂邱�Ƃ��ł���G�ɓ������Ă��Ă��A����u���Ȃ���for�𔲂���
                    if (pSetting->nMaxPlayer == 1)
                    {
                        if (StopFireEnemy(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                            D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                        {
                            break;
                        }
                    }
                }

                //�Η͕��A��ɍL����
                for (int nCntExplosion = 1; nCntExplosion < g_aBomb[nCntBomb].nFire + 1; nCntExplosion++)
                {
                    //�����̍L�����\��
                    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x
                        , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z + GRID_SIZE * nCntExplosion);

                    //�������󂹂Ȃ��u���b�N�ɓ������Ă��Ȃ����ǂ�������
                    //�������Ă��Ȃ��Ȃ�A������ݒu
                    if (HitNormalBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == false)
                    {
                        //����ݒu
                        SetFire(D3DXVECTOR3(g_aBomb[nCntBomb].posNew), g_aBomb[nCntBomb].type);

                        //���G�t�F�N�g
                        for (int nCnt = 0; nCnt < COUNT_EFFECT_FIRE; nCnt++)
                        {
                            fSpeedY = float(rand() % 800 + 300) / 100.0f;
                            //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                            fRadius = float(rand() % 5000 + 0) / 100.0f;
                            D3DXVECTOR3 pos = D3DXVECTOR3(g_aBomb[nCntBomb].posNew.x - sinf(fAngle)*fRadius, 0.0f, g_aBomb[nCntBomb].posNew.z - cosf(fAngle)*fRadius);
                            D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                            SetEffect(pos, move, EFFECT_SIZE_FIRE, EFFECTTYPE_FIRE); //���G�t�F�N�g
                        }
                    }

                    //�������Ă���Ȃ�Afor���甲����
                    else
                    {
                        break;
                    }

                    //�󂹂�u���b�N�ɔ���������������A����u���Ȃ���for���甲����
                    if (HitBrokenBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //�A�C�e���ɔ������������Ă��A����u���Ȃ���for�𔲂���
                    if (HitAllTypeItem(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //���e�ɔ������������Ă��Ă��A����u���Ȃ���for�𔲂���
                    if (HitBomb(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID * 2, PLAYER_HEIGHT, GRID_SIZE - COLLISION_AVOID * 2), nCntBomb) == true)
                    {
                        break;
                    }

                    //�X�g�[���[���A�������~�߂邱�Ƃ��ł���G�ɓ������Ă��Ă��A����u���Ȃ���for�𔲂���
                    if (pSetting->nMaxPlayer == 1)
                    {
                        if (StopFireEnemy(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                            D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                        {
                            break;
                        }
                    }
                }

                //�Η͕��A���ɍL����
                for (int nCntExplosion = 1; nCntExplosion < g_aBomb[nCntBomb].nFire + 1; nCntExplosion++)
                {
                    //�����̍L�����\��
                    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x
                        , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z - GRID_SIZE * nCntExplosion);

                    //�������󂹂Ȃ��u���b�N�ɓ������Ă��Ȃ����ǂ�������
                    //�������Ă��Ȃ��Ȃ�A������ݒu
                    if (HitNormalBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == false)
                    {
                        //����ݒu
                        SetFire(D3DXVECTOR3(g_aBomb[nCntBomb].posNew), g_aBomb[nCntBomb].type);

                        //���G�t�F�N�g
                        for (int nCnt = 0; nCnt < COUNT_EFFECT_FIRE; nCnt++)
                        {
                            fSpeedY = float(rand() % 800 + 300) / 100.0f;
                            //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                            fRadius = float(rand() % 5000 + 0) / 100.0f;
                            D3DXVECTOR3 pos = D3DXVECTOR3(g_aBomb[nCntBomb].posNew.x - sinf(fAngle)*fRadius, 0.0f, g_aBomb[nCntBomb].posNew.z - cosf(fAngle)*fRadius);
                            D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                            SetEffect(pos, move, EFFECT_SIZE_FIRE, EFFECTTYPE_FIRE); //���G�t�F�N�g
                        }
                    }

                    //�������Ă���Ȃ�Afor���甲����
                    else
                    {
                        break;
                    }

                    //�󂹂�u���b�N�ɔ���������������A����u���Ȃ���for���甲����
                    if (HitBrokenBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //�A�C�e���ɔ������������Ă��A����u���Ȃ���for�𔲂���
                    if (HitAllTypeItem(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //���e�ɔ������������Ă��Ă��A����u���Ȃ���for�𔲂���
                    if (HitBomb(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID * 2, PLAYER_HEIGHT, GRID_SIZE - COLLISION_AVOID * 2), nCntBomb) == true)
                    {
                        break;
                    }

                    //�X�g�[���[���A�������~�߂邱�Ƃ��ł���G�ɓ������Ă��Ă��A����u���Ȃ���for�𔲂���
                    if (pSetting->nMaxPlayer == 1)
                    {
                        if (StopFireEnemy(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                            D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                        {
                            break;
                        }
                    }
                }

                //���g�p��
                g_aBomb[nCntBomb].bUse = false;

                //�e�̍폜
                SHADOW *pShadow = GetShadow();//�e�̏����擾
                pShadow[g_aBomb[nCntBomb].nShadow].use = false;

                //������
                g_aBomb[nCntBomb].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].posGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].posNew = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

                //�N���u�������ɉ����āA���̃v���C���[���ēx�{����u����悤�ɂ���
                switch (g_aBomb[nCntBomb].type)
                {
                    //�����Y
                case SELECT_MOMO:
                    pPlayer->nBomb++;
                    break;

                    //�Y�����Y
                case SELECT_URA:
                    pPlayer2->nBomb++;
                    break;

                    //�����Y
                case SELECT_KIN:
                    pPlayer3->nBomb++;
                    break;

                    //�ꐡ�@�t
                case SELECT_ISUN:
                    pPlayer4->nBomb++;
                    break;
                }
            }
        }
    }
}
//=============================================================================
// �{���`�揈��
// Author:�㓡�T�V��
//=============================================================================
void DrawBomb(void)
{

    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̓ǂݍ���
    D3DXMATRIX mtxRot, mtxTrans, mtxscale;
    D3DMATERIAL9 matDef;//���݂̃}�e���A���ێ��p
    D3DXMATERIAL *pMat;//�}�e���A���f�[�^�ւ̃|�C���^

    for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        //���[���h�}�g���b�N�X�̏�����
        D3DXMatrixIdentity(&g_aBomb[nCntBomb].mtxWorld);
        if (g_aBomb[nCntBomb].bUse)
        {
            //�g�k�𔽉f
            D3DXMatrixScaling(&mtxscale, g_aBomb[nCntBomb].sizeVisual.x / g_aBomb[nCntBomb].size.x,
                g_aBomb[nCntBomb].sizeVisual.y / g_aBomb[nCntBomb].size.y
                , g_aBomb[nCntBomb].sizeVisual.z / g_aBomb[nCntBomb].size.z);
            D3DXMatrixMultiply(&g_aBomb[nCntBomb].mtxWorld, &g_aBomb[nCntBomb].mtxWorld, &mtxscale);
            //�����𔽉f
            D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBomb[nCntBomb].rot.y, g_aBomb[nCntBomb].rot.x, g_aBomb[nCntBomb].rot.z);
            D3DXMatrixMultiply(&g_aBomb[nCntBomb].mtxWorld, &g_aBomb[nCntBomb].mtxWorld, &mtxRot);
            //�ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans, g_aBomb[nCntBomb].pos.x, g_aBomb[nCntBomb].pos.y, g_aBomb[nCntBomb].pos.z);
            D3DXMatrixMultiply(&g_aBomb[nCntBomb].mtxWorld, &g_aBomb[nCntBomb].mtxWorld, &mtxTrans);

            //���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_aBomb[nCntBomb].mtxWorld);

            //���݂̃}�e���A�����擾����
            pDevice->GetMaterial(&matDef);

            //�}�e���A���f�[�^�̃|�C���^���擾
            pMat = (D3DXMATERIAL*)g_pBuffMatBomb->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)g_nNumMatBomb; nCntMat++)
            {
                //�}�e���A���̐ݒ�
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pDevice->SetTexture(0, g_pTexturehBomb[nCntMat]);
                //���f���p�[�c�̕`��
                g_pMeshBomb->DrawSubset(nCntMat);
            }

            //�ێ����Ă����}�e���A����߂�
            pDevice->SetMaterial(&matDef);
        }
    }

    //�e�N�X�`���̐ݒ�̉���
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// �{���̏��̎擾
// Author:�㓡�T�V��
//=============================================================================
BOMB *GetBomb(void)
{
    return &g_aBomb[0];
}

//=====================================================
// �{���̓��m�̏Փ˔���̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool CollisionBombs(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, int nNumberBomb)
{
    bool bCollision = false;
    int nCntBomb;					// ���[�J���ϐ�

    for (nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        // �{�����g��ꂽ��
        if (g_aBomb[nCntBomb].bUse == true)
        {
            //�ԍ����Ⴄ�Ȃ�
            if (nCntBomb != nNumberBomb)
            {
                //�����蔻����v�Z���A�������Ă���Ȃ�(�ׂ荇�킹�̃O���b�h�ɂ�����̂��Ȃ����߁AX��Z��-1���Ă���)
                if (BoxCollision(pPos, pPosOld, size, &g_aBomb[nCntBomb].pos,
                    D3DXVECTOR3(g_aBomb[nCntBomb].size.x - 1, g_aBomb[nCntBomb].size.y * 4, g_aBomb[nCntBomb].size.z - 1)) == true)
                {
                    //���݂��ɏR��ꂽ�{���Ȃ�
                    if (g_aBomb[nCntBomb].move.x > 0 && g_aBomb[nNumberBomb].move.x < 0 ||
                        g_aBomb[nCntBomb].move.x > 0 && g_aBomb[nNumberBomb].move.z > 0 ||
                        g_aBomb[nCntBomb].move.x > 0 && g_aBomb[nNumberBomb].move.z < 0 ||
                        g_aBomb[nCntBomb].move.x < 0 && g_aBomb[nNumberBomb].move.x > 0 ||
                        g_aBomb[nCntBomb].move.x < 0 && g_aBomb[nNumberBomb].move.z > 0 ||
                        g_aBomb[nCntBomb].move.x < 0 && g_aBomb[nNumberBomb].move.z < 0 ||
                        g_aBomb[nCntBomb].move.z > 0 && g_aBomb[nNumberBomb].move.x < 0 ||
                        g_aBomb[nCntBomb].move.z > 0 && g_aBomb[nNumberBomb].move.x > 0 ||
                        g_aBomb[nCntBomb].move.z > 0 && g_aBomb[nNumberBomb].move.z < 0 ||
                        g_aBomb[nCntBomb].move.z < 0 && g_aBomb[nNumberBomb].move.x > 0 ||
                        g_aBomb[nCntBomb].move.z < 0 && g_aBomb[nNumberBomb].move.z > 0 ||
                        g_aBomb[nCntBomb].move.z < 0 && g_aBomb[nNumberBomb].move.x < 0 )
                    {
                        //�Ԃ��������e���m�̉Η͂𒲂ׂ�
                        if (g_aBomb[nNumberBomb].nFire > g_aBomb[nCntBomb].nFire)       //�Ԃ��������̉Η͂������Ȃ�
                        {
                            BreakBomb(nCntBomb);    //�Ԃ���ꂽ�ق���j��
                        }
                        else if (g_aBomb[nNumberBomb].nFire == g_aBomb[nCntBomb].nFire) //�Η͂������Ȃ�
                        {
                            //�ǂ�����j��
                            BreakBomb(nNumberBomb);
                            BreakBomb(nCntBomb);
                        }
                        else if (g_aBomb[nNumberBomb].nFire < g_aBomb[nCntBomb].nFire)  //�Ԃ���ꂽ���̉Η͂������Ȃ�
                        {
                            BreakBomb(nNumberBomb); //�Ԃ������ق���j��
                        }
                    }

                    bCollision = true;
                }
            }
        }
    }

    return bCollision;
}

//=====================================================
// �{���̏Փ˔���̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool CollisionBomb(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bCollision = false;
    int nCntBomb;					// ���[�J���ϐ�

    for (nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        // �{�����g��ꂽ��
        if (g_aBomb[nCntBomb].bUse == true)
        {
            //�����蔻����v�Z���A�������Ă���Ȃ�
            if (BoxCollision(pPos, pPosOld, size, &g_aBomb[nCntBomb].pos,
                D3DXVECTOR3(g_aBomb[nCntBomb].size.x, g_aBomb[nCntBomb].size.y * 4, g_aBomb[nCntBomb].size.z)) == true)
            {
                bCollision = true;
            }
        }
    }

    return bCollision;
}

//=====================================================
// �{���̓����蔻��̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool HitBomb(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, int nNumberBomb)
{
    bool bHit = false;
    int nCntBomb;					// ���[�J���ϐ�

    for (nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        // �{�����g��ꂽ��
        if (g_aBomb[nCntBomb].bUse == true)
        {
            //�ԍ����Ⴄ�Ȃ�
            if (nCntBomb != nNumberBomb)
            {
                if (RectangleCollision(pPos, pPosOld, size, &g_aBomb[nCntBomb].pos, g_aBomb[nCntBomb].size) == true)
                {
                    bHit = true;
                }
            }
        }
    }

    return bHit;
}

//=====================================================
// �{���L�b�N����̐ݒ�
// Author:�㓡�T�V��
//=====================================================
void KickBomb(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, SELECT type)
{
    int nCntBomb;					// ���[�J���ϐ�

    for (nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        // �{�����g��ꂽ��
        if (g_aBomb[nCntBomb].bUse == true)
        {
            //�v���C���[�̈ʒu
            D3DXVECTOR3 box1Max = D3DXVECTOR3(size.x / 2, size.y / 2, size.z / 2) + *pPos;
            D3DXVECTOR3 box1Min = D3DXVECTOR3(-size.x / 2, -size.y / 2, -size.z / 2) + *pPos;

            //�{���̈ʒu
            D3DXVECTOR3 box2Max = D3DXVECTOR3(g_aBomb[nCntBomb].size.x / 2, g_aBomb[nCntBomb].size.y / 2, g_aBomb[nCntBomb].size.z / 2) + g_aBomb[nCntBomb].pos;
            D3DXVECTOR3 box2Min = D3DXVECTOR3(-g_aBomb[nCntBomb].size.x / 2, -g_aBomb[nCntBomb].size.y / 2, -g_aBomb[nCntBomb].size.z / 2) + g_aBomb[nCntBomb].pos;

            // �{���Ƃ̓����蔻��
            if ((box1Max.y > box2Min.y) &&
                (box1Min.y < box2Max.y) &&
                (box1Max.x > box2Min.x) &&
                (box1Min.x < box2Max.x) &&
                (box1Min.z < box2Max.z) &&
                (box1Max.z > box2Min.z))
            {

                if (box1Max.y + size.y > box2Min.y &&
                    pPosOld->y + (size.y / 2) <= box2Min.y)
                {
                    // �{���ɉ����瓖��������

                }
                else if (box1Min.x < box2Max.x &&
                    pPosOld->x - (size.x / 2) >= box2Max.x)
                {
                    if (g_aBomb[nCntBomb].bCheckGridLeft == false)
                    {
                        //�R�����l���L�^����
                        g_aBomb[nCntBomb].KickType = type;
                        //�{���L�b�N��
                        PlaySound(SOUND_LABEL_SE_KICK);
                        // �{���ɉE���瓖�������Ƃ�
                        g_aBomb[nCntBomb].move = D3DXVECTOR3(-KICK_MOVE, 0.0f, 0.0f);
                    }
                }
                else if (box1Max.x >  box2Min.x &&
                    pPosOld->x + (size.x / 2) <= box2Min.x)
                {
                    if (g_aBomb[nCntBomb].bCheckGridRight == false)
                    {
                        //�R�����l���L�^����
                        g_aBomb[nCntBomb].KickType = type;
                        //�{���L�b�N��
                        PlaySound(SOUND_LABEL_SE_KICK);
                        // �{���ɍ����瓖��������
                        g_aBomb[nCntBomb].move = D3DXVECTOR3(KICK_MOVE, 0.0f, 0.0f);
                    }
                }
                else if ((box1Min.y + size.y / 2) < box2Max.y &&
                    pPosOld->y - (0.0f) >= box2Max.y)
                {
                    // �{���ɏォ�瓖��������

                }
                else if (box1Min.z < box2Max.z &&
                    pPosOld->z - (size.z / 2) >= box2Max.z)
                {
                    if (g_aBomb[nCntBomb].bCheckGridDown == false)
                    {
                        //�R�����l���L�^����
                        g_aBomb[nCntBomb].KickType = type;
                        //�{���L�b�N��
                        PlaySound(SOUND_LABEL_SE_KICK);
                        // �{���ɉ����瓖��������
                        g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, -KICK_MOVE);
                    }

                }
                else if (box1Max.z > box2Min.z &&
                    pPosOld->z + (size.z / 2) <= box2Min.z)
                {
                    if (g_aBomb[nCntBomb].bCheckGridUp == false)
                    {
                        //�R�����l���L�^����
                        g_aBomb[nCntBomb].KickType = type;
                        //�{���L�b�N��
                        PlaySound(SOUND_LABEL_SE_KICK);
                        // �{���Ɏ�O���瓖��������
                        g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, KICK_MOVE);
                    }
                }
            }
        }
    }

}

//=====================================================
// �{���̐ݒ�
// Author:�㓡�T�V��
//=====================================================
void SetBomb(D3DXVECTOR3 pos, int nFire, int nCntTime, SELECT type)
{

    int nCntBomb;				// ���[�J���ϐ�

    for (nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        // �{�����g���ĂȂ��Ƃ�
        if (g_aBomb[nCntBomb].bUse == false)
        {
            // �{�����g�p���Ă����Ԃɂ���
            g_aBomb[nCntBomb].pos = pos;
            g_aBomb[nCntBomb].nFire = nFire;
            g_aBomb[nCntBomb].nCntTime = nCntTime;  //�����܂ł̎��Ԃ�R�Â�
            g_aBomb[nCntBomb].size = D3DXVECTOR3(GRID_SIZE, GRID_SIZE, GRID_SIZE);
            g_aBomb[nCntBomb].sizeVisual = D3DXVECTOR3(GRID_SIZE, GRID_SIZE, GRID_SIZE);
            g_aBomb[nCntBomb].type = type;
            g_aBomb[nCntBomb].KickType = SELECT_MAX;
            g_aBomb[nCntBomb].bFire = false;
            g_aBomb[nCntBomb].nShadow = SetShadow(g_aBomb[nCntBomb].pos, g_aBomb[nCntBomb].rot, 
                g_aBomb[nCntBomb].size.x, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_ALPHA_MAX));//�e�̐ݒ�
            g_aBomb[nCntBomb].bUse = true;
            g_aBomb[nCntBomb].bIncreaseSize = false;

            g_aBomb[nCntBomb].bCheckGridUp = false;
            g_aBomb[nCntBomb].bCheckGridDown = false;
            g_aBomb[nCntBomb].bCheckGridLeft = false;
            g_aBomb[nCntBomb].bCheckGridRight = false;

            break;
        }
    }
}

//=====================================================
// ��̃O���b�h���m�F
// Author:�㓡�T�V��
//=====================================================
bool CheckGridUp(int nCntBomb)
{
    // �ϐ��錾
    bool bExist = false;    // ���݂��Ă��邩�ǂ���

    // ��̃O���b�h�ɓ������ƁA�V�����ʒu�����肷��
    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x
            , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z + GRID_SIZE);

    //���e�̓��m�̓����蔻��
    if (CollisionBombs(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), nCntBomb) == true)
    {
        bExist = true;
    }

    //�u���b�N�Ƃ̓����蔻��
    if (bExist == false)
    {
        if (HitAllTypeBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID)) == true)
        {
            bExist = true;
        }
    }

    return bExist;
}

//=====================================================
// ���̃O���b�h���m�F
// Author:�㓡�T�V��
//=====================================================
bool CheckGridDown(int nCntBomb)
{
    // �ϐ��錾
    bool bExist = false;    // ���݂��Ă��邩�ǂ���

     // ���̃O���b�h�ɓ������ƁA�V�����ʒu�����肷��
    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x
        , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z - GRID_SIZE);

    //���e�̓��m�̓����蔻��
    if (CollisionBombs(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), nCntBomb) == true)
    {
        bExist = true;
    }

    //�u���b�N�Ƃ̓����蔻��
    if (bExist == false)
    {
        if (HitAllTypeBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID)) == true)
        {
            bExist = true;
        }
    }

    return bExist;
}

//=====================================================
// ���̃O���b�h���m�F
// Author:�㓡�T�V��
//=====================================================
bool CheckGridLeft(int nCntBomb)
{
    // �ϐ��錾
    bool bExist = false;    // ���݂��Ă��邩�ǂ���

    // ���̃O���b�h�ɓ������ƁA�V�����ʒu�����肷��
    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x - GRID_SIZE
        , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z);

    //���e�̓��m�̓����蔻��
    if (CollisionBombs(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), nCntBomb) == true)
    {
        bExist = true;
    }

    //�u���b�N�Ƃ̓����蔻��
    if (bExist == false)
    {
        if (HitAllTypeBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID)) == true)
        {
            bExist = true;
        }
    }

    return bExist;
}

//=====================================================
// �E�̃O���b�h���m�F
// Author:�㓡�T�V��
//=====================================================
bool CheckGridRight(int nCntBomb)
{
    // �ϐ��錾
    bool bExist = false;    // ���݂��Ă��邩�ǂ���

    // �E�̃O���b�h�ɓ������ƁA�V�����ʒu�����肷��
    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x + GRID_SIZE
        , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z);

    //���e�̓��m�̓����蔻��
    if (CollisionBombs(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), nCntBomb) == true)
    {
        bExist = true;
    }

    //�u���b�N�Ƃ̓����蔻��
    if (bExist == false)
    {
        if (HitAllTypeBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID)) == true)
        {
            bExist = true;
        }
    }

    return bExist;
}

//=====================================================
// �{����j�󂷂�
// Author:�㓡�T�V��
//=====================================================
void BreakBomb(int nCntBomb)
{
    //���g�p��
    g_aBomb[nCntBomb].bUse = false;

    //�{���j��G�t�F�N�g
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
        SetEffect(g_aBomb[nCntBomb].pos, move, EFFECT_SIZE_EXPLODE_ITEM, EFFECTTYPE_EXPLOSION);
    }

    //�e�̍폜
    SHADOW *pShadow = GetShadow();//�e�̏����擾
    pShadow[g_aBomb[nCntBomb].nShadow].use = false;

    //������
    g_aBomb[nCntBomb].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_aBomb[nCntBomb].posGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_aBomb[nCntBomb].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    //�N���u�������ɉ����āA���̃v���C���[���ēx�{����u����悤�ɂ���
    //�e�v���C���[�̎擾
    PLAYER*pPlayer = GetPlayer();
    PLAYER2*pPlayer2 = GetPlayer2();
    PLAYER3*pPlayer3 = GetPlayer3();
    PLAYER4*pPlayer4 = GetPlayer4();
    switch (g_aBomb[nCntBomb].type)
    {
        //�����Y
    case SELECT_MOMO:
        pPlayer->nBomb++;
        break;

        //�Y�����Y
    case SELECT_URA:
        pPlayer2->nBomb++;
        break;

        //�����Y
    case SELECT_KIN:
        pPlayer3->nBomb++;
        break;

        //�ꐡ�@�t
    case SELECT_ISUN:
        pPlayer4->nBomb++;
        break;
    }
}