//=============================================================================
//
// �ŋ��̘b����� [item.cpp]�̏��� (�A�C�e��)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "item.h"
#include "collision.h"
#include "player.h"
#include "bomb.h"
#include "fire.h"
#include "effect.h"
#include "stage.h"
#include "sound.h"
#include "score.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;   //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureItem[ITEMTYPE_MAX];  //�|���S���̃e�N�X�`��

ITEM g_aItem[MAX_ITEM];    //�G�t�F�N�g�̏��

//=============================================================================
// �A�C�e���̏���������
// Author:�㓡�T�V��
//=============================================================================
void InitItem(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    int nCntItem = 0;

    //�e�N�X�`���̓ǂݍ���
    //��
    g_aItem[nCntItem].type = ITEMTYPE_000;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item000.png",//�t�@�C���̖��O
            &g_pTextureItem[ITEMTYPE_000]);
    }

    //�{��
    g_aItem[nCntItem].type = ITEMTYPE_001;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item001.png",//�t�@�C���̖��O
            &g_pTextureItem[ITEMTYPE_001]);
    }

    //�X�s�[�h
    g_aItem[nCntItem].type = ITEMTYPE_002;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item002.png",//�t�@�C���̖��O
            &g_pTextureItem[ITEMTYPE_002]);
    }

    //�L�b�N
    g_aItem[nCntItem].type = ITEMTYPE_003;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item003.png",//�t�@�C���̖��O
            &g_pTextureItem[ITEMTYPE_003]);
    }

    //�n�[�g
    g_aItem[nCntItem].type = ITEMTYPE_004;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item004.png",//�t�@�C���̖��O
            &g_pTextureItem[ITEMTYPE_004]);
    }

    //�^�C��
    g_aItem[nCntItem].type = ITEMTYPE_005;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item005.png",//�t�@�C���̖��O
            &g_pTextureItem[ITEMTYPE_005]);
    }

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_3D)*NUM_VERTEX*MAX_ITEM,//�o�b�t�@�T�C�Y
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffItem,
        NULL);//���_���ւ̃|�C���^

    VERTEX_3D *pVtx = NULL;//���_���̃|�C���^

                    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

    for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        g_aItem[nCntItem].bUse = false;
        g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aItem[nCntItem].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aItem[nCntItem].nCntTime = 0;
        g_aItem[nCntItem].nCntInvincible = ITEM_INVINCIBLE_TIME;
        g_aItem[nCntItem].nCntVisible = ITEM_VISIBLE_TIME;
        g_aItem[nCntItem].type = ITEMTYPE_000;
        g_aItem[nCntItem].bSet = false;

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
// Author:�㓡�T�V��
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
// �A�C�e���̍X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateItem(void)
{
    for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        //�Z�b�g���Ȃ�
        if (g_aItem[nCntItem].bSet == true)
        {
            g_aItem[nCntItem].nCntTime++;   //�J�E���^���Z

            //���F�ŁA�Z�b�g����
            if (g_aItem[nCntItem].nCntTime >= ITEM_SET_TIME)
            {
                //�g�p����
                g_aItem[nCntItem].bUse = true;

                //������
                g_aItem[nCntItem].nCntTime = 0;
                g_aItem[nCntItem].bSet = false;  
            }
        }

        //�g�p���Ă���Ȃ�
        if (g_aItem[nCntItem].bUse == true)
        {
            //���ւ̖��G����
            g_aItem[nCntItem].nCntInvincible--;

            //�A�C�e�����Q�b�g�ł���܂ł̎��Ԃ𐔂���
            g_aItem[nCntItem].nCntVisible--;

            //���ւ̖��G���Ԃ��Ȃ��Ȃ牊�ɓ�����
            if (g_aItem[nCntItem].nCntInvincible <= 0)
            {
                g_aItem[nCntItem].nCntInvincible = 0;

                //���ɓ���������
                if (HitFire(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].pos,
                    D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE)) == true)
                {
                    //���g�p��
                    g_aItem[nCntItem].bUse = false;

                    //�A�C�e�������G�t�F�N�g
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
                        SetEffect(g_aItem[nCntItem].pos, move, EFFECT_SIZE_EXPLODE_ITEM, EFFECTTYPE_EXPLOSION);
                    }

                    //�O���b�h�����X�V
                    SetGridInfo(g_aItem[nCntItem].pos, false);

                    //������
                    g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                }
            }
            
            //�A�C�e�����Q�b�g�ł���悤�ɂ���
            if (g_aItem[nCntItem].nCntVisible < 0)
            {
                g_aItem[nCntItem].nCntVisible = 0;
            }

            //�{���ɓ���������
            if (HitBomb(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].pos,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID) , MAX_BOMB + 1) == true)
            {
                //���g�p��
                g_aItem[nCntItem].bUse = false;

                //�A�C�e�������G�t�F�N�g
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
                    SetEffect(g_aItem[nCntItem].pos, move, EFFECT_SIZE_EXPLODE_ITEM, EFFECTTYPE_EXPLOSION);
                }

                //�O���b�h�����X�V
                SetGridInfo(D3DXVECTOR3(g_aItem[nCntItem].pos.x, 0.0f, g_aItem[nCntItem].pos.z), false);

                //������
                g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
        }
    }
}

