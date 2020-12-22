//=============================================================================
//
// �ŋ��̘b����� [ui.cpp]�̏��� (�Q�[����ʂ�UI)
// Author :�㓡�T�V��
//
//=============================================================================
#include "ui.h"
#include "player.h"
#include "player2.h"
#include "player3.h"
#include "player4.h"
#include "time.h"
#include "game.h"
#include "setting.h"
#include "boss.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonUI[UITYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonUI = NULL;

UI g_aUI[MAX_UI];   //UI�̏��

int g_nTimeMinute;  //�������Ԃ̕�
int g_nCntStart;    //�Q�[���J�n�𐔂���
//=============================================================================
// UI����������
// Author:�㓡�T�V��
//=============================================================================
void InitUI(void)
{
    //�錾
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�������Ԃ̏�����
    SETTING *pSetting = GetSetting();
    g_nTimeMinute = pSetting->nTimeLimit;
    g_nCntStart = 0;

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/number000.png", &g_pTexturePolygonUI[UITYPE_000]);	//�������Ԃ̕�
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui001.png", &g_pTexturePolygonUI[UITYPE_001]);	    //:
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui002.png", &g_pTexturePolygonUI[UITYPE_002]);	    //�v���C���[1
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui003.png", &g_pTexturePolygonUI[UITYPE_003]);	    //�v���C���[2
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui004.png", &g_pTexturePolygonUI[UITYPE_004]);	    //�v���C���[3
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui005.png", &g_pTexturePolygonUI[UITYPE_005]);	    //�v���C���[4
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui006.png", &g_pTexturePolygonUI[UITYPE_006]);	    //�v���C���[1�̓��_
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui006.png", &g_pTexturePolygonUI[UITYPE_007]);	    //�v���C���[2�̓��_
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui006.png", &g_pTexturePolygonUI[UITYPE_008]);	    //�v���C���[3�̓��_
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui006.png", &g_pTexturePolygonUI[UITYPE_009]);	    //�v���C���[4�̓��_
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui007.png", &g_pTexturePolygonUI[UITYPE_010]);	    //��[���A�h��!
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui008.png", &g_pTexturePolygonUI[UITYPE_011]);	    //�}���`
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui009.png", &g_pTexturePolygonUI[UITYPE_012]);	    //��������
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui010.png", &g_pTexturePolygonUI[UITYPE_013]);	    //n�Z�b�g���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui011.png", &g_pTexturePolygonUI[UITYPE_014]);	    //�o�g�����C����/�X�g�[���[
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting013.png", &g_pTexturePolygonUI[UITYPE_015]); //�ӂ�/�S
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar003.png", &g_pTexturePolygonUI[UITYPE_016]);	    //HP�o�[�̔w�i
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar000.png", &g_pTexturePolygonUI[UITYPE_017]);	    //HP�o�[
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/HP.png", &g_pTexturePolygonUI[UITYPE_018]);         //HP�̕���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/score000.png", &g_pTexturePolygonUI[UITYPE_019]);   //�X�R�A�̔w�i
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar002.png", &g_pTexturePolygonUI[UITYPE_020]);	    //�{�X��HP�o�[�̔w�i
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar001.png", &g_pTexturePolygonUI[UITYPE_021]);     //�{�X��HP�o�[
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/BOSS.png", &g_pTexturePolygonUI[UITYPE_022]);       //BOSS

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX * MAX_UI, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonUI, NULL);	//�X�R�A�A�^�C���A�o�[�A����A���@�A��Փx
    VERTEX_2D *pVtx = NULL;	//�o�b�N�O���E���h�̒��_���ւ̃|�C���^

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffPolygonUI->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        g_aUI[nCntUI].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aUI[nCntUI].fSizeX = 0;
        g_aUI[nCntUI].fSizeY = 0;
        g_aUI[nCntUI].nCounterAnim = 0;
        g_aUI[nCntUI].nPatternAnim = 0;
        g_aUI[nCntUI].type = UITYPE_000;
        g_aUI[nCntUI].bUse = false;

        //���_���W�̐ݒ�i�E���Őݒ肷��j
        pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);

        //rhw�̐ݒ�i�l��1.0�Őݒ肷��j
        pVtx[0].rhw = 1.0f;
        pVtx[1].rhw = 1.0f;
        pVtx[2].rhw = 1.0f;
        pVtx[3].rhw = 1.0f;

        //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
        pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

        //�e�N�X�`�����W�̐ݒ�
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffPolygonUI->Unlock();

}

