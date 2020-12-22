//=====================================================
//
// PLASTIC TROOPERS�̏��� [block.cpp](�r���Ȃǂ̃u���b�N)
// Author:�㓡�T�V��
//
//=====================================================

//=====================================================
// �C���N���[�h�t�@�C��
//=====================================================
#include "block.h"
#include "camera.h"
#include "player.h"
#include "collision.h"
#include "enemy.h"
#include "boss.h"
#include "effect2.h"

//=====================================================
// �O���[�o���ϐ�
//=====================================================
BLOCK g_Block[MAX_BLOCK];

// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock[BLOCKTYPE_MAX] = {};

// ���b�V�����ւ̃|�C���^
LPD3DXMESH g_pMeshBlock[BLOCKTYPE_MAX] = {};
// �}�e���A�����ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatBlock[BLOCKTYPE_MAX] = {};
// �}�e���A�����̐�
DWORD g_nNumMatBlock[BLOCKTYPE_MAX] = {};


//=====================================================
// ����������
//=====================================================
void InitBlock(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^


    int nCntBlock;


    // �z�񏉊���
    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        g_Block[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Block[nCntBlock].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Block[nCntBlock].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Block[nCntBlock].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Block[nCntBlock].type = BLOCKTYPE_000;
        g_Block[nCntBlock].bUse = false;

    }

    // X�t�@�C���̓ǂݍ���
    D3DXLoadMeshFromX("MODEL/block000.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatBlock[BLOCKTYPE_000],
        NULL,
        &g_nNumMatBlock[BLOCKTYPE_000],
        &g_pMeshBlock[BLOCKTYPE_000]);

    for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        D3DXMATERIAL* d3dxmatrs = (D3DXMATERIAL*)g_pBuffMatBlock[BLOCKTYPE_000]->GetBufferPointer();
        for (int nCntMat = 0; nCntMat < (int)g_nNumMatBlock[BLOCKTYPE_000]; nCntMat++)
        {
            if (d3dxmatrs[nCntMat].pTextureFilename != NULL)
            {
                D3DXCreateTextureFromFile(pDevice, d3dxmatrs[nCntMat].pTextureFilename, &g_Block[nCntBlock].textureModel[nCntMat]);
            }
        }
    }
}

//=====================================================
// �I������
//=====================================================
void UninitBlock(void)
{
    // ���b�V���̔j��
    for (int nCntBlock = 0; nCntBlock < BLOCKTYPE_MAX; nCntBlock++)
    {
        if (g_pMeshBlock[nCntBlock] != NULL)
        {
            g_pMeshBlock[nCntBlock]->Release();
            g_pMeshBlock[nCntBlock] = NULL;
        }
        // �}�e���A���̔j��
        if (g_pBuffMatBlock[nCntBlock] != NULL)
        {
            g_pBuffMatBlock[nCntBlock]->Release();
            g_pBuffMatBlock[nCntBlock] = NULL;
        }
        // �e�N�X�`���̔j��
        for (int nCntMat = 0; nCntMat < (int)g_nNumMatBlock[BLOCKTYPE_000]; nCntMat++)
        {

            if (g_Block[nCntBlock].textureModel[nCntMat] != NULL)
            {
                g_Block[nCntBlock].textureModel[nCntMat]->Release();
                g_Block[nCntBlock].textureModel[nCntMat] = NULL;
            }
        }
    }
}

//=====================================================
// �X�V����
//=====================================================
void UpdateBlock(void)
{
    for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        if (g_Block[nCntBlock].bUse == true)
        {
            g_Block[nCntBlock].posOld = g_Block[nCntBlock].pos;

            //�{�X�ƃu���b�N������������
            if (CollisionBoss(&g_Block[nCntBlock].pos, &g_Block[nCntBlock].posOld,
                g_Block[nCntBlock].size) == true)
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
                    SetEffect2(g_Block[nCntBlock].pos, move, EFFECT2_SIZE_EXPLOSION, EFFECT2_INIT_COLOR_EXPLOSION, EFFECT2TYPE_EXPLOSION); //�����G�t�F�N�g
                }

                g_Block[nCntBlock].bUse = false;
            }

        }
    }
}

