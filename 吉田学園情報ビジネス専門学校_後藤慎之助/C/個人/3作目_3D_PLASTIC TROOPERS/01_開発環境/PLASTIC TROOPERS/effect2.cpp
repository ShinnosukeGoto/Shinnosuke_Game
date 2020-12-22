//=============================================================================
//
// PLASTIC TROOPERS [effect2.cpp]�̏��� (�p�[�e�B�N�����g�����G�t�F�N�g)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "player.h"
#include "effect2.h"
#include "camera.h"
#include "collision.h"
#include "enemy.h"
#include "boss.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect2 = NULL;   //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureEffect2[MAX_EFFECT2];  //�|���S���̃e�N�X�`��

EFFECT2 g_aEffect2[MAX_EFFECT2];    //�G�t�F�N�g�̏��

//=============================================================================
//�G�t�F�N�g2�̏���������
//=============================================================================
void InitEffect2(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    int nCntEffect2 = 0;

    //�e�N�X�`���̓ǂݍ���
    //�q�b�g
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_HIT;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle000.png",//�t�@�C���̖��O
            &g_pTextureEffect2[EFFECT2TYPE_HIT]);
    }

    //����
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_EXPLOSION;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle001.png",//�t�@�C���̖��O
            &g_pTextureEffect2[EFFECT2TYPE_EXPLOSION]);
    }

    //�y��
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_RUN;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle002.png",//�t�@�C���̖��O
            &g_pTextureEffect2[EFFECT2TYPE_RUN]);
    }

    //��
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_HEAL;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle003.png",//�t�@�C���̖��O
            &g_pTextureEffect2[EFFECT2TYPE_HEAL]);
    }

    //�Ԍ���
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_GEYSER;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle004.png",//�t�@�C���̖��O
            &g_pTextureEffect2[EFFECT2TYPE_GEYSER]);
    }

    //�X�g�[���\�[�h��
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_STORM;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle005.png",//�t�@�C���̖��O
            &g_pTextureEffect2[EFFECT2TYPE_STORM]);
    }

    //�X�g�[���\�[�h��
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_STORM2;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle006.png",//�t�@�C���̖��O
            &g_pTextureEffect2[EFFECT2TYPE_STORM2]);
    }

    //��
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_FIRE;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle007.png",//�t�@�C���̖��O
            &g_pTextureEffect2[EFFECT2TYPE_FIRE]);
    }

    //�L���L��
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_RARE;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle006.png",//�t�@�C���̖��O
            &g_pTextureEffect2[EFFECT2TYPE_RARE]);
    }

    //��
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_FIRE2;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle007.png",//�t�@�C���̖��O
            &g_pTextureEffect2[EFFECT2TYPE_FIRE2]);
    }

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_3D)*NUM_VERTEX*MAX_EFFECT2,//�o�b�t�@�T�C�Y
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffEffect2,
        NULL);//���_���ւ̃|�C���^

    VERTEX_3D *pVtx;//���_���̃|�C���^

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffEffect2->Lock(0, 0, (void**)&pVtx, 0);

    for (nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
    {
        g_aEffect2[nCntEffect2].bUse = false;

        g_aEffect2[nCntEffect2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEffect2[nCntEffect2].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEffect2[nCntEffect2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        g_aEffect2[nCntEffect2].fRadius = 0.0f;

        //���_���W�̐ݒ�
        pVtx[0].pos = D3DXVECTOR3(-(EFFECT2_SIZE_HIT / 2), (EFFECT2_SIZE_HIT / 2), 0.0f);
        pVtx[1].pos = D3DXVECTOR3((EFFECT2_SIZE_HIT / 2), (EFFECT2_SIZE_HIT / 2), 0.0f);
        pVtx[2].pos = D3DXVECTOR3(-(EFFECT2_SIZE_HIT / 2), -(EFFECT2_SIZE_HIT / 2), 0.0f);
        pVtx[3].pos = D3DXVECTOR3((EFFECT2_SIZE_HIT / 2), -(EFFECT2_SIZE_HIT / 2), 0.0f);
        //�@���̐ݒ�
        pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        //���_�J���[�̐ݒ�
        pVtx[0].col = g_aEffect2[nCntEffect2].col;
        pVtx[1].col = g_aEffect2[nCntEffect2].col;
        pVtx[2].col = g_aEffect2[nCntEffect2].col;
        pVtx[3].col = g_aEffect2[nCntEffect2].col;
        //�e�N�X�`�����W�̐ݒ�
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;  //���_�f�[�^��4���i�߂�
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffEffect2->Unlock();
}

//=============================================================================
// �G�t�F�N�g2�̏I������
//=============================================================================
void UninitEffect2(void)
{
    //�e�N�X�`���̉��
    for (int nCntTex = 0; nCntTex < EFFECT2TYPE_MAX; nCntTex++)
    {
        //�e�N�X�`���̉��
        if (g_pTextureEffect2[nCntTex] != NULL)
        {
            g_pTextureEffect2[nCntTex]->Release();
            g_pTextureEffect2[nCntTex] = NULL;
        }
    }
    //
    if (g_pVtxBuffEffect2 != NULL)
    {
        g_pVtxBuffEffect2->Release();
        g_pVtxBuffEffect2 = NULL;
    }
}

//=============================================================================
//�G�t�F�N�g2�̍X�V����
//=============================================================================
void UpdateEffect2(void)
{
    VERTEX_3D *pVtx;//���_���̃|�C���^

    //���_�J���[�̍X�V
    g_pVtxBuffEffect2->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
    {
        //�g���Ă���Ȃ�
        if (g_aEffect2[nCntEffect2].bUse == true)
        {
            switch (g_aEffect2[nCntEffect2].type)
            {
                //�q�b�g
            case EFFECT2TYPE_HIT:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].col.a -= 0.05f;
                break;

                //����
            case EFFECT2TYPE_EXPLOSION:
                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].move -= D3DXVECTOR3(0.0f, 0.1f, 0.0f);  //������藎�Ƃ�

                g_aEffect2[nCntEffect2].col.a -= 0.005f;  //������������

                break;

                //�y��
            case EFFECT2TYPE_RUN:

                g_aEffect2[nCntEffect2].pos.y += ((float)(rand() % 5) + 1);
                g_aEffect2[nCntEffect2].col.a -= 0.1f;

                //��
            case EFFECT2TYPE_HEAL:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].col.a -= 0.07f;
                break;

                //�Ԍ���
            case EFFECT2TYPE_GEYSER:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].move -= D3DXVECTOR3(0.0f, 0.1f, 0.0f);

                g_aEffect2[nCntEffect2].col.a -= 0.008f;
                break;

                //�X�g�[���\�[�h��
            case EFFECT2TYPE_STORM:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].col.a -= 0.008f;
                break;

                //�X�g�[���\�[�h��
            case EFFECT2TYPE_STORM2:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].col.a -= 0.008f;

                break;

                //��
            case EFFECT2TYPE_FIRE:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].col.a -= 0.015f;
                break;

                //�L���L��
            case EFFECT2TYPE_RARE:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].col.a -= 0.009f;
                break;

                //��2
            case EFFECT2TYPE_FIRE2:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].col.a -= 0.01f;
                break;
            }

            //�����x��0�Ŗ��g�p��
            if (g_aEffect2[nCntEffect2].col.a <= 0)
            {
                g_aEffect2[nCntEffect2].bUse = false;
            }

            //�F�Ɠ����x���X�V
            pVtx[0].col = g_aEffect2[nCntEffect2].col;
            pVtx[1].col = g_aEffect2[nCntEffect2].col;
            pVtx[2].col = g_aEffect2[nCntEffect2].col;
            pVtx[3].col = g_aEffect2[nCntEffect2].col;

        }
        pVtx += 4;
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffEffect2->Unlock();
}

