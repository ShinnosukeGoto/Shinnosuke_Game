//=============================================================================
//
// X-Bow Hunter [enemy.cpp]�̏��� (�U�R�G)
// Author :�㓡�T�V��
//
//=============================================================================
#include "enemy.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "boss.h"
#include "item.h"
#include "input.h"
#include "hunter.h"
#include "sound.h"
#include "title.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_apTextureEnemy[ENEMYTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffEnemy = NULL;

int g_nCounterAnimEn;			//���X�L�[�g�̃A�j���[�V�����J�E���^�[
int g_nPatternAnimEn;			//���X�L�[�g�̃A�j���[�V�����p�^�[��

int g_nCounterAnimEn1;			//���^���o�[�h�̃A�j���[�V�����J�E���^�[
int g_nPatternAnimEn1;			//���^���o�[�h�̃A�j���[�V�����p�^�[��

int g_nCounterAnimEn2;			//�O�\�N���V�̃A�j���[�V�����J�E���^�[
int g_nPatternAnimEn2;			//�O�\�N���V�̃A�j���[�V�����p�^�[��

int g_nChangeAnimGso;			//�O�\�N���V�̃e�N�X�`���𔽓]������Ƃ��Ɏg�p

int g_CntEnemy = 1;				//�e�������_���ɑłƂ��Ɏg�p

int g_nNumEnemyStage1;			//�X�e�[�W1�̓G�̑���
int g_nNumEnemyStage2;			//�X�e�[�W2�̓G�̑���
int g_nNumEnemyStage3;			//�X�e�[�W3�̓G�̑���
int g_nNumEnemyStage4;			//�X�e�[�W3�̓G�̑���

ENEMY g_aEnemy[MAX_ENEMY];		//�G�̏��
GAMELEVEL g_nEnemyLevel;		//�G�̃��x��

								//=============================================================================
								//�G�l�~�[����������
								//=============================================================================
