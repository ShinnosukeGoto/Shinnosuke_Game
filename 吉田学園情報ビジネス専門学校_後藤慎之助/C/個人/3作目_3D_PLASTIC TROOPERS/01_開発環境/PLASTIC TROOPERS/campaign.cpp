//=============================================================================
//
// PLASTIC TROOPERS [campaign.cpp]�̏��� (�{�X��|���ɍs�����[�h)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "campaign.h"
#include "fade.h"
#include "input.h"
#include "main.h"
#include "title.h"
#include "time.h"
#include "meshfield.h"
#include "ui.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "shadow.h"
#include "block.h"
#include "ui.h"
#include "enemy.h"
#include "wall.h"
#include "item.h"
#include "bullet.h"
#include "score.h"
#include "effect.h"
#include "bg.h"
#include "effect2.h"
#include "pause.h"
#include "controller.h"
#include "model.h"
#include "enemylife.h"
#include "sound.h"
#include "boss.h"
#include "time.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
CAMPAIGNSTATE g_campaignstate;
int g_nCntCampaign = 0;	//���̉�ʂ֍s���܂ł̎���

							//GAMELEVEL g_nGameLevel;		//�Q�[���̓�Փx

//=============================================================================
//�Q�[������������
//=============================================================================
void InitCampaign(void)
{
    InitPause();
	InitCamera();					//�J�����̏�����
	InitLight();					//���C�g�̏�����
	InitMeshField();
    InitWall();
	InitPlayer();					//���@�̏�����
    InitEnemy();
	InitShadow();
	InitBlock();
    InitUI();
    InitItem();
    InitBullet();
    InitScore();
    InitEffect();
    InitEffect2();
    //InitEnemyLife();
    InitBg();
    InitModel();
    InitBoss();
    InitTime();

    //UI�̔z�u
    SetUI(D3DXVECTOR3(1085.0f, 50.0f, 0.0f), 300.0f, 70.0f, UITYPE_000);   //�X�R�A�̔w�i
    SetUI(D3DXVECTOR3(165.0f, 50.0f, 0.0f), 300.0f, 70.0f, UITYPE_001);    //�L�����y�[��
    SetUI(D3DXVECTOR3(120.0f, 600.0f, 0.0f), 150.0f, 150.0f, UITYPE_005);  //�K�E�Q�[�W�̔w�i
    SetUI(D3DXVECTOR3(320.0f, 600.0f, 0.0f), 250.0f, 50.0f, UITYPE_006);   //HP�Q�[�W�̔w�i
    SetUI(D3DXVECTOR3(SCREEN_WIDTH/2, 650.0f, 0.0f), 350.0f, 50.0f, UITYPE_006);   //ENERGY�Q�[�W�̔w�i
    SetUI(D3DXVECTOR3(220.0f, 560.0f, 0.0f), 60.0f, 40.0f, UITYPE_008);   //HP
    SetUI(D3DXVECTOR3(535.0f, 610.0f, 0.0f), 120.0f, 40.0f, UITYPE_009);  //ENERGY
    SetUI(D3DXVECTOR3(320.0f, 600.0f, 0.0f), 250.0f - BAR_EXTEND, 50.0f - BAR_EXTEND, UITYPE_010);   //HP�Q�[�W
    SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, 650.0f, 0.0f), 350.0f - BAR_EXTEND, 50.0f - BAR_EXTEND, UITYPE_011);   //ENERGY�Q�[�W
    SetUI(D3DXVECTOR3(120.0f, 600.0f, 0.0f), 150.0f - BAR_EXTEND, 150.0f - BAR_EXTEND, UITYPE_012);  //�K�E�Q�[�W
    SetUI(D3DXVECTOR3(120.0f, 600.0f, 0.0f), 125.0f, 125.0f, UITYPE_013);  //�K�E�̐���
    SetUI(D3DXVECTOR3(955.0f, 620.0f, 0.0f), 110.0f, 110.0f, UITYPE_014);  //��
    SetUI(D3DXVECTOR3(1145.0f, 520.0f, 0.0f), 110.0f, 110.0f, UITYPE_015);  //�e
    SetUI(D3DXVECTOR3(1050.0f, 570.0f, 0.0f), 110.0f, 110.0f, UITYPE_016);  //Y�{�^��
    SetUI(D3DXVECTOR3(120.0f, 500.0f, 0.0f), 200.0f, 50.0f, UITYPE_017);  //�v���XR
    SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), 600.0f, 80.0f, UITYPE_018);   //�X�s�j���O�u���[�h
    SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), 600.0f, 80.0f, UITYPE_019);   //�X�v���b�h�t�@�C�A
    SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, 50.0f, 0.0f), 500.0f, 30.0f, UITYPE_020);   //�{�X��HP�Q�[�W�̔w�i
    SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, 50.0f, 0.0f), 500.0f - BAR_EXTEND, 30.0f - BAR_EXTEND, UITYPE_021);   //�{�X��HP�Q�[�W
    SetUI(D3DXVECTOR3(450.0f, 20.0f, 0.0f), 120.0f, 30.0f, UITYPE_022);   //BOSS�̕���

    //�ǂ̔z�u
    SetWall(D3DXVECTOR3(-WALL_SIZE, WALL_SIZE_Y, WALL_SIZE*3), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW); //��1
    SetWall(D3DXVECTOR3(-WALL_SIZE, WALL_SIZE_Y, WALL_SIZE), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW); //��2
    SetWall(D3DXVECTOR3(-WALL_SIZE, WALL_SIZE_Y, -WALL_SIZE), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW); //��3
    SetWall(D3DXVECTOR3(-WALL_SIZE, WALL_SIZE_Y, -WALL_SIZE*3), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW); //��4
    SetWall(D3DXVECTOR3(WALL_SIZE, WALL_SIZE_Y, WALL_SIZE), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW);  //�E1
    SetWall(D3DXVECTOR3(WALL_SIZE, WALL_SIZE_Y, WALL_SIZE*3), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW);  //�E2
    SetWall(D3DXVECTOR3(WALL_SIZE, WALL_SIZE_Y, -WALL_SIZE), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW);  //�E3
    SetWall(D3DXVECTOR3(WALL_SIZE, WALL_SIZE_Y, -WALL_SIZE*3), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW);  //�E4
    SetWall(D3DXVECTOR3(0.0f, WALL_SIZE_Y, -WALL_SIZE * 2), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f), WALLTYPE_RED1); //�O1
    SetWall(D3DXVECTOR3(0.0f, WALL_SIZE_Y, 0.0f), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f), WALLTYPE_RED2); //�O2
    SetWall(D3DXVECTOR3(0.0f, WALL_SIZE_Y, WALL_SIZE * 2), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f), WALLTYPE_RED3); //�O3
    SetWall(D3DXVECTOR3(0.0f, WALL_SIZE_Y, WALL_SIZE * 4), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f), WALLTYPE_YELLOW); //�O4
    SetWall(D3DXVECTOR3(0.0f, WALL_SIZE_Y, -WALL_SIZE * 4), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f), WALLTYPE_YELLOW);  //��

    //���f���̔z�u
    SetModel(D3DXVECTOR3(3000.0f, 0.0f, -3600.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_000);
    SetModel(D3DXVECTOR3(-300.0f, 0.0f, -3700.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_001);
    SetModel(D3DXVECTOR3(-2910.0f, 0.0f, -3300.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_000);
    SetModel(D3DXVECTOR3(-600.0f, 0.0f, -3400.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_001);
    SetModel(D3DXVECTOR3(800.0f, 0.0f, -2600.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_000);
    SetModel(D3DXVECTOR3(-100.0f, 0.0f, -2700.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_001);
    SetModel(D3DXVECTOR3(40.0f, 0.0f, -1700.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_000);
    SetModel(D3DXVECTOR3(-750.0f, 0.0f, -1900.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_001);
    SetModel(D3DXVECTOR3(4200.0f, 0.0f, -1800.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_000);
    SetModel(D3DXVECTOR3(-3750.0f, 0.0f, -1600.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_001);
    SetModel(D3DXVECTOR3(2200.0f, 0.0f, 3600.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_000);
    SetModel(D3DXVECTOR3(-2300.0f, 0.0f, 3700.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_001);
    SetModel(D3DXVECTOR3(510.0f, 0.0f, 3300.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_000);
    SetModel(D3DXVECTOR3(-2600.0f, 0.0f, 3400.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_001);
    SetModel(D3DXVECTOR3(3800.0f, 0.0f, 2600.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_000);
    SetModel(D3DXVECTOR3(-500.0f, 0.0f, 2700.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_001);
    SetModel(D3DXVECTOR3(140.0f, 0.0f, 1700.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_000);
    SetModel(D3DXVECTOR3(-1750.0f, 0.0f, 1900.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_001);
    SetModel(D3DXVECTOR3(-460.0f, 0.0f, -10.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_001);
    SetModel(D3DXVECTOR3(1996.0f, 0.0f, 1015.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_000);
    SetModel(D3DXVECTOR3(4646.0f, 0.0f, 46.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_001);
    SetModel(D3DXVECTOR3(-320.0f, 0.0f, -320.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_000);
    SetModel(D3DXVECTOR3(-826.0f, 0.0f, 510.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_001);
    SetModel(D3DXVECTOR3(1999.0f, 0.0f, -700.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_000);
    SetModel(D3DXVECTOR3(-4200.0f, 0.0f, 800.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_001);
    SetModel(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_002);//�Ƃ�
    SetModel(D3DXVECTOR3(-4800.0f, 700.0f, 3800.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_003);//��
    SetModel(D3DXVECTOR3(-3000.0f, 0.0f, 0.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_004);    //�s���~�b�h

    //�A�C�e���̔z�u
    SetItem(D3DXVECTOR3(-BLOCK_SIZE_X * 3, 325.0f, -BLOCK_SIZE_Z), ITEM_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_001); //�_�C��
    SetItem(D3DXVECTOR3(BLOCK_SIZE_X * 3, 325.0f, -BLOCK_SIZE_Z), ITEM_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_001); //�_�C��
    SetItem(D3DXVECTOR3(-BLOCK_SIZE_X * 3, 325.0f, 1200.0f + BLOCK_SIZE_Z * 3), ITEM_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_001);    //�_�C��
    SetItem(D3DXVECTOR3(BLOCK_SIZE_X * 3, 325.0f, 1200.0f + BLOCK_SIZE_Z * 3), ITEM_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_001);    //�_�C��

    // �u���b�N�̔z�u
    SetBlock(D3DXVECTOR3(-BLOCK_SIZE_X*2, 0.0f, -BLOCK_SIZE_Z*2), D3DXVECTOR3((BLOCK_SIZE_X), (BLOCK_SIZE_Y), (BLOCK_SIZE_Z)),BLOCKTYPE_000);
    SetBlock(D3DXVECTOR3(-BLOCK_SIZE_X * 3, 100.0f, -BLOCK_SIZE_Z), D3DXVECTOR3((BLOCK_SIZE_X), (BLOCK_SIZE_Y), (BLOCK_SIZE_Z)), BLOCKTYPE_000);
    SetBlock(D3DXVECTOR3(-BLOCK_SIZE_X * 4, 200.0f, 0.0f), D3DXVECTOR3((BLOCK_SIZE_X), (BLOCK_SIZE_Y), (BLOCK_SIZE_Z)), BLOCKTYPE_000);
    SetBlock(D3DXVECTOR3(BLOCK_SIZE_X * 2, 0.0f, -BLOCK_SIZE_Z * 2), D3DXVECTOR3((BLOCK_SIZE_X), (BLOCK_SIZE_Y), (BLOCK_SIZE_Z)), BLOCKTYPE_000);
    SetBlock(D3DXVECTOR3(BLOCK_SIZE_X * 3, 100.0f, -BLOCK_SIZE_Z), D3DXVECTOR3((BLOCK_SIZE_X), (BLOCK_SIZE_Y), (BLOCK_SIZE_Z)), BLOCKTYPE_000);
    SetBlock(D3DXVECTOR3(BLOCK_SIZE_X * 4, 200.0f, 0.0f), D3DXVECTOR3((BLOCK_SIZE_X), (BLOCK_SIZE_Y), (BLOCK_SIZE_Z)), BLOCKTYPE_000);
    SetBlock(D3DXVECTOR3(-BLOCK_SIZE_X * 4, 200.0f, 1200.0f + BLOCK_SIZE_Z * 2), D3DXVECTOR3((BLOCK_SIZE_X), (BLOCK_SIZE_Y), (BLOCK_SIZE_Z)), BLOCKTYPE_000);
    SetBlock(D3DXVECTOR3(-BLOCK_SIZE_X * 3, 100.0f, 1200.0f + BLOCK_SIZE_Z * 3), D3DXVECTOR3((BLOCK_SIZE_X), (BLOCK_SIZE_Y), (BLOCK_SIZE_Z)), BLOCKTYPE_000);
    SetBlock(D3DXVECTOR3(-BLOCK_SIZE_X * 2, 0.0f, 1200.0f + BLOCK_SIZE_Z * 4), D3DXVECTOR3((BLOCK_SIZE_X), (BLOCK_SIZE_Y), (BLOCK_SIZE_Z)), BLOCKTYPE_000);
    SetBlock(D3DXVECTOR3(BLOCK_SIZE_X * 4, 200.0f, 1200.0f + BLOCK_SIZE_Z * 2), D3DXVECTOR3((BLOCK_SIZE_X), (BLOCK_SIZE_Y), (BLOCK_SIZE_Z)), BLOCKTYPE_000);
    SetBlock(D3DXVECTOR3(BLOCK_SIZE_X * 3, 100.0f, 1200.0f + BLOCK_SIZE_Z * 3), D3DXVECTOR3((BLOCK_SIZE_X), (BLOCK_SIZE_Y), (BLOCK_SIZE_Z)), BLOCKTYPE_000);
    SetBlock(D3DXVECTOR3(BLOCK_SIZE_X * 2, 0.0f, 1200.0f + BLOCK_SIZE_Z * 4), D3DXVECTOR3((BLOCK_SIZE_X), (BLOCK_SIZE_Y), (BLOCK_SIZE_Z)), BLOCKTYPE_000);

    //�G�l�~�[�̐ݒu
    //��1�t�F�[�Y
    SetEnemy(D3DXVECTOR3(0.0f, 0.0f, -2500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_000);   //�L���m���i�L�[�j
    SetEnemy(D3DXVECTOR3(400.0f, 0.0f, -2800.0f), D3DXVECTOR3(5.0f, 0.0f, 0.0f), ENEMYTYPE_001);    //�X�p�C�_�[
    SetEnemy(D3DXVECTOR3(-400.0f, 0.0f, -2800.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), ENEMYTYPE_001);   //�X�p�C�_�[

    //��2�t�F�[�Y
    SetEnemy(D3DXVECTOR3(0.0f, 150.0f, -300.0f), D3DXVECTOR3(3.0f, 0.0f, 0.0f), ENEMYTYPE_002);   //�o�^�t���C�i�L�[�j
    SetEnemy(D3DXVECTOR3(0.0f, 0.0f, -650.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), ENEMYTYPE_004);     //�W�����v�{�b�N�X
    SetEnemy(D3DXVECTOR3(0.0f, 0.0f, -1300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_003);    //�S�[���f���X�p�C�_�[
    SetEnemy(D3DXVECTOR3(380.0f, 0.0f, -1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_000);    //�L���m��
    SetEnemy(D3DXVECTOR3(-380.0f, 0.0f, -1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_000);    //�L���m��

    //��3�t�F�[�Y
    SetEnemy(D3DXVECTOR3(380.0f, 0.0f, 1100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_005);     //�t�B�b�V��(�L�[)
    SetEnemy(D3DXVECTOR3(-380.0f, 0.0f, 1100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_005);    //�t�B�b�V��(�L�[)
    SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 1500.0f), D3DXVECTOR3(-4.0f, 0.0f, 0.0f), ENEMYTYPE_004);     //�W�����v�{�b�N�X
    SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 1300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_003);    //�S�[���f���X�p�C�_�[
    SetEnemy(D3DXVECTOR3(500.0f, 150.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 3.0f), ENEMYTYPE_002);   //�o�^�t���C
    SetEnemy(D3DXVECTOR3(-500.0f, 150.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), ENEMYTYPE_002);   //�o�^�t���C

    //��4�t�F�[�Y
    
    //�Q�[����BGM
    PlaySound(SOUND_LABEL_BGM001);

	//�O���[�o���ϐ��̏�����
    g_campaignstate = CAMPAIGNSTATE_NORMAL;
    g_nCntCampaign = 0;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCampaign(void)
{
    //�{�X��J�b�g�C�����A�v���C���[�ƃG�l�~�[�̍X�V���~�߂邽��
    CAMERA*pCamera = GetCamera();

    //�Ԍ���̂��߂̕ϐ�
    float fSpeedX = 0.0f;   //����X
    float fSpeedY = 0.0f;   //����Y
    float fSpeedZ = 0.0f;   //����Z
    int nPlusMinusX = 0;
    int nPlusMinusY = 0;
    int nPlusMinusZ = 0;
    float fAngle = 0.0f;    //�~��ɂ��邽��
    float fRadius = 0.0f; //�G�t�F�N�g�������a

	switch (g_campaignstate)
	{
	case CAMPAIGNSTATE_NORMAL:
        UpdateCamera();		//�J����
        UpdateLight();		//���C�g
        UpdateWall();
        UpdateBg();
        //�{�X��J�b�g�C�����A�v���C���[�ƃG�l�~�[�͍X�V���Ȃ�
        if (pCamera->state == CAMERASTATE_NORMAL)
        {
            UpdatePlayer();		//�v���C���[
            UpdateEnemy();
        }
        UpdateBoss();
        UpdateItem();
        UpdateBullet();
        UpdateEffect();
        UpdateEffect2();
        //UpdateShadow();
        UpdateUI();
        UpdateBlock();
        UpdateMeshField();
        UpdateScore();
        UpdateTime();
        //UpdateEnemyLife();

        //�Ԍ���G�t�F�N�g
        for (int nCnt = 0; nCnt < 2; nCnt++)
        {
            fSpeedX = float(rand() % 400 + 0) / 100.0f;
            fSpeedY = float(rand() % 1000 + 500) / 100.0f;
            fSpeedZ = float(rand() % 400 + 0) / 100.0f;
            nPlusMinusX = int(rand() % 2 + 1);
            nPlusMinusY = int(rand() % 2 + 1);
            nPlusMinusZ = int(rand() % 2 + 1);
            //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            fRadius = float(rand() % 5000 + 0) / 100.0f;

            if (nPlusMinusX == 1)
            {
                nPlusMinusX = 1;
            }
            else
            {
                nPlusMinusX = -1;
            }

            if (nPlusMinusY == 1)
            {
                nPlusMinusY = 1;
            }
            else
            {
                nPlusMinusY = 1;
            }

            if (nPlusMinusZ == 1)
            {
                nPlusMinusZ = 1;
            }
            else
            {
                nPlusMinusZ = -1;
            }

            D3DXVECTOR3 pos = D3DXVECTOR3(0.0f - sinf(fAngle)*fRadius, 0.0f, 800.0f - cosf(fAngle)*fRadius);
            D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX*nPlusMinusX, fSpeedY*nPlusMinusY, fSpeedZ*nPlusMinusZ);
            SetEffect2(pos, move, EFFECT2_SIZE_EXPLOSION, EFFECT2_INIT_COLOR_GEYSER, EFFECT2TYPE_GEYSER); //�Ԍ���
        }

        //�|�[�Y��
        if (GetJoypadTrigger(BUTTON_START))
        {
            g_campaignstate = CAMPAIGNSTATE_PAUSE;
        }

		break;

		//�Q�[���I�[�o�[
	case CAMPAIGNSTATE_GAMEOVER:
        UpdateCamera();		//�J����
        UpdateLight();		//���C�g
        UpdateWall();
        UpdateBg();
        //�{�X��J�b�g�C�����A�v���C���[�ƃG�l�~�[�͍X�V���Ȃ�
        if (pCamera->state == CAMERASTATE_NORMAL)
        {
            UpdatePlayer();		//�v���C���[
            UpdateEnemy();
        }
        UpdateBoss();
        UpdateItem();
        UpdateBullet();
        UpdateEffect();
        UpdateEffect2();
        //UpdateShadow();
        UpdateUI();
        UpdateBlock();
        UpdateMeshField();
        UpdateScore();
        //UpdateEnemyLife();

        //�Ԍ���G�t�F�N�g
        for (int nCnt = 0; nCnt < 2; nCnt++)
        {
            fSpeedX = float(rand() % 400 + 0) / 100.0f;
            fSpeedY = float(rand() % 1000 + 500) / 100.0f;
            fSpeedZ = float(rand() % 400 + 0) / 100.0f;
            nPlusMinusX = int(rand() % 2 + 1);
            nPlusMinusY = int(rand() % 2 + 1);
            nPlusMinusZ = int(rand() % 2 + 1);
            //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            fRadius = float(rand() % 5000 + 0) / 100.0f;

            if (nPlusMinusX == 1)
            {
                nPlusMinusX = 1;
            }
            else
            {
                nPlusMinusX = -1;
            }

            if (nPlusMinusY == 1)
            {
                nPlusMinusY = 1;
            }
            else
            {
                nPlusMinusY = 1;
            }

            if (nPlusMinusZ == 1)
            {
                nPlusMinusZ = 1;
            }
            else
            {
                nPlusMinusZ = -1;
            }

            D3DXVECTOR3 pos = D3DXVECTOR3(0.0f - sinf(fAngle)*fRadius, 0.0f, 800.0f - cosf(fAngle)*fRadius);
            D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX*nPlusMinusX, fSpeedY*nPlusMinusY, fSpeedZ*nPlusMinusZ);
            SetEffect2(pos, move, EFFECT2_SIZE_EXPLOSION, EFFECT2_INIT_COLOR_GEYSER, EFFECT2TYPE_GEYSER); //�Ԍ���
        }

        g_nCntCampaign++;
		if (g_nCntCampaign >= COUNT_WAIT_FINISH_GAME)
		{
            //�Q�[����BGM���Ƃ߂�
            StopSound(SOUND_LABEL_BGM001);
            StopSound(SOUND_LABEL_BGM004);
            g_campaignstate = CAMPAIGNSTATE_NONE;
			SetFade(MODE_RESULT);   //���U���g��
		}
		break;

        //�|�[�Y
    case CAMPAIGNSTATE_PAUSE:
        UpdatePause();

    	break;
	}

}

//=============================================================================
// �I������
//=============================================================================
void UninitCampaign(void)
{
    UninitPause();
	UninitCamera();//�J�����̏I��
	UninitLight();//���C�g�̏I��
    UninitWall();
    UninitBg();
	UninitPlayer();	//�v���C���[�̏I��
    UninitEnemy();
    UninitBoss();
    UninitItem();
    UninitBullet();
    UninitEffect();
    UninitEffect2();
    UninitUI();
	UninitShadow();
	UninitBlock();
	UninitMeshField();
    UninitScore();
    UninitTime();
    //UninitEnemyLife();
    UninitModel();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCampaign(void)
{
	SetCamera();					//�J�����̐ݒ�
    DrawEffect();
    DrawPlayer();//�v���C���[�̕`��
    DrawEnemy();
    DrawBoss();
    DrawMeshField();
    //DrawShadow();
    DrawBlock();
    DrawModel();
    DrawItem();
    DrawBullet();
    //DrawEnemyLife();
	DrawUI();
    DrawScore();
    DrawBg();
    DrawWall();
    DrawEffect2();

    if (g_campaignstate == CAMPAIGNSTATE_PAUSE)
    {
        DrawPause();
    }
}

//=============================================================================
// �Q�[����Ԃ̐ݒ�
//=============================================================================
void SetCampaignState(CAMPAIGNSTATE state)
{
	g_campaignstate = state;
    g_nCntCampaign = 0;
}
