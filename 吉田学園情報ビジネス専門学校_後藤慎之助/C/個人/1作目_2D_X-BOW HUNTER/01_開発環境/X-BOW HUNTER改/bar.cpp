//=============================================================================
//
// X-BOW HUNTER�̏��� [bar.cpp] (HP�o�[)
// Author :�㓡�T�V��
//
//=============================================================================
#include "bar.h"
#include "hunter.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureBar[2] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBar = NULL;

//=============================================================================
//HP�o�[����������
//=============================================================================
void InitBar(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HUNTER*pHunter = GetHunter();	//�n���^�[��HP���擾

									//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "bar000.png", &g_pTextureBar[0]);
	D3DXCreateTextureFromFile(pDevice, "bar003.png", &g_pTextureBar[1]);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX * 2, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBar, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffBar->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

													//HP���������ꍇ�A�摜�̉E������`�����߂�
	pVtx[0].pos = D3DXVECTOR3(20, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(20 + 320 * ((pHunter->nLife / HUNTER_LIFE)), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(20, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(20 + 320 * ((pHunter->nLife / HUNTER_LIFE)), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585, 0.0f);

	//HP�o�[�̔w�i
	pVtx[4].pos = D3DXVECTOR3(20.0f - BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585.0f - BAR_EXTEND, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(20.0f + 320.0f + BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585.0f - BAR_EXTEND, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(20.0f - BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585.0f + BAR_EXTEND, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(20.0f + 320.0f + BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585.0f + BAR_EXTEND, 0.0f);

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
	pVtx[0].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	pVtx[1].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	pVtx[2].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	pVtx[3].col = D3DCOLOR_RGBA(70, 185, 127, 255);

	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((pHunter->nLife) * (1 / HUNTER_LIFE), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((pHunter->nLife) * (1 / HUNTER_LIFE), 1.0f);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffBar->Unlock();
}

//=============================================================================
// HP�o�[�I������
//=============================================================================
void UninitBar(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureBar != NULL)
	{
		for (int nCntBar = 0; nCntBar < 2; nCntBar++)
		{
			g_pTextureBar[nCntBar]->Release();
			g_pTextureBar[nCntBar] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBar != NULL)
	{
		g_pVtxBuffBar->Release();
		g_pVtxBuffBar = NULL;
	}
}

//=============================================================================
// HP�o�[�X�V����
//=============================================================================
void UpdateBar(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	HUNTER*pHunter = GetHunter();	//�n���^�[��HP���擾

									//���_�o�b�t�@�����b�N
	g_pVtxBuffBar->Lock(0, 0, (void**)&pVtx, 0);

	//HP���������ꍇ�A�摜�̉E������`�����߂�
	if (pHunter->bDisp == true)
	{
		pVtx[0].pos = D3DXVECTOR3(20, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(20 + 320 * ((pHunter->nLife / HUNTER_LIFE)), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(20, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(20 + 320 * ((pHunter->nLife / HUNTER_LIFE)), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585, 0.0f);
	}

	//HP���Ȃ��Ȃ����ꍇ�A�摜��\�������Ȃ�
	if (pHunter->bDisp == false)
	{
		pVtx[0].pos = D3DXVECTOR3(20, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(20 + 320 * (0), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(20, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(20 + 320 * (0), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585, 0.0f);
	}


	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((pHunter->nLife / HUNTER_LIFE), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((pHunter->nLife / HUNTER_LIFE), 1.0f);

	//HP�ɉ����ăJ���[��ς���i���^�����甼���܂ŗ΁j
	if (pHunter->nLife <= HUNTER_LIFE&&pHunter->nLife > HUNTER_LIFE / 2)
	{
		pVtx[0].col = D3DCOLOR_RGBA(70, 185, 127, 255);
		pVtx[1].col = D3DCOLOR_RGBA(70, 185, 127, 255);
		pVtx[2].col = D3DCOLOR_RGBA(70, 185, 127, 255);
		pVtx[3].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	}

	//HP�ɉ����ăJ���[��ς���i��������4����1�܂ŉ��F�j
	if (pHunter->nLife <= HUNTER_LIFE / 2 && pHunter->nLife > HUNTER_LIFE / 4)
	{
		pVtx[0].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 236, 0, 255);
	}

	//HP�ɉ����ăJ���[��ς���i4����1����͐ԐF�j
	if (pHunter->nLife <= HUNTER_LIFE / 4)
	{
		pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffBar->Unlock();

}

//=============================================================================
// HP�o�[�`�揈��
//=============================================================================
void DrawBar(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HUNTER*pHunter = GetHunter();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffBar, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBar = 1; nCntBar >-1; nCntBar--)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBar[nCntBar]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBar, NUM_POLYGON);
	}

}