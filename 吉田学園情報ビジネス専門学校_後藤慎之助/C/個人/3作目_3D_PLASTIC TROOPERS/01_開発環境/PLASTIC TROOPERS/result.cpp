//=============================================================================
//
// PLASTIC TROOPERS�̏��� [result.cpp] (�Q�[���N���A���Q�[���I�[�o�[)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "result.h"
#include "player.h"
//#include "enemy.h"
#include "input.h"
#include "campaign.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "title.h"
#include "controller.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureResult[2] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;//���_�o�b�t�@�ւ̃|�C���^

PLAYER*g_pPlayer;
//MODE g_NextModeResult;  //���U���g�̎��̃��[�h

//=============================================================================
// ���U���g����������
//=============================================================================
void InitResult(void)
{
    //�ϐ��錾
    VERTEX_2D*pVtx;//���_���̃|�C���^
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    //�n���^�[�̏󋵂��擾
    g_pPlayer = GetPlayer();

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX * 1,//�o�b�t�@�T�C�Y
        D3DUSAGE_WRITEONLY,//(�Œ�j
        FVF_VERTEX_2D,//���_�t�H�[�}�b�g
        D3DPOOL_MANAGED,//�i�Œ�j
        &g_pVtxBuffResult,//�ϐ������ς��ƕύX���K�v
        NULL);

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/gameclear000.png", &g_pTextureResult[0]);	//�Q�[���N���A
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/gameover000.png", &g_pTextureResult[1]);	//�Q�[���I�[�o�[

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

    if (nFade == FADE_NONE)
    {
        if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(BUTTON_A) || GetJoypadTrigger(BUTTON_B) ||
            GetJoypadTrigger(BUTTON_X) || GetJoypadTrigger(BUTTON_Y) || GetJoypadTrigger(BUTTON_START))
        {
            //PlaySound(SOUND_LABEL_SE_ENTER);	//�v���X�G���^�[��
            //InitResult2();		//���U���g2��������
            SetFade(MODE_RANKING);//�����L���O��ʂɈړ�
        }
    }

}
//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
    //�e�N�X�`���̊J��
    for (int nCntResult = 0; nCntResult < 2; nCntResult++)
    {
        if (g_pTextureResult[nCntResult] != NULL)
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
    //���_�t�H�[�}�b�g�̐ݒ�
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

    //�n���^�[���������Ă���Ȃ�A�Q�[���N���A
    if (g_pPlayer->bDisp == true)
    {
        //�e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, g_pTextureResult[0]);
        //�|���S���̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
    }

    //�n���^�[���S�ł��Ă���Ȃ�Q�[���I�[�o�[
    if (g_pPlayer->bDisp == false)
    {
        //�e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, g_pTextureResult[1]);
        //�|���S���̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
    }
}