//=============================================================================
//
// �i�C���X�g�[���[�Y [floor.cpp]�̏��� (�Q�[����ʂ̏��̃e�N�X�`��)
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "player.h"
#include "input.h"
#include "enemy.h"
#include "block.h"
#include "floor.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonFloor[3] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonFloor = NULL;
D3DXVECTOR3 g_moveFloor;		//�w�i�̈ړ��ʁi�X�e�[�W�S�̂̒��������߂�j
int g_nCounterAnimFloorLeft;	//���̃A�j���[�V�����J�E���^�[
int g_nCounterAnimFloorRight;	//�E�̃A�j���[�V�����J�E���^�[
int g_nPatternAnimFloor;		//�A�j���[�V�����p�^�[��
int g_nShakeMoveFloor;			//�w�i���h���Ƃ��̐��l(���ށj
int g_nShakeMoveFloor2;			//�w�i���h���Ƃ��̐��l(�߂�)
bool g_bMoveFloor;				//�w�i�������Ă���
bool g_bShakeFloor;				//�w�i���h���
bool g_bBlockXF;				//�u���b�N�ɉ����瓖��������
MODE g_ModeFl;					//�Q�[����ʂɑΉ�������

//=============================================================================
//������������
//=============================================================================
void InitFloor(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/floor000.png", &g_pTexturePolygonFloor[0]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/floor001.png", &g_pTexturePolygonFloor[1]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/floor002.png", &g_pTexturePolygonFloor[2]);

	//�O���[�o���ϐ��̏�����
	g_moveFloor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nCounterAnimFloorLeft = 0;
	g_nCounterAnimFloorRight = 0;
	g_nPatternAnimFloor = 0;
	g_nShakeMoveFloor = 0;
	g_nShakeMoveFloor2 = 0;
	g_bMoveFloor = false;	//�ŏ��́A�w�i�͓����Ȃ�
	g_bShakeFloor = false;	//�ŏ��́A�w�i�͗h��Ȃ�
	g_bBlockXF = false;
	g_ModeFl = MODE_GAME;

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonFloor, NULL);
	VERTEX_2D *pVtxFloor;	//���̒��_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPolygonFloor->Lock(0, 0, (void**)&pVtxFloor, 0);	//���̏����͕ς��Ȃ�����

	//���_���W�̐ݒ�i�E���Őݒ肷��j
	pVtxFloor[0].pos = D3DXVECTOR3(0.0f, 580.0f + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
	pVtxFloor[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 580.0f + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
	pVtxFloor[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
	pVtxFloor[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);

	//rhw�̐ݒ�i�l��1.0�Őݒ肷��j
	pVtxFloor[0].rhw = 1.0f;
	pVtxFloor[1].rhw = 1.0f;
	pVtxFloor[2].rhw = 1.0f;
	pVtxFloor[3].rhw = 1.0f;

	//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
	pVtxFloor[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxFloor[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxFloor[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxFloor[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W���X�V
	pVtxFloor[0].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f, 0.0f);
	pVtxFloor[1].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f + 0.9925f, 0.0f);
	pVtxFloor[2].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f, 1.0f);
	pVtxFloor[3].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f + 0.9925f, 1.0f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygonFloor->Unlock();
}

//=============================================================================
// ���I������
//=============================================================================
void UninitFloor(void)
{

	//�e�N�X�`���̊J��
	for (int nCntBg = 0; nCntBg < 3; nCntBg++)
	{
		if (g_pTexturePolygonFloor[nCntBg] != NULL)
		{
			g_pTexturePolygonFloor[nCntBg]->Release();
			g_pTexturePolygonFloor[nCntBg] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPolygonFloor != NULL)
	{
		g_pVtxBuffPolygonFloor->Release();
		g_pVtxBuffPolygonFloor = NULL;
	}
}

//=============================================================================
// ���X�V����
//=============================================================================
void UpdateFloor(void)
{
	VERTEX_2D *pVtxFloor;	//���_���ւ̃|�C���^
	PLAYER*pPlayer = GetPlayer();		//�v���C���[�̏󋵂��擾
	g_ModeFl = GetMode();

	//�v���C���[���U�����󂯂����ǂ����擾
	g_bShakeFloor = HitPlayer2();

	g_bBlockXF = GetBlockX();

	//�v���C���[���U�����󂯂Ă��Ȃ��Ȃ�A�G���U�����󂯂Ă��邩�ǂ������擾
	if (g_bShakeFloor == false)
	{
		g_bShakeFloor = HitEnemy2();
	}

	g_pVtxBuffPolygonFloor->Lock(0, 0, (void**)&pVtxFloor, 0);	//���̏����͕ς��Ȃ�����

	//�Q�[��1(��̕���)
	if (g_ModeFl == MODE_GAME)
	{
		//�v���C���[���E�ɐi�ނɂ�āA�E�ɂ�����̂��o�Ă���
		if (pPlayer->bDisp == true)
		{

			//A��D���������͖���
			if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_D))
			{

			}

			//�E�ɐi��
			else if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D) && g_bBlockXF == false)
			{
				if (pPlayer->pos.x == SCREEN_WIDTH / 2)	//�v���C���[�������ɂ���Ƃ������w�i�𗬂�
				{
					g_moveFloor.x++;
					g_nCounterAnimFloorRight++;					//�J�E���^���Z
					if (g_nCounterAnimFloorRight == 2)			//����
					{
						g_nCounterAnimFloorRight = 0;
						g_nPatternAnimFloor++;
						if (g_nPatternAnimFloor == STAGE1_X)	//����
						{
							g_nPatternAnimFloor = 0;
						}
					}
				}

				//��ԉE�ȊO�́A�w�i�𗬂�
				if (g_moveFloor.x > STAGE1_X)
				{
					g_moveFloor.x = STAGE1_X;
				}
			}

			//���ɐi��
			else if (pPlayer->nAttackTime == 0 && g_bBlockXF == false && GetKeyboardPress(DIK_A))
			{
				if (pPlayer->pos.x == SCREEN_WIDTH / 2)	//�v���C���[�������ɂ���Ƃ������w�i�𗬂�
				{
					g_moveFloor.x--;
					g_nCounterAnimFloorLeft++;					//�J�E���^���Z
					if (g_nCounterAnimFloorLeft == 2)			//����
					{
						g_nCounterAnimFloorLeft = 0;
						g_nPatternAnimFloor--;
						if (g_nPatternAnimFloor == -STAGE1_X)	//����
						{
							g_nPatternAnimFloor = 0;
						}
					}
				}

				//��ԍ��ȊO�́A�w�i�𗬂�
				if (g_moveFloor.x < 0)
				{
					g_moveFloor.x = 0;
				}
			}

		}

		//��ʂ̒[�ł͔w�i�͓����Ă��Ȃ�
		if (g_moveFloor.x == 0)
		{
			g_bMoveFloor = false;
		}
		else if (g_moveFloor.x == STAGE1_X)
		{
			g_bMoveFloor = false;
		}
		else
		{
			g_bMoveFloor = true;
		}

		//�e�N�X�`�����W���X�V
		pVtxFloor[0].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f, 0.0f);
		pVtxFloor[1].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f + 0.9925f, 0.0f);
		pVtxFloor[2].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f, 1.0f);
		pVtxFloor[3].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f + 0.9925f, 1.0f);
	}


	//�Q�[��2(������)
	if (g_ModeFl == MODE_GAME2)
	{
		//�e�N�X�`�����W���X�V
		pVtxFloor[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtxFloor[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtxFloor[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtxFloor[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//�Q�[��3(���̒�)
	if (g_ModeFl == MODE_GAME3)
	{
		//�e�N�X�`�����W���X�V
		pVtxFloor[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtxFloor[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtxFloor[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtxFloor[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//�w�i��h�炷
	if (g_bShakeFloor == true)
	{
		g_nShakeMoveFloor++;	//�J�E���^���Z
		if (g_nShakeMoveFloor >= SHAKE_FLOOR)		//���ސ[��
		{
			g_nShakeMoveFloor = SHAKE_FLOOR;
			g_nShakeMoveFloor2--;
			if (g_nShakeMoveFloor2 <= -SHAKE_FLOOR)	//�߂�[��
			{
				g_nShakeMoveFloor = 0;
				g_nShakeMoveFloor2 = 0;
				g_bShakeFloor = false;
			}
		}
	}
	else
	{
		g_nShakeMoveFloor = 0;
		g_nShakeMoveFloor2 = 0;
	}

	//�Q�[��1�A�Q�[��2
	if (g_ModeFl == MODE_GAME|| g_ModeFl == MODE_GAME2)
	{

		//���_���W�̐ݒ�i�E���Őݒ肷��j
		pVtxFloor[0].pos = D3DXVECTOR3(0.0f, 580.0f + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
		pVtxFloor[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 580.0f + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
		pVtxFloor[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
		pVtxFloor[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
	}

	//�Q�[��3
	if (g_ModeFl == MODE_GAME3)
	{

		//���_���W�̐ݒ�i�E���Őݒ肷��j
		pVtxFloor[0].pos = D3DXVECTOR3(0.0f, 500.0f + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
		pVtxFloor[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 500.0f + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
		pVtxFloor[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
		pVtxFloor[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygonFloor->Unlock();
}

//=============================================================================
// ���`�揈��
//=============================================================================
void DrawFloor(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPolygonFloor, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�Q�[��1
	if (g_ModeFl == MODE_GAME)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonFloor[0]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	//�Q�[��2
	if (g_ModeFl == MODE_GAME2)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonFloor[1]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	//�Q�[��3
	if (g_ModeFl == MODE_GAME3)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonFloor[2]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

}