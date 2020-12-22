//=============================================================================
//
// �ŋ��̘b����� [bullet.cpp]�̏��� (�G�̒e)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "bullet.h"
#include "block.h"
#include "collision.h"
#include "effect.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"
#include "camera.h"
#include "sound.h"
#include "boss.h"
#include "shadow.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureBullet[BULLETTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//���_�o�b�t�@�ւ̃|�C���^
BULLET g_aBullet[MAX_BULLET];//�A�C�e���̏��

//=============================================================================
// �o���b�g����������
// Author:�㓡�T�V��
//=============================================================================
void InitBullet(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

    //���_�o�b�t�@�̐���
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_BULLET,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,		//���_�t�H�[�}�b�g
        D3DPOOL_MANAGED,//�i�Œ�j
        &g_pVtxBuffBullet,//�ϐ������ς��ƕύX���K�v
        NULL);

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/particle007.png", &g_pTextureBullet[BULLETTYPE_ENEMY]);   //�G�̒e
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/particle007.png", &g_pTextureBullet[BULLETTYPE_BOSS]);    //�{�X�̒e


    VERTEX_3D *pVtx = NULL;//���_���̃|�C���^

                    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     
        g_aBullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  
        g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBullet[nCntBullet].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBullet[nCntBullet].type = BULLETTYPE_ENEMY;
        g_aBullet[nCntBullet].nLife = 0;
        g_aBullet[nCntBullet].fDamage = 0.0f;
        g_aBullet[nCntBullet].bUse = false;
        g_aBullet[nCntBullet].nShadow = 0;

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
    g_pVtxBuffBullet->Unlock();

}

//=============================================================================
// �o���b�g�I������
// Author:�㓡�T�V��
//=============================================================================
void UninitBullet(void)
{
    //�e�N�X�`���̊J��
    for (int nCntBullet = 0; nCntBullet < BULLETTYPE_MAX; nCntBullet++)
    {
        if (g_pTextureBullet[nCntBullet] != NULL)
        {
            g_pTextureBullet[nCntBullet]->Release();
            g_pTextureBullet[nCntBullet] = NULL;
        }
    }

    //���_�o�b�t�@�̊J��
    if (g_pVtxBuffBullet != NULL)
    {
        g_pVtxBuffBullet->Release();
        g_pVtxBuffBullet = NULL;
    }
}

//=============================================================================
// �o���b�g�X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateBullet(void)
{
    VERTEX_3D *pVtx = NULL;//���_���̃|�C���^

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        if (g_aBullet[nCntBullet].bUse == true)
        {
            //1F�O�̈ʒu
            g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

            //�ړ��ʂ𔽉f������
            g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

            //�e��ݒ�
            SetPositionShadow(g_aBullet[nCntBullet].nShadow,
                D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, 0, g_aBullet[nCntBullet].pos.z), 0.0f);

            //�e�̎˒������߂�
            g_aBullet[nCntBullet].nLife--;

            //�u���b�N�ɓ���������e�̃��C�t���Ȃ���
            if (HitAllTypeBlock(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld,
                g_aBullet[nCntBullet].size) == true)
            {
                g_aBullet[nCntBullet].nLife = 0;

                //�q�b�g�G�t�F�N�g
                float fAngle = 0.0f;    //�p�x
                float fSpeed = 6.0f;    //����
                for (int nCnt = 0; nCnt < COUNT_EFFECT_HIT; nCnt++)
                {
                    //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                    D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle)*fSpeed, fSpeed, -cosf(fAngle)*fSpeed);
                    SetEffect(g_aBullet[nCntBullet].pos, move, EFFECT_SIZE_EVIL, EFFECTTYPE_EVIL); //�E�ӂ̔g��
                }
            }

            //�o���b�g�̋O�Ղ�ݒ�
            SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aBullet[nCntBullet].size.x, EFFECTTYPE_BULLET_TRAIL);

            //���C�t���Ȃ��Ȃ����疢�g�p��
            if (g_aBullet[nCntBullet].nLife <= 0)
            {
                g_aBullet[nCntBullet].bUse = false;	//�g�p���Ȃ���Ԃɂ���

                //�e�̍폜
                SHADOW *pShadow = GetShadow();//�e�̏����擾
                pShadow[g_aBullet[nCntBullet].nShadow].use = false;

                //������
                g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
        }
    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// �o���b�g�`�揈��
