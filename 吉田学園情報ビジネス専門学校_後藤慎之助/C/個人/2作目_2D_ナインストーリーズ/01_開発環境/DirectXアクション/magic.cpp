//=============================================================================
//
// �i�C���X�g�[���[�Y [magic.cpp]�̏����i�v���C���[�̖��@�j
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "magic.h"
#include "explosion.h"
#include "enemy.h"
#include "input.h"
#include "effect.h"
#include "bg.h"
#include "block.h"
#include "effect2.h"
#include "sound.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureMagic[MAGICTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffMagic = NULL;

int g_nCounterAnimLi;			//���̃A�j���[�V�����J�E���^�[
int g_nPatternAnimLi;			//���̃A�j���[�V�����p�^�[��

MAGIC g_aMagic[MAX_MAGIC];	//���@�̏��
bool g_bBgXM;		//�w�i�������Ă��邩�ǂ����擾����
bool g_bBlockXM;	//�u���b�N�ɓ������Ă��邩�ǂ����擾����

//=============================================================================
//���@�̏���������
//=============================================================================
void InitMagic(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntMagic;

	//���@�̏��̏�����
	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		g_aMagic[nCntMagic].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagic[nCntMagic].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagic[nCntMagic].bUse = false;
	}

	g_nCounterAnimLi = 0;	//���̃A�j���[�V����
	g_nPatternAnimLi = 0;	//���̃A�j���[�V����
	g_bBgXM = false;
	g_bBlockXM = false;

						//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/magic000.png", &g_pTextureMagic[0]);	//��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/magic001.png", &g_pTextureMagic[1]);	//��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/magic002.png", &g_pTextureMagic[2]);	//�z��

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_MAGIC, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffMagic, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

													//�e�̐���
	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
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
	g_pVtxBuffMagic->Unlock();
}

