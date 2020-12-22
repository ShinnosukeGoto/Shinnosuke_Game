//=============================================================================
//
// PLASTIC TROOPERS [effect.cpp]�̏��� (�����Ȃǂ̃G�t�F�N�g)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "effect.h"
#include "collision.h"
#include "player.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureEffect[EFFECTTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;//���_�o�b�t�@�ւ̃|�C���^
EFFECT g_Effect[MAX_EFFECT];//�A�C�e���̏��

//=============================================================================
//�G�t�F�N�g�̏���������
//=============================================================================
void InitEffect(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

    //���_�o�b�t�@�̐���
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EFFECT,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,		//���_�t�H�[�}�b�g
        D3DPOOL_MANAGED,//�i�Œ�j
        &g_pVtxBuffEffect,//�ϐ������ς��ƕύX���K�v
        NULL);

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/Lock-On.png", &g_pTextureEffect[0]);  //�^�[�Q�b�g
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/lost.png", &g_pTextureEffect[1]);     //�G�i�W�[�؂�
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/unlock.png", &g_pTextureEffect[2]);   //���b�N����
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/effect003.png", &g_pTextureEffect[3]);  //�y��

    VERTEX_3D *pVtx;//���_���̃|�C���^

                    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        g_Effect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Effect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Effect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        g_Effect[nCntEffect].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Effect[nCntEffect].type = EFFECTTYPE_000;
        g_Effect[nCntEffect].nCounterAnim = 0;
        g_Effect[nCntEffect].nPatternAnim = 0;
        g_Effect[nCntEffect].bUse = false;

        //���_���W�̐ݒ�i�E���Őݒ肷��j
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
        pVtx[0].col = g_Effect[nCntEffect].col;
        pVtx[1].col = g_Effect[nCntEffect].col;
        pVtx[2].col = g_Effect[nCntEffect].col;
        pVtx[3].col = g_Effect[nCntEffect].col;

        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;
    }
    //���_�f�[�^�A�����b�N
    g_pVtxBuffEffect->Unlock();

}

//=============================================================================
// �G�t�F�N�g�̏I������
//=============================================================================
void UninitEffect(void)
{
    //�e�N�X�`���̊J��
    for (int nCntEffect = 0; nCntEffect < EFFECTTYPE_MAX; nCntEffect++)
    {
        if (g_pTextureEffect[nCntEffect] != NULL)
        {
            g_pTextureEffect[nCntEffect]->Release();
            g_pTextureEffect[nCntEffect] = NULL;
        }
    }

    //���_�o�b�t�@�̊J��
    if (g_pVtxBuffEffect != NULL)
    {
        g_pVtxBuffEffect->Release();
        g_pVtxBuffEffect = NULL;
    }
}

//=============================================================================
//�G�t�F�N�g�̍X�V����
//=============================================================================
void UpdateEffect(void)
{
    VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

    //���_�o�b�t�@�����b�N
    g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        if (g_Effect[nCntEffect].bUse == true)
        {
            PLAYER*pPlayer = GetPlayer();

            switch (g_Effect[nCntEffect].type)
            {
                //�^�[�Q�b�g
            case EFFECTTYPE_000:

                //�G�t�F�N�g�̃J�E���^�[��i�߂�
                g_Effect[nCntEffect].nCounterAnim++;
                if (g_Effect[nCntEffect].nCounterAnim == 10)
                {
                    g_Effect[nCntEffect].nCounterAnim = 0;
                    g_Effect[nCntEffect].bUse = false;
                }

                ////�e�N�X�`�����W���X�V
                //pVtx[0].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f, 0.0f);
                //pVtx[1].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f + 0.2f, 0.0f);
                //pVtx[2].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f, 1.0f);
                //pVtx[3].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f + 0.2f, 1.0f);

                ////�p�^�[�����Ō�܂ŏI�����
                //if (g_Effect[nCntEffect].nPatternAnim > 5)
                //{
                //    g_Effect[nCntEffect].bUse = false;
                //}

                break;

                //�G�i�W�[�؂�
            case EFFECTTYPE_001:

                //�G�t�F�N�g�̃J�E���^�[��i�߂�
                g_Effect[nCntEffect].nCounterAnim++;
                if (g_Effect[nCntEffect].nCounterAnim == START_HEAL_OUT_ENERGY)
                {
                    g_Effect[nCntEffect].nCounterAnim = 0;
                    g_Effect[nCntEffect].bUse = false;
                }

                //�v���C���[���������Ă���Ȃ�v���C���[�̏�ɕ\��
                if (pPlayer->bDisp == true)
                {
                    g_Effect[nCntEffect].pos = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + PLAYER_HEIGHT * 2, pPlayer->pos.z);
                }
                else
                {
                    g_Effect[nCntEffect].bUse = false;
                }

                break;

                //���b�N����
            case EFFECTTYPE_002:

                //�ړ��ʂƈʒu�����т���
                g_Effect[nCntEffect].pos += g_Effect[nCntEffect].move;

                //�G�t�F�N�g�̃J�E���^�[��i�߂�
                g_Effect[nCntEffect].col.a -= 0.01f;
                if (g_Effect[nCntEffect].col.a < 0.0f)
                {
                    g_Effect[nCntEffect].bUse = false;
                }

                break;

                //���n�̉�
            case EFFECTTYPE_003:

                //�G�t�F�N�g2�̃J�E���^�[��i�߂āA�p�^�[����؂�ւ���
                g_Effect[nCntEffect].nCounterAnim++;
                if (g_Effect[nCntEffect].nCounterAnim == 2)
                {
                    g_Effect[nCntEffect].nCounterAnim = 0;
                    g_Effect[nCntEffect].nPatternAnim++;
                }

                //�e�N�X�`�����W���X�V
                pVtx[0].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f + 0.2f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f + 0.2f, 1.0f);

                //�p�^�[�����Ō�܂ŏI�����
                if (g_Effect[nCntEffect].nPatternAnim > 4)
                {
                    g_Effect[nCntEffect].bUse = false;
                }

                break;

                //�q�b�g
            case EFFECTTYPE_004:

                break;
            }

            //�F�Ɠ����x���X�V
            pVtx[0].col = g_Effect[nCntEffect].col;
            pVtx[1].col = g_Effect[nCntEffect].col;
            pVtx[2].col = g_Effect[nCntEffect].col;
            pVtx[3].col = g_Effect[nCntEffect].col;

        }
        pVtx += 4;
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffEffect->Unlock();
}