//=============================================================================
// UI�I������
// Author:�㓡�T�V��
//=============================================================================
void UninitUI(void)
{
    //�e�N�X�`���̊J��
    for (int nCntUI = 0; nCntUI < UITYPE_MAX; nCntUI++)
    {
        if (g_pTexturePolygonUI[nCntUI] != NULL)
        {
            g_pTexturePolygonUI[nCntUI]->Release();
            g_pTexturePolygonUI[nCntUI] = NULL;
        }
    }

    //���_�o�b�t�@�̊J��
    if (g_pVtxBuffPolygonUI != NULL)
    {
        g_pVtxBuffPolygonUI->Release();
        g_pVtxBuffPolygonUI = NULL;
    }
}

//=============================================================================
// UI�X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateUI(void)
{
    VERTEX_2D *pVtx = NULL;	            //�o�b�N�O���E���h�̒��_���ւ̃|�C���^
    GAME *pGame = GetGame();            //�Q�[���󋵂��擾
    SETTING *pSetting = GetSetting();   //�ݒ�����擾
    PLAYER *pPlayer = GetPlayer();      //�v���C���[���擾

    //�X�g�[���[�Ȃ�
    if (pSetting->nMaxPlayer == 1)
    {
        //��Փx�ӂ��Ȃ�
        if (pSetting->nLevel == SETTINGLEVEL_NORMAL)
        {
            //HP��250�ȏ�ɂȂ�Ȃ��悤�ɂ���
            if (pPlayer->fLife > PLAYER_STORY_LIFE)
            {
                pPlayer->fLife = PLAYER_STORY_LIFE;
            }
        }

        //��Փx�S�Ȃ�
        else
        {
            //HP��5�ȏ�ɂȂ�Ȃ��悤�ɂ���
            if (pPlayer->fLife > PLAYER_LIFE)
            {
                pPlayer->fLife = PLAYER_LIFE;
            }
        }
    }

    //�v���C���[�̗̑͂�0�������Ȃ��悤�ɂ���
    if (pPlayer->fLife < 0)
    {
        pPlayer->fLife = 0;
    }

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffPolygonUI->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        if (g_aUI[nCntUI].bUse == true)
        {
            //�������Ԃ̕��̐���
            if (g_aUI[nCntUI].type == UITYPE_000)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                pVtx[0].col = TIME_COLOR;
                pVtx[1].col = TIME_COLOR;
                pVtx[2].col = TIME_COLOR;
                pVtx[3].col = TIME_COLOR;

                //0��
                if (g_nTimeMinute == 0)
                {
                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
                }

                //1��
                else if (g_nTimeMinute == 1)
                {
                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.1f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);
                }

                //2��
                else if (g_nTimeMinute == 2)
                {
                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.2f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.3f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.2f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.3f, 1.0f);
                }

                //3��
                else if (g_nTimeMinute == 3)
                {
                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.3f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.4f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.3f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.4f, 1.0f);
                }

                //4��
                else if (g_nTimeMinute == 4)
                {
                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.4f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.4f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
                }

                //5��
                else if (g_nTimeMinute == 5)
                {
                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.6f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.6f, 1.0f);
                }
            }

            //:
            else if (g_aUI[nCntUI].type == UITYPE_001)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                pVtx[0].col = TIME_COLOR;
                pVtx[1].col = TIME_COLOR;
                pVtx[2].col = TIME_COLOR;
                pVtx[3].col = TIME_COLOR;

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //�v���C���[1
            else if (g_aUI[nCntUI].type == UITYPE_002)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //�v���C���[1���������Ă���Ȃ�
                if (pPlayer->bDisp == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //�v���C���[2
            else if (g_aUI[nCntUI].type == UITYPE_003)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //�v���C���[2���������Ă���Ȃ�
                PLAYER2 *pPlayer2 = GetPlayer2();
                if (pPlayer2->bDisp == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //�v���C���[3
            else if (g_aUI[nCntUI].type == UITYPE_004)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //�v���C���[3���������Ă���Ȃ�
                PLAYER3 *pPlayer3 = GetPlayer3();
                if (pPlayer3->bDisp == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //�v���C���[4
            else if (g_aUI[nCntUI].type == UITYPE_005)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //�v���C���[4���������Ă���Ȃ�
                PLAYER4 *pPlayer4 = GetPlayer4();
                if (pPlayer4->bDisp == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //�v���C���[1�̓��_
            else if (g_aUI[nCntUI].type == UITYPE_006)
            {
                //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //0�_
                if (pGame->nCntPointPlayer1 == 0)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* 0.0f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* 0.0f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
                }

                //1�_
                else if (pGame->nCntPointPlayer1 == 1)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
                }

                //2�_
                else if (pGame->nCntPointPlayer1 == 2)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
                }

                //3�_
                else if (pGame->nCntPointPlayer1 == 3)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.75f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.75f, 1.0f);
                }
            }

            //�v���C���[2�̓��_
            else if (g_aUI[nCntUI].type == UITYPE_007)
            {
                //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //0�_
                if (pGame->nCntPointPlayer2 == 0)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.0f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.0f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
                }

                //1�_
                else if (pGame->nCntPointPlayer2 == 1)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
                }

                //2�_
                else if (pGame->nCntPointPlayer2 == 2)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
                }

                //3�_
                else if (pGame->nCntPointPlayer2 == 3)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.75f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.75f, 1.0f);
                }
            }

            //�v���C���[3�̓��_
            else if (g_aUI[nCntUI].type == UITYPE_008)
            {
                //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //0�_
                if (pGame->nCntPointPlayer3 == 0)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.0f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.0f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
                }

                //1�_
                else if (pGame->nCntPointPlayer3 == 1)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
                }

                //2�_
                else if (pGame->nCntPointPlayer3 == 2)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
                }

                //3�_
                else if (pGame->nCntPointPlayer3 == 3)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.75f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.75f, 1.0f);
                }
            }

            //�v���C���[4�̓��_
            else if (g_aUI[nCntUI].type == UITYPE_009)
            {
                //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //0�_
                if (pGame->nCntPointPlayer4 == 0)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.0f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.0f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
                }

                //1�_
                else if (pGame->nCntPointPlayer4 == 1)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
                }

                //2�_
                else if (pGame->nCntPointPlayer4 == 2)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
                }

                //3�_
                else if (pGame->nCntPointPlayer4 == 3)
                {
                    //���_���W�̐ݒ�
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.75f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.75f, 1.0f);
                }
            }

            //��[���A�h��!
            else if (g_aUI[nCntUI].type == UITYPE_010)
            {
                g_nCntStart++;

                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                if (g_nCntStart <= COUNT_WAIT_START_GAME * 2)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }

                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                //��[��
                if (g_nCntStart <= COUNT_WAIT_START_GAME)
                {
                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
                }

                //�͂���!
                else
                {
                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
                }
            }

            //�}���`
            else if (g_aUI[nCntUI].type == UITYPE_011)
            {
                //��ʊO�ɏo�Ă����܂œ�����
                if (g_aUI[nCntUI].pos.x > -g_aUI[nCntUI].fSizeX)
                {
                    //2�b�ŉ�ʊO�ɏo��
                    g_aUI[nCntUI].pos.x -= SCREEN_WIDTH / 120.0f + g_aUI[nCntUI].fSizeX / 120.0f;
                }

                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //n�Z�b�g���
            else if (g_aUI[nCntUI].type == UITYPE_013)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //�ݒ�ɂ���āA�Z�b�g���̕\����ς���
                if (pSetting->nMaxRound == 1)
                {
                    //�e�N�X�`���̍��W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.3333f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.3333f, 1.0f);
                }
                else if (pSetting->nMaxRound == 2)
                {
                    //�e�N�X�`���̍��W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.3333f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.6666f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.3333f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.6666f, 1.0f);
                }
                else if (pSetting->nMaxRound == 3)
                {
                    //�e�N�X�`���̍��W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.6666f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.6666f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
                }
            }

            //�o�g�����C����/�X�g�[���[
            else if (g_aUI[nCntUI].type == UITYPE_014)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //�ݒ�ɂ���āA�o�g�����C�������X�g�[���[��ς���
                //�X�g�[���[
                if (pSetting->nMaxPlayer == 1)
                {
                    //�e�N�X�`���̍��W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
                }
                //�o�g�����C����
                else
                {
                    //�e�N�X�`���̍��W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
                }
            }

            //�ӂ�/�S
            else if (g_aUI[nCntUI].type == UITYPE_015)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //�ݒ�ɂ���āA�ӂ����S��ς���
                if (pSetting->nLevel == SETTINGLEVEL_NORMAL)
                {
                    //�e�N�X�`���̍��W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.6f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.8f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.6f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.8f, 1.0f);
                }
                else
                {
                    //�e�N�X�`���̍��W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.8f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.8f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
                }
            }

            //HP�Q�[�W
            else if (g_aUI[nCntUI].type == UITYPE_017)
            {
                //��Փx�ӂ��Ȃ�
                if (pSetting->nLevel == SETTINGLEVEL_NORMAL)
                {
                    //���_���W�̐ݒ�i���Q�[�W�j
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fLife / PLAYER_STORY_LIFE)),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fLife / PLAYER_STORY_LIFE)),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //HP�ɉ����ăJ���[��ς���i���^�����甼���܂ŗ΁j
                    if (pPlayer->fLife <= PLAYER_STORY_LIFE && pPlayer->fLife > PLAYER_STORY_LIFE / 2)
                    {
                        pVtx[0].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                    }

                    //HP�ɉ����ăJ���[��ς���i��������4����1�܂ŉ��F�j
                    else if (pPlayer->fLife <= PLAYER_STORY_LIFE / 2 && pPlayer->fLife > PLAYER_STORY_LIFE / 4)
                    {
                        pVtx[0].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                    }

                    //HP�ɉ����ăJ���[��ς���i4����1����͐ԐF�j
                    else if (pPlayer->fLife <= PLAYER_STORY_LIFE / 4)
                    {
                        pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                    }

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2((pPlayer->fLife / PLAYER_STORY_LIFE), 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2((pPlayer->fLife / PLAYER_STORY_LIFE), 1.0f);
                }

                //��Փx�S�Ȃ�
                else
                {
                    //���_���W�̐ݒ�i���Q�[�W�j
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fLife / PLAYER_LIFE)),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fLife / PLAYER_LIFE)),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //��Փx�S�́A��ɐԃQ�[�W
                    pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);

                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2((pPlayer->fLife / PLAYER_LIFE), 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2((pPlayer->fLife / PLAYER_LIFE), 1.0f);
                }

            }

            //�{�X��HP�Q�[�W
            else if (g_aUI[nCntUI].type == UITYPE_021)
            {
                BOSS *pBoss = GetBoss();

                //�{�X�̗̑͂��A0�������Ȃ��悤�ɂ���
                if (pBoss->fLife < 0.0f)
                {
                    pBoss->fLife = 0.0f;
                }

                //���_���W�̐ݒ�i�c�Q�[�W�j
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2) - (g_aUI[nCntUI].fSizeY)* (pBoss->fLife / BOSS_LIFE), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2) - (g_aUI[nCntUI].fSizeY)* (pBoss->fLife / BOSS_LIFE), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j(��)
                pVtx[0].col = D3DCOLOR_RGBA(188, 78, 189, 255);
                pVtx[1].col = D3DCOLOR_RGBA(188, 78, 189, 255);
                pVtx[2].col = D3DCOLOR_RGBA(188, 78, 189, 255);
                pVtx[3].col = D3DCOLOR_RGBA(188, 78, 189, 255);

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f - (pBoss->fLife / BOSS_LIFE));
                pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f - (pBoss->fLife / BOSS_LIFE));
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            else
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

        }
        pVtx += 4;
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffPolygonUI->Unlock();
}

