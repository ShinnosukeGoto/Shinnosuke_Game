//=============================================================================
//
// �ŋ��̘b�����̏��� [manual.cpp] (�V�ѕ�)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "input.h"
#include "game.h"
#include "manual.h"
#include "fade.h"
#include "controller.h"
#include "sound.h"
#include "setting.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureManual = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManual = NULL;//���_�o�b�t�@�ւ̃|�C���^

bool g_bUseHiddenCommand;  //�B���R�}���h�̎g�p����
bool g_bHiddenCommand1L;   //�B���R�}���h1
bool g_bHiddenCommand2R;   //�B���R�}���h2
bool g_bHiddenCommand3L;   //�B���R�}���h3
bool g_bHiddenCommand4R;   //�B���R�}���h4
bool g_bHiddenCommand5X;   //�B���R�}���h5
bool g_bHiddenCommand6X;   //�B���R�}���h6
bool g_bHiddenCommand7Y;   //�B���R�}���h7

bool g_bUseHiddenCommand2;  //�B���R�}���h�̎g�p����
bool g_bHiddenCommand1L2;   //�B���R�}���h1
bool g_bHiddenCommand2R2;   //�B���R�}���h2
bool g_bHiddenCommand3L2;   //�B���R�}���h3
bool g_bHiddenCommand4R2;   //�B���R�}���h4
bool g_bHiddenCommand5X2;   //�B���R�}���h5
bool g_bHiddenCommand6X2;   //�B���R�}���h6
bool g_bHiddenCommand7Y2;   //�B���R�}���h7

bool g_bUseHiddenCommand3;  //�B���R�}���h�̎g�p����
bool g_bHiddenCommand1L3;   //�B���R�}���h1
bool g_bHiddenCommand2R3;   //�B���R�}���h2
bool g_bHiddenCommand3L3;   //�B���R�}���h3
bool g_bHiddenCommand4R3;   //�B���R�}���h4
bool g_bHiddenCommand5X3;   //�B���R�}���h5
bool g_bHiddenCommand6X3;   //�B���R�}���h6
bool g_bHiddenCommand7Y3;   //�B���R�}���h7

bool g_bUseHiddenCommand4;  //�B���R�}���h�̎g�p����
bool g_bHiddenCommand1L4;   //�B���R�}���h1
bool g_bHiddenCommand2R4;   //�B���R�}���h2
bool g_bHiddenCommand3L4;   //�B���R�}���h3
bool g_bHiddenCommand4R4;   //�B���R�}���h4
bool g_bHiddenCommand5X4;   //�B���R�}���h5
bool g_bHiddenCommand6X4;   //�B���R�}���h6
bool g_bHiddenCommand7Y4;   //�B���R�}���h7

//=============================================================================
// �}�j���A������������
// Author:�㓡�T�V��
//=============================================================================
void InitManual(void)
{
    //�ϐ��錾
    VERTEX_2D *pVtx = NULL;//���_���̃|�C���^
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�B���R�}���h�̏�����
    g_bUseHiddenCommand = false;
    g_bHiddenCommand1L = false;
    g_bHiddenCommand2R = false;
    g_bHiddenCommand3L = false;
    g_bHiddenCommand4R = false;
    g_bHiddenCommand5X = false;
    g_bHiddenCommand6X = false;
    g_bHiddenCommand7Y = false;

    g_bUseHiddenCommand2 = false;
    g_bHiddenCommand1L2 = false;
    g_bHiddenCommand2R2 = false;
    g_bHiddenCommand3L2 = false;
    g_bHiddenCommand4R2 = false;
    g_bHiddenCommand5X2 = false;
    g_bHiddenCommand6X2 = false;
    g_bHiddenCommand7Y2 = false;

    g_bUseHiddenCommand3 = false;
    g_bHiddenCommand1L3 = false;
    g_bHiddenCommand2R3 = false;
    g_bHiddenCommand3L3 = false;
    g_bHiddenCommand4R3 = false;
    g_bHiddenCommand5X3 = false;
    g_bHiddenCommand6X3 = false;
    g_bHiddenCommand7Y3 = false;

    g_bUseHiddenCommand4 = false;
    g_bHiddenCommand1L4 = false;
    g_bHiddenCommand2R4 = false;
    g_bHiddenCommand3L4 = false;
    g_bHiddenCommand4R4 = false;
    g_bHiddenCommand5X4 = false;
    g_bHiddenCommand6X4 = false;
    g_bHiddenCommand7Y4 = false;

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/manual000.png", &g_pTextureManual);

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX,//�o�b�t�@�T�C�Y
        D3DUSAGE_WRITEONLY,//(�Œ�j
        FVF_VERTEX_2D,//���_�t�H�[�}�b�g
        D3DPOOL_MANAGED,//�i�Œ�j
        &g_pVtxBuffManual,//�ϐ������ς��ƕύX���K�v
        NULL);

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffManual->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

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
    g_pVtxBuffManual->Unlock();
}

