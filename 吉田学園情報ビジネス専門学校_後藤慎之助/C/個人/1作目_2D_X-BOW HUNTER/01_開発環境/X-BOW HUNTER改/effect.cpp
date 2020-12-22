//=============================================================================
//
// X-Bow Hunter�̏��� [effect.cpp] (�G�t�F�N�g)
// Author : �㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "effect.h"
#include "bullet.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;//���_�o�b�t�@�ւ̃|�C���^

EFFECT g_aEffect[MAX_EFFECT];	//�G�t�F�N�g�̏��

								//=============================================================================
								// ����������
								//=============================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_2D *pVtx;//���_���̃|�C���^
	int nCntEffect;//�e�̃J�E���g

				   //�I�u�W�F�N�g�̒��_�o�b�t�@����
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_EFFECT,//�o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,//(�Œ�j
		FVF_VERTEX_2D,//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,//�i�Œ�j
		&g_pVtxBuffEffect,//�ϐ������ς��ƕύX���K�v
		NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	//�e�̏��̏�����
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].bUse = false;
		//���_���W�̐ݒ�i�E���Őݒ肷��j
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);
		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//���_�J���[
		pVtx[0].col = D3DCOLOR_RGBA(0, 0, 0, 0);
		pVtx[1].col = D3DCOLOR_RGBA(0, 0, 0, 0);
		pVtx[2].col = D3DCOLOR_RGBA(0, 0, 0, 0);
		pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, 0);
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�f�[�^�A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	VERTEX_2D *pVtx;//���_���̃|�C���^
	int nCntEffect;//�G�t�F�N�g�̃J�E���g

				   //���_�o�b�t�@���b�N
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//�G�t�F�N�g�̎������Ԍ���
			g_aEffect[nCntEffect].nLife--;
			//�G�t�F�N�g�̔��a�����炷
			g_aEffect[nCntEffect].aRadius--;
			//�G�t�F�N�g�̓����x��������
			g_aEffect[nCntEffect].col.a--;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].aRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].aRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].aRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].aRadius, 0.0f);

			if (g_aEffect[nCntEffect].aRadius <= 0)	//�G�t�F�N�g�̔��a��0�ɂȂ�����
			{
				g_aEffect[nCntEffect].bUse = false;	//���g�p��Ԃɂ���
			}

		}
		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�f�[�^�A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//�e�N�X�`���̊J��
		if (g_pTextureEffect != NULL)
		{
			g_pTextureEffect->Release();
			g_pTextureEffect = NULL;
		}
		//���_�o�b�t�@�̊J��
		if (g_pVtxBuffEffect != NULL)
		{
			g_pVtxBuffEffect->Release();
			g_pVtxBuffEffect = NULL;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEffect;//�e�̃J�E���g

				   //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//�����_�[�X�e�[�g(���Z�����j
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//�����_�[�X�e�[�g(�ʏ�u�����h����)
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			//�e���g�p���Ȃ�`��
			// �`��̎��			�`��J�n���_	���`�悷�邩
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEffect, NUM_POLYGON);
		}
	}
}

//=============================================================================
//�G�t�F�N�g�̐ݒ�
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float aRadius, int nLife)
{
	VERTEX_2D *pVtx;//���_���̃|�C���^
	int nCntEffect;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].aRadius = aRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].bUse = true;//�g�p���Ă����Ԃɂ���

											  //���_�J���[�̍X�V
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].aRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].aRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].aRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].aRadius, 0.0f);

			break;
		}
		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�f�[�^�A�����b�N
	g_pVtxBuffEffect->Unlock();
}