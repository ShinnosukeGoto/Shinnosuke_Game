//=============================================================================
//
// PLASTIC TROOPERS�̏��� [title.cpp] (�^�C�g��)
// Author:�㓡�T�V��
//
//=============================================================================
#include "title.h"
#include "input.h"		//�L�[���͏���
#include "fade.h"
#include "campaign.h"
#include "sound.h"
#include "result.h"
#include "manual.h"
#include "select.h"
#include "controller.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLE_UI] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//���_�o�b�t�@�ւ̃|�C���^
int g_nCounterTitle;//�A�j���[�V�����J�E���^�[
int g_nPatternEnter;//�A�j���[�V�����p�^�[��

int g_nEnter;	//�G���^�[�����������
int g_nEnter2;	//�G���^�[�����������
int g_nCntUpDown;//�㉺�ɃJ�[�\���𓮂�����܂ł̎���
bool g_bUpDown; //�㉺�ɃJ�[�\���𓮂�����
MODE g_nMode;	//���[�h�̏��
LEVEL g_nLevel;		//�L�����y�[���̓�Փx
SURVIVALTIME g_nSurvivalTime;	//�T�o�C�o������

//=============================================================================
//�^�C�g������������
//=============================================================================
void InitTitle(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx = NULL;//���_���̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/title000.png", &g_pTextureTitle[0]);			//�^�C�g���̔w�i
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/PRESS_ANY_BUTTON.png", &g_pTextureTitle[1]);	//�v���X�{�^��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/CAMPAIGN.png", &g_pTextureTitle[2]);			//�L�����y�[��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/�͂Ă�.png", &g_pTextureTitle[3]);			//�T�o�C�o��
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/�͂Ă�.png", &g_pTextureTitle[4]);				//�o�[�T�X
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/�͂Ă�.png", &g_pTextureTitle[5]);			//�v���N�e�B�X
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/RANKING.png", &g_pTextureTitle[6]);				//�����L���O
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/EXIT.png", &g_pTextureTitle[7]);				//EXIT

	//�O���[�o���ϐ��̏�����
	g_nCounterTitle = 0;
	g_nPatternEnter = 0;
    g_nCntUpDown = 0;
    g_bUpDown = true;

	//�G���^�[�A���x���̏�����
	g_nEnter = 0;
	g_nEnter2 = 30;			//���̃G���^�[��������܂ł̎���
	g_nMode = MODE_CAMPAIGN;	//�ŏ��̃J�[�\���́A�L�����y�[��
	g_nLevel = LEVEL_NORMAL;	//�ŏ��̃J�[�\���̓m�[�}��
	g_nSurvivalTime = SURVIVALTIME_1m;	//�ŏ��̃J�[�\����1��

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
	//�^�C�g���̔w�i
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//�v���X�{�^��
	pVtx[4].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) - (PRESS_ENTER_Y / 2) + 146, 0.0f);
	pVtx[5].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) - (PRESS_ENTER_Y / 2) + 146, 0.0f);
	pVtx[6].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) + (PRESS_ENTER_Y / 2) + 146, 0.0f);
	pVtx[7].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) + (PRESS_ENTER_Y / 2) + 146, 0.0f);

	//�L�����y�[��
	pVtx[8].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 0.0f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 120.0f, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(SCREEN_WIDTH, 120.0f, 0.0f);

	//�T�o�C�o��
	pVtx[12].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 120.0f, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(SCREEN_WIDTH, 120.0f, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 240.0f, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(SCREEN_WIDTH, 240.0f, 0.0f);

	//�o�[�T�X
	pVtx[16].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 240.0f, 0.0f);
	pVtx[17].pos = D3DXVECTOR3(SCREEN_WIDTH, 240.0f, 0.0f);
	pVtx[18].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 360.0f, 0.0f);
	pVtx[19].pos = D3DXVECTOR3(SCREEN_WIDTH, 360.0f, 0.0f);

	//�v���N�e�B�X
	pVtx[20].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 360.0f, 0.0f);
	pVtx[21].pos = D3DXVECTOR3(SCREEN_WIDTH, 360.0f, 0.0f);
	pVtx[22].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 480.0f, 0.0f);
	pVtx[23].pos = D3DXVECTOR3(SCREEN_WIDTH, 480.0f, 0.0f);

	//�����L���O
	pVtx[24].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 480.0f, 0.0f);
	pVtx[25].pos = D3DXVECTOR3(SCREEN_WIDTH, 480.0f, 0.0f);
	pVtx[26].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 600.0f, 0.0f);
	pVtx[27].pos = D3DXVECTOR3(SCREEN_WIDTH, 600.0f, 0.0f);

	//EXIT
	pVtx[28].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 600.0f, 0.0f);
	pVtx[29].pos = D3DXVECTOR3(SCREEN_WIDTH, 600.0f, 0.0f);
	pVtx[30].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 720.0f, 0.0f);
	pVtx[31].pos = D3DXVECTOR3(SCREEN_WIDTH, 720.0f, 0.0f);

	//�e�틤�ʂ̐ݒ�
	for (int nCntTitle = 0; nCntTitle < TITLE_UI; nCntTitle++)
	{
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

		//�e�N�X�`���̍��W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�f�[�^�A�����b�N
	g_pVtxBuffTitle->Unlock();

    srand((unsigned int)time(0));	//�����̎�̏�����

	PlaySound(SOUND_LABEL_BGM000);	//�^�C�g����BGM
}

