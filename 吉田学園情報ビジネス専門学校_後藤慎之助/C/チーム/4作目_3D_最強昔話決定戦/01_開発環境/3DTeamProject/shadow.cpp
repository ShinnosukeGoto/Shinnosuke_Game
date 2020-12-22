//=============================================================================
//
// �|���S���̉e���� [shadow.cpp]
// Author:KUDO AYANO
//
//=============================================================================
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SHADOW 512
#define SHADOW_TEXTUER "TEXTURE/shadow.png"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;		//�|���S���̃e�N�X�`���ւ̃|�C���^
SHADOW g_Shadow[MAX_SHADOW];//�e�̏��

//=============================================================================
// ����������
// Author:KUDO AYANO
//=============================================================================
void InitShadow(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

    //���_�o�b�t�@�̐���
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,		//���_�t�H�[�}�b�g
        D3DPOOL_MANAGED,//�i�Œ�j
        &g_pVtxBuffShadow,//�ϐ������ς��ƕύX���K�v
        NULL);

    D3DXCreateTextureFromFile(pDevice, SHADOW_TEXTUER, &g_pTextureShadow);//�e�N�X�`���̓ǂݍ���

    VERTEX_3D *pVtx = NULL;//���_���̃|�C���^

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����
    for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
    {
        //�\���̂̏�����
        g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Shadow[nCntShadow].fDiameter = 0.0f;
        g_Shadow[nCntShadow].fDiameterMemory = 0.0f;
        g_Shadow[nCntShadow].fHeight = 0.0f;
        g_Shadow[nCntShadow].fHeightOld = 0.0f;
        g_Shadow[nCntShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_ALPHA_MAX);
        g_Shadow[nCntShadow].use = false;

        //���_���W�̐ݒ�
        pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

        //�@���x�N�g��
        pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

        //���_�J���[�̐ݒ�
        pVtx[0].col = g_Shadow[nCntShadow].col;
        pVtx[1].col = g_Shadow[nCntShadow].col;
        pVtx[2].col = g_Shadow[nCntShadow].col;
        pVtx[3].col = g_Shadow[nCntShadow].col;

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
// �X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateShadow(void)
{
    VERTEX_3D *pVtx = NULL;//���_���̃|�C���^

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
    {
        //�g���Ă���Ȃ�
        if (g_Shadow[nCntShadow].use == true)
        {
            //�n�ʂɉe������
            g_Shadow[nCntShadow].pos.y = SHADOW_POS_Y;

            //�����ɉ����āA�e�̑傫���Ɠ����x��ς���
            //���~��
            if (g_Shadow[nCntShadow].fHeight < g_Shadow[nCntShadow].fHeightOld)
            {
                g_Shadow[nCntShadow].fDiameter = g_Shadow[nCntShadow].fDiameter + g_Shadow[nCntShadow].fHeight / SHADOW_SIZE_RATE;
                g_Shadow[nCntShadow].col.a += SHADOW_ALPHA_RATE;
            }
            //��~��
            else if (g_Shadow[nCntShadow].fHeight > g_Shadow[nCntShadow].fHeightOld)
            {
                g_Shadow[nCntShadow].fDiameter = g_Shadow[nCntShadow].fDiameter - g_Shadow[nCntShadow].fHeight / SHADOW_SIZE_RATE;
                g_Shadow[nCntShadow].col.a -= SHADOW_ALPHA_RATE;
            }
            //�����Ȃ�
            else if (g_Shadow[nCntShadow].fHeight == g_Shadow[nCntShadow].fHeightOld)
            {
                //�n�ʂ���̍�����0
                if (g_Shadow[nCntShadow].fHeight == 0.0f)
                {
                    g_Shadow[nCntShadow].fDiameter = g_Shadow[nCntShadow].fDiameterMemory;  //���̑傫���ɂ���
                    g_Shadow[nCntShadow].col.a = SHADOW_ALPHA_MAX;
                }
            }

            //�傫���̏���A����
            //���
            if (g_Shadow[nCntShadow].fDiameter > g_Shadow[nCntShadow].fDiameterMemory)
            {
                g_Shadow[nCntShadow].fDiameter = g_Shadow[nCntShadow].fDiameterMemory;  //���̑傫���ɂ���
            }
            //����
            if (g_Shadow[nCntShadow].fDiameter < 0.0f)
            {
                g_Shadow[nCntShadow].fDiameter = 0.0f;  //0�������Ȃ��悤�ɂ���
            }

            //�����x�̏���A����
            //���
            if (g_Shadow[nCntShadow].col.a > SHADOW_ALPHA_MAX)
            {
                g_Shadow[nCntShadow].col.a = SHADOW_ALPHA_MAX;
            }
            //����
            if (g_Shadow[nCntShadow].col.a < SHADOW_ALPHA_MIN)
            {
                g_Shadow[nCntShadow].col.a = SHADOW_ALPHA_MIN;
            }

            //1F�O�̍������L�^
            g_Shadow[nCntShadow].fHeightOld = g_Shadow[nCntShadow].fHeight;

            //���_���W�̐ݒ�
            pVtx[0].pos = D3DXVECTOR3(-(g_Shadow[nCntShadow].fDiameter / 2), 0.0f, (g_Shadow[nCntShadow].fDiameter / 2));
            pVtx[1].pos = D3DXVECTOR3((g_Shadow[nCntShadow].fDiameter / 2), 0.0f, (g_Shadow[nCntShadow].fDiameter / 2));
            pVtx[2].pos = D3DXVECTOR3(-(g_Shadow[nCntShadow].fDiameter / 2), 0.0f, -(g_Shadow[nCntShadow].fDiameter / 2));
            pVtx[3].pos = D3DXVECTOR3((g_Shadow[nCntShadow].fDiameter / 2), 0.0f, -(g_Shadow[nCntShadow].fDiameter / 2));

            //���_�J���[�̐ݒ�
            pVtx[0].col = g_Shadow[nCntShadow].col;
            pVtx[1].col = g_Shadow[nCntShadow].col;
            pVtx[2].col = g_Shadow[nCntShadow].col;
            pVtx[3].col = g_Shadow[nCntShadow].col;
        }

        pVtx += 4;  //���_�f�[�^��4���i�߂�
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// �e�̐ݒ�
// Author:�㓡�T�V��
//=============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fDiameter, float fHeight ,D3DXCOLOR col)
{
    VERTEX_3D *pVtx = NULL;//���_���̃|�C���^

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

    int nCntShadow = 0;
    for (; nCntShadow < MAX_SHADOW; nCntShadow++)
    {
        if (!g_Shadow[nCntShadow].use)
        {
            g_Shadow[nCntShadow].use = true;
            g_Shadow[nCntShadow].pos = pos;
            g_Shadow[nCntShadow].rot = rot;
            g_Shadow[nCntShadow].fDiameter = fDiameter;
            g_Shadow[nCntShadow].fDiameterMemory = fDiameter;
            g_Shadow[nCntShadow].fHeight = fHeight;
            g_Shadow[nCntShadow].fHeightOld = fHeight;
            g_Shadow[nCntShadow].col.a = col.a;     //�����x�����т���

            //���_���W�̐ݒ�
            pVtx[0].pos = D3DXVECTOR3(-(g_Shadow[nCntShadow].fDiameter / 2), 0.0f, (g_Shadow[nCntShadow].fDiameter / 2));
            pVtx[1].pos = D3DXVECTOR3((g_Shadow[nCntShadow].fDiameter / 2), 0.0f, (g_Shadow[nCntShadow].fDiameter / 2));
            pVtx[2].pos = D3DXVECTOR3(-(g_Shadow[nCntShadow].fDiameter / 2), 0.0f, -(g_Shadow[nCntShadow].fDiameter / 2));
            pVtx[3].pos = D3DXVECTOR3((g_Shadow[nCntShadow].fDiameter / 2), 0.0f, -(g_Shadow[nCntShadow].fDiameter / 2));

            break;
        }

        pVtx += 4;  //���_�f�[�^��4���i�߂�
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffShadow->Unlock();

    return nCntShadow;
}

//=============================================================================
// �e�̏��̑��M
// Author:KUDO AYANO
//=============================================================================
SHADOW *GetShadow()
{
    return &g_Shadow[0];
}

//=============================================================================
// �e�̐ݒ�
// Author:KUDO AYANO
//=============================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fHeight)
{
    //�����ȊO��R�Â�
    g_Shadow[nIdxShadow].pos.x = pos.x;
    g_Shadow[nIdxShadow].pos.z = pos.z;

    g_Shadow[nIdxShadow].fHeight = fHeight;
}

//=============================================================================
// �`�揈��
// Author:KUDO AYANO
//=============================================================================
void DrawShadow(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^���擾
    D3DXMATRIX mtxRot, mtxTrans;//�s��v�Z�p�̃}�g���N�X
    for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
    {
        if (g_Shadow[nCntShadow].use == true)
        {
            //���Z�����ɐݒ�
            pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
            pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

            pDevice->SetTexture(0, g_pTextureShadow);

            //���[���h�}�g���N�X�̏�����
            D3DXMatrixIdentity(&g_Shadow[nCntShadow].mtxWorld);
            //�����𔽓]
            D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Shadow[nCntShadow].rot.y, g_Shadow[nCntShadow].rot.x, g_Shadow[nCntShadow].rot.z);
            D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorld, &g_Shadow[nCntShadow].mtxWorld, &mtxRot);
            //�ʒu�𔽓]
            D3DXMatrixTranslation(&mtxTrans, g_Shadow[nCntShadow].pos.x, g_Shadow[nCntShadow].pos.y, g_Shadow[nCntShadow].pos.z);
            D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorld, &g_Shadow[nCntShadow].mtxWorld, &mtxTrans);
            //���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_Shadow[nCntShadow].mtxWorld);
            //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
            pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));
            //���_�t�H�[�}�b�g�̐ݒ�
            pDevice->SetFVF(FVF_VERTEX_3D);
            //�|���S���̕`��
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntShadow, NUM_POLYGON);
            //�����_�[�X�e�[�g�ݒ�����Ƃɖ߂�
            pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
            pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        }
    }

    //�e�N�X�`���̐ݒ�̉���
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// �I������
// Author:KUDO AYANO
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