//=============================================================================
// �A�C�e���̕`�揈��
// Author:�㓡�T�V��
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
// �A�C�e���̐ݒ�
// Author:�㓡�T�V��
//=============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ITEMTYPE type)
{
    VERTEX_3D *pVtx = NULL;//���_���̃|�C���^

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        if (g_aItem[nCntItem].bSet == false && g_aItem[nCntItem].bUse == false)
        {
            //������
            g_aItem[nCntItem].pos = pos;
            g_aItem[nCntItem].size = ITEM_SIZE;
            g_aItem[nCntItem].move = move;
            g_aItem[nCntItem].type = type;
            g_aItem[nCntItem].nCntInvincible = ITEM_INVINCIBLE_TIME;
            g_aItem[nCntItem].nCntVisible = ITEM_VISIBLE_TIME;
            g_aItem[nCntItem].bSet = true;

            //�O���b�h����ݒ�
            SetGridInfo(D3DXVECTOR3(g_aItem[nCntItem].pos.x, 0.0f , g_aItem[nCntItem].pos.z), true);

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
// �A�C�e���̎擾
// Author:�㓡�T�V��
//=============================================================================
ITEM*GetItem(void)
{
    return &g_aItem[0];
}

//=====================================================
// �A�C�e���̓����蔻��̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool HitItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size , bool bPlayer, ITEMTYPE type)
{
    bool bHit = false;              //�����������ǂ���
    int nCntItem;					//���[�J���ϐ�

    for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        // �A�C�e�����g��ꂽ��
        if (g_aItem[nCntItem].bUse == true)
        {
            //�^�C�v�������Ȃ�
            if(g_aItem[nCntItem].type == type)
            {
                //�o������A���t���[���o�߂��Ă���Ȃ�
                if (g_aItem[nCntItem].nCntVisible == 0)
                {
                    //�����蔻����v�Z
                    if (RectangleCollision(pPos, pPosOld, size, &g_aItem[nCntItem].pos, g_aItem[nCntItem].size) == true)
                    {
                        //��������
                        bHit = true;

                        //�v���C���[�ɓ���������A���g�p�ɂ��āA�G�t�F�N�g��ݒu����
                        if (bPlayer == true)
                        {
                            PlaySound(SOUND_LABEL_SE_ITEM); //�A�C�e���擾��

                                                            //���g�p��
                            g_aItem[nCntItem].bUse = false;

                            //�X�R�A������
                            ScoreCalculation(GET_ITEM_SCORE);    //�A�C�e�����擾�������̃X�R�A

                            //�G�t�F�N�g�֘A�̕ϐ�
                            float fAngle = 0.0f;    //�p�x
                            float fSpeedY = 0.0f;   //����
                            float fRadius = GRID_SIZE / 2.0f; //�G�t�F�N�g���a

                            //�A�C�e���擾�G�t�F�N�g
                            for (int nCnt = 0; nCnt < COUNT_EFFECT_ITEM; nCnt++)
                            {
                                fSpeedY = float(rand() % 800 + 300) / 100.0f;
                                //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
                                fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                                D3DXVECTOR3 pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - sinf(fAngle)*fRadius, g_aItem[nCntItem].pos.y
                                    , g_aItem[nCntItem].pos.z - cosf(fAngle)*fRadius);
                                D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                                SetEffect(pos, move, EFFECT_SIZE_GET_ITEM, EFFECTTYPE_GET_ITEM); //�A�C�e���擾�G�t�F�N�g
                            }

                            //�O���b�h�����X�V
                            SetGridInfo(g_aItem[nCntItem].pos, false);

                            //������
                            g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                        }
                    }
                }
            }
        }
    }

    return bHit;
}


//=====================================================
// ���ׂĂ̎�ނ̃A�C�e���̓����蔻��̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool HitAllTypeItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bHit = false;              //�����������ǂ���
    int nCntItem;					//���[�J���ϐ�

    for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        // �A�C�e�����g��ꂽ��
        if (g_aItem[nCntItem].bUse == true)
        {
            //�����蔻����v�Z
            if (RectangleCollision(pPos, pPosOld, size, &g_aItem[nCntItem].pos, g_aItem[nCntItem].size) == true)
            {
                //��������
                bHit = true;
            }           
        }
    }

    return bHit;
}