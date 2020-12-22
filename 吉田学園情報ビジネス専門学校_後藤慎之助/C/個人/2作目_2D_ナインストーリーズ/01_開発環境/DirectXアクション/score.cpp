//=============================================================================
//
// �i�C���X�g�[���[�Y�̏��� [score.cpp] (�X�R�A)
// Author:�㓡�T�V��
//
//=============================================================================
#include "score.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;//���_�o�b�t�@�ւ̃|�C���^

int g_nScore;//�X�R�A�̏��

			 //=============================================================================
			 // ����������
			 //=============================================================================
void InitScore(void)
{
	//�ϐ��錾
	VERTEX_2D*pVtx;//���_���̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�X�R�A�̏�����
	g_nScore = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/number000.png", &g_pTextureScore);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * MAX_SCORE,//�o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,//(�Œ�j
		FVF_VERTEX_2D,//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,//�i�Œ�j
		&g_pVtxBuffScore,//�ϐ������ς��ƕύX���K�v
		NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

	for (int nScore = 0; nScore < MAX_SCORE; nScore++)
	{
		//���_���W�̐ݒ�i�E���Őݒ肷��j
		pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) - (SCORE_SIZE_X / 2) - 90, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) - (SCORE_SIZE_Y / 2) - 640, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) + (SCORE_SIZE_X / 2) - 90, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) - (SCORE_SIZE_Y / 2) - 640, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) - (SCORE_SIZE_X / 2) - 90, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) + (SCORE_SIZE_Y / 2) - 640, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) + (SCORE_SIZE_X / 2) - 90, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) + (SCORE_SIZE_Y / 2) - 640, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(33, 128, 221, 255);
		pVtx[1].col = D3DCOLOR_RGBA(33, 128, 221, 255);
		pVtx[2].col = D3DCOLOR_RGBA(33, 128, 221, 255);
		pVtx[3].col = D3DCOLOR_RGBA(33, 128, 221, 255);

		//�e�N�X�`���̍��W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�f�[�^�A�����b�N
	g_pVtxBuffScore->Unlock();

}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateScore(void)
{

}
//=============================================================================
// �I������
//=============================================================================
void UninitScore(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{

	//���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

								   //�X�R�A�̐����̕`��
	for (int nScore = 0; nScore < MAX_SCORE; nScore++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureScore);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nScore, NUM_POLYGON);
	}

}
//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
void ScoreEffect(int score)
{
	g_nScore += score;
	//�ϐ��錾
	VERTEX_2D*pVtx;//���_���̃|�C���^
	float fIndex;		//�w���@(�X�R�A�\���p)
	float fRadix = 10.0f;	//��@(�X�R�A�\���p)

							//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

												  //�X�R�A��0�_�������Ȃ��悤�ɂ���
	if (g_nScore < 0)
	{
		g_nScore = 0;
	}

	for (fIndex = 0; fIndex < MAX_SCORE; fIndex++)
	{
		int nScore = (int)powf(fRadix, MAX_SCORE - fIndex);
		int nScore2 = (int)powf(fRadix, MAX_SCORE - fIndex - 1);
		int nAnswer = g_nScore % nScore / nScore2;

		//�e�N�X�`���̍��W
		pVtx[28].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
		pVtx[29].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
		pVtx[30].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
		pVtx[31].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

		pVtx -= 4;//���_�f�[�^�̃|�C���^��4���i�߂�

	}
	//���_�f�[�^�A�����b�N
	g_pVtxBuffScore->Unlock();
}
//=============================================================================
// �X�R�A�̎擾
//=============================================================================
int GetScore(void)
{
	return g_nScore;
}