//=============================================================================
// ���@�̏I������
//=============================================================================
void UninitMagic(void)
{
	//�ϐ��錾
	int nCntMagic;

	//�e�N�X�`���̊J��
	for (nCntMagic = 0; nCntMagic < MAGICTYPE_MAX; nCntMagic++)
	{
		if (g_pTextureMagic[nCntMagic] != NULL)
		{
			g_pTextureMagic[nCntMagic]->Release();
			g_pTextureMagic[nCntMagic] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffMagic != NULL)
	{
		g_pVtxBuffMagic->Release();
		g_pVtxBuffMagic = NULL;
	}
}

//=============================================================================
// ���@�̍X�V����
//=============================================================================
void UpdateMagic(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntMagic;
	VERTEX_2D *pVtx;
	ENEMY*pEnemy;
	int nCntEnemy;
	PLAYER*pPlayer;
	BLOCK*pBlock;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		if (g_aMagic[nCntMagic].bUse == true)
		{

			//�G���擾
			pEnemy = GetEnemy();
			//���@���擾
			pPlayer = GetPlayer();
			//�w�i���擾
			g_bBgXM = GetBg();
			//�u���b�N�ɓ������Ă��邩�ǂ������擾
			g_bBlockXM = GetBlockX();
			//�u���b�N���擾
			pBlock = GetBlock();

			//�G�ւ̓����蔻��
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->bUse == true) //�G���g�p����Ă���
				{
					//�r�[�g���̓����蔻��
					if (pEnemy->type == ENEMYTYPE_000)
					{
						//���̓����蔻��͖��@�̒��S���W
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MOS / 2) &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MOS / 2) &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MOS / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MOS / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 80);

									//�����������_�ŉ���HP��0�ɂ���
									g_aMagic[nCntMagic].nLife = 0;
								}
							}

						}

						//��,�z���̓����蔻��͑S��
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_MOS / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_MOS / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_MOS / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_MOS / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 150);
								}
							}

							//�z��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 50);

									//�񕜃G�t�F�N�g
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//���@����
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��
								}
							}
						}
					}

					//�J���X�̓����蔻��
					if (pEnemy->type == ENEMYTYPE_001)
					{
						//���̓����蔻��͖��@�̒��S���W
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET / 2) &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET / 2) &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 80);

									//�����������_�ŉ���HP��0�ɂ���
									g_aMagic[nCntMagic].nLife = 0;
								}
							}

						}

						//��,�z���̓����蔻��͑S��
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_MET / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_MET / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_MET / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_MET / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 150);
								}
							}

							//�z��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 50);

									//�񕜃G�t�F�N�g
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//���@����
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��
								}
							}
						}
					}

					//�J���X����̓����蔻��
					if (pEnemy->type == ENEMYTYPE_002)
					{
						//���̓����蔻��͖��@�̒��S���W
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET�U / 2) &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET�U / 2) &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET�U / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET�U / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 80);

									//�����������_�ŉ���HP��0�ɂ���
									g_aMagic[nCntMagic].nLife = 0;
								}
							}

						}

						//��,�z���̓����蔻��͑S��
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_MET�U / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_MET�U / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_MET�U / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_MET�U / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 150);
								}
							}

							//�z��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 50);

									//�񕜃G�t�F�N�g
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//���@����
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��
								}
							}
						}
					}

					//�K�C�R�c�ƃ_�[�N�K�C�R�c�̓����蔻��
					if (pEnemy->type == ENEMYTYPE_003 || pEnemy->type == ENEMYTYPE_008)
					{
						//���̓����蔻��͖��@�̒��S���W
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_OCT / 2) &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_OCT / 2) &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_OCT / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_OCT / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 80);

									//�����������_�ŉ���HP��0�ɂ���
									g_aMagic[nCntMagic].nLife = 0;
								}
							}

			
						}

						//���A�z���̓����蔻��͑S��
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_OCT / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_OCT / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_OCT / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_OCT / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 150);
								}
							}

							//�z��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 50);

									//�񕜃G�t�F�N�g
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//���@����
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��
								}
							}
						}
					}

					//�C�m�V�V�̓����蔻��
					if (pEnemy->type == ENEMYTYPE_004)
					{
						//���̓����蔻��͖��@�̒��S���W
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GHO / 2) &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GHO / 2) &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GHO / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GHO / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 80);

									//�����������_�ŉ���HP��0�ɂ���
									g_aMagic[nCntMagic].nLife = 0;
								}
							}

						}

						//��,�z���̓����蔻��͑S��
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_GHO / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_GHO / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_GHO / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_GHO / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 150);
								}
							}

							//�z��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 50);

									//�񕜃G�t�F�N�g
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//���@����
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��
								}
							}
						}
					}

					//�R�E�����̓����蔻��
					if (pEnemy->type == ENEMYTYPE_005)
					{
						//���̓����蔻��͖��@�̒��S���W
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GSO / 2) &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GSO / 2) &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GSO / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GSO / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 80);

									//�����������_�ŉ���HP��0�ɂ���
									g_aMagic[nCntMagic].nLife = 0;
								}
							}

						}

						//���A�z���̓����蔻��͑S��
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_GSO / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_GSO / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_GSO / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_GSO / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 150);
								}
							}

							//�z��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 50);

									//�񕜃G�t�F�N�g
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//���@����
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��
								}
							}
						}
					}

					//�A�C�X�h���S���̓����蔻��
					if (pEnemy->type == ENEMYTYPE_006)
					{
						//���̓����蔻��͖��@�̒��S���W
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_BOSS_X / 2) - 60 &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_BOSS_X / 2) + 60 &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_BOSS_Y / 2) + 80)
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 90);

									//�����������_�ŉ���HP��0�ɂ���
									g_aMagic[nCntMagic].nLife = 0;
								}
							}

						}

						//���A�z���̓����蔻��͑S��
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_BOSS_X / 2) - 60 &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_BOSS_X / 2) + 60 &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_BOSS_Y / 2) + 100)
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 65);
								}
							}

							//�z��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 50);

									//�񕜃G�t�F�N�g
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//���@����
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��
								}
							}
						}
					}

					//�s���j�A�̓����蔻��
					if (pEnemy->type == ENEMYTYPE_007)
					{
						//���̓����蔻��͖��@�̒��S���W
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_PIR / 2) &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_PIR / 2) &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_PIR / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_PIR / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 80);

									//�����������_�ŉ���HP��0�ɂ���
									g_aMagic[nCntMagic].nLife = 0;
								}
							}
						}

						//���A�z���̓����蔻��͑S��
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_PIR / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_PIR / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_PIR / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_PIR / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 150);
								}
							}

							//�z��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//���i�q�b�g�Ȃ������߁A�m�[�}�����̂�
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 50);

									//�񕜃G�t�F�N�g
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//���@����
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��
								}
							}
						}
					}
				}
			}

			//�u���b�N�ւ̓����蔻��
			for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
			{
				if (pBlock->bUse == true) //�u���b�N���g�p����Ă���
				{
					//���A�󂹂�u���b�N�ւ̓����蔻��
					if (pBlock->type == BLOCKTYPE_006 || pBlock->type == BLOCKTYPE_009)
					{
						//���̓����蔻��͖��@�̒��S���W
						if (g_aMagic[nCntMagic].pos.x <= pBlock->pos.x + (pBlock->nSizeX / 2) &&
							g_aMagic[nCntMagic].pos.x >= pBlock->pos.x - (pBlock->nSizeX / 2) &&
							g_aMagic[nCntMagic].pos.y <= pBlock->pos.y + (pBlock->nSizeY / 2) &&
							g_aMagic[nCntMagic].pos.y >= pBlock->pos.y - (pBlock->nSizeY / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								//�u���b�N�Ƀ_���[�W
								pBlock->nLife = 0;
							}
						}

						//���̓����蔻��͗��S��
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pBlock->pos.x + (pBlock->nSizeX / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pBlock->pos.x - (pBlock->nSizeX / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pBlock->pos.y + (pBlock->nSizeY / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pBlock->pos.y - (pBlock->nSizeY / 2))
						{
							//��
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								//�u���b�N�Ƀ_���[�W
								pBlock->nLife = 0;
							}
						}
					}
				}
			}

			//�v���C���[�̑���ɑΉ������ړ�
			if (pPlayer->bDisp == true && g_bBgXM == true && g_bBlockXM == false)
			{
				if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
				{
					g_aMagic[nCntMagic].pos.x -= VALUE_MOVE;
				}

				if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
				{
					g_aMagic[nCntMagic].pos.x += VALUE_MOVE;
				}
			}

			//���̖��@�̈ړ�
			if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
			{
				//���_�̍��W�ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y - (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y - (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y + (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y + (g_aMagic[nCntMagic].nSize / 2), 0.0f);

				//�d��
				g_aMagic[nCntMagic].move.y += GRAVITY;

				//�ʒu�X�V
				g_aMagic[nCntMagic].pos.x += g_aMagic[nCntMagic].move.x;
				g_aMagic[nCntMagic].pos.y += g_aMagic[nCntMagic].move.y;
			}

			//���̖��@�̈ړ�
			else if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
			{
				//�A�j���[�V����
				g_nCounterAnimLi++;	//�J�E���^���Z
				if (g_nCounterAnimLi == 3)//����
				{
					g_nCounterAnimLi = 0;
					g_nPatternAnimLi++;
					if (g_nPatternAnimLi == 2)//����
					{
						g_nPatternAnimLi = 0;
					}
				}

				//�e�N�X�`�����W�̐ݒ�
				if (g_nPatternAnimLi == 0)
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}
				else if (g_nPatternAnimLi == 1)
				{
					pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				}

				g_aMagic[nCntMagic].pos.y += g_aMagic[nCntMagic].move.y;

				//���_�̍��W�ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (g_aMagic[nCntMagic].nSize / 2), 0 - (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (g_aMagic[nCntMagic].nSize / 2), 0 - (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y + (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y + (g_aMagic[nCntMagic].nSize / 2), 0.0f);
			}

			//�z���̖��@�̈ړ�
			else if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
			{
				//���_�̍��W�ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y - (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y - (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y + (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y + (g_aMagic[nCntMagic].nSize / 2), 0.0f);

				//���@�̈ʒu���擾
				pPlayer = GetPlayer();

				float fPposx = pPlayer->pos.x, fPposy = pPlayer->pos.y;	//���@�̍��W
				float fMposx = g_aMagic[nCntMagic].pos.x, fMposy = g_aMagic[nCntMagic].pos.y;		//�z�����@�̍��W
				float fAngle;											//�p�x

				fAngle = atan2f((fMposx - fPposx), (fMposy - fPposy));	//�p�x�����߂�

				g_aMagic[nCntMagic].move.x = -sinf(fAngle)*7.4f;		//X���W�X�V
				g_aMagic[nCntMagic].move.y = -cosf(fAngle)*7.4f;		//Y���W�X�V

				//�ʒu�X�V
				g_aMagic[nCntMagic].pos.x += g_aMagic[nCntMagic].move.x;
				g_aMagic[nCntMagic].pos.y += g_aMagic[nCntMagic].move.y;
			}

			//���@�̗̑͂����炷
			g_aMagic[nCntMagic].nLife--;

			//���@�̗̑͂�0
			if (g_aMagic[nCntMagic].nLife <= 0)
			{
				g_aMagic[nCntMagic].bUse = false;
			}

			//���̂Ƃ��̃G�t�F�N�g
			if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
			{
				SetEffect(g_aMagic[nCntMagic].pos, D3DXCOLOR(g_aMagic[nCntMagic].col.r, g_aMagic[nCntMagic].col.g, g_aMagic[nCntMagic].col.b, 1.0f)
					, (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].nLife / 4, DIRECT_RIGHT);
			}

			//���̂Ƃ��̃G�t�F�N�g
			else if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
			{
				SetEffect(g_aMagic[nCntMagic].pos, D3DXCOLOR(g_aMagic[nCntMagic].col.r, g_aMagic[nCntMagic].col.g, g_aMagic[nCntMagic].col.b, 1.0f)
					, (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].nLife / 4, DIRECT_RIGHT);

				if (g_aMagic[nCntMagic].pos.y > SCREEN_HEIGHT - PLAYER_SIZE + (PLAYER_SIZE / 2) - 80)
				{
					g_aMagic[nCntMagic].pos.y = SCREEN_HEIGHT - PLAYER_SIZE + (PLAYER_SIZE / 2) - 80;
					//��������
					SetExplosion(g_aMagic[nCntMagic].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 125);
				}
			}

			//�z���̂Ƃ��̃G�t�F�N�g
			else if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
			{
				SetEffect(g_aMagic[nCntMagic].pos, D3DXCOLOR(g_aMagic[nCntMagic].col.r, g_aMagic[nCntMagic].col.g, g_aMagic[nCntMagic].col.b, 1.0f)
					, (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].nLife / 4, DIRECT_RIGHT);
			}
		}

		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffMagic->Unlock();

}

