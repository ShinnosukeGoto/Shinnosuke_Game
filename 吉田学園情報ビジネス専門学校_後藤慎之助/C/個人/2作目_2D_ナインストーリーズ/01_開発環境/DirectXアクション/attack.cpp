//=============================================================================
//
// �i�C���X�g�[���[�Y [attack.cpp]�̏����i�v���C���[�̍U���j
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "attack.h"
#include "explosion.h"
#include "enemy.h"
#include "input.h"
#include "effect.h"
#include "block.h"


//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureAttack[ATTACKTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffAttack = NULL;
ATTACK g_aAttack[MAX_ATTACK];	//�U���̏��
int g_nUseAttack;			//�U�����g������

//=============================================================================
//�U���̏���������
//=============================================================================
void InitAttack(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntAttack;

	//�e�̏��̏�����
	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		g_aAttack[nCntAttack].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].bUse = false;
	}

	g_nUseAttack = 0;	//�g�����񐔂̏�����

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/attack004.png", &g_pTextureAttack[0]);	//��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/attack002.png", &g_pTextureAttack[1]);	//��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/attack002.png", &g_pTextureAttack[2]);	//��(���U���p)
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/attack200.png", &g_pTextureAttack[3]);	//�|
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/attack200.png", &g_pTextureAttack[4]);	//�|(���U���p)

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_ATTACK, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffAttack, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffAttack->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

													//�e�̐���
	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
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

		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffAttack->Unlock();
}

