//=============================================================================
//
// X-Bow Hunter�̏��� [result2.cpp] (�����L���O)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "result2.h"
#include "input.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "title.h"
#include "score.h"
#include "game.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureResult2[7] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult2 = NULL;//���_�o�b�t�@�ւ̃|�C���^

int g_nHuckerState2;	//P�L�[�������ꂽ��
int g_nResultScore;		//�ŏI�X�R�A

						//=============================================================================
						// ���U���g2����������
						//=============================================================================
void InitResult2(void)
{
	//�ϐ��錾
	VERTEX_2D*pVtx;//���_���̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//P���������󋵂̏�����
	g_nHuckerState2 = AREYOUHUCKER�H_NO;

	//���U���g�p�̃X�R�A��������
	g_nResultScore = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "gameend002.png", &g_pTextureResult2[0]);	//���������n���^�[
	D3DXCreateTextureFromFile(pDevice, "number000.png", &g_pTextureResult2[1]);		//�i���o�[
	D3DXCreateTextureFromFile(pDevice, "gameend003.png", &g_pTextureResult2[2]);	//��l�O�n���^�[
	D3DXCreateTextureFromFile(pDevice, "gameend004.png", &g_pTextureResult2[3]);	//�x�e�����n���^�[
	D3DXCreateTextureFromFile(pDevice, "gameend001.png", &g_pTextureResult2[4]);	//�W�F�m�T�C�h�n���^�[
	D3DXCreateTextureFromFile(pDevice, "gameend000.png", &g_pTextureResult2[5]);	//�n�b�J�[�G���f�B���O
	D3DXCreateTextureFromFile(pDevice, "gameend005.png", &g_pTextureResult2[6]);	//�O���n���^�[

																					//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * 9,//�o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,//(�Œ�j
		FVF_VERTEX_2D,//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,//�i�Œ�j
		&g_pVtxBuffResult2,//�ϐ������ς��ƕύX���K�v
		NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffResult2->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

													//���_���W�̐ݒ�i�E���Őݒ肷��j
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`���̍��W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int nResultScore = 0; nResultScore < MAX_SCORE; nResultScore++)
	{
		//���_���W�̐ݒ�i�E���Őݒ肷��j
		pVtx[4].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RESULT2_SIZE_X + RESULT2_SIZE_X / 2) - (RESULT2_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RESULT2_SIZE_Y / 2) - (RESULT2_SIZE_Y / 2) - 400, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RESULT2_SIZE_X + RESULT2_SIZE_X / 2) + (RESULT2_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RESULT2_SIZE_Y / 2) - (RESULT2_SIZE_Y / 2) - 400, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RESULT2_SIZE_X + RESULT2_SIZE_X / 2) - (RESULT2_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RESULT2_SIZE_Y / 2) + (RESULT2_SIZE_Y / 2) - 400, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RESULT2_SIZE_X + RESULT2_SIZE_X / 2) + (RESULT2_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RESULT2_SIZE_Y / 2) + (RESULT2_SIZE_Y / 2) - 400, 0.0f);

		//rhw�̐ݒ�
		pVtx[4].rhw = 1.0f;
		pVtx[5].rhw = 1.0f;
		pVtx[6].rhw = 1.0f;
		pVtx[7].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[4].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[5].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[6].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[7].col = D3DCOLOR_RGBA(0, 0, 0, 255);

		//�e�N�X�`���̍��W
		pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�f�[�^�A�����b�N
	g_pVtxBuffResult2->Unlock();
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult2(void)
{
	//�ϐ��錾
	VERTEX_2D*pVtx;//���_���̃|�C���^
	float fIndex;		//�w���@(�X�R�A�\���p)
	float fRadix = 10.0f;	//��@(�X�R�A�\���p)
	g_nResultScore = GetScore();	//�X�R�A�����U���g�ɔ��f
	int nFade = GetFade();	//�t�F�[�h�̏����l��


									//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffResult2->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

	for (fIndex = 0; fIndex < MAX_SCORE; fIndex++)
	{
		int nResultScore = (int)powf(fRadix, MAX_SCORE - fIndex);
		int nResultScore2 = (int)powf(fRadix, MAX_SCORE - fIndex - 1);
		int nAnswer = g_nResultScore % nResultScore / nResultScore2;

		//�e�N�X�`���̍��W
		pVtx[32].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
		pVtx[33].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
		pVtx[34].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
		pVtx[35].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

		pVtx -= 4;//���_�f�[�^�̃|�C���^��4���i�߂�

	}
	//���_�f�[�^�A�����b�N
	g_pVtxBuffResult2->Unlock();

	//Enter�L�[����������
	if (GetKeyboardPress(DIK_RETURN) && nFade == FADE_NONE)
	{
		StopSound(SOUND_LABEL_BGM002);
		StopSound(SOUND_LABEL_BGM003);
		PlaySound(SOUND_LABEL_SE_ENTER);	//�v���X�G���^�[��
		InitTitle();		//�^�C�g����������
		SetFade(MODE_TITLE);//�^�C�g����ʂɈړ�
		PlaySound(SOUND_LABEL_BGM000);	//�^�C�g����BGM
	}
}
//=============================================================================
// �I������
//=============================================================================
void UninitResult2(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureResult2 != NULL)
	{
		for (int nCntResultScore = 0; nCntResultScore < 7; nCntResultScore++)
		{
			g_pTextureResult2[nCntResultScore]->Release();
			g_pTextureResult2[nCntResultScore] = NULL;
		}

	}
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffResult2 != NULL)
	{
		g_pVtxBuffResult2->Release();
		g_pVtxBuffResult2 = NULL;
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult2(void)
{
	g_nResultScore = GetScore();	//�X�R�A�����U���g�ɔ��f

									//P�{�^���̎g�p�󋵂��擾
	g_nHuckerState2 = GetGameState();

	//���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffResult2, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

								   //���U���g2�̔w�i
	if (g_nHuckerState2 == AREYOUHUCKER�H_NO)	//�f�o�b�N�R�}���h�g�p�Ȃ�
	{
		//�X�R�A0�`19999pt
		if (g_nResultScore >= 0 && g_nResultScore < 20000)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResult2[0]);
			//���U���g�w�i�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
		}

		//�X�R�A20000�`49999pt
		if (g_nResultScore >= 20000 && g_nResultScore < 50000)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResult2[6]);
			//���U���g�w�i�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
		}

		//�X�R�A50000�`79999pt
		if (g_nResultScore >= 50000 && g_nResultScore < 80000)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResult2[2]);
			//���U���g�w�i�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
		}

		//�X�R�A80000�`99999pt
		if (g_nResultScore >= 80000 && g_nResultScore < 100000)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResult2[3]);
			//���U���g�w�i�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
		}

		//�X�R�A100000pt�`
		if (g_nResultScore >= 100000)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResult2[4]);
			//���U���g�w�i�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
		}
	}

	if (g_nHuckerState2 == AREYOUHUCKER�H_YES)	//�f�o�b�N�R�}���h�g�p����
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureResult2[5]);
		//���U���g�w�i�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
	}

	//�X�R�A�̐����̕`��
	for (int nResultScore = 1; nResultScore < MAX_SCORE + 1; nResultScore++)	//pVtx0�`3��ǂݍ��܂Ȃ��悤�ɂ��邽�߁A1����for���n�߂�
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureResult2[1]);
		//�����̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nResultScore, NUM_POLYGON);
	}

}