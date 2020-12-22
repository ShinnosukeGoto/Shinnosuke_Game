//=============================================================================
//
// �ŋ��̘b�����̏��� [block.cpp](�u���b�N)
// Author:�㓡�T�V��
//
//=============================================================================
#include "block.h"
#include "collision.h"
#include "stage.h"
#include "player.h"
#include "fire.h"
#include "item.h"
#include "effect.h"
#include "setting.h"
#include "score.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPD3DXMESH g_pMeshBlock[BLOCKTYPE_MAX] = {};                     //���b�V�����ւ̃|�C���^
DWORD g_nNumMatBlock[BLOCKTYPE_MAX] = {};                           //�}�e���A�����̐�
LPD3DXBUFFER g_pBuffMatBlock[BLOCKTYPE_MAX] = {};                //�}�e���A�����̃|�C���^

BLOCK g_aBlock[MAX_BLOCK];

//=============================================================================
// �u���b�N����������
// Author:�㓡�T�V��
//=============================================================================
void InitBlock(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̓ǂݍ���

    //X�t�@�C���̓ǂݍ���
    //�󂹂Ȃ��u���b�N
    D3DXLoadMeshFromX
    (
        "MODEL/normal_block.x",         //x�t�@�C���̃p�X
        D3DXMESH_SYSTEMMEM, //���b�V���쐬�p�̃I�v�V�������w��
        pDevice,            //�f�o�C�X
        NULL,               //(���g�p)
        &g_pBuffMatBlock[BLOCKTYPE_000],   //�}�e���A�����
        NULL,               //(���g�p)
        &g_nNumMatBlock[BLOCKTYPE_000],    //X�t�@�C���Ɏg�p����Ă���}�e���A���̐�
        &g_pMeshBlock[BLOCKTYPE_000]       //x�t�@�C���̃��b�V���f�[�^
    );

    //X�t�@�C���̓ǂݍ���
    //�󂹂�u���b�N
    D3DXLoadMeshFromX
    (
        "MODEL/broken_block.x",         //x�t�@�C���̃p�X
        D3DXMESH_SYSTEMMEM, //���b�V���쐬�p�̃I�v�V�������w��
        pDevice,            //�f�o�C�X
        NULL,               //(���g�p)
        &g_pBuffMatBlock[BLOCKTYPE_001],   //�}�e���A�����
        NULL,               //(���g�p)
        &g_nNumMatBlock[BLOCKTYPE_001],    //X�t�@�C���Ɏg�p����Ă���}�e���A���̐�
        &g_pMeshBlock[BLOCKTYPE_001]       //x�t�@�C���̃��b�V���f�[�^
    );

    //X�t�@�C���̓ǂݍ���
    //�����o������u���b�N
    D3DXLoadMeshFromX
    (
        "MODEL/broken_block.x",         //x�t�@�C���̃p�X
        D3DXMESH_SYSTEMMEM, //���b�V���쐬�p�̃I�v�V�������w��
        pDevice,            //�f�o�C�X
        NULL,               //(���g�p)
        &g_pBuffMatBlock[BLOCKTYPE_002],   //�}�e���A�����
        NULL,               //(���g�p)
        &g_nNumMatBlock[BLOCKTYPE_002],    //X�t�@�C���Ɏg�p����Ă���}�e���A���̐�
        &g_pMeshBlock[BLOCKTYPE_002]       //x�t�@�C���̃��b�V���f�[�^
    );

    //X�t�@�C���̓ǂݍ���
    //�{�����o������u���b�N
    D3DXLoadMeshFromX
    (
        "MODEL/broken_block.x",         //x�t�@�C���̃p�X
        D3DXMESH_SYSTEMMEM, //���b�V���쐬�p�̃I�v�V�������w��
        pDevice,            //�f�o�C�X
        NULL,               //(���g�p)
        &g_pBuffMatBlock[BLOCKTYPE_003],   //�}�e���A�����
        NULL,               //(���g�p)
        &g_nNumMatBlock[BLOCKTYPE_003],    //X�t�@�C���Ɏg�p����Ă���}�e���A���̐�
        &g_pMeshBlock[BLOCKTYPE_003]       //x�t�@�C���̃��b�V���f�[�^
    );

    //X�t�@�C���̓ǂݍ���
    //�X�s�[�h���o������u���b�N
    D3DXLoadMeshFromX
    (
        "MODEL/broken_block.x",         //x�t�@�C���̃p�X
        D3DXMESH_SYSTEMMEM, //���b�V���쐬�p�̃I�v�V�������w��
        pDevice,            //�f�o�C�X
        NULL,               //(���g�p)
        &g_pBuffMatBlock[BLOCKTYPE_004],   //�}�e���A�����
        NULL,               //(���g�p)
        &g_nNumMatBlock[BLOCKTYPE_004],    //X�t�@�C���Ɏg�p����Ă���}�e���A���̐�
        &g_pMeshBlock[BLOCKTYPE_004]       //x�t�@�C���̃��b�V���f�[�^
    );

    //X�t�@�C���̓ǂݍ���
    //�L�b�N���o������u���b�N
    D3DXLoadMeshFromX
    (
        "MODEL/broken_block.x",         //x�t�@�C���̃p�X
        D3DXMESH_SYSTEMMEM, //���b�V���쐬�p�̃I�v�V�������w��
        pDevice,            //�f�o�C�X
        NULL,               //(���g�p)
        &g_pBuffMatBlock[BLOCKTYPE_005],   //�}�e���A�����
        NULL,               //(���g�p)
        &g_nNumMatBlock[BLOCKTYPE_005],    //X�t�@�C���Ɏg�p����Ă���}�e���A���̐�
        &g_pMeshBlock[BLOCKTYPE_005]       //x�t�@�C���̃��b�V���f�[�^
    );

    //X�t�@�C���̓ǂݍ���
    //�v���b�V���[�u���b�N
    D3DXLoadMeshFromX
    (
        "MODEL/pressure_block.x",         //x�t�@�C���̃p�X
        D3DXMESH_SYSTEMMEM, //���b�V���쐬�p�̃I�v�V�������w��
        pDevice,            //�f�o�C�X
        NULL,               //(���g�p)
        &g_pBuffMatBlock[BLOCKTYPE_006],   //�}�e���A�����
        NULL,               //(���g�p)
        &g_nNumMatBlock[BLOCKTYPE_006],    //X�t�@�C���Ɏg�p����Ă���}�e���A���̐�
        &g_pMeshBlock[BLOCKTYPE_006]       //x�t�@�C���̃��b�V���f�[�^
    );

    //X�t�@�C���̓ǂݍ���
    //��u���b�N
    D3DXLoadMeshFromX
    (
        "MODEL/fence_block.x",         //x�t�@�C���̃p�X
        D3DXMESH_SYSTEMMEM, //���b�V���쐬�p�̃I�v�V�������w��
        pDevice,            //�f�o�C�X
        NULL,               //(���g�p)
        &g_pBuffMatBlock[BLOCKTYPE_007],   //�}�e���A�����
        NULL,               //(���g�p)
        &g_nNumMatBlock[BLOCKTYPE_007],    //X�t�@�C���Ɏg�p����Ă���}�e���A���̐�
        &g_pMeshBlock[BLOCKTYPE_007]       //x�t�@�C���̃��b�V���f�[�^
    );

    for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBlock[nCntBlock].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBlock[nCntBlock].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBlock[nCntBlock].size = D3DXVECTOR3(GRID_SIZE, GRID_SIZE, GRID_SIZE);
        g_aBlock[nCntBlock].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        g_aBlock[nCntBlock].type = BLOCKTYPE_000;
        g_aBlock[nCntBlock].bUse = false;
    }
}

