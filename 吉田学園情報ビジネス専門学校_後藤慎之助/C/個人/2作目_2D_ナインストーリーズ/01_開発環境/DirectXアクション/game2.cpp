//=============================================================================
//
// �i�C���X�g�[���[�Y�̏��� [game2.cpp] (�Q�[�����2(������))
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "game2.h"
#include "bg.h"			//�w�i�̊֘A�̏���
#include "bg2.h"
#include "player.h"		//�|���S���`��n�̏���
#include "explosion.h"	//�����̏���
#include "score.h"		//�X�R�A�̕\��
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "mpbar.h"
#include "main.h"
#include "title.h"
#include "result.h"
#include "time.h"
#include "bar.h"
#include "effect.h"
#include "attack.h"
#include "magic.h"
#include "enemy.h"
#include "block.h"
#include "floor.h"
#include "bullet.h"
#include "item.h"
#include "effect2.h"
#include "game3.h"
#include "ui.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
GAMESTATE2 g_gamestate2;
int g_nCunterGamestate2 = 0;	//���̉�ʂ֍s���܂ł̎���

int g_nCntStateGame2;		//T�A�ŉ��p
GAMELEVEL g_nGameLevel2;	//�Q�[���̓�Փx

//=============================================================================
//�Q�[��2���������� 
//=============================================================================
void InitGame2(void)
{
	InitBlock();		//�u���b�N�̏�����
	InitFloor();		//���̏�����
	//InitEffect2();		//�G�t�F�N�g2�̏�����

	InitItem();			//�A�C�e���̏�����
	//InitBullet();		//�G�̒e�̏�����
	//InitAttack();		//�U���̏�����
	//InitExplosion();	//�����̏�����

	//InitBg();			//�w�i������
	//InitBg2();			//�w�i������
	//InitScore();		//�X�R�A�̏�����
	//InitTime();			//�^�C���̏�����
	InitEnemy();		//�G�̏�����
	//InitMagic();		//���@�̏�����
	//InitEffect();		//�G�t�F�N�g�̏�����
	//InitPlayer();		//�v���C���[�̏�����

	//�Q�[���̓�Փx���擾
	g_nGameLevel2 = GetTitle();

	//�G�̐ݒu
	SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 300, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����
	SetEnemy(D3DXVECTOR3(0, 250, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//�R�E����

	//SetBlock(D3DXVECTOR3(500, 370, 0.0f), 120, 120, BLOCKTYPE_000,30);		//����(�f�o�b�N�p)
	//SetBlock(D3DXVECTOR3(500, 570, 0.0f), 120, 120, BLOCKTYPE_007, 30);		//���̒��ւ̃��[�v�]�[��(�f�o�b�N�p)
	
	//�O���[�o���ϐ��̏�����
	g_gamestate2 = GAMESTATE2_NORMAL;
	g_nCunterGamestate2 = 0;
	g_nCntStateGame2 = 0;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame2(void)
{
	switch (g_gamestate2)
	{
	case GAMESTATE2_NORMAL:
		UpdateTime();		//�^�C���̍X�V

		//T�L�[�������ꂽ��A�^�C�g����ʂփ��Z�b�g
		if (g_nCntStateGame2 == 0)
		{
			if (GetKeyboardPress(DIK_T))
			{
				SetMode(MODE_TITLE);
				InitTitle();
				StopSound(SOUND_LABEL_BGM001);	//�X�e�[�W1,2��BGM���Ƃ߂�
				g_nCntStateGame2 = 6000;	//T�L�[�A�ŉ��
			}
		}
		break;

	//���̃X�e�[�W��
	case GAMESTATE2_NEXTSTAGE:
		g_nCunterGamestate2++;
		if (g_nCunterGamestate2 >= COUNT_WAIT_NEXTSTAGE_GAME)
		{
			g_gamestate2 = GAMESTATE2_NONE;
			InitGame3();		//�Q�[��3�̏�����
			//��ʃ��[�h�̐ݒ�
			SetFade(MODE_GAME3);		//�X�e�[�W3��
			StopSound(SOUND_LABEL_BGM001);	//�X�e�[�W1,2��BGM���Ƃ߂�
		}
		break;

	//�Q�[���I�[�o�[
	case GAMESTATE2_GAMEOVER:
		g_nCunterGamestate2++;
		if (g_nCunterGamestate2 >= COUNT_WAIT_FINISH_GAME)
		{
			g_gamestate2 = GAMESTATE2_NONE;
			InitResult();		//���U���g��ʂ̏�����
			//��ʃ��[�h�̐ݒ�
			SetFade(MODE_RESULT);
			StopSound(SOUND_LABEL_BGM001);	//�X�e�[�W1,2��BGM���Ƃ߂�
		}
		break;
	}

	UpdateBg();			//�w�i�X�V
	UpdateBg2();		//�w�i�X�V
	UpdateFloor();		//���X�V
	UpdateBlock();		//�u���b�N�̍X�V
	UpdateItem();		//�A�C�e���̍X�V
	UpdateEnemy();		//�G�̍X�V
	UpdatePlayer();		//�v���C���[�̍X�V
	UpdateMpbar();		//���C�t�\���̍X�V
	UpdateBar();		//HP�o�[�̍X�V
	UpdateBullet();		//�G�̒e�̍X�V
	UpdateAttack();		//�U���̍X�V
	UpdateMagic();		//���@�̍X�V
	UpdateEffect();		//�G�t�F�N�g�̍X�V
	UpdateEffect2();	//�G�t�F�N�g2�̍X�V
	UpdateExplosion();	//�����̍X�V
	UpdateUI();			//UI�̍X�V
	UpdateScore();		//�X�R�A�̍X�V
}
//=============================================================================
// �I������
//=============================================================================
void UninitGame2(void)
{
	UninitBg();			//�w�i
	UninitBg2();		//�w�i
	UninitFloor();		//��
	UninitBlock();		//�u���b�N
	UninitItem();		//�A�C�e��
	UninitEnemy();		//�G
	UninitPlayer();		//�v���C���[
	UninitMpbar();		//���C�t�\��
	UninitBar();		//HP
	UninitBullet();		//�G�̒e
	UninitAttack();		//�U��
	UninitMagic();		//���@
	UninitEffect();		//�G�t�F�N�g
	UninitEffect2();	//�G�t�F�N�g2
	UninitExplosion();	//����
	UninitUI();			//UI
	UninitScore();		//�X�R�A
	UninitTime();		//�^�C��
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame2(void)
{
	DrawBg();		//�w�i�̕`��
	DrawBg2();		//�w�i�̕`��
	DrawFloor();	//���̕`��
	DrawBlock();	//�u���b�N�̕`��
	DrawItem();		//�A�C�e���̕`��
	DrawEnemy();	//�G�̕`��
	DrawPlayer();	//�v���C���[�̕`��
	DrawMpbar();		//���C�t�\���̕`��
	DrawBar();		//HP�o�[�̕`��
	DrawBullet();	//�G�̒e�̕`��
	DrawAttack();	//�U���̕`��
	DrawMagic();	//���@�̕`��
	DrawEffect();	//�G�t�F�N�g�̕`��
	DrawEffect2();	//�G�t�F�N�g2�̕`��
	DrawExplosion();//�����̕`��
	DrawUI();		//UI�̕`��
	DrawScore();	//�X�R�A�̕`��
	DrawTime();		//�^�C���̕`��
}

//=============================================================================
// �Q�[����Ԃ̐ݒ�
//=============================================================================
void SetGameState2(GAMESTATE2 state)
{
	g_gamestate2 = state;
	g_nCunterGamestate2 = 0;
}
