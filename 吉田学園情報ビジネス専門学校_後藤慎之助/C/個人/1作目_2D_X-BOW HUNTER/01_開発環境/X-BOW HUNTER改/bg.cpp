//=============================================================================
//
// X-Bow Hunter [bg.cpp]�̏��� (�����o�b�N�O���E���h�Ɨ���Ȃ�UI)
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "item.h"
#include "boss.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonBg[4] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonBg[2] = {};
int g_nCounterAnimBg;			//�A�j���[�V�����J�E���^�[
int g_nPatternAnimBg;			//�A�j���[�V�����p�^�[��

								//=============================================================================
								//�o�b�N�O���E���h����������
								//=============================================================================
void InitBg(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "bg000.png", &g_pTexturePolygonBg[0]);
	D3DXCreateTextureFromFile(pDevice, "score000.png", &g_pTexturePolygonBg[1]);
	D3DXCreateTextureFromFile(pDevice, "time000.png", &g_pTexturePolygonBg[2]);
	D3DXCreateTextureFromFile(pDevice, "stock000.png", &g_pTexturePolygonBg[3]);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonBg[0], NULL);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX * 3, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonBg[1], NULL);
	VERTEX_2D *pVtxBg;	//�o�b�N�O���E���h�̒��_���ւ̃|�C���^
	VERTEX_2D *pVtxUI;	//UI�̒��_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPolygonBg[0]->Lock(0, 0, (void**)&pVtxBg, 0);	//���̏����͕ς��Ȃ�����
	g_pVtxBuffPolygonBg[1]->Lock(0, 0, (void**)&pVtxUI, 0);	//���̏����͕ς��Ȃ�����

															//���_���W�̐ݒ�i�E���Őݒ肷��j
	pVtxBg[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtxBg[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtxBg[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtxBg[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//�X�R�A�̍��W�ݒ�
	pVtxUI[0].pos = D3DXVECTOR3(800.0f, 0.0f, 0.0f);
	pVtxUI[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtxUI[2].pos = D3DXVECTOR3(800.0f, 100.0f, 0.0f);
	pVtxUI[3].pos = D3DXVECTOR3(1280.0f, 100.0f, 0.0f);

	//�^�C���̍��W�ݒ�
	pVtxUI[4].pos = D3DXVECTOR3(530.0f, 0.0f, 0.0f);
	pVtxUI[5].pos = D3DXVECTOR3(680.0f, 0.0f, 0.0f);
	pVtxUI[6].pos = D3DXVECTOR3(530.0f, 110.0f, 0.0f);
	pVtxUI[7].pos = D3DXVECTOR3(680.0f, 110.0f, 0.0f);

	//�X�g�b�N�̍��W�ݒ�
	pVtxUI[8].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtxUI[9].pos = D3DXVECTOR3(350.0f, 0.0f, 0.0f);
	pVtxUI[10].pos = D3DXVECTOR3(0.0f, 110.0f, 0.0f);
	pVtxUI[11].pos = D3DXVECTOR3(350.0f, 110.0f, 0.0f);

	//rhw�̐ݒ�i�l��1.0�Őݒ肷��j
	pVtxBg[0].rhw = 1.0f;
	pVtxBg[1].rhw = 1.0f;
	pVtxBg[2].rhw = 1.0f;
	pVtxBg[3].rhw = 1.0f;
	pVtxUI[0].rhw = 1.0f;
	pVtxUI[1].rhw = 1.0f;
	pVtxUI[2].rhw = 1.0f;
	pVtxUI[3].rhw = 1.0f;
	pVtxUI[4].rhw = 1.0f;
	pVtxUI[5].rhw = 1.0f;
	pVtxUI[6].rhw = 1.0f;
	pVtxUI[7].rhw = 1.0f;
	pVtxUI[8].rhw = 1.0f;
	pVtxUI[9].rhw = 1.0f;
	pVtxUI[10].rhw = 1.0f;
	pVtxUI[11].rhw = 1.0f;

	//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
	pVtxBg[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtxBg[0].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f, 0.0f);
	pVtxBg[1].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f - 0.999f, 0.0f);
	pVtxBg[2].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f, 1.0f);
	pVtxBg[3].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f - 0.999f, 1.0f);
	pVtxUI[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtxUI[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtxUI[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtxUI[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtxUI[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtxUI[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtxUI[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtxUI[7].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtxUI[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtxUI[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtxUI[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtxUI[11].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N����
	for (int nCntBg = 0; nCntBg < 2; nCntBg++)
	{
		g_pVtxBuffPolygonBg[nCntBg]->Unlock();
	}
}

//=============================================================================
// �o�b�N�O���E���h�I������
//=============================================================================
void UninitBg(void)
{

	//�e�N�X�`���̊J��
	for (int nCntUI = 0; nCntUI < 4; nCntUI++)
	{
		if (g_pTexturePolygonBg != NULL)
		{
			g_pTexturePolygonBg[nCntUI]->Release();
			g_pTexturePolygonBg[nCntUI] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	for (int nCntBg = 0; nCntBg < 2; nCntBg++)
	{
		if (g_pVtxBuffPolygonBg != NULL)
		{
			g_pVtxBuffPolygonBg[nCntBg]->Release();
			g_pVtxBuffPolygonBg[nCntBg] = NULL;
		}
	}
}

//=============================================================================
// �o�b�N�O���E���h�X�V����
//=============================================================================
void UpdateBg(void)
{
	VERTEX_2D *pVtxBg;	//���_���ւ̃|�C���^
	ITEM *pItem;		//�A�C�e���̏����擾���邽��
	BOSS *pBoss;		//�{�X�̏����擾���邽��

	g_pVtxBuffPolygonBg[0]->Lock(0, 0, (void**)&pVtxBg, 0);	//���̏����͕ς��Ȃ�����

	pItem = GetItem();	//�A�C�e���̏����擾
	pBoss = GetBoss();	//�{�X�̏����擾

						//���֗����A�j���[�V����
	if (pItem->bUse == false)				//�A�C�e��������Ƃ��́A�w�i���~�߂āA�A�C�e���擾�𑣂�
	{
		g_nCounterAnimBg++;					//�J�E���^���Z
		if (g_nCounterAnimBg == 2)			//����
		{
			g_nCounterAnimBg = 0;
			g_nPatternAnimBg++;
			if (g_nPatternAnimBg == 1000)	//����
			{
				g_nPatternAnimBg = 0;
			}
		}
	}

	//�e�N�X�`�����W���X�V
	pVtxBg[0].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f, 0.0f);
	pVtxBg[1].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f - 0.999f, 0.0f);
	pVtxBg[2].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f, 1.0f);
	pVtxBg[3].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f - 0.999f, 1.0f);

	//�{�X��ȊO�̐F
	if (pBoss->bUse == false)
	{
		//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
		pVtxBg[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtxBg[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtxBg[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtxBg[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//�{�X��̎��̔w�i�F(�댯�Ȋ������o��)
	if (pBoss->bUse == true)
	{
		//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
		pVtxBg[0].col = D3DCOLOR_RGBA(249, 173, 0, 255);
		pVtxBg[1].col = D3DCOLOR_RGBA(249, 173, 0, 255);
		pVtxBg[2].col = D3DCOLOR_RGBA(249, 173, 0, 255);
		pVtxBg[3].col = D3DCOLOR_RGBA(249, 173, 0, 255);
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygonBg[0]->Unlock();
}

//=============================================================================
// �o�b�N�O���E���h�`�揈��
//=============================================================================
void DrawBg(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPolygonBg[0], 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePolygonBg[0]);
	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPolygonBg[1], 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCntBgUI = 0; nCntBgUI < 3; nCntBgUI++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonBg[nCntBgUI + 1]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBgUI, NUM_POLYGON);
	}
}