//=============================================================================
//�^�C�g���X�V����
//=============================================================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx = NULL;	//���_���ւ̃|�C���^
    DIJOYSTATE2 Controller = GetController();  //�R���g���[���[�̏����擾
	int nFade = GetFade();	//�t�F�[�h�̏����擾

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

	//Enter�L�[�������O
	if (g_nEnter == 0)
	{
        if (nFade == FADE_NONE)
        {
            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(BUTTON_A) || GetJoypadTrigger(BUTTON_B) ||
                GetJoypadTrigger(BUTTON_X) || GetJoypadTrigger(BUTTON_Y) || GetJoypadTrigger(BUTTON_START))
            {
                //PlaySound(SOUND_LABEL_SE_ENTER);	//�v���X�G���^�[��
                g_nEnter = 1;
            }
        }
	}

	//���[�h�I���i�I�����Ă�����̈ȊO�A�����x��������j
	if (g_nEnter == 1)
	{
        //�\���L�[��X�e�B�b�N�ŃJ�[�\���𓮂�����܂ł̃J�E���^�[
        if (g_bUpDown == false)
        {
            g_nCntUpDown++;
            if (g_nCntUpDown > 12)
            {
                g_nCntUpDown = 0;
                g_bUpDown = true;   //�A�b�v�_�E���\
            }
        }

		switch (g_nMode)
		{
		case MODE_CAMPAIGN:
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			for (int nCntTitle = 0; nCntTitle < 5; nCntTitle++)
			{
				pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

				pVtx += 4;
			}

            if (nFade == FADE_NONE)
            {
                //���������ꂽ�i���ړ��j
                if (GetKeyboardTrigger(DIK_DOWN) || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bUpDown == true 
                    || Controller.lY > 0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_SURVIVAL;
                    g_bUpDown = false;
                }

                //Enter�L�[����������
                if (g_nEnter2 == 0)
                {
                    if (GetJoypadTrigger(BUTTON_A) || GetJoypadTrigger(BUTTON_B) || GetKeyboardTrigger(DIK_RETURN))
                    {
                        SetFade(MODE_MANUAL);
                    }
                }
            }

			break;

		case MODE_SURVIVAL:
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			for (int nCntTitle = 0; nCntTitle < 4; nCntTitle++)
			{
				pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

				pVtx += 4;
			}

            if (nFade == FADE_NONE)
            {
                //���������ꂽ�i��ړ��j
                if (GetKeyboardTrigger(DIK_UP) || Controller.rgdwPOV[0] == BUTTON_UP && g_bUpDown == true 
                    || Controller.lY <0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_CAMPAIGN;
                    g_bUpDown = false;
                }

                //���������ꂽ�i���ړ��j
                else if (GetKeyboardTrigger(DIK_DOWN) || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bUpDown == true 
                    || Controller.lY >0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_VERSUS;
                    g_bUpDown = false;
                }

			}

			break;

		case MODE_VERSUS:
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			for (int nCntTitle = 0; nCntTitle < 3; nCntTitle++)
			{
				pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 55);

				pVtx += 4;
			}

            if (nFade == FADE_NONE)
            {
                //���������ꂽ�i��ړ��j
                if (GetKeyboardTrigger(DIK_UP) || Controller.rgdwPOV[0] == BUTTON_UP && g_bUpDown == true 
                    || Controller.lY <0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_SURVIVAL;
                    g_bUpDown = false;
                }

                //���������ꂽ�i���ړ��j
                else if (GetKeyboardTrigger(DIK_DOWN) || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bUpDown == true 
                    || Controller.lY >0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_PRACTICE;
                    g_bUpDown = false;
                }

            }

			break;

		case MODE_PRACTICE:
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

			pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			for (int nCntTitle = 0; nCntTitle < 2; nCntTitle++)
			{
				pVtx[24].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[25].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[26].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[27].col = D3DCOLOR_RGBA(255, 255, 255, 55);

				pVtx += 4;
			}

            if (nFade == FADE_NONE)
            {
                //���������ꂽ�i��ړ��j
                if (GetKeyboardTrigger(DIK_UP) || Controller.rgdwPOV[0] == BUTTON_UP && g_bUpDown == true 
                    || Controller.lY <0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_VERSUS;
                    g_bUpDown = false;
                }

                //���������ꂽ�i���ړ��j
                else if (GetKeyboardTrigger(DIK_DOWN) || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bUpDown == true 
                    || Controller.lY >0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_RANKING;
                    g_bUpDown = false;
                }

            }

			break;

		case MODE_RANKING:
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

			pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[24].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[25].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[26].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[27].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			pVtx[28].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[29].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[30].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[31].col = D3DCOLOR_RGBA(255, 255, 255, 55);

            if (nFade == FADE_NONE)
            {
                //���������ꂽ�i��ړ��j
                if (GetKeyboardTrigger(DIK_UP) || Controller.rgdwPOV[0] == BUTTON_UP && g_bUpDown == true 
                    || Controller.lY <0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_PRACTICE;
                    g_bUpDown = false;
                }

                //���������ꂽ�i���ړ��j
                else if (GetKeyboardTrigger(DIK_DOWN) || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bUpDown == true 
                    || Controller.lY >0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_EXIT;
                    g_bUpDown = false;
                }

            }

            //Enter�L�[����������
            if (g_nEnter2 == 0 && nFade == FADE_NONE)
            {
                if (GetJoypadTrigger(BUTTON_A) || GetJoypadTrigger(BUTTON_B) || GetKeyboardTrigger(DIK_RETURN))
                {
                    StopSound(SOUND_LABEL_BGM000);	//�^�C�g����BGM���~�߂�
                    SetFade(MODE_RANKING);
                }
            }

			break;

		case MODE_EXIT:
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

			pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[24].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[25].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[26].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[27].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[28].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[29].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[30].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[31].col = D3DCOLOR_RGBA(255, 255, 255, 255);

            if (nFade == FADE_NONE)
            {
                //���������ꂽ�i��ړ��j
                if (GetKeyboardTrigger(DIK_UP) || Controller.rgdwPOV[0] == BUTTON_UP && g_bUpDown == true 
                    || Controller.lY <0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_RANKING;

                    g_bUpDown = false;
                }

            }

            //Enter�L�[����������
            if (g_nEnter2 == 0 && nFade == FADE_NONE)
            {
                if (GetJoypadTrigger(BUTTON_A) || GetJoypadTrigger(BUTTON_B) || GetKeyboardTrigger(DIK_RETURN))
                {
                    SetFade(MODE_EXIT);
                }
            }

			break;
		}

		//���_�f�[�^���A�����b�N����
		g_pVtxBuffTitle->Unlock();

		//���̃G���^�[��������܂ł̎���
		g_nEnter2--;
		if (g_nEnter2 <= 0)
		{
			g_nEnter2 = 0;
		}

	}

}

