//=============================================================================
//
// �i�C���X�g�[���[�Y�̏��� [mpbar.cpp] (MP�o�[)
// Author :�㓡�T�V��
//
//=============================================================================
#include "mpbar.h"
#include "player.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureMpbar[2] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffMpbar = NULL;

int g_nCounterMpbar;
int g_nPatternMpbar;

//=============================================================================
//MP�o�[����������
//=============================================================================
void InitMpbar(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER*pPlayer = GetPlayer();	//�v���C���[��MP���擾

									//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar000.png", &g_pTextureMpbar[0]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar003.png", &g_pTextureMpbar[1]);

	//�O���[�o���ϐ��̏�����
	g_nCounterMpbar = 0;
	g_nPatternMpbar = 0;

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX * 2, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffMpbar, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffMpbar->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

	//MP���������ꍇ�A�摜�̉E������`�����߂�
	pVtx[0].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(70 + 320 * ((pPlayer->nMp / PLAYER_MP)), (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(70 + 320 * ((pPlayer->nMp / PLAYER_MP)), (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635, 0.0f);

	//MP�o�[�̔w�i
	pVtx[4].pos = D3DXVECTOR3(70.0f - MPBAR_EXTEND, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635.0f - MPBAR_EXTEND, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(70.0f + 320.0f + MPBAR_EXTEND, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635.0f - MPBAR_EXTEND, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(70.0f - MPBAR_EXTEND, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635.0f + MPBAR_EXTEND, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(70.0f + 320.0f + MPBAR_EXTEND, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635.0f + MPBAR_EXTEND, 0.0f);

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
	pVtx[0].col = D3DCOLOR_RGBA(8, 123, 202, 255);
	pVtx[1].col = D3DCOLOR_RGBA(8, 123, 202, 255);
	pVtx[2].col = D3DCOLOR_RGBA(8, 123, 202, 255);
	pVtx[3].col = D3DCOLOR_RGBA(8, 123, 202, 255);

	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((pPlayer->nMp) * (1 / PLAYER_MP), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((pPlayer->nMp) * (1 / PLAYER_MP), 1.0f);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffMpbar->Unlock();
}

//=============================================================================
// MP�o�[�I������
//=============================================================================
void UninitMpbar(void)
{
	//�e�N�X�`���̊J��
	for (int nCntMpbar = 0; nCntMpbar < 2; nCntMpbar++)
	{
		if (g_pTextureMpbar[nCntMpbar] != NULL)
		{
			g_pTextureMpbar[nCntMpbar]->Release();
			g_pTextureMpbar[nCntMpbar] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffMpbar != NULL)
	{
		g_pVtxBuffMpbar->Release();
		g_pVtxBuffMpbar = NULL;
	}
}

//=============================================================================
// MP�o�[�X�V����
//=============================================================================
void UpdateMpbar(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	PLAYER*pPlayer = GetPlayer();	//�v���C���[��MP���擾

									//���_�o�b�t�@�����b�N
	g_pVtxBuffMpbar->Lock(0, 0, (void**)&pVtx, 0);

	//MP��101�ȏ�ɂȂ�Ȃ��悤�ɂ���
	if (pPlayer->nMp > PLAYER_MP)
	{
		pPlayer->nMp = PLAYER_MP;
	}

	//���Ԃ̊T�O���l��
	if (pPlayer->bDisp == true)
	{
		g_nCounterMpbar++;	//�J�E���^���Z
		if (g_nCounterMpbar == 29)	//0.5�b��1��
		{
			g_nCounterMpbar = 0;
			g_nPatternMpbar++;
			if (g_nPatternMpbar == 1)
			{
				g_nPatternMpbar = 0;

				//MP���ő�l�łȂ��Ȃ�A1MP��
				if (pPlayer->nMp < PLAYER_MP)
				{
					pPlayer->nMp += 1;
				}
			}
		}
	}

	//MP���������ꍇ�A�摜�̉E������`�����߂�
	if (pPlayer->bDisp == true)
	{
		pVtx[0].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(70 + 320 * ((pPlayer->nMp / PLAYER_MP)), (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(70 + 320 * ((pPlayer->nMp / PLAYER_MP)), (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635, 0.0f);
	}

	//HP���Ȃ��Ȃ����ꍇ�A�摜��\�������Ȃ�
	if (pPlayer->bDisp == false)
	{
		pVtx[0].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(70 + 320 * (0), (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(70 + 320 * (0), (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635, 0.0f);
	}

	//���I����
	if (pPlayer->Mtype == MSELECT_FIRE)
	{
		if (pPlayer->nMp < FIRE_MP)
		{
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		}
		else
		{
			pVtx[0].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[1].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[2].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[3].col = D3DCOLOR_RGBA(8, 123, 202, 255);
		}
	}

	//���I����
	if (pPlayer->Mtype == MSELECT_LIGHTNING)
	{
		if (pPlayer->nMp < LIGHTNING_MP)
		{
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		}
		else
		{
			pVtx[0].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[1].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[2].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[3].col = D3DCOLOR_RGBA(8, 123, 202, 255);
		}
	}

	//�z���I����
	if (pPlayer->Mtype == MSELECT_DRAIN)
	{
		if (pPlayer->nMp < DRAIN_MP)
		{
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		}
		else
		{
			pVtx[0].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[1].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[2].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[3].col = D3DCOLOR_RGBA(8, 123, 202, 255);
		}
	}

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((pPlayer->nMp / PLAYER_MP), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((pPlayer->nMp / PLAYER_MP), 1.0f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffMpbar->Unlock();

}

//=============================================================================
// MP�o�[�`�揈��
//=============================================================================
void DrawMpbar(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER*pPlayer = GetPlayer();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffMpbar, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntMpbar = 1; nCntMpbar >-1; nCntMpbar--)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureMpbar[nCntMpbar]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntMpbar, NUM_POLYGON);
	}

}