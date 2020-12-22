//=============================================================================
//
// �i�C���X�g�[���[�Y [effect2.cpp]�̏��� (�񕜂��i�W�����v�Ȃǂ̃G�t�F�N�g�j
// Author :�㓡�T�V��
//
//=============================================================================
#include "effect2.h"
#include "block.h"
#include "player.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureEffect2[EFFECTTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffEffect2 = NULL;
EFFECT2 g_aEffect2[MAX_EFFECT2];						//�G�t�F�N�g2�̏��

//=============================================================================
//�G�t�F�N�g2����������
//=============================================================================
void InitEffect2(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�G�t�F�N�g2���̏�����
	for (int nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
	{
		g_aEffect2[nCntEffect2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect2[nCntEffect2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect2[nCntEffect2].nCounterAnim = 0;
		g_aEffect2[nCntEffect2].nPatternAnim = 0;
		g_aEffect2[nCntEffect2].nSize = 0;
		g_aEffect2[nCntEffect2].bUse = false;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/effect000.png", &g_pTextureEffect2[0]);	//��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/effect001.png", &g_pTextureEffect2[1]);	//��i�W�����v
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/effect002.png", &g_pTextureEffect2[2]);	//����
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/effect003.png", &g_pTextureEffect2[3]);	//���n�̉�
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/effect004.png", &g_pTextureEffect2[4]);	//���@���w�i�֋z�����܂��

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_EFFECT2, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEffect2, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffEffect2->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

	//�G�t�F�N�g2��pVtx�𐶐�
	for (int nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
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
		pVtx[0].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.125f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.125f + 0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.125f + 0.125f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffEffect2->Unlock();
}

//=============================================================================
// �G�t�F�N�g2�I������
//=============================================================================
void UninitEffect2(void)
{
	//�e�N�X�`���̊J��
	for (int nCntEffect2 = 0; nCntEffect2 < EFFECTTYPE_MAX; nCntEffect2++)
	{
		if (g_pTextureEffect2[nCntEffect2] != NULL)
		{
			g_pTextureEffect2[nCntEffect2] -> Release();
			g_pTextureEffect2[nCntEffect2] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffEffect2 != NULL)
	{
		g_pVtxBuffEffect2->Release();
		g_pVtxBuffEffect2 = NULL;
	}
}

//=============================================================================
// �G�t�F�N�g2�X�V����
//=============================================================================
void UpdateEffect2(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEffect2;
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEffect2->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
	{
		if (g_aEffect2[nCntEffect2].bUse == true)
		{
			//��
			if (g_aEffect2[nCntEffect2].type == EFFECTTYPE_000)
			{
				//�G�t�F�N�g2�̃J�E���^�[��i�߂āA�p�^�[����؂�ւ���
				g_aEffect2[nCntEffect2].nCounterAnim++;
				if (g_aEffect2[nCntEffect2].nCounterAnim == 2)
				{
					g_aEffect2[nCntEffect2].nCounterAnim = 0;
					g_aEffect2[nCntEffect2].nPatternAnim++;
				}

				//�e�N�X�`�����W���X�V
				pVtx[0].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f + 0.2f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f + 0.2f, 1.0f);

				//�p�^�[�����Ō�܂ŏI�����
				if (g_aEffect2[nCntEffect2].nPatternAnim > 5)
				{
					g_aEffect2[nCntEffect2].bUse = false;
				}
			}

			//��i�W�����v
			if (g_aEffect2[nCntEffect2].type == EFFECTTYPE_001)
			{
				//�G�t�F�N�g2�̃J�E���^�[��i�߂āA�p�^�[����؂�ւ���
				g_aEffect2[nCntEffect2].nCounterAnim++;
				if (g_aEffect2[nCntEffect2].nCounterAnim == 2)
				{
					g_aEffect2[nCntEffect2].nCounterAnim = 0;
					g_aEffect2[nCntEffect2].nPatternAnim++;
				}

				//�e�N�X�`�����W���X�V
				pVtx[0].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f + 0.2f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f + 0.2f, 1.0f);

				//�p�^�[�����Ō�܂ŏI�����
				if (g_aEffect2[nCntEffect2].nPatternAnim > 5)
				{
					g_aEffect2[nCntEffect2].bUse = false;
				}
			}

			//����
			if (g_aEffect2[nCntEffect2].type == EFFECTTYPE_002)
			{
				//�G�t�F�N�g2�̃J�E���^�[��i�߂āA�p�^�[����؂�ւ���
				g_aEffect2[nCntEffect2].nCounterAnim++;
				if (g_aEffect2[nCntEffect2].nCounterAnim == 3)
				{
					g_aEffect2[nCntEffect2].nCounterAnim = 0;
					g_aEffect2[nCntEffect2].nPatternAnim++;
				}

				//�e�N�X�`�����W���X�V
				pVtx[0].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.1f + 0.1f, 1.0f);

				//�r���ł�������
				if (g_aEffect2[nCntEffect2].nPatternAnim == 60)
				{
					SetBlock(D3DXVECTOR3(g_aEffect2[nCntEffect2].pos.x, g_aEffect2[nCntEffect2].pos.y + 90,0.0f), 85, 170, BLOCKTYPE_006, 160);	//���(HP80)
				}

				//�p�^�[�����Ō�܂ŏI�����
				if (g_aEffect2[nCntEffect2].nPatternAnim > 60)
				{
					g_aEffect2[nCntEffect2].col.a -= 0.02;

					//�����x��0.1����������疢�g�p��
					if (g_aEffect2[nCntEffect2].col.a <= 0.1)
					{
						g_aEffect2[nCntEffect2].bUse = false;
					}
				}

				//���_�J���[�̍X�V
				pVtx[0].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, g_aEffect2[nCntEffect2].col.a);
				pVtx[1].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, g_aEffect2[nCntEffect2].col.a);
				pVtx[2].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, g_aEffect2[nCntEffect2].col.a);
				pVtx[3].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, g_aEffect2[nCntEffect2].col.a);
			}

			//���n�̉�
			if (g_aEffect2[nCntEffect2].type == EFFECTTYPE_003)
			{
				//�G�t�F�N�g2�̃J�E���^�[��i�߂āA�p�^�[����؂�ւ���
				g_aEffect2[nCntEffect2].nCounterAnim++;
				if (g_aEffect2[nCntEffect2].nCounterAnim == 2)
				{
					g_aEffect2[nCntEffect2].nCounterAnim = 0;
					g_aEffect2[nCntEffect2].nPatternAnim++;
				}

				//�e�N�X�`�����W���X�V
				pVtx[0].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f + 0.2f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f + 0.2f, 1.0f);

				//�p�^�[�����Ō�܂ŏI�����
				if (g_aEffect2[nCntEffect2].nPatternAnim > 4)
				{
					g_aEffect2[nCntEffect2].bUse = false;
				}
			}

			//���@���w�i�֋z�����܂��
			if (g_aEffect2[nCntEffect2].type == EFFECTTYPE_004)
			{
				//�G�t�F�N�g2�̃J�E���^�[��i�߂āA�p�^�[����؂�ւ���
				g_aEffect2[nCntEffect2].nCounterAnim++;
				if (g_aEffect2[nCntEffect2].nCounterAnim == 7)
				{
					g_aEffect2[nCntEffect2].nCounterAnim = 0;
					g_aEffect2[nCntEffect2].nPatternAnim++;
				}

				//�e�N�X�`�����W���X�V
				pVtx[0].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.0625f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.0625f + 0.0625f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.0625f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.0625f + 0.0625f, 1.0f);

				//���_���W
				pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 50, 0, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 50, 0, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 2, 0.0f);

				//���_�J���[�̍X�V
				pVtx[0].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, 1.0f);
				pVtx[1].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, 1.0f);
				pVtx[2].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, 1.0f);
				pVtx[3].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, 1.0f);

				//�p�^�[�����Ō�܂ŏI�����
				if (g_aEffect2[nCntEffect2].nPatternAnim > 15)
				{
					g_aEffect2[nCntEffect2].bUse = false;
					HitPlayer(1000);
				}
			}
		}
		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffEffect2->Unlock();
}

