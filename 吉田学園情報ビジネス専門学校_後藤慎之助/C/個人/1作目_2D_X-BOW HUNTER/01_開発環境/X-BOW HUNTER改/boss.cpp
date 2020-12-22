//=============================================================================
//
// DirectX�V���[�e�B���O [boss.cpp] (�t�@�C���[�o�[�h)
// Author :�㓡�T�V��
//
//=============================================================================
#include "boss.h"
#include "explosion.h"
#include "bullet.h"
#include <time.h>
#include "game.h"
#include "score.h"
#include "item.h"
#include "sound.h"
#include "enemy.h"
#include "title.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_apTextureBoss[MAX_TYPE_BOSS] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBoss = NULL;

int g_nCounterAnimBoss;			//�A�j���[�V�����J�E���^�[
int g_nPatternAnimBoss;			//�A�j���[�V�����p�^�[��
int g_nCntBoss = 1;				//�e�������_���ɑłƂ��Ɏg�p

BOSS g_aBoss[MAX_BOSS];			//�{�X�̏��
GAMELEVEL g_nBossLevel;			//�{�X�̃��x��
								//=============================================================================
								//�{�X����������
								//=============================================================================
void InitBoss(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntBoss;

	//�e�̏��̏�����
	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		g_aBoss[nCntBoss].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].state = BOSSSTATE_NORMAL;
		g_aBoss[nCntBoss].nType = 0;
		g_aBoss[nCntBoss].bUse = false;
	}
	g_nBossLevel = GetTitle();				//��Փx���擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "boss000.png", &g_apTextureBoss[0]);	//�t�@�C���[�o�[�h

																			//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BOSS, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBoss, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

	pVtx[0].pos = D3DXVECTOR3(350.0f, 50.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(430.0f, 50.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(350.0f, 130.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(430.0f, 130.0f, 0.0f);

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
	g_pVtxBuffBoss->Unlock();
}

//=============================================================================
// �{�X�I������
//=============================================================================
void UninitBoss(void)
{
	//�ϐ��錾
	int nCntTex;

	//�e�N�X�`���̊J��
	for (nCntTex = 0; nCntTex < MAX_TYPE_BOSS; nCntTex++)
	{
		if (g_apTextureBoss != NULL)
		{
			g_apTextureBoss[nCntTex]->Release();
			g_apTextureBoss[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}
}

//=============================================================================
// �{�X�X�V����
//=============================================================================
void UpdateBoss(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntBoss;	//�{�X�𐔂���

					//�A�j���[�V����
	g_nCounterAnimBoss++;	//�J�E���^���Z
	if (g_nCounterAnimBoss == 9)//����
	{
		g_nCounterAnimBoss = 0;
		g_nPatternAnimBoss++;
		if (g_nPatternAnimBoss == 3)//����
		{
			g_nPatternAnimBoss = 0;
		}
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse == true)
		{
			switch (g_aBoss[nCntBoss].state)
			{
			case BOSSSTATE_NORMAL:
				//�s���`�Ȃ�A�s���`��Ԃƃ��[�v
				if (g_aBoss[nCntBoss].nLife < BOSS_LIFE / 4)
				{
					g_aBoss[nCntBoss].state = BOSSSTATE_PINCH;

					//�ʏ�`��ɖ߂�����
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

				}
				break;
			case BOSSSTATE_PINCH:
				//��Փx�m�[�}��
				if (g_nBossLevel == GAMELEVEL_NORMAL)
				{
					//�K�E �Ǐ]�Ή��u���X
					if ((rand() % (g_nCntBoss * 50)) == 0)
					{
						PlaySound(SOUND_LABEL_SE_SHOT3);	//�V���b�g��
						SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(-30, 0, 0), BULLETTYPE_BOSS_PINCH, 95, D3DXCOLOR(255, 255, 255, 255), 30);
					}
				}
				//��Փx�n�[�h
				if (g_nBossLevel == GAMELEVEL_HARD)
				{
					//�K�E �Ǐ]�Ή��u���X
					if ((rand() % (g_nCntBoss * 25)) == 0)
					{
						PlaySound(SOUND_LABEL_SE_SHOT3);	//�V���b�g��
						SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(-30, 0, 0), BULLETTYPE_BOSS_PINCH, 95, D3DXCOLOR(255, 255, 255, 255), 30);
					}
				}

				g_aBoss[nCntBoss].nCounterState--;
				if (g_aBoss[nCntBoss].nCounterState <= 0)
				{
					g_aBoss[nCntBoss].state = BOSSSTATE_NORMAL;

					//�s���`�`��ɂ��鏈��
					pVtx[0].col = D3DCOLOR_RGBA(246, 120, 59, 255);
					pVtx[1].col = D3DCOLOR_RGBA(246, 120, 59, 255);
					pVtx[2].col = D3DCOLOR_RGBA(246, 120, 59, 255);
					pVtx[3].col = D3DCOLOR_RGBA(246, 120, 59, 255);
				}
				break;
			case BOSSSTATE_DAMAGE:
				g_aBoss[nCntBoss].nCounterState--;
				if (g_aBoss[nCntBoss].nCounterState <= 0)
				{
					g_aBoss[nCntBoss].state = BOSSSTATE_NORMAL;

					//�ʏ�`��ɖ߂�����
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				break;
			}

			//��Փx�m�[�}��
			if (g_nBossLevel == GAMELEVEL_NORMAL)
			{
				//�e�̔���
				if ((rand() % (g_nCntBoss * 100)) == 0)
				{
					SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(-10, 20, 0), BULLETTYPE_BOSS, 50, D3DXCOLOR(255, 255, 255, 255), 10);
				}

				//�e�̔���
				if ((rand() % (g_nCntBoss * 100)) == 0)
				{
					SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(0, 20, 0), BULLETTYPE_BOSS, 50, D3DXCOLOR(255, 255, 255, 255), 10);
				}

				//�e�̔���
				if ((rand() % (g_nCntBoss * 100)) == 0)
				{
					SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(10, 20, 0), BULLETTYPE_BOSS, 50, D3DXCOLOR(255, 255, 255, 255), 10);
				}

				//�{�X�̈ړ�
				g_aBoss[nCntBoss].pos -= g_aBoss[nCntBoss].move * 3;
			}
			//��Փx�n�[�h
			if (g_nBossLevel == GAMELEVEL_HARD)
			{
				//�e�̔���
				if ((rand() % (g_nCntBoss * 20)) == 0)
				{
					SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(-10, 20, 0), BULLETTYPE_BOSS, 50, D3DXCOLOR(255, 255, 255, 255), 10);
				}

				//�e�̔���
				if ((rand() % (g_nCntBoss * 20)) == 0)
				{
					SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(0, 20, 0), BULLETTYPE_BOSS, 50, D3DXCOLOR(255, 255, 255, 255), 10);
				}

				//�e�̔���
				if ((rand() % (g_nCntBoss * 20)) == 0)
				{
					SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(10, 20, 0), BULLETTYPE_BOSS, 50, D3DXCOLOR(255, 255, 255, 255), 10);
				}

				//�{�X�̈ړ�
				g_aBoss[nCntBoss].pos -= g_aBoss[nCntBoss].move * 5;
			}

			//�{�X�̈ړ��͈́iX��)
			if (g_aBoss[nCntBoss].pos.x < 0 + (BOSS_SIZE / 2) + 300 || g_aBoss[nCntBoss].pos.x > SCREEN_WIDTH - (BOSS_SIZE / 2))
			{
				g_aBoss[nCntBoss].move.x *= -1;
			}

			//�{�X�̈ړ��͈́iY��)
			if (g_aBoss[nCntBoss].pos.y < 0 + (BOSS_SIZE / 2) || g_aBoss[nCntBoss].pos.y > SCREEN_HEIGHT - (BOSS_SIZE / 2) - 200)
			{
				g_aBoss[nCntBoss].move.y *= -1;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y - (BOSS_SIZE / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y - (BOSS_SIZE / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y + (BOSS_SIZE / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y + (BOSS_SIZE / 2), 0.0f);

			//�e�N�X�`�����W���X�V
			pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimBoss * 0.33f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimBoss * 0.33f + 0.33f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimBoss * 0.33f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimBoss * 0.33f + 0.33f, 1.0f);

		}
		pVtx += 4;
	}
	//���_�f�[�^���A�����b�N����
	g_pVtxBuffBoss->Unlock();
}