//=============================================================================
//�u���b�N�I������
// Author:�㓡�T�V��
//=============================================================================
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
    }
}

//=============================================================================
// �u���b�N�X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateBlock(void)
{
    SETTING *pSetting = GetSetting();

    for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        if (g_aBlock[nCntBlock].bUse == true)
        {
            //1F�O�̈ʒu���L��
            g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;

            //�󂹂Ȃ��u���b�N�A�v���b�V���[�u���b�N�A��u���b�N�ȊO
            if (g_aBlock[nCntBlock].type != BLOCKTYPE_000 && g_aBlock[nCntBlock].type != BLOCKTYPE_006 &&g_aBlock[nCntBlock].type != BLOCKTYPE_007)
            {
                //���ɓ���������
                if (HitFire(&g_aBlock[nCntBlock].pos, &g_aBlock[nCntBlock].pos,
                    D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE)) == true)
                {
                    //30f�����ē����ɂ���
                    g_aBlock[nCntBlock].col.a -= 0.034f;

                }
            }

            //�v���b�V���[�u���b�N�̈ړ�
            if (g_aBlock[nCntBlock].type == BLOCKTYPE_006)
            {
                //�d��
                g_aBlock[nCntBlock].move.y -= GRAVITY;

                //�ړ��ʂƈʒu��R�Â�
                g_aBlock[nCntBlock].pos += g_aBlock[nCntBlock].move;

                //���ɓ�������
                if (g_aBlock[nCntBlock].pos.y < 0.0f)
                {
                    g_aBlock[nCntBlock].pos.y = 0.0f;
                    g_aBlock[nCntBlock].move.y = 0.0f;
                }
            }

            //��u���b�N�̈ړ�
            if (g_aBlock[nCntBlock].type == BLOCKTYPE_007)
            {
                //��ɏオ��
                g_aBlock[nCntBlock].move.y += GRAVITY * 2;

                //�ړ��ʂƈʒu��R�Â�
                g_aBlock[nCntBlock].pos += g_aBlock[nCntBlock].move;

                //���ɏ����
                if (g_aBlock[nCntBlock].pos.y > 0.0f)
                {
                    g_aBlock[nCntBlock].pos.y = 0.0f;
                    g_aBlock[nCntBlock].move.y = 0.0f;
                }
            }

            //�����ɂȂ����疢�g�p��
            if (g_aBlock[nCntBlock].col.a <= 0.0f)
            {
                //���g�p��
                g_aBlock[nCntBlock].bUse = false;

                //�O���b�h�����X�V
                SetGridInfo(g_aBlock[nCntBlock].pos, false);

                //�X�R�A������
                ScoreCalculation(BREAK_BLOCK_SCORE);    //�u���b�N���󂵂����̃X�R�A

                //�u���b�N�̎�ނ��ƂɁA�A�C�e���̏o��
                switch (g_aBlock[nCntBlock].type)
                {
                    //���o��
                case BLOCKTYPE_002:
                    SetItem(D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + GRID_SIZE / 2.0f
                        , g_aBlock[nCntBlock].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_000);
                    break;

                    //�{���o��
                case BLOCKTYPE_003:
                    SetItem(D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + GRID_SIZE / 2.0f
                        , g_aBlock[nCntBlock].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_001);
                    break;

                    //�X�s�[�h�o��
                case BLOCKTYPE_004:
                    SetItem(D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + GRID_SIZE / 2.0f
                        , g_aBlock[nCntBlock].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_002);
                    break;

                    //�L�b�N�o��
                case BLOCKTYPE_005:
                    SetItem(D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + GRID_SIZE / 2.0f
                        , g_aBlock[nCntBlock].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_003);
                    break;
                }

                //������
                g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }

            //�u���b�N�̃N���b�V������ (�T�h���f�X������Ȃ�A������g�p�j
            if (pSetting->bSuddenDeath == true)
            {
                if (CrushingBlock(&g_aBlock[nCntBlock].pos, &g_aBlock[nCntBlock].posOld,
                    D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE), nCntBlock) == true)
                {
                    //���g�p��
                    g_aBlock[nCntBlock].bUse = false;

                    //�O���b�h�����X�V
                    SetGridInfo(g_aBlock[nCntBlock].pos, false);

                    //�u���b�N�����G�t�F�N�g
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
                        SetEffect(g_aBlock[nCntBlock].pos, move, EFFECT_SIZE_EXPLODE_ITEM, EFFECTTYPE_EXPLOSION);
                    }

                    //������
                    g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                }
            }
        }
    }
}

