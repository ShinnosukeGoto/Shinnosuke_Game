//=============================================================================
//
// PLASTIC TROOPERS�̏��� [bg.cpp] (�F���̔w�i)
// Author : �㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "input.h"
#include "bg.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================

//���b�V���t�B�[���h�̃e�N�X�`��
LPDIRECT3DTEXTURE9 g_pTextureBg = NULL;
//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;
//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffBg = NULL;

//���b�V���t�B�[���h�̃��[���h�}�g���b�N�X
D3DXMATRIX g_mtxWorldBg;

BG g_Bg;//�t�B�[���h�̏��

//=============================================================================
//�o�b�N�O���E���h����������
//=============================================================================
void InitBg(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    g_Bg.nBgVertexNUM = (g_Bg.nBgBlockX + 1) * (g_Bg.nBgBlockZ + 1);
    g_Bg.nBgIndexNUM = ((g_Bg.nBgBlockX + 1) * 2) * g_Bg.nBgBlockZ + ((g_Bg.nBgBlockZ - 1) * 2);
    g_Bg.nBgPrimitiveNUM = ((g_Bg.nBgBlockX * g_Bg.nBgBlockZ) * 2) + ((g_Bg.nBgBlockZ - 1) * 4);

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice,
        "TEXTURE/�F��.jpg",//�t�@�C���̖��O
        &g_pTextureBg);
    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_3D)* g_Bg.nBgVertexNUM,//�o�b�t�@�T�C�Y
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffBg,
        NULL);//���_���ւ̃|�C���^

    VERTEX_3D *pVtx;//���_���̃|�C���^

                    //���_�o�b�t�@�����b�N
    g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

    int nCntPos = 0;
    int nRadius = 50;
    float fAngle = 360.0f / g_Bg.nBgBlockX;
    float fRadiusAngle = 0;
    float fRadius = 0;
    float fRadiusY = 0;

    for (int nCntMeshZ = 0; nCntMeshZ < g_Bg.nBgBlockZ + 1; nCntMeshZ++)
    {
        fRadiusAngle = 90 - (180 / g_Bg.nBgBlockZ) * nCntMeshZ;
        fRadius = cosf(D3DXToRadian(fRadiusAngle)) * BG_SIZE;
        fRadiusY = sinf(D3DXToRadian(fRadiusAngle)) * BG_SIZE;

        for (int nCntMeshX = 0; nCntMeshX < g_Bg.nBgBlockX + 1; nCntMeshX++)
        {
            //pVtx[nCntPos].pos = D3DXVECTOR3(sinf(D3DXToRadian(fAngle * nCntMeshX)) * Bg_SIZE, nRadius * nCntMeshZ, cosf(D3DXToRadian(fAngle * nCntMeshX)) * Bg_SIZE);

            pVtx[nCntPos].pos = D3DXVECTOR3((sinf(D3DXToRadian(fAngle * nCntMeshX)) * fRadius), fRadiusY, (cosf(D3DXToRadian(fAngle * nCntMeshX)))* fRadius);
            nCntPos++;
        }
    }

    //�@���̐ݒ�
    for (int nCntnor = 0; nCntnor < g_Bg.nBgVertexNUM; nCntnor++)
    {
        //���_�J���[�̐ݒ�
        pVtx[nCntnor].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    }

    //�e�N�X�`�����W�̐ݒ�
    int nCntTex = 0;

    for (int nCntMeshY = 0; nCntMeshY< g_Bg.nBgBlockZ + 1; nCntMeshY++)
    {
        for (int nCntMeshX = 0; nCntMeshX < g_Bg.nBgBlockX + 1; nCntMeshX++)
        {
            pVtx[nCntTex].tex = D3DXVECTOR2(nCntMeshX, nCntMeshY);

            nCntTex++;
        }
    }

    for (int nCntcol = 0; nCntcol < g_Bg.nBgVertexNUM; nCntcol++)
    {
        //���_�J���[�̐ݒ�
        pVtx[nCntcol].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffBg->Unlock();

    int nCntIndex = 0;
    int nCntMeshX;
    int nCntMeshZ;

    pDevice->CreateIndexBuffer(
        sizeof(WORD) * g_Bg.nBgIndexNUM,//�C���f�b�N�X�̐�
        D3DUSAGE_WRITEONLY,
        D3DFMT_INDEX16,
        D3DPOOL_MANAGED,
        &g_pIdxBuffBg,
        NULL);//���_���ւ̃|�C���^

              //�C���f�b�N�X�o�b�t�@�����b�N
    WORD* pIdx;
    g_pIdxBuffBg->Lock(0, 0, (void**)&pIdx, 0);

    for (nCntMeshZ = 0; nCntMeshZ < g_Bg.nBgBlockZ; nCntMeshZ++)
    {
        if (nCntMeshZ != 0)
        {
            pIdx[nCntIndex] = nCntMeshX + (g_Bg.nBgBlockX + 1) * (nCntMeshZ);

            nCntIndex++;
        }

        for (nCntMeshX = 0; nCntMeshX < g_Bg.nBgBlockX + 1; nCntMeshX++)
        {
            pIdx[nCntIndex] = nCntMeshX + (g_Bg.nBgBlockX + 1) * (nCntMeshZ + 1);

            nCntIndex++;

            pIdx[nCntIndex] = nCntMeshX + (g_Bg.nBgBlockX + 1) * (nCntMeshZ);

            nCntIndex++;
        }

        if (nCntMeshZ != g_Bg.nBgBlockZ - 1)
        {
            pIdx[nCntIndex] = nCntMeshX - 1 + (g_Bg.nBgBlockX + 1) * (nCntMeshZ);

            nCntIndex++;
        }
    }

    //�C���f�b�N�X�o�b�t�@�̃A�����b�N
    g_pIdxBuffBg->Unlock();
}

