//=============================================================================
//
// �i�C���X�g�[���[�Y�̏��� [title.cpp] (�^�C�g��)
// Author:�㓡�T�V��
//
//=============================================================================
#include "title.h"
#include "input.h"		//�L�[���͏���
#include "bg.h"
#include "fade.h"
#include "game.h"
#include "sound.h"
#include "result.h"
#include "manual.h"
#include "select.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLE_UI] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//���_�o�b�t�@�ւ̃|�C���^
int g_nCounterTitle;//�A�j���[�V�����J�E���^�[
int g_nPatternEnter;//�A�j���[�V�����p�^�[��

int g_nEnter;	//�G���^�[�����������
int g_nEnter2;	//�G���^�[�����������
GAMELEVEL g_nLevel;	//���x���̏��

//=============================================================================
//�^�C�g������������
//=============================================================================
void InitTitle(void)
{
	//�ϐ��錾
	VERTEX_2D*pVtx;//���_���̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/title000.png", &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/press_enter.png", &g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/NORMAL000.png", &g_pTextureTitle[2]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/HARD000.png", &g_pTextureTitle[3]);

	//�O���[�o���ϐ��̏�����
	g_nCounterTitle = 0;
	g_nPatternEnter = 0;

	//�G���^�[�A���x���̏�����
	g_nEnter = 0;
	g_nEnter2 = 30;		//���̃G���^�[��������܂ł̎���
	g_nLevel = GAMELEVEL_NORMAL;

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * TITLE_UI,//�o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,//(�Œ�j
		FVF_VERTEX_2D,//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,//�i�Œ�j
		&g_pVtxBuffTitle,//�ϐ������ς��ƕύX���K�v
		NULL);

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

												  //���_���W�̐ݒ�i�E���Őݒ肷��j
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//�v���X�G���^�[�̈ʒu
	pVtx[4].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) - (PRESS_ENTER_Y / 2) + 146, 0.0f);
	pVtx[5].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) - (PRESS_ENTER_Y / 2) + 146, 0.0f);
	pVtx[6].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) + (PRESS_ENTER_Y / 2) + 146, 0.0f);
	pVtx[7].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) + (PRESS_ENTER_Y / 2) + 146, 0.0f);

	//NORMAL�̈ʒu
	pVtx[8].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (NORMAL_HARD_X / 2) - 300, (SCREEN_HEIGHT / 1.5) - (NORMAL_HARD_Y / 2) + 146, 0.0f);
	pVtx[9].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (NORMAL_HARD_X / 2) - 300, (SCREEN_HEIGHT / 1.5) - (NORMAL_HARD_Y / 2) + 146, 0.0f);
	pVtx[10].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (NORMAL_HARD_X / 2) - 300, (SCREEN_HEIGHT / 1.5) + (NORMAL_HARD_Y / 2) + 146, 0.0f);
	pVtx[11].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (NORMAL_HARD_X / 2) - 300, (SCREEN_HEIGHT / 1.5) + (NORMAL_HARD_Y / 2) + 146, 0.0f);

	//HARD�̈ʒu
	pVtx[12].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (NORMAL_HARD_X / 2) + 300, (SCREEN_HEIGHT / 1.5) - (NORMAL_HARD_Y / 2) + 146, 0.0f);
	pVtx[13].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (NORMAL_HARD_X / 2) + 300, (SCREEN_HEIGHT / 1.5) - (NORMAL_HARD_Y / 2) + 146, 0.0f);
	pVtx[14].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (NORMAL_HARD_X / 2) + 300, (SCREEN_HEIGHT / 1.5) + (NORMAL_HARD_Y / 2) + 146, 0.0f);
	pVtx[15].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (NORMAL_HARD_X / 2) + 300, (SCREEN_HEIGHT / 1.5) + (NORMAL_HARD_Y / 2) + 146, 0.0f);

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

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[8].col = D3DCOLOR_RGBA(0, 0, 0, 255);
	pVtx[9].col = D3DCOLOR_RGBA(0, 0, 0, 255);
	pVtx[10].col = D3DCOLOR_RGBA(0, 0, 0, 255);
	pVtx[11].col = D3DCOLOR_RGBA(0, 0, 0, 255);

	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 155);

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

	//���_�f�[�^�A�����b�N
	g_pVtxBuffTitle->Unlock();

	PlaySound(SOUND_LABEL_BGM000);	//�^�C�g����BGM
}
//=============================================================================
//�X�V����
//=============================================================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nFade = GetFade();	//�t�F�[�h�̏����l��

							//�_�ł���A�j���[�V����
	g_nCounterTitle++;
	if (g_nCounterTitle == 6)//����
	{
		g_nCounterTitle = 0;
		g_nPatternEnter++;
		if (g_nPatternEnter == 9)//�����x�̃p�^�[��
		{
			g_nPatternEnter = 0;
		}
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	////���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
	if (g_nPatternEnter == 0)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	if (g_nPatternEnter == 1)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 205);
	}

	if (g_nPatternEnter == 2)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	}

	if (g_nPatternEnter == 3)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 105);
	}

	if (g_nPatternEnter == 4)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	}

	if (g_nPatternEnter == 5)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 105);
	}

	if (g_nPatternEnter == 6)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	}

	if (g_nPatternEnter == 7)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 205);
	}

	if (g_nPatternEnter == 8)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//�m�[�}���I����
	if (g_nLevel == GAMELEVEL_NORMAL)
	{
		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	}

	//�n�[�h�I����
	if (g_nLevel == GAMELEVEL_HARD)
	{
		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffTitle->Unlock();

	//Enter�L�[�������O
	if (g_nEnter == 0)
	{
		if (GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_ENTER);	//�v���X�G���^�[��
			g_nEnter = 1;
		}
	}

	//��Փx�I��
	if (g_nEnter == 1)
	{
		//A�L�[�������ꂽ�i���ړ�,�m�[�}���j
		if (GetKeyboardTrigger(DIK_A) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_nLevel = GAMELEVEL_NORMAL;
		}
		//D�L�[�������ꂽ�i�E�ړ�,�n�[�h�j
		if (GetKeyboardTrigger(DIK_D) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_nLevel = GAMELEVEL_HARD;
		}
		//���L�[�������ꂽ�i���ړ�,�m�[�}���j
		if (GetKeyboardTrigger(DIK_LEFT) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_nLevel = GAMELEVEL_NORMAL;
		}
		//���L�[�������ꂽ�i�E�ړ�,�n�[�h�j
		if (GetKeyboardTrigger(DIK_RIGHT) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_nLevel = GAMELEVEL_HARD;
		}

		//���̃G���^�[��������܂ł̎���
		g_nEnter2--;
		if (g_nEnter2 <= 0)
		{
			g_nEnter2 = 0;
		}

		//Enter�L�[����������
		if (g_nEnter2 == 0 && GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_ENTER);	//�v���X�G���^�[��
			InitGameFirst();					//�Q�[���̏�����1
			InitSelect();						//�Z���N�g��ʂ̏�����
			SetFade(MODE_SELECT);				//�Z���N�g��ʂɈړ�
		}
	}
}
//=============================================================================
//�I������
//=============================================================================
void UninitTitle(void)
{
	//�e�N�X�`���̊J��
	for (int nCntTitle = 0; nCntTitle < TITLE_UI; nCntTitle++)
	{
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}
//=============================================================================
//�`�揈��
//=============================================================================
void DrawTitle(void)
{
	//���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitle[0]);
	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);

	//Enter�L�[�������O
	if (g_nEnter == 0)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[1]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 1, NUM_POLYGON);
	}

	//Enter�L�[����������
	if (g_nEnter == 1)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[2]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 2, NUM_POLYGON);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[3]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 3, NUM_POLYGON);
	}
}

//=============================================================================
// ��Փx�̎擾
//=============================================================================
GAMELEVEL GetTitle(void)
{
	return g_nLevel;
}