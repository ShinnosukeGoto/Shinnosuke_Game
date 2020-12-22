//=============================================================================
//
// �t�F�[�h�֌W�̏��� [fade.cpp]
// Author:�㓡�T�V��
//
//=============================================================================
#include "fade.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;//���_�o�b�t�@�ւ̃|�C���^

FADE g_Fade;			//�t�F�[�h���
MODE g_modeNext;		//���̉��
D3DXCOLOR g_colorFade;	//�t�F�[�h�F

//=============================================================================
// ����������
//=============================================================================
void InitFade(MODE modeNext)
{
	g_Fade = FADE_OUT;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_2D *pVtx;//���_���̃|�C���^

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/fade000.png", &g_pTextureFade);	//�X�R�A�̔w�i

					//�I�u�W�F�N�g�̒��_�o�b�t�@����
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,//(�Œ�j
		FVF_VERTEX_2D,//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,//�i�Œ�j
		&g_pVtxBuffFade,//�ϐ������ς��ƕύX���K�v
		NULL);
	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�i�E���Őݒ肷��j
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//���_�J���[
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;
	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^�A�����b�N
	g_pVtxBuffFade->Unlock();

}
//=============================================================================
//�I������
//=============================================================================
void UninitFade(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateFade(void)
{
	VERTEX_2D *pVtx;//���_���̃|�C���^

	if (g_Fade != FADE_NONE)
	{//�t�F�[�h�������̎�
		if (g_Fade == FADE_IN)
		{//�t�F�[�h�C��
			g_colorFade.a -= FADE_RATE;
			if (g_colorFade.a <= 0.0f)
			{
				g_colorFade.a = 0.0f;
				g_Fade = FADE_NONE;

				//SetMode(g_modeNext);
			}
		}

		if (g_Fade == FADE_OUT)
		{//�t�F�[�h
			g_colorFade.a += FADE_RATE;
			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_Fade = FADE_IN;

				SetMode(g_modeNext);//���[�h�̐ݒ�
			}
		}
	}
	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);
	//���_���(�J���[�̂ݍX�V)
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;
	//���_�f�[�^�A�����b�N
	g_pVtxBuffFade->Unlock();
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawFade(void)
{
	//���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureFade);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}
//=============================================================================
//�t�F�[�h�̐ݒ�
//=============================================================================
void SetFade(MODE modeNext)
{
	g_Fade = FADE_OUT;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
}
//=============================================================================
//�t�F�[�h��Ԃ̎擾
//=============================================================================
FADE GetFade(void)
{
	return g_Fade;
}