//=============================================================================
//�^�C�g���I������
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
//�^�C�g���`�揈��
//=============================================================================
void DrawTitle(void)
{
	//���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

	//�e�N�X�`���̐ݒ�
	//�^�C�g���̔w�i
	pDevice->SetTexture(0, g_pTextureTitle[0]);
	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);

	//Enter�L�[�������O
	if (g_nEnter == 0)
	{
		//�v���X�{�^��
		pDevice->SetTexture(0, g_pTextureTitle[1]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 1, NUM_POLYGON);
	}

	//Enter�L�[����������
	if (g_nEnter == 1)
	{
		//�e�I�����̕`��
		for (int nCntTitle = 2; nCntTitle < 8; nCntTitle++)
		{
			//�L�����y�[��
			pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTitle, NUM_POLYGON);
		}
	}
}

//=============================================================================
// ���[�h�I���̎擾
//=============================================================================
MODE GetModeTitle(void)
{
	return g_nMode;
}

//=============================================================================
// �L�����y�[���̓�Փx�I���̎擾
//=============================================================================
LEVEL GetLevelTitle(void)
{
	return g_nLevel;
}

//=============================================================================
// �T�o�C�o�����ԑI���̎擾
//=============================================================================
SURVIVALTIME GetSurvivalTitle(void)
{
	return g_nSurvivalTime;
}