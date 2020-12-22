//=============================================================================
//
// �i�C���X�g�[���[�Y [bg.cpp]�̏��� (�o�b�N�O���E���h)
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "player.h"
#include "input.h"
#include "enemy.h"
#include "block.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonBg[3] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonBg = NULL;
D3DXVECTOR3 g_moveBg;			//�w�i�̈ړ��ʁi�X�e�[�W�S�̂̒��������߂�j
int g_nCounterAnimBgLeft;		//���̃A�j���[�V�����J�E���^�[
int g_nCounterAnimBgRight;		//�E�̃A�j���[�V�����J�E���^�[
int g_nPatternAnimBg;			//�A�j���[�V�����p�^�[��
int g_nShakeMoveBg;				//�w�i���h���Ƃ��̐��l(���ށj
int g_nShakeMoveBg2;			//�w�i���h���Ƃ��̐��l(�߂�)
bool g_bMoveBg;					//�w�i�������Ă���
bool g_bShakeBg;				//�w�i���h���
bool g_bBlockXB;				//�u���b�N�ɉ����瓖��������
bool g_bBlockYB;				//�u���b�N����~�ʒu�ɂ��ǂ蒅�����Ƃ�
MODE g_ModeBg;					//�Q�[����ʂɑΉ������w�i

//=============================================================================
//�o�b�N�O���E���h����������
//=============================================================================
void InitBg(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bg000.jpg", &g_pTexturePolygonBg[0]);	//��̕���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bg001.png", &g_pTexturePolygonBg[1]);	//�������̔w�i
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bg009.png", &g_pTexturePolygonBg[2]);	//���̒�

	//�O���[�o���ϐ��̏�����
	g_moveBg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nCounterAnimBgLeft = 0;
	g_nCounterAnimBgRight = 0;
	g_nPatternAnimBg = 0;
	g_nShakeMoveBg = 0;
	g_nShakeMoveBg2 = 0;
	g_bMoveBg = false;	//�ŏ��́A�w�i�͓����Ȃ�
	g_bShakeBg = false;	//�ŏ��́A�w�i�͗h��Ȃ�
	g_bBlockXB = false;
	g_bBlockYB = false;
	g_ModeBg = MODE_GAME;

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonBg, NULL);
	VERTEX_2D *pVtxBg;	//�o�b�N�O���E���h�̒��_���ւ̃|�C���^

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPolygonBg->Lock(0, 0, (void**)&pVtxBg, 0);	//���̏����͕ς��Ȃ�����

	//���_���W�̐ݒ�i�E���Őݒ肷��j
	pVtxBg[0].pos = D3DXVECTOR3(0.0f, -7.0f, 0.0f);
	pVtxBg[1].pos = D3DXVECTOR3(SCREEN_WIDTH, -7.0f, 0.0f);
	pVtxBg[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT�U, 0.0f);
	pVtxBg[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT�U, 0.0f);

	//rhw�̐ݒ�i�l��1.0�Őݒ肷��j
	pVtxBg[0].rhw = 1.0f;
	pVtxBg[1].rhw = 1.0f;
	pVtxBg[2].rhw = 1.0f;
	pVtxBg[3].rhw = 1.0f;

	//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
	pVtxBg[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtxBg[0].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f, 0.0f);
	pVtxBg[1].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f + 0.997f, 0.0f);
	pVtxBg[2].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f, 1.0f);
	pVtxBg[3].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f + 0.997f, 1.0f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygonBg->Unlock();
}

