//=============================================================================
//
// X-BOW HUNTER�̏��� [wing.cpp] (�{�X�펞�̗�)
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "hunter.h"
#include "wing.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureWing = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffWing = NULL;
int g_nCounterAnimWing;			//�A�j���[�V�����J�E���^�[
int g_nPatternAnimWing;			//�A�j���[�V�����p�^�[��

WING g_Wing;	//�E�B���O�̏��

				//=============================================================================
				//�E�B���O����������
				//=============================================================================
void InitWing(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_nCounterAnimWing = 0;
	g_nPatternAnimWing = 0;

	//�E�B���O���̏�����
	g_Wing.bUse = false;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "wing000.png", &g_pTextureWing);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffWing, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffWing->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

													//���_���W�̐ݒ�i�E���Őݒ肷��j
	pVtx[0].pos = D3DXVECTOR3(0.0f - (WING_SIZE / 2), 0.0f - (WING_SIZE / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f + (WING_SIZE / 2), 0.0f - (WING_SIZE / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f - (WING_SIZE / 2), 0.0f + (WING_SIZE / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f + (WING_SIZE / 2), 0.0f + (WING_SIZE / 2), 0.0f);

	//rhw�̐ݒ�i�l��1.0�Őݒ肷��j
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
	pVtx[0].col = D3DCOLOR_RGBA(0, 166, 248, 200);
	pVtx[1].col = D3DCOLOR_RGBA(0, 166, 248, 200);
	pVtx[2].col = D3DCOLOR_RGBA(0, 166, 248, 200);
	pVtx[3].col = D3DCOLOR_RGBA(0, 166, 248, 200);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f + 0.33f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f + 0.33f, 1.0f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffWing->Unlock();
}

//=============================================================================
// �E�B���O�I������
//=============================================================================
void UninitWing(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureWing != NULL)
	{
		g_pTextureWing->Release();
		g_pTextureWing = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffWing != NULL)
	{
		g_pVtxBuffWing->Release();
		g_pVtxBuffWing = NULL;
	}
}

//=============================================================================
// �E�B���O�X�V����
//=============================================================================
void UpdateWing(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	HUNTER *pHunter;

	pHunter = GetHunter();

	//�H���g���Ă��邩�n���^�[���������Ă���Ȃ�X�V
	if (g_Wing.bUse == true && pHunter->bDisp == true)
	{
		g_nCounterAnimWing++;	//�J�E���^���Z
		if (g_nCounterAnimWing == 7)
		{
			g_nCounterAnimWing = 0;
			g_nPatternAnimWing++;
			if (g_nPatternAnimWing == 3)
			{
				g_nPatternAnimWing = 0;
			}
		}

		//���_�o�b�t�@�����b�N
		g_pVtxBuffWing->Lock(0, 0, (void**)&pVtx, 0);

		//�e�N�X�`�����W���X�V
		pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f + 0.34f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f + 0.34f, 1.0f);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pHunter->pos.x - (WING_SIZE / 2) - 20, pHunter->pos.y - (WING_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pHunter->pos.x + (WING_SIZE / 2) - 20, pHunter->pos.y - (WING_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pHunter->pos.x - (WING_SIZE / 2) - 20, pHunter->pos.y + (WING_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pHunter->pos.x + (WING_SIZE / 2) - 20, pHunter->pos.y + (WING_SIZE / 2), 0.0f);

		//���@���s���`��Ԃ��X�g�b�N0�ŉH������
		if (pHunter->nLife > 0 && pHunter->nLife < HUNTER_LIFE / 4 && pHunter->nStock == 0)
		{
			g_Wing.bUse = false;
		}

		//���_�f�[�^���A�����b�N����
		g_pVtxBuffWing->Unlock();
	}
}

//=============================================================================
// �E�B���O�`�揈��
//=============================================================================
void DrawWing(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HUNTER *pHunter;

	pHunter = GetHunter();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffWing, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Wing.bUse == true && pHunter->bDisp == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureWing);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}


//=============================================================================
// �E�B���O�̐ݒ�
//=============================================================================
void SetWing(void)
{
	if (g_Wing.bUse == false)
	{
		g_Wing.bUse = true;
	}
}

//=============================================================================
// �E�B���O�̎擾
//=============================================================================
WING*GetWing(void)
{
	return &g_Wing;
}