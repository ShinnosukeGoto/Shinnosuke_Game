//=============================================================================
//
// �i�C���X�g�[���[�Y�̏��� [select.cpp] (����Ɩ��@�̑I��)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "input.h"
//#include "game.h"
#include "select.h"
#include "fade.h"
//#include "sound.h"
#include "manual.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureSelect[SELECT_UI] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelect = NULL;//���_�o�b�t�@�ւ̃|�C���^

CSELECT g_nCSelect;	//�L�����I��
WSELECT g_nWSelect;	//����I��
SSELECT g_nSSelect;	//���I��

					//=============================================================================
					// �Z���N�g����������
					//=============================================================================
void InitSelect(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�I���̏�����
	g_nCSelect = CSELECT_ART;	//�ŏ��̃J�[�\���̓A�[�T�[
	g_nWSelect = WSELECT_SWORD;	//�ŏ��̃J�[�\���͌�
	g_nSSelect = SSELECT_NO;	//�ŏ��̃J�[�\���͏��Ȃ�

								//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select999.png", &g_pTextureSelect[0]);	//�w�i
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select000.png", &g_pTextureSelect[1]);	//�Q�[��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select001.png", &g_pTextureSelect[2]);	//�V�ѕ�
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select002.png", &g_pTextureSelect[3]);	//�����L���O
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select003.png", &g_pTextureSelect[4]);	//�^�C�g���ɖ߂�

																						//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX*SELECT_UI,//�o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,//(�Œ�j
		FVF_VERTEX_2D,//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,//�i�Œ�j
		&g_pVtxBuffSelect,//�ϐ������ς��ƕύX���K�v
		NULL);
	VERTEX_2D*pVtx;//���_���̃|�C���^

				   //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

												   //���_���W�̐ݒ�i�E���Őݒ肷��j
												   //�Z���N�g�̔w�i
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	//�Q�[��
	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	//�V�ѕ�
	pVtx[8].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0.0f);
	//�����L���O
	pVtx[12].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT, 0.0f);
	//�^�C�g���ɖ߂�
	pVtx[16].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	pVtx[17].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0.0f);
	pVtx[18].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT, 0.0f);
	pVtx[19].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;
	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;
	pVtx[12].rhw = 1.0f;
	pVtx[13].rhw = 1.0f;
	pVtx[14].rhw = 1.0f;
	pVtx[15].rhw = 1.0f;
	pVtx[16].rhw = 1.0f;
	pVtx[17].rhw = 1.0f;
	pVtx[18].rhw = 1.0f;
	pVtx[19].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//�e�N�X�`���̍��W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[16].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^�A�����b�N
	g_pVtxBuffSelect->Unlock();
}
//=============================================================================
// �Z���N�g�X�V����
//=============================================================================
void UpdateSelect(void)
{
	//VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	//int nFade = GetFade();	//�t�F�[�h�̏����l��

	//						//���_�o�b�t�@�����b�N
	//g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	////���[�h�I��
	//switch (g_nMSelect)
	//{
	//case MSELECT_GAME:
	//	//�Q�[�������邢
	//	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	//���L�[�������ꂽ�i�E�ړ�,�V�ѕ��I���j
	//	if (GetKeyboardTrigger(DIK_RIGHT) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_MANUAL;
	//	}

	//	//���L�[�������ꂽ�i���ړ�,�����L���O�I���j
	//	if (GetKeyboardTrigger(DIK_DOWN) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_RANKING;
	//	}

	//	//Enter�L�[����������
	//	if (GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾���Ńv���X�G���^�[���̑�p
	//		SetFade(MODE_GAME);//�Q�[����ʂɈړ�
	//	}

	//	break;

	//case MSELECT_MANUAL:
	//	//�V�ѕ������邢
	//	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	//���L�[�������ꂽ�i���ړ�,�Q�[���I���j
	//	if (GetKeyboardTrigger(DIK_LEFT) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_GAME;
	//	}

	//	//���L�[�������ꂽ�i���ړ�,�^�C�g���ɖ߂�I���j
	//	if (GetKeyboardTrigger(DIK_DOWN) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_RETURN;
	//	}

	//	//Enter�L�[����������
	//	if (GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾���Ńv���X�G���^�[���̑�p
	//		SetFade(MODE_MANUAL);//�V�ѕ���ʂɈړ�
	//	}

	//	break;

	//case MSELECT_RANKING:
	//	//�����L���O�����邢
	//	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//	pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	//���L�[�������ꂽ�i�E�ړ�,�^�C�g���ɖ߂�I���j
	//	if (GetKeyboardTrigger(DIK_RIGHT) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_RETURN;
	//	}

	//	//���L�[�������ꂽ�i��ړ�,�Q�[���I���j
	//	if (GetKeyboardTrigger(DIK_UP) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_GAME;
	//	}

	//	//Enter�L�[����������
	//	if (GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾���Ńv���X�G���^�[���̑�p
	//		//SetFade(MODE_RANKING);//�����L���O��ʂɈړ�
	//	}

	//	break;

	//case MSELECT_RETURN:
	//	//�^�C�g���ɖ߂邪���邢
	//	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//	//���L�[�������ꂽ�i���ړ�,�����L���O�I���j
	//	if (GetKeyboardTrigger(DIK_LEFT) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_RANKING;
	//	}

	//	//���L�[�������ꂽ�i��ړ�,�V�ѕ��I���j
	//	if (GetKeyboardTrigger(DIK_UP) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_MANUAL;
	//	}

	//	//Enter�L�[����������
	//	if (GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾���Ńv���X�G���^�[���̑�p
	//		SetFade(MODE_TITLE);//�^�C�g����ʂɈړ�
	//	}

	//	break;
	//}

	////���_�f�[�^���A�����b�N����
	//g_pVtxBuffSelect->Unlock();

}

//=============================================================================
// �Z���N�g�I������
//=============================================================================
void UninitSelect(void)
{
	//�e�N�X�`���̊J��
	for (int nCntSelect = 0; nCntSelect < SELECT_UI; nCntSelect++)
	{
		if (g_pTextureSelect[nCntSelect] != NULL)
		{
			g_pTextureSelect[nCntSelect]->Release();
			g_pTextureSelect[nCntSelect] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffSelect != NULL)
	{
		g_pVtxBuffSelect->Release();
		g_pVtxBuffSelect = NULL;
	}
}
//=============================================================================
// �Z���N�g�`�揈��
//=============================================================================
void DrawSelect(void)
{
	//���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffSelect, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

	for (int nCntSelect = 0; nCntSelect < SELECT_UI; nCntSelect++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureSelect[nCntSelect]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntSelect, NUM_POLYGON);
	}
}

//=============================================================================
// �L�����I���̎擾
//=============================================================================
CSELECT GetCSelect(void)
{
	return g_nCSelect;
}

//=============================================================================
// ����I���̎擾
//=============================================================================
WSELECT GetWSelect(void)
{
	return g_nWSelect;
}

//=============================================================================
// ���I���̎擾
//=============================================================================
SSELECT GetSSelect(void)
{
	return g_nSSelect;
}