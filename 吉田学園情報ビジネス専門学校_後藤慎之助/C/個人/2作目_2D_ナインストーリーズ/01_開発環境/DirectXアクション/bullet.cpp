//=============================================================================
//
// �i�C���X�g�[���[�Y�̏��� [bullet.cpp] (�G�̒e)
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "input.h"
#include "block.h"
#include "bg.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureBullet[BULLETTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBullet = NULL;
BULLET g_aBullet[MAX_BULLET];

bool g_bBgXBU;					//�X�e�[�W1�̉����������ǂ���
bool g_bBlockXBU;				//�u���b�N�ɉ����瓖������

MODE g_ModeBullet;				//�Q�[����ʂɑΉ�

int g_nCounterAnimBo;			//�{�X�̒e�p
int g_nPatternAnimBo;			//�{�X�̒e�̃A�j���[�V����

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
		g_aBullet[nCntBullet].nCounterAnim = 0;
		g_aBullet[nCntBullet].nPatternAnim = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	//�O���[�o���ϐ��̏�����
	g_bBgXBU = false;
	g_bBlockXBU = false;
	g_ModeBullet = MODE_GAME;
	g_nCounterAnimBo = 0;
	g_nPatternAnimBo = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bullet000.png", &g_pTextureBullet[0]);	//�K�C�R�c�̒e
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bullet001.png", &g_pTextureBullet[1]);	//�_�[�N�K�C�R�c�̒e
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bullet002.png", &g_pTextureBullet[2]);	//�{�X�̒e

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

													//�e�̐���
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
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
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// �o���b�g�I������
//=============================================================================
void UninitBullet(void)
{
	//�e�N�X�`���̊J��
	for (int nCntBullet = 0; nCntBullet < BULLETTYPE_MAX; nCntBullet++)
	{
		if (g_pTextureBullet[nCntBullet] != NULL)
		{
			g_pTextureBullet[nCntBullet]->Release();
			g_pTextureBullet[nCntBullet] = NULL;
		}
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
	PLAYER*pPlayer;

	//�Q�[���̃X�e�[�W���擾
	g_ModeBullet = GetMode();

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//�A�j���[�V����
			g_aBullet[nCntBullet].nCounterAnim++;	//�J�E���^���Z
			if (g_aBullet[nCntBullet].nCounterAnim == 1)//����
			{
				g_aBullet[nCntBullet].nCounterAnim = 0;
				g_aBullet[nCntBullet].nPatternAnim++;
				if (g_aBullet[nCntBullet].nPatternAnim == 6)//����
				{
					g_aBullet[nCntBullet].nPatternAnim = 0;
				}
			}

			//�G���擾
			pEnemy = GetEnemy();

			//���@���擾
			pPlayer = GetPlayer();

			//�w�i�̉����������ǂ����擾
			g_bBgXBU = GetBg();
			//�u���b�N�ɉ����瓖���������ǂ����擾
			g_bBlockXBU = GetBlockX();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				//�G������Ă��Ă��A���@������Γ�����
				if (pPlayer->bDisp == true)
				{
					//���@�̓����蔻��
					if (g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + (PLAYER_SIZE / 2) - 10 &&
						g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - (PLAYER_SIZE / 2) + 10 &&
						g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + (PLAYER_SIZE / 2) - 10 &&
						g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - (PLAYER_SIZE / 2) + 10)
					{
						if (pPlayer->state == PLAYERSTATE_NORMAL && g_aBullet[nCntBullet].type == BULLETTYPE_OCT)//�K�C�R�c�̒e
						{
							//���@�Ƀ_���[�W
							HitPlayer(20);
						}

						if (pPlayer->state == PLAYERSTATE_NORMAL && g_aBullet[nCntBullet].type == BULLETTYPE_GHO)//�_�[�N�K�C�R�c�̒e
						{
							//���@�Ƀ_���[�W
							HitPlayer(25);
						}

						if (pPlayer->state == PLAYERSTATE_NORMAL && g_aBullet[nCntBullet].type == BULLETTYPE_BOSS)//�{�X�̒e
						{
							//��ɐ�����΂�(��苗����ɐ�����΂��ꂽ��A���ꉉ�o�Q�[���I�[�o�[)
							pPlayer->move.y = 0.0f;
							pPlayer->move.y -= 35.5f;
							//��΂��ꂽ�ہA������x�����󒆃W�����v���ł���悤�ɂ���
							pPlayer->bJump = true;
							pPlayer->bSecondJump = true;
						}

						//�����������_�Œe��HP��0�ɂ���
						g_aBullet[nCntBullet].nLife = 0;
					}
				}
			}

			//�_�[�N�K�C�R�c�̒e�͓r���ŒǏ]
			if (g_aBullet[nCntBullet].type == BULLETTYPE_GHO)
			{
				//�r���Ŏ��@�̈ʒu���擾���A������ɔ��ł���
				if (g_aBullet[nCntBullet].nLife == 120)
				{
					//���@���擾
					pPlayer = GetPlayer();

					float fHposx = pPlayer->pos.x, fHposy = pPlayer->pos.y;							//�ڕW(�v���C���[)�̍��W
					float fBposx = g_aBullet[nCntBullet].pos.x, fBposy = g_aBullet[nCntBullet].pos.y;	//�G�̍��W
					float fAngle;																	//�p�x

					fAngle = atan2f((fBposx - fHposx), (fBposy - fHposy));	//�p�x�����߂�

					g_aBullet[nCntBullet].move.x = -sin(fAngle)*11.0f;
					g_aBullet[nCntBullet].move.y = -cos(fAngle)*11.0f;
				}
			}

			//�e�̈ړ����x(move�����{���ƁA�S�̓I�ɑ����A��Փx�������������߁A0.5�������Ă���)
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move*0.5;

			//�Q�[��1�ŁA�v���C���[�̑���ɑΉ������ړ�
			if (g_ModeBullet == MODE_GAME && pPlayer->bDisp == true && g_bBgXBU == true && g_bBlockXBU == false)
			{
				if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
				{
					g_aBullet[nCntBullet].pos.x -= VALUE_MOVE;
				}

				if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
				{
					g_aBullet[nCntBullet].pos.x += VALUE_MOVE;
				}
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y - (g_aBullet[nCntBullet].nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y - (g_aBullet[nCntBullet].nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y + (g_aBullet[nCntBullet].nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y + (g_aBullet[nCntBullet].nSize / 2), 0.0f);

			//�e�̎˒������߂�
			g_aBullet[nCntBullet].nLife--;

			//��
			if (g_aBullet[nCntBullet].type == BULLETTYPE_OCT || g_aBullet[nCntBullet].type == BULLETTYPE_GHO)
			{
				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(g_aBullet[nCntBullet].nPatternAnim*0.166f + 0.166f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aBullet[nCntBullet].nPatternAnim*0.166f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aBullet[nCntBullet].nPatternAnim*0.166f + 0.166f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aBullet[nCntBullet].nPatternAnim*0.166f, 1.0f);

				//���C�t���Ȃ��Ȃ����疢�g�p��
				if (g_aBullet[nCntBullet].nLife <= 0)
				{
					g_aBullet[nCntBullet].bUse = false;	//�g�p���Ȃ���Ԃɂ���
				}
			}

			//�{�X�̒e
			if (g_aBullet[nCntBullet].type == BULLETTYPE_BOSS)
			{
				//�A�j���[�V����
				g_nCounterAnimBo++;	//�J�E���^���Z
				if (g_nCounterAnimBo == 3)//����
				{
					g_nCounterAnimBo = 0;
					g_nPatternAnimBo++;
					if (g_nPatternAnimBo == 2)//����
					{
						g_nPatternAnimBo = 0;
					}
				}

				//�e�N�X�`�����W�̐ݒ�
				if (g_nPatternAnimBo == 0)
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}
				else if (g_nPatternAnimBo == 1)
				{
					pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				}

				//���ԂŃg���l�[�h���I���
				if (g_aBullet[nCntBullet].nLife <= 0)
				{
					g_aBullet[nCntBullet].col.a -= 0.05;

					//�����x��0.1����������疢�g�p��
					if (g_aBullet[nCntBullet].col.a <= 0.1)
					{
						g_aBullet[nCntBullet].bUse = false;
					}
				}
				//���_�J���[�̍X�V
				pVtx[0].col = D3DXCOLOR(g_aBullet[nCntBullet].col.r,g_aBullet[nCntBullet].col.g, g_aBullet[nCntBullet].col.b, g_aBullet[nCntBullet].col.a);
				pVtx[1].col = D3DXCOLOR(g_aBullet[nCntBullet].col.r,g_aBullet[nCntBullet].col.g, g_aBullet[nCntBullet].col.b, g_aBullet[nCntBullet].col.a);
				pVtx[2].col = D3DXCOLOR(g_aBullet[nCntBullet].col.r,g_aBullet[nCntBullet].col.g, g_aBullet[nCntBullet].col.b, g_aBullet[nCntBullet].col.a);
				pVtx[3].col = D3DXCOLOR(g_aBullet[nCntBullet].col.r,g_aBullet[nCntBullet].col.g, g_aBullet[nCntBullet].col.b, g_aBullet[nCntBullet].col.a);
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
			pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].type]);
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
			g_aBullet[nCntBullet].col.r = 1.0f;
			g_aBullet[nCntBullet].col.g = 1.0f;
			g_aBullet[nCntBullet].col.b = 1.0f;
			g_aBullet[nCntBullet].col.a = 1.0f;

			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].nLife = nLife;
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