//=============================================================================
// �o�b�N�O���E���h�I������
//=============================================================================
void UninitBg(void)
{
    //�e�N�X�`���̉��
    if (g_pTextureBg != NULL)
    {
        g_pTextureBg->Release();
        g_pTextureBg = NULL;
    }
    //���_�o�b�t�@�̊J��
    if (g_pVtxBuffBg != NULL)
    {
        g_pVtxBuffBg->Release();
        g_pVtxBuffBg = NULL;
    }
    //�C���f�b�N�X�o�b�t�@�̊J��
    if (g_pIdxBuffBg != NULL)
    {
        g_pIdxBuffBg->Release();
        g_pIdxBuffBg = NULL;
    }
}

//=============================================================================
//�o�b�N�O���E���h�X�V����
//=============================================================================
void UpdateBg(void)
{
    g_Bg.rot.y += BG_ROT_SPEED;
}


//=============================================================================
//�o�b�N�O���E���h�`�揈��
//=============================================================================
void DrawBg(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�|�C���^
    D3DXMATRIX mtxRot, mtxTrans;//�s��v�Z�p�̃}�g���N�X

    //�A���t�@�e�X�g��L����
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //�A���t�@�e�X�g��l�̐ݒ�
    pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    //�A���t�@�e�X�g�̔�r���@�̐ݒ�iGREATER�͊�l���傫���ꍇ)
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //���[���h�}�g���N�X�̏�����
    D3DXMatrixIdentity(&g_mtxWorldBg);
    //�����𔽉f
    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Bg.rot.y, g_Bg.rot.x, g_Bg.rot.z);
    D3DXMatrixMultiply(&g_mtxWorldBg, &g_mtxWorldBg, &mtxRot);
    //�ʒu�𔽉f
    D3DXMatrixTranslation(&mtxTrans, g_Bg.pos.x, g_Bg.pos.y, g_Bg.pos.z);
    D3DXMatrixMultiply(&g_mtxWorldBg, &g_mtxWorldBg, &mtxTrans);
    //���[���h�}�g���b�N�X�̐ݒ�
    pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBg);
    pDevice->SetTexture(0, g_pTextureBg);
    //�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
    pDevice->SetIndices(g_pIdxBuffBg);
    //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
    pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_3D));
    //���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF(FVF_VERTEX_3D);
    //�|���S���̕`��
    pDevice->DrawIndexedPrimitive(
        D3DPT_TRIANGLESTRIP,
        0, 0,
        g_Bg.nBgVertexNUM,//���_��
        0,
        g_Bg.nBgPrimitiveNUM//�|���S����
    );

    //�A���t�@�e�X�g�𖳌���
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}