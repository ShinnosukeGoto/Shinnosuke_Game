//=============================================================================
//
// PLASTIC TROOPERS [item.cpp]�̏��� (�A�C�e��)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "item.h"
#include "collision.h"
#include "effect2.h"
#include "player.h"
#include "score.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;   //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureItem[ITEMTYPE_MAX];  //�|���S���̃e�N�X�`��

ITEM g_aItem[MAX_ITEM];    //�G�t�F�N�g�̏��

//=============================================================================
//�A�C�e���̏���������
//=============================================================================
void InitItem(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    int nCntItem = 0;

    //�e�N�X�`���̓ǂݍ���
    //�n�[�g
    g_aItem[nCntItem].type = ITEMTYPE_000;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item003.png",//�t�@�C���̖��O
            &g_pTextureItem[ITEMTYPE_000]);
    }
    //�_�C��
    g_aItem[nCntItem].type = ITEMTYPE_001;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item001.png",//�t�@�C���̖��O
            &g_pTextureItem[ITEMTYPE_001]);
    }
    g_aItem[nCntItem].type = ITEMTYPE_002;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/magic001.png",//�t�@�C���̖��O
            &g_pTextureItem[ITEMTYPE_002]);
    }
    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_3D)*NUM_VERTEX*MAX_ITEM,//�o�b�t�@�T�C�Y
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffItem,
        NULL);//���_���ւ̃|�C���^

    VERTEX_3D *pVtx;//���_���̃|�C���^

                    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

    for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        g_aItem[nCntItem].bUse = false;

        g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aItem[nCntItem].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        //g_aItem[nCntItem].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

        //���_���W�̐ݒ�
        pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        //�@���̐ݒ�
        pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        //���_�J���[�̐ݒ�
        pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        //�e�N�X�`�����W�̐ݒ�
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;  //���_�f�[�^��4���i�߂�
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffItem->Unlock();
}

//=============================================================================
// �A�C�e���̏I������
//=============================================================================
void UninitItem(void)
{
    //�e�N�X�`���̉��
    for (int nCntTex = 0; nCntTex < ITEMTYPE_MAX; nCntTex++)
    {
        //�e�N�X�`���̉��
        if (g_pTextureItem[nCntTex] != NULL)
        {
            g_pTextureItem[nCntTex]->Release();
            g_pTextureItem[nCntTex] = NULL;
        }
    }
    //
    if (g_pVtxBuffItem != NULL)
    {
        g_pVtxBuffItem->Release();
        g_pVtxBuffItem = NULL;
    }
}

//=============================================================================
//�A�C�e���̍X�V����
//=============================================================================
void UpdateItem(void)
{
    for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        if (g_aItem[nCntItem].bUse == true)
        {
            //�n�[�g
            if (g_aItem[nCntItem].type == ITEMTYPE_000)
            {
                //�ړ��ʂƈʒu��R�Â�
                g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;
                //�d��
                g_aItem[nCntItem].move.y -= GRAVITY;
                //���̔���
                if(g_aItem[nCntItem].pos.y < 0 + g_aItem[nCntItem].size.y / 2)
                {
                    g_aItem[nCntItem].move.y = 0;
                    g_aItem[nCntItem].pos.y = 0 + g_aItem[nCntItem].size.y / 2;
                }
            }
        }
    }
}

