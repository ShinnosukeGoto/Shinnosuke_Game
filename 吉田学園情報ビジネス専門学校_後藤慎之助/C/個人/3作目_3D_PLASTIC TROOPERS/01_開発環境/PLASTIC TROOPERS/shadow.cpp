//=============================================================================
//
// 3D�Q�[���̂ЂȌ`�̏���[sahdow.cpp]
// Author:�㓡�T�V��
//
//=============================================================================
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SHADOW_SIZE (8.0f)
#define MAX_SHADOW (2)
#define SHADOW_TEXTUER "TEXTURE/shadow.png"
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;		//�|���S���̃e�N�X�`���ւ̃|�C���^
SHADOW g_Shadow[MAX_SHADOW];//�e�̏��

                            //=============================================================================
                            //����������
                            //=============================================================================
void InitShadow(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
    for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
    {
        g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Shadow[nCntShadow].bUse = false;
    }

    D3DXCreateTextureFromFile(pDevice, SHADOW_TEXTUER, &g_pTextureShadow);//�e�N�X�`���̓ǂݍ���

                                                                          //���_�o�b�t�@�̐���
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_SHADOW,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,		//���_�t�H�[�}�b�g
        D3DPOOL_MANAGED,//�i�Œ�j
        &g_pVtxBuffShadow,//�ϐ������ς��ƕύX���K�v
        NULL);

    VERTEX_3D *pVtx;//���_���̃|�C���^

                    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
    {
        //���_���W�̏������i�E���Őݒ肷��j
        pVtx[0].pos = D3DXVECTOR3(0.0f - SHADOW_SIZE, 0.0f, 0.0f + SHADOW_SIZE);
        pVtx[1].pos = D3DXVECTOR3(0.0f + SHADOW_SIZE, 0.0f, 0.0f + SHADOW_SIZE);
        pVtx[2].pos = D3DXVECTOR3(0.0f - SHADOW_SIZE, 0.0f, 0.0f - SHADOW_SIZE);
        pVtx[3].pos = D3DXVECTOR3(0.0f + SHADOW_SIZE, 0.0f, 0.0f - SHADOW_SIZE);

        //�@���x�N�g��
        pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

        //���_�J���[�̐ݒ�
        pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;
    }
    //���_�f�[�^�A�����b�N
    g_pVtxBuffShadow->Unlock();
}
//=============================================================================
//�X�V����
//=============================================================================
void UpdateShadow(void)
{

}
//=============================================================================
//�e�̐ݒ�
//=============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    int nCntShadow = 0;
    for (; nCntShadow < MAX_SHADOW; nCntShadow++)
    {
        if (g_Shadow[nCntShadow].bUse == false)
        {

            g_Shadow[nCntShadow].bUse = true;
            g_Shadow[nCntShadow].pos = pos;
            g_Shadow[nCntShadow].rot = rot;
            break;
        }
    }
    return nCntShadow;
}
//=============================================================================
//�e�̐ݒ�
//model.cpp�ŌĂяo��
//=============================================================================
void SetPositionShadow(int nldxShadow, D3DXVECTOR3 pos)
{
    g_Shadow[nldxShadow].pos = pos;
}
//=============================================================================
//�`�揈��
//=============================================================================
void DrawShadow(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^���擾
    D3DXMATRIX mtxRot, mtxTrans;//�s��v�Z�p�̃}�g���N�X

    //���Z�����ɐݒ�
    pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    //�e�N�X�`���̐ݒ�
    pDevice->SetTexture(0, g_pTextureShadow);

    for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
    {

        if (g_Shadow[nCntShadow].bUse == true)
        {
            //���[���h�}�g���N�X�̏�����
            D3DXMatrixIdentity(&g_Shadow[nCntShadow].mtxWorld);
            //�����𔽉f
            D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Shadow[nCntShadow].rot.y, g_Shadow[nCntShadow].rot.x, g_Shadow[nCntShadow].rot.z);
            D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorld, &g_Shadow[nCntShadow].mtxWorld, &mtxRot);
            //�ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans, g_Shadow[nCntShadow].pos.x, g_Shadow[nCntShadow].pos.y, g_Shadow[nCntShadow].pos.z);
            D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorld, &g_Shadow[nCntShadow].mtxWorld, &mtxTrans);
            //���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_Shadow[nCntShadow].mtxWorld);
            //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
            pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));
            //���_�t�H�[�}�b�g�̐ݒ�
            pDevice->SetFVF(FVF_VERTEX_3D);
            //�|���S���̕`��
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
        }
    }

    //�����_�[�X�e�[�g�ݒ�����Ƃɖ߂�
    pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    //�e�N�X�`���̐ݒ�̉���
    pDevice->SetTexture(0, NULL);
}
//=============================================================================
//�I������
//=============================================================================
void UninitShadow(void)
{
    //���_�o�b�t�@�̊J��
    if (g_pVtxBuffShadow != NULL)
    {
        g_pVtxBuffShadow->Release();
        g_pVtxBuffShadow = NULL;
    }
    //�e�N�X�`���̊J��
    if (g_pTextureShadow != NULL)
    {
        g_pTextureShadow->Release();
        g_pTextureShadow = NULL;
    }
}