void InitEnemy(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEnemy;
	g_nNumEnemyStage1 = ENEMY_STAGE1;		//�X�e�[�W1�̏����� (�E�̐����͓G�̐�)
	g_nNumEnemyStage2 = ENEMY_STAGE2;		//�X�e�[�W2�̏����� (�E�̐����͓G�̐�)
	g_nNumEnemyStage3 = ENEMY_STAGE3;		//�X�e�[�W3�̏����� (�E�̐����͓G�̐�)
	g_nNumEnemyStage4 = ENEMY_STAGE4;		//�X�e�[�W4�̏����� (�E�̐����͓G�̐�)
	g_nChangeAnimGso = 0;		//�O�\�N���V�̌�����������

								//�G�̏��̏�����
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nLife = 0;
	}
	g_nEnemyLevel = GetTitle();				//��Փx���擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "enemy000.png", &g_apTextureEnemy[0]);	//���X�L�[�g
	D3DXCreateTextureFromFile(pDevice, "enemy001.png", &g_apTextureEnemy[1]);	//���^���o�[�h
	D3DXCreateTextureFromFile(pDevice, "enemy200.png", &g_apTextureEnemy[2]);	//���^���o�[�h����
	D3DXCreateTextureFromFile(pDevice, "enemy300.png", &g_apTextureEnemy[3]);	//�I�N�g�p�X
	D3DXCreateTextureFromFile(pDevice, "enemy003.png", &g_apTextureEnemy[4]);	//�S�[�X�g
	D3DXCreateTextureFromFile(pDevice, "enemy005.png", &g_apTextureEnemy[5]);	//�O�\�N���V

																				//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_ENEMY, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEnemy, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
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
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// �G�l�~�[�I������
//=============================================================================
void UninitEnemy(void)
{
	//�ϐ��錾
	int nCntTex;

	//�e�N�X�`���̊J��
	for (nCntTex = 0; nCntTex < ENEMYTYPE_MAX; nCntTex++)
	{
		if (g_apTextureEnemy != NULL)
		{
			g_apTextureEnemy[nCntTex]->Release();
			g_apTextureEnemy[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=============================================================================
// �G�l�~�[�X�V����
//=============================================================================
void UpdateEnemy(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntGhost = 0, nAnimGhost = 0;		//�S�[�X�g�̃A�j���[�V�����Ɏg�p

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{

			//���X�L�[�g
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_000)
			{
				//�A�j���[�V����
				g_nCounterAnimEn++;	//�J�E���^���Z
				if (g_nCounterAnimEn == 2)//����
				{
					g_nCounterAnimEn = 0;
					g_nPatternAnimEn++;
					if (g_nPatternAnimEn == 2)//����
					{
						g_nPatternAnimEn = 0;
					}
				}
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					//��Փx�m�[�}��
					if (g_nEnemyLevel == GAMELEVEL_NORMAL)
					{
						//�e�̔���
						if ((rand() % (g_CntEnemy * 1046)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-3, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 203, 100, 155), 15);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 1046)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-4, 6, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 203, 100, 155), 15);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 1046)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 203, 100, 155), 15);
						}
					}
					//��Փx�n�[�h
					if (g_nEnemyLevel == GAMELEVEL_HARD)
					{
						//�e�̔���
						if ((rand() % (g_CntEnemy * 46)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-3, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 203, 100, 155), 15);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 46)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-4, 6, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 203, 100, 155), 15);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 46)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 203, 100, 155), 15);
						}
					}

					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//���X�L�[�g�̈ړ�
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 1;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MOS / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MOS / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MOS / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MOS / 2), 0.0f);

				//�e�N�X�`�����W���X�V
				pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimEn * 0.5f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimEn * 0.5f + 0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimEn * 0.5f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimEn * 0.5f + 0.5f, 1.0f);

				//���X�L�[�g�̈ړ��͈�
				if (g_aEnemy[nCntEnemy].pos.x < 0 + (ENEMY_SIZE_MOS / 2) + 640 || g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH - (ENEMY_SIZE_MOS / 2) - 100)
				{
					g_aEnemy[nCntEnemy].move.x *= -1;
				}

				if (g_aEnemy[nCntEnemy].pos.y < 0 + (ENEMY_SIZE_MOS / 2) + 200 || g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT - (ENEMY_SIZE_MOS / 2) - 300)
				{
					g_aEnemy[nCntEnemy].move.y *= -1;
				}
			}

			//���^���o�[�h
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_001)
			{
				//�A�j���[�V����
				g_nCounterAnimEn1++;	//�J�E���^���Z
				if (g_nCounterAnimEn1 == 8)//����
				{
					g_nCounterAnimEn1 = 0;
					g_nPatternAnimEn1++;
					if (g_nPatternAnimEn1 == 3)//����
					{
						g_nPatternAnimEn1 = 0;
					}
				}
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					//��Փx�m�[�}��
					if (g_nEnemyLevel == GAMELEVEL_NORMAL)
					{
						//�e�̔���
						if ((rand() % (g_CntEnemy * 530)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-3, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 150, 200, 155), 25);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 530)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(4, 6, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 150, 200, 155), 25);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 530)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 150, 200, 155), 25);
						}
					}
					//��Փx�n�[�h
					if (g_nEnemyLevel == GAMELEVEL_HARD)
					{
						//�e�̔���
						if ((rand() % (g_CntEnemy * 30)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-3, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 150, 200, 155), 25);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 30)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(4, 6, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 150, 200, 155), 25);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 30)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 150, 200, 155), 25);
						}
					}

					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//��Փx�m�[�}��
				if (g_nEnemyLevel == GAMELEVEL_NORMAL)
				{
					//���^���o�[�h�̈ړ�
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 2;
				}
				//��Փx�n�[�h
				if (g_nEnemyLevel == GAMELEVEL_HARD)
				{
					//���^���o�[�h�̈ړ�
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 3;
				}

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MET / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MET / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MET / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MET / 2), 0.0f);

				//�e�N�X�`�����W���X�V
				pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f + 0.333f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f + 0.333f, 1.0f);


				//���^���o�[�h�̈ړ��͈�
				if (g_aEnemy[nCntEnemy].pos.x < 0 + (ENEMY_SIZE_MET / 2) + 440 || g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH - (ENEMY_SIZE_MET / 2) - 50)
				{
					g_aEnemy[nCntEnemy].move.x *= -1;
				}

				if (g_aEnemy[nCntEnemy].pos.y < 0 + (ENEMY_SIZE_MET / 2) + 150 || g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT - (ENEMY_SIZE_MET / 2) - 110)
				{
					g_aEnemy[nCntEnemy].move.y *= -1;
				}
			}

			//���^���o�[�h����
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_002)
			{
				//�A�j���[�V����
				g_nCounterAnimEn1++;	//�J�E���^���Z
				if (g_nCounterAnimEn1 == 8)//����
				{
					g_nCounterAnimEn1 = 0;
					g_nPatternAnimEn1++;
					if (g_nPatternAnimEn1 == 3)//����
					{
						g_nPatternAnimEn1 = 0;
					}
				}
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					//��Փx�m�[�}��
					if (g_nEnemyLevel == GAMELEVEL_NORMAL)
					{
						//�e�̔���
						if ((rand() % (g_CntEnemy * 140)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(6, 10, 0), BULLETTYPE_ENEMY�U, 150, D3DXCOLOR(0, 100, 200, 155), 30);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 140)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(3, 10, 0), BULLETTYPE_ENEMY�U, 150, D3DXCOLOR(0, 100, 200, 155), 30);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 140)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-6, 10, 0), BULLETTYPE_ENEMY�U, 150, D3DXCOLOR(200, 50, 200, 155), 30);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 140)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(6, -10, 0), BULLETTYPE_ENEMY�U, 150, D3DXCOLOR(200, 50, 200, 155), 30);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 140)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-3, -10, 0), BULLETTYPE_ENEMY�U, 150, D3DXCOLOR(200, 50, 200, 155), 30);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 140)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-6, -10, 0), BULLETTYPE_ENEMY�U, 150, D3DXCOLOR(0, 100, 200, 155), 30);
						}
					}
					//��Փx�n�[�h
					if (g_nEnemyLevel == GAMELEVEL_HARD)
					{
						//�e�̔���
						if ((rand() % (g_CntEnemy * 40)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(6, 10, 0), BULLETTYPE_ENEMY�U, 150, D3DXCOLOR(0, 100, 200, 155), 30);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 40)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(3, 10, 0), BULLETTYPE_ENEMY�U, 150, D3DXCOLOR(0, 100, 200, 155), 30);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 40)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-6, 10, 0), BULLETTYPE_ENEMY�U, 150, D3DXCOLOR(200, 50, 200, 155), 30);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 40)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(6, -10, 0), BULLETTYPE_ENEMY�U, 150, D3DXCOLOR(200, 50, 200, 155), 30);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 40)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-3, -10, 0), BULLETTYPE_ENEMY�U, 150, D3DXCOLOR(200, 50, 200, 155), 30);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 40)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-6, -10, 0), BULLETTYPE_ENEMY�U, 150, D3DXCOLOR(0, 100, 200, 155), 30);
						}
					}

					if (g_aEnemy[nCntEnemy].nLife < ENEMY_LIFE_MET�U / 4)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_PINCH;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

					}

					break;

				case ENEMYSTATE_PINCH:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						//�s���`�`��ɂ��鏈��
						pVtx[0].col = D3DCOLOR_RGBA(250, 199, 33, 255);
						pVtx[1].col = D3DCOLOR_RGBA(250, 199, 33, 255);
						pVtx[2].col = D3DCOLOR_RGBA(250, 199, 33, 255);
						pVtx[3].col = D3DCOLOR_RGBA(250, 199, 33, 255);
					}
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//��Փx�m�[�}��
				if (g_nEnemyLevel == GAMELEVEL_NORMAL)
				{
					//���^���o�[�h����̈ړ�
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 2.5;
				}
				//��Փx�n�[�h
				if (g_nEnemyLevel == GAMELEVEL_HARD)
				{
					//���^���o�[�h����̈ړ�
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 3.5;
				}

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MET�U / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MET�U / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MET�U / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MET�U / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MET�U / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MET�U / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MET�U / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MET�U / 2), 0.0f);

				//�e�N�X�`�����W���X�V
				pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f + 0.333f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f + 0.333f, 1.0f);

				//���^���o�[�h����̈ړ��͈�
				if (g_aEnemy[nCntEnemy].pos.x < 0 + (ENEMY_SIZE_MET�U / 2) + 240 || g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH - (ENEMY_SIZE_MET�U / 2) - 450)
				{
					g_aEnemy[nCntEnemy].move.x *= -1;
				}

				if (g_aEnemy[nCntEnemy].pos.y < 0 + (ENEMY_SIZE_MET�U / 2) + 100 || g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT - (ENEMY_SIZE_MET�U / 2) - 110)
				{
					g_aEnemy[nCntEnemy].move.y *= -1;
				}

			}

			//�I�N�g�p�X
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_003)
			{
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					//��Փx�m�[�}��
					if (g_nEnemyLevel == GAMELEVEL_NORMAL)
					{
						//�e�̔���
						if ((rand() % (g_CntEnemy * 300)) == 0)
						{
							//�n���^�[�̈ʒu���擾
							HUNTER*pHunter;
							pHunter = GetHunter();

							float fHposx = pHunter->pos.x, fHposy = pHunter->pos.y;							//�ڕW(�n���^�[)�̍��W
							float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;	//�G�̍��W
							float fAngle;																	//�p�x

							fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//�p�x�����߂�
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-sinf(fAngle)*14.5f, -cosf(fAngle)*14.5f, 0), BULLETTYPE_OCT, 95, D3DXCOLOR(0, 200, 100, 255), 50);
							PlaySound(SOUND_LABEL_SE_SHOT2);	//�I�N�g�p�X�̃V���b�g��
						}
					}
					//��Փx�n�[�h
					if (g_nEnemyLevel == GAMELEVEL_HARD)
					{
						//�e�̔���
						if ((rand() % (g_CntEnemy * 70)) == 0)
						{
							//�n���^�[�̈ʒu���擾
							HUNTER*pHunter;
							pHunter = GetHunter();

							float fHposx = pHunter->pos.x, fHposy = pHunter->pos.y;							//�ڕW(�n���^�[)�̍��W
							float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;	//�G�̍��W
							float fAngle;																	//�p�x

							fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//�p�x�����߂�
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-sinf(fAngle)*14.5f, -cosf(fAngle)*14.5f, 0), BULLETTYPE_OCT, 95, D3DXCOLOR(0, 200, 100, 255), 50);
							PlaySound(SOUND_LABEL_SE_SHOT2);	//�I�N�g�p�X�̃V���b�g��
						}
					}
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_OCT / 2), 0.0f);

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			//�S�[�X�g
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_004)
			{
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					//��Փx�m�[�}��
					if (g_nEnemyLevel == GAMELEVEL_NORMAL)
					{
						//�e�̔���
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, -2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 155), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, -4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 3, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 5, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, -2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, -4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 450)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 450)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 3, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 5, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}
					}
					//��Փx�n�[�h
					if (g_nEnemyLevel == GAMELEVEL_HARD)
					{
						//�e�̔���
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, -2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 155), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, -4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 3, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 5, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, -2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, -4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 150)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 150)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 3, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 150)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//�e�̔���
						if ((rand() % (g_CntEnemy * 150)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 5, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}
					}

					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//�S�[�X�g�̈ړ�
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 0.5;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GHO / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GHO / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GHO / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GHO / 2), 0.0f);

				//A�L�[��������Ă��Ȃ��Ԃ́A�p������������
				pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 105);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 105);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 105);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 105);

				//A�L�[�������ꂽ�i�p�����������ԉ����j
				if (GetKeyboardPress(DIK_A))
				{
					nCntGhost = 1;
				}

				//�p�����������Ԃ���������
				if (nCntGhost > 0)
				{
					nCntGhost = 0;
					nAnimGhost = 1;
				}

				//�e�N�X�`����p�����������Ԃ������
				if (nAnimGhost == 1)
				{
					nAnimGhost = 0;
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}

				//�S�[�X�g�̈ړ��͈�
				if (g_aEnemy[nCntEnemy].pos.x < 0 + (ENEMY_SIZE_GHO / 2) + 440 || g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH - (ENEMY_SIZE_GHO / 2) - 350)
				{
					g_aEnemy[nCntEnemy].move.x *= -1;
				}

				if (g_aEnemy[nCntEnemy].pos.y < 0 + (ENEMY_SIZE_GHO / 2) + 200 || g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT - (ENEMY_SIZE_GHO / 2) - 210)
				{
					g_aEnemy[nCntEnemy].move.y *= -1;
				}
			}

			//�O�\�N���V
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_005)
			{
				//�A�j���[�V����
				g_nCounterAnimEn2++;	//�J�E���^���Z
				if (g_nCounterAnimEn2 == 8)//����
				{
					g_nCounterAnimEn2 = 0;
					g_nPatternAnimEn2++;
					if (g_nPatternAnimEn2 == 2)//����
					{
						g_nPatternAnimEn2 = 0;
					}
				}

				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//�O�\�N���V�̈ړ�
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 5;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GSO / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GSO / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GSO / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GSO / 2), 0.0f);

				//�O�\�N���V�̈ړ��͈�
				if (g_aEnemy[nCntEnemy].pos.x < 0 + (ENEMY_SIZE_GSO / 2) + 640)
				{
					g_aEnemy[nCntEnemy].move.x *= -1;
					g_nChangeAnimGso = 1;
				}

				//�e�N�X�`����������
				if (g_nChangeAnimGso == 0)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f + 0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f + 0.5f, 1.0f);
				}

				//�e�N�X�`�����E����
				if (g_nChangeAnimGso == 1)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f + 0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f + 0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f, 1.0f);
				}

				//��ʂ��o���疢�g�p��
				if (g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH + (ENEMY_SIZE_GSO / 2))
				{
					g_aEnemy[nCntEnemy].bUse = false;
					g_nChangeAnimGso = 0;	//���ɏo�Ă���O�\�N���V���������ɂ���
				}
			}
		}
		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffEnemy->Unlock();

}

