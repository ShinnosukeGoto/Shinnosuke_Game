//=============================================================================
//
// PLASTIC TROOPERS�̏��� [ranking.cpp] (�����L���O)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "ranking.h"
#include "input.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "title.h"
#include "score.h"
#include "controller.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureRanking[2] = {};//�e�N�X�`���ւ̃|�C���^(�g���e�N�X�`����2��)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;//���_�o�b�t�@�ւ̃|�C���^

int g_nRankingScore[RANKING_SCORE_MAX];		//��r����X�R�A
int g_nRank[RANKING_SCORE_MAX];             //�����N�t�����邽��
int g_nCampaignScore;                       //�Q�[�����[�h�̃X�R�A

//=============================================================================
// �����L���O����������
//=============================================================================
void InitRanking(void)
{
    //�ϐ��錾
    VERTEX_2D*pVtx;//���_���̃|�C���^
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�^�C�g���̃��[�h�I�����擾
    MODE Title = GetModeTitle();

    //�L�����y�[������J�ڂ����Ȃ�
    if (Title == MODE_CAMPAIGN)
    {
        g_nCampaignScore = GetScore();
    }

    //�����L���O����J�ڂ����Ȃ�
    else if (Title == MODE_RANKING)
    {
        g_nCampaignScore = 0;
    }

    //�X�R�A�̓ǂݍ���
    ReadScore();

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ranking000.png", &g_pTextureRanking[0]);//�X�R�A�̔w�i
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/number000.png", &g_pTextureRanking[1]);	//����

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX * /*RANKING_SCORE_MAX * MAX_SCORE + 4*/200,//�o�b�t�@�T�C�Y(+4�́A�����L���O�̔w�i�̕�)
        D3DUSAGE_WRITEONLY,//(�Œ�j
        FVF_VERTEX_2D,//���_�t�H�[�}�b�g
        D3DPOOL_MANAGED,//�i�Œ�j
        &g_pVtxBuffRanking,//�ϐ������ς��ƕύX���K�v
        NULL);

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    //���_���W�̐ݒ�i�E���Őݒ肷��j
    //�����L���O�̔w�i
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

    //1��
    for (int nResultScore = 0; nResultScore < MAX_SCORE; nResultScore++)
    {
        //���_���W�̐ݒ�i�E���Őݒ肷��j
        pVtx[4].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT, 0.0f);
        pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT, 0.0f);
        pVtx[6].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT, 0.0f);
        pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT, 0.0f);

        //rhw�̐ݒ�
        pVtx[4].rhw = 1.0f;
        pVtx[5].rhw = 1.0f;
        pVtx[6].rhw = 1.0f;
        pVtx[7].rhw = 1.0f;

        //�����N�C��������A�J���[��ς���
        if (g_nRank[RANKING_SCORE_MAX - 1] == 0)
        {
            //���_�J���[�̐ݒ�
            pVtx[4].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[5].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[6].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[7].col = D3DCOLOR_RGBA(255, 0, 0, 255);
        }
        else
        {
            //���_�J���[�̐ݒ�
            pVtx[4].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[5].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[6].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[7].col = D3DCOLOR_RGBA(0, 0, 0, 255);
        }

        //�e�N�X�`���̍��W
        pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[5].tex = D3DXVECTOR2(0.1f, 0.0f);
        pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[7].tex = D3DXVECTOR2(0.1f, 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
        }    
    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffRanking->Unlock();
    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    //2��
    for (int nResultScore = 0; nResultScore < MAX_SCORE; nResultScore++)
    {
        //���_���W�̐ݒ�i�E���Őݒ肷��j
        pVtx[36].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE, 0.0f);
        pVtx[37].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE, 0.0f);
        pVtx[38].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE, 0.0f);
        pVtx[39].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE, 0.0f);

        //rhw�̐ݒ�
        pVtx[36].rhw = 1.0f;
        pVtx[37].rhw = 1.0f;
        pVtx[38].rhw = 1.0f;
        pVtx[39].rhw = 1.0f;

        //�����N�C��������A�J���[��ς���
        if (g_nRank[RANKING_SCORE_MAX - 1] == 1)
        {
            //���_�J���[�̐ݒ�
            pVtx[36].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[37].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[38].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[39].col = D3DCOLOR_RGBA(255, 0, 0, 255);
        }
        else
        {
            //���_�J���[�̐ݒ�
            pVtx[36].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[37].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[38].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[39].col = D3DCOLOR_RGBA(0, 0, 0, 255);
        }

        //�e�N�X�`���̍��W
        pVtx[36].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[37].tex = D3DXVECTOR2(0.1f, 0.0f);
        pVtx[38].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[39].tex = D3DXVECTOR2(0.1f, 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
        }
    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffRanking->Unlock();
    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    //3��
    for (int nResultScore = 0; nResultScore < MAX_SCORE; nResultScore++)
    {
        //���_���W�̐ݒ�i�E���Őݒ肷��j
        pVtx[68].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 2, 0.0f);
        pVtx[69].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 2, 0.0f);
        pVtx[70].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 2, 0.0f);
        pVtx[71].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 2, 0.0f);

        //rhw�̐ݒ�
        pVtx[68].rhw = 1.0f;
        pVtx[69].rhw = 1.0f;
        pVtx[70].rhw = 1.0f;
        pVtx[71].rhw = 1.0f;

        //�����N�C��������A�J���[��ς���
        if (g_nRank[RANKING_SCORE_MAX - 1] == 2)
        {
            //���_�J���[�̐ݒ�
            pVtx[68].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[69].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[70].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[71].col = D3DCOLOR_RGBA(255, 0, 0, 255);
        }
        else
        {
            //���_�J���[�̐ݒ�
            pVtx[68].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[69].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[70].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[71].col = D3DCOLOR_RGBA(0, 0, 0, 255);
        }

        //�e�N�X�`���̍��W
        pVtx[68].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[69].tex = D3DXVECTOR2(0.1f, 0.0f);
        pVtx[70].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[71].tex = D3DXVECTOR2(0.1f, 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
        }
    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffRanking->Unlock();
    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    //4��
    for (int nResultScore = 0; nResultScore < MAX_SCORE; nResultScore++)
    {
        //���_���W�̐ݒ�i�E���Őݒ肷��j
        pVtx[100].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 3, 0.0f);
        pVtx[101].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 3, 0.0f);
        pVtx[102].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 3, 0.0f);
        pVtx[103].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 3, 0.0f);

        //rhw�̐ݒ�
        pVtx[100].rhw = 1.0f;
        pVtx[101].rhw = 1.0f;
        pVtx[102].rhw = 1.0f;
        pVtx[103].rhw = 1.0f;

        //�����N�C��������A�J���[��ς���
        if (g_nRank[RANKING_SCORE_MAX - 1] == 3)
        {
            //���_�J���[�̐ݒ�
            pVtx[100].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[101].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[102].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[103].col = D3DCOLOR_RGBA(255, 0, 0, 255);
        }
        else
        {
            //���_�J���[�̐ݒ�
            pVtx[100].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[101].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[102].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[103].col = D3DCOLOR_RGBA(0, 0, 0, 255);
        }

        //�e�N�X�`���̍��W
        pVtx[100].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[101].tex = D3DXVECTOR2(0.1f, 0.0f);
        pVtx[102].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[103].tex = D3DXVECTOR2(0.1f, 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
        }
    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffRanking->Unlock();
    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    //5��
    for (int nResultScore = 0; nResultScore < MAX_SCORE; nResultScore++)
    {
        //���_���W�̐ݒ�i�E���Őݒ肷��j
        pVtx[132].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 4, 0.0f);
        pVtx[133].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 4, 0.0f);
        pVtx[134].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 4, 0.0f);
        pVtx[135].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 4, 0.0f);

        //rhw�̐ݒ�
        pVtx[132].rhw = 1.0f;
        pVtx[133].rhw = 1.0f;
        pVtx[134].rhw = 1.0f;
        pVtx[135].rhw = 1.0f;

        //�����N�C��������A�J���[��ς���
        if (g_nRank[RANKING_SCORE_MAX - 1] == 4)
        {
            //���_�J���[�̐ݒ�
            pVtx[132].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[133].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[134].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[135].col = D3DCOLOR_RGBA(255, 0, 0, 255);
        }
        else
        {
            //���_�J���[�̐ݒ�
            pVtx[132].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[133].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[134].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[135].col = D3DCOLOR_RGBA(0, 0, 0, 255);
        }

        //�e�N�X�`���̍��W
        pVtx[132].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[133].tex = D3DXVECTOR2(0.1f, 0.0f);
        pVtx[134].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[135].tex = D3DXVECTOR2(0.1f, 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
        }
    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffRanking->Unlock();
    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    //�ϐ��錾
    float fIndex;		//�w���@(�X�R�A�\���p)
    float fRadix = 10.0f;	//��@(�X�R�A�\���p)

    //1�ʂ̃X�R�A�̌v�Z
    for (fIndex = 0; fIndex < MAX_SCORE; fIndex++)
    {
        int nResultScore = (int)powf(fRadix, MAX_SCORE - fIndex);
        int nResultScore2 = (int)powf(fRadix, MAX_SCORE - fIndex - 1);
        int nAnswer = g_nRankingScore[0] % nResultScore / nResultScore2;

        //�e�N�X�`���̍��W
        pVtx[32].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
        pVtx[33].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
        pVtx[34].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
        pVtx[35].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);


        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx -= 4;//���_�f�[�^�̃|�C���^��4���i�߂�
        }
    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffRanking->Unlock();
    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    //2�ʂ̃X�R�A�̌v�Z
    for (fIndex = 0; fIndex < MAX_SCORE; fIndex++)
    {
        int nResultScore = (int)powf(fRadix, MAX_SCORE - fIndex);
        int nResultScore2 = (int)powf(fRadix, MAX_SCORE - fIndex - 1);
        int nAnswer = g_nRankingScore[1] % nResultScore / nResultScore2;

        //�e�N�X�`���̍��W
        pVtx[64].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
        pVtx[65].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
        pVtx[66].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
        pVtx[67].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx -= 4;//���_�f�[�^�̃|�C���^��4���i�߂�
        }
    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffRanking->Unlock();
    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    //3�ʂ̃X�R�A�̌v�Z
    for (fIndex = 0; fIndex < MAX_SCORE; fIndex++)
    {
        int nResultScore = (int)powf(fRadix, MAX_SCORE - fIndex);
        int nResultScore2 = (int)powf(fRadix, MAX_SCORE - fIndex - 1);
        int nAnswer = g_nRankingScore[2] % nResultScore / nResultScore2;

        //�e�N�X�`���̍��W
        pVtx[96].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
        pVtx[97].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
        pVtx[98].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
        pVtx[99].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx -= 4;//���_�f�[�^�̃|�C���^��4���i�߂�
        }
    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffRanking->Unlock();
    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    //4�ʂ̃X�R�A�̌v�Z
    for (fIndex = 0; fIndex < MAX_SCORE; fIndex++)
    {
        int nResultScore = (int)powf(fRadix, MAX_SCORE - fIndex);
        int nResultScore2 = (int)powf(fRadix, MAX_SCORE - fIndex - 1);
        int nAnswer = g_nRankingScore[3] % nResultScore / nResultScore2;

        //�e�N�X�`���̍��W
        pVtx[128].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
        pVtx[129].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
        pVtx[130].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
        pVtx[131].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx -= 4;//���_�f�[�^�̃|�C���^��4���i�߂�
        }
    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffRanking->Unlock();
    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    //5�ʂ̃X�R�A�̌v�Z
    for (fIndex = 0; fIndex < MAX_SCORE; fIndex++)
    {
        int nResultScore = (int)powf(fRadix, MAX_SCORE - fIndex);
        int nResultScore2 = (int)powf(fRadix, MAX_SCORE - fIndex - 1);
        int nAnswer = g_nRankingScore[4] % nResultScore / nResultScore2;

        //�e�N�X�`���̍��W
        pVtx[160].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
        pVtx[161].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
        pVtx[162].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
        pVtx[163].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx -= 4;//���_�f�[�^�̃|�C���^��4���i�߂�
        }
    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffRanking->Unlock();

    //PlaySound(SOUND_LABEL_BGM003);	//���U���g��
}

