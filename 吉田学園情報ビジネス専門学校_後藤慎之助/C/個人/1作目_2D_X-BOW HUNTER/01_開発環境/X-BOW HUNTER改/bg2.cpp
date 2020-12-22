//=============================================================================
//
// X-Bow Hunter [bg2.cpp]�̏��� (�Y���u�j
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "bg2.h"
#include "boss.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonBg2 = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonBg2 = NULL;
int g_nCounterAnimBg2;			//�A�j���[�V�����J�E���^�[
int g_nPatternAnimBg2;			//�A�j���[�V�����p�^�[��
int g_nCntTime;		//���Ԃ𐔂���
int g_nCntLight;	//�u�̓_��

					//=============================================================================
					//�o�b�N�O���E���h����������
					//=============================================================================
void InitBg2(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "bg001.png", &g_pTexturePolygonBg2);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonBg2, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPolygonBg2->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

														//���_���W�̐ݒ�i�E���Őݒ肷��j
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	//rhw�̐ݒ�i�l��1.0�Őݒ肷��j
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 150);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 150);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 150);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 150);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2 *0.98f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2 *0.98f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2 *0.98f - 0.98f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2 *0.98f - 0.98f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygonBg2->Unlock();
}

//=============================================================================
// �o�b�N�O���E���h�I������
//=============================================================================
void UninitBg2(void)
{
	//�e�N�X�`���̊J��
	if (g_pTexturePolygonBg2 != NULL)
	{
		g_pTexturePolygonBg2->Release();
		g_pTexturePolygonBg2 = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPolygonBg2 != NULL)
	{
		g_pVtxBuffPolygonBg2->Release();
		g_pVtxBuffPolygonBg2 = NULL;
	}
}

//=============================================================================
// �o�b�N�O���E���h�X�V����
//=============================================================================
void UpdateBg2(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	BOSS *pBoss;		//�{�X���擾���邽��

						//��ɗ����A�j���[�V����
	g_nCounterAnimBg2++;	//�J�E���^���Z
	if (g_nCounterAnimBg2 == 4)//����
	{
		g_nCounterAnimBg2 = 0;
		g_nPatternAnimBg2++;
		if (g_nPatternAnimBg2 == 1000)//����
		{
			g_nPatternAnimBg2 = 0;
		}
	}

	//�_�ł���A�j���[�V����
	g_nCntTime++;
	if (g_nCntTime == 12)//����
	{
		g_nCntTime = 0;
		g_nCntLight++;
		if (g_nCntLight == 9)//�����x
		{
			g_nCntLight = 0;
		}
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPolygonBg2->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W���X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2 *0.98f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2 *0.98f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2 *0.98f - 0.98f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2 *0.98f - 0.98f);

	//�{�X�̏����擾
	pBoss = GetBoss();

	//�{�X��ȊO�̐F
	if (pBoss->bUse == false)
	{

		////���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
		if (g_nCntLight == 0)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		}

		if (g_nCntLight == 1)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 155);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 155);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 155);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		}

		if (g_nCntLight == 2)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 105);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 105);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 105);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		}

		if (g_nCntLight == 3)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		}

		if (g_nCntLight == 4)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 5);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 5);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 5);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 5);
		}

		if (g_nCntLight == 5)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		}

		if (g_nCntLight == 6)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 105);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 105);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 105);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		}

		if (g_nCntLight == 7)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 155);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 155);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 155);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		}

		if (g_nCntLight == 8)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		}
	}

	//�{�X��͐Ԃ��u
	if (pBoss->bUse == true)
	{
		////���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
		if (g_nCntLight == 0)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 205);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 205);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 205);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 205);
		}

		if (g_nCntLight == 1)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 155);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 155);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 155);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 155);
		}

		if (g_nCntLight == 2)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 105);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 105);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 105);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 105);
		}

		if (g_nCntLight == 3)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 55);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 55);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 55);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 55);
		}

		if (g_nCntLight == 4)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 5);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 5);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 5);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 5);
		}

		if (g_nCntLight == 5)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 55);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 55);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 55);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 55);
		}

		if (g_nCntLight == 6)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 105);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 105);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 105);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 105);
		}

		if (g_nCntLight == 7)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 155);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 155);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 155);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 155);
		}

		if (g_nCntLight == 8)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 205);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 205);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 205);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 205);
		}
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygonBg2->Unlock();
}

//=============================================================================
// �o�b�N�O���E���h�`�揈��
//=============================================================================
void DrawBg2(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPolygonBg2, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePolygonBg2);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}