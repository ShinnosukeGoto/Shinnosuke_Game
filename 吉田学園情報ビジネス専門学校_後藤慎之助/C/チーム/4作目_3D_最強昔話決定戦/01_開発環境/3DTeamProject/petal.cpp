//=============================================================================
//
// �ŋ��̘b�����[petal.cpp]�̏��� (�Ԃт�j
// Author :�㓡�T�V��
//
//=============================================================================
#include "petal.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePetal[PETALTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPetal = NULL;
PETAL g_aPetal[MAX_PETAL];						//�Ԃт�̏��

//=============================================================================
// �Ԃт珉��������
// Author:�㓡�T�V��
//=============================================================================
void InitPetal(void)
{
    //�錾
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/petal.png", &g_pTexturePetal[PETALTYPE_000]);	//���̉Ԃт�

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_PETAL, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPetal, NULL);
    VERTEX_2D *pVtx = NULL;	//���_���ւ̃|�C���^

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffPetal->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

    //�Ԃт��pVtx�𐶐�
    for (int nCntPetal = 0; nCntPetal < MAX_PETAL; nCntPetal++)
    {
        g_aPetal[nCntPetal].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aPetal[nCntPetal].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aPetal[nCntPetal].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        g_aPetal[nCntPetal].fSize = 0.0f;
        g_aPetal[nCntPetal].fAngle = 0.0f;
        g_aPetal[nCntPetal].fRotSpeed = 0.0f;
        g_aPetal[nCntPetal].type = PETALTYPE_000;
        g_aPetal[nCntPetal].bUse = false;

        g_aPetal[nCntPetal].vertex1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aPetal[nCntPetal].vertex2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aPetal[nCntPetal].vertex3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aPetal[nCntPetal].vertex4 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

        //���_���W�̐ݒ�i�E���Őݒ肷��j
        pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

        //rhw�̐ݒ�i�l��1.0�Őݒ肷��j
        pVtx[0].rhw = 1.0f;
        pVtx[1].rhw = 1.0f;
        pVtx[2].rhw = 1.0f;
        pVtx[3].rhw = 1.0f;

        //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
        pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

        //�e�N�X�`�����W�̐ݒ�
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffPetal->Unlock();
}

//=============================================================================
// �Ԃт�I������
// Author:�㓡�T�V��
//=============================================================================
void UninitPetal(void)
{
    //�e�N�X�`���̊J��
    for (int nCntPetal = 0; nCntPetal < PETALTYPE_MAX; nCntPetal++)
    {
        if (g_pTexturePetal[nCntPetal] != NULL)
        {
            g_pTexturePetal[nCntPetal]->Release();
            g_pTexturePetal[nCntPetal] = NULL;
        }
    }

    //���_�o�b�t�@�̊J��
    if (g_pVtxBuffPetal != NULL)
    {
        g_pVtxBuffPetal->Release();
        g_pVtxBuffPetal = NULL;
    }
}

