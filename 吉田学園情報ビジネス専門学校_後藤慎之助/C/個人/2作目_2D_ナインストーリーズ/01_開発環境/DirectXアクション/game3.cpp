//=============================================================================
//
// �i�C���X�g�[���[�Y�̏��� [game3.cpp] (�Q�[�����2(���̒�)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
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
GAMESTATE3 g_gamestate3;
int g_nCunterGamestate3 = 0;	//���̉�ʂ֍s���܂ł̎���

int g_nCntStateGame3;		//T�A�ŉ��p
GAMELEVEL g_nGameLevel3;	//�Q�[���̓�Փx

//=============================================================================
//�Q�[��3���������� 
//=============================================================================
void InitGame3(void)
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
	g_nGameLevel3 = GetTitle();

	//�{�X�̐ݒu
	SetEnemy(D3DXVECTOR3(1050, 425, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_006);		//�A�C�X�h���S��

	//�������P�̐ݒu
	SetItem(D3DXVECTOR3(1100, 200, 0.0f), ITEMTYPE_004, PLAYER_SIZE*2);

	//�{�X���BGM
	PlaySound(SOUND_LABEL_BGM004);	//�{�X��̋�

	//�O���[�o���ϐ��̏�����
	g_gamestate3 = GAMESTATE3_NORMAL;
	g_nCunterGamestate3 = 0;
	g_nCntStateGame3 = 0;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame3(void)
{
	switch (g_gamestate3)
	{
	case GAMESTATE3_NORMAL:
		UpdateTime();		//�^�C���̍X�V

		//T�L�[�������ꂽ��A�^�C�g����ʂփ��Z�b�g
		if (g_nCntStateGame3 == 0)
		{
			if (GetKeyboardPress(DIK_T))
			{
				SetMode(MODE_TITLE);
				InitTitle();
				StopSound(SOUND_LABEL_BGM004);	//�{�X���BGM���~�߂�
				g_nCntStateGame3 = 6000;	//T�L�[�A�ŉ��
			}
		}
		break;

		//�Q�[���N���A
	case GAMESTATE3_GAMECLEAR:
		g_nCunterGamestate3++;
		if (g_nCunterGamestate3 >= COUNT_WAIT_NEXTSTAGE_GAME)
		{
			g_gamestate3 = GAMESTATE3_NONE;
			InitResult();		//���U���g��ʂ̏�����
			//��ʃ��[�h�̐ݒ�
			SetFade(MODE_RESULT);		//�N���A��ʂ�
			//StopSound(SOUND_LABEL_BGM004);	//�{�X���BGM���~�߂�(�G�l�~�[�̕��Ŏ~�߂Ă���̂ŃR�����g�A�E�g)
		}
		break;

		//�Q�[���I�[�o�[
	case GAMESTATE3_GAMEOVER:
		g_nCunterGamestate3++;
		if (g_nCunterGamestate3 >= COUNT_WAIT_FINISH_GAME)
		{
			g_gamestate3 = GAMESTATE3_NONE;
			InitResult();		//���U���g��ʂ̏�����
			//��ʃ��[�h�̐ݒ�
			SetFade(MODE_RESULT);
			StopSound(SOUND_LABEL_BGM004);	//�{�X���BGM���~�߂�
		}
		break;
	}

	UpdateBg();			//�w�i�X�V
	//UpdateBg2();		//�w�i�X�V
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
void UninitGame3(void)
{
	UninitBg();			//�w�i
	//UninitBg2();		//�w�i
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
void DrawGame3(void)
{
	DrawBg();		//�w�i�̕`��
	//DrawBg2();		//�w�i�̕`��
	DrawFloor();	//���̕`��
	DrawBlock();	//�u���b�N�̕`��
	DrawItem();		//�A�C�e���̕`��
	DrawEnemy();	//�G�̕`��
	DrawPlayer();	//�v���C���[�̕`��
	DrawMpbar();	//���C�t�\���̕`��
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
void SetGameState3(GAMESTATE3 state)
{
	g_gamestate3 = state;
	g_nCunterGamestate3 = 0;
}