//=============================================================================
// �}�j���A���X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateManual(void)
{
    int nFade = GetFade();	            //�t�F�[�h�̏����l��
    SETTING *pSetting = GetSetting();   //�ݒ�̏����擾

    //�o�g�����C�����Ȃ�A�B���R�}���h���g����
    if (pSetting->nMaxPlayer >= 2)
    {
        //====================================
        //�v���C���[1
        //====================================
        //8��Y
        if (g_bHiddenCommand7Y == true)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_Y))
            {
                g_bUseHiddenCommand = true; //�B���R�}���h�K�p
            }
        }

        //7��Y
        if (g_bHiddenCommand6X == true)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_Y))
            {
                g_bHiddenCommand7Y = true;
            }
        }

        //6��X
        if (g_bHiddenCommand5X == true)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_X))
            {
                g_bHiddenCommand6X = true;
            }
        }

        //5��X
        if (g_bHiddenCommand4R == true)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_X))
            {
                g_bHiddenCommand5X = true;
            }
        }

        //4��R
        if (g_bHiddenCommand3L == true)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_R1) || GetJoypadTrigger(PLAYER_1, BUTTON_R2))
            {
                g_bHiddenCommand4R = true;
            }
        }

        //3��L
        if (g_bHiddenCommand2R == true)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_L1) || GetJoypadTrigger(PLAYER_1, BUTTON_L2))
            {
                g_bHiddenCommand3L = true;
            }
        }

        //2��R
        if (g_bHiddenCommand1L == true)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_R1) || GetJoypadTrigger(PLAYER_1, BUTTON_R2))
            {
                g_bHiddenCommand2R = true;
            }
        }

        //1��L
        if (g_bHiddenCommand1L == false)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_L1) || GetJoypadTrigger(PLAYER_1, BUTTON_L2))
            {
                g_bHiddenCommand1L = true;
            }
        }

        //====================================
        //�v���C���[2
        //====================================
        //8��Y
        if (g_bHiddenCommand7Y2 == true)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_Y))
            {
                g_bUseHiddenCommand2 = true; //�B���R�}���h�K�p
            }
        }

        //7��Y
        if (g_bHiddenCommand6X2 == true)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_Y))
            {
                g_bHiddenCommand7Y2 = true;
            }
        }

        //6��X
        if (g_bHiddenCommand5X2 == true)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_X))
            {
                g_bHiddenCommand6X2 = true;
            }
        }

        //5��X
        if (g_bHiddenCommand4R2 == true)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_X))
            {
                g_bHiddenCommand5X2 = true;
            }
        }

        //4��R
        if (g_bHiddenCommand3L2 == true)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_R1) || GetJoypadTrigger(PLAYER_2, BUTTON_R2))
            {
                g_bHiddenCommand4R2 = true;
            }
        }

        //3��L
        if (g_bHiddenCommand2R2 == true)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_L1) || GetJoypadTrigger(PLAYER_2, BUTTON_L2))
            {
                g_bHiddenCommand3L2 = true;
            }
        }

        //2��R
        if (g_bHiddenCommand1L2 == true)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_R1) || GetJoypadTrigger(PLAYER_2, BUTTON_R2))
            {
                g_bHiddenCommand2R2 = true;
            }
        }

        //1��L
        if (g_bHiddenCommand1L2 == false)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_L1) || GetJoypadTrigger(PLAYER_2, BUTTON_L2))
            {
                g_bHiddenCommand1L2 = true;
            }
        }

        //====================================
        //�v���C���[3
        //====================================
        //8��Y
        if (g_bHiddenCommand7Y3 == true)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_Y))
            {
                g_bUseHiddenCommand3 = true; //�B���R�}���h�K�p
            }
        }

        //7��Y
        if (g_bHiddenCommand6X3 == true)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_Y))
            {
                g_bHiddenCommand7Y3 = true;
            }
        }

        //6��X
        if (g_bHiddenCommand5X3 == true)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_X))
            {
                g_bHiddenCommand6X3 = true;
            }
        }

        //5��X
        if (g_bHiddenCommand4R3 == true)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_X))
            {
                g_bHiddenCommand5X3 = true;
            }
        }

        //4��R
        if (g_bHiddenCommand3L3 == true)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_R1) || GetJoypadTrigger(PLAYER_3, BUTTON_R2))
            {
                g_bHiddenCommand4R3 = true;
            }
        }

        //3��L
        if (g_bHiddenCommand2R3 == true)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_L1) || GetJoypadTrigger(PLAYER_3, BUTTON_L2))
            {
                g_bHiddenCommand3L3 = true;
            }
        }

        //2��R
        if (g_bHiddenCommand1L3 == true)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_R1) || GetJoypadTrigger(PLAYER_3, BUTTON_R2))
            {
                g_bHiddenCommand2R3 = true;
            }
        }

        //1��L
        if (g_bHiddenCommand1L3 == false)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_L1) || GetJoypadTrigger(PLAYER_3, BUTTON_L2))
            {
                g_bHiddenCommand1L3 = true;
            }
        }

        //====================================
        //�v���C���[4
        //====================================
        //8��Y
        if (g_bHiddenCommand7Y4 == true)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_Y))
            {
                g_bUseHiddenCommand4 = true; //�B���R�}���h�K�p
            }
        }

        //7��Y
        if (g_bHiddenCommand6X4 == true)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_Y))
            {
                g_bHiddenCommand7Y4 = true;
            }
        }

        //6��X
        if (g_bHiddenCommand5X4 == true)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_X))
            {
                g_bHiddenCommand6X4 = true;
            }
        }

        //5��X
        if (g_bHiddenCommand4R4 == true)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_X))
            {
                g_bHiddenCommand5X4 = true;
            }
        }

        //4��R
        if (g_bHiddenCommand3L4 == true)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_R1) || GetJoypadTrigger(PLAYER_4, BUTTON_R2))
            {
                g_bHiddenCommand4R4 = true;
            }
        }

        //3��L
        if (g_bHiddenCommand2R4 == true)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_L1) || GetJoypadTrigger(PLAYER_4, BUTTON_L2))
            {
                g_bHiddenCommand3L4 = true;
            }
        }

        //2��R
        if (g_bHiddenCommand1L4 == true)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_R1) || GetJoypadTrigger(PLAYER_4, BUTTON_R2))
            {
                g_bHiddenCommand2R4 = true;
            }
        }

        //1��L
        if (g_bHiddenCommand1L4 == false)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_L1) || GetJoypadTrigger(PLAYER_4, BUTTON_L2))
            {
                g_bHiddenCommand1L4 = true;
            }
        }
    }

    //Enter�L�[����������
    if (nFade == FADE_NONE)
    {
        if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
            || GetJoypadTrigger(PLAYER_1, BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_START))
        {
            StopSound(SOUND_LABEL_BGM_MENU);	//���j���[��BGM
            PlaySound(SOUND_LABEL_SE_DECISION);	//���艹
            SetFade(MODE_GAME);//�Q�[����ʂɈړ�
        }
    }

}

