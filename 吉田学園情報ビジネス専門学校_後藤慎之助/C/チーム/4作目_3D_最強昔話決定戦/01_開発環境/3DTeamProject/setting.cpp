//=============================================================================
//
// �ŋ��̘b�����̏��� [setting.cpp] (�ݒ�I��)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "input.h"
#include "game.h"
#include "setting.h"
#include "fade.h"
#include "sound.h"
#include "manual.h"
#include "controller.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureSetting[SETTING_UI] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSetting = NULL;//���_�o�b�t�@�ւ̃|�C���^

SETTING g_Setting;	//�ݒ�I��

//�J�[�\���𓮂�����܂ł̃J�E���^�[
int g_nCntCursor;   //�J�[�\���𓮂�����܂ł̎���
bool g_bMoveCursor; //�J�[�\���𓮂�����

//=============================================================================
// �ݒ菉��������
// Author:�㓡�T�V��
//=============================================================================
void InitSetting(void)
{
    //�ϐ��錾
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�\���̂̏�����(�ŏ��̃J�[�\���̂ݏ���������)
    g_Setting.type = SETTINGTYPE_PLAYER;

    //�O���[�o���ϐ��̏�����
    g_nCntCursor = 0;
    g_bMoveCursor = true;

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting000.png", &g_pTextureSetting[0]);	//�w�i
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting001.png", &g_pTextureSetting[1]);	//�v���C���[
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting004.png", &g_pTextureSetting[2]);	//���E���h
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting002.png", &g_pTextureSetting[3]);	//��������
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting003.png", &g_pTextureSetting[4]);	//�J�n�ʒu
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting005.png", &g_pTextureSetting[5]);	//�T�h���f�X
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting006.png", &g_pTextureSetting[6]);	//�X�e�[�W
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting007.png", &g_pTextureSetting[7]);	//�v���C���[�̑I����
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting013.png", &g_pTextureSetting[8]);	//���E���h�̑I����
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting008.png", &g_pTextureSetting[9]);	//�������Ԃ̑I����
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting009.png", &g_pTextureSetting[10]);	//�J�n�ʒu�̑I����
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting010.png", &g_pTextureSetting[11]);	//�T�h���f�X�̑I����
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting014.png", &g_pTextureSetting[12]);	//�X�e�[�W�̑I����
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/OK.png", &g_pTextureSetting[13]);	        //OK
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting011.png", &g_pTextureSetting[14]);	//�����
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting012.png", &g_pTextureSetting[15]);	//�E���

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX*SETTING_UI,//�o�b�t�@�T�C�Y
        D3DUSAGE_WRITEONLY,//(�Œ�j
        FVF_VERTEX_2D,//���_�t�H�[�}�b�g
        D3DPOOL_MANAGED,//�i�Œ�j
        &g_pVtxBuffSetting,//�ϐ������ς��ƕύX���K�v
        NULL);

    VERTEX_2D *pVtx = NULL;//���_���̃|�C���^

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffSetting->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    //���_���W�̐ݒ�i�E���Őݒ肷��j
    //�ݒ�̔w�i
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    //�v���C���[
    pVtx[4].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_PLAYER_POS;
    pVtx[5].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_PLAYER_POS;
    pVtx[6].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_PLAYER_POS;
    pVtx[7].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_PLAYER_POS;

    //���E���h
    pVtx[8].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_ROUND_POS;
    pVtx[9].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_ROUND_POS;
    pVtx[10].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_ROUND_POS;
    pVtx[11].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_ROUND_POS;

    //��������
    pVtx[12].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;
    pVtx[13].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;
    pVtx[14].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;
    pVtx[15].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;

    //�J�n�ʒu
    pVtx[16].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;
    pVtx[17].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;
    pVtx[18].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;
    pVtx[19].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;

    //�T�h���f�X
    pVtx[20].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;
    pVtx[21].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;
    pVtx[22].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;
    pVtx[23].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;

    //�X�e�[�W
    pVtx[24].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;
    pVtx[25].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;
    pVtx[26].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;
    pVtx[27].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;

    //�v���C���[���̑I����
    pVtx[28].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_PLAYER_POS;
    pVtx[29].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_PLAYER_POS;
    pVtx[30].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_PLAYER_POS;
    pVtx[31].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_PLAYER_POS;

    //���E���h�̑I����
    pVtx[32].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_ROUND_POS;
    pVtx[33].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_ROUND_POS;
    pVtx[34].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_ROUND_POS;
    pVtx[35].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_ROUND_POS;

    //�������Ԃ̑I����
    pVtx[36].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;
    pVtx[37].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;
    pVtx[38].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;
    pVtx[39].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;

    //�J�n�ʒu�̑I����
    pVtx[40].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;
    pVtx[41].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;
    pVtx[42].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;
    pVtx[43].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;

    //�T�h���f�X�̑I����
    pVtx[44].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;
    pVtx[45].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;
    pVtx[46].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;
    pVtx[47].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;

    //�X�e�[�W�̑I����
    pVtx[48].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;
    pVtx[49].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;
    pVtx[50].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;
    pVtx[51].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;

    //OK
    pVtx[52].pos = D3DXVECTOR3(-OK_SIZE_X, -OK_SIZE_Y, 0.0f) + OK_POS;
    pVtx[53].pos = D3DXVECTOR3(OK_SIZE_X, -OK_SIZE_Y, 0.0f) + OK_POS;
    pVtx[54].pos = D3DXVECTOR3(-OK_SIZE_X, OK_SIZE_Y, 0.0f) + OK_POS;
    pVtx[55].pos = D3DXVECTOR3(OK_SIZE_X, OK_SIZE_Y, 0.0f) + OK_POS;

    //�����
    pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
    pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
    pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
    pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

    //�E���
    pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
    pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
    pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
    pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

    //�e�틤�ʂ̐ݒ�
    for (int nCntSetting = 0; nCntSetting < SETTING_UI; nCntSetting++)
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
    g_pVtxBuffSetting->Unlock();
}
//=============================================================================
// �ݒ�X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateSetting(void)
{
    VERTEX_2D *pVtx = NULL;	//���_���ւ̃|�C���^
    int nFade = GetFade();	//�t�F�[�h�̏����l��

    //�R���g���[���[�̏����擾����
    DIJOYSTATE2 Controller = GetController(PLAYER_1);

    //���_�o�b�t�@�����b�N
    g_pVtxBuffSetting->Lock(0, 0, (void**)&pVtx, 0);

    //�\���L�[��X�e�B�b�N�ŃJ�[�\���𓮂�����܂ł̃J�E���^�[
    if (g_bMoveCursor == false)
    {
        g_nCntCursor++;
        if (g_nCntCursor > COUNT_MOVE_CURSOR)
        {
            g_nCntCursor = 0;
            g_bMoveCursor = true;   //��������
        }
    }

    //OK�̓����x��������
    pVtx[52].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[53].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[54].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[55].col = D3DCOLOR_RGBA(255, 255, 255, 55);

    //�ݒ�I��
    switch (g_Setting.type)
    {
        //===================
        //�l���I��
        //===================
    case SETTINGTYPE_PLAYER:

        if (nFade == FADE_NONE)
        {
            //���L�[�������ꂽ�i��ړ�,OK�I���ցj
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Setting.type = SETTINGTYPE_OK;

                g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
            }

            //���L�[�������ꂽ�i���ړ�,���E���h�I���ցj
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                || GetJoypadTrigger(PLAYER_1,BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
                || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Setting.type = SETTINGTYPE_ROUND;

                g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
            }

            //���L�[�������ꂽ
            if (g_Setting.nMaxPlayer > 1)
            {
                if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_LEFT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //�I����

                    g_Setting.nMaxPlayer--;

                    g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                }
            }

            //���L�[�������ꂽ
            if (g_Setting.nMaxPlayer < 4)
            {
                if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_RIGHT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //�I����

                    g_Setting.nMaxPlayer++;

                    g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                }
            }

        }

        //��ԍ��̑I�����łȂ��Ȃ�
        if (g_Setting.nMaxPlayer != 1)
        {
            //���J�[�\���̈ʒu
            pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + LEFT_CURSOR_POS;
            pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + LEFT_CURSOR_POS;
            pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + LEFT_CURSOR_POS;
            pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + LEFT_CURSOR_POS;
        }
        else
        {
            //���J�[�\���̈ʒu
            pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        }

        //��ԉE�̑I�����łȂ��Ȃ�
        if (g_Setting.nMaxPlayer != 4)
        {
            //�E�J�[�\���̈ʒu
            pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + RIGHT_CURSOR_POS;
            pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + RIGHT_CURSOR_POS;
            pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + RIGHT_CURSOR_POS;
            pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + RIGHT_CURSOR_POS;
        }
        else
        {
            //�E�J�[�\���̈ʒu
            pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        }

        break;

        //===================
        //���E���h�I��/��Փx�I��
        //===================
    case SETTINGTYPE_ROUND:

        if (nFade == FADE_NONE)
        {
            //���L�[�������ꂽ�i��ړ�,�v���C���[�I���ցj
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Setting.type = SETTINGTYPE_PLAYER;

                g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
            }

            //�݂�ȂŗV�ԂȂ�
            if (g_Setting.nMaxPlayer != 1)
            {
                //���L�[�������ꂽ�i���ړ�,�������ԑI���ցj
                if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                    || GetJoypadTrigger(PLAYER_1, BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
                    || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                    g_Setting.type = SETTINGTYPE_TIME;

                    g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                }

                //���L�[�������ꂽ
                if (g_Setting.nMaxRound > 1)
                {
                    if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 && g_bMoveCursor == true
                        || Controller.rgdwPOV[0] == BUTTON_LEFT && g_bMoveCursor == true)
                    {
                        PlaySound(SOUND_LABEL_SE_SELECT);   //�I����

                        g_Setting.nMaxRound--;

                        g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                    }
                }

                //���L�[�������ꂽ
                if (g_Setting.nMaxRound < 3)
                {
                    if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 && g_bMoveCursor == true
                        || Controller.rgdwPOV[0] == BUTTON_RIGHT && g_bMoveCursor == true)
                    {
                        PlaySound(SOUND_LABEL_SE_SELECT);   //�I����

                        g_Setting.nMaxRound++;

                        g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                    }
                }
            }

            //�ЂƂ�ŗV�ԂȂ�
            else
            {
                //���L�[�������ꂽ�i���ړ�,OK�I���ցj
                if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                    || GetJoypadTrigger(PLAYER_1, BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
                    || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                    g_Setting.type = SETTINGTYPE_OK;

                    g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                }

                //���L�[�������ꂽ
                if (g_Setting.nLevel > SETTINGLEVEL_NORMAL)
                {
                    if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 && g_bMoveCursor == true
                        || Controller.rgdwPOV[0] == BUTTON_LEFT && g_bMoveCursor == true)
                    {
                        PlaySound(SOUND_LABEL_SE_SELECT);   //�I����

                        g_Setting.nLevel--;

                        g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                    }
                }

                //���L�[�������ꂽ
                if (g_Setting.nLevel < SETTINGLEVEL_EXPERT)
                {
                    if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 && g_bMoveCursor == true
                        || Controller.rgdwPOV[0] == BUTTON_RIGHT && g_bMoveCursor == true)
                    {
                        PlaySound(SOUND_LABEL_SE_SELECT);   //�I����

                        g_Setting.nLevel++;

                        g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                    }
                }
            }
        }

        //�݂�ȂŗV�ԂȂ�
        if (g_Setting.nMaxPlayer != 1)
        {
            //��ԍ��̑I�����łȂ��Ȃ�
            if (g_Setting.nMaxRound != 1)
            {
                //���J�[�\���̈ʒu
                pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
                pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
                pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
                pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
            }
            else
            {
                //���J�[�\���̈ʒu
                pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            }

            //��ԉE�̑I�����łȂ��Ȃ�
            if (g_Setting.nMaxRound != 3)
            {
                //�E�J�[�\���̈ʒu
                pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
                pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
                pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
                pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
            }
            else
            {
                //�E�J�[�\���̈ʒu
                pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            }
        }

        //�ЂƂ�ŗV�ԂȂ�
        else
        {
            //��ԍ��̑I�����łȂ��Ȃ�
            if (g_Setting.nLevel != SETTINGLEVEL_NORMAL)
            {
                //���J�[�\���̈ʒu
                pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
                pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
                pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
                pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
            }
            else
            {
                //���J�[�\���̈ʒu
                pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            }

            //��ԉE�̑I�����łȂ��Ȃ�
            if (g_Setting.nLevel != SETTINGLEVEL_EXPERT)
            {
                //�E�J�[�\���̈ʒu
                pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
                pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
                pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
                pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
            }
            else
            {
                //�E�J�[�\���̈ʒu
                pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            }
        }

        break;

        //===================
        //�������ԑI��
        //===================
    case SETTINGTYPE_TIME:

        if (nFade == FADE_NONE)
        {
            //���L�[�������ꂽ�i��ړ�,���E���h�I���ցj
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Setting.type = SETTINGTYPE_ROUND;

                g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
            }

            //���L�[�������ꂽ�i���ړ�,�J�n�ʒu�I���ցj
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                || GetJoypadTrigger(PLAYER_1, BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
                || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Setting.type = SETTINGTYPE_START_POS;

                g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
            }

            //���L�[�������ꂽ
            if (g_Setting.nTimeLimit > 3)
            {
                if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_LEFT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //�I����

                    g_Setting.nTimeLimit--;

                    g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                }
            }

            //���L�[�������ꂽ
            if (g_Setting.nTimeLimit < 5)
            {
                if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_RIGHT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //�I����

                    g_Setting.nTimeLimit++;

                    g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                }
            }
        }

        //��ԍ��̑I�����łȂ��Ȃ�
        if (g_Setting.nTimeLimit != 3)
        {
            //���J�[�\���̈ʒu
            pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + LEFT_CURSOR_POS;
            pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + LEFT_CURSOR_POS;
            pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + LEFT_CURSOR_POS;
            pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + LEFT_CURSOR_POS;
        }
        else
        {
            //���J�[�\���̈ʒu
            pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        }

        //��ԉE�̑I�����łȂ��Ȃ�
        if (g_Setting.nTimeLimit != 5)
        {
            //�E�J�[�\���̈ʒu
            pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + RIGHT_CURSOR_POS;
            pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + RIGHT_CURSOR_POS;
            pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + RIGHT_CURSOR_POS;
            pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + RIGHT_CURSOR_POS;
        }
        else
        {
            //�E�J�[�\���̈ʒu
            pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        }


        break;

        //===================
        //�J�n�ʒu�I��
        //===================
    case SETTINGTYPE_START_POS:

        if (nFade == FADE_NONE)
        {
            //���L�[�������ꂽ�i��ړ�,�������ԑI���ցj
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Setting.type = SETTINGTYPE_TIME;

                g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
            }

            //���L�[�������ꂽ�i���ړ�,�T�h���f�X�I���ցj
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                || GetJoypadTrigger(PLAYER_1, BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
                || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Setting.type = SETTINGTYPE_SUDDEN_DEATH;

                g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
            }

            //���L�[�������ꂽ
            if (g_Setting.bRandomStartPos != false)
            {
                if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_LEFT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //�I����

                    g_Setting.bRandomStartPos = false;  //�Œ�

                    g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                }
            }

            //���L�[�������ꂽ
            if (g_Setting.bRandomStartPos != true)
            {
                if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_RIGHT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //�I����

                    g_Setting.bRandomStartPos = true;   //�����_��

                    g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                }
            }
        }

        //���̑I����
        if (g_Setting.bRandomStartPos == false)
        {
            //�����
            pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

            //�E�J�[�\���̈ʒu
            pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + RIGHT_CURSOR_POS;
            pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + RIGHT_CURSOR_POS;
            pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + RIGHT_CURSOR_POS;
            pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + RIGHT_CURSOR_POS;
        }

        //�E�̑I����
        else
        {
            //���J�[�\���̈ʒu
            pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + LEFT_CURSOR_POS;
            pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + LEFT_CURSOR_POS;
            pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + LEFT_CURSOR_POS;
            pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + LEFT_CURSOR_POS;

            //�E���
            pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        }

        break;

        //===================
        //�T�h���f�X�I��
        //===================
    case SETTINGTYPE_SUDDEN_DEATH:

        if (nFade == FADE_NONE)
        {
            //���L�[�������ꂽ�i��ړ�,�J�n�ʒu�I���ցj
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Setting.type = SETTINGTYPE_START_POS;

                g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
            }

            //���L�[�������ꂽ�i���ړ�,�X�e�[�W�I���ցj
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                || GetJoypadTrigger(PLAYER_1, BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
                || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Setting.type = SETTINGTYPE_STAGE;

                g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
            }

            //���L�[�������ꂽ
            if (g_Setting.bSuddenDeath != true)
            {
                if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_LEFT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //�I����

                    g_Setting.bSuddenDeath = true;

                    g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                }
            }

            //���L�[�������ꂽ
            if (g_Setting.bSuddenDeath != false)
            {
                if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_RIGHT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //�I����

                    g_Setting.bSuddenDeath = false;

                    g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                }
            }
        }

        //���̑I����
        if (g_Setting.bSuddenDeath == true)
        {
            //�����
            pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

            //�E�J�[�\���̈ʒu
            pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + RIGHT_CURSOR_POS;
            pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + RIGHT_CURSOR_POS;
            pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + RIGHT_CURSOR_POS;
            pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + RIGHT_CURSOR_POS;
        }

        //�E�̑I����
        else
        {
            //���J�[�\���̈ʒu
            pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + LEFT_CURSOR_POS;
            pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + LEFT_CURSOR_POS;
            pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + LEFT_CURSOR_POS;
            pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + LEFT_CURSOR_POS;

            //�E���
            pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        }

        break;

        //===================
        //�X�e�[�W�I��
        //===================
    case SETTINGTYPE_STAGE:

        if (nFade == FADE_NONE)
        {
            //���L�[�������ꂽ�i��ړ�,�T�h���f�X�I���ցj
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Setting.type = SETTINGTYPE_SUDDEN_DEATH;

                g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
            }

            //���L�[�������ꂽ�i���ړ�,OK�I���ցj
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                || GetJoypadTrigger(PLAYER_1, BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
                || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Setting.type = SETTINGTYPE_OK;

                g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
            }

            //���L�[�������ꂽ
            if (g_Setting.nStage != SETTINGSTAGE_NORMAL)
            {
                if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_LEFT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //�I����

                    g_Setting.nStage--;

                    g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                }
            }

            //���L�[�������ꂽ
            if (g_Setting.nStage != SETTINGSTAGE_RANDOM)
            {
                if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_RIGHT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //�I����

                    g_Setting.nStage++;

                    g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                }
            }
        }

        //��ԍ��̑I�����łȂ��Ȃ�
        if (g_Setting.nStage != SETTINGSTAGE_NORMAL)
        {
            //���J�[�\���̈ʒu
            pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + LEFT_CURSOR_POS;
            pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + LEFT_CURSOR_POS;
            pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + LEFT_CURSOR_POS;
            pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + LEFT_CURSOR_POS;
        }
        else
        {
            //�����
            pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        }

        //��ԉE�̑I�����łȂ��Ȃ�
        if (g_Setting.nStage != SETTINGSTAGE_RANDOM)
        {
            //�E�J�[�\���̈ʒu
            pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + RIGHT_CURSOR_POS;
            pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + RIGHT_CURSOR_POS;
            pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + RIGHT_CURSOR_POS;
            pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + RIGHT_CURSOR_POS;
        }
        else
        {
            //�E���
            pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        }

        break;

        //===================
        //OK�I��
        //===================
    case SETTINGTYPE_OK:

        //�I�𒆂́AOK�̓����x���グ��
        pVtx[52].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[53].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[54].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[55].col = D3DCOLOR_RGBA(255, 255, 255, 255);

        //���E�J�[�\�����A�ʏ�Ƃ͋t�̈ʒu�ɂ��āA�I�����Ă��邱�Ƃ��킩��₷������
        //���J�[�\���̈ʒu
        pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + OK_POS + RIGHT_CURSOR_POS;
        pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + OK_POS + RIGHT_CURSOR_POS;
        pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + OK_POS + RIGHT_CURSOR_POS;
        pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + OK_POS + RIGHT_CURSOR_POS;
        //�E�J�[�\���̈ʒu
        pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + OK_POS + LEFT_CURSOR_POS;
        pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + OK_POS + LEFT_CURSOR_POS;
        pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + OK_POS + LEFT_CURSOR_POS;
        pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + OK_POS + LEFT_CURSOR_POS;

        if (nFade == FADE_NONE)
        {

            //�݂�ȂŗV�ԂȂ�
            if (g_Setting.nMaxPlayer != 1)
            {
                //���L�[�������ꂽ�i��ړ�,�X�e�[�W�I���ցj
                if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                    g_Setting.type = SETTINGTYPE_STAGE;

                    g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                }
            }

            //�ЂƂ�ŗV�ԂȂ�
            else
            {
                //���L�[�������ꂽ�i��ړ�,���E���h�I���ցj
                if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                    g_Setting.type = SETTINGTYPE_ROUND;

                    g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
                }
            }

            //���L�[�������ꂽ�i���ړ�,�v���C�l���I���ցj
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //�I����
                g_Setting.type = SETTINGTYPE_PLAYER;

                g_bMoveCursor = false;   //�J�[�\����A���œ������Ȃ�����
            }

            //����L�[�������ꂽ
            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_B)
                || GetJoypadTrigger(PLAYER_1, BUTTON_A))
            {
                PlaySound(SOUND_LABEL_SE_DECISION);   //���艹
                SetFade(MODE_MENU); //���j���[�֖߂�
            }

        }

        break;
    }

    //===========================================
    //�I�����̃e�N�X�`���̔��f
    //===========================================
    //===================
    //�v���C���[�I��
    //===================
    if (g_Setting.nMaxPlayer == 1)
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[28].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[29].tex = D3DXVECTOR2(0.25f, 0.0f);
        pVtx[30].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[31].tex = D3DXVECTOR2(0.25f, 1.0f);
    }
    else if (g_Setting.nMaxPlayer == 2)
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[28].tex = D3DXVECTOR2(0.25f, 0.0f);
        pVtx[29].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[30].tex = D3DXVECTOR2(0.25f, 1.0f);
        pVtx[31].tex = D3DXVECTOR2(0.5f, 1.0f);
    }
    else if (g_Setting.nMaxPlayer == 3)
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[28].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[29].tex = D3DXVECTOR2(0.75f, 0.0f);
        pVtx[30].tex = D3DXVECTOR2(0.5f, 1.0f);
        pVtx[31].tex = D3DXVECTOR2(0.75f, 1.0f);
    }
    else if (g_Setting.nMaxPlayer == 4)
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[28].tex = D3DXVECTOR2(0.75f, 0.0f);
        pVtx[29].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[30].tex = D3DXVECTOR2(0.75f, 1.0f);
        pVtx[31].tex = D3DXVECTOR2(1.0f, 1.0f);
    }

    //===================
    //���E���h�I��
    //===================
    //�݂�ȂŗV�ԂȂ�
    if (g_Setting.nMaxPlayer != 1)
    {
        if (g_Setting.nMaxRound == 1)
        {
            //�e�N�X�`���̍��W�̐ݒ�
            pVtx[32].tex = D3DXVECTOR2(0.0f, 0.0f);
            pVtx[33].tex = D3DXVECTOR2(0.2f, 0.0f);
            pVtx[34].tex = D3DXVECTOR2(0.0f, 1.0f);
            pVtx[35].tex = D3DXVECTOR2(0.2f, 1.0f);
        }
        else if (g_Setting.nMaxRound == 2)
        {
            //�e�N�X�`���̍��W�̐ݒ�
            pVtx[32].tex = D3DXVECTOR2(0.2f, 0.0f);
            pVtx[33].tex = D3DXVECTOR2(0.4f, 0.0f);
            pVtx[34].tex = D3DXVECTOR2(0.2f, 1.0f);
            pVtx[35].tex = D3DXVECTOR2(0.4f, 1.0f);
        }
        else if (g_Setting.nMaxRound == 3)
        {
            //�e�N�X�`���̍��W�̐ݒ�
            pVtx[32].tex = D3DXVECTOR2(0.4f, 0.0f);
            pVtx[33].tex = D3DXVECTOR2(0.6f, 0.0f);
            pVtx[34].tex = D3DXVECTOR2(0.4f, 1.0f);
            pVtx[35].tex = D3DXVECTOR2(0.6f, 1.0f);
        }
    }

    //===================
    //��Փx�I��
    //===================
    //�ЂƂ�ŗV�ԂȂ�
    if (g_Setting.nMaxPlayer == 1)
    {
        if (g_Setting.nLevel == SETTINGLEVEL_NORMAL)
        {
            //�e�N�X�`���̍��W�̐ݒ�
            pVtx[32].tex = D3DXVECTOR2(0.6f, 0.0f);
            pVtx[33].tex = D3DXVECTOR2(0.8f, 0.0f);
            pVtx[34].tex = D3DXVECTOR2(0.6f, 1.0f);
            pVtx[35].tex = D3DXVECTOR2(0.8f, 1.0f);
        }
        else if (g_Setting.nLevel == SETTINGLEVEL_EXPERT)
        {
            //�e�N�X�`���̍��W�̐ݒ�
            pVtx[32].tex = D3DXVECTOR2(0.8f, 0.0f);
            pVtx[33].tex = D3DXVECTOR2(1.0f, 0.0f);
            pVtx[34].tex = D3DXVECTOR2(0.8f, 1.0f);
            pVtx[35].tex = D3DXVECTOR2(1.0f, 1.0f);
        }
    }

    //===================
    //�������ԑI��
    //===================
    if (g_Setting.nTimeLimit == 3)
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[36].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[37].tex = D3DXVECTOR2(0.3333f, 0.0f);
        pVtx[38].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[39].tex = D3DXVECTOR2(0.3333f, 1.0f);
    }
    else if (g_Setting.nTimeLimit == 4)
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[36].tex = D3DXVECTOR2(0.3333f, 0.0f);
        pVtx[37].tex = D3DXVECTOR2(0.6666f, 0.0f);
        pVtx[38].tex = D3DXVECTOR2(0.3333f, 1.0f);
        pVtx[39].tex = D3DXVECTOR2(0.6666f, 1.0f);
    }
    else if (g_Setting.nTimeLimit == 5)
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[36].tex = D3DXVECTOR2(0.6666f, 0.0f);
        pVtx[37].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[38].tex = D3DXVECTOR2(0.6666f, 1.0f);
        pVtx[39].tex = D3DXVECTOR2(1.0f, 1.0f);
    }

    //===================
    //�J�n�ʒu�I��
    //===================
    if (g_Setting.bRandomStartPos == true)
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[40].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[41].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[42].tex = D3DXVECTOR2(0.5f, 1.0f);
        pVtx[43].tex = D3DXVECTOR2(1.0f, 1.0f);
    }
    else
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[40].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[41].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[42].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[43].tex = D3DXVECTOR2(0.5f, 1.0f);
    }

    //===================
    //�T�h���f�X�I��
    //===================
    if (g_Setting.bSuddenDeath == true)
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[44].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[45].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[46].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[47].tex = D3DXVECTOR2(0.5f, 1.0f);
    }
    else
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[44].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[45].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[46].tex = D3DXVECTOR2(0.5f, 1.0f);
        pVtx[47].tex = D3DXVECTOR2(1.0f, 1.0f);
    }

    //===================
    //�X�e�[�W�I��
    //===================
    if (g_Setting.nStage == SETTINGSTAGE_NORMAL)
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[48].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[49].tex = D3DXVECTOR2(0.2f, 0.0f);
        pVtx[50].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[51].tex = D3DXVECTOR2(0.2f, 1.0f);
    }
    else if (g_Setting.nStage == SETTINGSTAGE_PLAIN)
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[48].tex = D3DXVECTOR2(0.2f, 0.0f);
        pVtx[49].tex = D3DXVECTOR2(0.4f, 0.0f);
        pVtx[50].tex = D3DXVECTOR2(0.2f, 1.0f);
        pVtx[51].tex = D3DXVECTOR2(0.4f, 1.0f);
    }
    else if (g_Setting.nStage == SETTINGSTAGE_RUIN)
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[48].tex = D3DXVECTOR2(0.4f, 0.0f);
        pVtx[49].tex = D3DXVECTOR2(0.6f, 0.0f);
        pVtx[50].tex = D3DXVECTOR2(0.4f, 1.0f);
        pVtx[51].tex = D3DXVECTOR2(0.6f, 1.0f);
    }
    else if (g_Setting.nStage == SETTINGSTAGE_CROSS)
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[48].tex = D3DXVECTOR2(0.6f, 0.0f);
        pVtx[49].tex = D3DXVECTOR2(0.8f, 0.0f);
        pVtx[50].tex = D3DXVECTOR2(0.6f, 1.0f);
        pVtx[51].tex = D3DXVECTOR2(0.8f, 1.0f);
    }
    else if (g_Setting.nStage == SETTINGSTAGE_RANDOM)
    {
        //�e�N�X�`���̍��W�̐ݒ�
        pVtx[48].tex = D3DXVECTOR2(0.8f, 0.0f);
        pVtx[49].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[50].tex = D3DXVECTOR2(0.8f, 1.0f);
        pVtx[51].tex = D3DXVECTOR2(1.0f, 1.0f);
    }

    //===========================================
    //�݂�ȂŗV�Ԃ��A�ЂƂ�ŗV�Ԃ��őI������ς���
    //===========================================
    //�݂�ȂŗV�ԂȂ�
    if (g_Setting.nMaxPlayer != 1)
    {
        //���E���h�I��
        pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[9].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[11].tex = D3DXVECTOR2(0.5f, 1.0f);

        //��������
        pVtx[12].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;
        pVtx[13].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;
        pVtx[14].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;
        pVtx[15].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;

        //�J�n�ʒu
        pVtx[16].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;
        pVtx[17].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;
        pVtx[18].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;
        pVtx[19].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;

        //�T�h���f�X
        pVtx[20].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;
        pVtx[21].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;
        pVtx[22].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;
        pVtx[23].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;

        //�X�e�[�W
        pVtx[24].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;
        pVtx[25].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;
        pVtx[26].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;
        pVtx[27].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;

        //�������Ԃ̑I����
        pVtx[36].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;
        pVtx[37].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;
        pVtx[38].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;
        pVtx[39].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;

        //�J�n�ʒu�̑I����
        pVtx[40].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;
        pVtx[41].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;
        pVtx[42].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;
        pVtx[43].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;

        //�T�h���f�X�̑I����
        pVtx[44].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;
        pVtx[45].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;
        pVtx[46].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;
        pVtx[47].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;

        //�X�e�[�W�̑I����
        pVtx[48].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;
        pVtx[49].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;
        pVtx[50].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;
        pVtx[51].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;
    }

    //�ЂƂ�ŗV�ԂȂ�
    else
    {
        //��Փx�I��
        pVtx[8].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[10].tex = D3DXVECTOR2(0.5f, 1.0f);
        pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

        //��������
        pVtx[12].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[13].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[14].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[15].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        //�J�n�ʒu
        pVtx[16].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[17].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[18].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[19].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        //�T�h���f�X
        pVtx[20].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[21].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[22].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[23].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        //�X�e�[�W
        pVtx[24].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[25].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[26].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[27].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        //�������Ԃ̑I����
        pVtx[36].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[37].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[38].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[39].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        //�J�n�ʒu�̑I����
        pVtx[40].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[41].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[42].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[43].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        //�T�h���f�X�̑I����
        pVtx[44].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[45].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[46].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[47].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        //�X�e�[�W�̑I����
        pVtx[48].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[49].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[50].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[51].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

    }


    //���_�f�[�^���A�����b�N����
    g_pVtxBuffSetting->Unlock();

}

