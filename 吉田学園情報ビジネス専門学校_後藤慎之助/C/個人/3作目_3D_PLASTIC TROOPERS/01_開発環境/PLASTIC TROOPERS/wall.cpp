//=============================================================================
//
// PLASTIC TROOPERS�̏��� [wall.h]�i���F�����̕ǁj
// Author:�㓡�T�V��
//
//=============================================================================
#include "wall.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureWall[WALLTYPE_MAX] = {};		//�|���S���̃e�N�X�`���ւ̃|�C���^
WALL g_Wall[MAX_WALL];//�ǂ̏��

int g_nCounterAnimWall;		//�A�j���[�V�����J�E���^�[
int g_nPatternAnimWall;		//�A�j���[�V�����p�^�[��
//=============================================================================
//�ǂ̏���������
//=============================================================================
void InitWall(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

    //�O���[�o���ϐ��̏�����
    g_nCounterAnimWall = 0;
    g_nPatternAnimWall = 0;

    //���_�o�b�t�@�̐���
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX*MAX_WALL,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,		//���_�t�H�[�}�b�g
        D3DPOOL_MANAGED,//�i�Œ�j
        &g_pVtxBuffWall,//�ϐ������ς��ƕύX���K�v
        NULL);

    D3DXCreateTextureFromFile(pDevice, "TEXTURE/wall000.png", &g_pTextureWall[WALLTYPE_YELLOW]);//�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/wall001.png", &g_pTextureWall[WALLTYPE_RED1]);//�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/wall001.png", &g_pTextureWall[WALLTYPE_RED2]);//�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/wall002.png", &g_pTextureWall[WALLTYPE_RED3]);//�e�N�X�`���̓ǂݍ���

    VERTEX_3D *pVtx;//���_���̃|�C���^

                    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
    {
        g_Wall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Wall[nCntWall].type = WALLTYPE_YELLOW;
        g_Wall[nCntWall].bUse = false;

        //���_���W�̏������i�E���Őݒ肷��j
        pVtx[0].pos = D3DXVECTOR3(0.0f - WALL_SIZE, 0.0f, 0.0f + WALL_SIZE_Y);
        pVtx[1].pos = D3DXVECTOR3(0.0f + WALL_SIZE, 0.0f, 0.0f + WALL_SIZE_Y);
        pVtx[2].pos = D3DXVECTOR3(0.0f - WALL_SIZE, 0.0f, 0.0f - WALL_SIZE_Y);
        pVtx[3].pos = D3DXVECTOR3(0.0f + WALL_SIZE, 0.0f, 0.0f - WALL_SIZE_Y);

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
    g_pVtxBuffWall->Unlock();
}
//=============================================================================
//�ǂ̍X�V����
//=============================================================================
void UpdateWall(void)
{
    VERTEX_3D *pVtx = NULL;	//���_���ւ̃|�C���^

    g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

                                                    //���ɗ����A�j���[�V����
    g_nCounterAnimWall++;					//�J�E���^���Z
    if (g_nCounterAnimWall == 2)			//����
    {
        g_nCounterAnimWall = 0;
        g_nPatternAnimWall++;
        if (g_nPatternAnimWall == 1000)	//����
        {
            g_nPatternAnimWall = 0;
        }
    }

    //�e�N�X�`�����W���X�V
    pVtx[0].tex = D3DXVECTOR2(0.0f, g_nPatternAnimWall *0.004f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, g_nPatternAnimWall *0.004f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, g_nPatternAnimWall *0.004f - 0.996f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, g_nPatternAnimWall *0.004f - 0.996f);

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffWall->Unlock();
}
//=============================================================================
//�ǂ̐ݒ�
//=============================================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, WALLTYPE type)
{
    for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
    {
        if (g_Wall[nCntWall].bUse == false)
        {
            g_Wall[nCntWall].bUse = true;
            g_Wall[nCntWall].pos = pos;
            g_Wall[nCntWall].rot = rot;
            g_Wall[nCntWall].type = type;
            break;
        }
    }
}
//=============================================================================
//�ǂ̕`�揈��
//=============================================================================
void DrawWall(void)
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

    for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
    {
        if (g_Wall[nCntWall].bUse == true)
        {
            //���[���h�}�g���N�X�̏�����
            D3DXMatrixIdentity(&g_Wall[nCntWall].mtxWorld);
            //�����𔽉f
            D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);
            D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxRot);
            //�ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);
            D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxTrans);
            //���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCntWall].mtxWorld);
            //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
            pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));
            //���_�t�H�[�}�b�g�̐ݒ�
            pDevice->SetFVF(FVF_VERTEX_3D);
            //�e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, g_pTextureWall[g_Wall[nCntWall].type]);
            //�|���S���̕`��
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
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
//�ǂ̏I������
//=============================================================================
void UninitWall(void)
{
    //���_�o�b�t�@�̊J��
    if (g_pVtxBuffWall != NULL)
    {
        g_pVtxBuffWall->Release();
        g_pVtxBuffWall = NULL;
    }
    //�e�N�X�`���̊J��
    for (int nCntWall = 0; nCntWall < WALLTYPE_MAX; nCntWall++)
    {
        if (g_pTextureWall[nCntWall] != NULL)
        {
            g_pTextureWall[nCntWall]->Release();
            g_pTextureWall[nCntWall] = NULL;
        }
    }
}

//=============================================================================
//�ǂ̎擾
//=============================================================================
WALL*GetWall(void)
{
    return &g_Wall[0];
}