//=============================================================================
// �U���̏I������
//=============================================================================
void UninitAttack(void)
{
	//�ϐ��錾
	int nCntAttack;

	//�e�N�X�`���̊J��
	for (nCntAttack = 0; nCntAttack < ATTACKTYPE_MAX; nCntAttack++)
	{
		if (g_pTextureAttack[nCntAttack] != NULL)
		{
			g_pTextureAttack[nCntAttack]->Release();
			g_pTextureAttack[nCntAttack] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffAttack != NULL)
	{
		g_pVtxBuffAttack->Release();
		g_pVtxBuffAttack = NULL;
	}
}

//=============================================================================
// �U���̍X�V����
//=============================================================================
void UpdateAttack(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntAttack;
	VERTEX_2D *pVtx;
	ENEMY*pEnemy;
	int nCntEnemy;
	BLOCK*pBlock;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffAttack->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == true)
		{
			//�G���擾
			pEnemy = GetEnemy();
			
			//�u���b�N���擾
			pBlock = GetBlock();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->bUse == true) //�G���g�p����Ă���
				{
					if (pEnemy->state == ENEMYSTATE_NORMAL)
					{
						//�r�[�g���̓����蔻��
						if (pEnemy->type == ENEMYTYPE_000)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MOS / 2) &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MOS / 2) &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MOS / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MOS / 2))
							{
								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 60);
								}

								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 40);
								}

								//�}�~���U��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 80);
								}

								//�|
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 30);
									//�����������_�Œe��HP��0�ɂ���
									g_aAttack[nCntAttack].fLife = 0;
								}

							}
						}

						//�J���X�̓����蔻��
						if (pEnemy->type == ENEMYTYPE_001)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET / 2) &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET / 2) &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET / 2))
							{
								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 60);
								}

								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 40);
								}

								//�}�~���U��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 80);
								}

								//�|
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 30);
									//�����������_�Œe��HP��0�ɂ���
									g_aAttack[nCntAttack].fLife = 0;
								}
							}
						}

						//�J���X����̓����蔻��
						if (pEnemy->type == ENEMYTYPE_002)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET�U / 2) &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET�U / 2) &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET�U / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET�U / 2))
							{
								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 60);
								}

								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 40);
								}

								//�}�~���U��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 80);
								}

								//�|
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 30);
									//�����������_�Œe��HP��0�ɂ���
									g_aAttack[nCntAttack].fLife = 0;
								}
							}
						}

						//�K�C�R�c�ƃ_�[�N�K�C�R�c�̓����蔻��
						if (pEnemy->type == ENEMYTYPE_003 || pEnemy->type == ENEMYTYPE_008)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_OCT / 2) &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_OCT / 2) &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_OCT / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_OCT / 2))
							{
								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 60);

								}

								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 40);
								}

								//�}�~���U��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 80);
								}

								//�|
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 30);
									//�����������_�Œe��HP��0�ɂ���
									g_aAttack[nCntAttack].fLife = 0;
								}
							}
						}

						//�C�m�V�V�̓����蔻��
						if (pEnemy->type == ENEMYTYPE_004)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GHO / 2) &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GHO / 2) &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GHO / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GHO / 2))
							{
								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 60);

								}

								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 40);
								}

								//�}�~���U��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 80);
								}

								//�|
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 30);
									//�����������_�Œe��HP��0�ɂ���
									g_aAttack[nCntAttack].fLife = 0;
								}
							}
						}

						//�R�E�����̓����蔻��
						if (pEnemy->type == ENEMYTYPE_005)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GSO / 2) &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GSO / 2) &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GSO / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GSO / 2))
							{
								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 60);

								}

								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 40);
								}

								//�}�~���U��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 80);
								}

								//�|
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//�G�Ƀ_���[�W(�R�E�����͋|��ђ�)
									HitEnemy(nCntEnemy, 30);
								}
							}
						}

						//�A�C�X�h���S���̓����蔻��
						if (pEnemy->type == ENEMYTYPE_006)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_BOSS_X / 2) - 60 &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_BOSS_X / 2) + 60 &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_BOSS_Y / 2) + 80)
							{
								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 40);

								}

								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 30);
								}

								//�}�~���U��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 60);
								}

								//�|
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 25);
									//�����������_�Œe��HP��0�ɂ���
									g_aAttack[nCntAttack].fLife = 0;
								}
							}
						}

						//�s���j�A�̓����蔻��
						if (pEnemy->type == ENEMYTYPE_007)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_PIR / 2) &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_PIR / 2) &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_PIR / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_PIR / 2))
							{
								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 60);

								}

								//��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 40);
								}

								//�}�~���U��
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 80);
								}

								//�|
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//�G�Ƀ_���[�W
									HitEnemy(nCntEnemy, 30);
									//�����������_�Œe��HP��0�ɂ���
									g_aAttack[nCntAttack].fLife = 0;
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
					//�󂹂�u���b�N�ւ̓����蔻��
					if (pBlock->type == BLOCKTYPE_009)
					{
						//�����蔻��͍U���̒��S���W
						if (g_aAttack[nCntAttack].pos.x <= pBlock->pos.x + (pBlock->nSizeX / 2) &&
							g_aAttack[nCntAttack].pos.x >= pBlock->pos.x - (pBlock->nSizeX / 2) &&
							g_aAttack[nCntAttack].pos.y <= pBlock->pos.y + (pBlock->nSizeY / 2) &&
							g_aAttack[nCntAttack].pos.y >= pBlock->pos.y - (pBlock->nSizeY / 2))
						{
							//�u���b�N�Ƀ_���[�W
							pBlock->nLife = 0;
						}
					}
				}
			}

			//�U���̗̑͂����炷
			g_aAttack[nCntAttack].fLife--;

			//�U���̗̑͂�0
			if (g_aAttack[nCntAttack].fLife <= 0)
			{
				////��������
				//SetExplosion(g_aAttack[nCntAttack].pos, D3DXCOLOR(255, 255, 255, 255), g_aAttack[nCntAttack].nSize);
				g_aAttack[nCntAttack].bUse = false;
			}

			//���_�̍��W�ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x - (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].pos.y - (g_aAttack[nCntAttack].nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x + (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].pos.y - (g_aAttack[nCntAttack].nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x - (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].pos.y + (g_aAttack[nCntAttack].nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x + (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].pos.y + (g_aAttack[nCntAttack].nSize / 2), 0.0f);

			//���̈ړ���
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
			{
				if (g_aAttack[nCntAttack].fLife < 11)
				{
					//�U���̈ړ��ʂ͏��Ȃ�
					g_aAttack[nCntAttack].pos += g_aAttack[nCntAttack].move*0.6;
				}
				else
				{
					//�U���̈ړ���
					g_aAttack[nCntAttack].pos += g_aAttack[nCntAttack].move;
				}
			}
			//���̈ړ���
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
			{
				if (g_aAttack[nCntAttack].fLife < 11)
				{
					//�U���̈ړ���
					g_aAttack[nCntAttack].pos += g_aAttack[nCntAttack].move;
				}
				else
				{
					//�U���̈ړ��ʂ͏��Ȃ�
					g_aAttack[nCntAttack].pos += g_aAttack[nCntAttack].move*0.7;
				}
			}
			//�|�̈ړ���
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
			{
				//�U���̈ړ���
				g_aAttack[nCntAttack].pos += g_aAttack[nCntAttack].move;
			}

			//�����
			if (g_aAttack[nCntAttack].nDirect == DIRECT_UP)
			{
				//���E���̂Ƃ��̃G�t�F�N�g
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);

					SetEffect(g_aAttack[nCntAttack].pos, D3DXCOLOR(g_aAttack[nCntAttack].col.r, g_aAttack[nCntAttack].col.g, g_aAttack[nCntAttack].col.b, 1.0f)
						, (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].fLife / 2, DIRECT_UP);
				}

				//�|�̂Ƃ��̃G�t�F�N�g
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[0].tex = D3DXVECTOR2(0.5f, 1.0f);
				}
			}

			//������
			if (g_aAttack[nCntAttack].nDirect == DIRECT_DOWN)
			{
				//���E���̂Ƃ��̃G�t�F�N�g
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

					SetEffect(g_aAttack[nCntAttack].pos, D3DXCOLOR(g_aAttack[nCntAttack].col.r, g_aAttack[nCntAttack].col.g, g_aAttack[nCntAttack].col.b, 1.0f)
						, (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].fLife / 2, DIRECT_DOWN);
				}

				//�|�̂Ƃ��̃G�t�F�N�g(�E����)
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}

				//�|�̂Ƃ��̃G�t�F�N�g(������)
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				}
			}

			//�E����
			if (g_aAttack[nCntAttack].nDirect == DIRECT_RIGHT)
			{
				//���E���̂Ƃ��̃G�t�F�N�g
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

					SetEffect(g_aAttack[nCntAttack].pos, D3DXCOLOR(g_aAttack[nCntAttack].col.r, g_aAttack[nCntAttack].col.g, g_aAttack[nCntAttack].col.b, 1.0f)
						, (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].fLife / 2, DIRECT_RIGHT);
				}

				//�|�̂Ƃ��̃G�t�F�N�g
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
				}
			}

			//������
			if (g_aAttack[nCntAttack].nDirect == DIRECT_LEFT)
			{
				//���E���̂Ƃ��̃G�t�F�N�g
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);

					SetEffect(g_aAttack[nCntAttack].pos, D3DXCOLOR(g_aAttack[nCntAttack].col.r, g_aAttack[nCntAttack].col.g, g_aAttack[nCntAttack].col.b, 1.0f)
						, (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].fLife / 2, DIRECT_LEFT);
				}

				//�|�̂Ƃ��̃G�t�F�N�g
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
				}
			}

		}

		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffAttack->Unlock();

}

