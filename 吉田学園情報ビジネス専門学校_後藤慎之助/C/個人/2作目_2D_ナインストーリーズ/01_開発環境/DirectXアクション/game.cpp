//=============================================================================
//
// �i�C���X�g�[���[�Y�̏��� [game.cpp] (�Q�[�����1(��̃X�e�[�W))
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "game.h"
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
#include "game2.h"
#include "ui.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
GAMESTATE g_gamestate;
int g_nCunterGamestate = 0;	//���̉�ʂ֍s���܂ł̎���

int g_nCntStateGame;		//T�A�ŉ��p
GAMELEVEL g_nGameLevel;		//�Q�[���̓�Փx

//=============================================================================
//�Q�[������������ (��x�ɓǂݍ��ނƏd���Ȃ�̂�3�ɂ킯�Ă���)
//=============================================================================
void InitGameFirst(void)
{
	InitBlock();		//�u���b�N�̏�����
	InitFloor();		//���̏�����
	InitEffect2();		//�G�t�F�N�g2�̏�����
}

void InitGameSecond(void)
{
	InitItem();			//�A�C�e���̏�����
	InitBullet();		//�G�̒e�̏�����
	InitAttack();		//�U���̏�����
	InitExplosion();	//�����̏�����
	InitUI();			//UI�̏�����
}

