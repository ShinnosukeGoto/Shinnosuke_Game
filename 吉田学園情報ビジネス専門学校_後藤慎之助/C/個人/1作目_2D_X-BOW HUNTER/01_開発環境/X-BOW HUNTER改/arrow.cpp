//=============================================================================
//
// X-Bow Hunter [arrow.cpp]�̏����i�v���C���[�̖�j
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "hunter.h"
#include "arrow.h"
#include "explosion.h"
#include "enemy.h"
#include "boss.h"
#include "storm.h"
#include "input.h"
#include "item.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureArrow[ARROWTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffArrow = NULL;
ARROW g_aArrow[MAX_ARROW];	//��̏��
int g_nUseArrow;			//����g������

							//=============================================================================
							//��̏���������
							//=============================================================================
void InitArrow(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntArrow;

	//�e�̏��̏�����
	for (nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		g_aArrow[nCntArrow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aArrow[nCntArrow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aArrow[nCntArrow].bUse = false;
	}

	g_nUseArrow = 0;	//�g�����񐔂̏�����

						//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "arrow.png", &g_pTextureArrow[0]);
	D3DXCreateTextureFromFile(pDevice, "arrow200.png", &g_pTextureArrow[1]);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_ARROW, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffArrow, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

													//�e�̐���
	for (nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
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
	g_pVtxBuffArrow->Unlock();
}

//=============================================================================
// ��̏I������
//=============================================================================
void UninitArrow(void)
{
	//�ϐ��錾
	int nCntArrow;

	for (nCntArrow = 0; nCntArrow < ARROWTYPE_MAX; nCntArrow++)
	{
		//�e�N�X�`���̊J��
		if (g_pTextureArrow != NULL)
		{
			g_pTextureArrow[nCntArrow]->Release();
			g_pTextureArrow[nCntArrow] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffArrow != NULL)
	{
		g_pVtxBuffArrow->Release();
		g_pVtxBuffArrow = NULL;
	}
}

//=============================================================================
// ��̍X�V����
//=============================================================================
void UpdateArrow(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntArrow;
	VERTEX_2D *pVtx;
	int nDirect = 0;	//���_�t�߂Ō�����ς��邽�߂Ɏg�p
	ENEMY*pEnemy;
	int nCntEnemy;
	BOSS*pBoss;
	int nCntBoss;
	STORM*pStorm;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		if (g_aArrow[nCntArrow].bUse == true)
		{
			//�G���擾
			pEnemy = GetEnemy();
			//�{�X���擾
			pBoss = GetBoss();
			//�X�g�[�����擾
			pStorm = GetStorm();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->bUse == true) //�G���g�p����Ă���
				{
					//���X�L�[�g�̓����蔻��
					if (pEnemy->type == ENEMYTYPE_000)
					{
						if (g_aArrow[nCntArrow].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MOS / 2) &&
							g_aArrow[nCntArrow].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MOS / 2) &&
							g_aArrow[nCntArrow].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MOS / 2) &&
							g_aArrow[nCntArrow].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MOS / 2))
						{
							//�G�Ƀ_���[�W
							HitEnemy(nCntEnemy++, 10);

							//�����������_�Œe��HP��0�ɂ���
							g_aArrow[nCntArrow].nLife = 0;
						}
					}

					//���^���o�[�h�̓����蔻��
					if (pEnemy->type == ENEMYTYPE_001)
					{
						if (g_aArrow[nCntArrow].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET / 2) &&
							g_aArrow[nCntArrow].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET / 2) &&
							g_aArrow[nCntArrow].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET / 2) &&
							g_aArrow[nCntArrow].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET / 2))
						{
							//�G�Ƀ_���[�W
							HitEnemy(nCntEnemy++, 10);

							//�����������_�Œe��HP��0�ɂ���
							g_aArrow[nCntArrow].nLife = 0;
						}
					}

					//���^���o�[�h����̓����蔻��
					if (pEnemy->type == ENEMYTYPE_002)
					{
						if (g_aArrow[nCntArrow].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET�U / 2) &&
							g_aArrow[nCntArrow].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET�U / 2) &&
							g_aArrow[nCntArrow].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET�U / 2) &&
							g_aArrow[nCntArrow].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET�U / 2))
						{
							//�G�Ƀ_���[�W
							HitEnemy(nCntEnemy++, 10);

							//�����������_�Œe��HP��0�ɂ���
							g_aArrow[nCntArrow].nLife = 0;
						}
					}

					//�I�N�g�p�X�̓����蔻��
					if (pEnemy->type == ENEMYTYPE_003)
					{
						if (g_aArrow[nCntArrow].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_OCT / 2) &&
							g_aArrow[nCntArrow].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_OCT / 2) &&
							g_aArrow[nCntArrow].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_OCT / 2) &&
							g_aArrow[nCntArrow].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_OCT / 2))
						{
							//�G�Ƀ_���[�W
							HitEnemy(nCntEnemy++, 10);

							//�����������_�Œe��HP��0�ɂ���
							g_aArrow[nCntArrow].nLife = 0;
						}
					}

					//�S�[�X�g�̓����蔻��
					if (pEnemy->type == ENEMYTYPE_004)
					{
						if (g_aArrow[nCntArrow].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GHO / 2) - 10 &&
							g_aArrow[nCntArrow].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GHO / 2) + 10 &&
							g_aArrow[nCntArrow].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GHO / 2) - 10 &&
							g_aArrow[nCntArrow].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GHO / 2) + 10)
						{
							if (GetKeyboardPress(DIK_A))	//�p�����������ԂłȂ��Ƃ��ɓ����蔻�肪�o��
							{
								//�G�Ƀ_���[�W
								HitEnemy(nCntEnemy++, 10);

								//�����������_�Œe��HP��0�ɂ���
								g_aArrow[nCntArrow].nLife = 0;
							}
						}
					}

					//�O�\�N���V�̓����蔻��
					if (pEnemy->type == ENEMYTYPE_005)
					{
						if (g_aArrow[nCntArrow].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GSO / 2) &&
							g_aArrow[nCntArrow].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GSO / 2) &&
							g_aArrow[nCntArrow].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GSO / 2) &&
							g_aArrow[nCntArrow].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GSO / 2) - 10)
						{
							//�G�Ƀ_���[�W
							HitEnemy(nCntEnemy++, 10);

							//�����������_�Œe��HP��0�ɂ���
							g_aArrow[nCntArrow].nLife = 0;
						}
					}
				}
			}


			for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pBoss++)
			{
				if (pBoss->bUse == true) //�{�X���g�p����Ă���
				{
					//�G�̓����蔻��
					if (g_aArrow[nCntArrow].pos.x <= pBoss->pos.x + (BOSS_SIZE / 2) - 150 &&
						g_aArrow[nCntArrow].pos.x >= pBoss->pos.x - (BOSS_SIZE / 2) + 80 &&
						g_aArrow[nCntArrow].pos.y <= pBoss->pos.y + (BOSS_SIZE / 2) - 150 &&
						g_aArrow[nCntArrow].pos.y >= pBoss->pos.y - (BOSS_SIZE / 2) + 100)
					{
						//�G�Ƀ_���[�W
						HitBoss(nCntBoss++, 10);

						//�����������_�Œe��HP��0�ɂ���
						g_aArrow[nCntArrow].nLife = 0;
					}

					//��X�g�[���ɓ����������̏���
					if (pStorm->bUse == true)
					{
						//�e�ƃX�g�[���̓����蔻��
						if (g_aArrow[nCntArrow].pos.x <= pStorm->pos.x + (pStorm->nSize / 2) + 85 &&
							g_aArrow[nCntArrow].pos.x >= pStorm->pos.x - (pStorm->nSize / 2) - 85 &&
							g_aArrow[nCntArrow].pos.y <= pStorm->pos.y + (pStorm->nSize / 2) + 85 &&
							g_aArrow[nCntArrow].pos.y >= pStorm->pos.y - (pStorm->nSize / 2) - 85)
						{
							//�����������_�Œe��HP��0�ɂ���
							g_aArrow[nCntArrow].nLife = 0;
						}
					}
				}
			}

			//��̈ړ�
			g_aArrow[nCntArrow].pos += g_aArrow[nCntArrow].move;

			//���_�̍��W�ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x - (g_aArrow[nCntArrow].nSize / 2), g_aArrow[nCntArrow].pos.y - (g_aArrow[nCntArrow].nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x + (g_aArrow[nCntArrow].nSize / 2), g_aArrow[nCntArrow].pos.y - (g_aArrow[nCntArrow].nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x - (g_aArrow[nCntArrow].nSize / 2), g_aArrow[nCntArrow].pos.y + (g_aArrow[nCntArrow].nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x + (g_aArrow[nCntArrow].nSize / 2), g_aArrow[nCntArrow].pos.y + (g_aArrow[nCntArrow].nSize / 2), 0.0f);

			//�Ȏˎ��̖�ɑ΂���d��
			if (g_aArrow[nCntArrow].type == ARROWTYPE_BEND)
			{
				g_aArrow[nCntArrow].move.y += BOW_MOVE;
				g_aArrow[nCntArrow].pos.y += g_aArrow[nCntArrow].move.y;	//�ʒu�X�V
				g_aArrow[nCntArrow].move.y += (0.0 - g_aArrow[nCntArrow].move.x)*RATE_MOVE;
			}

			//��̗̑͂����炷
			g_aArrow[nCntArrow].nLife--;

			//�Ȏˎ��ɖ�̗̑͂��Q�Ƃ��āA���_�t�߂Ō�����ς���
			if (g_aArrow[nCntArrow].type == ARROWTYPE_BEND)
			{
				if (g_aArrow[nCntArrow].nLife < 33)	//��̗̑�66�̔���
				{
					//�e�N�X�`�����W���X�V(�E������)
					pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
				}
				else
				{
					//�e�N�X�`�����W���X�V(�E�����)
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}
			}

			//���ˎ��̓e�N�X�`���̌������Œ肷��
			if (g_aArrow[nCntArrow].type == ARROWTYPE_STRAIGHT)
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			//��̗̑͂�0
			if (g_aArrow[nCntArrow].nLife <= 0)
			{
				//��������
				SetExplosion(g_aArrow[nCntArrow].pos, D3DXCOLOR(255, 255, 255, 255), g_aArrow[nCntArrow].nSize);
				g_aArrow[nCntArrow].bUse = false;
			}
		}

		//��ʂ��o���疢�g�p��
		if (g_aArrow[nCntArrow].pos.x < 0 - (g_aArrow[nCntArrow].nSize / 2) || g_aArrow[nCntArrow].pos.x > SCREEN_WIDTH + (g_aArrow[nCntArrow].nSize / 2))
		{
			g_aArrow[MAX_ARROW].bUse = false;
		}
		if (g_aArrow[nCntArrow].pos.y < 0 - (g_aArrow[nCntArrow].nSize / 2) || g_aArrow[nCntArrow].pos.y > SCREEN_WIDTH + (g_aArrow[nCntArrow].nSize / 2))
		{
			g_aArrow[MAX_ARROW].bUse = false;
		}

		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffArrow->Unlock();

}