//=============================================================================
// �{�X�`�揈��
//=============================================================================
void DrawBoss(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntBoss;

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffBoss, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTextureBoss[0]);

	//�|���S���̕`��
	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse == true)//�G���g�p���Ȃ�`��
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureBoss[g_aBoss[nCntBoss].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
}

//=============================================================================
// �{�X�̐ݒ菈��
//=============================================================================
void SetBoss(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType)
{
	//�ϐ��錾
	int nCntBoss;
	VERTEX_2D *pVtx;

	StopSound(SOUND_LABEL_BGM001);	//����Ă���BGM���~�߂�
	PlaySound(SOUND_LABEL_BGM004);	//�{�X���BGM��������

									//���_�o�b�t�@�����b�N
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse == false)
		{
			//�ʒu��ݒ�
			g_aBoss[nCntBoss].pos = pos;
			g_aBoss[nCntBoss].move = move;

			//�G�̗̑�
			g_aBoss[nCntBoss].nLife = BOSS_LIFE;	//�t�@�C���[�o�[�h

													//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y - (BOSS_SIZE / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y - (BOSS_SIZE / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y + (BOSS_SIZE / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y + (BOSS_SIZE / 2), 0.0f);

			g_aBoss[nCntBoss].bUse = true;
			break;
		}
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffBoss->Unlock();
}

