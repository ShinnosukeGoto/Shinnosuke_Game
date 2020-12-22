//=============================================================================
//
// �ŋ��̘b�����̏��� [result.cpp] (���ʔ��\)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "result.h"
#include "input.h"
#include "game.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "title.h"
#include "controller.h"
#include "score.h"
#include "setting.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_apTextureResult[RESULTSTATE_MAX] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;//���_�o�b�t�@�ւ̃|�C���^

SELECT g_Winner;    //�N����������

//=============================================================================
// ���U���g����������
// Author:�㓡�T�V��
//=============================================================================
void InitResult(void)
{
    //�X�R�A�̏�����(���_�͏��������Ȃ�)
    InitScore();

    //�ϐ��錾
    VERTEX_2D *pVtx = NULL;//���_���̃|�C���^
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�Q�[���̏󋵂��擾
    GAME*pGame = GetGame();
    g_Winner = pGame->winner;   //���҂����т���

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX * 1,//�o�b�t�@�T�C�Y
        D3DUSAGE_WRITEONLY,//(�Œ�j
        FVF_VERTEX_2D,//���_�t�H�[�}�b�g
        D3DPOOL_MANAGED,//�i�Œ�j
        &g_pVtxBuffResult,//�ϐ������ς��ƕύX���K�v
        NULL);

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/result000.png", &g_apTextureResult[RESULTSTATE_GAMEOVER]);	//�Q�[���I�[�o�[
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/result001.png", &g_apTextureResult[RESULTSTATE_WIN1]);	    //�v���C���[1�̏���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/result002.png", &g_apTextureResult[RESULTSTATE_WIN2]);	    //�v���C���[2�̏���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/result003.png", &g_apTextureResult[RESULTSTATE_WIN3]);	    //�v���C���[3�̏���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/result004.png", &g_apTextureResult[RESULTSTATE_WIN4]);	    //�v���C���[4�̏���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/result005.png", &g_apTextureResult[RESULTSTATE_GAMECLEAR]);	//�Q�[���N���A
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/result006.png", &g_apTextureResult[RESULTSTATE_PERFECT]);	//�p�[�t�F�N�g

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

    PlaySound(SOUND_LABEL_BGM_RESULT);	//���U���g��BGM
}
//=============================================================================
// ���U���g�X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateResult(void)
{
    //�X�R�A�̍X�V
    UpdateScore();

    int nFade = GetFade();	//�t�F�[�h�̏����l��

    if (nFade == FADE_NONE)
    {
        //�ݒ���擾
        SETTING *pSetting = GetSetting();

        //�o�g�����C�����Ȃ�
        if (pSetting->nMaxPlayer >= 2)
        {

            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_1, BUTTON_X) || GetJoypadTrigger(PLAYER_1, BUTTON_Y) || GetJoypadTrigger(PLAYER_1, BUTTON_START)
                || GetJoypadTrigger(PLAYER_2, BUTTON_A) || GetJoypadTrigger(PLAYER_2, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_2, BUTTON_X) || GetJoypadTrigger(PLAYER_2, BUTTON_Y) || GetJoypadTrigger(PLAYER_2, BUTTON_START)
                || GetJoypadTrigger(PLAYER_3, BUTTON_A) || GetJoypadTrigger(PLAYER_3, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_3, BUTTON_X) || GetJoypadTrigger(PLAYER_3, BUTTON_Y) || GetJoypadTrigger(PLAYER_3, BUTTON_START)
                || GetJoypadTrigger(PLAYER_4, BUTTON_A) || GetJoypadTrigger(PLAYER_4, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_4, BUTTON_X) || GetJoypadTrigger(PLAYER_4, BUTTON_Y) || GetJoypadTrigger(PLAYER_4, BUTTON_START))
            {
                StopSound(SOUND_LABEL_BGM_RESULT);	//���U���g��BGM
                PlaySound(SOUND_LABEL_SE_DECISION);	//���艹
                SetFade(MODE_TITLE);//�^�C�g����ʂɈړ�
            }
        }

        //�X�g�[���[�Ȃ�
        else
        {
            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_1, BUTTON_X) || GetJoypadTrigger(PLAYER_1, BUTTON_Y) || GetJoypadTrigger(PLAYER_1, BUTTON_START))
            {
                PlaySound(SOUND_LABEL_SE_DECISION);	//���艹
                SetFade(MODE_RANKING);//�����L���O��ʂɈړ�
            }
        }
    }
}

//=============================================================================
// ���U���g�I������
// Author:�㓡�T�V��
//=============================================================================
void UninitResult(void)
{
    //�X�R�A�̏I������
    UninitScore();

    //�e�N�X�`���̊J��
    for (int nCntResult = 0; nCntResult < RESULTSTATE_MAX; nCntResult++)
    {
        if (g_apTextureResult[nCntResult] != NULL)
        {
            g_apTextureResult[nCntResult]->Release();
            g_apTextureResult[nCntResult] = NULL;
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
// ���U���g�`�揈��
// Author:�㓡�T�V��
//=============================================================================
void DrawResult(void)
{
    //���_�t�H�[�}�b�g�̐ݒ�
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

    //�Q�[���I�[�o�[
    if (g_Winner == SELECT_MAX)
    {
        //�e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, g_apTextureResult[RESULTSTATE_GAMEOVER]);
        //�|���S���̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
    }

    //1P�̏���
    else if (g_Winner == SELECT_MOMO)
    {
        //�e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, g_apTextureResult[RESULTSTATE_WIN1]);
        //�|���S���̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
    }

    //2P�̏���
    else if (g_Winner == SELECT_URA)
    {
        //�e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, g_apTextureResult[RESULTSTATE_WIN2]);
        //�|���S���̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
    }

    //3P�̏���
    else if (g_Winner == SELECT_KIN)
    {
        //�e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, g_apTextureResult[RESULTSTATE_WIN3]);
        //�|���S���̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
    }

    //4P�̏���
    else if (g_Winner == SELECT_ISUN)
    {
        //�e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, g_apTextureResult[RESULTSTATE_WIN4]);
        //�|���S���̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
    }

    //�Q�[���N���A
    else if (g_Winner == SELECT_BOSS)
    {
        //�X�R�A��18���_�𒴂�����
        int nScore = GetScore();
        if (nScore > 180000)
        {
            //�e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, g_apTextureResult[RESULTSTATE_PERFECT]);
            //�|���S���̕`��
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
        }
        //�X�R�A��18���_�ȉ��Ȃ�
        else
        {
            //�e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, g_apTextureResult[RESULTSTATE_GAMECLEAR]);
            //�|���S���̕`��
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
        }
    }

    //�ݒ���擾
    SETTING *pSetting = GetSetting();

    //�X�g�[���[�Ȃ�
    if (pSetting->nMaxPlayer == 1)
    {
        //�X�R�A�̕`��
        DrawScore();
    }
}