//=============================================================================
// �u���b�N�`�揈��
// Author:�㓡�T�V��
//=============================================================================
void DrawBlock(void)
{

    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̓ǂݍ���
    D3DXMATRIX mtxRot, mtxTrans, mtxscale;
    D3DMATERIAL9 matDef;//���݂̃}�e���A���ێ��p
    D3DXMATERIAL *pMat;//�}�e���A���f�[�^�ւ̃|�C���^

    for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        //���[���h�}�g���b�N�X�̏�����
        D3DXMatrixIdentity(&g_aBlock[nCntBlock].mtxWorld);
        if (g_aBlock[nCntBlock].bUse)
        {
            //�g�k�𔽉f
            D3DXMatrixScaling(&mtxscale, g_aBlock[nCntBlock].size.x / GRID_SIZE, g_aBlock[nCntBlock].size.y / GRID_SIZE, g_aBlock[nCntBlock].size.z / GRID_SIZE);
            D3DXMatrixMultiply(&g_aBlock[nCntBlock].mtxWorld, &g_aBlock[nCntBlock].mtxWorld, &mtxscale);
            //�����𔽉f
            D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBlock[nCntBlock].rot.y, g_aBlock[nCntBlock].rot.x, g_aBlock[nCntBlock].rot.z);
            D3DXMatrixMultiply(&g_aBlock[nCntBlock].mtxWorld, &g_aBlock[nCntBlock].mtxWorld, &mtxRot);
            //�ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans, g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, g_aBlock[nCntBlock].pos.z);
            D3DXMatrixMultiply(&g_aBlock[nCntBlock].mtxWorld, &g_aBlock[nCntBlock].mtxWorld, &mtxTrans);

            //���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_aBlock[nCntBlock].mtxWorld);

            //���݂̃}�e���A�����擾����
            pDevice->GetMaterial(&matDef);

            //�}�e���A���f�[�^�̃|�C���^���擾
            pMat = (D3DXMATERIAL*)g_pBuffMatBlock[g_aBlock[nCntBlock].type]->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)g_nNumMatBlock[g_aBlock[nCntBlock].type]; nCntMat++)
            {
                //�}�e���A���̐ݒ�
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

                //���f���p�[�c�̕`��
                g_pMeshBlock[g_aBlock[nCntBlock].type]->DrawSubset(nCntMat);
            }

            //�ێ����Ă����}�e���A����߂�
            pDevice->SetMaterial(&matDef);
        }
    }

    //�e�N�X�`���̐ݒ�̉���
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// �u���b�N���̎擾
// Author:�㓡�T�V��
//=============================================================================
BLOCK * GetBlock(void)
{
    return &g_aBlock[0];
}

