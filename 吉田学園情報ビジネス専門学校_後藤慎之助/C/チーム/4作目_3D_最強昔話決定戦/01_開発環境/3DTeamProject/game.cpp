//=============================================================================
//
// �ŋ��̘b�����̏��� [game.cpp] (�Q�[�����)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "game.h"
#include "sound.h"
#include "fade.h"
#include "stage.h"
#include "block.h"
#include "camera.h"
#include "light.h"
#include "meshfield.h"
#include "player.h"
#include "player2.h"
#include "player3.h"
#include "player4.h"
#include "bomb.h"
#include "fire.h"
#include "item.h"
#include "effect.h"
#include "pause.h"
#include "controller.h"
#include "ui.h"
#include "time.h"
#include "shadow.h"
#include "setting.h"
#include "score.h"
#include "enemy.h"
#include "bullet.h"
#include "boss.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================

//�u���b�N�̃����_���z�u
int g_nCntBrokenBlock;  //�����̉󂹂�u���b�N�̐�
int g_nCntFireBlock;    //���o���u���b�N�̐�
int g_nCntBombBlock;    //�{���o���u���b�N�̐�
int g_nCntSpeedBlock;   //�X�s�[�h�o���u���b�N�̐�
int g_nCntKickBlock;    //�L�b�N�o���u���b�N�̐�

//�Q�[���̊Ǘ�
GAME g_Game;            //�Q�[���̏��
bool g_bFinishRound;    //���E���h�I������
bool g_bFinishGame;     //�Q�[���I������
int g_nCunterGamestate;	//���̉�ʂ֍s���܂ł̎���

//=============================================================================
// �Q�[������������
// Author:�㓡�T�V��
//=============================================================================
void InitGame(void)
{
    //�e�v���C���[�̃|�C���g�̏�����(�ŏ��̂�)
    g_Game.nCntPointPlayer1 = 0;
    g_Game.nCntPointPlayer2 = 0;
    g_Game.nCntPointPlayer3 = 0;
    g_Game.nCntPointPlayer4 = 0;

    //��l�p���[�h��I�����Ă��A�ݒ肪�ς��Ȃ��悤�ɁA�ݒ���L��
    //�ݒ�̏��擾
    SETTING *pSetting = GetSetting();
    g_Game.nMemoryTimeLimit = pSetting->nTimeLimit;
    g_Game.bMemoryRandomStartPos = pSetting->bRandomStartPos;
    g_Game.bMemorySuddenDeath = pSetting->bSuddenDeath;

    ResetScore();   //�X�R�A�̏�����
    ResetGame();    //�Q�[���̏�����
}

//=============================================================================
// ���̃��E���h�ւ̏���������
// Author:�㓡�T�V��
//=============================================================================
void InitNextGame(void)
{
    ResetScore();   //�X�R�A�̏�����(�o�g�����C�����ł̓X�R�A������Ȃ��̂ŏ�����)
    ResetGame();    //�Q�[���̏�����
}