//=============================================================================
// �o�b�N�O���E���h�I������
//=============================================================================
void UninitBg(void)
{
	//�e�N�X�`���̊J��
	for (int nCntBg = 0; nCntBg < 3; nCntBg++)
	{
		if (g_pTexturePolygonBg[nCntBg] != NULL)
		{
			g_pTexturePolygonBg[nCntBg]->Release();
			g_pTexturePolygonBg[nCntBg] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPolygonBg != NULL)
	{
		g_pVtxBuffPolygonBg ->Release();
		g_pVtxBuffPolygonBg = NULL;
	}
}

//=============================================================================
// �o�b�N�O���E���h�X�V����
//=============================================================================
void UpdateBg(void)
{
	VERTEX_2D *pVtxBg;	//���_���ւ̃|�C���^
	PLAYER*pPlayer = GetPlayer();		//�v���C���[�̏󋵂��擾

	//�v���C���[���U�����󂯂����ǂ����擾
	g_bShakeBg = HitPlayer2();

	g_bBlockXB = GetBlockX();

	//�Q�[���̃X�e�[�W���擾
	g_ModeBg = GetMode();

	//�v���C���[���U�����󂯂Ă��Ȃ��Ȃ�A�G���U�����󂯂Ă��邩�ǂ������擾
	if (g_bShakeBg == false)
	{
		g_bShakeBg = HitEnemy2();
	}

	g_pVtxBuffPolygonBg->Lock(0, 0, (void**)&pVtxBg, 0);	//���̏����͕ς��Ȃ�����

	//�Q�[��1(��̕���)
	if (g_ModeBg == MODE_GAME)
	{
		//�v���C���[���E�ɐi�ނɂ�āA�E�ɂ�����̂��o�Ă���
		if (pPlayer->bDisp == true)
		{

			//A��D���������͖���
			if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_D))
			{

			}

			//�E�ɐi��
			else if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D) && g_bBlockXB == false)
			{
				if (pPlayer->pos.x == SCREEN_WIDTH / 2)	//�v���C���[�������ɂ���Ƃ������w�i�𗬂�
				{
					g_moveBg.x++;
					g_nCounterAnimBgRight++;					//�J�E���^���Z
					if (g_nCounterAnimBgRight == 2)			//����
					{
						g_nCounterAnimBgRight = 0;
						g_nPatternAnimBg++;
						if (g_nPatternAnimBg == STAGE1_X)	//����
						{
							g_nPatternAnimBg = 0;
						}
					}
				}

				//��ԉE�ȊO�́A�w�i�𗬂�
				if (g_moveBg.x > STAGE1_X)
				{
					g_moveBg.x = STAGE1_X;
				}
			}

			//���ɐi��
			else if (pPlayer->nAttackTime == 0 && g_bBlockXB == false && GetKeyboardPress(DIK_A))
			{
				if (pPlayer->pos.x == SCREEN_WIDTH / 2)	//�v���C���[�������ɂ���Ƃ������w�i�𗬂�
				{
					g_moveBg.x--;
					g_nCounterAnimBgLeft++;					//�J�E���^���Z
					if (g_nCounterAnimBgLeft == 2)			//����
					{
						g_nCounterAnimBgLeft = 0;
						g_nPatternAnimBg--;
						if (g_nPatternAnimBg == -STAGE1_X)	//����
						{
							g_nPatternAnimBg = 0;
						}
					}
				}

				//��ԍ��ȊO�́A�w�i�𗬂�
				if (g_moveBg.x < 0)
				{
					g_moveBg.x = 0;
				}
			}
		}

		//��ʂ̒[�ł͔w�i�͓����Ă��Ȃ�
		if (g_moveBg.x == 0)
		{
			g_bMoveBg = false;
		}
		else if (g_moveBg.x == STAGE1_X)
		{
			g_bMoveBg = false;
		}
		else
		{
			g_bMoveBg = true;
		}

		//�e�N�X�`�����W���X�V
		pVtxBg[0].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f, 0.0f);
		pVtxBg[1].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f + 0.997f, 0.0f);
		pVtxBg[2].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f, 1.0f);
		pVtxBg[3].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f + 0.997f, 1.0f);
	}

	//�Q�[��2(���̓���)
	if (g_ModeBg == MODE_GAME2)
	{
		//�u���b�N����~�ʒu�ɂ��ǂ蒅�������ǂ������擾
		g_bBlockYB = GetBlockY();

		//���ɗ����A�j���[�V����
		if (g_bBlockYB == false)
		{
			g_nCounterAnimBgRight++;					//�J�E���^���Z
			if (g_nCounterAnimBgRight == 2)			//����
			{
				g_nCounterAnimBgRight = 0;
				g_nPatternAnimBg++;
				if (g_nPatternAnimBg == 1000)	//����
				{
					g_nPatternAnimBg = 0;
				}
			}
		}

		g_bMoveBg = false;	//�w�i�͓����Ă��Ȃ�

		//�e�N�X�`�����W���X�V
		pVtxBg[0].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg *0.002f);
		pVtxBg[1].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg *0.002f);
		pVtxBg[2].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg *0.002f - 0.998f);
		pVtxBg[3].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg *0.002f - 0.998f);
	}

	//�Q�[��3(���̒�)
	if (g_ModeBg == MODE_GAME3)
	{
		g_bMoveBg = false;	//�w�i�͓����Ă��Ȃ�
		pVtxBg[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtxBg[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtxBg[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtxBg[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//�w�i��h�炷
	if (g_bShakeBg == true)
	{
		g_nShakeMoveBg++;	//�J�E���^���Z
		if (g_nShakeMoveBg >= SHAKE_BG)		//���ސ[��
		{
			g_nShakeMoveBg = SHAKE_BG;
			g_nShakeMoveBg2--;
			if (g_nShakeMoveBg2 <= -SHAKE_BG)	//�߂�[��
			{
				g_nShakeMoveBg = 0;
				g_nShakeMoveBg2 = 0;
				g_bShakeBg = false;
			}
		}
	}
	else
	{
		g_nShakeMoveBg = 0;
		g_nShakeMoveBg2 = 0;
	}

	//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
	pVtxBg[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//���_���W�̐ݒ�i�E���Őݒ肷��j
	pVtxBg[0].pos = D3DXVECTOR3(0.0f, -7.0f + g_nShakeMoveBg + g_nShakeMoveBg2, 0.0f);
	pVtxBg[1].pos = D3DXVECTOR3(SCREEN_WIDTH, -7.0f + g_nShakeMoveBg + g_nShakeMoveBg2, 0.0f);
	pVtxBg[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT�U + g_nShakeMoveBg + g_nShakeMoveBg2, 0.0f);
	pVtxBg[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT�U + g_nShakeMoveBg + g_nShakeMoveBg2, 0.0f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygonBg->Unlock();
}

//=============================================================================
// �o�b�N�O���E���h�`�揈��
//=============================================================================
void DrawBg(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPolygonBg, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�Q�[��1�̔w�i
	if (g_ModeBg == MODE_GAME)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonBg[0]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
	}

	//�Q�[��2�̔w�i
	if (g_ModeBg == MODE_GAME2)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonBg[1]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
	}

	//�Q�[��3�̔w�i
	if (g_ModeBg == MODE_GAME3)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonBg[2]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
	}

}

//==============================================================================
// �o�b�O�O���E���h�̎擾
//==============================================================================
bool GetBg(void)
{
	return g_bMoveBg;
}