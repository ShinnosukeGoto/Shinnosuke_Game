//=============================================================================
//
// �i�C���X�g�[���[�Y�̏��� [block.cpp](�u���b�N)
// Author:�㓡�T�V��
//
//=============================================================================
#include "block.h"
#include "player.h"
#include "input.h"
#include "bg.h"
#include "game.h"
#include "game2.h"
#include "fade.h"
#include "explosion.h"
#include "item.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureBlock[BLOCKTYPE_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

int g_nCounterAnimBlock;	//�A�j���[�V�����J�E���^�[
int g_nPatternAnimBlock;		//�A�j���[�V�����p�^�[��
BLOCK g_aBlock[MAX_BLOCK];
bool g_bBgXB;					//�X�e�[�W1�̉����������ǂ���
bool g_bBlockX;					//�u���b�N�̉��ɐG��Ă��邩�ǂ���
bool g_bBlockY;					//�X�e�[�W2�Ńu���b�N����~�ʒu�܂ł��ǂ蒅�������ǂ���
MODE g_ModeBlock;				//�Q�[����ʂɑΉ�

//=====================================================
// ����������
//=====================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nCntBlock;		// ���[�J���ϐ�

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].nSizeX = 0;
		g_aBlock[nCntBlock].nSizeY = 0;
		g_aBlock[nCntBlock].type = BLOCKTYPE_000;
		g_aBlock[nCntBlock].nLife = 0;
		g_aBlock[nCntBlock].bUse = false;
	}

	//�O���[�o���ϐ��̏�����
	g_nCounterAnimBlock = 0;
	g_nPatternAnimBlock = 0;
	g_bBgXB = false;
	g_bBlockX = false;
	g_bBlockY = false;
	g_ModeBlock = MODE_GAME;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/block000.png",&g_pTextureBlock[0]);		//�ʂ蔲���s�\�ȃu���b�N
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/block002.png", &g_pTextureBlock[1]);	//�ʂ蔲���\�Ȕ�
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/object000.png", &g_pTextureBlock[2]);	//W�₶�邵�Ŕ�
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/object001.png", &g_pTextureBlock[3]);	//��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/entrance000.png", &g_pTextureBlock[4]);	//������
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/block002.png", &g_pTextureBlock[5]);	//�ʂ蔲���s�\�ȃu���b�N
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/block004.png", &g_pTextureBlock[6]);	//���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/effect002.png", &g_pTextureBlock[7]);	//���̒��ւ̃��[�v�]�[��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/object002.png", &g_pTextureBlock[8]);	//W�Ŕ�
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/block005.png", &g_pTextureBlock[9]);	//�󂹂�u���b�N
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/tower000.png", &g_pTextureBlock[10]);	//�^���[
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/block003.png", &g_pTextureBlock[11]);	//��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/object003.png", &g_pTextureBlock[12]);	//�c�ɒ�����
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/object004.png", &g_pTextureBlock[13]);	//S�₶�邵�Ŕ�
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/object005.png", &g_pTextureBlock[14]);	//�A�X���`�b�N�̒�
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/block006.png", &g_pTextureBlock[15]);	//�g�Q�u���b�N

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BLOCK,		// �o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,					// (�Œ�)
		FVF_VERTEX_2D,						// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// (�Œ�)
		&g_pVtxBuffBlock,					// �ϐ������ς��ƕύX���K�v
		NULL);

	VERTEX_2D * pVtx;		// ���_���ւ̃|�C���^

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);	// ���̏����͕ς��Ȃ�����

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		// ���_���W�̐ݒ�( Z�̌`�ɔz�u����)  / �l�p�`�̒��_�̈ʒu
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// rhw�̐ݒ�(�l��1.0�ŌŒ�)		/ ���܂�ς��邱�Ƃ͂Ȃ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		// ���_�J���[�̐ݒ�(0�`255�̐��l�Őݒ�)	/ �S�̂̐F
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
															// �e�N�X�`�����W�̐ݒ�		��ʂɑ΂��Ă̕�����
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// �u���b�N�̏I������
//=============================================================================
void UninitBlock(void)
{
	//�e�N�X�`���̊J��
	for (int nCntBlock = 0; nCntBlock < BLOCKTYPE_MAX; nCntBlock++)
	{
		if (g_pTextureBlock[nCntBlock] != NULL)
		{
			g_pTextureBlock[nCntBlock]->Release();
			g_pTextureBlock[nCntBlock] = NULL;
		}
	}
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//=============================================================================
// �u���b�N�̍X�V����
//=============================================================================
void UpdateBlock(void)
{
	VERTEX_2D *pVtx;//���_���̃|�C���^
	PLAYER*pPlayer;
	ITEM*pItem;

	//�Q�[���̃X�e�[�W���擾
	g_ModeBlock = GetMode();

	//���_�o�b�t�@���b�N
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			//�Q�[��1(��̕���)
			if (g_ModeBlock == MODE_GAME)
			{
				//�w�i�̉����������ǂ����擾
				g_bBgXB = GetBg();
				pPlayer = GetPlayer();

				//�v���C���[�̑���ɑΉ������ړ�
				if (pPlayer->bDisp == true && g_bBgXB == true && g_bBlockX == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aBlock[nCntBlock].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aBlock[nCntBlock].pos.x += VALUE_MOVE;
					}
				}

				//�󂹂�u���b�N
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_009)
				{
					//�U����������Ɖ���
					if (g_aBlock[nCntBlock].nLife <= 0)
					{
						//��������
						SetExplosion(g_aBlock[nCntBlock].pos, D3DXCOLOR(255, 255, 255, 255), g_aBlock[nCntBlock].nSizeX);
						g_aBlock[nCntBlock].bUse = false;
					}
				}

				//��
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_011)
				{
					//�p�^�[����؂�ւ���
					g_nCounterAnimBlock++;
					if (g_nCounterAnimBlock == 20)
					{
						g_nCounterAnimBlock = 0;
						g_nPatternAnimBlock++;
						if (g_nPatternAnimBlock == 3)
						{
							g_nPatternAnimBlock = 0;
						}
					}

					//�e�N�X�`�����W���X�V
					pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.333f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.333f + 0.333f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.333f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.333f + 0.333f, 1.0f);
				}
				else
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}
			}

			//�Q�[��2(������)
			if (g_ModeBlock == MODE_GAME2)
			{
				//���t�g�ɏ���Ă��邩��A�u���b�N�͉��Ɉړ�
				g_aBlock[nCntBlock].pos.y += 1.0;

				//���t�g���~�܂邩��A�u���b�N���~�܂�
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_001)	//���E�̑���
				{
					if (g_aBlock[nCntBlock].pos.y > 480)
					{
						g_aBlock[nCntBlock].pos.y = 480;
					}
				}

				//���t�g���~�܂邩��A�u���b�N���~�܂�
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_005)	//�^�񒆂̑���
				{
					if (g_aBlock[nCntBlock].pos.y > 280)
					{
						g_aBlock[nCntBlock].pos.y = 280;
					}
				}

				//���t�g���~�܂邩��AW�Ŕ��~�܂�
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_008)	//W�Ŕ�
				{
					if (g_aBlock[nCntBlock].pos.y > 240)
					{
						g_aBlock[nCntBlock].pos.y = 240;
					}
				}

				//���t�g���~�܂邩��A���[�v�]�[�����~�܂�
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_007)	//�^�񒆂̃��[�v�]�[��
				{
					if (g_aBlock[nCntBlock].pos.y > 230)
					{
						g_aBlock[nCntBlock].pos.y = 230;
						g_bBlockY = true;
					}

					//�p�^�[����؂�ւ���
					g_nCounterAnimBlock++;
					if (g_nCounterAnimBlock == 3)
					{
						g_nCounterAnimBlock = 0;
						g_nPatternAnimBlock++;
						if (g_nPatternAnimBlock == 10)
						{
							g_nPatternAnimBlock = 0;
						}
					}

					//�e�N�X�`�����W���X�V
					pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.1f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.1f + 0.1f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.1f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.1f + 0.1f, 1.0f);
				}
				else
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}
			}

			//�Q�[��3(���̒�)
			if (g_ModeBlock == MODE_GAME3)
			{
				pItem = GetItem();	//�P�̏󋵂��擾

				//���
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_006)
				{
					//�d��
					g_aBlock[nCntBlock].pos.y += 2.5;	//���̗������x(2.0)

					//���ɓ�����
					if (g_aBlock[nCntBlock].pos.y > 650 - (g_aBlock[nCntBlock].nSizeY / 2))
					{
						g_aBlock[nCntBlock].pos.y = 650 - (g_aBlock[nCntBlock].nSizeY / 2);
						g_aBlock[nCntBlock].nLife--;//���ɓ����邱�ƂŃ��C�t�����炷
					}

					//�P���~�o���������n����
					if (pItem->type == ITEMTYPE_005 && pItem->bUse == true)
					{
						g_aBlock[nCntBlock].nLife = 0;
					}

					//���Ԃł�炪�n����
					if (g_aBlock[nCntBlock].nLife <= 0)
					{
						g_aBlock[nCntBlock].col.r -= 0.01;
						g_aBlock[nCntBlock].col.a -= 0.02;

						//�����x��0.1����������疢�g�p��
						if (g_aBlock[nCntBlock].col.a <= 0.1)
						{
							g_aBlock[nCntBlock].bUse = false;
						}
					}

					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

					//���_�J���[�̍X�V
					pVtx[0].col = D3DXCOLOR(g_aBlock[nCntBlock].col.r, g_aBlock[nCntBlock].col.g, g_aBlock[nCntBlock].col.b, g_aBlock[nCntBlock].col.a);
					pVtx[1].col = D3DXCOLOR(g_aBlock[nCntBlock].col.r, g_aBlock[nCntBlock].col.g, g_aBlock[nCntBlock].col.b, g_aBlock[nCntBlock].col.a);
					pVtx[2].col = D3DXCOLOR(g_aBlock[nCntBlock].col.r, g_aBlock[nCntBlock].col.g, g_aBlock[nCntBlock].col.b, g_aBlock[nCntBlock].col.a);
					pVtx[3].col = D3DXCOLOR(g_aBlock[nCntBlock].col.r, g_aBlock[nCntBlock].col.g, g_aBlock[nCntBlock].col.b, g_aBlock[nCntBlock].col.a);
				}
			}

			//���_���W�̐ݒ�i�E���Őݒ肷��j
			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);

		}
		pVtx += 4;
	}

	//���_�f�[�^�A�����b�N
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// �u���b�N�̕`�揈��
//=============================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBlock[g_aBlock[nCntBlock].type]);

			// �`��̎��		�`��J�n���_	���`�悷�邩
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBlock, NUM_POLYGON);
		}
	}
}