//=============================================================================
// �ݒ�I������
// Author:�㓡�T�V��
//=============================================================================
void UninitSetting(void)
{
    //�e�N�X�`���̊J��
    for (int nCntSetting = 0; nCntSetting < SETTING_UI; nCntSetting++)
    {
        if (g_pTextureSetting[nCntSetting] != NULL)
        {
            g_pTextureSetting[nCntSetting]->Release();
            g_pTextureSetting[nCntSetting] = NULL;
        }
    }

    //���_�o�b�t�@�̊J��
    if (g_pVtxBuffSetting != NULL)
    {
        g_pVtxBuffSetting->Release();
        g_pVtxBuffSetting = NULL;
    }
}
//=============================================================================
// �ݒ�`�揈��
// Author:�㓡�T�V��
//=============================================================================
void DrawSetting(void)
{
    //���_�t�H�[�}�b�g�̐ݒ�
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffSetting, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

    for (int nCntSetting = 0; nCntSetting < SETTING_UI; nCntSetting++)
    {
        //�e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, g_pTextureSetting[nCntSetting]);

        //�|���S���̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntSetting, NUM_POLYGON);
    }
}

//=============================================================================
// �ݒ�I���̎擾
// Author:�㓡�T�V��
//=============================================================================
SETTING *GetSetting(void)
{
    return &g_Setting;
}

//=============================================================================
// �ݒ��ʂɓ���Ȃ��Ă��A�ŏ��ɏ������������
// Author:�㓡�T�V��
//=============================================================================
void InitFirstSetting(void)
{
    //�\���̂̏�����
    g_Setting.nMaxPlayer = FIRST_SETTING_MAX_PLAYER;
    g_Setting.nMaxRound = FIRST_SETTING_MAX_ROUND;
    g_Setting.nTimeLimit = FIRST_SETTING_TIME_LIMIT;
    g_Setting.bRandomStartPos = FIRST_SETTING_RANDOM_START_POS;
    g_Setting.bSuddenDeath = FIRST_SETTING_SUDDEN_DEATH;
    g_Setting.nStage = FIRST_SETTING_STAGE;
    g_Setting.nLevel = FIRST_SETTING_LEVEL;

    g_Setting.type = SETTINGTYPE_PLAYER;
}