//=====================================================
// �u���b�N�̏Փ˔���̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    // �ϐ��錾
    bool bCollision = false;    // �Փ˂��Ă��邩�ǂ���
    int nCntBlock;

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // �u���b�N���g��ꂽ��
        if (g_aBlock[nCntBlock].bUse == true)
        {
            // �Փ˔�����v�Z
            if (BoxCollision(pPos, pPosOld, size, &g_aBlock[nCntBlock].pos, 
                D3DXVECTOR3(g_aBlock[nCntBlock].size.x, g_aBlock[nCntBlock].size.y * 4, g_aBlock[nCntBlock].size.z)) == true)
            {
                // �Փ˂̃t���O��true��
                bCollision = true;
            }
        }
    }

    return bCollision;
}

//=====================================================
// ���Ȃ��u���b�N�̓����蔻��̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool HitNormalBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    // �ϐ��錾
    bool bHit = false;  // �������Ă��邩�ǂ���
    int nCntBlock;

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // �u���b�N���g��ꂽ��
        if (g_aBlock[nCntBlock].bUse == true)
        {
            //���Ȃ��u���b�N�Ȃ�
            if (g_aBlock[nCntBlock].type == BLOCKTYPE_000 || g_aBlock[nCntBlock].type == BLOCKTYPE_006 || g_aBlock[nCntBlock].type == BLOCKTYPE_007)
            {
                //�����蔻����v�Z
                if (RectangleCollision(pPos, pPosOld, size, &g_aBlock[nCntBlock].pos, g_aBlock[nCntBlock].size) == true)
                {
                    bHit = true;
                }
            }
        }
    }

    return bHit;
}