//=============================================================================
// UI�`�揈��
// Author:�㓡�T�V��
//=============================================================================
void DrawUI(void)
{
    //�錾
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�A���t�@�e�X�g��L����
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //�A���t�@�e�X�g��l�̐ݒ�
    pDevice->SetRenderState(D3DRS_ALPHAREF, 30);
    //�A���t�@�e�X�g�̔�r���@�̐ݒ�iGREATER�͊�l���傫���ꍇ)
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffPolygonUI, 0, sizeof(VERTEX_2D));
    //���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF(FVF_VERTEX_2D);

    //�|���S���̕`��
    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        if (g_aUI[nCntUI].bUse == true)//�G���g�p���Ȃ�`��
        {
            //�e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, g_pTexturePolygonUI[g_aUI[nCntUI].type]);

            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntUI, NUM_POLYGON);
        }
    }

    //�A���t�@�e�X�g�𖳌���
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    //�e�N�X�`���̐ݒ�̉���
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// UI�̐ݒ菈��
// Author:�㓡�T�V��
//=============================================================================
void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, UITYPE type)
{
    //�ϐ��錾
    VERTEX_2D *pVtx = NULL;

    //���_�o�b�t�@�����b�N
    g_pVtxBuffPolygonUI->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        if (g_aUI[nCntUI].bUse == false)
        {
            //�ʒu��ݒ�
            g_aUI[nCntUI].pos = pos;
            g_aUI[nCntUI].fSizeX = fSizeX;
            g_aUI[nCntUI].fSizeY = fSizeY;
            g_aUI[nCntUI].type = type;
            g_aUI[nCntUI].bUse = true;
            break;
        }
        pVtx += 4;
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffPolygonUI->Unlock();

}