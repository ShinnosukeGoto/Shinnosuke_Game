//=============================================================================
//
// �ŋ��̘b�����̏��� [game.h] (�Q�[�����)
// Author:�㓡�T�V��
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
// �\���̒�`
//=============================================================================
//�Q�[���̏��
typedef enum
{
    GAMESTATE_NONE,         //�Ȃ�(�������I�������A�����̏�Ԃɂ���)
    GAMESTATE_START,        //�J�n
    GAMESTATE_NORMAL,       //�ʏ�
    GAMESTATE_FINSIH_ROUND, //���E���h�I��
    GAMESTATE_FINSIH_GAME,  //�Q�[���I��
    GAMESTATE_PAUSE,        //�|�[�Y
    GAMESTATE_MAX           //�ő吔
}GAMESTATE;

//�Q�[���̊Ǘ����
typedef struct
{
    int nCounter;           //�Q���l��
    int nCntPointPlayer1;   //�v���C���[1�̓��_
    int nCntPointPlayer2;   //�v���C���[2�̓��_
    int nCntPointPlayer3;   //�v���C���[3�̓��_
    int nCntPointPlayer4;   //�v���C���[4�̓��_
    D3DXVECTOR3 startPos1;  //�J�n�ʒu1
    D3DXVECTOR3 startPos2;  //�J�n�ʒu2
    D3DXVECTOR3 startPos3;  //�J�n�ʒu3
    D3DXVECTOR3 startPos4;  //�J�n�ʒu4
    SELECT winner;          //����
    GAMESTATE state;        //�Q�[�����
    int nMemoryTimeLimit;         //�������Ԃ��L��
    bool bMemoryRandomStartPos;   //�����_���ȊJ�n�ʒu���ǂ������L��
    bool bMemorySuddenDeath;      //�T�h���f�X�����邩�ǂ������L��
}GAME;

//=============================================================================
//�v���g�^�C�v�錾
//=============================================================================
void InitGame(void);               //�ŏ��̃Q�[���̏�����
void InitNextGame(void);           //���̃��E���h�֍s���Ƃ��̏�����
void UpdateGame(void);
void UninitGame(void);
void DrawGame(void);

void SetGameState(GAMESTATE state);//�Q�[���̏��
GAME*GetGame(void);
void ResetGame(void);              //���ʂ̏��������e
#endif