// Author:�㓡�T�V��
//=============================================================================
void DrawBullet(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^���擾
    D3DXMATRIX mtxRot, mtxTrans;//�s��v�Z�p�̃}�g���N�X

    //�A���t�@�e�X�g��L����
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //�A���t�@�e�X�g��l�̐ݒ�
    pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
    //�A���t�@�e�X�g�̔�r���@�̐ݒ�iGREATER�͊�l���傫���ꍇ)
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //���C�g�̉e�����󂯂Ȃ�����
    DWORD ambient;
    pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
    pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
    pDevice->LightEnable(0, FALSE);

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        //�g�p���Ă���Ȃ�
        if (g_aBullet[nCntBullet].bUse == true)
        {
            //���[���h�}�g���N�X�̏�����
            D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);
            //��]�̋t�s��𐶐�
            pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
            D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxRot);
            g_aBullet[nCntBullet].mtxWorld._41 = 0;//�ړ��͂��������Ȃ��̂�0����
            g_aBullet[nCntBullet].mtxWorld._42 = 0;//�ړ��͂��������Ȃ��̂�0����
            g_aBullet[nCntBullet].mtxWorld._43 = 0;//�ړ��͂��������Ȃ��̂�0����

                                                  //�ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
            D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);
            //���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);
            //�e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].type]);
            //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
            pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));
            //���_�t�H�[�}�b�g�̐ݒ�
            pDevice->SetFVF(FVF_VERTEX_3D);
            //�|���S���̕`��
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, NUM_POLYGON);

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
// �o���b�g�̐ݒ�
// Author:�㓡�T�V��
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, int nLife, float fDamage, BULLETTYPE type)
{
    VERTEX_3D *pVtx = NULL;//���_���̃|�C���^

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        if (g_aBullet[nCntBullet].bUse == false)
        {
            g_aBullet[nCntBullet].bUse = true;
            g_aBullet[nCntBullet].pos = pos;
            g_aBullet[nCntBullet].size = size;
            g_aBullet[nCntBullet].move = move;
            g_aBullet[nCntBullet].nLife = nLife;
            g_aBullet[nCntBullet].fDamage = fDamage;
            g_aBullet[nCntBullet].type = type;

            //���_���W�̐ݒ�
            pVtx[0].pos = D3DXVECTOR3(-(g_aBullet[nCntBullet].size.x / 2), (g_aBullet[nCntBullet].size.y / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3((g_aBullet[nCntBullet].size.x / 2), (g_aBullet[nCntBullet].size.y / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(-(g_aBullet[nCntBullet].size.x / 2), -(g_aBullet[nCntBullet].size.y / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3((g_aBullet[nCntBullet].size.x / 2), -(g_aBullet[nCntBullet].size.y / 2), 0.0f);

            //�e�̉e�̐ݒ�
            g_aBullet[nCntBullet].nShadow = SetShadow(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aBullet[nCntBullet].size.x, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_ALPHA_MAX));

            break;
        }
        pVtx += 4;
    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// �o���b�g�̎擾
// Author:�㓡�T�V��
//=============================================================================
BULLET*GetBullet(void)
{
    return &g_aBullet[0];
}

//=====================================================
// �e�̏Փ˔���̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool CollisionBullet(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bCollision = false;  //�Փ˂������ǂ���

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        // �e���g��ꂽ��
        if (g_aBullet[nCntBullet].bUse == true)
        {

            //�����蔻����v�Z
            if (RectangleCollision(pPos, pPosOld, size, &g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].size) == true)
            {
                //�G�l�~�[�̒e(���������疢�g�p��)
                if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
                {
                    //�m�b�N�o�b�N
                    PLAYER *pPlayer = GetPlayer();
                    if (pPlayer->nCntInvincibleFrame == 0)  //���G�łȂ��Ȃ�
                    {
                        float fKnockback = VALUE_KNOCKBACK;
                        pPlayer->move.x = pPlayer->pos.x - g_aBullet[nCntBullet].posOld.x;
                        pPlayer->move.z = pPlayer->pos.z - g_aBullet[nCntBullet].posOld.z;
                        D3DXVec3Normalize(&pPlayer->move, &pPlayer->move);  //�x�N�g���𐳋K��
                        pPlayer->move.x = pPlayer->move.x*fKnockback;
                        pPlayer->move.z = pPlayer->move.z*fKnockback;
                    }

                    //���@�Ƀ_���[�W
                    HitPlayer(g_aBullet[nCntBullet].fDamage);

                    bCollision = true;    //�Փ�
                    g_aBullet[nCntBullet].nLife = 0;  //���������烉�C�t��0��

                    //�q�b�g�G�t�F�N�g
                    float fAngle = 0.0f;    //�p�x
                    float fSpeed = 6.0f;    //����
                    for (int nCnt = 0; nCnt < COUNT_EFFECT_HIT; nCnt++)
                    {
                        //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                        D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle)*fSpeed, fSpeed, -cosf(fAngle)*fSpeed);
                        SetEffect(g_aBullet[nCntBullet].pos, move, EFFECT_SIZE_EVIL, EFFECTTYPE_EVIL); //�E�ӂ̔g��
                    }
                }

                //�{�X�̒e(�������Ă����g�p�ɂȂ�Ȃ�)
                else if (g_aBullet[nCntBullet].type == BULLETTYPE_BOSS)
                {

                    //�m�b�N�o�b�N�ƃq�b�g�G�t�F�N�g
                    PLAYER *pPlayer = GetPlayer();
                    if (pPlayer->nCntInvincibleFrame == 0)  //���G�łȂ��Ȃ�
                    {
                        float fKnockback = VALUE_KNOCKBACK;
                        pPlayer->move.x = pPlayer->pos.x - g_aBullet[nCntBullet].posOld.x;
                        pPlayer->move.z = pPlayer->pos.z - g_aBullet[nCntBullet].posOld.z;
                        D3DXVec3Normalize(&pPlayer->move, &pPlayer->move);  //�x�N�g���𐳋K��
                        pPlayer->move.x = pPlayer->move.x*fKnockback;
                        pPlayer->move.z = pPlayer->move.z*fKnockback;

                        float fAngle = 0.0f;    //�p�x
                        float fSpeed = 6.0f;    //����
                        for (int nCnt = 0; nCnt < COUNT_EFFECT_HIT; nCnt++)
                        {
                            //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle)*fSpeed, fSpeed, -cosf(fAngle)*fSpeed);
                            SetEffect(g_aBullet[nCntBullet].pos, move, EFFECT_SIZE_EVIL, EFFECTTYPE_EVIL); //�E�ӂ̔g��
                        }
                    }

                    //���@�Ƀ_���[�W
                    HitPlayer(g_aBullet[nCntBullet].fDamage);

                    bCollision = true;    //�Փ�
                }
            }
        }
    }
    return bCollision;    //�Փ˂������ǂ�����Ԃ�
}

//=====================================================
// �e�̓����蔻��̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool HitBullet(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bHit = false;  //�Փ˂������ǂ���

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        // �e���g��ꂽ��
        if (g_aBullet[nCntBullet].bUse == true)
        {

            //�����蔻����v�Z
            if (RectangleCollision(pPos, pPosOld, size, &g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].size) == true)
            {
                bHit = true;    //�Փ�

                //�q�b�g�G�t�F�N�g
                float fAngle = 0.0f;    //�p�x
                float fSpeed = 6.0f;    //����
                for (int nCnt = 0; nCnt < COUNT_EFFECT_HIT; nCnt++)
                {
                    //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                    D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle)*fSpeed, fSpeed, -cosf(fAngle)*fSpeed);
                    SetEffect(g_aBullet[nCntBullet].pos, move, EFFECT_SIZE_EVIL, EFFECTTYPE_EVIL); //�E�ӂ̔g��
                }

                //�G�l�~�[�̒e�Ȃ�
                if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
                {
                    g_aBullet[nCntBullet].nLife = 0;  //���������烉�C�t��0��
                }
            }
        }
    }
    return bHit;    //�Փ˂������ǂ�����Ԃ�
}