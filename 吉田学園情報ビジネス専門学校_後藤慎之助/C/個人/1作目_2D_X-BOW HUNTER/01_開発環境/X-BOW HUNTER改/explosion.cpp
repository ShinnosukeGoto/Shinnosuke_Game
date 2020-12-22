//=============================================================================
//
// X-Bow Hunter [explosion.cpp]�̏��� (�����j
// Author :�㓡�T�V��
//
//=============================================================================
#include "explosion.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_EXPLOSION (128)	//�����̍ő吔

//=============================================================================
//�\���̒�`
//=============================================================================

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureExplosion = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffExplosion = NULL;
EXPLOSION g_aExplosion[MAX_EXPLOSION];						//�����̏��

															//=============================================================================
															//��������������
															//=============================================================================
void InitExplosion(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntExplosion;

	//�������̏�����
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "explosion000.png", &g_pTextureExplosion);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_EXPLOSION, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

														//��̐��������𐶐�
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//���_���W�̐ݒ�i�E���Őݒ肷��j
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�i�l��1.0�Őݒ肷��j
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f + 0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f + 0.125f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// �����I������
//=============================================================================
void UninitExplosion(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// �����X�V����
//=============================================================================
void UpdateExplosion(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntExplosion;
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			//�����̃J�E���^�[��i�߂āA�p�^�[����؂�ւ���
			g_aExplosion[nCntExplosion].nCounterAnim++;
			if (g_aExplosion[nCntExplosion].nCounterAnim == 1)	//�����̑���4
			{
				g_aExplosion[nCntExplosion].nCounterAnim = 0;
				g_aExplosion[nCntExplosion].nPatternAnim++;
			}

			//�e�N�X�`�����W���X�V
			pVtx[0].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f + 0.125f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f + 0.125f, 0.0f);

			//�p�^�[�����Ō�܂ŏI�����
			if (g_aExplosion[nCntExplosion].nPatternAnim > 8)
			{
				g_aExplosion[nCntExplosion].bUse = false;
			}
		}
		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// �����`�揈��
//=============================================================================
void DrawExplosion(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntExplosion;

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�|���S���̕`��
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)	//�������g�p���Ȃ�`��
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureExplosion);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}

//=============================================================================
// �����ݒ菈��
//=============================================================================
bool SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, int nSize)
{
	//�ϐ��錾
	int nCntExplosion;
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;

			//���_���̍X�V
			//���_���W
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - (nSize / 2), g_aExplosion[nCntExplosion].pos.y - (nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + (nSize / 2), g_aExplosion[nCntExplosion].pos.y - (nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - (nSize / 2), g_aExplosion[nCntExplosion].pos.y + (nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + (nSize / 2), g_aExplosion[nCntExplosion].pos.y + (nSize / 2), 0.0f);
			//���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f + 0.125f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f + 0.125f, 0.0f);

			break;
		}
		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
	return(true);
}