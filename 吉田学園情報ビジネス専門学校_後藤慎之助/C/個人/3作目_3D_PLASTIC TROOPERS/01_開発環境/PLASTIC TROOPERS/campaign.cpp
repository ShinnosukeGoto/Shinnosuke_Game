//=============================================================================
//
// PLASTIC TROOPERS [campaign.cpp]の処理 (ボスを倒しに行くモード)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
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
//グローバル変数宣言
//=============================================================================
CAMPAIGNSTATE g_campaignstate;
int g_nCntCampaign = 0;	//次の画面へ行くまでの時間

							//GAMELEVEL g_nGameLevel;		//ゲームの難易度

//=============================================================================
//ゲーム初期化処理
//=============================================================================
void InitCampaign(void)
{
    InitPause();
	InitCamera();					//カメラの初期化
	InitLight();					//ライトの初期化
	InitMeshField();
    InitWall();
	InitPlayer();					//自機の初期化
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

    //UIの配置
    SetUI(D3DXVECTOR3(1085.0f, 50.0f, 0.0f), 300.0f, 70.0f, UITYPE_000);   //スコアの背景
    SetUI(D3DXVECTOR3(165.0f, 50.0f, 0.0f), 300.0f, 70.0f, UITYPE_001);    //キャンペーン
    SetUI(D3DXVECTOR3(120.0f, 600.0f, 0.0f), 150.0f, 150.0f, UITYPE_005);  //必殺ゲージの背景
    SetUI(D3DXVECTOR3(320.0f, 600.0f, 0.0f), 250.0f, 50.0f, UITYPE_006);   //HPゲージの背景
    SetUI(D3DXVECTOR3(SCREEN_WIDTH/2, 650.0f, 0.0f), 350.0f, 50.0f, UITYPE_006);   //ENERGYゲージの背景
    SetUI(D3DXVECTOR3(220.0f, 560.0f, 0.0f), 60.0f, 40.0f, UITYPE_008);   //HP
    SetUI(D3DXVECTOR3(535.0f, 610.0f, 0.0f), 120.0f, 40.0f, UITYPE_009);  //ENERGY
    SetUI(D3DXVECTOR3(320.0f, 600.0f, 0.0f), 250.0f - BAR_EXTEND, 50.0f - BAR_EXTEND, UITYPE_010);   //HPゲージ
    SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, 650.0f, 0.0f), 350.0f - BAR_EXTEND, 50.0f - BAR_EXTEND, UITYPE_011);   //ENERGYゲージ
    SetUI(D3DXVECTOR3(120.0f, 600.0f, 0.0f), 150.0f - BAR_EXTEND, 150.0f - BAR_EXTEND, UITYPE_012);  //必殺ゲージ
    SetUI(D3DXVECTOR3(120.0f, 600.0f, 0.0f), 125.0f, 125.0f, UITYPE_013);  //必殺の数字
    SetUI(D3DXVECTOR3(955.0f, 620.0f, 0.0f), 110.0f, 110.0f, UITYPE_014);  //剣
    SetUI(D3DXVECTOR3(1145.0f, 520.0f, 0.0f), 110.0f, 110.0f, UITYPE_015);  //銃
    SetUI(D3DXVECTOR3(1050.0f, 570.0f, 0.0f), 110.0f, 110.0f, UITYPE_016);  //Yボタン
    SetUI(D3DXVECTOR3(120.0f, 500.0f, 0.0f), 200.0f, 50.0f, UITYPE_017);  //プレスR
    SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), 600.0f, 80.0f, UITYPE_018);   //スピニングブレード
    SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), 600.0f, 80.0f, UITYPE_019);   //スプレッドファイア
    SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, 50.0f, 0.0f), 500.0f, 30.0f, UITYPE_020);   //ボスのHPゲージの背景
    SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, 50.0f, 0.0f), 500.0f - BAR_EXTEND, 30.0f - BAR_EXTEND, UITYPE_021);   //ボスのHPゲージ
    SetUI(D3DXVECTOR3(450.0f, 20.0f, 0.0f), 120.0f, 30.0f, UITYPE_022);   //BOSSの文字

    //壁の配置
    SetWall(D3DXVECTOR3(-WALL_SIZE, WALL_SIZE_Y, WALL_SIZE*3), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW); //左1
    SetWall(D3DXVECTOR3(-WALL_SIZE, WALL_SIZE_Y, WALL_SIZE), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW); //左2
    SetWall(D3DXVECTOR3(-WALL_SIZE, WALL_SIZE_Y, -WALL_SIZE), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW); //左3
    SetWall(D3DXVECTOR3(-WALL_SIZE, WALL_SIZE_Y, -WALL_SIZE*3), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW); //左4
    SetWall(D3DXVECTOR3(WALL_SIZE, WALL_SIZE_Y, WALL_SIZE), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW);  //右1
    SetWall(D3DXVECTOR3(WALL_SIZE, WALL_SIZE_Y, WALL_SIZE*3), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW);  //右2
    SetWall(D3DXVECTOR3(WALL_SIZE, WALL_SIZE_Y, -WALL_SIZE), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW);  //右3
    SetWall(D3DXVECTOR3(WALL_SIZE, WALL_SIZE_Y, -WALL_SIZE*3), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, D3DXToRadian(90.0f)), WALLTYPE_YELLOW);  //右4
    SetWall(D3DXVECTOR3(0.0f, WALL_SIZE_Y, -WALL_SIZE * 2), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f), WALLTYPE_RED1); //前1
    SetWall(D3DXVECTOR3(0.0f, WALL_SIZE_Y, 0.0f), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f), WALLTYPE_RED2); //前2
    SetWall(D3DXVECTOR3(0.0f, WALL_SIZE_Y, WALL_SIZE * 2), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f), WALLTYPE_RED3); //前3
    SetWall(D3DXVECTOR3(0.0f, WALL_SIZE_Y, WALL_SIZE * 4), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f), WALLTYPE_YELLOW); //前4
    SetWall(D3DXVECTOR3(0.0f, WALL_SIZE_Y, -WALL_SIZE * 4), D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f), WALLTYPE_YELLOW);  //後

    //モデルの配置
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
    SetModel(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_002);//とげ
    SetModel(D3DXVECTOR3(-4800.0f, 700.0f, 3800.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_003);//月
    SetModel(D3DXVECTOR3(-3000.0f, 0.0f, 0.0f), D3DXVECTOR3((MODEL_SIZE_X), (MODEL_SIZE_X), (MODEL_SIZE_X)), MODELTYPE_004);    //ピラミッド

    //アイテムの配置
    SetItem(D3DXVECTOR3(-BLOCK_SIZE_X * 3, 325.0f, -BLOCK_SIZE_Z), ITEM_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_001); //ダイヤ
    SetItem(D3DXVECTOR3(BLOCK_SIZE_X * 3, 325.0f, -BLOCK_SIZE_Z), ITEM_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_001); //ダイヤ
    SetItem(D3DXVECTOR3(-BLOCK_SIZE_X * 3, 325.0f, 1200.0f + BLOCK_SIZE_Z * 3), ITEM_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_001);    //ダイヤ
    SetItem(D3DXVECTOR3(BLOCK_SIZE_X * 3, 325.0f, 1200.0f + BLOCK_SIZE_Z * 3), ITEM_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_001);    //ダイヤ

    // ブロックの配置
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

    //エネミーの設置
    //第1フェーズ
    SetEnemy(D3DXVECTOR3(0.0f, 0.0f, -2500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_000);   //キャノン（キー）
    SetEnemy(D3DXVECTOR3(400.0f, 0.0f, -2800.0f), D3DXVECTOR3(5.0f, 0.0f, 0.0f), ENEMYTYPE_001);    //スパイダー
    SetEnemy(D3DXVECTOR3(-400.0f, 0.0f, -2800.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), ENEMYTYPE_001);   //スパイダー

    //第2フェーズ
    SetEnemy(D3DXVECTOR3(0.0f, 150.0f, -300.0f), D3DXVECTOR3(3.0f, 0.0f, 0.0f), ENEMYTYPE_002);   //バタフライ（キー）
    SetEnemy(D3DXVECTOR3(0.0f, 0.0f, -650.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), ENEMYTYPE_004);     //ジャンプボックス
    SetEnemy(D3DXVECTOR3(0.0f, 0.0f, -1300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_003);    //ゴールデンスパイダー
    SetEnemy(D3DXVECTOR3(380.0f, 0.0f, -1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_000);    //キャノン
    SetEnemy(D3DXVECTOR3(-380.0f, 0.0f, -1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_000);    //キャノン

    //第3フェーズ
    SetEnemy(D3DXVECTOR3(380.0f, 0.0f, 1100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_005);     //フィッシュ(キー)
    SetEnemy(D3DXVECTOR3(-380.0f, 0.0f, 1100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_005);    //フィッシュ(キー)
    SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 1500.0f), D3DXVECTOR3(-4.0f, 0.0f, 0.0f), ENEMYTYPE_004);     //ジャンプボックス
    SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 1300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_003);    //ゴールデンスパイダー
    SetEnemy(D3DXVECTOR3(500.0f, 150.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 3.0f), ENEMYTYPE_002);   //バタフライ
    SetEnemy(D3DXVECTOR3(-500.0f, 150.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), ENEMYTYPE_002);   //バタフライ

    //第4フェーズ
    
    //ゲームのBGM
    PlaySound(SOUND_LABEL_BGM001);

	//グローバル変数の初期化
    g_campaignstate = CAMPAIGNSTATE_NORMAL;
    g_nCntCampaign = 0;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCampaign(void)
{
    //ボス戦カットイン時、プレイヤーとエネミーの更新を止めるため
    CAMERA*pCamera = GetCamera();

    //間欠泉のための変数
    float fSpeedX = 0.0f;   //速さX
    float fSpeedY = 0.0f;   //速さY
    float fSpeedZ = 0.0f;   //速さZ
    int nPlusMinusX = 0;
    int nPlusMinusY = 0;
    int nPlusMinusZ = 0;
    float fAngle = 0.0f;    //円状にするため
    float fRadius = 0.0f; //エフェクト発生半径

	switch (g_campaignstate)
	{
	case CAMPAIGNSTATE_NORMAL:
        UpdateCamera();		//カメラ
        UpdateLight();		//ライト
        UpdateWall();
        UpdateBg();
        //ボス戦カットイン時、プレイヤーとエネミーは更新しない
        if (pCamera->state == CAMERASTATE_NORMAL)
        {
            UpdatePlayer();		//プレイヤー
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

        //間欠泉エフェクト
        for (int nCnt = 0; nCnt < 2; nCnt++)
        {
            fSpeedX = float(rand() % 400 + 0) / 100.0f;
            fSpeedY = float(rand() % 1000 + 500) / 100.0f;
            fSpeedZ = float(rand() % 400 + 0) / 100.0f;
            nPlusMinusX = int(rand() % 2 + 1);
            nPlusMinusY = int(rand() % 2 + 1);
            nPlusMinusZ = int(rand() % 2 + 1);
            //0～3.14を出す*(0 or 1)*-1で正負を決める
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
            SetEffect2(pos, move, EFFECT2_SIZE_EXPLOSION, EFFECT2_INIT_COLOR_GEYSER, EFFECT2TYPE_GEYSER); //間欠泉
        }

        //ポーズへ
        if (GetJoypadTrigger(BUTTON_START))
        {
            g_campaignstate = CAMPAIGNSTATE_PAUSE;
        }

		break;

		//ゲームオーバー
	case CAMPAIGNSTATE_GAMEOVER:
        UpdateCamera();		//カメラ
        UpdateLight();		//ライト
        UpdateWall();
        UpdateBg();
        //ボス戦カットイン時、プレイヤーとエネミーは更新しない
        if (pCamera->state == CAMERASTATE_NORMAL)
        {
            UpdatePlayer();		//プレイヤー
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

        //間欠泉エフェクト
        for (int nCnt = 0; nCnt < 2; nCnt++)
        {
            fSpeedX = float(rand() % 400 + 0) / 100.0f;
            fSpeedY = float(rand() % 1000 + 500) / 100.0f;
            fSpeedZ = float(rand() % 400 + 0) / 100.0f;
            nPlusMinusX = int(rand() % 2 + 1);
            nPlusMinusY = int(rand() % 2 + 1);
            nPlusMinusZ = int(rand() % 2 + 1);
            //0～3.14を出す*(0 or 1)*-1で正負を決める
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
            SetEffect2(pos, move, EFFECT2_SIZE_EXPLOSION, EFFECT2_INIT_COLOR_GEYSER, EFFECT2TYPE_GEYSER); //間欠泉
        }

        g_nCntCampaign++;
		if (g_nCntCampaign >= COUNT_WAIT_FINISH_GAME)
		{
            //ゲームのBGMをとめる
            StopSound(SOUND_LABEL_BGM001);
            StopSound(SOUND_LABEL_BGM004);
            g_campaignstate = CAMPAIGNSTATE_NONE;
			SetFade(MODE_RESULT);   //リザルトへ
		}
		break;

        //ポーズ
    case CAMPAIGNSTATE_PAUSE:
        UpdatePause();

    	break;
	}

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCampaign(void)
{
    UninitPause();
	UninitCamera();//カメラの終了
	UninitLight();//ライトの終了
    UninitWall();
    UninitBg();
	UninitPlayer();	//プレイヤーの終了
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
// 描画処理
//=============================================================================
void DrawCampaign(void)
{
	SetCamera();					//カメラの設定
    DrawEffect();
    DrawPlayer();//プレイヤーの描画
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
// ゲーム状態の設定
//=============================================================================
void SetCampaignState(CAMPAIGNSTATE state)
{
	g_campaignstate = state;
    g_nCntCampaign = 0;
}