//=============================================================================
// �G�t�F�N�g2�`�揈��
//=============================================================================
void DrawEffect2(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEffect2;

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffEffect2, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�|���S���̕`��
	for (nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
	{
		if (g_aEffect2[nCntEffect2].bUse == true)	//�G�t�F�N�g2���g�p���Ȃ�`��
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect2[g_aEffect2[nCntEffect2].type]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect2 * 4, 2);
		}
	}
}

//=============================================================================
// �G�t�F�N�g2�ݒ菈��
//=============================================================================
void SetEffect2(D3DXVECTOR3 pos, D3DXCOLOR col, int nSize, EFFECTTYPE type)
{
	//�ϐ��錾
	int nCntEffect2;
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEffect2->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
	{
		if (g_aEffect2[nCntEffect2].bUse == false)
		{
			g_aEffect2[nCntEffect2].pos = pos;
			g_aEffect2[nCntEffect2].nCounterAnim = 0;
			g_aEffect2[nCntEffect2].nPatternAnim = 0;
			g_aEffect2[nCntEffect2].nSize = nSize;
			g_aEffect2[nCntEffect2].type = type;

			//���_���̍X�V
			//���_���W
			pVtx[0].pos = D3DXVECTOR3(g_aEffect2[nCntEffect2].pos.x - (g_aEffect2[nCntEffect2].nSize / 2), g_aEffect2[nCntEffect2].pos.y - (g_aEffect2[nCntEffect2].nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect2[nCntEffect2].pos.x + (g_aEffect2[nCntEffect2].nSize / 2), g_aEffect2[nCntEffect2].pos.y - (g_aEffect2[nCntEffect2].nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect2[nCntEffect2].pos.x - (g_aEffect2[nCntEffect2].nSize / 2), g_aEffect2[nCntEffect2].pos.y + (g_aEffect2[nCntEffect2].nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect2[nCntEffect2].pos.x + (g_aEffect2[nCntEffect2].nSize / 2), g_aEffect2[nCntEffect2].pos.y + (g_aEffect2[nCntEffect2].nSize / 2), 0.0f);

			g_aEffect2[nCntEffect2].col = col;

			g_aEffect2[nCntEffect2].bUse = true;

			break;
		}
		pVtx += 4;
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffEffect2->Unlock();
}