//=============================================================================
// �����L���O�X�V����
//=============================================================================
void UpdateRanking(void)
{
    int nFade = GetFade();	//�t�F�[�h�̏����l��

                            //Enter�L�[����������
    if (nFade == FADE_NONE)
    {
        if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(BUTTON_A) || GetJoypadTrigger(BUTTON_B) || GetJoypadTrigger(BUTTON_START))
        {
            //PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾���Ńv���X�G���^�[���̑�p
            SetFade(MODE_TITLE);//�^�C�g����ʂɈړ�
                                   //StopSound(SOUND_LABEL_BGM000);//�Ȃ̒�~(����̕�)
        }
    }
}
//=============================================================================
// �����L���O�I������
//=============================================================================
void UninitRanking(void)
{
    //�e�N�X�`���̊J��
    for (int nCntResultScore = 0; nCntResultScore < 2; nCntResultScore++)
    {
        if (g_pTextureRanking[nCntResultScore] != NULL)
        {
            g_pTextureRanking[nCntResultScore]->Release();
            g_pTextureRanking[nCntResultScore] = NULL;
        }
    }
    //���_�o�b�t�@�̊J��
    if (g_pVtxBuffRanking != NULL)
    {
        g_pVtxBuffRanking->Release();
        g_pVtxBuffRanking = NULL;
    }
}

