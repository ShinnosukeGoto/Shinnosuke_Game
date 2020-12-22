//=============================================================================
//
// DirectX�V���[�e�B���O [bullet.cpp] (�G�̒e)
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "hunter.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "boss.h"
#include "effect.h"
#include "storm.h"
#include "input.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureBullet = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBullet = NULL;
BULLET g_aBullet[MAX_BULLET];

//=============================================================================
//�o���b�g����������
//=============================================================================
void InitBullet(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntBullet;

	//�e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].bUse = false;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "bullet000.png", &g_pTextureBullet);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

													//�e�̐���
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
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
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// �o���b�g�I������
//=============================================================================
void UninitBullet(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// �o���b�g�X�V����
//=============================================================================
void UpdateBullet(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntBullet;
	VERTEX_2D *pVtx;
	ENEMY*pEnemy;
	int nCntEnemy;
	BOSS*pBoss;
	int nCntBoss;
	HUNTER*pHunter;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//P�L�[�������ꂽ�i�f�o�b�N�R�}���h�j
			if (GetKeyboardPress(DIK_P))
			{
				g_aBullet[nCntBullet].bUse = false;	//�g�p���Ȃ���Ԃɂ���
				break;
			}

			//�G���擾
			pEnemy = GetEnemy();

			//�{�X���擾
			pBoss = GetBoss();

			//���@���擾
			pHunter = GetHunter();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->bUse == true) //�G���g�p����Ă���
				{
					if (pHunter->bDisp == true)
					{
						//���@�̓����蔻��
						if (g_aBullet[nCntBullet].pos.x <= pHunter->pos.x + (HUNTER_SIZE / 2) - 10 &&
							g_aBullet[nCntBullet].pos.x >= pHunter->pos.x - (HUNTER_SIZE / 2) + 10 &&
							g_aBullet[nCntBullet].pos.y <= pHunter->pos.y + (HUNTER_SIZE / 2) - 10 &&
							g_aBullet[nCntBullet].pos.y >= pHunter->pos.y - (HUNTER_SIZE / 2) + 10)
						{
							if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)//�G�̒e
							{
								//���@�Ƀ_���[�W
								HitHunter(10);
							}

							if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY�U || g_aBullet[nCntBullet].type == BULLETTYPE_GHO)//���^���o�[�h����ƃS�[�X�g�̒e
							{
								//���@�Ƀ_���[�W
								HitHunter(30);
							}

							if (g_aBullet[nCntBullet].type == BULLETTYPE_OCT)//�I�N�g�p�X�̒e
							{
								//���@�Ƀ_���[�W
								HitHunter(50);
							}

							//�����������_�Œe��HP��0�ɂ���
							g_aBullet[nCntBullet].nLife = 0;
						}
					}
				}
			}

			for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pBoss++)
			{
				if (pBoss->bUse == true) //�{�X���g�p����Ă���
				{
					if (pHunter->bDisp == true)
					{
						//���@�̓����蔻��
						if (g_aBullet[nCntBullet].pos.x <= pHunter->pos.x + (HUNTER_SIZE / 2) - 10 &&
							g_aBullet[nCntBullet].pos.x >= pHunter->pos.x - (HUNTER_SIZE / 2) + 10 &&
							g_aBullet[nCntBullet].pos.y <= pHunter->pos.y + (HUNTER_SIZE / 2) - 10 &&
							g_aBullet[nCntBullet].pos.y >= pHunter->pos.y - (HUNTER_SIZE / 2) + 10)
						{
							//���@�Ƀ_���[�W
							HitHunter(10);

							//�����������_�Œe��HP��0�ɂ���
							g_aBullet[nCntBullet].nLife = 0;

							//���@�ɒ��ڃ{�X�̒e���������������A���h�����悭���邽�߂Ɏ��@�̂���(�A���q�b�g���Ȃ��ʒu)�ɃX�g�[���𔭐�
							SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 30, 0.0f), D3DXVECTOR3(-10, -10, 0), 35, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 35);
							SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 30, 0.0f), D3DXVECTOR3(9.5, -9.5, 0), 38, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 30);
							SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 30, 0.0f), D3DXVECTOR3(-9.0, -9.0, 0), 41, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 25);
							SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 30, 0.0f), D3DXVECTOR3(8.5, -8.5, 0), 44, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 20);
							SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 30, 0.0f), D3DXVECTOR3(-8.0, -8.0, 0), 47, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 15);
							SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 30, 0.0f), D3DXVECTOR3(7.5, -7.5, 0), 50, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 10);
						}
					}
				}
			}

			if (g_aBullet[nCntBullet].type == BULLETTYPE_BOSS_PINCH)
			{
				//�{�X�ƃn���^�[�̈ʒu���擾
				pHunter = GetHunter();
				pBoss = GetBoss();

				float fHposx = pHunter->pos.x, fHposy = pHunter->pos.y;	//�ڕW(�n���^�[)�̍��W
				float fBposx = pBoss->pos.x, fBposy = pBoss->pos.y;		//�G�̍��W
				float fAngle;											//�p�x

				fAngle = atan2f((fBposx - fHposx), (fBposy - fHposy));	//�p�x�����߂�

				g_aBullet[nCntBullet].move.x = -sinf(fAngle)*17.5f;		//X���W�X�V
				g_aBullet[nCntBullet].move.y = -cosf(fAngle)*17.5f;		//Y���W�X�V
			}

			//�e�̈ړ����x(move�����{���ƁA�S�̓I�ɑ����A��Փx�������������߁A0.5�������Ă���)
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move*0.5;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y - (g_aBullet[nCntBullet].nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y - (g_aBullet[nCntBullet].nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y + (g_aBullet[nCntBullet].nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y + (g_aBullet[nCntBullet].nSize / 2), 0.0f);

			//���ɒe���o�Ă�������
			if (g_aBullet[nCntBullet].pos.x < g_aBullet[nCntBullet].nSize - (g_aBullet[nCntBullet].nSize / 2))
			{
				g_aBullet[nCntBullet].bUse = false;
			}

			//�E�ɒe���o�Ă�������
			if (g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH - g_aBullet[nCntBullet].nSize + (g_aBullet[nCntBullet].nSize / 2))
			{
				g_aBullet[nCntBullet].bUse = false;
			}

			//��ɒe���o�Ă�������
			if (g_aBullet[nCntBullet].pos.y < g_aBullet[nCntBullet].nSize - (g_aBullet[nCntBullet].nSize / 2))
			{
				g_aBullet[nCntBullet].bUse = false;
			}

			//�n�ʂɂɒe������������
			if (g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT - g_aBullet[nCntBullet].nSize + (g_aBullet[nCntBullet].nSize / 2) - 80)
			{
				//�{�X�ƃI�N�g�p�X�ȊO�̏ꍇ�i�I�N�g�p�X�͒n�ʂ𔇂��e�Ȃ̂ŁA���g�p�ɂ��Ȃ��j
				if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY || g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY�U ||
					g_aBullet[nCntBullet].type == BULLETTYPE_GHO || g_aBullet[nCntBullet].type == BULLETTYPE_BOSS_PINCH)
				{
					g_aBullet[nCntBullet].nLife = 0;
				}

				//�{�X�̏ꍇ�A�X�g�[���𔭐�������
				if (g_aBullet[nCntBullet].type == BULLETTYPE_BOSS)
				{
					g_aBullet[nCntBullet].nLife = 0;
					SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 35, 0.0f), D3DXVECTOR3(-10, -10, 0), 35, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 35);
					SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 35, 0.0f), D3DXVECTOR3(9.5, -9.5, 0), 38, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 30);
					SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 35, 0.0f), D3DXVECTOR3(-9.0, -9.0, 0), 41, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 25);
					SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 35, 0.0f), D3DXVECTOR3(8.5, -8.5, 0), 44, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 20);
					SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 35, 0.0f), D3DXVECTOR3(-8.0, -8.0, 0), 47, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 15);
					SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 35, 0.0f), D3DXVECTOR3(7.5, -7.5, 0), 50, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 10);
				}
			}

			//�e�̎˒������߂�
			g_aBullet[nCntBullet].nLife--;
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				//��������
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].nSize);

				g_aBullet[nCntBullet].bUse = false;	//�g�p���Ȃ���Ԃɂ���
			}

			//�G�t�F�N�g������(�I�N�g�p�X�ȊO)
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY || g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY�U
				|| g_aBullet[nCntBullet].type == BULLETTYPE_GHO || g_aBullet[nCntBullet].type == BULLETTYPE_BOSS ||
				g_aBullet[nCntBullet].type == BULLETTYPE_BOSS_PINCH)
			{
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(g_aBullet[nCntBullet].col.r, g_aBullet[nCntBullet].col.g, g_aBullet[nCntBullet].col.b, 1.0f)
					, (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].nLife);
			}
		}

		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffBullet->Unlock();

}

//=============================================================================
// �o���b�g�`�揈��
//=============================================================================
void DrawBullet(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntBullet;

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)			//�e���g�p���Ȃ�`��
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, NUM_POLYGON);
		}
	}

}

//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type, int nLife, D3DXCOLOR col, int nSize)
{
	//�ϐ��錾
	int nCntBullet;
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			//�ʒu��ݒ�
			g_aBullet[nCntBullet].pos = pos;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (nSize / 2), g_aBullet[nCntBullet].pos.y - (nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (nSize / 2), g_aBullet[nCntBullet].pos.y - (nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (nSize / 2), g_aBullet[nCntBullet].pos.y + (nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (nSize / 2), g_aBullet[nCntBullet].pos.y + (nSize / 2), 0.0f);

			//���_�J���[��ݒ�
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].nLife = nLife;
			g_aBullet[nCntBullet].col = col;
			g_aBullet[nCntBullet].nSize = nSize;
			g_aBullet[nCntBullet].bUse = true;

			break;
		}
		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
//�e�̎擾
//=============================================================================
BULLET*GetBullet(void)
{
	return &g_aBullet[0];
}