//=====================================================
// �`�揈��
//=====================================================
void DrawBlock(void)
{
    int nCntBlock;
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;
    D3DMATERIAL9 matDef;		// ���݂̃}�e���A���ێ��p
    D3DXMATERIAL* pMat;			// �}�e���A���f�[�^�ւ̃|�C���^


    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        if (g_Block[nCntBlock].bUse == true)
        {
            // ���[���h�}�g���b�N�X�̏�����
            D3DXMatrixIdentity(&g_Block[nCntBlock].mtxWorld);

            // �X�P�[���𔽉f
            D3DXMatrixScaling(&mtxScale,
                g_Block[nCntBlock].size.x / g_Block[nCntBlock].size.x,
                g_Block[nCntBlock].size.y / g_Block[nCntBlock].size.y,
                g_Block[nCntBlock].size.z / g_Block[nCntBlock].size.z);
            D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorld, &g_Block[nCntBlock].mtxWorld, &mtxScale);
            // �����𔽉f
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_Block[nCntBlock].rot.y,
                g_Block[nCntBlock].rot.x,
                g_Block[nCntBlock].rot.z);
            D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorld, &g_Block[nCntBlock].mtxWorld, &mtxRot);
            // �ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans,
                g_Block[nCntBlock].pos.x,
                g_Block[nCntBlock].pos.y,
                g_Block[nCntBlock].pos.z);
            D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorld, &g_Block[nCntBlock].mtxWorld, &mtxTrans);


            // ���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_Block[nCntBlock].mtxWorld);
            // �e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, NULL);
            // ���݂̃}�e���A�����擾����
            pDevice->GetMaterial(&matDef);
            // �}�e���A���f�[�^�ւ̃|�C���^���擾
            pMat = (D3DXMATERIAL*)g_pBuffMatBlock[g_Block[nCntBlock].type]->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)g_nNumMatBlock[g_Block[nCntBlock].type]; nCntMat++)
            {
                // �e�N�X�`���̐ݒ�
                pDevice->SetTexture(0, g_Block[nCntBlock].textureModel[nCntMat]);
                pMat[nCntMat].MatD3D.Ambient.r = 1.0f;
                pMat[nCntMat].MatD3D.Ambient.g = 1.0f;
                pMat[nCntMat].MatD3D.Ambient.b = 1.0f;
                pMat[nCntMat].MatD3D.Ambient.a = 1.0f;
                // �}�e���A���̐ݒ�
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                // ���f���p�[�c
                g_pMeshBlock[g_Block[nCntBlock].type]->DrawSubset(nCntMat);
            }
            // �ێ����Ă����}�e���A����߂�
            pDevice->SetMaterial(&matDef);
        }
    }
}

//=====================================================
// �Z�b�g�u���b�N
//=====================================================
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 size, BLOCKTYPE type)
{

    int nCntBlock;				// ���[�J���ϐ�

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // �u���b�N���g���ĂȂ��Ƃ�
        if (g_Block[nCntBlock].bUse == false)
        {
            // �Ώۂ̔z��̗v�f�ɁAused��pos��size���Z�b�g

            // �u���b�N���g�p���Ă����Ԃɂ���

            g_Block[nCntBlock].pos = pos;
            g_Block[nCntBlock].size = size;
            g_Block[nCntBlock].type = type;
            g_Block[nCntBlock].bUse = true;
            break;
        }
    }
}

