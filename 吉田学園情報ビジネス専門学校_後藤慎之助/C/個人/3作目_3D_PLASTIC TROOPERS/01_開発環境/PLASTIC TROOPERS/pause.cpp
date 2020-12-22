//=============================================================================
//
// PLASTIC TROOPERS�̏��� [pause.cpp] (�|�[�Y�@�\)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "pause.h"
#include "fade.h"
#include "controller.h"
#include "campaign.h"
#include "sound.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTexturePause[3] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;//���_�o�b�t�@�ւ̃|�C���^

PAUSE g_Pause;  //�|�[�Y���̑I����

//=============================================================================
// ���U���g����������
//=============================================================================
void InitPause(void)
{
    //�ϐ��錾
    VERTEX_2D*pVtx = NULL;//���_���̃|�C���^
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�O���[�o���ϐ��̏�����
    g_Pause = PAUSE_CONTINUE_GAME;

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX * 3,//�o�b�t�@�T�C�Y
        D3DUSAGE_WRITEONLY,//(�Œ�j
        FVF_VERTEX_2D,//���_�t�H�[�}�b�g
        D3DPOOL_MANAGED,//�i�Œ�j
        &g_pVtxBuffPause,//�ϐ������ς��ƕύX���K�v
        NULL);

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/pause000.png", &g_pTexturePause[0]);	//�|�[�Y�̃^�C�g���Ɣw�i
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/pause001.png", &g_pTexturePause[1]);	//�Q�[�����Â���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/pause002.png", &g_pTexturePause[2]);	//�Q�[������߂�

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    //���_���W�̐ݒ�i�E���Őݒ肷��j
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    //�Q�[�����Â���
    pVtx[4].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (460 / 2), (SCREEN_HEIGHT / 1.5) - (80 / 2) + 46, 0.0f);
    pVtx[5].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (460 / 2), (SCREEN_HEIGHT / 1.5) - (80 / 2) + 46, 0.0f);
    pVtx[6].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (460 / 2), (SCREEN_HEIGHT / 1.5) + (80 / 2) + 46, 0.0f);
    pVtx[7].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (460 / 2), (SCREEN_HEIGHT / 1.5) + (80 / 2) + 46, 0.0f);

    pVtx[8].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (460 / 2), (SCREEN_HEIGHT / 1.5) - (80 / 2) + 146, 0.0f);
    pVtx[9].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (460 / 2), (SCREEN_HEIGHT / 1.5) - (80 / 2) + 146, 0.0f);
    pVtx[10].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (460 / 2), (SCREEN_HEIGHT / 1.5) + (80 / 2) + 146, 0.0f);
    pVtx[11].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (460 / 2), (SCREEN_HEIGHT / 1.5) + (80 / 2) + 146, 0.0f);

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

    //���_�f�[�^�A�����b�N
    g_pVtxBuffPause->Unlock();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePause(void)
{
    int nFade = GetFade();	//�t�F�[�h�̏����l��
    DIJOYSTATE2 Controller = GetController();  //�R���g���[���[�̏����擾
    VERTEX_2D *pVtx = NULL;

    g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    if (nFade == FADE_NONE)
    {
        //�I���������(�Q�[�����Â���)
        if (Controller.lY < 0 || Controller.rgdwPOV[0] == BUTTON_UP)
        {
            g_Pause = PAUSE_CONTINUE_GAME;
        }

        //�I����������(�Q�[������߂�)
        else if (Controller.lY > 0 || Controller.rgdwPOV[0] == BUTTON_DOWN)
        {
            g_Pause = PAUSE_QUIT_GAME;
        }

        //A/B�{�^���őI��
        if (GetJoypadTrigger(BUTTON_A) || GetJoypadTrigger(BUTTON_B))
        {
            //�Q�[�����Â���
            if (g_Pause == PAUSE_CONTINUE_GAME)
            {
                //���[�h���擾
                MODE mode = GetMode();

                //�L�����y�[��
                if (mode == MODE_CAMPAIGN)
                {
                    SetCampaignState(CAMPAIGNSTATE_NORMAL);
                }
            }

            //�Q�[������߂�
            else
            {
                //�Q�[����BGM���Ƃ߂�
                StopSound(SOUND_LABEL_BGM001);
                StopSound(SOUND_LABEL_BGM004);
                //�^�C�g����
                SetFade(MODE_TITLE);
            }
        }

        //�X�^�[�g�{�^���ŃQ�[���ɖ߂�
        else if (GetJoypadTrigger(BUTTON_START))
        {
            //���[�h���擾
            MODE mode = GetMode();

            //�L�����y�[��
            if (mode == MODE_CAMPAIGN)
            {
                g_Pause = PAUSE_CONTINUE_GAME;  //�J�[�\�����Q�[�����Â���ɖ߂�
                SetCampaignState(CAMPAIGNSTATE_NORMAL);//�|�[�Y����
            }
        }

        //�I�����̓����x��ς���
        switch (g_Pause)
        {
        case PAUSE_CONTINUE_GAME:

            //�Q�[�����Â���𖾂邭
            pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

            //�Q�[������߂���Â�
            pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
            pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
            pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
            pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

            break;

        case PAUSE_QUIT_GAME:

            //�Q�[�����Â�����Â�
            pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
            pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
            pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
            pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

            //�Q�[������߂�𖾂邭
            pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

            break;
        }

    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffPause->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitPause(void)
{
    //�e�N�X�`���̊J��
    for (int nCntPause = 0; nCntPause < 3; nCntPause++)
    {
        if (g_pTexturePause[nCntPause] != NULL)
        {
            g_pTexturePause[nCntPause]->Release();
            g_pTexturePause[nCntPause] = NULL;
        }
    }

    //���_�o�b�t�@�̊J��
    if (g_pVtxBuffPause != NULL)
    {
        g_pVtxBuffPause->Release();
        g_pVtxBuffPause = NULL;
    }
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPause(void)
{
    //���_�t�H�[�}�b�g�̐ݒ�
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�
 
    for (int nCntPause = 0; nCntPause < 3; nCntPause++)
    {
        //�e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, g_pTexturePause[nCntPause]);
        //�|���S���̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntPause, NUM_POLYGON);
    }

}