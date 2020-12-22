//=============================================================================
//
// X-Bow Hunter�̏��� [item.cpp] (�A�C�e��)
// Author :�㓡�T�V��
//
//=============================================================================
#include "hunter.h"
#include "item.h"
#include "game.h"
#include "score.h"
#include "boss.h"
#include "wing.h"
#include "sound.h"
#include "time.h"
#include "input.h"
#include "arrow.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_apTextureItem[ITEMTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffItem = NULL;

int g_nCounterAnimItem;			//�A�j���[�V�����J�E���^�[
int g_nPatternAnimItem;			//�A�j���[�V�����p�^�[��
int g_nTimeScore;				//�c�莞�ԂɑΉ������X�R�A
int g_nCntClick;				//���ł����񐔂��J�E���g

ITEM g_aItem[MAX_ITEM];			//�A�C�e���̏��
								//=============================================================================
								//�}�N����`
								//=============================================================================
#define ITEM_MOVE (1.18f)	//�A�C�e���̈ړ����x

								//=============================================================================
								//�A�C�e������������
								//=============================================================================
void InitItem(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntItem;

	//�A�C�e���̏��̏�����
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].nType = 0;
		g_aItem[nCntItem].bUse = false;
	}
	g_nCounterAnimItem = 0;
	g_nPatternAnimItem = 0;
	g_nTimeScore = 0;
	g_nCntClick = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "item400.png", &g_apTextureItem[0]);		//���H��
	D3DXCreateTextureFromFile(pDevice, "item200.png", &g_apTextureItem[1]);		//�����̉H

																				//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_ITEM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffItem, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

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

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// �A�C�e���I������
//=============================================================================
void UninitItem(void)
{
	//�e�N�X�`���̊J��
	for (int nCntItem = 0; nCntItem < ITEMTYPE_MAX; nCntItem++)
	{
		if (g_apTextureItem != NULL)
		{
			g_apTextureItem[nCntItem]->Release();
			g_apTextureItem[nCntItem] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//=============================================================================
// �A�C�e���X�V����
//=============================================================================
void UpdateItem(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	HUNTER*pHunter;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			g_nCounterAnimItem++;	//�J�E���^���Z
			if (g_nCounterAnimItem == 15)//����
			{
				g_nCounterAnimItem = 0;
				g_nPatternAnimItem++;
				if (g_nPatternAnimItem == 2)//����
				{
					g_nPatternAnimItem = 0;
				}
			}

			//�n���^�[���擾
			pHunter = GetHunter();

			if (pHunter->bDisp == true)
			{
				//���@���A�C�e���ɓ����������̓����蔻��
				if (g_aItem[nCntItem].pos.x <= pHunter->pos.x + (ITEM_SIZE / 2) &&
					g_aItem[nCntItem].pos.x >= pHunter->pos.x - (ITEM_SIZE / 2) &&
					g_aItem[nCntItem].pos.y <= pHunter->pos.y + (ITEM_SIZE / 2) &&
					g_aItem[nCntItem].pos.y >= pHunter->pos.y - (ITEM_SIZE / 2))
				{
					//���H��
					if (g_aItem[nCntItem].nType == ITEMTYPE_000)
					{
						//���g�p��
						g_aItem[nCntItem].bUse = false;

						PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��

						ScoreEffect(5000);

						//�s���`��Ԃ��X�g�b�N0�i�H�������Ȃ����)�ŁA�{�X��ɓ���Ƃ��ɁA�H�������邽�߂ɁA���C�t�𑝂₷
						if (pHunter->nLife > 0 && pHunter->nLife < HUNTER_LIFE / 4 && pHunter->nStock == 0)
						{
							pHunter->nLife = 100;
						}
						SetWing();	//�H���͂₷

									//�A�C�e�����Ƃ邱�ƂŃ{�X���o��
						SetBoss(D3DXVECTOR3(940, 240, 0.0f), D3DXVECTOR3(-1, -1, 0), 0);
					}

					//�����̉H
					if (g_aItem[nCntItem].nType == ITEMTYPE_001)
					{
						//���g�p��
						g_aItem[nCntItem].bUse = false;

						//�c�莞�Ԃ��擾
						g_nTimeScore = GetTime();

						//��̎g�p�񐔂��擾
						g_nCntClick = GetUseArrow();

						PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��

														//�X�R�A�ɁA���@�̎c��̗͂ƁA�c�莞�ԂƁA�c��X�g�b�N�ɉ��������̂������A�U���񐔕�����
						ScoreEffect(pHunter->nLife * 40 + g_nTimeScore * 100 + pHunter->nStock * 10000 + g_nCntClick*(-1));

						//�{�X��ŉH�����Ă���Ƃ��ɁA�H�𐶂₵�ăQ�[���N���A�̌��h�����悭����
						if (pHunter->nLife > 0 && pHunter->nLife < HUNTER_LIFE / 4 && pHunter->nStock == 0)
						{
							pHunter->nLife = 100;	//�H�������邽�߂ɁA���C�t�𑝂₷
							SetWing();
						}

						//�Q�[���N���A��
						SetGameState(GAMESTATE_END);
					}
				}
			}
			//�A�C�e���ɑ΂���d��
			g_aItem[nCntItem].move.y += ITEM_MOVE;
			g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;	//�ʒu�X�V
			g_aItem[nCntItem].move.y += (0.0 - g_aItem[nCntItem].move.x)*RATE_MOVE;

			//�n�ʂɂ���Ƃ�
			if (g_aItem[nCntItem].pos.y > SCREEN_HEIGHT - ITEM_SIZE + (ITEM_SIZE / 2) - 80)
			{
				g_aItem[nCntItem].pos.y = SCREEN_HEIGHT - ITEM_SIZE + (ITEM_SIZE / 2) - 80;
				g_aItem[nCntItem].move.y = 0;	//�n�ʂɂ���Ƃ��͏d�͂�^���Ȃ�
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y - (ITEM_SIZE / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y - (ITEM_SIZE / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y + (ITEM_SIZE / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y + (ITEM_SIZE / 2), 0.0f);

			//�e�N�X�`�����W���X�V
			pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimItem * 0.5f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimItem * 0.5f + 0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimItem * 0.5f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimItem * 0.5f + 0.5f, 1.0f);
		}
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// �A�C�e���`�揈��
//=============================================================================
void DrawItem(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntItem;

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�|���S���̕`��
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)//�A�C�e�����g�p���Ȃ�`��
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureItem[g_aItem[nCntItem].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
}

//=============================================================================
// �A�C�e���̐ݒ菈��
//=============================================================================
void SetItem(D3DXVECTOR3 pos, ITEMTYPE nType)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntItem;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{
			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].bUse = true;
			g_aItem[nCntItem].nType = nType;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y - (ITEM_SIZE / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y - (ITEM_SIZE / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y + (ITEM_SIZE / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y + (ITEM_SIZE / 2), 0.0f);

			break;
		}
	}
	//���_�f�[�^���A�����b�N����
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
//�A�C�e���̎擾
//=============================================================================
ITEM*GetItem(void)
{
	return &g_aItem[0];
}