//=====================================================
// ����u���b�N�̓����蔻��̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool HitBrokenBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bHit = false;
    int nCntBlock;					// ���[�J���ϐ�

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // �u���b�N���g��ꂽ��
        if (g_aBlock[nCntBlock].bUse == true)
        {
            //����u���b�N�Ȃ�
            if (g_aBlock[nCntBlock].type != BLOCKTYPE_000 && g_aBlock[nCntBlock].type != BLOCKTYPE_006)
            {
                //�����蔻����v�Z
                if (RectangleCollision(pPos, pPosOld, size, &g_aBlock[nCntBlock].pos, g_aBlock[nCntBlock].size) == true)
                {
                    bHit = true;
                }
            }
        }
    }

    return bHit;
}

//=====================================================
// ���ׂĂ̎�ނ̃u���b�N�̓����蔻��̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool HitAllTypeBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bHit = false;
    int nCntBlock;					// ���[�J���ϐ�

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // �u���b�N���g��ꂽ��
        if (g_aBlock[nCntBlock].bUse == true)
        {
            //�����蔻����v�Z
            if (RectangleCollision(pPos, pPosOld, size, &g_aBlock[nCntBlock].pos, g_aBlock[nCntBlock].size) == true)
            {
                bHit = true;
            }
        }
    }

    return bHit;
}

//=====================================================
// �u���b�N�̐ݒu
// Author:�㓡�T�V��
//=====================================================
void SetBlock(D3DXVECTOR3 pos, BLOCKTYPE type)
{

    int nCntBlock;				// ���[�J���ϐ�

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // �u���b�N���g���ĂȂ��Ƃ�
        if (g_aBlock[nCntBlock].bUse == false)
        {
            // �u���b�N���g�p���Ă����Ԃɂ���
            g_aBlock[nCntBlock].pos = pos;
            g_aBlock[nCntBlock].posOld = pos;
            g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aBlock[nCntBlock].size = D3DXVECTOR3(GRID_SIZE, GRID_SIZE, GRID_SIZE);
            g_aBlock[nCntBlock].type = type;
            g_aBlock[nCntBlock].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
            g_aBlock[nCntBlock].bUse = true;

            //�v���b�V���[�u���b�N�̏ꍇ�A�O���b�h�������Ȃ�
            if (g_aBlock[nCntBlock].type != BLOCKTYPE_006)
            {
                //�O���b�h�̏���ݒ�
                SetGridInfo(g_aBlock[nCntBlock].pos, true);
            }

            break;
        }
    }
}

//=====================================================
// �N���b�V������̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool CrushingBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, int nNumberBlock)
{
    bool bCrush = false;       //�����Ԃ��ꂽ���ǂ���     
    int nCntBlock;

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // �u���b�N���g��ꂽ��
        if (g_aBlock[nCntBlock].bUse == true)
        {
            //�ԍ����Ⴄ�Ȃ�
            if (nCntBlock != nNumberBlock)
            {
                //�v���C���[�̈ʒu
                D3DXVECTOR3 box1Max = D3DXVECTOR3(size.x / 2, size.y, size.z / 2) + *pPos;
                D3DXVECTOR3 box1Min = D3DXVECTOR3(-size.x / 2, -size.y, -size.z / 2) + *pPos;

                //�u���b�N�̈ʒu
                D3DXVECTOR3 box2Max = D3DXVECTOR3(g_aBlock[nCntBlock].size.x / 2, g_aBlock[nCntBlock].size.y,
                    g_aBlock[nCntBlock].size.z / 2) + g_aBlock[nCntBlock].pos;
                D3DXVECTOR3 box2Min = D3DXVECTOR3(-g_aBlock[nCntBlock].size.x / 2, 0.0f,
                    -g_aBlock[nCntBlock].size.z / 2) + g_aBlock[nCntBlock].pos;

                // �u���b�N�Ƃ̓����蔻��
                if ((box1Max.y > box2Min.y) &&
                    (box1Min.y < box2Max.y) &&
                    (box1Max.x > box2Min.x) &&
                    (box1Min.x < box2Max.x) &&
                    (box1Min.z < box2Max.z) &&
                    (box1Max.z > box2Min.z))
                {

                    if (box1Max.y > box2Min.y&&
                        pPosOld->y + size.y / 2 < box2Min.y)
                    {   //y���̉�
                        bCrush = true;
                    }
                }
            }
        }
    }

    return bCrush;
}