//=============================================================================
// �Q�[���X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateGame(void)
{
    //�|�[�Y�̏��擾
    PAUSE *pPause = GetPause();

    //�ݒ�̏��擾
    SETTING *pSetting = GetSetting();

    switch (g_Game.state)
    {
        //�J�n���
    case GAMESTATE_START:
        UpdateCamera();
        UpdateLight();
        UpdateBlock();
        UpdateBomb();
        UpdateFire();
        UpdateItem();
        UpdateEffect();
        UpdatePlayer();
        if (pSetting->nMaxPlayer >= 2)
        {
            UpdatePlayer2();
            if (pSetting->nMaxPlayer >= 3)
            {
                UpdatePlayer3();
                if (pSetting->nMaxPlayer >= 4)
                {
                    UpdatePlayer4();
                }
            }
        }
        UpdateUI();
        UpdateShadow();
        UpdateScore();
        //�X�g�[���[�Ȃ�
        if (pSetting->nMaxPlayer == 1)
        {
            UpdateEnemy();
            UpdateBullet();
            UpdateBoss();
        }

        g_nCunterGamestate++;
        if (g_nCunterGamestate >= COUNT_WAIT_START_GAME)
        {
            SetGameState(GAMESTATE_NORMAL);    //�Q�[�����ɂ���
                     
            PlaySound(SOUND_LABEL_SE_START);    //�X�^�[�g�̓J
            PlaySound(SOUND_LABEL_BGM_GAME);    //�Q�[����BGM
        }
        break;

        //�Q�[����
    case GAMESTATE_NORMAL:
        UpdateCamera();
        UpdateLight();
        UpdateBlock();
        UpdateBomb();
        UpdateFire();
        UpdateItem();
        UpdateEffect();
        UpdatePlayer();
        if (pSetting->nMaxPlayer >= 2)
        {
            UpdatePlayer2();
            if (pSetting->nMaxPlayer >= 3)
            {
                UpdatePlayer3();
                if (pSetting->nMaxPlayer >= 4)
                {
                    UpdatePlayer4();
                }
            }
        }
        UpdateUI();
        UpdateTime();
        UpdateShadow();
        UpdateScore();
        //�X�g�[���[�Ȃ�
        if (pSetting->nMaxPlayer == 1)
        {
            UpdateEnemy();
            UpdateBullet();
            UpdateBoss();
        }

        //�|�[�Y��
        if (GetJoypadTrigger(PLAYER_1, BUTTON_START))
        {
            PlaySound(SOUND_LABEL_SE_DECISION);   //���艹
            pPause->who = SELECT_MOMO;
            g_Game.state = GAMESTATE_PAUSE;
        }
        else if (GetJoypadTrigger(PLAYER_2, BUTTON_START))
        {
            if (pSetting->nMaxPlayer >= 2)
            {
                PlaySound(SOUND_LABEL_SE_DECISION);   //���艹
                pPause->who = SELECT_URA;
                g_Game.state = GAMESTATE_PAUSE;
            }
        }
        else if (GetJoypadTrigger(PLAYER_3, BUTTON_START))
        {
            if (pSetting->nMaxPlayer >= 3)
            {
                PlaySound(SOUND_LABEL_SE_DECISION);   //���艹
                pPause->who = SELECT_KIN;
                g_Game.state = GAMESTATE_PAUSE;
            }
        }
        else if (GetJoypadTrigger(PLAYER_4, BUTTON_START))
        {
            if (pSetting->nMaxPlayer >= 4)
            {
                PlaySound(SOUND_LABEL_SE_DECISION);   //���艹
                pPause->who = SELECT_ISUN;
                g_Game.state = GAMESTATE_PAUSE;
            }
        }

        break;

    	//���̃��E���h��
    case GAMESTATE_FINSIH_ROUND:
        UpdateCamera();
        UpdateLight();
        UpdateBlock();
        UpdateBomb();
        UpdateFire();
        UpdateItem();
        UpdateEffect();
        UpdatePlayer();
        if (pSetting->nMaxPlayer >= 2)
        {
            UpdatePlayer2();
            if (pSetting->nMaxPlayer >= 3)
            {
                UpdatePlayer3();
                if (pSetting->nMaxPlayer >= 4)
                {
                    UpdatePlayer4();
                }
            }
        }
        UpdateUI();
        UpdateShadow();
        UpdateScore();
        //�X�g�[���[�Ȃ�
        if (pSetting->nMaxPlayer == 1)
        {
            UpdateEnemy();
            UpdateBullet();
            UpdateBoss();
        }

    	g_nCunterGamestate++;
    	if (g_nCunterGamestate >= COUNT_WAIT_FINISH_GAME)
    	{
            g_Game.state = GAMESTATE_NONE;
    		SetFade(MODE_NEXT_GAME);                    //���̃��E���h��
            StopSound(SOUND_LABEL_BGM_GAME);            //�Q�[���Ȃ��~
            StopSound(SOUND_LABEL_BGM_SUDDEN_DEATH);    //�T�h���f�X�̉��y���~
    	}
    	break;

        //�Q�[���I��
    case GAMESTATE_FINSIH_GAME:
        UpdateCamera();
        UpdateLight();
        UpdateBlock();
        UpdateBomb();
        UpdateFire();
        UpdateItem();
        UpdateEffect();
        UpdatePlayer();
        if (pSetting->nMaxPlayer >= 2)
        {
            UpdatePlayer2();
            if (pSetting->nMaxPlayer >= 3)
            {
                UpdatePlayer3();
                if (pSetting->nMaxPlayer >= 4)
                {
                    UpdatePlayer4();
                }
            }
        }
        UpdateUI();
        UpdateShadow();
        UpdateScore();
        //�X�g�[���[�Ȃ�
        if (pSetting->nMaxPlayer == 1)
        {
            UpdateEnemy();
            UpdateBullet();
            UpdateBoss();
        }

        g_nCunterGamestate++;
        if (g_nCunterGamestate >= COUNT_WAIT_FINISH_GAME)
        {
            //��l�p�ŕς�����ݒ��߂�
            pSetting->nTimeLimit = g_Game.nMemoryTimeLimit;
            pSetting->bRandomStartPos = g_Game.bMemoryRandomStartPos;
            pSetting->bSuddenDeath = g_Game.bMemorySuddenDeath;

            g_Game.state = GAMESTATE_NONE;
            SetFade(MODE_RESULT);                       //���U���g��
            StopSound(SOUND_LABEL_BGM_GAME);            //�Q�[���Ȃ��~
            StopSound(SOUND_LABEL_BGM_SUDDEN_DEATH);    //�T�h���f�X�̉��y���~
        }
        break;

        //�|�[�Y
    case GAMESTATE_PAUSE:
        UpdatePause();

        break;
    }

    //�}���`�v���C��
    if (pSetting->nMaxPlayer >= 2)
    {
        //���E���h�I������
        if (g_bFinishRound == false)
        {
            //�N�����������ꍇ
            if (g_Game.nCounter == 1)
            {
                PlaySound(SOUND_LABEL_SE_FINISH);    //�t�B�j�b�V���̌��ʉ�

                                                     //2�x������ʂ�Ȃ�����
                g_bFinishRound = true;

                //�e�v���C���[�̎擾
                PLAYER*pPlayer = GetPlayer();
                PLAYER2*pPlayer2 = GetPlayer2();
                PLAYER3*pPlayer3 = GetPlayer3();
                PLAYER4*pPlayer4 = GetPlayer4();

                //�N����������
                if (pPlayer->bDisp == true)
                {
                    g_Game.nCntPointPlayer1++;
                }
                else if (pPlayer2->bDisp == true)
                {
                    g_Game.nCntPointPlayer2++;
                }
                else if (pPlayer3->bDisp == true)
                {
                    g_Game.nCntPointPlayer3++;
                }
                else if (pPlayer4->bDisp == true)
                {
                    g_Game.nCntPointPlayer4++;
                }

                //���̃��E���h��
                SetGameState(GAMESTATE_FINSIH_ROUND);
            }

            //���������̏ꍇ
            else if (g_Game.nCounter <= 0)
            {
                SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 400.0f, 200.0f, UITYPE_012);  //��������

                PlaySound(SOUND_LABEL_SE_FINISH);    //�t�B�j�b�V���̌��ʉ�

                                                     //2�x������ʂ�Ȃ�����
                g_bFinishRound = true;

                //���̃��E���h��
                SetGameState(GAMESTATE_FINSIH_ROUND);
            }
        }

        //�Q�[���I������
        if (g_bFinishGame == false)
        {
            //�v���C���[1���������ꍇ
            if (g_Game.nCntPointPlayer1 == pSetting->nMaxRound)
            {
                //2�x������ʂ�Ȃ�����
                g_bFinishGame = true;

                //�v���C���[1��������
                g_Game.winner = SELECT_MOMO;

                //�Q�[���I��
                SetGameState(GAMESTATE_FINSIH_GAME);
            }

            //�v���C���[2���������ꍇ
            else if (g_Game.nCntPointPlayer2 == pSetting->nMaxRound)
            {
                //2�x������ʂ�Ȃ�����
                g_bFinishGame = true;

                //�v���C���[1��������
                g_Game.winner = SELECT_URA;

                //�Q�[���I��
                SetGameState(GAMESTATE_FINSIH_GAME);
            }

            //�v���C���[3���������ꍇ
            else if (g_Game.nCntPointPlayer3 == pSetting->nMaxRound)
            {
                //2�x������ʂ�Ȃ�����
                g_bFinishGame = true;

                //�v���C���[1��������
                g_Game.winner = SELECT_KIN;

                //�Q�[���I��
                SetGameState(GAMESTATE_FINSIH_GAME);
            }

            //�v���C���[4���������ꍇ
            else if (g_Game.nCntPointPlayer4 == pSetting->nMaxRound)
            {
                //2�x������ʂ�Ȃ�����
                g_bFinishGame = true;

                //�v���C���[1��������
                g_Game.winner = SELECT_ISUN;

                //�Q�[���I��
                SetGameState(GAMESTATE_FINSIH_GAME);
            }
        }
    }

}