//=============================================================================
// ��̕`�揈��
//=============================================================================
void DrawArrow(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntArrow;

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffArrow, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//��̕`��
	for (nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		if (g_aArrow[nCntArrow].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureArrow[g_aArrow[nCntArrow].type]);

			//��g�p���Ȃ�`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntArrow * 4, NUM_POLYGON);
		}
	}
}

//=============================================================================
// ��̐ݒ菈��
//=============================================================================
void SetArrow(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, ARROWTYPE type, int nSize)
{
	//�ϐ��錾
	int nCntArrow;
	VERTEX_2D *pVtx;
	ITEM*pItem = GetItem();

	//�A�C�e���������Ă��Ȃ��Ƃ��Ɍ�������̐������Z
	if (pItem->bUse == false)
	{
		g_nUseArrow++;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		if (g_aArrow[nCntArrow].bUse == false)
		{
			//�ʒu��ݒ�
			g_aArrow[nCntArrow].pos = pos;
			g_aArrow[nCntArrow].nSize = nSize;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x - (nSize / 2), g_aArrow[nCntArrow].pos.y - (nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x + (nSize / 2), g_aArrow[nCntArrow].pos.y - (nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x - (nSize / 2), g_aArrow[nCntArrow].pos.y + (nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x + (nSize / 2), g_aArrow[nCntArrow].pos.y + (nSize / 2), 0.0f);

			//���_�J���[��ݒ�
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aArrow[nCntArrow].move = move;
			g_aArrow[nCntArrow].nLife = nLife;
			g_aArrow[nCntArrow].col = col;
			g_aArrow[nCntArrow].type = type;
			g_aArrow[nCntArrow].bUse = true;

			break;
		}
		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffArrow->Unlock();
}
//=============================================================================
//��̎擾
//=============================================================================
ARROW*GetArrow(void)
{
	return &g_aArrow[0];
}

//=============================================================================
//��̎g�p�񐔂̎擾
//=============================================================================
int GetUseArrow(void)
{
	return g_nUseArrow;
}