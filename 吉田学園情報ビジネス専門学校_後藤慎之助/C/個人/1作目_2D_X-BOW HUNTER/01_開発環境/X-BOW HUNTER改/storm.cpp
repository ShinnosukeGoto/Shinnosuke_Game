//=============================================================================
//
// X-BOWHUNTER �̏��� [storm.cpp] (�{�X�̃X�g�[��)
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "hunter.h"
#include "bullet.h"
#include "explosion.h"
#include "boss.h"
#include "storm.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureStorm = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffStorm = NULL;
STORM g_aStorm[MAX_STORM];//�X�g�[���̏��

						  //=============================================================================
						  //�X�g�[������������
						  //=============================================================================
void InitStorm(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntStorm;

	//�X�g�[���̏��̏�����
	for (nCntStorm = 0; nCntStorm < MAX_STORM; nCntStorm++)
	{
		g_aStorm[nCntStorm].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aStorm[nCntStorm].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aStorm[nCntStorm].bUse = false;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "bullet000.png", &g_pTextureStorm);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_STORM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffStorm, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffStorm->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

													//�X�g�[���̐���
	for (nCntStorm = 0; nCntStorm < MAX_STORM; nCntStorm++)
	{
		//rhw�̐ݒ�i�l��1.0�Őݒ肷��j
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffStorm->Unlock();
}

//=============================================================================
// �X�g�[���I������
//=============================================================================
void UninitStorm(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureStorm != NULL)
	{
		g_pTextureStorm->Release();
		g_pTextureStorm = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffStorm != NULL)
	{
		g_pVtxBuffStorm->Release();
		g_pVtxBuffStorm = NULL;
	}
}

//=============================================================================
// �X�g�[���X�V����
//=============================================================================
void UpdateStorm(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntStorm;
	VERTEX_2D *pVtx;
	HUNTER*pHunter;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffStorm->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStorm = 0; nCntStorm < MAX_STORM; nCntStorm++)
	{
		if (g_aStorm[nCntStorm].bUse == true)
		{
			//���@���擾
			pHunter = GetHunter();

			if (pHunter->bDisp == true)
			{
				//���@�̓����蔻��
				if (g_aStorm[nCntStorm].pos.x <= pHunter->pos.x + (HUNTER_SIZE / 2) &&
					g_aStorm[nCntStorm].pos.x >= pHunter->pos.x - (HUNTER_SIZE / 2) &&
					g_aStorm[nCntStorm].pos.y <= pHunter->pos.y + (HUNTER_SIZE / 2) &&
					g_aStorm[nCntStorm].pos.y >= pHunter->pos.y - (HUNTER_SIZE / 2))
				{
					//���@�Ƀ_���[�W
					HitHunter(10);
					SetExplosion(g_aStorm[nCntStorm].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 25);
					//�����������_�Œe��HP��0�ɂ���
					g_aStorm[nCntStorm].nLife = 0;
				}
			}

		}

		//�X�g�[���̈ړ�
		g_aStorm[nCntStorm].pos += g_aStorm[nCntStorm].move;

		//�{�X�̒e�����������n�ʂ�X������́A���E�̈ړ��͈�
		if (g_aStorm[nCntStorm].pos.x < g_aStorm[nCntStorm].nHitPosX - 13 || g_aStorm[nCntStorm].pos.x > g_aStorm[nCntStorm].nHitPosX + 13)
		{
			g_aStorm[nCntStorm].move.x *= -1;
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x - (g_aStorm[nCntStorm].nSize / 2), g_aStorm[nCntStorm].pos.y - (g_aStorm[nCntStorm].nSize / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x + (g_aStorm[nCntStorm].nSize / 2), g_aStorm[nCntStorm].pos.y - (g_aStorm[nCntStorm].nSize / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x - (g_aStorm[nCntStorm].nSize / 2), g_aStorm[nCntStorm].pos.y + (g_aStorm[nCntStorm].nSize / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x + (g_aStorm[nCntStorm].nSize / 2), g_aStorm[nCntStorm].pos.y + (g_aStorm[nCntStorm].nSize / 2), 0.0f);

		//���ɒe���o�Ă�������
		if (g_aStorm[nCntStorm].pos.x < g_aStorm[nCntStorm].nSize - (g_aStorm[nCntStorm].nSize / 2))
		{
			g_aStorm[nCntStorm].bUse = false;
		}

		//�E�ɒe���o�Ă�������
		if (g_aStorm[nCntStorm].pos.x > SCREEN_WIDTH - g_aStorm[nCntStorm].nSize + (g_aStorm[nCntStorm].nSize / 2))
		{
			g_aStorm[nCntStorm].bUse = false;
		}

		//��ɒe���o�Ă�������
		if (g_aStorm[nCntStorm].pos.y < g_aStorm[nCntStorm].nSize - (g_aStorm[nCntStorm].nSize / 2))
		{
			g_aStorm[nCntStorm].bUse = false;
		}

		//�e�̎˒������߂�
		g_aStorm[nCntStorm].nLife--;
		if (g_aStorm[nCntStorm].nLife <= 0)
		{
			g_aStorm[nCntStorm].bUse = false;	//�g�p���Ȃ���Ԃɂ���
		}
		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffStorm->Unlock();

}

//=============================================================================
// �X�g�[���`�揈��
//=============================================================================
void DrawStorm(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntStorm;

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffStorm, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureStorm);

	//�|���S���̕`��
	for (nCntStorm = 0; nCntStorm < MAX_STORM; nCntStorm++)
	{
		if (g_aStorm[nCntStorm].bUse == true)			//�e���g�p���Ȃ�`��
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntStorm * 4, NUM_POLYGON);
		}
	}
}

//=============================================================================
// �X�g�[���̐ݒ菈��
//=============================================================================
void SetStorm(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, int nHitPosX, int nSize)
{
	//�ϐ��錾
	int nCntStorm;
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffStorm->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStorm = 0; nCntStorm < MAX_STORM; nCntStorm++)
	{
		if (g_aStorm[nCntStorm].bUse == false)
		{
			//�ʒu��ݒ�
			g_aStorm[nCntStorm].pos = pos;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x - (nSize / 2), g_aStorm[nCntStorm].pos.y - (nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x + (nSize / 2), g_aStorm[nCntStorm].pos.y - (nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x - (nSize / 2), g_aStorm[nCntStorm].pos.y + (nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x + (nSize / 2), g_aStorm[nCntStorm].pos.y + (nSize / 2), 0.0f);

			g_aStorm[nCntStorm].move = move;
			g_aStorm[nCntStorm].nLife = nLife;
			g_aStorm[nCntStorm].col = col;
			g_aStorm[nCntStorm].nHitPosX = nHitPosX;
			g_aStorm[nCntStorm].nSize = nSize;
			g_aStorm[nCntStorm].bUse = true;

			pVtx[0].col = g_aStorm[nCntStorm].col;
			pVtx[1].col = g_aStorm[nCntStorm].col;
			pVtx[2].col = g_aStorm[nCntStorm].col;
			pVtx[3].col = g_aStorm[nCntStorm].col;
			break;
		}
		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffStorm->Unlock();
}

//=============================================================================
// �X�g�[���̎擾
//=============================================================================
STORM*GetStorm(void)
{
	return &g_aStorm[0];
}