//=============================================================================
// �Q�[���I������
// Author:�㓡�T�V��
//=============================================================================
void UninitGame(void)
{
    //�ݒ�̏��擾
    SETTING *pSetting = GetSetting();

    UninitPause();
    UninitCamera();
    UninitLight();
    UninitMeshField();
    UninitBlock();
    UninitBomb();
    UninitFire();
    UninitItem();
    UninitEffect();
    UninitPlayer();
    if (pSetting->nMaxPlayer >= 2)
    {
        UninitPlayer2();
        if (pSetting->nMaxPlayer >= 3)
        {
            UninitPlayer3();
            if (pSetting->nMaxPlayer >= 4)
            {
                UninitPlayer4();
            }
        }
    }
    UninitUI();
    UninitTime();
    UninitShadow();
    UninitScore();
    //�X�g�[���[�Ȃ�
    if (pSetting->nMaxPlayer == 1)
    {
        UninitEnemy();
        UninitBullet();
        UninitBoss();
    }
}

//=============================================================================
// �Q�[���`�揈��
// Author:�㓡�T�V��
//=============================================================================
void DrawGame(void)
{
    //�ݒ�̏��擾
    SETTING *pSetting = GetSetting();

    SetCamera();
    DrawMeshField();
    DrawBlock();
    DrawBomb();
    DrawFire();
    DrawItem();
    DrawPlayer();
    if (pSetting->nMaxPlayer >= 2)
    {
        DrawPlayer2();
        if (pSetting->nMaxPlayer >= 3)
        {
            DrawPlayer3();
            if (pSetting->nMaxPlayer >= 4)
            {
                DrawPlayer4();
            }
        }
    }
    DrawUI();
    DrawTime();
    DrawShadow();
    //�X�g�[���[�Ȃ�`��
    if (pSetting->nMaxPlayer == 1)
    {
        DrawScore();
        DrawEnemy();
        DrawBoss();
        DrawBullet();
    }
    DrawEffect();

    //�|�[�Y�̕`��
    if (g_Game.state == GAMESTATE_PAUSE)
    {
        DrawPause();
    }
}