//=============================================================================
// ���@�̕`�揈��
//=============================================================================
void DrawMagic(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntMagic;

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffMagic, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���@�̕`��
	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		if (g_aMagic[nCntMagic].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMagic[g_aMagic[nCntMagic].type]);

			//���@���g�p���Ȃ�`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMagic * 4, NUM_POLYGON);
		}
	}
}

//=============================================================================
// ���@�̐ݒ菈��
//=============================================================================
void SetMagic(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, MAGICTYPE type, int nSize)
{
	//�ϐ��錾
	int nCntMagic;
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		if (g_aMagic[nCntMagic].bUse == false)
		{
			//�ʒu��ݒ�
			g_aMagic[nCntMagic].pos = pos;
			g_aMagic[nCntMagic].nSize = nSize;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (nSize / 2), g_aMagic[nCntMagic].pos.y - (nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (nSize / 2), g_aMagic[nCntMagic].pos.y - (nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (nSize / 2), g_aMagic[nCntMagic].pos.y + (nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (nSize / 2), g_aMagic[nCntMagic].pos.y + (nSize / 2), 0.0f);

			//���_�J���[��ݒ�
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aMagic[nCntMagic].move = move;
			g_aMagic[nCntMagic].nLife = nLife;
			g_aMagic[nCntMagic].col = col;
			g_aMagic[nCntMagic].type = type;
			//g_aMagic[nCntMagic].nDirect = nDirect;
			g_aMagic[nCntMagic].bUse = true;

			break;
		}
		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffMagic->Unlock();
}
//=============================================================================
//���@�̎擾
//=============================================================================
MAGIC*GetMagic(void)
{
	return &g_aMagic[0];
}