//=============================================================================
// �U���̕`�揈��
//=============================================================================
void DrawAttack(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntAttack;

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffAttack, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�U���̕`��
	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureAttack[g_aAttack[nCntAttack].type]);

			//�U�����g�p���Ȃ�`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAttack * 4, NUM_POLYGON);
		}
	}
}

//=============================================================================
// �U���̐ݒ菈��
//=============================================================================
void SetAttack(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fLife, D3DXCOLOR col, ATTACKTYPE type, int nSize, DIRECT nDirect)
{
	//�ϐ��錾
	int nCntAttack;
	VERTEX_2D *pVtx;
	/*ITEM*pItem = GetItem();*/

	//�������U���̐������Z
	g_nUseAttack++;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffAttack->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == false)
		{
			//�ʒu��ݒ�
			g_aAttack[nCntAttack].pos = pos;
			g_aAttack[nCntAttack].nSize = nSize;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x - (nSize / 2), g_aAttack[nCntAttack].pos.y - (nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x + (nSize / 2), g_aAttack[nCntAttack].pos.y - (nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x - (nSize / 2), g_aAttack[nCntAttack].pos.y + (nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x + (nSize / 2), g_aAttack[nCntAttack].pos.y + (nSize / 2), 0.0f);

			//���_�J���[��ݒ�
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aAttack[nCntAttack].move = move;
			g_aAttack[nCntAttack].fLife = fLife;
			g_aAttack[nCntAttack].col = col;
			g_aAttack[nCntAttack].type = type;
			g_aAttack[nCntAttack].nDirect = nDirect;
			g_aAttack[nCntAttack].bUse = true;

			break;
		}
		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffAttack->Unlock();
}
//=============================================================================
//�U���̎擾
//=============================================================================
ATTACK*GetAttack(void)
{
	return &g_aAttack[0];
}

//=============================================================================
//�U���̎g�p�񐔂̎擾
//=============================================================================
int GetUseAttack(void)
{
	return g_nUseAttack;
}