//=============================================================================
// �G�l�~�[�`�揈��
//=============================================================================
void DrawEnemy(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEnemy;

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	//�|���S���̕`��
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)//�G���g�p���Ȃ�`��
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].type]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEnemy, NUM_POLYGON);
		}
	}
}

//=============================================================================
// �G�l�~�[�̐ݒ菈��
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE type)
{
	//�ϐ��錾
	int nCntEnemy;
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			//�ʒu��ݒ�
			g_aEnemy[nCntEnemy].pos = pos;

			g_aEnemy[nCntEnemy].move = move;

			g_aEnemy[nCntEnemy].type = type;

			//���X�L�[�g
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_000)
			{
				//�G�̗̑�
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_MOS;
			}

			//���^���o�[�h
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_001)
			{
				//�G�̗̑�
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_MET;
			}

			//���^���o�[�h����
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_002)
			{
				//�G�̗̑�
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_MET�U;
			}

			//�I�N�g�p�X
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_003)
			{
				//�G�̗̑�
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_OCT;
			}

			//�S�[�X�g
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_004)
			{
				//�G�̗̑�
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_GHO;
			}

			//�O�\�N���V
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_005)
			{
				//�G�̗̑�
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_GSO;
				PlaySound(SOUND_LABEL_SE_STAR);
			}

			//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			g_aEnemy[nCntEnemy].bUse = true;
			break;
		}
		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffEnemy->Unlock();

}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
ENEMY*GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=============================================================================
// �G�l�~�[�̃_���[�W�ݒ�
//=============================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	PlaySound(SOUND_LABEL_SE_EXPLOSION);	//������

											//���_�o�b�t�@�����b�N
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	//���X�L�[�g
	if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_000)
	{
		if (g_aEnemy[nCntEnemy].nLife == 0)
		{
			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_MOS);

			g_aEnemy[nCntEnemy].bUse = false;

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(510);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(1010);
			}

			g_nNumEnemyStage1--;
		}
		else
		{
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				//�_���[�W��ԂɑJ��
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
				g_aEnemy[nCntEnemy].nCounterState = 5;

				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_DAMAGE)
				{
					//���_�J���[�̐ݒ�
					pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
					pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
					pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
					pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				}
				pVtx += 4;
			}
			ScoreEffect(10);
		}
	}

	//���^���o�[�h
	if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_001)
	{
		if (g_aEnemy[nCntEnemy].nLife == 0)
		{
			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_MET);

			g_aEnemy[nCntEnemy].bUse = false;

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(760);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(1510);
			}

			g_nNumEnemyStage2--;

		}

		else if (g_aEnemy[nCntEnemy].nLife == (ENEMY_LIFE_MET / 2))
		{
			//�_���[�W��ԂɑJ��
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
			g_aEnemy[nCntEnemy].nCounterState = 5;
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);

			//�̗͔����ŃI�N�g�p�X���o��
			SetEnemy(D3DXVECTOR3(1000, 590, 0.0f), D3DXVECTOR3(0, 0, 0.0f), ENEMYTYPE_003);

			ScoreEffect(10);
		}

		else
		{
			//�_���[�W��ԂɑJ��
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
			g_aEnemy[nCntEnemy].nCounterState = 5;
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);

			ScoreEffect(10);
		}
	}

	//���^���o�[�h����
	if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_002)
	{
		if (g_aEnemy[nCntEnemy].nLife <= 0)
		{
			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_MET�U);

			g_aEnemy[nCntEnemy].bUse = false;

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(1510);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(3010);
			}

			g_nNumEnemyStage4--;
		}

		else if (g_aEnemy[nCntEnemy].nLife == (ENEMY_LIFE_MET�U / 2))
		{
			//�_���[�W��ԂɑJ��
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
			g_aEnemy[nCntEnemy].nCounterState = 5;
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);

			//�̗͔����ŃO�\�N���V���o��
			SetEnemy(D3DXVECTOR3(1200, 590, 0.0f), D3DXVECTOR3(-1, 0, 0.0f), ENEMYTYPE_005);

			ScoreEffect(10);
		}

		else if (g_aEnemy[nCntEnemy].nLife < (ENEMY_LIFE_MET�U / 4))
		{
			//�s���`��ԂɑJ��
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_PINCH;
			g_aEnemy[nCntEnemy].nCounterState = 5;
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);

			ScoreEffect(10);
		}

		else
		{
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				//�_���[�W��ԂɑJ��
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
				g_aEnemy[nCntEnemy].nCounterState = 5;
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			}

			ScoreEffect(10);
		}
	}

	//�I�N�g�p�X
	if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_003)
	{
		if (g_aEnemy[nCntEnemy].nLife == 0)
		{
			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_OCT);

			g_aEnemy[nCntEnemy].bUse = false;

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(1510);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(3010);
			}

			g_nNumEnemyStage2--;
		}

		else
		{
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				//�_���[�W��ԂɑJ��
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
				g_aEnemy[nCntEnemy].nCounterState = 5;

				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx += 4;
			}
			ScoreEffect(10);
		}
	}

	//�S�[�X�g
	if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_004)
	{
		if (g_aEnemy[nCntEnemy].nLife <= 0)
		{
			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_GHO);

			g_aEnemy[nCntEnemy].bUse = false;

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(2510);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(5010);
			}

			g_nNumEnemyStage3--;
		}

		else if (g_aEnemy[nCntEnemy].nLife == (ENEMY_LIFE_GHO / 2))
		{
			//�_���[�W��ԂɑJ��
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
			g_aEnemy[nCntEnemy].nCounterState = 5;
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);

			//�̗͔����ŃO�\�N���V���o��
			SetEnemy(D3DXVECTOR3(1200, 590, 0.0f), D3DXVECTOR3(-1, 0, 0.0f), ENEMYTYPE_005);

			ScoreEffect(10);
		}

		else
		{
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				//�_���[�W��ԂɑJ��
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
				g_aEnemy[nCntEnemy].nCounterState = 5;
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			}

			ScoreEffect(10);
		}
	}

	//�O�\�N���V
	if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_005)
	{
		if (g_aEnemy[nCntEnemy].nLife <= 0)
		{
			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_GHO);

			g_aEnemy[nCntEnemy].bUse = false;

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(5010);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(10010);
			}

			g_nChangeAnimGso = 0;	//���ɏo�Ă���O�\�N���V���������ɂ���
		}

		else
		{
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				//�_���[�W��ԂɑJ��
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
				g_aEnemy[nCntEnemy].nCounterState = 5;
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			}

			ScoreEffect(10);
		}
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffEnemy->Unlock();

	//�X�e�[�W1����2��
	if (g_nNumEnemyStage1 == 0)	//0�ȉ��ɂ���ƁA�����̖�œ|�����Ƃ��Ƀo�O�邽��0�Ɠ��l�ɂ���
	{
		SetEnemy(D3DXVECTOR3(1180, 240, 0.0f), D3DXVECTOR3(-1.0, 1.0, 0.0f), ENEMYTYPE_001);
		g_nNumEnemyStage1 = 10;	//�g���I����������́A�o�O��������邽��10�ɂ���
	}

	//�X�e�[�W2����3��
	if (g_nNumEnemyStage2 == 0)	//0�ȉ��ɂ���ƁA�����̖�œ|�����Ƃ��Ƀo�O�邽��0�Ɠ��l�ɂ���
	{
		SetEnemy(D3DXVECTOR3(840, 300, 0.0f), D3DXVECTOR3(-0.5, 0.5, 0.0f), ENEMYTYPE_004);
		g_nNumEnemyStage2 = 10;	//�g���I����������́A�o�O��������邽��10�ɂ���
	}

	//�X�e�[�W3����4��
	if (g_nNumEnemyStage3 == 0)	//0�ȉ��ɂ���ƁA�����̖�œ|�����Ƃ��Ƀo�O�邽��0�Ɠ��l�ɂ���
	{
		SetEnemy(D3DXVECTOR3(540, 200, 0.0f), D3DXVECTOR3(1.0, 1.0, 0.0f), ENEMYTYPE_002);
		g_nNumEnemyStage3 = 10;	//�g���I����������́A�o�O��������邽��10�ɂ���
	}

	//�X�e�[�W4����A�C�e��(�H)��
	if (g_nNumEnemyStage4 == 0)	//0�ȉ��ɂ���ƁA�����̖�œ|�����Ƃ��Ƀo�O�邽��0�Ɠ��l�ɂ���
	{
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_000);
		g_nNumEnemyStage4 = 10;	//�g���I����������́A�o�O��������邽��10�ɂ���
	}

}