//=====================================================
// ���n����̐ݒ�
//=====================================================
bool LandBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntBlock;					// ���[�J���ϐ�
    PLAYER *pPlayer;
    bool isGround = false;

    pPlayer = GetPlayer();
    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // �u���b�N���g��ꂽ��
        if (g_Block[nCntBlock].bUse == true)
        {
            //�v���C���[�̈ʒu
            D3DXVECTOR3 box1Max = D3DXVECTOR3(size.x / 2, size.y / 2, size.z / 2) + *pPos;
            D3DXVECTOR3 box1Min = D3DXVECTOR3(-size.x / 2, -size.y / 2, -size.z / 2) + *pPos;

            //�u���b�N�̈ʒu
            D3DXVECTOR3 box2Max = D3DXVECTOR3(g_Block[nCntBlock].size.x / 2, g_Block[nCntBlock].size.y / 2, g_Block[nCntBlock].size.z / 2) + g_Block[nCntBlock].pos;
            D3DXVECTOR3 box2Min = D3DXVECTOR3(-g_Block[nCntBlock].size.x / 2, -g_Block[nCntBlock].size.y / 2, -g_Block[nCntBlock].size.z / 2) + g_Block[nCntBlock].pos;

            // �u���b�N�Ƃ̓����蔻��
            if ((box1Max.y > box2Min.y) &&
                (box1Min.y < box2Max.y) &&
                (box1Max.x > box2Min.x) &&
                (box1Min.x < box2Max.x) &&
                (box1Min.z < box2Max.z) &&
                (box1Max.z > box2Min.z))
            {

                if (box1Max.y + size.y > box2Min.y &&
                    pPlayer->posOld.y + (size.y / 2) <= box2Min.y)
                {
                    // �u���b�N�ɉ����瓖��������
                    pPlayer->pos.y = box2Min.y - (size.y / 2);		// �v���C���[���u���b�N�ɂ߂荞�܂Ȃ��悤�ɂ���
                    pPlayer->move.y = 0;
                }
                else if (box1Min.x < box2Max.x &&
                    pPlayer->posOld.x - (size.x / 2) >= box2Max.x)
                {
                    // �u���b�N�ɍ����瓖�������Ƃ�
                    pPlayer->pos.x = box2Max.x + (size.x / 2);	// �v���C���[�������߂�

                }
                else if (box1Max.x >  box2Min.x &&
                    pPlayer->posOld.x + (size.x / 2) <= box2Min.x)
                {
                    // �u���b�N�ɉE���瓖��������
                    pPlayer->pos.x = box2Min.x - (size.x / 2);
                }
                else if ((box1Min.y + size.y / 2) < box2Max.y &&
                    pPlayer->posOld.y - (0.0f) >= box2Max.y)
                {
                    // �u���b�N�ɏォ�瓖��������
                    pPlayer->pos.y = box2Max.y + (0.0f);
                    isGround = true;			// ���n����
                }
                else if (box1Min.z < box2Max.z &&
                    pPlayer->posOld.z - (size.z / 2) >= box2Max.z)
                {
                    // �u���b�N�ɉ����瓖��������
                    pPlayer->pos.z = box2Max.z + (size.z / 2);

                }
                else if (box1Max.z > box2Min.z &&
                    pPlayer->posOld.z + (size.z / 2) <= box2Min.z)
                {
                    // �u���b�N�ɉ����瓖��������
                    pPlayer->pos.z = box2Min.z - (size.z / 2);

                }
            }
        }
    }
    return isGround;
}

//=====================================================
// �G�̃u���b�N�ɑ΂��锻��
//=====================================================
bool EnemyHitBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntBlock;					// ���[�J���ϐ�
    bool isGround = false;

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // �u���b�N���g��ꂽ��
        if (g_Block[nCntBlock].bUse == true)
        {
            //�����蔻����v�Z
            BoxCollision(pPos, pPosOld, size, &g_Block[nCntBlock].pos, g_Block[nCntBlock].size);
        }
    }
    return isGround;
}

//=====================================================
// �u���b�N�̎擾
//=====================================================
BLOCK*GetBlock(void)
{
    return &g_Block[0];		// �l��Ԃ�(�n��)
}

//=====================================================
// �����蔻��̐ݒ�
//=====================================================
bool HitBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntBlock;					// ���[�J���ϐ�
    bool bHit = false;

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // �A�C�e�����g��ꂽ��
        if (g_Block[nCntBlock].bUse == true)
        {
            if (RectangleCollision(pPos, pPosOld, size, &g_Block[nCntBlock].pos, g_Block[nCntBlock].size) == true)
            {
                bHit = true;
            }
        }
    }

    return bHit;
}