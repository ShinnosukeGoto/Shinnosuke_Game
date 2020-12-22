//=============================================================================
//
// �ŋ��̘b�����̏��� [menu.cpp] (���j���[�I��)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "input.h"
#include "game.h"
#include "menu.h"
#include "fade.h"
#include "sound.h"
#include "manual.h"
#include "controller.h"
#include "setting.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureMenu[MENU_UI] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMenu = NULL;//���_�o�b�t�@�ւ̃|�C���^

MENU g_Menu;	//���j���[�I��

float g_fMoveMenuBgX;   //���j���[�w�i������(��)
float g_fMoveMenuBgY;   //���j���[�w�i������(�c)
//=============================================================================
// ���j���[����������
// Author:�㓡�T�V��
//=============================================================================
void InitMenu(void)
{
    //�ϐ��錾
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�O���[�o���ϐ��̏�����
    g_Menu = MENU_GAME;	//�ŏ��̃J�[�\���̓Q�[��
    g_fMoveMenuBgX = 0.0f;
    g_fMoveMenuBgY = 0.0f;

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/menu000.png", &g_pTextureMenu[0]);	//�w�i
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/menu001.png", &g_pTextureMenu[1]);	//�V��
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/menu002.png", &g_pTextureMenu[2]);	//�ݒ�
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/menu003.png", &g_pTextureMenu[3]);	//�����L���O
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/menu004.png", &g_pTextureMenu[4]);	//EXIT

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX*MENU_UI,//�o�b�t�@�T�C�Y
        D3DUSAGE_WRITEONLY,//(�Œ�j
        FVF_VERTEX_2D,//���_�t�H�[�}�b�g
        D3DPOOL_MANAGED,//�i�Œ�j
        &g_pVtxBuffMenu,//�ϐ������ς��ƕύX���K�v
        NULL);
    VERTEX_2D *pVtx = NULL;//���_���̃|�C���^

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    //���_���W�̐ݒ�i�E���Őݒ肷��j
    //���j���[�̔w�i
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
    //�Q�[��
    pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f);
    pVtx[6].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f);
    pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
    //�ݒ�
    pVtx[8].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f);
    pVtx[9].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[10].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
    pVtx[11].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0.0f);
    //�����L���O
    pVtx[12].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f);
    pVtx[13].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
    pVtx[14].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[15].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT, 0.0f);
    //EXIT
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
    g_pVtxBuffMenu->Unlock();

}