//=============================================================================
//�A�C�e���̕`�揈��
//=============================================================================
void DrawItem(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    D3DXMATRIX mtxRot, mtxTrans;//�s��v�Z�p�̃}�g���N�X

    //�A���t�@�e�X�g��L����
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //�A���t�@�e�X�g��l�̐ݒ�
    pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    //�A���t�@�e�X�g�̔�r���@�̐ݒ�
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //�}�e���A���̐ݒ�
    D3DMATERIAL9 material;
    ZeroMemory(&material, sizeof(D3DMATERIAL9));
    material.Ambient.r = 1.0f;
    material.Ambient.g = 1.0f;
    material.Ambient.b = 1.0f;
    material.Ambient.a = 1.0f;
    pDevice->SetMaterial(&material);
    pDevice->SetRenderState(D3DRS_AMBIENT, 0x44444444);

    //���C�g�̉e�����󂯂Ȃ�����
    DWORD ambient;
    pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
    pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
    pDevice->LightEnable(0, FALSE);

    for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        if (g_aItem[nCntItem].bUse == true)
        {
            //���[���h�}�g���N�X�̏�����
            D3DXMatrixIdentity(&g_aItem[nCntItem].mtxWorld);
            //��]�̋t�s��𐶐�
            pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
            D3DXMatrixInverse(&g_aItem[nCntItem].mtxWorld, NULL, &mtxRot);
            g_aItem[nCntItem].mtxWorld._41 = 0;
            g_aItem[nCntItem].mtxWorld._42 = 0;
            g_aItem[nCntItem].mtxWorld._43 = 0;

            ////�ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans,
                g_aItem[nCntItem].pos.x,
                g_aItem[nCntItem].pos.y,
                g_aItem[nCntItem].pos.z);

            D3DXMatrixMultiply(
                &g_aItem[nCntItem].mtxWorld,
                &g_aItem[nCntItem].mtxWorld,
                &mtxTrans);

            //���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_aItem[nCntItem].mtxWorld);

            pDevice->SetTexture(0, g_pTextureItem[g_aItem[nCntItem].type]);
            //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
            pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_3D));
            //���_�t�H�[�}�b�g�̐ݒ�
            pDevice->SetFVF(FVF_VERTEX_3D);

            //�|���S���̕`��
            pDevice->DrawPrimitive(
                D3DPT_TRIANGLESTRIP,
                nCntItem * 4,
                NUM_POLYGON);
        }
    }

    //�A���t�@�e�X�g�𖳌���
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    //���C�g�̉e�����󂯂�悤�ɂ���
    pDevice->SetRenderState(D3DRS_AMBIENT, ambient);
    pDevice->LightEnable(0, TRUE);

    //�e�N�X�`���̐ݒ�̉���
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
//�A�C�e���̐ݒ�
//=============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, ITEMTYPE type)
{
    VERTEX_3D *pVtx;//���_���̃|�C���^
    //PLAYER *pPlayer;
    //pPlayer = GetPlayer();

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {


        if (/*pPlayer->pos.x > pos.x - CAMERA_VIEW_RANGE&&
            pPlayer->pos.x < pos.x + CAMERA_VIEW_RANGE&&
            pPlayer->pos.z > pos.z - CAMERA_VIEW_RANGE&&
            pPlayer->pos.z < pos.z + CAMERA_VIEW_RANGE&&*/
            g_aItem[nCntItem].bUse == false)
        {
            //������
            g_aItem[nCntItem].pos = pos;
            g_aItem[nCntItem].size = size;
            g_aItem[nCntItem].move = move;
            //g_aItem[nCntItem].col = col;
            g_aItem[nCntItem].type = type;
            g_aItem[nCntItem].bUse = true;//�g�p���Ă����Ԃɂ���

            //���_���W�̐ݒ�
            pVtx[0].pos = D3DXVECTOR3(-(g_aItem[nCntItem].size.x / 2), (g_aItem[nCntItem].size.y / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3((g_aItem[nCntItem].size.x / 2), (g_aItem[nCntItem].size.y / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(-(g_aItem[nCntItem].size.x / 2), -(g_aItem[nCntItem].size.y / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3((g_aItem[nCntItem].size.x / 2), -(g_aItem[nCntItem].size.y / 2), 0.0f);

            break;
        }


        pVtx += 4;  //���_�f�[�^��4���i�߂�
    }
    //���_�f�[�^���A�����b�N����
    g_pVtxBuffItem->Unlock();
}

//=============================================================================
//�A�C�e���̎擾
//=============================================================================
ITEM*GetItem(void)
{
    return &g_aItem[0];
}

//=====================================================
// �A�C�e���̓����蔻��̐ݒ�
//=====================================================
void CollisionItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntItem;					// ���[�J���ϐ�

    for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        // �A�C�e�����g��ꂽ��
        if (g_aItem[nCntItem].bUse == true)
        {
            if (RectangleCollision(pPos, pPosOld, size, &g_aItem[nCntItem].pos, g_aItem[nCntItem].size) == true)
            {
                float fAngle = 0.0f;    //�p�x
                float fSpeedY = 0.0f;   //����
                float fRadius = 50.0f; //�G�t�F�N�g���a
                PLAYER*pPlayer = GetPlayer();
                //�n�[�g
                switch (g_aItem[nCntItem].type)
                {
                case ITEMTYPE_000:
                    //�񕜃G�t�F�N�g
                    pPlayer->fLife += 125.0f;    //��
                    ScoreCalculation(1000);     //�X�R�A���Z
                    for (int nCnt = 0; nCnt < 200; nCnt++)
                    {
                        fSpeedY = float(rand() % 800 + 300) / 100.0f;
                        //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                        D3DXVECTOR3 pos = D3DXVECTOR3(pPlayer->pos.x - sinf(fAngle)*fRadius, pPlayer->pos.y, pPlayer->pos.z - cosf(fAngle)*fRadius);
                        D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                        SetEffect2(pos, move, EFFECT2_SIZE_HEAL, EFFECT2_INIT_COLOR_HEAL, EFFECT2TYPE_HEAL); //�񕜃G�t�F�N�g
                    }
                    break;

                case ITEMTYPE_001:
                    //�L���L���G�t�F�N�g
                    ScoreCalculation(5000);     //�X�R�A���Z
                    for (int nCnt = 0; nCnt < 200; nCnt++)
                    {
                        fSpeedY = float(rand() % 800 + 300) / 100.0f;
                        //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                        D3DXVECTOR3 pos = D3DXVECTOR3(pPlayer->pos.x - sinf(fAngle)*fRadius, pPlayer->pos.y, pPlayer->pos.z - cosf(fAngle)*fRadius);
                        D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                        SetEffect2(pos, move, EFFECT2_SIZE_HEAL, EFFECT2_INIT_COLOR_HEAL, EFFECT2TYPE_RARE); //�L���L���G�t�F�N�g
                    }
                    break;
                }

                g_aItem[nCntItem].bUse = false;
            }
        }
    }
}