//=============================================================================
// �����L���O�`�揈��
//=============================================================================
void DrawRanking(void)
{
    //���_�t�H�[�}�b�g�̐ݒ�
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));
    pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

    //�����L���O�̔w�i
    //�e�N�X�`���̐ݒ�
    pDevice->SetTexture(0, g_pTextureRanking[0]);
    //���U���g�w�i�̕`��
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);

    //�X�R�A�̐����̕`��
    for (int nResultScore = 1; nResultScore < /*RANKING_SCORE_MAX * MAX_SCORE + 1*/200; nResultScore++)	//pVtx0�`3��ǂݍ��܂Ȃ��悤�ɂ��邽�߁A1����for���n�߂�
    {
        //�e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, g_pTextureRanking[1]);
        //�����̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nResultScore, NUM_POLYGON);
    }

}

//==============================================================
// �X�R�A�̓ǂݍ���
//==============================================================
void ReadScore(void)
{
    FILE *pFile = NULL; //�t�@�C���|�C���^

    //�o�͏���
    pFile = fopen("score.txt", "r");

    if (pFile != NULL)
    {//�t�@�C���I�[�v������
        for (int nCntScore = 0; nCntScore < RANKING_SCORE_MAX; nCntScore++)
        { //��ʃX�R�A�̏o�͏���
            fscanf(pFile, "%d", &g_nRankingScore[nCntScore]);
        }
        fclose(pFile);
    }

    g_nRankingScore[RANKING_SCORE_MAX-1] = g_nCampaignScore;

    //�����N�\�[�g
    for (int nCntScore = 0; nCntScore < RANKING_SCORE_MAX; nCntScore++)
    {
        g_nRank[nCntScore] = 0;
        for (int nData = 0; nData < RANKING_SCORE_MAX; nData++)
        {
            if (g_nRankingScore[nCntScore] < g_nRankingScore[nData])
            {
                g_nRank[nCntScore]++;
            }
        }
    }

    //�X�R�A���͏���
    pFile = fopen("score.txt", "w");
    if (pFile != NULL)
    {//�t�@�C���I�[�v������
        for (int nCntRank = 0; nCntRank < RANKING_SCORE_MAX; nCntRank++)
        {
            for (int nCntScore = 0; nCntScore < RANKING_SCORE_MAX; nCntScore++)
            {
                if (g_nRank[nCntScore] == nCntRank)
                {
                    fprintf(pFile, "%d\n", g_nRankingScore[nCntScore]);
                }
            }
        }
        fclose(pFile);
    }

    //�X�R�A�o�͏���
    pFile = fopen("score.txt", "r");
    if (pFile != NULL)
    {//�t�@�C���I�[�v������
        for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
        { //��ʃX�R�A�̏o�͏���
            fscanf(pFile, "%d", &g_nRankingScore[nCntScore]);
        }
        fclose(pFile);
    }

}