//=============================================================================
// �Ԃт�X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdatePetal(void)
{
    //�錾
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    VERTEX_2D *pVtx = NULL;

    //���_�o�b�t�@�����b�N
    g_pVtxBuffPetal->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntPetal = 0; nCntPetal < MAX_PETAL; nCntPetal++)
    {
        //�g���Ă���Ȃ�
        if (g_aPetal[nCntPetal].bUse == true)
        {
            //�ړ��ʂƈʒu��R�Â�
            g_aPetal[nCntPetal].pos += g_aPetal[nCntPetal].move;

            //�p�x�ɁA��]���x�𑫂�
            g_aPetal[nCntPetal].fAngle += g_aPetal[nCntPetal].fRotSpeed;

            //==========================================================================================================
            //�摜���A�摜�̒��S�����ɉ�]������
            //����̒��_
            g_aPetal[nCntPetal].vertex1.x = -(g_aPetal[nCntPetal].fSize / 2)*cosf(g_aPetal[nCntPetal].fAngle)
                - ( - (g_aPetal[nCntPetal].fSize / 2))*sinf(g_aPetal[nCntPetal].fAngle);
            g_aPetal[nCntPetal].vertex1.y = -(g_aPetal[nCntPetal].fSize / 2)*sinf(g_aPetal[nCntPetal].fAngle)
                + ( - (g_aPetal[nCntPetal].fSize / 2))*cosf(g_aPetal[nCntPetal].fAngle);

            //�E��̒��_
            g_aPetal[nCntPetal].vertex2.x = (g_aPetal[nCntPetal].fSize / 2)*cosf(g_aPetal[nCntPetal].fAngle)
                - ( - (g_aPetal[nCntPetal].fSize / 2))*sinf(g_aPetal[nCntPetal].fAngle);
            g_aPetal[nCntPetal].vertex2.y = (g_aPetal[nCntPetal].fSize / 2)*sinf(g_aPetal[nCntPetal].fAngle)
                + ( - (g_aPetal[nCntPetal].fSize / 2))*cosf(g_aPetal[nCntPetal].fAngle);

            //�����̒��_
            g_aPetal[nCntPetal].vertex3.x = -(g_aPetal[nCntPetal].fSize / 2)*cosf(g_aPetal[nCntPetal].fAngle)
                - (g_aPetal[nCntPetal].fSize / 2)*sinf(g_aPetal[nCntPetal].fAngle);
            g_aPetal[nCntPetal].vertex3.y = -(g_aPetal[nCntPetal].fSize / 2)*sinf(g_aPetal[nCntPetal].fAngle)
                + (g_aPetal[nCntPetal].fSize / 2)*cosf(g_aPetal[nCntPetal].fAngle);

            //�E���̒��_
            g_aPetal[nCntPetal].vertex4.x = (g_aPetal[nCntPetal].fSize / 2)*cosf(g_aPetal[nCntPetal].fAngle)
                - (g_aPetal[nCntPetal].fSize / 2)*sinf(g_aPetal[nCntPetal].fAngle);
            g_aPetal[nCntPetal].vertex4.y = (g_aPetal[nCntPetal].fSize / 2)*sinf(g_aPetal[nCntPetal].fAngle)
                + (g_aPetal[nCntPetal].fSize / 2)*cosf(g_aPetal[nCntPetal].fAngle);
            //==========================================================================================================

            //���_���̍X�V
            pVtx[0].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x, g_aPetal[nCntPetal].pos.y , 0.0f) + g_aPetal[nCntPetal].vertex1;
            pVtx[1].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x, g_aPetal[nCntPetal].pos.y , 0.0f) + g_aPetal[nCntPetal].vertex2;
            pVtx[2].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x, g_aPetal[nCntPetal].pos.y , 0.0f) + g_aPetal[nCntPetal].vertex3;
            pVtx[3].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x, g_aPetal[nCntPetal].pos.y , 0.0f) + g_aPetal[nCntPetal].vertex4;

            //���_�J���[
            pVtx[0].col = g_aPetal[nCntPetal].col;
            pVtx[1].col = g_aPetal[nCntPetal].col;
            pVtx[2].col = g_aPetal[nCntPetal].col;
            pVtx[3].col = g_aPetal[nCntPetal].col;

            //��ʊO�Ŗ��g�p��
            switch (g_aPetal[nCntPetal].type)
            {
                //�Ԃт�̏ꍇ
            case PETALTYPE_000:
                //��
                if (g_aPetal[nCntPetal].pos.x < -g_aPetal[nCntPetal].fSize)
                {
                    g_aPetal[nCntPetal].bUse = false;
                }

                //��
                if (g_aPetal[nCntPetal].pos.y > SCREEN_HEIGHT + g_aPetal[nCntPetal].fSize)
                {
                    g_aPetal[nCntPetal].bUse = false;
                }

                break;
            }
        }
        pVtx += 4;
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffPetal->Unlock();
}

//=============================================================================
// �Ԃт�`�揈��
// Author:�㓡�T�V��
//=============================================================================
void DrawPetal(void)
{
    //�錾
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffPetal, 0, sizeof(VERTEX_2D));

    //���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF(FVF_VERTEX_2D);

    //�|���S���̕`��
    for (int nCntPetal = 0; nCntPetal < MAX_PETAL; nCntPetal++)
    {
        if (g_aPetal[nCntPetal].bUse == true)	//�Ԃт炪�g�p���Ȃ�`��
        {
            //�e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, g_pTexturePetal[g_aPetal[nCntPetal].type]);

            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPetal * 4, 2);
        }
    }
}

//=============================================================================
// �Ԃт�ݒ菈��
// Author:�㓡�T�V��
//=============================================================================
void SetPetal(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSize, float fAngle, float fRotSpeed, PETALTYPE type)
{
    //�ϐ��錾
    VERTEX_2D *pVtx = NULL;

    //���_�o�b�t�@�����b�N
    g_pVtxBuffPetal->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntPetal = 0; nCntPetal < MAX_PETAL; nCntPetal++)
    {
        if (g_aPetal[nCntPetal].bUse == false)
        {
            g_aPetal[nCntPetal].pos = pos;
            g_aPetal[nCntPetal].move = move;
            g_aPetal[nCntPetal].col = col;
            g_aPetal[nCntPetal].fSize = fSize;
            g_aPetal[nCntPetal].fAngle = fAngle;
            g_aPetal[nCntPetal].fRotSpeed = fRotSpeed;
            g_aPetal[nCntPetal].type = type;

            //���_�ɑ΂���\���̂̏��̏�����
            g_aPetal[nCntPetal].vertex1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aPetal[nCntPetal].vertex2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aPetal[nCntPetal].vertex3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aPetal[nCntPetal].vertex4 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

            //���_���̍X�V
            //���_���W
            pVtx[0].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x - (g_aPetal[nCntPetal].fSize / 2), g_aPetal[nCntPetal].pos.y - (g_aPetal[nCntPetal].fSize / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x + (g_aPetal[nCntPetal].fSize / 2), g_aPetal[nCntPetal].pos.y - (g_aPetal[nCntPetal].fSize / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x - (g_aPetal[nCntPetal].fSize / 2), g_aPetal[nCntPetal].pos.y + (g_aPetal[nCntPetal].fSize / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x + (g_aPetal[nCntPetal].fSize / 2), g_aPetal[nCntPetal].pos.y + (g_aPetal[nCntPetal].fSize / 2), 0.0f);

            g_aPetal[nCntPetal].bUse = true;

            break;
        }
        pVtx += 4;
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffPetal->Unlock();
}