//=============================================================================
// �Q�[����Ԃ̐ݒ�
// Author:�㓡�T�V��
//=============================================================================
void SetGameState(GAMESTATE state)
{
    g_Game.state = state;
    g_nCunterGamestate = 0;
}

//=============================================================================
// �Q�[���󋵂̎擾
// Author:�㓡�T�V��
//=============================================================================
GAME*GetGame(void)
{
    return &g_Game;
}

//=============================================================================
// ���ʂ̏��������e
// Author:�㓡�T�V��
//=============================================================================
void ResetGame(void)
{
    //�����̎�̏�����
    srand((unsigned int)time(0));

    //�ݒ�̏��擾
    SETTING *pSetting = GetSetting();

    //�X�g�[���[�Ȃ�A�����Őݒ���Œ艻����
    if (pSetting->nMaxPlayer == 1)
    {
        pSetting->nTimeLimit = 5;               //��������5��
        pSetting->bRandomStartPos = false;      //�J�n�ʒu�͌Œ�
        pSetting->bSuddenDeath = false;         //�T�h���f�X�Ȃ�
    }

    //�\���̂̏�����
    g_Game.nCounter = pSetting->nMaxPlayer;                                             //�v���C���[�̐l��
    g_Game.state = GAMESTATE_START;                                                     //�J�n��Ԃɂ���
    g_Game.winner = SELECT_MAX;                                                         //���҂�������

    //���ꂼ��̃v���C���[�̏����ʒu
    g_Game.startPos1 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);
    g_Game.startPos2 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);
    g_Game.startPos3 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);
    g_Game.startPos4 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

    //�X�g�[���[�Ȃ�A�v���C���[1�̏����ʒu��ς���
    if (pSetting->nMaxPlayer == 1)
    {
        g_Game.startPos1 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z / 2 + GRID_SIZE / 2);
    }

    //�J�n�ʒu�������_���Ȃ�
    if (pSetting->bRandomStartPos == true)
    {
        int nRandamPosPlayer1 = 0;    //�v���C���[1�̏����ʒu
        int nRandamPosPlayer2 = 0;    //�v���C���[2�̏����ʒu
        int nRandamPosPlayer3 = 0;    //�v���C���[3�̏����ʒu
        int nRandamPosPlayer4 = 0;    //�v���C���[4�̏����ʒu

        nRandamPosPlayer1 = rand() % 4 + 1;    //1�`4�𓾂�

        //�v���C���[�̏����ʒu�����߂�
        if (nRandamPosPlayer1 == 1)
        {
            g_Game.startPos1 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

            nRandamPosPlayer2 = rand() % 3 + 1;    //1�`3�𓾂�

            if (nRandamPosPlayer2 == 1)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1�`2�𓾂�

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 2)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1�`2�𓾂�

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 3)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1�`2�𓾂�

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);
                }
            }
        }
        else if (nRandamPosPlayer1 == 2)
        {
            g_Game.startPos1 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

            nRandamPosPlayer2 = rand() % 3 + 1;    //1�`3�𓾂�

            if (nRandamPosPlayer2 == 1)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1�`2�𓾂�

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 2)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1�`2�𓾂�

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 3)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1�`2�𓾂�

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);
                }
            }
        }
        else if (nRandamPosPlayer1 == 3)
        {
            g_Game.startPos1 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

            nRandamPosPlayer2 = rand() % 3 + 1;    //1�`3�𓾂�

            if (nRandamPosPlayer2 == 1)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1�`2�𓾂�

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 2)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1�`2�𓾂�

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 3)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1�`2�𓾂�

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);
                }
            }
        }
        else if (nRandamPosPlayer1 == 4)
        {
            g_Game.startPos1 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

            nRandamPosPlayer2 = rand() % 3 + 1;    //1�`3�𓾂�

            if (nRandamPosPlayer2 == 1)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1�`2�𓾂�

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 2)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1�`2�𓾂�

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 3)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1�`2�𓾂�

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);
                }
            }
        }
    }

    //�O���[�o���ϐ��̏�����
    g_nCunterGamestate = 0;
    g_nCntBrokenBlock = 0;
    g_nCntFireBlock = 0;
    g_nCntBombBlock = 0;
    g_nCntSpeedBlock = 0;
    g_nCntKickBlock = 0;
    g_bFinishRound = false;
    g_bFinishGame = false;

    //�v���C���[�̎g�p�󋵂̏�����
    InitDispPlayer();
    InitDispPlayer2();
    InitDispPlayer3();
    InitDispPlayer4();

    //�����Ŕj��ł���u���b�N�����Ƃ��p�̕ϐ�
    float posX = 0.0f;
    float posZ = 0.0f;
    D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    InitPause();
    InitShadow();
    InitCamera();
    InitLight();
    InitBlock();
    InitBomb();
    InitFire();
    InitStage();
    InputStageData();    //�X�e�[�W�̓ǂݍ��݂́A�u���b�N�̏��������I�������ɂ��
    InitMeshField();
    InitPlayer();
    if (pSetting->nMaxPlayer >= 2)
    {
        InitPlayer2();
        if (pSetting->nMaxPlayer >= 3)
        {
            InitPlayer3();
            if (pSetting->nMaxPlayer >= 4)
            {
                InitPlayer4();
            }
        }
    }
    InitItem();
    InitEffect();
    InitUI();
    InitTime();
    InitScore();
    //�X�g�[���[�Ȃ�
    if (pSetting->nMaxPlayer == 1)
    {
        InitEnemy();
        InitBullet();
        InitBoss();
    }

    //UI�̔z�u
    SetUI(D3DXVECTOR3(610.0f, 43.0f, 0.0f), TIME_SIZE_X, TIME_SIZE_Y, UITYPE_000);             //�������Ԃ̕�
    SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, 43.0f, 0.0f), TIME_SIZE_X, TIME_SIZE_Y, UITYPE_001);   //:
    if (pSetting->nMaxPlayer >= 2)
    {
        SetUI(D3DXVECTOR3(140.0f, 200.0f, 0.0f), 200.0f, 200.0f, UITYPE_002);                                   //�v���C���[1
        SetUI(D3DXVECTOR3(SCREEN_WIDTH - 140.0f, 200.0f, 0.0f), 200.0f, 200.0f, UITYPE_003);                    //�v���C���[2

        SetUI(D3DXVECTOR3(140.0f, 325.0f, 0.0f), 200.0f, 50.0f, UITYPE_006);                                    //�v���C���[1�̓��_
        SetUI(D3DXVECTOR3(SCREEN_WIDTH - 140.0f, 325.0f, 0.0f), 200.0f, 50.0f, UITYPE_007);                     //�v���C���[2�̓��_

        if (pSetting->nMaxPlayer >= 3)
        {
            SetUI(D3DXVECTOR3(140.0f, SCREEN_HEIGHT - 190.0f, 0.0f), 200.0f, 200.0f, UITYPE_004);                   //�v���C���[3

            SetUI(D3DXVECTOR3(140.0f, SCREEN_HEIGHT - 65.0f, 0.0f), 200.0f, 50.0f, UITYPE_008);                     //�v���C���[3�̓��_

            if (pSetting->nMaxPlayer >= 4)
            {
                SetUI(D3DXVECTOR3(SCREEN_WIDTH - 140.0f, SCREEN_HEIGHT - 190.0f, 0.0f), 200.0f, 200.0f, UITYPE_005);    //�v���C���[4

                SetUI(D3DXVECTOR3(SCREEN_WIDTH - 140.0f, SCREEN_HEIGHT - 65.0f, 0.0f), 200.0f, 50.0f, UITYPE_009);      //�v���C���[4�̓��_
            }
        }

        SetUI(D3DXVECTOR3(SCREEN_WIDTH - 140.0f, 40.0f, 0.0f), 240.0f, 60.0f, UITYPE_013);                  //n�Z�b�g���
    }
    SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 400.0f, 200.0f, UITYPE_010);              //��[���h��
    SetUI(D3DXVECTOR3(140.0f, 35.0f, 0.0f), 240.0f, 60.0f, UITYPE_014);                                     //�o�g�����C����/�X�g�[���[

    //�X�g�[���[��UI/�G
    if (pSetting->nMaxPlayer == 1)
    {
        SetUI(D3DXVECTOR3(SCREEN_WIDTH - 120.0f, 35.0f, 0.0f), 120.0f, 60.0f, UITYPE_015);                                   //�ӂ�/�S
        SetUI(D3DXVECTOR3(SCREEN_WIDTH / 4 + 115.0f, 44.0f, 0.0f), 250.0f, 50.0f, UITYPE_016);                               //HP�o�[�̔w�i
        SetUI(D3DXVECTOR3(SCREEN_WIDTH / 4 + 115.0f, 44.0f, 0.0f), 250.0f - BAR_EXTEND, 50.0f - BAR_EXTEND, UITYPE_017);     //HP�o�[
        SetUI(D3DXVECTOR3(SCREEN_WIDTH / 5 + 25.0f, 44.0f, 0.0f), 60.0f, 40.0f, UITYPE_018);                                 //HP�̕���
        SetUI(D3DXVECTOR3(SCREEN_WIDTH - 358.0f, 48.0f, 0.0f), 320.0f, 60.0f, UITYPE_019);                                   //�X�R�A�̔w�i

        SetEnemy(D3DXVECTOR3(1400.0f, 0.0f, 1000.0f), D3DXVECTOR3(ENEMY_GHOST_SPEED, 0.0f, 0.0f), ENEMYTYPE_GHOST);         //�S�[�X�g
        SetEnemy(D3DXVECTOR3(1400.0f, 0.0f, 200.0f), D3DXVECTOR3(ENEMY_GHOST_SPEED, 0.0f, 0.0f), ENEMYTYPE_GHOST);          //�S�[�X�g
        SetEnemy(D3DXVECTOR3(1400.0f, 0.0f, 600.0f), D3DXVECTOR3(ENEMY_UMBRELLA_SPEED, 0.0f, 0.0f), ENEMYTYPE_UMBRELLA);    //�A���u����
        SetEnemy(D3DXVECTOR3(2300.0f, 0.0f, 900.0f), D3DXVECTOR3(ENEMY_UMBRELLA_SPEED, 0.0f, 0.0f), ENEMYTYPE_UMBRELLA);    //�A���u����
        SetEnemy(D3DXVECTOR3(2300.0f, 0.0f, 300.0f), D3DXVECTOR3(ENEMY_UMBRELLA_SPEED, 0.0f, 0.0f), ENEMYTYPE_UMBRELLA);    //�A���u����
        SetEnemy(D3DXVECTOR3(3000.0f, 0.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_CANNON);                      //�L���m��
        SetEnemy(D3DXVECTOR3(3000.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_CANNON);                      //�L���m��
        SetEnemy(D3DXVECTOR3(3600.0f, 0.0f, 700.0f), D3DXVECTOR3(ENEMY_RED_GHOST_SPEED, 0.0f, 0.0f), ENEMYTYPE_RED_GHOST);  //���b�h�S�[�X�g
        SetEnemy(D3DXVECTOR3(3700.0f, 0.0f, 1100.0f), D3DXVECTOR3(ENEMY_GHOST_SPEED, 0.0f, 0.0f), ENEMYTYPE_GHOST);         //�S�[�X�g
        SetEnemy(D3DXVECTOR3(4000.0f, 0.0f, 300.0f), D3DXVECTOR3(ENEMY_RED_GHOST_SPEED, 0.0f, 0.0f), ENEMYTYPE_RED_GHOST);  //���b�h�S�[�X�g
        SetEnemy(D3DXVECTOR3(4300.0f, 0.0f, 100.0f), D3DXVECTOR3(ENEMY_UMBRELLA_SPEED, 0.0f, 0.0f), ENEMYTYPE_UMBRELLA);    //�A���u����
        SetEnemy(D3DXVECTOR3(4400.0f, 0.0f, 700.0f), D3DXVECTOR3(ENEMY_RED_GHOST_SPEED, 0.0f, 0.0f), ENEMYTYPE_RED_GHOST);  //���b�h�S�[�X�g

        //�A�C�e���̔z�u
        //�n�[�g�͓�Փx�m�[�}���Ȃ�o��
        if (pSetting->nLevel == SETTINGLEVEL_NORMAL)
        {
            SetItem(D3DXVECTOR3(3200.0f, GRID_SIZE / 2.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_004);
            SetItem(D3DXVECTOR3(4700.0f, GRID_SIZE / 2.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_004);
        }
        //�^�C�}�[
        SetItem(D3DXVECTOR3(3300.0f, GRID_SIZE / 2.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_005);
    }

    //�l�p�ƁA�t�߂̃u���b�N�ɂ��O���b�h�̎g�p���������(�󂹂�u���b�N��z�u�ł��Ȃ��悤�ɂ��邽��)
    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 1100.0f), true);  //����
    SetGridInfo(D3DXVECTOR3(200.0f, 0.0f, 1100.0f), true);
    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 1000.0f), true);

    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 1100.0f), true);  //�E��
    SetGridInfo(D3DXVECTOR3(1200.0f, 0.0f, 1100.0f), true);
    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 1000.0f), true);

    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 100.0f), true);  //����
    SetGridInfo(D3DXVECTOR3(200.0f, 0.0f, 100.0f), true);
    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 200.0f), true);

    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 100.0f), true);  //�E��
    SetGridInfo(D3DXVECTOR3(1200.0f, 0.0f, 100.0f), true);
    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 200.0f), true);

    //�}���`�v���C���́A�����_���ɁA�����̉󂹂�u���b�N��ݒu
    if (pSetting->nMaxPlayer >= 2)
    {
        while (g_nCntBrokenBlock != MAX_BROKEN_BLOCK
            - MAX_FIRE_BLOCK - MAX_BOMB_BLOCK - MAX_SPEED_BLOCK - MAX_KICK_BLOCK)//�ݒu������܂Ń��[�v(�ݒu����92 - �A�C�e�����łȂ��Ȃ烋�[�v)
        {
            //�����_����X�AZ���W�����߂�
            posX = float(rand() % 1200 + 100);
            posZ = float(rand() % 1000 + 100);
            pos = D3DXVECTOR3(posX, 0.0f, posZ);

            //�O���b�h���W�ɕϊ�����
            pos = ConversionGrid(&pos);

            //���̃O���b�h�ɔz�u�����Ȃ��Ȃ�z�u
            if (ConfirmationGrid(pos) == false)
            {
                //�j��ł���u���b�N��z�u
                SetBlock(pos, BLOCKTYPE_001);
                g_nCntBrokenBlock++;
            }
        }

        //�����_���ɁA���o���u���b�N��ݒu
        while (g_nCntFireBlock != MAX_FIRE_BLOCK)//�ݒu������܂Ń��[�v
        {
            //�����_����X�AZ���W�����߂�
            posX = float(rand() % 1200 + 100);
            posZ = float(rand() % 1000 + 100);
            pos = D3DXVECTOR3(posX, 0.0f, posZ);

            //�O���b�h���W�ɕϊ�����
            pos = ConversionGrid(&pos);

            //���̃O���b�h�ɔz�u�����Ȃ��Ȃ�z�u
            if (ConfirmationGrid(pos) == false)
            {
                //���o���u���b�N��z�u
                SetBlock(pos, BLOCKTYPE_002);
                g_nCntFireBlock++;
            }
        }

        //�����_���ɁA�{���o���u���b�N��ݒu
        while (g_nCntBombBlock != MAX_BOMB_BLOCK)//�ݒu������܂Ń��[�v
        {
            //�����_����X�AZ���W�����߂�
            posX = float(rand() % 1200 + 100);
            posZ = float(rand() % 1000 + 100);
            pos = D3DXVECTOR3(posX, 0.0f, posZ);

            //�O���b�h���W�ɕϊ�����
            pos = ConversionGrid(&pos);

            //���̃O���b�h�ɔz�u�����Ȃ��Ȃ�z�u
            if (ConfirmationGrid(pos) == false)
            {
                //�{���o���u���b�N��z�u
                SetBlock(pos, BLOCKTYPE_003);
                g_nCntBombBlock++;
            }
        }

        //�����_���ɁA�X�s�[�h�o���u���b�N��ݒu
        while (g_nCntSpeedBlock != MAX_SPEED_BLOCK)//�ݒu������܂Ń��[�v
        {
            //�����_����X�AZ���W�����߂�
            posX = float(rand() % 1200 + 100);
            posZ = float(rand() % 1000 + 100);
            pos = D3DXVECTOR3(posX, 0.0f, posZ);

            //�O���b�h���W�ɕϊ�����
            pos = ConversionGrid(&pos);

            //���̃O���b�h�ɔz�u�����Ȃ��Ȃ�z�u
            if (ConfirmationGrid(pos) == false)
            {
                //�X�s�[�h�o���u���b�N��z�u
                SetBlock(pos, BLOCKTYPE_004);
                g_nCntSpeedBlock++;
            }
        }

        //�����_���ɁA�L�b�N�o���u���b�N��ݒu
        while (g_nCntKickBlock != MAX_KICK_BLOCK)//�ݒu������܂Ń��[�v
        {
            //�����_����X�AZ���W�����߂�
            posX = float(rand() % 1200 + 100);
            posZ = float(rand() % 1000 + 100);
            pos = D3DXVECTOR3(posX, 0.0f, posZ);

            //�O���b�h���W�ɕϊ�����
            pos = ConversionGrid(&pos);

            //���̃O���b�h�ɔz�u�����Ȃ��Ȃ�z�u
            if (ConfirmationGrid(pos) == false)
            {
                //�L�b�N�o���u���b�N��z�u
                SetBlock(pos, BLOCKTYPE_005);
                g_nCntKickBlock++;
            }
        }
    }

    //�l�p�ƁA�t�߂̃u���b�N�̃O���b�h��������(�v���C���[���f�X�����Ƃ��A�A�C�e�����U��΂߂邽��)
    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 1100.0f), false);  //����
    SetGridInfo(D3DXVECTOR3(200.0f, 0.0f, 1100.0f), false);
    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 1000.0f), false);

    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 1100.0f), false);  //�E��
    SetGridInfo(D3DXVECTOR3(1200.0f, 0.0f, 1100.0f), false);
    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 1000.0f), false);

    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 100.0f), false);  //����
    SetGridInfo(D3DXVECTOR3(200.0f, 0.0f, 100.0f), false);
    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 200.0f), false);

    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 100.0f), false);  //�E��
    SetGridInfo(D3DXVECTOR3(1200.0f, 0.0f, 100.0f), false);
    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 200.0f), false);

    //�J�n�l���ɍ��킹���A�J�n���̊|����
    if (g_Game.nCounter == 2)
    {
        int nStartSound = 0;    //�����_���Ɋ|�����𗬂�

        nStartSound = rand() % 2 + 1;

        if (nStartSound == 1)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE);
        }
        else if (nStartSound == 2)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE2);
        }
    }
   
    else if (g_Game.nCounter == 3)
    {
        int nStartSound = 0;    //�����_���Ɋ|�����𗬂�

        nStartSound = rand() % 3 + 1;

        if (nStartSound == 1)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE);
        }
        else if (nStartSound == 2)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE2);
        }
        else if (nStartSound == 3)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE3);
        }
    }

    else if (g_Game.nCounter == 4)
    {
        int nStartSound = 0;    //�����_���Ɋ|�����𗬂�

        nStartSound = rand() % 4 + 1;

        if (nStartSound == 1)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE);
        }
        else if (nStartSound == 2)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE2);
        }
        else if (nStartSound == 3)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE3);
        }
        else if (nStartSound == 4)
        {
            PlaySound(SOUND_LABEL_SE_START_WOMAN_VOICE);
        }
    }
}