//=============================================================================
// ���j���[�X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateMenu(void)
{
    VERTEX_2D *pVtx = NULL;	//���_���ւ̃|�C���^
    int nFade = GetFade();	//�t�F�[�h�̏����l��

    //�R���g���[���[�̏����擾����
    DIJOYSTATE2 Controller = GetController(PLAYER_1);

    //���_�o�b�t�@�����b�N
    g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

    //�w�i�𓮂���
    g_fMoveMenuBgX++;
    g_fMoveMenuBgY++;
    if (g_fMoveMenuBgX == 1000)
    {
        g_fMoveMenuBgX = 0;
    }

    if (g_fMoveMenuBgY == 1000)
    {
        g_fMoveMenuBgY = 0;
    }
    pVtx[0].tex = D3DXVECTOR2(g_fMoveMenuBgX *0.001f, g_fMoveMenuBgY *0.001f);
    pVtx[1].tex = D3DXVECTOR2(g_fMoveMenuBgX *0.001f + 0.999f, g_fMoveMenuBgY *0.001f);
    pVtx[2].tex = D3DXVECTOR2(g_fMoveMenuBgX *0.001f, g_fMoveMenuBgY *0.001f + 0.999f);
    pVtx[3].tex = D3DXVECTOR2(g_fMoveMenuBgX *0.001f + 0.999f, g_fMoveMenuBgY *0.001f + 0.999f);

    //�ݒ���A�V�Ԃɔ��f
    SETTING *pSetting = GetSetting();
    if (pSetting->nMaxPlayer == 1)
    {
        pVtx[4].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[6].tex = D3DXVECTOR2(0.5f, 1.0f);
        pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
    }
    else
    {
        pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[5].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
    }

    //���j���[�I��
    switch (g_Menu)
    {
    case MENU_GAME:
        //�Q�[�������邢
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

        if (nFade == FADE_NONE)
        {
            //���L�[�������ꂽ�i�E�ړ�,�ݒ�I���j
            if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 || Controller.rgdwPOV[0] == BUTTON_RIGHT)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Menu = MENU_SETTING;
            }

            //���L�[�������ꂽ�i���ړ�,�����L���O�I���j
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 || Controller.rgdwPOV[0] == BUTTON_DOWN)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Menu = MENU_RANKING;
            }

            //Enter�L�[����������
            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_1, BUTTON_START))
            {
                PlaySound(SOUND_LABEL_SE_DECISION);	//���艹
                SetFade(MODE_MANUAL);   //�V�ѕ���ʂɈړ�
            }
        }

        break;

    case MENU_SETTING:
        //�ݒ肪���邢
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

        pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        if (nFade == FADE_NONE)
        {
            //���L�[�������ꂽ�i���ړ�,�Q�[���I���j
            if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 || Controller.rgdwPOV[0] == BUTTON_LEFT)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Menu = MENU_GAME;
            }

            //���L�[�������ꂽ�i���ړ�,EXIT�I���j
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 || Controller.rgdwPOV[0] == BUTTON_DOWN)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Menu = MENU_EXIT;
            }

            //Enter�L�[����������
            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_1, BUTTON_START))
            {
                PlaySound(SOUND_LABEL_SE_DECISION);	//���艹
                SetFade(MODE_SETTING);//�ݒ��ʂɈړ�
            }
        }

        break;

    case MENU_RANKING:
        //�����L���O�����邢
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);

        pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        if (nFade == FADE_NONE)
        {
            //���L�[�������ꂽ�i�E�ړ�,EXIT�I���j
            if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 || Controller.rgdwPOV[0] == BUTTON_RIGHT)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Menu = MENU_EXIT;
            }

            //���L�[�������ꂽ�i��ړ�,�Q�[���I���j
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 || Controller.rgdwPOV[0] == BUTTON_UP)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Menu = MENU_GAME;
            }

            //Enter�L�[����������
            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_1, BUTTON_START))
            {
                PlaySound(SOUND_LABEL_SE_DECISION);	//���艹
                SetFade(MODE_RANKING);//�����L���O��ʂɈړ�
            }
        }

        break;

    case MENU_EXIT:
        //EXIT�����邢
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

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

        if (nFade == FADE_NONE)
        {
            //���L�[�������ꂽ�i���ړ�,�����L���O�I���j
            if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 || Controller.rgdwPOV[0] == BUTTON_LEFT)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Menu = MENU_RANKING;
            }

            //���L�[�������ꂽ�i��ړ�,�ݒ�I���j
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 || Controller.rgdwPOV[0] == BUTTON_UP)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Menu = MENU_SETTING;
            }

            //Enter�L�[����������
            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_1, BUTTON_START))
            {
                StopSound(SOUND_LABEL_BGM_MENU);	//���j���[��BGM
                PlaySound(SOUND_LABEL_SE_DECISION);	//���艹
                SetFade(MODE_EXIT);//�Q�[���S�̂��I��
            }
        }

        break;
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffMenu->Unlock();

}

//=============================================================================
// ���j���[�I������
// Author:�㓡�T�V��
//=============================================================================
void UninitMenu(void)
{
    //�e�N�X�`���̊J��
    for (int nCntMenu = 0; nCntMenu < MENU_UI; nCntMenu++)
    {
        if (g_pTextureMenu[nCntMenu] != NULL)
        {
            g_pTextureMenu[nCntMenu]->Release();
            g_pTextureMenu[nCntMenu] = NULL;
        }
    }

    //���_�o�b�t�@�̊J��
    if (g_pVtxBuffMenu != NULL)
    {
        g_pVtxBuffMenu->Release();
        g_pVtxBuffMenu = NULL;
    }
}
//=============================================================================
// ���j���[�`�揈��
// Author:�㓡�T�V��
//=============================================================================
void DrawMenu(void)
{
    //���_�t�H�[�}�b�g�̐ݒ�
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffMenu, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

    for (int nCntMenu = 0; nCntMenu < MENU_UI; nCntMenu++)
    {
        //�e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, g_pTextureMenu[nCntMenu]);

        //�|���S���̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntMenu, NUM_POLYGON);
    }
}

//=============================================================================
// ���j���[�I���̎擾
// Author:�㓡�T�V��
//=============================================================================
MENU GetMenu(void)
{
    return g_Menu;
}