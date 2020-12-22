//=============================================================================
//
// �ŋ��̘b�����̏��� [title.cpp] (�^�C�g��)
// Author:�㓡�T�V��
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "manual.h"
#include "sound.h"
#include "controller.h"
#include "petal.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_apTextureTitle[2] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//���_�o�b�t�@�ւ̃|�C���^

int g_nCounterTitle;//�A�j���[�V�����J�E���^�[
int g_nPatternEnter;//�A�j���[�V�����p�^�[��

int g_nCntSetPetal; //���̉Ԃт���܂��܂ł̃J�E���g

//=============================================================================
// �^�C�g������������
// Author:�㓡�T�V��
//=============================================================================
void InitTitle(void)
{
    //�����̎�̏�����
    srand((unsigned int)time(0));

    //�Ԃт��������
    InitPetal();

    //�ϐ��錾
    VERTEX_2D *pVtx = NULL;//���_���̃|�C���^
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/title000.png", &g_apTextureTitle[0]);
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/PRESS_ANY_BUTTON.png", &g_apTextureTitle[1]);

    //�O���[�o���ϐ��̏�����
    g_nCounterTitle = 0;
    g_nPatternEnter = 0;
    g_nCntSetPetal = SET_PETAL_FRAME;

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX * 2,//�o�b�t�@�T�C�Y
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
    //rhw�̐ݒ�
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    pVtx[4].rhw = 1.0f;
    pVtx[5].rhw = 1.0f;
    pVtx[6].rhw = 1.0f;
    pVtx[7].rhw = 1.0f;
    //���_�J���[�̐ݒ�
    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

    pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    //�e�N�X�`���̍��W
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

    //���_�f�[�^�A�����b�N
    g_pVtxBuffTitle->Unlock();

    PlaySound(SOUND_LABEL_BGM_TITLE);	//�^�C�g����BGM
}
//=============================================================================
// �^�C�g���X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateTitle(void)
{
    //�Ԃт���X�V
    UpdatePetal();

    //�Ԃт�֘A�̕ϐ�
    float fSpeedX = 0.0f;   //����X
    float fSpeedY = 0.0f;   //����Y
    float fSize = 0.0f;     //�傫��
    float fAlpha = 0.0f;    //�����x
    float fAngle = 0.0f;    //�p�x
    float fRotSpeed = 0.0f; //��]���x
    int nRotDirect;         //��]����

    //���̉Ԃт�����Ԋu�Őݒ�
    g_nCntSetPetal++;
    if(g_nCntSetPetal >= SET_PETAL_FRAME)
    {
        g_nCntSetPetal = 0;

        fSpeedX = float(rand() % 450 + 40) / 100.0f;
        fSpeedY = float(rand() % 270 + 40) / 100.0f;
        fSize = float(rand() % 4500 + 2000) / 100.0f;
        fAlpha = float(rand() % 100 + 65) / 100.0f;
        fAngle = float(rand() % 360 + 0) / 1.0f;
        fRotSpeed = float(rand() % 1000 + 5) / 10000.0f;
        nRotDirect = rand() % 2 + 1;

        //��]�����̐����𓾂�
        if (nRotDirect == 2)
        {
            nRotDirect = -1;
        }

        D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH + 50.0f, 0.0f , 0.0f);   //�����ʒu
        D3DXVECTOR3 move = D3DXVECTOR3(-fSpeedX, fSpeedY, 0.0f);            //�ړ���
        D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);                //�F

        SetPetal(pos, move, col, fSize, fAngle, fRotSpeed * nRotDirect,PETALTYPE_000); //�Ԃт��ݒ�
    }

    VERTEX_2D *pVtx = NULL;	//���_���ւ̃|�C���^
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

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffTitle->Unlock();

    //Enter�L�[����������
    if (nFade == FADE_NONE)
    {
        if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B) ||
            GetJoypadTrigger(PLAYER_1, BUTTON_X) || GetJoypadTrigger(PLAYER_1, BUTTON_Y) || GetJoypadTrigger(PLAYER_1, BUTTON_START))
        {
            StopSound(SOUND_LABEL_BGM_TITLE);	//�^�C�g����BGM���~�߂�
            PlaySound(SOUND_LABEL_SE_DECISION);	//���艹
            PlaySound(SOUND_LABEL_BGM_MENU);	//���j���[��BGM
            SetFade(MODE_MENU); //���j���[��ʂ�
        }
    }
}
//=============================================================================
// �^�C�g���I������
// Author:�㓡�T�V��
//=============================================================================
void UninitTitle(void)
{
    //�Ԃт�̃e�N�X�`�����J��
    UninitPetal();

    //�e�N�X�`���̊J��
    for (int nCntTitle = 0; nCntTitle < 2; nCntTitle++)
    {
        if (g_apTextureTitle[nCntTitle] != NULL)
        {
            g_apTextureTitle[nCntTitle]->Release();
            g_apTextureTitle[nCntTitle] = NULL;
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
// �^�C�g���`�揈��
// Author:�㓡�T�V��
//=============================================================================
void DrawTitle(void)
{
    //���_�t�H�[�}�b�g�̐ݒ�
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

    for (int nCntTitle = 0; nCntTitle < 2; nCntTitle++)
    {
        //�e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, g_apTextureTitle[nCntTitle]);
        //�|���S���̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTitle, NUM_POLYGON);
    }

    //�Ԃт��`��
    DrawPetal();

}