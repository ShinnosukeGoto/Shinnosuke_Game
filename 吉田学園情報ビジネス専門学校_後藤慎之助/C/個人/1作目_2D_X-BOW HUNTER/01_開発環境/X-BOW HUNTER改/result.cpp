//=============================================================================
//
// X-Bow Hunter�̏��� [result.cpp] (�Q�[���N���A���Q�[���I�[�o�[)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "result.h"
#include "hunter.h"
#include "enemy.h"
#include "input.h"
#include "game.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "title.h"
#include "result2.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureResult[3] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;//���_�o�b�t�@�ւ̃|�C���^

int g_nHuckerState;		//P�L�[�������ꂽ��
						//=============================================================================
						// ����������
						//=============================================================================
void InitResult(void)
{
	//�ϐ��錾
	VERTEX_2D*pVtx;//���_���̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * 3,//�o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,//(�Œ�j
		FVF_VERTEX_2D,//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,//�i�Œ�j
		&g_pVtxBuffResult,//�ϐ������ς��ƕύX���K�v
		NULL);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "gameclear000.png", &g_pTextureResult[0]);	//�Q�[���N���A
	D3DXCreateTextureFromFile(pDevice, "gameclear001.png", &g_pTextureResult[1]);	//�Q�[���N���A�i�n�b�J�[�j
	D3DXCreateTextureFromFile(pDevice, "gameover000.png", &g_pTextureResult[2]);	//�Q�[���I�[�o�[

	//P���������󋵂̏�����
	g_nHuckerState = AREYOUHUCKER�H_NO;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

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

	//���_�f�[�^�A�����b�N
	g_pVtxBuffResult->Unlock();
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	int nFade = GetFade();	//�t�F�[�h�̏����l��

	//Enter�L�[����������
	if (GetKeyboardPress(DIK_RETURN) && nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_ENTER);	//�v���X�G���^�[��
		InitResult2();		//���U���g2��������
		SetFade(MODE_RESULT2);//���U���g2��ʂɈړ�
	}

}
//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureResult != NULL)
	{
		for (int nCntResult = 0; nCntResult < 3; nCntResult++)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	//�n���^�[�̏󋵂��擾
	HUNTER *pHunter = GetHunter();

	//P�{�^���̎g�p�󋵂��擾
	g_nHuckerState = GetGameState();

	//���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

								   //�n���^�[���������Ă���Ȃ�A�Q�[���N���A
	if (pHunter->bDisp == true)
	{
		if (g_nHuckerState == AREYOUHUCKER�H_NO)	//�f�o�b�N�R�}���h�g�p�Ȃ�
		{
			PlaySound(SOUND_LABEL_BGM003);	//���U���g��
											//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResult[0]);
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
		}

		if (g_nHuckerState == AREYOUHUCKER�H_YES)	//�f�o�b�N�R�}���h�g�p����
		{
			PlaySound(SOUND_LABEL_BGM002);	//�n�b�J�[��
											//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResult[1]);
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
		}
	}

	//�n���^�[���S�ł��Ă���Ȃ�Q�[���I�[�o�[
	if (pHunter->bDisp == false)
	{
		if (g_nHuckerState == AREYOUHUCKER�H_NO)	//�f�o�b�N�R�}���h�g�p�Ȃ�
		{
			PlaySound(SOUND_LABEL_BGM003);	//���U���g��
											//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResult[2]);
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
		}

		if (g_nHuckerState == AREYOUHUCKER�H_YES)	//�f�o�b�N�R�}���h�g�p����
		{
			PlaySound(SOUND_LABEL_BGM002);	//�n�b�J�[��
											//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResult[2]);
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
		}
	}
}