//=============================================================================
//�G�t�F�N�g2�̕`�揈��
//=============================================================================
void DrawEffect2(void)
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

    for (int nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
    {
        if (g_aEffect2[nCntEffect2].bUse == true)
        {
            //�����Ɠy���ȊO�͉��Z����
            if (g_aEffect2[nCntEffect2].type != EFFECT2TYPE_EXPLOSION)
            {
                //�����_�[�X�e�[�g(���Z�����ɂ���)
                pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
                pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
            }

            //���[���h�}�g���N�X�̏�����
            D3DXMatrixIdentity(&g_aEffect2[nCntEffect2].mtxWorld);
            //��]�̋t�s��𐶐�
            pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
            D3DXMatrixInverse(&g_aEffect2[nCntEffect2].mtxWorld, NULL, &mtxRot);
            g_aEffect2[nCntEffect2].mtxWorld._41 = 0;
            g_aEffect2[nCntEffect2].mtxWorld._42 = 0;
            g_aEffect2[nCntEffect2].mtxWorld._43 = 0;

            ////�ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans,
                g_aEffect2[nCntEffect2].pos.x,
                g_aEffect2[nCntEffect2].pos.y,
                g_aEffect2[nCntEffect2].pos.z);

            D3DXMatrixMultiply(
                &g_aEffect2[nCntEffect2].mtxWorld,
                &g_aEffect2[nCntEffect2].mtxWorld,
                &mtxTrans);

            //���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_aEffect2[nCntEffect2].mtxWorld);

            pDevice->SetTexture(0, g_pTextureEffect2[g_aEffect2[nCntEffect2].type]);
            //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
            pDevice->SetStreamSource(0, g_pVtxBuffEffect2, 0, sizeof(VERTEX_3D));
            //���_�t�H�[�}�b�g�̐ݒ�
            pDevice->SetFVF(FVF_VERTEX_3D);

            //�|���S���̕`��
            pDevice->DrawPrimitive(
                D3DPT_TRIANGLESTRIP,
                nCntEffect2 * 4,
                NUM_POLYGON);

            //�����ȊO�͉��Z����
            if (g_aEffect2[nCntEffect2].type != EFFECT2TYPE_EXPLOSION)
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
//�G�t�F�N�g2�̐ݒ�
//=============================================================================
void SetEffect2(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius, D3DXCOLOR col, EFFECT2TYPE type)
{
    VERTEX_3D *pVtx;//���_���̃|�C���^
    PLAYER *pPlayer;
    pPlayer = GetPlayer();

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffEffect2->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
    {

        //����ɓ����Ă���Ȃ�
        if (pPlayer->pos.x > pos.x - CAMERA_VIEW_RANGE&&
            pPlayer->pos.x < pos.x + CAMERA_VIEW_RANGE&&
            pPlayer->pos.z > pos.z - CAMERA_VIEW_RANGE&&
            pPlayer->pos.z < pos.z + CAMERA_VIEW_RANGE&&
            g_aEffect2[nCntEffect2].bUse == false)
        {
            //������
            g_aEffect2[nCntEffect2].pos = pos;
            g_aEffect2[nCntEffect2].move = move;
            g_aEffect2[nCntEffect2].fRadius = fRadius;
            g_aEffect2[nCntEffect2].col = col;
            g_aEffect2[nCntEffect2].type = type;
            g_aEffect2[nCntEffect2].bUse = true;//�g�p���Ă����Ԃɂ���

            //���_���W�̐ݒ�
            pVtx[0].pos = D3DXVECTOR3(-(g_aEffect2[nCntEffect2].fRadius / 2), (g_aEffect2[nCntEffect2].fRadius / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3((g_aEffect2[nCntEffect2].fRadius / 2), (g_aEffect2[nCntEffect2].fRadius / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(-(g_aEffect2[nCntEffect2].fRadius / 2), -(g_aEffect2[nCntEffect2].fRadius / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3((g_aEffect2[nCntEffect2].fRadius / 2), -(g_aEffect2[nCntEffect2].fRadius / 2), 0.0f);

            break;
        }


        pVtx += 4;  //���_�f�[�^��4���i�߂�
    }
    //���_�f�[�^���A�����b�N����
    g_pVtxBuffEffect2->Unlock();
}

//=====================================================
// ���@�ƃG�t�F�N�g�Ƃ̓����蔻��̐ݒ�
//=====================================================
void CollisionEffectPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntEffect2;					// ���[�J���ϐ�

    for (nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
    {
        //�G�t�F�N�g���g��ꂽ��
        if (g_aEffect2[nCntEffect2].bUse == true)
        {
            //�Ԍ���
            if (g_aEffect2[nCntEffect2].type == EFFECT2TYPE_GEYSER)
            {
                //�������Ă���Ȃ�
                if (RectangleCollision(pPos, pPosOld, size, &g_aEffect2[nCntEffect2].pos, D3DXVECTOR3(g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius)) == true)
                {

                    PLAYER*pPlayer = GetPlayer();

                    if (pPlayer->bChanceAttack == false)    //�K�E�U�����ȊO
                    {
                        pPlayer->move.y += 0.12f;    //�㏸
                    }
                }
            }

            //�{�X�̉Β�
            else if (g_aEffect2[nCntEffect2].type == EFFECT2TYPE_FIRE2)
            {
                //�������Ă���Ȃ�
                if (RectangleCollision(pPos, pPosOld, size, &g_aEffect2[nCntEffect2].pos, D3DXVECTOR3(g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius)) == true)
                {
                    PLAYER*pPlayer = GetPlayer();

                    //���@�Ƀ_���[�W
                    pPlayer->fLife -= 0.25f;
                }
            }
        }
    }
}

//=====================================================
// �G�ƃG�t�F�N�g�Ƃ̓����蔻��̐ݒ�
//=====================================================
void CollisionEffectEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, int nCnt)
{
    int nCntEffect2;					// ���[�J���ϐ�

    for (nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
    {
        //�G�t�F�N�g���g��ꂽ��
        if (g_aEffect2[nCntEffect2].bUse == true)
        {
            //��
            if (g_aEffect2[nCntEffect2].type == EFFECT2TYPE_FIRE)
            {
                //�������Ă���Ȃ�
                if (RectangleCollision(pPos, pPosOld, size, &g_aEffect2[nCntEffect2].pos, D3DXVECTOR3(g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius)) == true)
                {
                    HitEnemy(nCnt, 5, 0.2f);
                }
            }
        }
    }
}

//=====================================================
// �{�X�ƃG�t�F�N�g�Ƃ̓����蔻��̐ݒ�
//=====================================================
void CollisionEffectBoss(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntEffect2;					// ���[�J���ϐ�

    for (nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
    {
        //�G�t�F�N�g���g��ꂽ��
        if (g_aEffect2[nCntEffect2].bUse == true)
        {
            //��
            if (g_aEffect2[nCntEffect2].type == EFFECT2TYPE_FIRE)
            {
                //�������Ă���Ȃ�
                if (RectangleCollision(pPos, pPosOld, size, &g_aEffect2[nCntEffect2].pos, D3DXVECTOR3(g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius)) == true)
                {
                    HitBoss(1, 0.025f);
                }
            }
        }
    }
}