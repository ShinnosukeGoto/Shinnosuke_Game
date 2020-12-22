//=============================================================================
//
// �i�C���X�g�[���[�Y�̏��� [item.cpp] (�A�C�e��)
// Author :�㓡�T�V��
//
//=============================================================================
#include "player.h"
#include "item.h"
#include "game.h"
#include "score.h"
#include "sound.h"
#include "time.h"
#include "input.h"
#include "attack.h"
#include "bg.h"
#include "block.h"
#include "magic.h"
#include "explosion.h"
#include "effect2.h"
#include "fade.h"
#include "game3.h"
#include "enemy.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_apTextureItem[ITEMTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffItem = NULL;

int g_nTimeScore;				//�c�莞�ԂɑΉ������X�R�A
int g_nCntClick;				//���ł����񐔂��J�E���g

ITEM g_aItem[MAX_ITEM];			//�A�C�e���̏��
bool g_bBgXI;					//�X�e�[�W1�̉����������ǂ���
bool g_bBlockXI;				//�u���b�N�ɉ����瓖������
MODE g_ModeItem;				//�Q�[����ʂɑΉ�

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
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].type = ITEMTYPE_000;
		g_aItem[nCntItem].nSize = 0;
		g_aItem[nCntItem].bUse = false;
	}

	//�O���[�o���ϐ��̏�����
	g_nTimeScore = 0;
	g_nCntClick = 0;
	g_bBgXI = false;
	g_bBlockXI = false;
	g_ModeItem = MODE_GAME;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/item000.png", &g_apTextureItem[0]);		//��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/item001.png", &g_apTextureItem[1]);		//�_�C�������h
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/item002.png", &g_apTextureItem[2]);		//�M
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/item003.png", &g_apTextureItem[3]);		//�n�[�g
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/item004.png", &g_apTextureItem[4]);		//���̕P
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/item005.png", &g_apTextureItem[5]);		//�P

																				//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_ITEM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffItem, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//���_���W�̐ݒ�i�E���Őݒ肷��j
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);

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

		pVtx += 4;
	}

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
		if (g_apTextureItem[nCntItem] != NULL)
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
	PLAYER*pPlayer;
	ATTACK*pAttack;
	MAGIC*pMagic;
	//�Q�[���̃X�e�[�W���擾
	g_ModeItem = GetMode();
	ENEMY*pEnemy = GetEnemy();
	int nFade = GetFade();	//�t�F�[�h�̏����l��

	//���_�o�b�t�@�����b�N
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			//���@���擾
			pPlayer = GetPlayer();
			//�U�����擾
			pAttack = GetAttack();
			//���@���擾
			pMagic = GetMagic();
			//�w�i�̉����������ǂ����擾
			g_bBgXI = GetBg();
			//�u���b�N�ɉ����瓖���������ǂ����擾
			g_bBlockXI = GetBlockX();

			if (pPlayer->bDisp == true)
			{
				//��
				if (g_aItem[nCntItem].type == ITEMTYPE_000)
				{
					//�U�����A�C�e���ɓ����������̓����蔻��
					if (pAttack->bUse == true)
					{
						if (g_aItem[nCntItem].pos.x <= pAttack->pos.x + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.x >= pAttack->pos.x - (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y <= pAttack->pos.y + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y >= pAttack->pos.y - (g_aItem[nCntItem].nSize / 2))
						{
							//��������
							SetExplosion(g_aItem[nCntItem].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aItem[nCntItem].nSize);

							//���g�p��
							g_aItem[nCntItem].bUse = false;

							SetItem(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y + 5.0f, 0.0f), ITEMTYPE_001, 60);	//�₪��ꂽ��_�C�������h���o�Ă���
						}
					}

					//���@���A�C�e���ɓ����������̓����蔻��
					if (pMagic->bUse == true)
					{
						if (g_aItem[nCntItem].pos.x <= pMagic->pos.x + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.x >= pMagic->pos.x - (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y <= pMagic->pos.y + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y >= pMagic->pos.y - (g_aItem[nCntItem].nSize / 2))
						{
							//��������
							SetExplosion(g_aItem[nCntItem].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aItem[nCntItem].nSize);

							//���g�p��
							g_aItem[nCntItem].bUse = false;

							SetItem(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y + 5.0f, 0.0f), ITEMTYPE_001, 60);	//�₪��ꂽ��_�C�������h���o�Ă���
						}
					}
				}

				//�M
				if (g_aItem[nCntItem].type == ITEMTYPE_002)
				{
					//�U�����A�C�e���ɓ����������̓����蔻��
					if (pAttack->bUse == true)
					{
						if (g_aItem[nCntItem].pos.x <= pAttack->pos.x + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.x >= pAttack->pos.x - (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y <= pAttack->pos.y + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y >= pAttack->pos.y - (g_aItem[nCntItem].nSize / 2))
						{
							//��������
							SetExplosion(g_aItem[nCntItem].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aItem[nCntItem].nSize);

							//���g�p��
							g_aItem[nCntItem].bUse = false;

							SetItem(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, 0.0f), ITEMTYPE_003, 50);	//�M����ꂽ��n�[�g���o�Ă���
						}
					}

					//���@���A�C�e���ɓ����������̓����蔻��
					if (pMagic->bUse == true)
					{
						if (g_aItem[nCntItem].pos.x <= pMagic->pos.x + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.x >= pMagic->pos.x - (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y <= pMagic->pos.y + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y >= pMagic->pos.y - (g_aItem[nCntItem].nSize / 2))
						{
							//��������
							SetExplosion(g_aItem[nCntItem].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aItem[nCntItem].nSize);

							//���g�p��
							g_aItem[nCntItem].bUse = false;

							SetItem(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y + 10.0f, 0.0f), ITEMTYPE_003, 50);	//�₪��ꂽ��n�[�g���o�Ă���
						}
					}
				}

				//���̕P
				if (g_aItem[nCntItem].type == ITEMTYPE_004)
				{
					//�G��|������A��炩��P���o�Ă���
					if (pEnemy->bUse == false)
					{
						//���g�p��
						g_aItem[nCntItem].bUse = false;
						SetItem(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, 0.0f), ITEMTYPE_005, PLAYER_SIZE);
					}
				}

				//�P
				if (g_aItem[nCntItem].type == ITEMTYPE_005)
				{
					//���ɂ������ړ�
					g_aItem[nCntItem].pos.y += 1.0;

					//���t�g���~�܂邩��A�A�C�e�����~�܂�
					if (g_aItem[nCntItem].pos.y > SCREEN_HEIGHT - PLAYER_SIZE + (PLAYER_SIZE / 2) - 80)
					{
						g_aItem[nCntItem].pos.y = SCREEN_HEIGHT - PLAYER_SIZE + (PLAYER_SIZE / 2) - 80;
					}
				}

				//���@���A�C�e���ɓ����������̓����蔻��
				if (g_aItem[nCntItem].pos.x <= pPlayer->pos.x + (g_aItem[nCntItem].nSize / 2) &&
					g_aItem[nCntItem].pos.x >= pPlayer->pos.x - (g_aItem[nCntItem].nSize / 2) &&
					g_aItem[nCntItem].pos.y <= pPlayer->pos.y + (g_aItem[nCntItem].nSize / 2) &&
					g_aItem[nCntItem].pos.y >= pPlayer->pos.y - (g_aItem[nCntItem].nSize / 2))
				{
					//�_�C�������h
					if (g_aItem[nCntItem].type == ITEMTYPE_001)
					{
						//���g�p��
						g_aItem[nCntItem].bUse = false;

						PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��

						ScoreEffect(4600);

					}

					//�n�[�g
					if (g_aItem[nCntItem].type == ITEMTYPE_003)
					{
						//���g�p��
						g_aItem[nCntItem].bUse = false;

						//�񕜃G�t�F�N�g
						SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE,EFFECTTYPE_000);

						//��
						pPlayer->nLife += 100;
						pPlayer->nMp += 50;

						PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��

						ScoreEffect(500);

					}

					//�P
					if (g_aItem[nCntItem].type == ITEMTYPE_005 && nFade == FADE_NONE)
					{
						//�c�莞�Ԃ��擾
						g_nTimeScore = GetTime();

						//��̎g�p�񐔂��擾
						g_nCntClick = GetUseAttack();

						PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��

						//�X�R�A�ɁA���@�̎c��̗͂ƁA�c�莞�ԂƁA�c��X�g�b�N�ɉ��������̂������A�U���񐔕�����
						ScoreEffect(pPlayer->nLife * 30 + g_nTimeScore * 100 + pPlayer->nStock * 10000 + g_nCntClick*(-1));

						//�Q�[���N���A��
						SetGameState3(GAMESTATE3_GAMECLEAR);
					}
				}
			}

			//�v���C���[�̑���ɑΉ������ړ��i�Q�[��1�j
			if (g_ModeItem == MODE_GAME && pPlayer->bDisp == true && g_bBgXI == true && g_bBlockXI == false)
			{
				if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
				{
					g_aItem[nCntItem].pos.x -= VALUE_MOVE;
				}

				if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
				{
					g_aItem[nCntItem].pos.x += VALUE_MOVE;
				}
			}

			//�Q�[��2�ŁA����ɂ����Ă���A�C�e��
			if (g_ModeItem == MODE_GAME2)
			{
				//���t�g�ɏ���Ă��邩��A�A�C�e���͉��Ɉړ�
				g_aItem[nCntItem].pos.y += 1.0;

				//���t�g���~�܂邩��A�A�C�e�����~�܂�
				if (g_aItem[nCntItem].pos.y > 435)
				{
					g_aItem[nCntItem].pos.y = 435;
				}
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y - (g_aItem[nCntItem].nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y - (g_aItem[nCntItem].nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y + (g_aItem[nCntItem].nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y + (g_aItem[nCntItem].nSize / 2), 0.0f);

		}
		pVtx += 4;
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
			pDevice->SetTexture(0, g_apTextureItem[g_aItem[nCntItem].type]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4* nCntItem, NUM_POLYGON);
		}
	}
}

//=============================================================================
// �A�C�e���̐ݒ菈��
//=============================================================================
void SetItem(D3DXVECTOR3 pos, ITEMTYPE type, int nSize)
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
			g_aItem[nCntItem].type = type;
			g_aItem[nCntItem].nSize = nSize;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y - (g_aItem[nCntItem].nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y - (g_aItem[nCntItem].nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y + (g_aItem[nCntItem].nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y + (g_aItem[nCntItem].nSize / 2), 0.0f);

			break;
		}
		pVtx += 4;
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