//=============================================================================
//�G�t�F�N�g�̕`�揈��
//=============================================================================
void DrawEffect(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^���擾
    D3DXMATRIX mtxRot, mtxTrans;//�s��v�Z�p�̃}�g���N�X

    //�A���t�@�e�X�g��L����
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //�A���t�@�e�X�g��l�̐ݒ�
    pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    //�A���t�@�e�X�g�̔�r���@�̐ݒ�iGREATER�͊�l���傫���ꍇ)
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //���C�g�̉e�����󂯂Ȃ�����
    DWORD ambient;
    pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
    pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
    pDevice->LightEnable(0, FALSE);

    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        if (g_Effect[nCntEffect].bUse == true)
        {
            //���[���h�}�g���N�X�̏�����
            D3DXMatrixIdentity(&g_Effect[nCntEffect].mtxWorld);
            //��]�̋t�s��𐶐�
            pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
            D3DXMatrixInverse(&g_Effect[nCntEffect].mtxWorld, NULL, &mtxRot);
            g_Effect[nCntEffect].mtxWorld._41 = 0;//�ړ��͂��������Ȃ��̂�0����
            g_Effect[nCntEffect].mtxWorld._42 = 0;//�ړ��͂��������Ȃ��̂�0����
            g_Effect[nCntEffect].mtxWorld._43 = 0;//�ړ��͂��������Ȃ��̂�0����

            //�ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans, g_Effect[nCntEffect].pos.x, g_Effect[nCntEffect].pos.y, g_Effect[nCntEffect].pos.z);
            D3DXMatrixMultiply(&g_Effect[nCntEffect].mtxWorld, &g_Effect[nCntEffect].mtxWorld, &mtxTrans);
            //���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_Effect[nCntEffect].mtxWorld);
            //�e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, g_pTextureEffect[g_Effect[nCntEffect].type]);
            //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
            pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));
            //���_�t�H�[�}�b�g�̐ݒ�
            pDevice->SetFVF(FVF_VERTEX_3D);
            //�|���S���̕`��
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, NUM_POLYGON);

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
//�G�t�F�N�g�̐ݒ�
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move,EFFECTTYPE type)
{
    VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

                        //���_�o�b�t�@�����b�N
    g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        if (g_Effect[nCntEffect].bUse == false)
        {
            //�A�j���[�V�����p�^�[���̏�����
            g_Effect[nCntEffect].nCounterAnim = 0;
            g_Effect[nCntEffect].nPatternAnim = 0;

            g_Effect[nCntEffect].pos = pos;
            g_Effect[nCntEffect].size = size;
            g_Effect[nCntEffect].move = move;
            g_Effect[nCntEffect].type = type;

            g_Effect[nCntEffect].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

            g_Effect[nCntEffect].bUse = true;

            //���_���W�̐ݒ�
            pVtx[0].pos = D3DXVECTOR3(-(g_Effect[nCntEffect].size.x / 2), (g_Effect[nCntEffect].size.y / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3((g_Effect[nCntEffect].size.x / 2), (g_Effect[nCntEffect].size.y / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(-(g_Effect[nCntEffect].size.x / 2), -(g_Effect[nCntEffect].size.y / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3((g_Effect[nCntEffect].size.x / 2), -(g_Effect[nCntEffect].size.y / 2), 0.0f);

            break;
        }
        pVtx += 4;
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffEffect->Unlock();
}

//=============================================================================
//�G�t�F�N�g�̎擾
//=============================================================================
EFFECT*GetEffect(void)
{
    return &g_Effect[0];
}