//=============================================================================
// �}�j���A���I������
// Author:�㓡�T�V��
//=============================================================================
void UninitManual(void)
{
    //�e�N�X�`���̊J��
    if (g_pTextureManual != NULL)
    {
        g_pTextureManual->Release();
        g_pTextureManual = NULL;
    }
    //���_�o�b�t�@�̊J��
    if (g_pVtxBuffManual != NULL)
    {
        g_pVtxBuffManual->Release();
        g_pVtxBuffManual = NULL;
    }
}

//=============================================================================
// �}�j���A���`�揈��
// Author:�㓡�T�V��
//=============================================================================
void DrawManual(void)
{
    //���_�t�H�[�}�b�g�̐ݒ�
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffManual, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

                                   //�e�N�X�`���̐ݒ�
    pDevice->SetTexture(0, g_pTextureManual);

    //�|���S���̕`��
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
}

//=============================================================================
// �B���R�}���h����
// Author:�㓡�T�V��
//=============================================================================
bool GetHiddenCommand(void)
{
    return g_bUseHiddenCommand;
}

//=============================================================================
// �B���R�}���h2����
// Author:�㓡�T�V��
//=============================================================================
bool GetHiddenCommand2(void)
{
    return g_bUseHiddenCommand2;
}

//=============================================================================
// �B���R�}���h3����
// Author:�㓡�T�V��
//=============================================================================
bool GetHiddenCommand3(void)
{
    return g_bUseHiddenCommand3;
}

//=============================================================================
// �B���R�}���h4����
// Author:�㓡�T�V��
//=============================================================================
bool GetHiddenCommand4(void)
{
    return g_bUseHiddenCommand4;
}