void InitGameThird(void)
{
	InitBg();			//�w�i������
	InitBg2();			//�w�i������
	InitScore();		//�X�R�A�̏�����
	InitTime();			//�^�C���̏�����
	InitEnemy();		//�G�̏�����
	InitMagic();		//���@�̏�����
	InitEffect();		//�G�t�F�N�g�̏�����
	InitPlayer();		//�v���C���[�̏�����

	//�Q�[���̓�Փx���擾
	g_nGameLevel = GetTitle();

	//�G�̐ݒu
	SetEnemy(D3DXVECTOR3(1040, 600, 0.0f), D3DXVECTOR3(-0.5, 0.0, 0.0f), ENEMYTYPE_000);	//�r�[�g��
	SetEnemy(D3DXVECTOR3(1550, 550, 0.0f), D3DXVECTOR3(-0.5, 0.0, 0.0f), ENEMYTYPE_000);	//�r�[�g��
	SetEnemy(D3DXVECTOR3(2215, 350, 0.0f), D3DXVECTOR3(-1.0, 0.0, 0.0f), ENEMYTYPE_001);	//�J���X
	SetEnemy(D3DXVECTOR3(5015, 250, 0.0f), D3DXVECTOR3(-2.5, 0.0, 0.0f), ENEMYTYPE_002);	//�J���X����
	SetEnemy(D3DXVECTOR3(15000, 200, 0.0f), D3DXVECTOR3(-2.5, 0.0, 0.0f), ENEMYTYPE_002);	//�J���X����
	SetEnemy(D3DXVECTOR3(3015, 590, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);		//�K�C�R�c
	SetEnemy(D3DXVECTOR3(3750, 400, 0.0f), D3DXVECTOR3(-0.7, 0.0, 0.0f), ENEMYTYPE_000);	//�r�[�g��
	SetEnemy(D3DXVECTOR3(4350, 600, 0.0f), D3DXVECTOR3(-0.5, 0.0, 0.0f), ENEMYTYPE_000);	//�r�[�g��
	SetEnemy(D3DXVECTOR3(4550, 350, 0.0f), D3DXVECTOR3(-1.0, 0.0, 0.0f), ENEMYTYPE_001);	//�J���X
	SetEnemy(D3DXVECTOR3(5270, 100, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_007);		//�s���j�A
	SetEnemy(D3DXVECTOR3(5550, 100, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_007);		//�s���j�A
	SetEnemy(D3DXVECTOR3(5830, 100, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_007);		//�s���j�A
	SetEnemy(D3DXVECTOR3(6550, 600, 0.0f), D3DXVECTOR3(-0.5, 0.0, 0.0f), ENEMYTYPE_000);	//�r�[�g��
	SetEnemy(D3DXVECTOR3(8050, 250, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_008);		//�_�[�N�K�C�R�c
	SetEnemy(D3DXVECTOR3(9150, 400, 0.0f), D3DXVECTOR3(-0.5, 0.0, 0.0f), ENEMYTYPE_000);	//�r�[�g��
	SetEnemy(D3DXVECTOR3(9400, 200, 0.0f), D3DXVECTOR3(-0.5, 0.0, 0.0f), ENEMYTYPE_000);	//�r�[�g��
	SetEnemy(D3DXVECTOR3(10500, 570, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_004);	//�C�m�V�V

	//�Q�[�����x���n�[�h�̏ꍇ�A�G��������
	if (g_nGameLevel == GAMELEVEL_HARD)
	{
		SetEnemy(D3DXVECTOR3(3750, 590, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);		//�K�C�R�c
		SetEnemy(D3DXVECTOR3(4450, 175, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);		//�K�C�R�c
	}

	//�u���b�N�̐ݒu
	//SetBlock(D3DXVECTOR3(500, 570, 0.0f), 120, 120, BLOCKTYPE_004, 30);		//������(�f�o�b�N�p)
	SetBlock(D3DXVECTOR3(450, 580, 0.0f), 80, 80, BLOCKTYPE_002,30);		//�₶�邵�Ŕ�
	SetBlock(D3DXVECTOR3(1500, 600, 0.0f), 300, 50, BLOCKTYPE_000, 30);		//�ʂ�Ȃ�
	SetBlock(D3DXVECTOR3(2040, 590, 0.0f), 150, 70,BLOCKTYPE_000, 30);		//�ʂ�Ȃ�
	SetBlock(D3DXVECTOR3(2215, 525, 0.0f), 200, 200, BLOCKTYPE_000, 30);	//�ʂ�Ȃ�
	SetBlock(D3DXVECTOR3(2800, 525, 0.0f), 250, 200, BLOCKTYPE_003, 30);	//��
	SetBlock(D3DXVECTOR3(2800, 425, 0.0f), 250, 10, BLOCKTYPE_001, 30);		//�Ƃ̉���
	SetBlock(D3DXVECTOR3(3700, 525, 0.0f), 300, 200, BLOCKTYPE_003, 30);	//��
	SetBlock(D3DXVECTOR3(3700, 425, 0.0f), 300, 10, BLOCKTYPE_001, 30);		//�Ƃ̉���
	SetBlock(D3DXVECTOR3(4350, 425, 0.0f), 300, 400, BLOCKTYPE_012, 30);	//�c�ɒ�����
	SetBlock(D3DXVECTOR3(4350, 225, 0.0f), 300, 10, BLOCKTYPE_001, 30);		//�Ƃ̉���
	SetBlock(D3DXVECTOR3(4550, 525, 0.0f), 300, 200, BLOCKTYPE_003, 30);	//��
	SetBlock(D3DXVECTOR3(4550, 425, 0.0f), 300, 10, BLOCKTYPE_001, 30);		//�Ƃ̉���
	SetBlock(D3DXVECTOR3(3600, 590, 0.0f), 70, 70, BLOCKTYPE_000, 30);		//�ʂ�Ȃ�
	SetBlock(D3DXVECTOR3(5550, 500, 0.0f), 900, 250, BLOCKTYPE_011, 30);	//��
	SetBlock(D3DXVECTOR3(5550, 320, 0.0f), 1000, 10, BLOCKTYPE_001, 30);	//��
	SetBlock(D3DXVECTOR3(5030, 575, 0.0f), 200, 100, BLOCKTYPE_000, 30);	//�ʂ�Ȃ�
	SetBlock(D3DXVECTOR3(5100, 425, 0.0f), 100, 200, BLOCKTYPE_000, 30);	//�ʂ�Ȃ�(�[)
	SetBlock(D3DXVECTOR3(6000, 425, 0.0f), 100, 200, BLOCKTYPE_000, 30);	//�ʂ�Ȃ�(�[)
	SetBlock(D3DXVECTOR3(6070, 575, 0.0f), 200, 100, BLOCKTYPE_000, 30);	//�ʂ�Ȃ�
	SetBlock(D3DXVECTOR3(7100, 545, 0.0f), 350, 180, BLOCKTYPE_014, 30);	//�A�X���`�b�N�̒�
	SetBlock(D3DXVECTOR3(7100, 455, 0.0f), 350, 20, BLOCKTYPE_001, 30);		//�A�X���`�b�N��
	SetBlock(D3DXVECTOR3(7000, 415, 0.0f), 80, 80, BLOCKTYPE_013, 30);		//S�Ŕ�
	SetBlock(D3DXVECTOR3(7600, 465, 0.0f), 150, 335, BLOCKTYPE_014, 30);	//�A�X���`�b�N�̒�
	SetBlock(D3DXVECTOR3(7600, 305, 0.0f), 150, 20, BLOCKTYPE_001, 30);		//�A�X���`�b�N��
	SetBlock(D3DXVECTOR3(7790, 535, 0.0f), 150, 185, BLOCKTYPE_014, 30);	//�A�X���`�b�N�̒�
	SetBlock(D3DXVECTOR3(7790, 445, 0.0f), 150, 20, BLOCKTYPE_001, 30);		//�A�X���`�b�N��
	SetBlock(D3DXVECTOR3(7350, 590, 0.0f), 80, 80, BLOCKTYPE_015, 30);		//�g�Q�u���b�N
	SetBlock(D3DXVECTOR3(7430, 590, 0.0f), 80, 80, BLOCKTYPE_015, 30);		//�g�Q�u���b�N
	SetBlock(D3DXVECTOR3(7750, 590, 0.0f), 80, 80, BLOCKTYPE_015, 30);		//�g�Q�u���b�N
	SetBlock(D3DXVECTOR3(7830, 590, 0.0f), 80, 80, BLOCKTYPE_015, 30);		//�g�Q�u���b�N
	SetBlock(D3DXVECTOR3(8050, 465, 0.0f), 200, 335, BLOCKTYPE_014, 30);	//�A�X���`�b�N�̒�
	SetBlock(D3DXVECTOR3(8050, 305, 0.0f), 200, 20, BLOCKTYPE_001, 30);		//�A�X���`�b�N��
	SetBlock(D3DXVECTOR3(8480, 415, 0.0f), 125, 435, BLOCKTYPE_014, 30);	//�A�X���`�b�N�̒�
	SetBlock(D3DXVECTOR3(8480, 208, 0.0f), 125, 30, BLOCKTYPE_001, 30);		//�A�X���`�b�N��
	SetBlock(D3DXVECTOR3(8300, 593, 0.0f), 530, 90, BLOCKTYPE_014, 30);		//�A�X���`�b�N�̒�
	SetBlock(D3DXVECTOR3(8300, 538, 0.0f), 530, 20, BLOCKTYPE_001, 30);		//�A�X���`�b�N��
	SetBlock(D3DXVECTOR3(8480, 490, 0.0f), 80, 80, BLOCKTYPE_015, 30);		//�g�Q�u���b�N
	SetBlock(D3DXVECTOR3(8900, 580, 0.0f), 100, 100, BLOCKTYPE_009, 30);	//�󂹂�u���b�N
	SetBlock(D3DXVECTOR3(9100, 475, 0.0f), 400, 100, BLOCKTYPE_000, 30);	//�ʂ�Ȃ�
	SetBlock(D3DXVECTOR3(9400, 425, 0.0f), 200, 400, BLOCKTYPE_000, 30);	//�z����Ȃ�&�ʂ�Ȃ�
	SetBlock(D3DXVECTOR3(11000, 300, 0.0f), 500, 650, BLOCKTYPE_010, 30);	//�^���[
	SetBlock(D3DXVECTOR3(11000, 570, 0.0f), 110, 110, BLOCKTYPE_004, 30);	//������
	SetBlock(D3DXVECTOR3(10885, 570, 0.0f), 120, 120, BLOCKTYPE_008, 30);	//W�Ŕ�

	//�A�C�e���̐ݒu
	SetItem(D3DXVECTOR3(200, 585, 0.0f), ITEMTYPE_000, 70);		//��
	SetItem(D3DXVECTOR3(3250, 585, 0.0f), ITEMTYPE_000, 70);	//��
	SetItem(D3DXVECTOR3(3800, 385, 0.0f), ITEMTYPE_000, 70);	//��
	SetItem(D3DXVECTOR3(4250, 185, 0.0f), ITEMTYPE_000, 70);	//��
	SetItem(D3DXVECTOR3(4500, 590, 0.0f), ITEMTYPE_002, 70);	//�M
	SetItem(D3DXVECTOR3(7600, 585, 0.0f), ITEMTYPE_000, 70);	//��
	SetItem(D3DXVECTOR3(8300, 590, 0.0f), ITEMTYPE_000, 70);	//��
	SetItem(D3DXVECTOR3(8480, 160, 0.0f), ITEMTYPE_000, 70);	//��
	SetItem(D3DXVECTOR3(9180, 585, 0.0f), ITEMTYPE_000, 70);	//��
	SetItem(D3DXVECTOR3(9250, 585, 0.0f), ITEMTYPE_000, 70);	//��
	SetItem(D3DXVECTOR3(10800, 590, 0.0f), ITEMTYPE_002, 70);	//�M
	SetItem(D3DXVECTOR3(11200, 590, 0.0f), ITEMTYPE_002, 70);	//�M

	//���@�̐ݒu
	SetPlayer(D3DXVECTOR3(SCREEN_WIDTH/4, 590, 0.0f));

	InitMpbar();			//���C�t�\���̏�����(SetPlayer�ŁAnStock�𒲐����Ă��珉�����j
	InitBar();			//���C�t�\���̏�����(SetPlayer�ŁAnMpbar�𒲐����Ă��珉�����j

	//�X�e�[�W1,2��BGM
	PlaySound(SOUND_LABEL_BGM001);

	//�O���[�o���ϐ��̏�����
	g_gamestate = GAMESTATE_NORMAL;
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

		//T�L�[�������ꂽ��A�^�C�g����ʂփ��Z�b�g
		if (g_nCntStateGame == 0)
		{
			if (GetKeyboardPress(DIK_T))
			{
				SetMode(MODE_TITLE);
				InitTitle();
				StopSound(SOUND_LABEL_BGM001);	//�Q�[����BGM���Ƃ߂�
				g_nCntStateGame = 6000;	//T�L�[�A�ŉ��
			}
		}

		break;

	//���̃X�e�[�W��
	case GAMESTATE_NEXTSTAGE:
		g_nCunterGamestate++;
		if (g_nCunterGamestate >= COUNT_WAIT_NEXTSTAGE_GAME)
		{
			g_gamestate = GAMESTATE_NONE;
			InitGame2();		//�Q�[��2�̏�����
			//��ʃ��[�h�̐ݒ�
			SetFade(MODE_GAME2);		//�X�e�[�W2��
		}
		break;

	//�Q�[���I�[�o�[
	case GAMESTATE_GAMEOVER:
		g_nCunterGamestate++;
		if (g_nCunterGamestate >= COUNT_WAIT_FINISH_GAME)
		{
			g_gamestate = GAMESTATE_NONE;
			InitResult();		//���U���g��ʂ̏�����
			//��ʃ��[�h�̐ݒ�
			SetFade(MODE_RESULT);
			StopSound(SOUND_LABEL_BGM001);//�Ȃ̒�~(����̕�)
		}
		break;
	}

	UpdateBg();			//�w�i�X�V
	UpdateFloor();		//���X�V
	UpdateBg2();		//�w�i�X�V
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
void UninitGame(void)
{
	UninitBg();			//�w�i
	UninitFloor();		//��
	UninitBg2();		//�w�i
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
void DrawGame(void)
{
	DrawBg();		//�w�i�̕`��
	DrawFloor();	//���̕`��
	DrawBg2();		//�w�i�̕`��
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
void SetGameState(GAMESTATE state)
{
	g_gamestate = state;
	g_nCunterGamestate = 0;
}
