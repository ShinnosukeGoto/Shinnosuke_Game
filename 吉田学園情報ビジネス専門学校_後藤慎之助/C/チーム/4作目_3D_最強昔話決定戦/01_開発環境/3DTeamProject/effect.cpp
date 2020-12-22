//=============================================================================
//
// �ŋ��̘b�����̏��� [effect.cpp] (�p�[�e�B�N�����g�����G�t�F�N�g)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "effect.h"
#include "boss.h"
#include "player.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;   //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureEffect[MAX_EFFECT];  //�|���S���̃e�N�X�`��

EFFECT g_aEffect[MAX_EFFECT];    //�G�t�F�N�g�̏��

//=============================================================================
// �G�t�F�N�g�̏���������
// Author:�㓡�T�V��
//=============================================================================
void InitEffect(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    int nCntEffect = 0;

    //�e�N�X�`���̓ǂݍ���
    //��
    g_aEffect[nCntEffect].type = EFFECTTYPE_FIRE;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle007.png",//�t�@�C���̖��O
            &g_pTextureEffect[EFFECTTYPE_FIRE]);
    }

    //����
    g_aEffect[nCntEffect].type = EFFECTTYPE_EXPLOSION;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle001.png",//�t�@�C���̖��O
            &g_pTextureEffect[EFFECTTYPE_EXPLOSION]);
    }

    //�A�C�e���擾
    g_aEffect[nCntEffect].type = EFFECTTYPE_GET_ITEM;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle006.png",//�t�@�C���̖��O
            &g_pTextureEffect[EFFECTTYPE_GET_ITEM]);
    }

    //�E�ӂ̔g��
    g_aEffect[nCntEffect].type = EFFECTTYPE_EVIL;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle000.png",//�t�@�C���̖��O
            &g_pTextureEffect[EFFECTTYPE_EVIL]);
    }

    //�o���b�g�̋O��
    g_aEffect[nCntEffect].type = EFFECTTYPE_BULLET_TRAIL;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle000.png",//�t�@�C���̖��O
            &g_pTextureEffect[EFFECTTYPE_BULLET_TRAIL]);
    }

    //�C��
    g_aEffect[nCntEffect].type = EFFECTTYPE_BOSS_STUN;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/stun.png",//�t�@�C���̖��O
            &g_pTextureEffect[EFFECTTYPE_BOSS_STUN]);
    }

    //�C��
    g_aEffect[nCntEffect].type = EFFECTTYPE_PLAYER_STUN;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/stun.png",//�t�@�C���̖��O
            &g_pTextureEffect[EFFECTTYPE_PLAYER_STUN]);
    }

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_3D)*NUM_VERTEX*MAX_EFFECT,//�o�b�t�@�T�C�Y
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffEffect,
        NULL);//���_���ւ̃|�C���^

    VERTEX_3D *pVtx = NULL;//���_���̃|�C���^

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

    for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        g_aEffect[nCntEffect].bUse = false;
        g_aEffect[nCntEffect].type = EFFECTTYPE_FIRE;
        g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        g_aEffect[nCntEffect].fRadius = 0.0f;
        g_aEffect[nCntEffect].nLife = 0;

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
        pVtx[0].col = g_aEffect[nCntEffect].col;
        pVtx[1].col = g_aEffect[nCntEffect].col;
        pVtx[2].col = g_aEffect[nCntEffect].col;
        pVtx[3].col = g_aEffect[nCntEffect].col;
        //�e�N�X�`�����W�̐ݒ�
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;  //���_�f�[�^��4���i�߂�
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// �G�t�F�N�g�̏I������
// Author:�㓡�T�V��
//=============================================================================
void UninitEffect(void)
{
    //�e�N�X�`���̉��
    for (int nCntTex = 0; nCntTex < EFFECTTYPE_MAX; nCntTex++)
    {
        //�e�N�X�`���̉��
        if (g_pTextureEffect[nCntTex] != NULL)
        {
            g_pTextureEffect[nCntTex]->Release();
            g_pTextureEffect[nCntTex] = NULL;
        }
    }
    //
    if (g_pVtxBuffEffect != NULL)
    {
        g_pVtxBuffEffect->Release();
        g_pVtxBuffEffect = NULL;
    }
}