//=====================================================
// �u���b�N�̐ݒ�
//=====================================================
void SetBlock(D3DXVECTOR3 pos, int nSizeX, int nSizeY, BLOCKTYPE type, int nLife)
{
	VERTEX_2D *pVtx;
	int nCntBlock;		// ���[�J���ϐ�

	//���_�f�[�^�����b�N
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].nSizeX = nSizeX;
			g_aBlock[nCntBlock].nSizeY = nSizeY;
			g_aBlock[nCntBlock].type = type;
			g_aBlock[nCntBlock].nLife = nLife;

			g_aBlock[nCntBlock].col.r = 1.0f;
			g_aBlock[nCntBlock].col.g = 1.0f;
			g_aBlock[nCntBlock].col.b = 1.0f;
			g_aBlock[nCntBlock].col.a = 1.0f;

			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);

			g_aBlock[nCntBlock].bUse = true;
			break;
		}
		pVtx += 4;
	}

	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// �u���b�N���̑��M
//=============================================================================
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}

//=============================================================================
//�u���b�N�̓����蔻��
//=============================================================================
bool CollisionBlock(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, int nSize)
{
	PLAYER *pPlayer;
	int nCntBlock;		// ���[�J���ϐ�

	bool bLand = false;		//���n�����̂��ǂ���
	g_bBlockX = false;		//�u���b�N�ɉ����瓖�����Ă��邩�ǂ���
	pPlayer = GetPlayer();	//�v���C���[�̏󋵂��l��
	int nFade = GetFade();	//�t�F�[�h�̏����l��

	//�w�i�̉����������ǂ����擾
	g_bBgXB = GetBg();

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			//�ʂ蔲���ł��Ȃ��u���b�N
			if (g_aBlock[nCntBlock].type == BLOCKTYPE_000 || g_aBlock[nCntBlock].type == BLOCKTYPE_009)
			{
				//�w�i�������Ȃ��Ƃ�
				if (g_bBgXB == false)
				{
					// �u���b�N�Ƃ̓����蔻��
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// �E
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// ��
					{
						if (pPlayer->pos.y + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{	// �u���b�N�ɏォ�瓖��������
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeY / 2);		// �u���b�N�̏�ɏ��
							bLand = true;		// �u���b�N�̏�ɒ��n���Ă���
						}
						else if (pPlayer->pos.y - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y - (PLAYER_SIZE / 2) >= g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))
						{	// �u���b�N�ɉ����瓖��������
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y + (PLAYER_SIZE / 2) + (g_aBlock[nCntBlock].nSizeY / 2);		// �v���C���[���u���b�N�ɂ߂荞�܂Ȃ��悤�ɂ���
						}
						else if (pPlayer->pos.x + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->posOld.x + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))
						{	// �u���b�N�ɉE���瓖��������
							pPlayer->pos.x = g_aBlock[nCntBlock].pos.x - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeX / 2);
						}
						else if (pPlayer->pos.x - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->posOld.x - (PLAYER_SIZE / 2) >= g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))
						{	// �u���b�N�ɍ����瓖��������
							pPlayer->pos.x = g_aBlock[nCntBlock].pos.x + (PLAYER_SIZE / 2) + (g_aBlock[nCntBlock].nSizeX / 2);
						}
					}
				}

				//�w�i�������Ƃ�
				if (g_bBgXB == true)
				{
					// �u���b�N�Ƃ̓����蔻��
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// �E
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// ��
					{
						if (pPlayer->pos.y + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{	// �u���b�N�ɏォ�瓖��������
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeY / 2);		// �u���b�N�̏�ɏ��
							bLand = true;		// �u���b�N�̏�ɒ��n���Ă���
						}
						else if (pPlayer->pos.y - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y - (PLAYER_SIZE / 2) >= g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))
						{	// �u���b�N�ɉ����瓖��������
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y + (PLAYER_SIZE / 2) + (g_aBlock[nCntBlock].nSizeY / 2);		// �v���C���[���u���b�N�ɂ߂荞�܂Ȃ��悤�ɂ���
						}
						else if (pPlayer->pos.x + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->pos.x + (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))
						{	// �u���b�N�ɉE���瓖��������
							//�������Ȃ獶�ɓ�����
							if (pPlayer->bLeft == true)
							{
								g_bBlockX = false;
							}
							else
							{
								g_bBlockX = true;
							}
						}
						else if (pPlayer->pos.x - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->pos.x - (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))
						{	// �u���b�N�ɍ����瓖��������
							//�E�����Ȃ�E�ɓ�����
							if (pPlayer->bLeft == false)
							{
								g_bBlockX = false;
							}
							else
							{
								g_bBlockX = true;
							}
						}
					}
				}
			}

			//�ʂ蔲���ł���u���b�N
			if (g_aBlock[nCntBlock].type == BLOCKTYPE_001 || g_aBlock[nCntBlock].type == BLOCKTYPE_005)
			{
				//�w�i�������Ȃ��Ƃ�
				if (g_bBgXB == false)
				{
					// �u���b�N�Ƃ̓����蔻��
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// �E
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// ��
					{
						if (pPlayer->pos.y + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{	// �u���b�N�ɏォ�瓖��������
							if (GetKeyboardPress(DIK_S))
							{
								
							}
							else
							{
								pPlayer->pos.y = g_aBlock[nCntBlock].pos.y - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeY / 2);		// �u���b�N�̏�ɏ��
							}
							bLand = true;		// �u���b�N�̏�ɒ��n���Ă���
						}
					}
				}

				//�w�i�������Ƃ�
				if (g_bBgXB == true)
				{
					// �u���b�N�Ƃ̓����蔻��
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// �E
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// ��
					{
						if (pPlayer->pos.y + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{	// �u���b�N�ɏォ�瓖��������
							if (GetKeyboardPress(DIK_S))
							{
								
							}
							else
							{
								pPlayer->pos.y = g_aBlock[nCntBlock].pos.y - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeY / 2);		// �u���b�N�̏�ɏ��
							}
							bLand = true;		// �u���b�N�̏�ɒ��n���Ă���
						}
					}
				}
			}

			//������
			if (g_aBlock[nCntBlock].type == BLOCKTYPE_004)
			{
				//�w�i�������Ȃ��Ƃ�
				if (g_bBgXB == false)
				{
					// �u���b�N�Ƃ̓����蔻��
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// �E
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// ��
					{
						if (GetKeyboardPress(DIK_W) && nFade == FADE_NONE)
						{
							//��������
							SetGameState(GAMESTATE_NEXTSTAGE);
						}
					}
				}

				//�w�i�������Ƃ�
				if (g_bBgXB == true)
				{
					// �u���b�N�Ƃ̓����蔻��
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// �E
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// ��
					{
						if (GetKeyboardPress(DIK_W) && nFade == FADE_NONE)
						{
							//��������
							SetGameState(GAMESTATE_NEXTSTAGE);
						}
					}
				}
			}

			//���
			if (g_aBlock[nCntBlock].type == BLOCKTYPE_006)
			{
				//�w�i�������Ȃ��Ƃ�
				if (g_bBgXB == false)
				{
					// �u���b�N�Ƃ̓����蔻��
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))-50) &&	// ��
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// �E
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// ��
					{
						if (pPlayer->pos.y + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{	// �u���b�N�ɏォ�瓖��������
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeY / 2);		// ���̏�ɏ��
							bLand = true;		// �u���b�N�̏�ɒ��n���Ă���
						}
						else if (pPlayer->pos.x + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->posOld.x + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))
						{	// �u���b�N�ɉE���瓖��������
							pPlayer->pos.x = g_aBlock[nCntBlock].pos.x - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeX / 2);
						}
						else if (pPlayer->pos.x - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->posOld.x - (PLAYER_SIZE / 2) >= g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))
						{	// �u���b�N�ɍ����瓖��������
							pPlayer->pos.x = g_aBlock[nCntBlock].pos.x + (PLAYER_SIZE / 2) + (g_aBlock[nCntBlock].nSizeX / 2);
						}
						else if (pPlayer->pos.y - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2)-50 &&	//���̉��͍U������
							pPlayer->pos.y - (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{
							//�v���C���[���m�[�}����ԂȂ�
							if (pPlayer->state == PLAYERSTATE_NORMAL)
							{
								//���@�Ƀ_���[�W
								HitPlayer(15);
							}
						}
					}
				}
			}

			//���̒��ւ̃��[�v�]�[��
			if (g_aBlock[nCntBlock].type == BLOCKTYPE_007)
			{
				//�w�i�������Ȃ��Ƃ�
				if (g_bBgXB == false)
				{
					// �u���b�N�Ƃ̓����蔻��
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// �E
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// ��
					{
						if (GetKeyboardPress(DIK_W) && nFade == FADE_NONE)
						{
							//��������
							SetGameState2(GAMESTATE2_NEXTSTAGE);
						}
					}
				}

				//�w�i�������Ƃ�
				if (g_bBgXB == true)
				{
					// �u���b�N�Ƃ̓����蔻��
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// �E
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// ��
					{
						if (GetKeyboardPress(DIK_W) && nFade == FADE_NONE)
						{
							//��������
							SetGameState2(GAMESTATE2_NEXTSTAGE);
						}
					}
				}
			}

			//��
			if (g_aBlock[nCntBlock].type == BLOCKTYPE_011)
			{
				//�w�i�������Ȃ��Ƃ�
				if (g_bBgXB == false)
				{
					// �u���b�N�Ƃ̓����蔻��
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2)) + 70) &&	// ��
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// �E
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// ��
					{
						//����
						pPlayer->move.y = 0.0f;
						pPlayer->move.y -= 4.0f;
						pPlayer->bJump = false;
					}
				}

				//�w�i�������Ƃ�
				if (g_bBgXB == true)
				{
					// �u���b�N�Ƃ̓����蔻��
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))+70) &&	// ��
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// �E
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// ��
					{
						//����
						pPlayer->move.y = 0.0f;
						pPlayer->move.y -= 4.0f;
						pPlayer->bJump = false;
					}
				}
			}

			//�g�Q�u���b�N
			if (g_aBlock[nCntBlock].type == BLOCKTYPE_015)
			{
				//�w�i�������Ȃ��Ƃ�
				if (g_bBgXB == false)
				{
					// �u���b�N�Ƃ̓����蔻��
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// �E
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// ��
					{
						if (pPlayer->pos.y + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{	// �u���b�N�ɏォ�瓖��������
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeY / 2);		// �u���b�N�̏�ɏ��
							//�v���C���[���m�[�}����ԂȂ�
							if (pPlayer->state == PLAYERSTATE_NORMAL)
							{
								//���@�Ƀ_���[�W
								HitPlayer(15);
							}
							bLand = true;		// �u���b�N�̏�ɒ��n���Ă���
						}
						else if (pPlayer->pos.y - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y - (PLAYER_SIZE / 2) >= g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))
						{	// �u���b�N�ɉ����瓖��������
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y + (PLAYER_SIZE / 2) + (g_aBlock[nCntBlock].nSizeY / 2);		// �v���C���[���u���b�N�ɂ߂荞�܂Ȃ��悤�ɂ���
						}
						else if (pPlayer->pos.x + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->posOld.x + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))
						{	// �u���b�N�ɉE���瓖��������
							pPlayer->pos.x = g_aBlock[nCntBlock].pos.x - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeX / 2);
						}
						else if (pPlayer->pos.x - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->posOld.x - (PLAYER_SIZE / 2) >= g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))
						{	// �u���b�N�ɍ����瓖��������
							pPlayer->pos.x = g_aBlock[nCntBlock].pos.x + (PLAYER_SIZE / 2) + (g_aBlock[nCntBlock].nSizeX / 2);
						}
					}
				}

				//�w�i�������Ƃ�
				if (g_bBgXB == true)
				{
					// �u���b�N�Ƃ̓����蔻��
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// ��
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// �E
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// ��
					{
						if (pPlayer->pos.y + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{	// �u���b�N�ɏォ�瓖��������
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeY / 2);		// �u���b�N�̏�ɏ��
							//�v���C���[���m�[�}����ԂȂ�
							if (pPlayer->state == PLAYERSTATE_NORMAL)
							{
								//���@�Ƀ_���[�W
								HitPlayer(15);
							}
							bLand = true;		// �u���b�N�̏�ɒ��n���Ă���
						}
						else if (pPlayer->pos.y - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y - (PLAYER_SIZE / 2) >= g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))
						{	// �u���b�N�ɉ����瓖��������
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y + (PLAYER_SIZE / 2) + (g_aBlock[nCntBlock].nSizeY / 2);		// �v���C���[���u���b�N�ɂ߂荞�܂Ȃ��悤�ɂ���
						}
						else if (pPlayer->pos.x + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->pos.x + (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))
						{	// �u���b�N�ɉE���瓖��������
							//�������Ȃ獶�ɓ�����
							if (pPlayer->bLeft == true)
							{
								g_bBlockX = false;
							}
							else
							{
								g_bBlockX = true;
							}
						}
						else if (pPlayer->pos.x - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->pos.x - (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))
						{	// �u���b�N�ɍ����瓖��������
							//�E�����Ȃ�E�ɓ�����
							if (pPlayer->bLeft == false)
							{
								g_bBlockX = false;
							}
							else
							{
								g_bBlockX = true;
							}
						}
					}
				}
			}
		}
	}
	return bLand;
}

//=============================================================================
// �u���b�N���̑��M(�u���b�N�ɉ����瓖��������)
//=============================================================================
bool GetBlockX(void)
{
	return g_bBlockX;
}

//=============================================================================
// �u���b�N���̑��M(�u���b�N�����Œ�~�ʒu�ɂ��ǂ蒅�������ǂ���)
//=============================================================================
bool GetBlockY(void)
{
	return g_bBlockY;
}