//=============================================================================
// �{�X�̎擾
//=============================================================================
BOSS*GetBoss(void)
{
	return &g_aBoss[0];
}

//=============================================================================
// �{�X�̃_���[�W�ݒ�
//=============================================================================
void HitBoss(int nCntBoss, int nDamage)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	PlaySound(SOUND_LABEL_SE_EXPLOSION);	//������

											//���_�o�b�t�@�����b�N
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	g_aBoss[nCntBoss].nLife -= nDamage;
	if (g_aBoss[nCntBoss].nLife <= 0)
	{
		//��������
		SetExplosion(g_aBoss[nCntBoss].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BOSS_SIZE);

		g_aBoss[nCntBoss].bUse = false;

		//��Փx�m�[�}��
		if (g_nBossLevel == GAMELEVEL_NORMAL)
		{
			ScoreEffect(11100);
		}
		//��Փx�n�[�h
		if (g_nBossLevel == GAMELEVEL_HARD)
		{
			ScoreEffect(15100);
		}

		StopSound(SOUND_LABEL_BGM004);	//�{�X���BGM���~�߂�

		PlaySound(SOUND_LABEL_SE_VOICE);	//�{�X���j��

		SetItem(g_aBoss[nCntBoss].pos, ITEMTYPE_001);
	}

	else if (g_aBoss[nCntBoss].nLife < BOSS_LIFE / 4)
	{
		g_aBoss[nCntBoss].state = BOSSSTATE_PINCH;
		g_aBoss[nCntBoss].nCounterState = 5;
		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 236, 0, 255);

		ScoreEffect(100);
	}

	else
	{
		//�_���[�W��ԂɑJ��
		g_aBoss[nCntBoss].state = BOSSSTATE_DAMAGE;
		g_aBoss[nCntBoss].nCounterState = 5;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 236, 0, 255);

		ScoreEffect(100);
	}
	//���_�f�[�^���A�����b�N����
	g_pVtxBuffBoss->Unlock();
}