//=============================================================================
// �G�t�F�N�g�̍X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateEffect(void)
{
    VERTEX_3D *pVtx = NULL;//���_���̃|�C���^

    //���_�J���[�X�V�̂��߁A���b�N
    g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        //�g���Ă���Ȃ�
        if (g_aEffect[nCntEffect].bUse == true)
        {
            switch (g_aEffect[nCntEffect].type)
            {
                //��
            case EFFECTTYPE_FIRE:

                g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

                g_aEffect[nCntEffect].col.a -= FADE_SPEED_EFFECT_FIRE;
                break;

                //����
            case EFFECTTYPE_EXPLOSION:

                g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

                g_aEffect[nCntEffect].move -= D3DXVECTOR3(0.0f, 0.1f, 0.0f);  //������藎�Ƃ�

                g_aEffect[nCntEffect].col.a -= FADE_SPEED_EFFECT_EXPLOSION;  //������������

                break;

                //�A�C�e���擾
            case EFFECTTYPE_GET_ITEM:

                g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

                g_aEffect[nCntEffect].col.a -= FADE_SPEED_EFFECT_ITEM;
                break;

                //�E�ӂ̔g��
            case EFFECTTYPE_EVIL:

                g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

                g_aEffect[nCntEffect].col.a -= FADE_SPEED_EFFECT_EVIL;
                break;

                //�o���b�g�̋O��
            case EFFECTTYPE_BULLET_TRAIL:

                g_aEffect[nCntEffect].fRadius -= FADE_SPEED_EFFECT_BULLET_TRAIL;

                //�傫����0�ɂȂ����疢�g�p��
                if (g_aEffect[nCntEffect].fRadius <= 0.0f)
                {
                    g_aEffect[nCntEffect].fRadius = 0.0f;
                    g_aEffect[nCntEffect].bUse = false;
                }

                break;

                //�{�X�̃X�^��
            case EFFECTTYPE_BOSS_STUN:

                g_aEffect[nCntEffect].nLife--;

                //���C�t��0�ɂȂ�����A�����x��������
                if (g_aEffect[nCntEffect].nLife <= 0)
                {
                    g_aEffect[nCntEffect].nLife = 0;
                    g_aEffect[nCntEffect].col.a -= FADE_SPEED_EFFECT_STUN;
                }

                break;

                //�v���C���[�̃X�^��
            case EFFECTTYPE_PLAYER_STUN:

                g_aEffect[nCntEffect].nLife--;

                //���C�t��0�ɂȂ�����A�����x��������
                if (g_aEffect[nCntEffect].nLife <= 0)
                {
                    g_aEffect[nCntEffect].nLife = 0;
                    g_aEffect[nCntEffect].col.a -= FADE_SPEED_EFFECT_STUN;
                }

                break;
            }

            //�����x��0�Ŗ��g�p��
            if (g_aEffect[nCntEffect].col.a <= 0.0f)
            {
                g_aEffect[nCntEffect].col.a = 0.0f;
                g_aEffect[nCntEffect].bUse = false;
            }

            //���_���W�̐ݒ�
            pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);

            //�F�Ɠ����x���X�V
            pVtx[0].col = g_aEffect[nCntEffect].col;
            pVtx[1].col = g_aEffect[nCntEffect].col;
            pVtx[2].col = g_aEffect[nCntEffect].col;
            pVtx[3].col = g_aEffect[nCntEffect].col;

        }
        pVtx += 4;
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// �G�t�F�N�g�̕`�揈��
// Author:�㓡�T�V��
//=============================================================================
void DrawEffect(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    D3DXMATRIX mtxRot, mtxTrans;//�s��v�Z�p�̃}�g���N�X

    //�A���t�@�e�X�g��L����
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //�A���t�@�e�X�g��l�̐ݒ�
    pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
    //�A���t�@�e�X�g�̔�r���@�̐ݒ�
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //���C�g�̉e�����󂯂Ȃ�����
    DWORD ambient;
    pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
    pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
    pDevice->LightEnable(0, FALSE);

    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        if (g_aEffect[nCntEffect].bUse == true)
        {
            //�����ƃX�^���ȊO�͉��Z����
            if (g_aEffect[nCntEffect].type != EFFECTTYPE_EXPLOSION &&
                g_aEffect[nCntEffect].type != EFFECTTYPE_BOSS_STUN &&
                g_aEffect[nCntEffect].type != EFFECTTYPE_PLAYER_STUN)
            {
                //�����_�[�X�e�[�g(���Z�����ɂ���)
                pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
                pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
            }

            //���[���h�}�g���N�X�̏�����
            D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);
            //��]�̋t�s��𐶐�
            pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
            D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorld, NULL, &mtxRot);
            g_aEffect[nCntEffect].mtxWorld._41 = 0;
            g_aEffect[nCntEffect].mtxWorld._42 = 0;
            g_aEffect[nCntEffect].mtxWorld._43 = 0;

            //�ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans,
                g_aEffect[nCntEffect].pos.x,
                g_aEffect[nCntEffect].pos.y,
                g_aEffect[nCntEffect].pos.z);

            D3DXMatrixMultiply(
                &g_aEffect[nCntEffect].mtxWorld,
                &g_aEffect[nCntEffect].mtxWorld,
                &mtxTrans);

            //���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

            pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].type]);
            //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
            pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));
            //���_�t�H�[�}�b�g�̐ݒ�
            pDevice->SetFVF(FVF_VERTEX_3D);

            //�|���S���̕`��
            pDevice->DrawPrimitive(
                D3DPT_TRIANGLESTRIP,
                nCntEffect * 4,
                NUM_POLYGON);

            //�����ȊO�͉��Z����
            if (g_aEffect[nCntEffect].type != EFFECTTYPE_EXPLOSION &&
                g_aEffect[nCntEffect].type != EFFECTTYPE_BOSS_STUN &&
                g_aEffect[nCntEffect].type != EFFECTTYPE_PLAYER_STUN)
            {
                //�����_�[�X�e�[�g(���Z������߂�)
                pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
                pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            }
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
// �G�t�F�N�g�̐ݒ�
// Author:�㓡�T�V��
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius, EFFECTTYPE type)
{
    VERTEX_3D *pVtx = NULL;//���_���̃|�C���^

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        if (g_aEffect[nCntEffect].bUse == false)
        {
            //������
            g_aEffect[nCntEffect].pos = pos;
            g_aEffect[nCntEffect].move = move;
            g_aEffect[nCntEffect].fRadius = fRadius;
            g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
            g_aEffect[nCntEffect].type = type;
            g_aEffect[nCntEffect].bUse = true;  //�g�p���Ă����Ԃɂ���
            g_aEffect[nCntEffect].nLife = 10;   //���̒l

            if (g_aEffect[nCntEffect].type == EFFECTTYPE_BOSS_STUN)
            {
                g_aEffect[nCntEffect].nLife = STUN_TIME;
            }
            else if (g_aEffect[nCntEffect].type == EFFECTTYPE_PLAYER_STUN)
            {
                g_aEffect[nCntEffect].nLife = PLAYER_STUN_TIME;
            }

            //���_���W�̐ݒ�
            pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);

            break;
        }

        pVtx += 4;  //���_�f�[�^��4���i�߂�
    }
    //���_�f�[�^���A�����b�N����
    g_pVtxBuffEffect->Unlock();
}