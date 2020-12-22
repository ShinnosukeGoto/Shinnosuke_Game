//=============================================================================
//
// X-Bow Hunter [hunter.cpp]�̏��� (�n���^�[�����@)
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "hunter.h"
#include "input.h"
#include "arrow.h"
#include "explosion.h"
#include "enemy.h"
#include "boss.h"
#include "bullet.h"
#include "game.h"
#include "wing.h"
#include "sound.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define VALUE_MOVE (5.5f)		//�H���Ȃ��n���^�[�̈ړ���
#define VALUE_JUMP (22.5f)		//�W�����v�̋���
#define VALUE_MOVE_WING (0.95f)	//�H���������n���^�[�̈ړ���
#define ARROW_CD (12)			//���̖��łĂ�܂ł̎���
#define BLUE_ARROW_CD (10)		//���̖��łĂ�܂ł̎���
#define GRAVITY (1.0f)			//�d��

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygon = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygon = NULL;
D3DXVECTOR3 g_movePolygon;//�|���S���̈ړ���
int g_nCounterAnimHunt;		//�A�j���[�V�����J�E���^�[
int g_nPatternAnimHunt;		//�A�j���[�V�����p�^�[��
HUNTER g_Hunter;			//���@�̏��
int g_nArrowCD;				//��̃N�[���^�C��

							//=============================================================================
							//�n���^�[����������
							//=============================================================================
