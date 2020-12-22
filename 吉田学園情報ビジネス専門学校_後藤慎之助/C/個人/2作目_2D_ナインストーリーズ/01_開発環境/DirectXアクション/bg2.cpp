//=============================================================================
//
// �i�C���X�g�[���[�Y [bg2.cpp]�̏��� (��ƍ��j
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "bg2.h"
#include "input.h"
#include "player.h"
#include "bg.h"
#include "block.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonBg2[2] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonBg2 = NULL;
int g_nCounterAnimBg2Left;		//���̃A�j���[�V�����J�E���^�[
int g_nCounterAnimBg2Right;		//�E�̃A�j���[�V�����J�E���^�[
int g_nCounterAnimBg2;			//�A�j���[�V�����J�E���^�[
int g_nPatternAnimBg2X;			//�A�j���[�V�����p�^�[��X
int g_nPatternAnimBg2Y;			//�A�j���[�V�����p�^�[��Y
int g_nCntTime;		//���Ԃ𐔂���
int g_nCntLight;	//�u�̓_��
bool g_bMoveBg2;				//�w�i�������Ă���
bool g_bBlockXB2;				//�u���b�N�ɉ����瓖��������
bool g_bBlockYB2;				//�u���b�N����~�ʒu�ɂ��ǂ蒅�����Ƃ�
MODE g_ModeBg2;					//�Q�[����ʂɑΉ������w�i

//=============================================================================
//�o�b�N�O���E���h����������
//=============================================================================
void InitBg2(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�O���[�o���ϐ��̏�����
	g_nCounterAnimBg2Left = 0;
	g_nCounterAnimBg2Right = 0;
	g_nCounterAnimBg2 = 0;
	g_nPatternAnimBg2X = 0;
	g_nPatternAnimBg2Y = 0;
	g_nCntTime = 0;
	g_nCntLight = 0;
	g_bMoveBg2 = false;
	g_bBlockXB2 = false;
	g_bBlockYB2 = false;
	g_ModeBg2 = MODE_GAME;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bg002.png", &g_pTexturePolygonBg2[0]);	//��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bg003.png", &g_pTexturePolygonBg2[1]);	//��

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*2, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonBg2, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPolygonBg2->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

														//���_���W�̐ݒ�i�E���Őݒ肷��j
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�i�l��1.0�Őݒ肷��j
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 205);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 205);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 205);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 205);
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W���X�V
	pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f, g_nPatternAnimBg2Y *0.001f);
	pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f + 0.997f, g_nPatternAnimBg2Y *0.001f);
	pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f, g_nPatternAnimBg2Y *0.001f - 0.999f);
	pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f + 0.997f, g_nPatternAnimBg2Y *0.001f - 0.999f);

	//�e�N�X�`�����W���X�V
	pVtx[4].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2Y *0.01f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2Y *0.01f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2Y *0.01f - 0.99f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2Y *0.01f - 0.99f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygonBg2->Unlock();
}

//=============================================================================
// �o�b�N�O���E���h�I������
//=============================================================================
void UninitBg2(void)
{
	//�e�N�X�`���̊J��
	for (int nCntBg = 0; nCntBg < 2; nCntBg++)	//�g���Ă���摜��
	{
		if (g_pTexturePolygonBg2[nCntBg] != NULL)
		{
			g_pTexturePolygonBg2[nCntBg]->Release();
			g_pTexturePolygonBg2[nCntBg] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPolygonBg2 != NULL)
	{
		g_pVtxBuffPolygonBg2->Release();
		g_pVtxBuffPolygonBg2 = NULL;
	}
}

//=============================================================================
// �o�b�N�O���E���h�X�V����
//=============================================================================
void UpdateBg2(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	PLAYER *pPlayer;

	//�Q�[���̃X�e�[�W���擾
	g_ModeBg2 = GetMode();

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPolygonBg2->Lock(0, 0, (void**)&pVtx, 0);

	//��
	if (g_ModeBg2 == MODE_GAME)
	{
		//���ɗ����A�j���[�V����
		g_nCounterAnimBg2++;	//�J�E���^���Z
		if (g_nCounterAnimBg2 == 1)//����
		{
			g_nCounterAnimBg2 = 0;
			g_nPatternAnimBg2Y++;
			if (g_nPatternAnimBg2Y == 1000)//����
			{
				g_nPatternAnimBg2Y = 0;
			}
		}

		//�v���C���[�̏󋵂��擾
		pPlayer = GetPlayer();

		//�w�i�̏󋵂��擾
		g_bMoveBg2 = GetBg();

		//�u���b�N�ɉ����瓖�����Ă��邩�ǂ����擾
		g_bBlockXB2 = GetBlockX();

		//�v���C���[�̑���ɑΉ������ړ�
		if (pPlayer->bDisp == true && g_bMoveBg2 == true && g_bBlockXB2 == false)
		{
			if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
			{
				g_nCounterAnimBg2Right++;					//�J�E���^���Z
				if (g_nCounterAnimBg2Right == 2)			//����
				{
					g_nCounterAnimBg2Right = 0;
					g_nPatternAnimBg2X++;
					if (g_nPatternAnimBg2X == 1000)	//����
					{
						g_nPatternAnimBg2X = 0;
					}
				}
			}

			if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
			{
				g_nCounterAnimBg2Left++;					//�J�E���^���Z
				if (g_nCounterAnimBg2Left == 2)			//����
				{
					g_nCounterAnimBg2Left = 0;
					g_nPatternAnimBg2X--;
					if (g_nPatternAnimBg2X == -1000)	//����
					{
						g_nPatternAnimBg2X = 0;
					}
				}
			}
		}

		//�e�N�X�`�����W���X�V
		pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f, g_nPatternAnimBg2Y *0.001f);
		pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f + 0.997f, g_nPatternAnimBg2Y *0.001f);
		pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f, g_nPatternAnimBg2Y *0.001f - 0.999f);
		pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f + 0.997f, g_nPatternAnimBg2Y *0.001f - 0.999f);

		//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
		if (g_nCntLight == 0)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		}
	}

	//��
	if (g_ModeBg2 == MODE_GAME2)
	{
		//�u���b�N����~�ʒu�ɂ��ǂ蒅�������ǂ������擾
		g_bBlockYB2 = GetBlockY();

		//���ɗ����A�j���[�V����
		if (g_bBlockYB2 == false)
		{
			g_nCounterAnimBg2++;	//�J�E���^���Z
			if (g_nCounterAnimBg2 == 4)//����
			{
				g_nCounterAnimBg2 = 0;
				g_nPatternAnimBg2Y++;
				if (g_nPatternAnimBg2Y == 100)//����
				{
					g_nPatternAnimBg2Y = 0;
				}
			}
		}

		//�e�N�X�`�����W���X�V
		pVtx[4].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2Y *0.01f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2Y *0.01f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2Y *0.01f - 0.99f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2Y *0.01f - 0.99f);
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygonBg2->Unlock();
}

//=============================================================================
// �o�b�N�O���E���h�`�揈��
//=============================================================================
void DrawBg2(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPolygonBg2, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�Q�[��1 ��
	if (g_ModeBg2 == MODE_GAME)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonBg2[0]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4*0, NUM_POLYGON);
	}

	//�Q�[��2 ��
	if (g_ModeBg2 == MODE_GAME2)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonBg2[1]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4*1, NUM_POLYGON);
	}
}