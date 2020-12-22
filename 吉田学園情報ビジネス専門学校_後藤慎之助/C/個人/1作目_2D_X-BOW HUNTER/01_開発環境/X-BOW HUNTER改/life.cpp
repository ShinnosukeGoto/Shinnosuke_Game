//=============================================================================
//
// X-BOW HUNTER�̏��� [life.cpp] (���C�t)
// Author :�㓡�T�V��
//
//=============================================================================
#include "life.h"
#include "hunter.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureLife = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffLife = NULL;

//=============================================================================
//���C�t����������
//=============================================================================
void InitLife(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HUNTER*pHunter = GetHunter();	//�n���^�[�̃X�g�b�N���擾

									//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "life001.png", &g_pTextureLife);


	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffLife, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

													//�X�g�b�N���������ꍇ�A�摜�̉E������`�����߂�
	pVtx[0].pos = D3DXVECTOR3(55, (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) - (LIFE_SIZE_Y / 2) - 625, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(55 + 320 * (0.25*(pHunter->nStock + 1)), (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) - (LIFE_SIZE_Y / 2) - 625, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(55, (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) + (LIFE_SIZE_Y / 2) - 625, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(55 + 320 * (0.25*(pHunter->nStock + 1)), (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) + (LIFE_SIZE_Y / 2) - 625, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2((pHunter->nStock + 1) * 0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((pHunter->nStock + 1) * 0.25f, 1.0f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffLife->Unlock();
}

//=============================================================================
// ���C�t�I������
//=============================================================================
void UninitLife(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//=============================================================================
// ���C�t�X�V����
//=============================================================================
void UpdateLife(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	HUNTER*pHunter = GetHunter();	//�n���^�[�̃X�g�b�N���擾

									//���_�o�b�t�@�����b�N
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	//�X�g�b�N���������ꍇ�A�摜�̉E������`�����߂�
	pVtx[0].pos = D3DXVECTOR3(55, (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) - (LIFE_SIZE_Y / 2) - 625, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(55 + 320 * (0.25*(pHunter->nStock + 1)), (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) - (LIFE_SIZE_Y / 2) - 625, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(55, (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) + (LIFE_SIZE_Y / 2) - 625, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(55 + 320 * (0.25*(pHunter->nStock + 1)), (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) + (LIFE_SIZE_Y / 2) - 625, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((pHunter->nStock + 1) * 0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((pHunter->nStock + 1) * 0.25f, 1.0f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffLife->Unlock();
}

//=============================================================================
// ���C�t�`�揈��
//=============================================================================
void DrawLife(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HUNTER*pHunter = GetHunter();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureLife);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}