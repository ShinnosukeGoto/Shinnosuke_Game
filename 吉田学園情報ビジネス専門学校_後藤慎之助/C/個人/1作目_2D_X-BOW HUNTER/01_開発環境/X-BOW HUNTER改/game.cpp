//=============================================================================
//
// X-Bow Hunter�̏��� [game.cpp] (�Q�[�����)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "game.h"
#include "bg.h"			//�w�i�̊֘A�̏���
#include "bg2.h"		//�w�i�̊֘A�̏���
#include "hunter.h"		//�|���S���`��n�̏���
#include "arrow.h"		//�e�̔��ˏ���
#include "bullet.h"		//�e�̔��ˏ���
#include "explosion.h"	//�����̏���
#include "enemy.h"		//�G�̏���
#include "boss.h"		//�{�X�̏���
#include "effect.h"
#include "score.h"		//�X�R�A�̕\��
#include "item.h"
#include "storm.h"
#include "wing.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "life.h"
#include "main.h"
#include "title.h"
#include "result.h"
#include "time.h"
#include "bar.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
GAMESTATE g_gamestate;
HUCKERSTATE g_huckerstate;	//P�{�^���g�p�̗L��
int g_nCunterGamestate = 0;	//���U���g�܂ōs������

int g_nCntStateGame;		//ENTER�A�ŉ��p
							//=============================================================================
							//�Q�[������������ (��x�ɓǂݍ��ނƏd���Ȃ�̂�2�ɂ킯�Ă���)
							//=============================================================================
void InitGame1(void)
{
	InitItem();			//�A�C�e���̏�����
	InitWing();			//�E�B���O�̏�����
	InitArrow();		//��̏�����
	InitExplosion();	//�����̏�����
}

void InitGame2(void)
{
	InitBg();			//�w�i������
	InitBg2();			//�w�i������
	InitScore();		//�X�R�A�̏�����
	InitTime();			//�^�C���̏�����
	InitEnemy();		//�G�̏�����
	InitBoss();			//�{�X�̏�����
	InitBullet();		//�e�̏�����
	InitStorm();		//�X�g�[���̏�����
	InitEffect();		//�G�t�F�N�g�̏�����
	InitHunter();		//�v���C���[�̏�����

	SetEnemy(D3DXVECTOR3(1040, 240, 0.0f), D3DXVECTOR3(-0.5, 0.5, 0.0f), ENEMYTYPE_000);
	SetEnemy(D3DXVECTOR3(1140, 340, 0.0f), D3DXVECTOR3(-1.0, 0.5, 0.0f), ENEMYTYPE_000);

	SetHunter(D3DXVECTOR3(100, 590, 0.0f));

	InitLife();			//���C�t�\���̏�����(SetHunter�ŁAnStock�𒲐����Ă��珉�����j
	InitBar();			//���C�t�\���̏�����(SetHunter�ŁAnLife�𒲐����Ă��珉�����j

	PlaySound(SOUND_LABEL_BGM001);

	//�O���[�o���ϐ��̏�����
	g_gamestate = GAMESTATE_NORMAL;
	g_huckerstate = AREYOUHUCKER�H_NO;
	g_nCunterGamestate = 0;
	g_nCntStateGame = 0;
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	switch (g_gamestate)
	{
	case GAMESTATE_NORMAL:
		UpdateTime();		//�^�C���̍X�V

							//P�L�[�������ꂽ��A���U���g��ʂ�ω������邽�߂Ƀn�b�J�[�Ɣ��f
		if (GetKeyboardPress(DIK_P))
		{
			g_huckerstate = AREYOUHUCKER�H_YES;
		}

		//T�L�[�������ꂽ��A�^�C�g����ʂփ��Z�b�g
		if (g_nCntStateGame == 0)
		{
			if (GetKeyboardPress(DIK_T))
			{
				SetMode(MODE_TITLE);
				InitTitle();
				StopSound(SOUND_LABEL_BGM001);	//�Q�[����BGM���Ƃ߂�
				StopSound(SOUND_LABEL_BGM004);	//�{�X���BGM���~�߂�
				g_nCntStateGame = 6000;	//T�L�[�A�ŉ��
			}
		}

		break;
	case GAMESTATE_END:
		g_nCunterGamestate++;
		if (g_nCunterGamestate >= COUNT_WAIT_FINISH_GAME)
		{
			g_gamestate = GAMESTATE_NONE;
			InitResult();		//���U���g��ʂ̏�����
								//��ʃ��[�h�̐ݒ�
			SetFade(MODE_RESULT);
			StopSound(SOUND_LABEL_BGM001);//�Ȃ̒�~(����̕�)
			StopSound(SOUND_LABEL_BGM004);	//�{�X���BGM���~�߂�
		}
		break;
	}

	UpdateBg();			//�w�i�X�V
	UpdateBg2();		//�w�i�X�V
	UpdateScore();		//�X�R�A�̍X�V
	UpdateItem();		//�A�C�e���̍X�V
	UpdateEnemy();		//�G�̍X�V
	UpdateBoss();		//�{�X�̍X�V
	UpdateWing();		//�E�B���O�̍X�V
	UpdateHunter();		//�v���C���[�̍X�V
	UpdateLife();		//���C�t�\���̍X�V
	UpdateBar();		//HP�o�[�̍X�V
	UpdateArrow();		//��̍X�V
	UpdateBullet();		//�e�̍X�V
	UpdateStorm();		//�X�g�[���̍X�V
	UpdateEffect();		//�G�t�F�N�g�̍X�V
	UpdateExplosion();	//�����̍X�V

}
//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	UninitBg();			//�w�i
	UninitBg2();		//�w�i
	UninitScore();		//�X�R�A
	UninitTime();		//�^�C��
	UninitItem();		//�A�C�e��
	UninitEnemy();		//�G
	UninitBoss();		//�{�X
	UninitWing();		//�E�B���O
	UninitHunter();		//�v���C���[
	UninitLife();		//���C�t�\��
	UninitBar();		//HP
	UninitArrow();		//��
	UninitBullet();		//�e
	UninitStorm();		//�X�g�[��
	UninitEffect();		//�G�t�F�N�g
	UninitExplosion();	//����
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
{
	DrawBg();		//�w�i�̕`��
	DrawBg2();		//�w�i�̕`��
	DrawScore();	//�X�R�A�̕`��
	DrawTime();		//�^�C���̕`��
	DrawItem();		//�A�C�e���̕`��
	DrawEnemy();	//�G�̕`��
	DrawBoss();		//�{�X�̕`��
	DrawWing();		//�E�B���O�̕`��
	DrawHunter();	//�v���C���[�̕`��
	DrawLife();		//���C�t�\���̕`��
	DrawBar();		//HP�o�[�̕`��
	DrawArrow();	//��̕`��
	DrawBullet();	//�e�̕`��
	DrawStorm();	//�X�g�[���̕`��
	DrawEffect();	//�G�t�F�N�g�̕`��
	DrawExplosion();//�����̕`��
}

//=============================================================================
// �Q�[����Ԃ̐ݒ�
//=============================================================================
void SetGameState(GAMESTATE state)
{
	g_gamestate = state;
	g_nCunterGamestate = 0;
}

//=============================================================================
// �Q�[���̏�Ԃ̎擾
//=============================================================================
HUCKERSTATE GetGameState(void)
{
	return g_huckerstate;
}