void InitHunter(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_nCounterAnimHunt = 0;
	g_nPatternAnimHunt = 0;

	//�n���^�[�̏�����
	g_Hunter.state = HUNTERSTATE_NORMAL;
	g_Hunter.pos = D3DXVECTOR3(100.0f, 590.0f, 0.0f);
	g_Hunter.nLife = HUNTER_LIFE;
	g_Hunter.nCounterState = 0;
	g_Hunter.bDisp = false;
	g_Hunter.nStock = 2;	//�c�@�̑����̏�����
	g_movePolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nArrowCD = ARROW_CD;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "hunter003.png", &g_pTexturePolygon);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygon, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

														//���_���W�̐ݒ�i�E���Őݒ肷��j
	pVtx[0].pos = D3DXVECTOR3(g_Hunter.pos.x - (HUNTER_SIZE / 2), g_Hunter.pos.y - (HUNTER_SIZE / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Hunter.pos.x + (HUNTER_SIZE / 2), g_Hunter.pos.y - (HUNTER_SIZE / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Hunter.pos.x - (HUNTER_SIZE / 2), g_Hunter.pos.y + (HUNTER_SIZE / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Hunter.pos.x + (HUNTER_SIZE / 2), g_Hunter.pos.y + (HUNTER_SIZE / 2), 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// �n���^�[�I������
//=============================================================================
void UninitHunter(void)
{
	//�e�N�X�`���̊J��
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//=============================================================================
// �n���^�[�X�V����
//=============================================================================
void UpdateHunter(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	ENEMY*pEnemy;
	int nCntEnemy;
	BOSS*pBoss;
	int nCntBoss;
	WING*pWing;

	int nDirect = 0;		//���͂��ꂽ�����𐔂���
	int nChange = 0;		//������ς���
	int nAttack = 0;		//�U���{�^���𐔂���
	int nOblique = 0;		//�΂߂�����

							//���_�o�b�t�@�����b�N
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//�v���C���[�̏��
	switch (g_Hunter.state)
	{
	case HUNTERSTATE_NORMAL:	//�ʏ�̏��
								//�s���`��Ԃƃ��[�v
		if (g_Hunter.nLife < HUNTER_LIFE / 4)
		{
			g_Hunter.state = HUNTERSTATE_PINCH;

			//�ʏ�`��ɖ߂�����
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		break;

	case HUNTERSTATE_PINCH:	//�s���`���
		g_Hunter.nCounterState--;
		if (g_Hunter.nCounterState <= 0)
		{
			g_Hunter.state = HUNTERSTATE_NORMAL;

			//�s���`�`��ɂ��鏈��
			pVtx[0].col = D3DCOLOR_RGBA(246, 120, 59, 255);
			pVtx[1].col = D3DCOLOR_RGBA(246, 120, 59, 255);
			pVtx[2].col = D3DCOLOR_RGBA(246, 120, 59, 255);
			pVtx[3].col = D3DCOLOR_RGBA(246, 120, 59, 255);
		}
		break;

	case HUNTERSTATE_DAMAGE:	//�_���[�W���
		g_Hunter.nCounterState--;
		if (g_Hunter.nCounterState <= 0)
		{
			g_Hunter.state = HUNTERSTATE_NORMAL;

			//�ʏ�`��ɖ߂�����
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		break;

	case HUNTERSTATE_APPEAR:	//�o���܂ł̏��
		g_Hunter.nCounterState--;
		g_Hunter.bDisp = false;	//�o���܂ŁA�\�������Ȃ�
		if (g_Hunter.nCounterState <= 0)
		{
			PlaySound(SOUND_LABEL_SE_ITEM);	//������

			g_Hunter.state = HUNTERSTATE_NORMAL;

			//�ʏ�`��ɖ߂�����
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			g_Hunter.bDisp = true;	//�\��������
		}
		break;
	}

	//�n���^�[���\������Ă���Ȃ�X�V
	if (g_Hunter.bDisp == true)
	{
		//�A�j���[�V����
		g_nCounterAnimHunt++;	//�J�E���^���Z
		if (g_nCounterAnimHunt == 10)	//fps
		{
			g_nCounterAnimHunt = 0;
			g_nPatternAnimHunt++;
			if (g_nPatternAnimHunt == 2)//����
			{
				g_nPatternAnimHunt = 0;
			}
		}

		//�G���擾
		pEnemy = GetEnemy();
		pBoss = GetBoss();
		//�E�B���O�̎g�p�󋵂��擾
		pWing = GetWing();

		//�H���g�p���Ă��Ȃ��Ƃ��̏���
		if (pWing->bUse == false)
		{
			//A�L�[�������ꂽ�i���ړ��j
			if (GetKeyboardPress(DIK_A))
			{
				g_Hunter.pos.x -= VALUE_MOVE;
				nDirect--;
			}

			//D�L�[�������ꂽ�i�E�ړ��j
			if (GetKeyboardPress(DIK_D))
			{
				g_Hunter.pos.x += VALUE_MOVE;
				nDirect++;
			}

			//A��D�L�[�ŁA�������ɌŒ�
			if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_D))
			{
				nDirect--;
			}

			if (GetKeyboardPress(DIK_SPACE))
			{
				nAttack++;
				if (g_nArrowCD % ARROW_CD == 0)
				{
					SetArrow(D3DXVECTOR3(g_Hunter.pos.x + 20.5f, g_Hunter.pos.y + 5.5, 0.0f), D3DXVECTOR3(7.9, -7.9, 0), 66, D3DXCOLOR(255, 255, 255, 255), ARROWTYPE_BEND, 50);
					PlaySound(SOUND_LABEL_SE_SHOT);	//�V���b�g��
				}
			}
			g_nArrowCD++;

			//�H���g�p���Ă��Ȃ��Ƃ��ɂ�����v���C���[�ւ̏d��
			g_movePolygon.y += GRAVITY;	//�d��
			g_Hunter.pos.y += g_movePolygon.y;	//�ʒu�X�V
			g_movePolygon.y += (0.0 - g_movePolygon.x)*RATE_MOVE;
		}

		//�H���g�p���Ă���Ƃ��̏���
		if (pWing->bUse == true)
		{
			//A�L�[�������ꂽ(���ړ�)
			if (GetKeyboardPress(DIK_A))
			{
				nDirect++;
				if (GetKeyboardPress(DIK_W))//A��W�������ɉ����ꂽ
				{
					g_movePolygon.x -= sinf(D3DX_PI / 4) * VALUE_MOVE_WING;
					g_movePolygon.y -= cosf(D3DX_PI / 4) * VALUE_MOVE_WING;
				}
				else if (GetKeyboardPress(DIK_S))//A��S�������ɉ����ꂽ
				{
					g_movePolygon.x -= (D3DX_PI / 4) * VALUE_MOVE_WING;
					g_movePolygon.y += (D3DX_PI / 4) * VALUE_MOVE_WING;
				}
				else				//������������
				{
					g_movePolygon.x -= VALUE_MOVE_WING;
				}

			}

			//D�L�[�������ꂽ(�E�ړ�)
			else if (GetKeyboardPress(DIK_D))
			{
				nDirect++;
				if (GetKeyboardPress(DIK_W))//D��W�������ɉ����ꂽ
				{
					g_movePolygon.x += sinf(D3DX_PI / 4) * VALUE_MOVE_WING;
					g_movePolygon.y -= cosf(D3DX_PI / 4) * VALUE_MOVE_WING;
				}
				else if (GetKeyboardPress(DIK_S))//D��S�������ɉ����ꂽ
				{
					g_movePolygon.x += sinf(D3DX_PI / 4) * VALUE_MOVE_WING;
					g_movePolygon.y += cosf(D3DX_PI / 4) * VALUE_MOVE_WING;
				}
				else				//�E����������
				{
					g_movePolygon.x += VALUE_MOVE_WING;
				}
			}

			//W�L�[�������ꂽ(��ړ�)
			else if (GetKeyboardPress(DIK_W))
			{
				g_movePolygon.y -= VALUE_MOVE_WING;
				nDirect++;
			}

			//S�L�[�������ꂽ(���ړ�)
			else if (GetKeyboardPress(DIK_S))
			{
				g_movePolygon.y += VALUE_MOVE_WING;
				nDirect++;
			}

			//�X�y�[�X�L�[�������ꂽ(�H������Ƃ��͍���)
			if (GetKeyboardPress(DIK_SPACE))
			{
				if (g_nArrowCD % BLUE_ARROW_CD == 0)
				{
					SetArrow(D3DXVECTOR3(g_Hunter.pos.x + 20.5f, g_Hunter.pos.y + 5.5, 0.0f), D3DXVECTOR3(14, 0, 0), 33, D3DXCOLOR(255, 255, 255, 255), ARROWTYPE_STRAIGHT, 55);
					PlaySound(SOUND_LABEL_SE_SHOT);	//�V���b�g��
				}
			}
			g_nArrowCD++;

			//�ʒu�X�V
			g_Hunter.pos.x += g_movePolygon.x;
			g_Hunter.pos.y += g_movePolygon.y;

			//����
			g_movePolygon.x += (0.0 - g_movePolygon.x)*RATE_MOVE;
			g_movePolygon.y += (0.0 - g_movePolygon.y)*RATE_MOVE;
		}

		//���̕�
		if (g_Hunter.pos.x < HUNTER_SIZE - (HUNTER_SIZE / 2))
		{
			g_Hunter.pos.x = HUNTER_SIZE - (HUNTER_SIZE / 2);
		}

		//�E�̕�
		if (g_Hunter.pos.x > SCREEN_WIDTH - HUNTER_SIZE + (HUNTER_SIZE / 2))
		{
			g_Hunter.pos.x = SCREEN_WIDTH - HUNTER_SIZE + (HUNTER_SIZE / 2);
		}

		//��̕�
		if (g_Hunter.pos.y < HUNTER_SIZE - (HUNTER_SIZE / 2) + 146)
		{
			g_Hunter.pos.y = HUNTER_SIZE - (HUNTER_SIZE / 2) + 146;
		}

		//�n�ʂɂ���Ƃ�
		if (g_Hunter.pos.y > SCREEN_HEIGHT - HUNTER_SIZE + (HUNTER_SIZE / 2) - 80)
		{
			g_Hunter.pos.y = SCREEN_HEIGHT - HUNTER_SIZE + (HUNTER_SIZE / 2) - 80;

			//���A�H���g�p���Ă��Ȃ��Ƃ��ɃW�����v���ł���
			if (pWing->bUse == false)
			{
				g_movePolygon.y = 0;	//�n�ʂɂ���Ƃ��͏d�͂�^���Ȃ�

				//�W�����v����
				if (GetKeyboardTrigger(DIK_W))
				{
					g_movePolygon.y -= VALUE_JUMP;	//�W�����v
					PlaySound(SOUND_LABEL_SE_JUMP);	//�W�����v��
				}
			}
		}

		//�������ɕς���
		if (nDirect < 0)
		{
			nDirect = 0;
			nChange = 1;
		}

		//�E�����ɕς���
		if (nDirect > 0)
		{
			nDirect = 0;
			nChange = 0;
		}

		//�E�΂ߑO������
		if (nAttack > 0)
		{
			nAttack = 0;
			nOblique = 1;
		}

		for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{
				//���@�����X�L�[�g�ɓ����������̓����蔻��
				if (pEnemy->type == ENEMYTYPE_000)
				{
					if (g_Hunter.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MOS / 2) &&
						g_Hunter.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MOS / 2) &&
						g_Hunter.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MOS / 2) &&
						g_Hunter.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MOS / 2))
					{
						//���@�Ƀ_���[�W
						HitHunter(1000);
					}
				}

				//���@�����^���o�[�h�ɓ����������̓����蔻��
				if (pEnemy->type == ENEMYTYPE_001)
				{
					if (g_Hunter.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET / 2) &&
						g_Hunter.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET / 2) &&
						g_Hunter.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET / 2) &&
						g_Hunter.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET / 2))
					{
						//���@�Ƀ_���[�W
						HitHunter(1000);
					}
				}

				//���@�����^���o�[�h����ɓ����������̓����蔻��
				if (pEnemy->type == ENEMYTYPE_002)
				{
					if (g_Hunter.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET�U / 2) &&
						g_Hunter.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET�U / 2) &&
						g_Hunter.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET�U / 2) &&
						g_Hunter.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET�U / 2))
					{
						//���@�Ƀ_���[�W
						HitHunter(1000);
					}
				}

				//���@���I�N�g�p�X�ɓ����������̓����蔻��
				if (pEnemy->type == ENEMYTYPE_003)
				{
					if (g_Hunter.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_OCT / 2) &&
						g_Hunter.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_OCT / 2) &&
						g_Hunter.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_OCT / 2) &&
						g_Hunter.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_OCT / 2))
					{
						//���@�Ƀ_���[�W
						HitHunter(1000);
					}
				}

				//���@���S�[�X�g�ɓ����������̓����蔻��
				if (pEnemy->type == ENEMYTYPE_004)
				{
					if (g_Hunter.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GHO / 2) &&
						g_Hunter.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GHO / 2) &&
						g_Hunter.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GHO / 2) &&
						g_Hunter.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GHO / 2))
					{
						//���@�Ƀ_���[�W
						HitHunter(1000);
					}
				}

				//���@���O�\�N���V�ɓ����������̓����蔻��
				if (pEnemy->type == ENEMYTYPE_004)
				{
					if (g_Hunter.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GSO / 2) &&
						g_Hunter.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GSO / 2) &&
						g_Hunter.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GSO / 2) &&
						g_Hunter.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GSO / 2))
					{
						//���@�Ƀ_���[�W
						HitHunter(10);
					}
				}
			}
		}

		for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pBoss++)
		{
			if (pBoss->bUse == true)
			{
				//���@���{�X�ɓ����������̓����蔻��
				if (g_Hunter.pos.x <= pBoss->pos.x + (BOSS_SIZE / 2) - 50 &&
					g_Hunter.pos.x >= pBoss->pos.x - (BOSS_SIZE / 2) + 50 &&
					g_Hunter.pos.y <= pBoss->pos.y + (BOSS_SIZE / 2) - 50 &&
					g_Hunter.pos.y >= pBoss->pos.y - (BOSS_SIZE / 2) + 50)
				{
					//���@�Ƀ_���[�W
					HitHunter(1000);
				}
			}
		}

		//�e�N�X�`�����W���X�V
		pVtx[0 + nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.33f, 0.0f);
		pVtx[1 - nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.33f + 0.33f, 0.0f);
		pVtx[2 + nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.33f, 1.0f);
		pVtx[3 - nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.33f + 0.33f, 1.0f);

		//�e�N�X�`���̌������E�΂ߑO�ɍX�V
		if (nOblique == 1)
		{
			nOblique = 0;
			pVtx[0].tex = D3DXVECTOR2(0.67f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.67f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Hunter.pos.x - (HUNTER_SIZE / 2), g_Hunter.pos.y - (HUNTER_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Hunter.pos.x + (HUNTER_SIZE / 2), g_Hunter.pos.y - (HUNTER_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Hunter.pos.x - (HUNTER_SIZE / 2), g_Hunter.pos.y + (HUNTER_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Hunter.pos.x + (HUNTER_SIZE / 2), g_Hunter.pos.y + (HUNTER_SIZE / 2), 0.0f);
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// �n���^�[�`�揈��
//=============================================================================
void DrawHunter(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�n���^�[���\������Ă���Ȃ�`��
	if (g_Hunter.bDisp == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygon);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// �n���^�[�̐ݒ菈��
//=============================================================================
void SetHunter(D3DXVECTOR3 pos)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//�n���^�[���\������Ă��Ȃ����A�X�g�b�N�������Ԃ̂Ƃ��ɐݒ�\
	if (g_Hunter.bDisp == false && g_Hunter.nStock >-1)
	{
		g_Hunter.bDisp = true;
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Hunter.pos.x - (HUNTER_SIZE / 2), g_Hunter.pos.y - (HUNTER_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Hunter.pos.x + (HUNTER_SIZE / 2), g_Hunter.pos.y - (HUNTER_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Hunter.pos.x - (HUNTER_SIZE / 2), g_Hunter.pos.y + (HUNTER_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Hunter.pos.x + (HUNTER_SIZE / 2), g_Hunter.pos.y + (HUNTER_SIZE / 2), 0.0f);

		//�ʒu��ݒ�
		g_Hunter.pos = pos;
		g_movePolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�n���^�[�̗̑�
		g_Hunter.nLife = HUNTER_LIFE;
	}

	//�H��������Ƃ��̃J���[��ʏ�̐F�ɖ߂�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// �n���^�[�̎擾
//=============================================================================
HUNTER*GetHunter(void)
{
	return &g_Hunter;
}

//=============================================================================
// �n���^�[�̃_���[�W�ݒ�
//=============================================================================
void HitHunter(int nDamage)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	PlaySound(SOUND_LABEL_SE_EXPLOSION);	//������

											//���_�o�b�t�@�����b�N
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	g_Hunter.nLife -= nDamage;
	if (g_Hunter.nLife <= 0)
	{
		//��������
		SetExplosion(g_Hunter.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), HUNTER_SIZE);

		//�n���^�[�𖢎g�p��
		g_Hunter.bDisp = false;

		//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//���̎��@��ݒu
		if (g_Hunter.nStock > -1)
		{
			SetHunter(D3DXVECTOR3(5, 590, 0.0f));
			g_Hunter.state = HUNTERSTATE_APPEAR;				//�o����Ԃɂ���
			g_Hunter.nCounterState = COUNT_WAIT_NEXT_HUNTER;	//�o���܂ł̎��Ԃ𐔂���
		}

		//�ݒu��ɁA���@�̑��������炷
		g_Hunter.nStock--;
	}

	else if (g_Hunter.nLife > 0 && g_Hunter.nLife < HUNTER_LIFE / 4)
	{
		g_Hunter.state = HUNTERSTATE_PINCH;
		g_Hunter.nCounterState = 5;
		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
	}

	else
	{
		//�_���[�W��ԂɑJ��
		g_Hunter.state = HUNTERSTATE_DAMAGE;
		g_Hunter.nCounterState = 5;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygon->Unlock();

	//�Q�[���I�[�o�[
	if (g_Hunter.nStock < 0)
	{
		g_Hunter.bDisp = false;
		SetGameState(GAMESTATE_END);
	}
}