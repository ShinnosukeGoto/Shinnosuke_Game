//=============================================================================
//
// �i�C���X�g�[���[�Y [ui.cpp]�̏��� (�Q�[����ʂ�UI)
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "ui.h"
#include "title.h"
#include "select.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonUI[11] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonUI = NULL;

GAMELEVEL g_nUILevel;	//���x���̏��
WSELECT g_nUIWtype;		//����I��
MSELECT g_nUIMtype;		//���@�I��

//=============================================================================
//UI����������
//=============================================================================
void InitUI(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_nUILevel = GetTitle();		//��Փx���擾
	g_nUIWtype = GetWSelect();		//����I�����擾
	g_nUIMtype = GetMSelect();		//���@�I�����擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/score000.png", &g_pTexturePolygonUI[0]);	//�X�R�A�̔w�i
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/time000.png", &g_pTexturePolygonUI[1]);		//�^�C���̔w�i
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/hpmp000.png", &g_pTexturePolygonUI[2]);		//HP,MP�o�[�̔w�i
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI000.png", &g_pTexturePolygonUI[3]);		//��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI001.png", &g_pTexturePolygonUI[4]);		//��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI002.png", &g_pTexturePolygonUI[5]);		//�|
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI003.png", &g_pTexturePolygonUI[6]);		//��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI004.png", &g_pTexturePolygonUI[7]);		//��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI005.png", &g_pTexturePolygonUI[8]);		//�h���C��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/NORMAL000.png", &g_pTexturePolygonUI[9]);	//NORMAL
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/HARD000.png", &g_pTexturePolygonUI[10]);	//HARD

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX * 6, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonUI, NULL);	//�X�R�A�A�^�C���A�o�[�A����A���@�A��Փx
	VERTEX_2D *pVtxUI;	//�o�b�N�O���E���h�̒��_���ւ̃|�C���^

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPolygonUI->Lock(0, 0, (void**)&pVtxUI, 0);	//���̏����͕ς��Ȃ�����

	//�X�R�A�̍��W�ݒ�
	pVtxUI[0].pos = D3DXVECTOR3(900.0f, 20.0f, 0.0f);
	pVtxUI[1].pos = D3DXVECTOR3(1260.0f, 20.0f, 0.0f);
	pVtxUI[2].pos = D3DXVECTOR3(900.0f, 80.0f, 0.0f);
	pVtxUI[3].pos = D3DXVECTOR3(1260.0f, 80.0f, 0.0f);

	//�^�C���̍��W�ݒ�
	pVtxUI[4].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - 75, 0.0f, 0.0f);
	pVtxUI[5].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + 75, 0.0f, 0.0f);
	pVtxUI[6].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - 75, 110.0f, 0.0f);
	pVtxUI[7].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + 75, 110.0f, 0.0f);

	//HPMP�̍��W�ݒ�
	pVtxUI[8].pos = D3DXVECTOR3(20.0f, 20.0f, 0.0f);
	pVtxUI[9].pos = D3DXVECTOR3(70.0f, 20.0f, 0.0f);
	pVtxUI[10].pos = D3DXVECTOR3(20.0f, 90.0f, 0.0f);
	pVtxUI[11].pos = D3DXVECTOR3(70.0f, 90.0f, 0.0f);

	//����I���̍��W�ݒ�
	pVtxUI[12].pos = D3DXVECTOR3(70.0f, 90.0f, 0.0f);
	pVtxUI[13].pos = D3DXVECTOR3(160.0f, 90.0f, 0.0f);
	pVtxUI[14].pos = D3DXVECTOR3(70.0f, 130.0f, 0.0f);
	pVtxUI[15].pos = D3DXVECTOR3(160.0f, 130.0f, 0.0f);

	//���@�I���̍��W�ݒ�
	pVtxUI[16].pos = D3DXVECTOR3(165.0f, 90.0f, 0.0f);
	pVtxUI[17].pos = D3DXVECTOR3(365.0f, 90.0f, 0.0f);
	pVtxUI[18].pos = D3DXVECTOR3(165.0f, 130.0f, 0.0f);
	pVtxUI[19].pos = D3DXVECTOR3(365.0f, 130.0f, 0.0f);

	//��Փx�I���̍��W�ݒ�
	pVtxUI[20].pos = D3DXVECTOR3(180.0f, 0.0f, 0.0f);
	pVtxUI[21].pos = D3DXVECTOR3(280.0f, 0.0f, 0.0f);
	pVtxUI[22].pos = D3DXVECTOR3(180.0f, 30.0f, 0.0f);
	pVtxUI[23].pos = D3DXVECTOR3(280.0f, 30.0f, 0.0f);

	//rhw�̐ݒ�i�l��1.0�Őݒ肷��j
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
	pVtxUI[12].rhw = 1.0f;
	pVtxUI[13].rhw = 1.0f;
	pVtxUI[14].rhw = 1.0f;
	pVtxUI[15].rhw = 1.0f;
	pVtxUI[16].rhw = 1.0f;
	pVtxUI[17].rhw = 1.0f;
	pVtxUI[18].rhw = 1.0f;
	pVtxUI[19].rhw = 1.0f;
	pVtxUI[20].rhw = 1.0f;
	pVtxUI[21].rhw = 1.0f;
	pVtxUI[22].rhw = 1.0f;
	pVtxUI[23].rhw = 1.0f;

	//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
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
	pVtxUI[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[16].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[17].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[18].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[19].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[20].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[21].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[22].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[23].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
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
	pVtxUI[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtxUI[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtxUI[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtxUI[15].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtxUI[16].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtxUI[17].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtxUI[18].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtxUI[19].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtxUI[20].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtxUI[21].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtxUI[22].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtxUI[23].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygonUI->Unlock();
}

//=============================================================================
// UI�I������
//=============================================================================
void UninitUI(void)
{
	//�e�N�X�`���̊J��
	for (int nCntUI = 0; nCntUI < 11; nCntUI++)
	{
		if (g_pTexturePolygonUI[nCntUI] != NULL)
		{
			g_pTexturePolygonUI[nCntUI]->Release();
			g_pTexturePolygonUI[nCntUI] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPolygonUI != NULL)
	{
		g_pVtxBuffPolygonUI->Release();
		g_pVtxBuffPolygonUI = NULL;
	}
}

//=============================================================================
// UI�X�V����
//=============================================================================
void UpdateUI(void)
{
	
}

//=============================================================================
// UI�`�揈��
//=============================================================================
void DrawUI(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPolygonUI, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�X�R�A�A�^�C���A�o�[
	for (int nCntUI = 0; nCntUI < 3; nCntUI++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonUI[nCntUI]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntUI, NUM_POLYGON);
	}

	//����
	if (g_nUIWtype == WSELECT_SWORD)	//��
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonUI[3]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, NUM_POLYGON);
	}

	if (g_nUIWtype == WSELECT_LANCE)	//��
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonUI[4]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, NUM_POLYGON);
	}

	if (g_nUIWtype == WSELECT_BOW)		//�|
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonUI[5]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, NUM_POLYGON);
	}

	//���@
	if (g_nUIMtype == MSELECT_FIRE)			//��
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonUI[6]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, NUM_POLYGON);
	}

	if (g_nUIMtype == MSELECT_LIGHTNING)	//��
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonUI[7]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, NUM_POLYGON);
	}

	if (g_nUIMtype == MSELECT_DRAIN)		//�z��
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonUI[8]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, NUM_POLYGON);
	}

	//��Փx
	if (g_nUILevel == GAMELEVEL_NORMAL)		//NORMAL
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonUI[9]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, NUM_POLYGON);
	}

	if (g_nUILevel == GAMELEVEL_HARD)	//HARD
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePolygonUI[10]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, NUM_POLYGON);
	}
}
