//=============================================================================
//
// �i�C���X�g�[���[�Y [enemy.cpp]�̏��� (�Q�[���ɏo�Ă���G)
// Author :�㓡�T�V��
//
//=============================================================================
#include "enemy.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "item.h"
#include "input.h"
#include "player.h"
#include "sound.h"
#include "title.h"
#include "game.h"
#include "bg.h"
#include "block.h"
#include "effect2.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_apTextureEnemy[ENEMYTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffEnemy = NULL;

int g_CntEnemy = 1;				//�e�������_���ɑłƂ��Ɏg�p
ENEMY g_aEnemy[MAX_ENEMY];		//�G�̏��
GAMELEVEL g_nEnemyLevel;		//�G�̃��x��
bool g_bHitEnemy;				//�G�ɍU���������������ǂ���
bool g_bBgXE;					//�X�e�[�W1�̉����������ǂ���
bool g_bBlockXE;				//�u���b�N�ɉ����瓖������
int g_nCntBat;					//�R�E������|�������𐔂���
int g_nCntBorn;					//�K�C�R�c��|�������𐔂���
MODE g_ModeEnemy;				//�Q�[����ʂɑΉ������G

//=============================================================================
//�G�l�~�[����������
//=============================================================================
void InitEnemy(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEnemy;

	//�O���[�o���ϐ��̏�����
	g_bHitEnemy = false;		//�G�ɍU���������������ǂ�����������
	g_bBgXE = false;
	g_bBlockXE = false;
	g_nCntBat = 0;
	g_nCntBorn = 0;
	g_ModeEnemy = MODE_GAME;

	//�G�̏��̏�����
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].type = ENEMYTYPE_000;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nPosX = 0;
		g_aEnemy[nCntEnemy].nCounterAnim = 0;
		g_aEnemy[nCntEnemy].nPatternAnim = 0;
		g_aEnemy[nCntEnemy].bLeft = true;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].nAttackTime = 0;
	}
	g_nEnemyLevel = GetTitle();				//��Փx���擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy000.png", &g_apTextureEnemy[0]);	//�r�[�g��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy001.png", &g_apTextureEnemy[1]);	//�J���X
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy002.png", &g_apTextureEnemy[2]);	//�J���X����
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy003.png", &g_apTextureEnemy[3]);	//�K�C�R�c
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy004.png", &g_apTextureEnemy[4]);	//�C�m�V�V
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy005.png", &g_apTextureEnemy[5]);	//�R�E����
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy006.png", &g_apTextureEnemy[6]);	//�A�C�X�h���S��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy007.png", &g_apTextureEnemy[7]);	//�s���j�A
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy008.png", &g_apTextureEnemy[8]);	//�_�[�N�K�C�R�c

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
		if (g_apTextureEnemy[nCntTex] != NULL)
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
	PLAYER *pPlayer;
	int nCntGhost = 0, nAnimGhost = 0;		//�C�m�V�V�̃A�j���[�V�����Ɏg�p

											//���_�o�b�t�@�����b�N
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//�v���C���[���擾
			pPlayer = GetPlayer();
			//�w�i�̉����������ǂ����擾
			g_bBgXE = GetBg();
			//�u���b�N�ɉ����瓖���������ǂ����擾
			g_bBlockXE = GetBlockX();
			//�Q�[����ʂ��擾
			g_ModeEnemy = GetMode();

			//�r�[�g��
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_000)
			{
				//�A�j���[�V����
				g_aEnemy[nCntEnemy].nCounterAnim++;	//�J�E���^���Z
				if (g_aEnemy[nCntEnemy].nCounterAnim == 60)//����
				{
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim++;
					if (g_aEnemy[nCntEnemy].nPatternAnim == 2)//����
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}
				}
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//�r�[�g���̈ړ�
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 1;
				g_aEnemy[nCntEnemy].nPosX += 1;

				//�v���C���[�̑���ɑΉ������ړ�
				if (pPlayer->bDisp == true && g_bBgXE == true && g_bBlockXE == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].pos.x += VALUE_MOVE;
					}
				}

				//�r�[�g���̈ړ��͈�
				if (g_aEnemy[nCntEnemy].nPosX > 60)
				{
					//������ς���
					g_aEnemy[nCntEnemy].bLeft = !g_aEnemy[nCntEnemy].bLeft;	//bool�^�̃g�O��
					g_aEnemy[nCntEnemy].move.x *= -1;
					g_aEnemy[nCntEnemy].nPosX *= -1;	
				}

				//�e�N�X�`����������
				if (g_aEnemy[nCntEnemy].bLeft == true)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
				}

				//�e�N�X�`�����E����
				else if (g_aEnemy[nCntEnemy].bLeft == false)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
				}

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MOS / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MOS / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MOS / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MOS / 2), 0.0f);

			}

			//�J���X
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_001)
			{
				//�A�j���[�V����
				g_aEnemy[nCntEnemy].nCounterAnim++;	//�J�E���^���Z
				if (g_aEnemy[nCntEnemy].nCounterAnim == 8)//����
				{
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim++;
					if (g_aEnemy[nCntEnemy].nPatternAnim == 3)//����
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}
				}
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//�J���X�̈ړ�
				//��Փx�m�[�}��
				if (g_nEnemyLevel == GAMELEVEL_NORMAL)
				{
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 2.0;
				}
				//��Փx�n�[�h
				if (g_nEnemyLevel == GAMELEVEL_HARD)
				{
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 3.0;
				}
				g_aEnemy[nCntEnemy].nPosX += 1;

				//�v���C���[�̑���ɑΉ������ړ�
				if (pPlayer->bDisp == true && g_bBgXE == true && g_bBlockXE == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].pos.x += VALUE_MOVE;
					}
				}

				//�J���X�̈ړ��͈�
				if (g_aEnemy[nCntEnemy].nPosX > 150)
				{
					//������ς���
					g_aEnemy[nCntEnemy].bLeft = !g_aEnemy[nCntEnemy].bLeft;	//bool�^�̃g�O��
					g_aEnemy[nCntEnemy].move.x *= -1;
					g_aEnemy[nCntEnemy].nPosX *= -1;
				}

				//�e�N�X�`����������
				if (g_aEnemy[nCntEnemy].bLeft == true)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f + 0.333f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f + 0.333f, 1.0f);
				}

				//�e�N�X�`�����E����
				else if (g_aEnemy[nCntEnemy].bLeft == false)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f + 0.333f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f + 0.333f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f, 1.0f);
				}

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MET / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MET / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MET / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MET / 2), 0.0f);
			}

			//�J���X����
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_002)
			{
				//�A�j���[�V����
				g_aEnemy[nCntEnemy].nCounterAnim++;	//�J�E���^���Z
				if (g_aEnemy[nCntEnemy].nCounterAnim == 7)//����
				{
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim++;
					if (g_aEnemy[nCntEnemy].nPatternAnim == 3)//����
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}
				}
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//�J���X����̈ړ�
				//��Փx�m�[�}��
				if (g_nEnemyLevel == GAMELEVEL_NORMAL)
				{
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 2.5;
				}
				//��Փx�n�[�h
				if (g_nEnemyLevel == GAMELEVEL_HARD)
				{
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 3.5;
				}

				//�v���C���[�̑���ɑΉ������ړ�
				if (pPlayer->bDisp == true && g_bBgXE == true && g_bBlockXE == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].pos.x += VALUE_MOVE;
					}
				}

				//�J���X����̈ړ��͈�
				if (g_aEnemy[nCntEnemy].pos.x < 0)
				{
					//��ʂ̒[���o�čs�����疢�g�p�ɂ���
					g_aEnemy[nCntEnemy].bUse = false;
				}

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f + 0.333f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f + 0.333f, 1.0f);

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MET�U / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MET�U / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MET�U / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MET�U / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MET�U / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MET�U / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MET�U / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MET�U / 2), 0.0f);
			}

			//�K�C�R�c
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_003)
			{
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					//��Փx�m�[�}��
					if (g_nEnemyLevel == GAMELEVEL_NORMAL)
					{
						//�N�[���^�C�����I����Ă���U���\
						if (g_aEnemy[nCntEnemy].nAttackTime == 0)
						{
							//�e�̔���
							if ((rand() % (g_CntEnemy * 150)) == 0)
							{
								//�v���C���[�̈ʒu���擾
								pPlayer = GetPlayer();

								float fHposx = pPlayer->pos.x, fHposy = pPlayer->pos.y;							//�ڕW(�v���C���[)�̍��W
								float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;	//�G�̍��W
								float fAngle;																	//�p�x

								fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//�p�x�����߂�

								//�e�𔭎ˁi�������j
								if (g_aEnemy[nCntEnemy].bLeft == true)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-sinf(fAngle)*9.0f, -cosf(fAngle)*9.0f, 0), BULLETTYPE_OCT, 95, D3DXCOLOR(255, 255, 255, 255), 50);
								}
								//�e�𔭎ˁi�E�����j
								if (g_aEnemy[nCntEnemy].bLeft == false)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-sinf(fAngle)*9.0f, -cosf(fAngle)*9.0f, 0), BULLETTYPE_OCT, 95, D3DXCOLOR(255, 255, 255, 255), 50);
								}
								//PlaySound(SOUND_LABEL_SE_SHOT2);	//�K�C�R�c�̃V���b�g��
								g_aEnemy[nCntEnemy].nAttackTime = ATTACK_CD*2;	//�U���̃N�[���^�C��
							}
						}
					}
					//��Փx�n�[�h
					if (g_nEnemyLevel == GAMELEVEL_HARD)
					{
						//�N�[���^�C�����I����Ă���U���\
						if (g_aEnemy[nCntEnemy].nAttackTime == 0)
						{
							//�e�̔���
							if ((rand() % (g_CntEnemy * 70)) == 0)
							{
								//�v���C���[�̈ʒu���擾
								pPlayer = GetPlayer();

								float fHposx = pPlayer->pos.x, fHposy = pPlayer->pos.y;							//�ڕW(�v���C���[)�̍��W
								float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;	//�G�̍��W
								float fAngle;																	//�p�x

								fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//�p�x�����߂�
								//�e�𔭎ˁi�������j
								if (g_aEnemy[nCntEnemy].bLeft == true)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-sinf(fAngle)*10.0f, -cosf(fAngle)*10.0f, 0), BULLETTYPE_OCT, 95, D3DXCOLOR(255, 255, 255, 255), 50);
								}
								//�e�𔭎ˁi�E�����j
								if (g_aEnemy[nCntEnemy].bLeft == false)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-sinf(fAngle)*10.0f, -cosf(fAngle)*10.0f, 0), BULLETTYPE_OCT, 95, D3DXCOLOR(255, 255, 255, 255), 50);
								}
								//PlaySound(SOUND_LABEL_SE_SHOT2);	//�K�C�R�c�̃V���b�g��
								g_aEnemy[nCntEnemy].nAttackTime = ATTACK_CD*2;	//�U���̃N�[���^�C��
							}
						}
					}
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//�K�C�R�c�̈ړ�
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 1;

				//�U���p�̃e�N�X�`���ɕύX
				if (g_aEnemy[nCntEnemy].nAttackTime > 0)
				{
					g_aEnemy[nCntEnemy].nAttackTime--;
					g_aEnemy[nCntEnemy].nPatternAnim = 1;
				}

				//�U�����Ă��Ȃ��Ƃ��̃e�N�X�`��
				if (g_aEnemy[nCntEnemy].nAttackTime == 0)
				{
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//�v���C���[�̑���ɑΉ������ړ�
				if (pPlayer->bDisp == true && g_bBgXE == true && g_bBlockXE == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].pos.x += VALUE_MOVE;
					}
				}

				//�Q�[��2�ŁA���t�g�ɂ���Ƃ�
				if (g_ModeEnemy == MODE_GAME2)
				{
					//�d��
					g_aEnemy[nCntEnemy].move.y += GRAVITY;

					if (g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT - ENEMY_SIZE_OCT + (ENEMY_SIZE_OCT / 2) - 80)
					{
						g_aEnemy[nCntEnemy].pos.y = SCREEN_HEIGHT - ENEMY_SIZE_OCT + (ENEMY_SIZE_OCT / 2) - 80;

						g_aEnemy[nCntEnemy].move.y = 0;	//�n�ʂɂ���Ƃ��͏d�͂�^���Ȃ�
					}
				}

				//������
				if (pPlayer->pos.x < g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = true;
				}

				//�E����
				if (pPlayer->pos.x > g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = false;
				}

				//�e�N�X�`����������
				if (g_aEnemy[nCntEnemy].bLeft == true)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
				}

				//�e�N�X�`�����E����
				else if (g_aEnemy[nCntEnemy].bLeft == false)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
				}

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_OCT / 2), 0.0f);
			}

			//�C�m�V�V
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_004)
			{
				//�A�j���[�V����
				g_aEnemy[nCntEnemy].nCounterAnim++;	//�J�E���^���Z
				if (g_aEnemy[nCntEnemy].nCounterAnim == 15)//����
				{
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim++;
					if (g_aEnemy[nCntEnemy].nPatternAnim == 2)//����
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}
				}

				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}

				//������
				if (pPlayer->pos.x < g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = true;
				}

				//�E����
				if (pPlayer->pos.x > g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = false;
				}

				//�e�N�X�`����������
				if (g_aEnemy[nCntEnemy].bLeft == true)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f + 0.5f, 1.0f);
				}

				//�e�N�X�`�����E����
				else if (g_aEnemy[nCntEnemy].bLeft == false)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
				}

				//�v���C���[�̑���ɑΉ������ړ�
				if (pPlayer->bDisp == true && g_bBgXE == true && g_bBlockXE == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].pos.x += VALUE_MOVE;
					}
				}

				//�C�m�V�V�̈ړ�(�v���C���[���ʏ펞�̃C�m�V�V���猩�ĉ�ʂ̎O���̈�̒����ɓ������瓮��)
				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_NORMAL)
				{
					if (pPlayer->pos.x + (PLAYER_SIZE / 2) >= g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GHO / 2) - (SCREEN_WIDTH / 3) &&
						pPlayer->pos.x + (PLAYER_SIZE / 2) < g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GHO / 2) + (SCREEN_WIDTH / 3) ||
						pPlayer->pos.x - (PLAYER_SIZE / 2) <= g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GHO / 2) + (SCREEN_WIDTH / 3) &&
						pPlayer->pos.x - (PLAYER_SIZE / 2) > g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GHO / 2) - (SCREEN_WIDTH / 3))
					{
						//���@�̈ʒu���擾
						pPlayer = GetPlayer();

						float fPposx = pPlayer->pos.x, fPposy = pPlayer->pos.y;	//���@�̍��W
						float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;		//�C�m�V�V�̍��W
						float fAngle;											//�p�x

						fAngle = atan2f((fEposx - fPposx), (fEposy - fPposy));	//�p�x�����߂�
						
						g_aEnemy[nCntEnemy].move.x = -sin(fAngle);
						//��Փx�m�[�}��
						if (g_nEnemyLevel == GAMELEVEL_NORMAL)
						{
							g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x * 4.2;
						}
						//��Փx�n�[�h
						if (g_nEnemyLevel == GAMELEVEL_HARD)
						{
							g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x * 4.8;
						}

						//�e�N�X�`����������
						if (g_aEnemy[nCntEnemy].bLeft == true)
						{
							//�e�N�X�`�����W�̐ݒ�
							pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
							pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
							pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
							pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
						}

						//�e�N�X�`�����E����
						else if (g_aEnemy[nCntEnemy].bLeft == false)
						{
							//�e�N�X�`�����W�̐ݒ�
							pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
							pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
							pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
							pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
						}
					}
				}

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GHO / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GHO / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GHO / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GHO / 2), 0.0f);
			}

			//�R�E����
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_005)
			{
				//�A�j���[�V����
				g_aEnemy[nCntEnemy].nCounterAnim++;	//�J�E���^���Z
				if (g_aEnemy[nCntEnemy].nCounterAnim == 5)//����
				{
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim++;
					if (g_aEnemy[nCntEnemy].nPatternAnim == 2)//����
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}
				}

				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}

				//������
				if (pPlayer->pos.x < g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = true;
				}

				//�E����
				if (pPlayer->pos.x > g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = false;
				}

				//�e�N�X�`����������
				if (g_aEnemy[nCntEnemy].bLeft == true)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
				}

				//�e�N�X�`�����E����
				else if (g_aEnemy[nCntEnemy].bLeft == false)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
				}

				//���@�̈ʒu���擾
				pPlayer = GetPlayer();

				float fPposx = pPlayer->pos.x, fPposy = pPlayer->pos.y;	//�ڕW(���@)�̍��W
				float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;		//�R�E�����̍��W
				float fAngle;											//�p�x

				fAngle = atan2f((fEposx - fPposx), (fEposy - fPposy));	//�p�x�����߂�

				g_aEnemy[nCntEnemy].move.x = -sin(fAngle);
				g_aEnemy[nCntEnemy].move.y = -cos(fAngle);

				//��Փx�m�[�}��
				if (g_nEnemyLevel == GAMELEVEL_NORMAL)
				{
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 2.0;
				}
				//��Փx�n�[�h
				if (g_nEnemyLevel == GAMELEVEL_HARD)
				{
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 3.5;
				}

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GSO / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GSO / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GSO / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GSO / 2), 0.0f);
			}

			//�A�C�X�h���S��
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_006)
			{

				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

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
					//�N�[���^�C�����I����Ă���U���\
					if (g_aEnemy[nCntEnemy].nAttackTime == 0)
					{
						//���̔���
						if ((rand() % (g_CntEnemy * 150)) == 0)
						{
							//�v���C���[�̈ʒu���擾
							pPlayer = GetPlayer();

							//����ɂ�����������
							SetEffect2(D3DXVECTOR3(pPlayer->pos.x, 180, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE * 2, EFFECTTYPE_002);

							g_aEnemy[nCntEnemy].nAttackTime = ATTACK_CD * 2;	//�U���̃N�[���^�C��
						}
					}

					//�g���l�[�h�̔���
					if ((rand() % (g_CntEnemy * 150)) == 0)
					{
						//�v���C���[�̈ʒu���擾
						pPlayer = GetPlayer();

						//�e�𔭎ˁi�������j
						if (g_aEnemy[nCntEnemy].bLeft == true)
						{
							float fHposx = pPlayer->pos.x, fHposy = pPlayer->pos.y;							//�ڕW(�v���C���[)�̍��W
							float fEposx = g_aEnemy[nCntEnemy].pos.x - 60, fEposy = g_aEnemy[nCntEnemy].pos.y + 90;	//�G�̍��W
							float fAngle;																	//�p�x

							fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//�p�x�����߂�

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 60, g_aEnemy[nCntEnemy].pos.y + 90, 0.0f), D3DXVECTOR3(-sinf(fAngle)*10.0f, -cosf(fAngle)*10.0f, 0), BULLETTYPE_BOSS, 95, D3DXCOLOR(255, 255, 255, 255), 85);
						}
						//�e�𔭎ˁi�E�����j
						if (g_aEnemy[nCntEnemy].bLeft == false)
						{
							float fHposx = pPlayer->pos.x, fHposy = pPlayer->pos.y;							//�ڕW(�v���C���[)�̍��W
							float fEposx = g_aEnemy[nCntEnemy].pos.x + 60, fEposy = g_aEnemy[nCntEnemy].pos.y + 90;	//�G�̍��W
							float fAngle;																	//�p�x

							fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//�p�x�����߂�

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 60, g_aEnemy[nCntEnemy].pos.y + 90, 0.0f), D3DXVECTOR3(-sinf(fAngle)*10.0f, -cosf(fAngle)*10.0f, 0), BULLETTYPE_BOSS, 95, D3DXCOLOR(255, 255, 255, 255), 85);
						}

					}

					//���@�̈ʒu���擾
					pPlayer = GetPlayer();

					float fPposx = pPlayer->pos.x, fPposy = pPlayer->pos.y;	//���@�̍��W
					float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;		//�A�C�X�h���S���̍��W
					float fAngle;											//�p�x

					fAngle = atan2f((fEposx - fPposx), (fEposy - fPposy));	//�p�x�����߂�

					g_aEnemy[nCntEnemy].move.x = -sin(fAngle);

					//�N�[���^�C�����I����Ă���ړ��\
					if (g_aEnemy[nCntEnemy].nAttackTime == 0)
					{
						//HP��4����1�����������ړ����x�A�b�v
						if (g_aEnemy[nCntEnemy].nLife < 375)
						{
							g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 0.8;
						}

						//�A�C�X�h���S���̈ړ�
						else
						{
							g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 0.2;
						}
					}
				}

				//��Փx�n�[�h
				if (g_nEnemyLevel == GAMELEVEL_HARD)
				{
					//�N�[���^�C�����I����Ă���U���\
					if (g_aEnemy[nCntEnemy].nAttackTime == 0)
					{
						//���̔���
						if ((rand() % (g_CntEnemy * 70)) == 0)
						{
							//�v���C���[�̈ʒu���擾
							pPlayer = GetPlayer();

							//����ɂ�����������
							SetEffect2(D3DXVECTOR3(pPlayer->pos.x, 180, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE * 2, EFFECTTYPE_002);

							g_aEnemy[nCntEnemy].nAttackTime = ATTACK_CD * 2;	//�U���̃N�[���^�C��
						}
					}

					//�g���l�[�h�̔���
					if ((rand() % (g_CntEnemy * 70)) == 0)
					{
						//�v���C���[�̈ʒu���擾
						pPlayer = GetPlayer();

						//�e�𔭎ˁi�������j
						if (g_aEnemy[nCntEnemy].bLeft == true)
						{
							float fHposx = pPlayer->pos.x, fHposy = pPlayer->pos.y;							//�ڕW(�v���C���[)�̍��W
							float fEposx = g_aEnemy[nCntEnemy].pos.x - 60, fEposy = g_aEnemy[nCntEnemy].pos.y + 90;	//�G�̍��W
							float fAngle;																	//�p�x

							fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//�p�x�����߂�

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 60, g_aEnemy[nCntEnemy].pos.y+90, 0.0f), D3DXVECTOR3(-sinf(fAngle)*12.0f, -cosf(fAngle)*12.0f, 0), BULLETTYPE_BOSS, 95, D3DXCOLOR(255, 255, 255, 255), 85);
						}
						//�e�𔭎ˁi�E�����j
						if (g_aEnemy[nCntEnemy].bLeft == false)
						{
							float fHposx = pPlayer->pos.x, fHposy = pPlayer->pos.y;							//�ڕW(�v���C���[)�̍��W
							float fEposx = g_aEnemy[nCntEnemy].pos.x + 60, fEposy = g_aEnemy[nCntEnemy].pos.y + 90;	//�G�̍��W
							float fAngle;																	//�p�x

							fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//�p�x�����߂�

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 60, g_aEnemy[nCntEnemy].pos.y+90, 0.0f), D3DXVECTOR3(-sinf(fAngle)*12.0f, -cosf(fAngle)*12.0f, 0), BULLETTYPE_BOSS, 95, D3DXCOLOR(255, 255, 255, 255), 85);
						}

					}

					//���@�̈ʒu���擾
					pPlayer = GetPlayer();

					float fPposx = pPlayer->pos.x, fPposy = pPlayer->pos.y;	//���@�̍��W
					float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;		//�A�C�X�h���S���̍��W
					float fAngle;											//�p�x

					fAngle = atan2f((fEposx - fPposx), (fEposy - fPposy));	//�p�x�����߂�

					g_aEnemy[nCntEnemy].move.x = -sin(fAngle);

					//�N�[���^�C�����I����Ă���ړ��\
					if (g_aEnemy[nCntEnemy].nAttackTime == 0)
					{
						//HP��4����1�����������ړ����x�A�b�v
						if (g_aEnemy[nCntEnemy].nLife < 375)
						{
							g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 2.0;
						}

						//�A�C�X�h���S���̈ړ�
						else
						{
							g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 0.5;
						}
					}
				}

				//������
				if (pPlayer->pos.x < g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = true;
				}

				//�E����
				if (pPlayer->pos.x > g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = false;
				}

				//�U���p�̃e�N�X�`���ɕύX
				if (g_aEnemy[nCntEnemy].nAttackTime > 0)
				{
					g_aEnemy[nCntEnemy].nAttackTime--;

					//�e�N�X�`����������
					if (g_aEnemy[nCntEnemy].bLeft == true)
					{
						//�e�N�X�`�����W�̐ݒ�
						pVtx[0].tex = D3DXVECTOR2(0.75f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.75f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
					}

					//�e�N�X�`�����E����
					else if (g_aEnemy[nCntEnemy].bLeft == false)
					{
						//�e�N�X�`�����W�̐ݒ�
						pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.75f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.75f, 1.0f);
					}
				}
				else
				{
					//�A�j���[�V����
					g_aEnemy[nCntEnemy].nCounterAnim++;	//�J�E���^���Z
					if (g_aEnemy[nCntEnemy].nCounterAnim == 15)//����
					{
						g_aEnemy[nCntEnemy].nCounterAnim = 0;
						g_aEnemy[nCntEnemy].nPatternAnim++;
						if (g_aEnemy[nCntEnemy].nPatternAnim == 2)//����
						{
							g_aEnemy[nCntEnemy].nPatternAnim = 0;
						}
					}

					//�e�N�X�`����������
					if (g_aEnemy[nCntEnemy].bLeft == true)
					{
						//�e�N�X�`�����W�̐ݒ�
						pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f + 0.25f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f + 0.25f, 1.0f);
					}

					//�e�N�X�`�����E����
					else if (g_aEnemy[nCntEnemy].bLeft == false)
					{
						//�e�N�X�`�����W�̐ݒ�
						pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f + 0.25f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f + 0.25f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f, 1.0f);
					}
				}

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_BOSS_X / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_BOSS_Y / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_BOSS_X / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_BOSS_Y / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_BOSS_X / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_BOSS_Y / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_BOSS_X / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_BOSS_Y / 2), 0.0f);
			}

			//�s���j�A
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_007)
			{
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//�s���j�A�̈ړ�
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 1;

				//�v���C���[�̑���ɑΉ������ړ�
				if (pPlayer->bDisp == true && g_bBgXE == true && g_bBlockXE == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].pos.x += VALUE_MOVE;
					}
				}

				if (g_aEnemy[nCntEnemy].move.y == -1.0f)
				{
					//������ς���
					g_aEnemy[nCntEnemy].bLeft = !g_aEnemy[nCntEnemy].bLeft;	//bool�^�̃g�O��
				}

				//�d��
				g_aEnemy[nCntEnemy].move.y += 0.5;

				//���ʂ����яo��
				if (g_aEnemy[nCntEnemy].move.y > 20.0f)
				{
					//������ς���
					g_aEnemy[nCntEnemy].bLeft = !g_aEnemy[nCntEnemy].bLeft;	//bool�^�̃g�O��

					//�W�����v�̂悤�ɔ�тł�
					g_aEnemy[nCntEnemy].move.y = 0;
					g_aEnemy[nCntEnemy].move.y -= 20.0f;
				}

				//�������̃e�N�X�`��
				if (g_aEnemy[nCntEnemy].bLeft == true)
				{
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//�㏸���̃e�N�X�`��
				if (g_aEnemy[nCntEnemy].bLeft == false)
				{
					g_aEnemy[nCntEnemy].nPatternAnim = 1;
				}

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_PIR / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_PIR / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_PIR / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_PIR / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_PIR / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_PIR / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_PIR / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_PIR / 2), 0.0f);

			}

			//�_�[�N�K�C�R�c
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_008)
			{
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					//��Փx�m�[�}��
					if (g_nEnemyLevel == GAMELEVEL_NORMAL)
					{
						//�N�[���^�C�����I����Ă���U���\
						if (g_aEnemy[nCntEnemy].nAttackTime == 0)
						{
							//�e�̔���
							if ((rand() % (g_CntEnemy * 150)) == 0)
							{
								//�e�𔭎ˁi�������j
								if (g_aEnemy[nCntEnemy].bLeft == true)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-2, -2, 0), BULLETTYPE_GHO, 180, D3DXCOLOR(255, 255, 255, 255), 75);
								}
								//�e�𔭎ˁi�E�����j
								if (g_aEnemy[nCntEnemy].bLeft == false)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(2, -2, 0), BULLETTYPE_GHO, 180, D3DXCOLOR(255, 255, 255, 255), 75);
								}
								//PlaySound(SOUND_LABEL_SE_SHOT2);	//�K�C�R�c�̃V���b�g��
								g_aEnemy[nCntEnemy].nAttackTime = ATTACK_CD * 2;	//�U���̃N�[���^�C��
							}
						}
					}
					//��Փx�n�[�h
					if (g_nEnemyLevel == GAMELEVEL_HARD)
					{
						//�N�[���^�C�����I����Ă���U���\
						if (g_aEnemy[nCntEnemy].nAttackTime == 0)
						{
							//�e�̔���
							if ((rand() % (g_CntEnemy * 70)) == 0)
							{
								//�e�𔭎ˁi�������j
								if (g_aEnemy[nCntEnemy].bLeft == true)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-2, -2, 0), BULLETTYPE_GHO, 180, D3DXCOLOR(255, 255, 255, 255), 75);
								}
								//�e�𔭎ˁi�E�����j
								if (g_aEnemy[nCntEnemy].bLeft == false)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(2, -2, 0), BULLETTYPE_GHO, 180, D3DXCOLOR(255, 255, 255, 255), 75);
								}
								//PlaySound(SOUND_LABEL_SE_SHOT2);	//�K�C�R�c�̃V���b�g��
								g_aEnemy[nCntEnemy].nAttackTime = ATTACK_CD * 2;	//�U���̃N�[���^�C��
							}
						}
					}
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//�ʏ�`��ɖ߂�����
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//�_�[�N�K�C�R�c�̈ړ�
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 1;

				//�U���p�̃e�N�X�`���ɕύX
				if (g_aEnemy[nCntEnemy].nAttackTime > 0)
				{
					g_aEnemy[nCntEnemy].nAttackTime--;
					g_aEnemy[nCntEnemy].nPatternAnim = 1;
				}

				//�U�����Ă��Ȃ��Ƃ��̃e�N�X�`��
				if (g_aEnemy[nCntEnemy].nAttackTime == 0)
				{
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//�v���C���[�̑���ɑΉ������ړ�
				if (pPlayer->bDisp == true && g_bBgXE == true && g_bBlockXE == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].pos.x += VALUE_MOVE;
					}
				}

				//������
				if (pPlayer->pos.x < g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = true;
				}

				//�E����
				if (pPlayer->pos.x > g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = false;
				}

				//�e�N�X�`����������
				if (g_aEnemy[nCntEnemy].bLeft == true)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
				}

				//�e�N�X�`�����E����
				else if (g_aEnemy[nCntEnemy].bLeft == false)
				{
					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
				}

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_OCT / 2), 0.0f);
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

			//�r�[�g��
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_000)
			{
				//�G�̗̑�
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_MOS;
			}

			//�J���X
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_001)
			{
				//�G�̗̑�
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_MET;
			}

			//�J���X����
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_002)
			{
				//�G�̗̑�
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_MET�U;
			}

			//�K�C�R�c
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_003)
			{
				//��Փx�m�[�}��
				if (g_nEnemyLevel == GAMELEVEL_NORMAL)
				{
					//�G�̗̑�
					g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_OCT_NORMAL;
				}

				//��Փx�n�[�h
				if (g_nEnemyLevel == GAMELEVEL_HARD)
				{
					//�G�̗̑�
					g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_OCT_HARD;
				}
			}

			//�C�m�V�V
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_004)
			{
				//�G�̗̑�
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_GHO;
			}

			//�R�E����
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_005)
			{
				//�G�̗̑�
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_GSO;
			}

			//�A�C�X�h���S��
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_006)
			{
				//�G�̗̑�
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_BOSS;
			}

			//�s���j�A
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_007)
			{
				//�G�̗̑�
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_PIR;
			}

			//�_�[�N�K�C�R�c
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_008)
			{
				//�G�̗̑�
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_DARK;
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
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nBat;			//�R�E�������o������ʒu

	PlaySound(SOUND_LABEL_SE_EXPLOSION);	//������

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)//�G�̗̑͂�0�ȉ��ɂȂ�����
	{
		//�r�[�g���̃X�R�A
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_000)
		{
			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_MOS);

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(500);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(1000);
			}
		}

		//�J���X�̃X�R�A
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_001)
		{
			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_MET);

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(750);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(1500);
			}
		}

		//�J���X����̃X�R�A
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_002)
		{
			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_MET�U);

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(7500);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(10000);
			}
		}

		//�K�C�R�c�̃X�R�A
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_003)
		{
			//�Q�[��2�ł́A�|�����K�C�R�c�𐔂���
			if (g_ModeEnemy == MODE_GAME2)
			{
				g_nCntBorn++;

				//��Փx�m�[�}��
				if (g_nEnemyLevel == GAMELEVEL_NORMAL && g_nCntBorn >= 3)
				{
					g_nCntBorn = 0;

					//���̒��ւ̑����ݒu
					SetBlock(D3DXVECTOR3(300, 0, 0.0f), 200, 30, BLOCKTYPE_001, 30);	//���̑���
					SetBlock(D3DXVECTOR3(980, 0, 0.0f), 200, 30, BLOCKTYPE_001, 30);	//�E�̑���
					SetBlock(D3DXVECTOR3(550, -240, 0.0f), 80, 80, BLOCKTYPE_008, 30);	//W�Ŕ�
					SetBlock(D3DXVECTOR3(640, -200, 0.0f), 300, 30, BLOCKTYPE_005, 30);	//���̒��ւ̑���

					//���E�̑���ɃA�C�e����ݒu
					SetItem(D3DXVECTOR3(250, -45, 0.0f), ITEMTYPE_002, 70);		//�M
					SetItem(D3DXVECTOR3(1030, -45, 0.0f), ITEMTYPE_002, 70);	//�M

					//���̒��ւ̃��[�v�]�[����ݒu
					SetBlock(D3DXVECTOR3(640, -250, 0.0f), 150, 150, BLOCKTYPE_007, 30);	//���̒��ւ̃��[�v�]�[��
				}

				//��Փx�n�[�h
				if (g_nEnemyLevel == GAMELEVEL_HARD && g_nCntBorn >= 6)
				{
					g_nCntBorn = 0;

					//���̒��ւ̑����ݒu
					SetBlock(D3DXVECTOR3(300, 0, 0.0f), 200, 30, BLOCKTYPE_001, 30);	//���̑���
					SetBlock(D3DXVECTOR3(980, 0, 0.0f), 200, 30, BLOCKTYPE_001, 30);	//�E�̑���
					SetBlock(D3DXVECTOR3(550, -240, 0.0f), 80, 80, BLOCKTYPE_008, 30);	//W�Ŕ�
					SetBlock(D3DXVECTOR3(640, -200, 0.0f), 300, 30, BLOCKTYPE_005, 30);	//���̒��ւ̑���

					//���E�̑���ɃA�C�e����ݒu
					SetItem(D3DXVECTOR3(250, -45, 0.0f), ITEMTYPE_002, 70);		//�M
					SetItem(D3DXVECTOR3(1030, -45, 0.0f), ITEMTYPE_002, 70);	//�M

					//���̒��ւ̃��[�v�]�[����ݒu
					SetBlock(D3DXVECTOR3(640, -250, 0.0f), 150, 150, BLOCKTYPE_007, 30);	//���̒��ւ̃��[�v�]�[��
				}
			}

			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_OCT);

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(2500);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(5000);
			}
		}

		//�C�m�V�V�̃X�R�A
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_004)
		{
			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_GHO);

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(3000);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(4000);
			}
		}

		//�R�E�����̃X�R�A
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_005)
		{
			//�R�E������|�������𐔂���
			g_nCntBat++;

			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_GSO);

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				//10�̂��K�C�R�c���~�点��
				if (g_nCntBat == 10)//��
				{
					SetEnemy(D3DXVECTOR3(270, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//�K�C�R�c
				}

				if (g_nCntBat == 20)//�E
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 270, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//�K�C�R�c
				}

				if (g_nCntBat == 30)//��
				{
					SetEnemy(D3DXVECTOR3(320, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//�K�C�R�c
				}

				//�R�E������|��������30�̈ȉ��Ȃ�A�����_���Ŏ��̃R�E�����𐶐�
				if (g_nCntBat < 30)
				{

					nBat = rand() % 15 + 1;

					//�E�[
					if (nBat == 1)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 400, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//�E�[
					if (nBat == 2)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 200, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//�E�[
					if (nBat == 3)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 600, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//�E�[
					if (nBat == 4)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 300, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//�E�[
					if (nBat == 5)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 500, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//���[
					if (nBat == 6)
					{
						SetEnemy(D3DXVECTOR3(0, 200, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//���[
					if (nBat == 7)
					{
						SetEnemy(D3DXVECTOR3(0, 400, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//���[
					if (nBat == 8)
					{
						SetEnemy(D3DXVECTOR3(0, 600, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//���[
					if (nBat == 9)
					{
						SetEnemy(D3DXVECTOR3(0, 300, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//���[
					if (nBat == 10)
					{
						SetEnemy(D3DXVECTOR3(0, 500, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//������
					if (nBat == 11)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//������
					if (nBat == 12)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 +100, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//������
					if (nBat == 13)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 +200, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//������
					if (nBat == 14)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 -100, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//������
					if (nBat == 15)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 -200, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}
				}

				ScoreEffect(250);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				//10�̂��K�C�R�c���~�点��
				if (g_nCntBat == 10)//��
				{
					SetEnemy(D3DXVECTOR3(270, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//�K�C�R�c
				}

				if (g_nCntBat == 20)//�E
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 270, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//�K�C�R�c
				}

				if (g_nCntBat == 30)//��
				{
					SetEnemy(D3DXVECTOR3(320, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//�K�C�R�c
				}

				if (g_nCntBat == 40)//�E
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 320, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//�K�C�R�c
				}

				if (g_nCntBat == 50)//���E
				{
					SetEnemy(D3DXVECTOR3(370, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//�K�C�R�c
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 370, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//�K�C�R�c
				}

				//�R�E������|��������50�̈ȉ��Ȃ�A�����_���Ŏ��̃R�E�����𐶐�
				if (g_nCntBat < 50)
				{

					nBat = rand() % 15 + 1;

					//�E�[
					if (nBat == 1)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 400, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//�E�[
					if (nBat == 2)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 200, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//�E�[
					if (nBat == 3)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 600, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//�E�[
					if (nBat == 4)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 300, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//�E�[
					if (nBat == 5)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 500, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//���[
					if (nBat == 6)
					{
						SetEnemy(D3DXVECTOR3(0, 200, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//���[
					if (nBat == 7)
					{
						SetEnemy(D3DXVECTOR3(0, 400, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//���[
					if (nBat == 8)
					{
						SetEnemy(D3DXVECTOR3(0, 600, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//���[
					if (nBat == 9)
					{
						SetEnemy(D3DXVECTOR3(0, 300, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//���[
					if (nBat == 10)
					{
						SetEnemy(D3DXVECTOR3(0, 500, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//������
					if (nBat == 11)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//������
					if (nBat == 12)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 100, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//������
					if (nBat == 13)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//������
					if (nBat == 14)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 100, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}

					//������
					if (nBat == 15)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
					}
				}

				ScoreEffect(500);
			}
		}

		//�A�C�X�h���S��
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_006)
		{
			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_BOSS_Y);

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(12500);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(21875);
			}

			PlaySound(SOUND_LABEL_SE_VOICE);	//�{�X���j��
			StopSound(SOUND_LABEL_BGM004);	//�{�X���BGM���~�߂�
		}

		//�s���j�A
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_007)
		{
			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_PIR);

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(1000);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(2000);
			}
		}

		//�_�[�N�K�C�R�c�̃X�R�A
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_008)
		{
			//��������
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_OCT);

			//��Փx�m�[�}��
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(4000);
			}
			//��Փx�n�[�h
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(6000);
			}
		}

		g_aEnemy[nCntEnemy].bUse = false;//�G�𖢎g�p�ɂ���

		// �ݒ��߂�
		pVtx[(nCntEnemy * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntEnemy * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntEnemy * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntEnemy * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	else
	{
		//�_���[�W��Ԃ�
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;

		//�{�X�̓_���[�W��Ԃ�20f�̎���
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_006)
		{
			g_aEnemy[nCntEnemy].nCounterState = INVINCIBLE_FRAME�U;
		}
		else
		{
			g_aEnemy[nCntEnemy].nCounterState = INVINCIBLE_FRAME;
		}

		if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_DAMAGE)
		{
			// ���_�J���[�̐ݒ�
			pVtx[(nCntEnemy * 4) + 0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[(nCntEnemy * 4) + 1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[(nCntEnemy * 4) + 2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[(nCntEnemy * 4) + 3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		}
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// �G�l�~�[�̃_���[�W�ݒ�2
//=============================================================================
bool HitEnemy2(void)
{
	return g_bHitEnemy;
}