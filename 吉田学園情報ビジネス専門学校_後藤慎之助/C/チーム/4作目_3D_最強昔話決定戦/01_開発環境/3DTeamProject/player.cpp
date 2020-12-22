//=============================================================================
//
// 最強昔話決定戦の処理[player.cpp]（自機1）
// Author:後藤慎之助
//
//=============================================================================

//===================================================================================================
// インクルードファイル
//===================================================================================================
#include "player.h"
#include "camera.h"
#include "input.h"
#include "controller.h"
#include "block.h"
#include "sound.h"
#include "stage.h"
#include "bomb.h"
#include "fire.h"
#include "game.h"
#include "item.h"
#include "collision.h"
#include "effect.h"
#include "shadow.h"
#include "setting.h"
#include "manual.h"
#include "enemy.h"
#include "bullet.h"
#include "ui.h"
#include "boss.h"

//===================================================================================================
// グローバル変数定義
//===================================================================================================
PLAYER g_Player;        //プレイヤーの情報

int g_nFrameCountPlayer;				// 現在のフレーム数

int g_nPlayKeySetlndexPlayer;			// 再生中のキーセットインデックス

int g_nPlayerCurrentAnimationPlayer;	// 再生中のアニメーション

int g_nFremePlayer;					// フレーム数

int g_nCntKeyPlayer;					// キー数

int g_nChangeFlamePlayer;			// モーションのフレーム

MOTIONSTATE_PLAYER g_MotionState;			// モーションの構造体
MOTIONSTATE_PLAYER g_MotionOldState;		// 1F前のモーションの構造体

D3DXVECTOR3 g_aNumPlayerPos[MAX_PARTS];		// posの差分
D3DXVECTOR3 g_aNumPlayerRot[MAX_PARTS];		// rotの差分
D3DXVECTOR3 g_aNumPlayerRotDest[MAX_PARTS];	// rotの差分

ANIMATION_PLAYER g_aPlayerMotion[MOTIONSTATE_PLAYER_MAX];	// モーションの情報
                                    // モデルパーツのデータ
ModelParts_Player g_aPlayer[MAX_PARTS] =
{
    { "MODEL/player/00_body.x" },
    { "MODEL/player/01_head.x" },
    { "MODEL/player/02_armR.x" },
    { "MODEL/player/03_handR.x" },
    { "MODEL/player/04_armL.x" },
    { "MODEL/player/05_handL.x" },
    { "MODEL/player/06_legR.x" },
    { "MODEL/player/07_footR.x" },
    { "MODEL/player/08_legL.x" },
    { "MODEL/player/09_footL.x" },
};

bool g_bEvilMomo;       //殺意の波動に目覚めているかどうか

bool g_bSetSnake;       //ツチノコをセットしたかどうか

bool g_bSetFenceBlock;  //柵をセットしたかどうか

int g_nCntStunPlayer;   //スタンの時間を数える

//===================================================================================================
// プレイヤー初期化処理
// Author:後藤慎之助
//===================================================================================================
void InitPlayer(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //ゲーム状態の取得
    GAME *pGame = GetGame();

    //設定の取得
    SETTING *pSetting = GetSetting();

    //隠しコマンド使用判定を取得
    g_bEvilMomo = GetHiddenCommand();

    //グローバル変数の初期化
    g_MotionState = MOTIONSTATE_PLAYER_IDLE;

    g_nFrameCountPlayer = 0;				// 現在のフレーム数

    g_nPlayKeySetlndexPlayer = 0;			// 再生中のキーセットインデックス

    g_nPlayerCurrentAnimationPlayer = MOTIONSTATE_PLAYER_IDLE;	// 再生中のアニメーション

    g_nFremePlayer = 0;					// フレーム数

    g_nCntKeyPlayer = 1;				// キー数

    g_nChangeFlamePlayer = 20;			// モーションのフレーム

    //ストーリーに使う、グローバル変数の初期化
    g_bSetSnake = false;
    g_bSetFenceBlock = false;
    g_nCntStunPlayer = PLAYER_STUN_TIME;

    //構造体の初期化
    g_Player.pos = pGame->startPos1;		            // 初期位置
    g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期位置
    g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 初期向き
    g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	    // 初期移動量
    g_Player.posGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期位置
    g_Player.bDisp = true;
    //ストーリーかどうか
    if (pSetting->nMaxPlayer == 1)
    {
        //難易度:ふつう
        if (pSetting->nLevel == SETTINGLEVEL_NORMAL)
        {
            g_Player.fLife = PLAYER_STORY_LIFE;	//自機の体力
        }

        //難易度:鬼
        else
        {
            g_Player.fLife = PLAYER_LIFE;	//自機の体力
        }
    }
    else
    {
        //隠しコマンド使用
        if (g_bEvilMomo == true)
        {
            g_Player.fLife = BOMB_DAMAGE + PLAYER_LIFE;	    //殺意の波動は爆風を一回耐える
        }

        //隠しコマンド未使用
        else
        {
            g_Player.fLife = PLAYER_LIFE;
        }
        
    }
    g_Player.fSpeed = VALUE_MOVE;
    //隠しコマンド使用
    if (g_bEvilMomo == true)
    {
        g_Player.nSpeedRank = PLAYER_FIRST_SPEEDRANK + 2;   //殺意の波動は2ランク速い
        g_Player.nBomb = PLAYER_FIRST_BOMB + 1;             //殺意の波動はボムを2つおける
        g_Player.nCanKick = PLAYER_FIRST_KICK + 1;          //殺意の波動はキックできる
    }

    //隠しコマンド未使用
    else
    {
        g_Player.nSpeedRank = PLAYER_FIRST_SPEEDRANK;
        g_Player.nBomb = PLAYER_FIRST_BOMB;
        g_Player.nCanKick = PLAYER_FIRST_KICK;
    }
    g_Player.nFire = PLAYER_FIRST_FIRE;
    g_Player.nShadow = SetShadow(g_Player.pos, g_Player.rot, GRID_SIZE, g_Player.pos.y, 
        D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_ALPHA_MAX));//影の設定
    g_Player.nCntInvincibleFrame = 0;
    g_Player.bTransparence = false;
    g_Player.nMaxBomb = g_Player.nBomb;
    g_Player.bStun = false;

    LoadPlayer();
    LoadPlayerAnimation();

    // モデルパーツの初期化
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        //		g_Player.modelParts[nCntParts].pos = g_aPlayer[nCntParts].pos;			// 初期位置
        //		g_Player.modelParts[nCntParts].rot = g_aPlayer[nCntParts].rot;			// 初期角度

        g_Player.modelParts[nCntParts].nldxModelParent = 0;
    }
    // ファイルの読み込み
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        g_Player.modelParts[nCntParts].nldxModelParent = g_aPlayer[nCntParts].nParents;

        // Xファイルの読み込み
        D3DXLoadMeshFromX(g_aPlayer[nCntParts].pFileName,
            D3DXMESH_SYSTEMMEM,
            pDevice,
            NULL,
            &g_Player.modelParts[nCntParts].pBuffMat,
            NULL,
            &g_Player.modelParts[nCntParts].nNumMat,
            &g_Player.modelParts[nCntParts].pMesh);

    }
}

//===================================================================================================
// プレイヤー終了処理
// Author:後藤慎之助
//===================================================================================================
void UninitPlayer(void)
{
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        // メッシュの破棄
        if (g_Player.modelParts[nCntParts].pMesh != NULL)
        {
            g_Player.modelParts[nCntParts].pMesh->Release();
            g_Player.modelParts[nCntParts].pMesh = NULL;
        }
        // マテリアルの破棄
        if (g_Player.modelParts[nCntParts].pBuffMat != NULL)
        {
            g_Player.modelParts[nCntParts].pBuffMat->Release();
            g_Player.modelParts[nCntParts].pBuffMat = NULL;
        }
    }
}

//===================================================================================================
// プレイヤー更新処理
// Author:後藤慎之助
//===================================================================================================
void UpdatePlayer(void)
{
    //コントローラーの情報を取得する
    DIJOYSTATE2 Controller = GetController(PLAYER_1);
    //カメラの情報を取得する
    CAMERA *pCamera = GetCamera();
    //ゲームの状況を取得する
    GAME *pGame = GetGame();
    //設定の情報を取得する
    SETTING *pSetting = GetSetting();

    //移動ランクに応じた、速さを得る
    switch (g_Player.nSpeedRank)
    {
    case PLAYER_FIRST_SPEEDRANK:
        g_Player.fSpeed = VALUE_MOVE;
        break;

    case PLAYER_FIRST_SPEEDRANK + 1:
        g_Player.fSpeed = VALUE_MOVE + VALUE_UPMOVE;
        break;

    case PLAYER_FIRST_SPEEDRANK + 2:
        g_Player.fSpeed = VALUE_MOVE + VALUE_UPMOVE * 2.0f;
        break;

    case PLAYER_FIRST_SPEEDRANK + 3:
        g_Player.fSpeed = VALUE_MOVE + VALUE_UPMOVE * 3.0f;
        break;

    case PLAYER_FIRST_SPEEDRANK + 4:
        g_Player.fSpeed = VALUE_MOVE + VALUE_UPMOVE * 4.0f;
        break;
    }

    //無敵時間
    if (g_Player.nCntInvincibleFrame > 0)
    {
        g_Player.nCntInvincibleFrame--;                     //無敵時間を数える
        g_Player.bTransparence = !g_Player.bTransparence;   //無敵中は、点滅させる
    }
    else
    {
        g_Player.bTransparence = false; //透明を解除
    }

    //生存しているなら
    if (g_Player.bDisp == true && pGame->state == GAMESTATE_NORMAL && pCamera->state == CAMERASTATE_NORMAL ||
        g_Player.bDisp == true && pGame->state == GAMESTATE_NORMAL && pCamera->state == CAMERASTATE_STORY)
    {
        // プレイヤーが動いていないとき
        D3DXVECTOR3 lengthCheckVec = g_Player.move;
        lengthCheckVec.y = 0;
        if (D3DXVec3Length(&lengthCheckVec) <= 1.5f)
        {
            g_MotionState = MOTIONSTATE_PLAYER_IDLE;    //モーションをアイドルにする
        }

        //1F前の位置を記憶
        g_Player.posOld = g_Player.pos;

        //グリッドの中心の値を得る
        g_Player.posGrid = ConversionGrid(&g_Player.pos);

        //ノックバックを制御
        if (g_Player.move.x != 0.0f || g_Player.move.z != 0.0f)
        {
            g_Player.move.x *= 0.8f;
            g_Player.move.z *= 0.8f;
        }

        //移動量と位置を紐づけ
        g_Player.pos += g_Player.move;

        //スタンしていないなら
        if (g_Player.bStun == false)
        {
            //スティックが倒れているなら移動
            if (Controller.lY != 0 || Controller.lX != 0)
            {
                float fAngle = atan2(Controller.lX, Controller.lY*-1);//スティックの角度を求める
                float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//スティックの角度を求める

                g_Player.pos.x += sinf((pCamera->rot.y + fAngle))*g_Player.fSpeed;
                g_Player.pos.z += cosf((pCamera->rot.y + fAngle))*g_Player.fSpeed;
                g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                                                        //キャラの向きを変える
                g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
            }

            //十字キーでの移動
            else
            {
                //左
                if (Controller.rgdwPOV[0] == BUTTON_LEFT)
                {
                    g_Player.pos.x -= g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x + D3DXToRadian(90.0f);
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                }

                //左上
                if (Controller.rgdwPOV[0] == BUTTON_LEFT_UP)
                {
                    g_Player.pos.x -= sinf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.pos.z += cosf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x + D3DXToRadian(135.0f);
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                }

                //左下
                if (Controller.rgdwPOV[0] == BUTTON_DOWN_LEFT)
                {
                    g_Player.pos.x -= sinf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.pos.z -= cosf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x + D3DXToRadian(45.0f);
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                }

                //右
                if (Controller.rgdwPOV[0] == BUTTON_RIGHT)
                {
                    g_Player.pos.x += g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x - D3DXToRadian(90.0f);
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                }

                //右上
                if (Controller.rgdwPOV[0] == BUTTON_UP_RIGHT)
                {
                    g_Player.pos.x += sinf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.pos.z += cosf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x - D3DXToRadian(135.0f);
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                }

                //右下
                if (Controller.rgdwPOV[0] == BUTTON_RIGHT_DOWN)
                {
                    g_Player.pos.x += sinf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.pos.z -= cosf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x - D3DXToRadian(45.0f);
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                }

                //上
                if (Controller.rgdwPOV[0] == BUTTON_UP)
                {
                    g_Player.pos.z += g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x + D3DX_PI;
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                }

                //下
                if (Controller.rgdwPOV[0] == BUTTON_DOWN)
                {
                    g_Player.pos.z -= g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x;
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                }
            }

            //ストーリーモードの仮の操作===========================================================================================
            if (Controller.lY == 0 && Controller.lX == 0 && Controller.rgdwPOV[0] == BUTTON_NEUTRAL && pSetting->nMaxPlayer == 1)
            {
                //WASDキーで移動
                //Aキーが押された(左移動)
                if (GetKeyboardPress(DIK_A))
                {
                    if (GetKeyboardPress(DIK_W))//AとWが同時に押された
                    {
                        g_Player.pos.x -= sinf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.pos.z += cosf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x + D3DXToRadian(135.0f);
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                    }
                    else if (GetKeyboardPress(DIK_S))//AとSが同時に押された
                    {
                        g_Player.pos.x -= sinf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.pos.z -= cosf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x + D3DXToRadian(45.0f);
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                    }
                    else if (GetKeyboardPress(DIK_D))
                    {

                    }
                    else				//左だけ押した
                    {
                        g_Player.pos.x -= g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x + D3DXToRadian(90.0f);
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                    }

                }

                //Dキーが押された(右移動)
                else if (GetKeyboardPress(DIK_D))
                {
                    if (GetKeyboardPress(DIK_W))//DとWが同時に押された
                    {
                        g_Player.pos.x += sinf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.pos.z += cosf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x - D3DXToRadian(135.0f);
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする

                    }
                    else if (GetKeyboardPress(DIK_S))//DとSが同時に押された
                    {
                        g_Player.pos.x += sinf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.pos.z -= cosf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x - D3DXToRadian(45.0f);
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                    }
                    else if (GetKeyboardPress(DIK_A))
                    {

                    }
                    else				//右だけ押した
                    {
                        g_Player.pos.x += g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x - D3DXToRadian(90.0f);
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                    }

                }

                //Wキーが押された(上移動)
                else if (GetKeyboardPress(DIK_W))
                {
                    if (GetKeyboardPress(DIK_S))
                    {

                    }
                    else
                    {
                        g_Player.pos.z += g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x + D3DX_PI;
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                    }
                }

                //Sキーが押された(下移動)
                else if (GetKeyboardPress(DIK_S))
                {
                    if (GetKeyboardPress(DIK_W))
                    {

                    }
                    else
                    {
                        g_Player.pos.z -= g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x;
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//モーションを歩きにする
                    }
                }

                //爆弾を置く(仮)
                if (GetKeyboardTrigger(DIK_RETURN) && g_Player.nBomb > 0 && g_Player.nCntInvincibleFrame == 0)
                {
                    //爆弾と重なっていないなら、置ける
                    if (HitBomb(&g_Player.pos, &g_Player.posOld,
                        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BOMB + 1) == false)
                    {
                        PlaySound(SOUND_LABEL_SE_SET_BOMB);   //ボム設置音
                        SetBomb(g_Player.posGrid, g_Player.nFire, TIME_BOMB, SELECT_MOMO);
                        g_Player.nBomb--;
                    }

                }
            }
            //===============================================================================================

            //爆弾を置く
            if (GetJoypadTrigger(PLAYER_1, BUTTON_B) && g_Player.nBomb > 0 && g_Player.nCntInvincibleFrame == 0)
            {
                //殺意の波動に目覚めてないなら
                if (g_bEvilMomo == false)
                {
                    //爆弾と重なっていないなら、3秒で爆発するボムを置ける
                    if (HitBomb(&g_Player.pos, &g_Player.posOld,
                        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BOMB + 1) == false)
                    {
                        PlaySound(SOUND_LABEL_SE_SET_BOMB);   //ボム設置音
                        SetBomb(g_Player.posGrid, g_Player.nFire, TIME_BOMB, SELECT_MOMO);
                        g_Player.nBomb--;
                    }
                }

                //殺意の波動に目覚めているなら
                else
                {
                    //爆弾と重なっていないなら、2秒で爆発するボムを置ける
                    if (HitBomb(&g_Player.pos, &g_Player.posOld,
                        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BOMB + 1) == false)
                    {
                        PlaySound(SOUND_LABEL_SE_SET_BOMB);   //ボム設置音
                        SetBomb(g_Player.posGrid, g_Player.nFire, TIME_BOMB - 60, SELECT_MOMO);
                        g_Player.nBomb--;
                    }
                }
            }
        }

        //クラッシュ判定
        if (CrushingBlock(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BLOCK + 1) == true)
        {
            HitPlayer(BOMB_DAMAGE*4);
        }

        //ブロックの当たり判定
        CollisionBlock(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID));

        //キックできない
        if (g_Player.nCanKick <= 0)
        {
            //爆弾の当たり判定
            CollisionBomb(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID));
        }

        //キックできる
        else
        {
            //爆弾のキック判定
            KickBomb(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - (GRID_SIZE/2.0f + g_Player.fSpeed)
                    , PLAYER_HEIGHT * 2, GRID_SIZE -(GRID_SIZE / 2.0f + g_Player.fSpeed)), SELECT_MOMO);

            //爆弾の当たり判定
            CollisionBomb(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID));
        }

        //炎の当たり判定
        if (HitFire(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed, PLAYER_HEIGHT * 2,
                GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed)) == true)
        {
            HitPlayer(BOMB_DAMAGE);
        }

        //ストーリーなら
        if (pSetting->nMaxPlayer == 1)
        {
            //エネミーとの当たり判定
            if (CollisionEnemy(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed
                    , PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed), true) == true)
            {
                HitPlayer(HIT_ENEMY_DAMAGE);
            }

            //ボスとの当たり判定
            if (CollisionBoss(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed
                    , PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed), true) == true)
            {
                HitPlayer(HIT_ENEMY_DAMAGE);
            }

            //バレットとの当たり判定
            CollisionBullet(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed
                    , PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed));

            //ハートとの当たり判定
            if (HitItem(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_004) == true)
            {
                g_Player.fLife += PLAYER_STORY_LIFE / 2;    //半分回復
            }

            //タイマーとの当たり判定
            if (HitItem(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_005) == true)
            {
                g_nTimeMinute += 1;     //1分追加
            }

            //ツチノコ出現
            if (g_Player.pos.x >= 2300.0f && g_bSetSnake == false)
            {
                g_bSetSnake = true;
                SetEnemy(D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z / 2 + GRID_SIZE / 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_SNAKE);  //プレイヤーの初期位置に出現
            }

            //柵出現（ボス戦移行）
            if (g_Player.pos.x >= 4550.0f && g_bSetFenceBlock == false)
            {
                g_Player.pos.x = 4600.0f;           //柵にめり込むのを回避する
                StopSound(SOUND_LABEL_BGM_GAME);    //ゲーム音を止める
                g_bSetFenceBlock = true;
                SetBlock(D3DXVECTOR3(4500.0f, -GRID_SIZE, 600.0f), BLOCKTYPE_007);  //柵ブロック配置
                pCamera->state = CAMERASTATE_REACH_BOSS;    //ボス戦カットイン
            }

            //スタンしているなら
            if (g_Player.bStun == true)
            {
                //スタンのエフェクト
                if (g_nCntStunPlayer == PLAYER_STUN_TIME)
                {
                    SetEffect(D3DXVECTOR3(g_Player.pos.x, PLAYER_HEIGHT * 3, g_Player.pos.z)
                        , D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECT_SIZE_STUN, EFFECTTYPE_PLAYER_STUN); //プレイヤースタンエフェクト
                }

                g_MotionState = MOTIONSTATE_PLAYER_REGRET;    //悔しがり

                g_nCntStunPlayer--; //スタン時間を数える

                //スタン解除
                if (g_nCntStunPlayer <= 0)
                {
                    g_nCntStunPlayer = PLAYER_STUN_TIME;
                    g_Player.bStun = false;
                }
            }
        }

        //==============================================================================================================================================================
        //アイテムの当たり判定
        //炎
        if (HitItem(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true ,ITEMTYPE_000) == true)
        {
            g_Player.nFire++;
        }

        //ボム
        if (HitItem(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_001) == true)
        {
            g_Player.nBomb++;
            g_Player.nMaxBomb++;
        }

        //スピード
        if (HitItem(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_002) == true)
        {
            g_Player.nSpeedRank++;
        }

        //キック
        if (HitItem(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_003) == true)
        {
            g_Player.nCanKick++;
        }
        //==============================================================================================================================================================

        //影を設定
        SetPositionShadow(g_Player.nShadow, D3DXVECTOR3(g_Player.pos.x, 0, g_Player.pos.z), g_Player.pos.y);

        //死亡判定
        if (g_Player.fLife <= 0.0f)
        {
            //影の削除
            SHADOW *pShadow = GetShadow();//影の情報を取得
            pShadow[g_Player.nShadow].use = false;

            g_Player.bDisp = false;    //自機を未使用に

            PlaySound(SOUND_LABEL_SE_MAN_VOICE);   //やられた時の声

            //プレイヤー爆発エフェクト
            float fSpeedX = 0.0f;   //速さX
            float fSpeedY = 0.0f;   //速さY
            float fSpeedZ = 0.0f;   //速さZ
            int nPlusMinusX = 0;    //符号X
            int nPlusMinusZ = 0;    //符号Z
            for (int nCnt = 0; nCnt < COUNT_EFFECT_EXPLODE_PLAYER; nCnt++)
            {
                fSpeedX = float(rand() % 500) / 100.0f;
                fSpeedY = float(rand() % 800 + 300) / 100.0f;
                fSpeedZ = float(rand() % 500) / 100.0f;
                nPlusMinusX = int(rand() % 2 + 1);
                nPlusMinusZ = int(rand() % 2 + 1);
                if (nPlusMinusX == 1)
                {
                    nPlusMinusX = 1;
                }
                else
                {
                    nPlusMinusX = -1;
                }

                if (nPlusMinusZ == 1)
                {
                    nPlusMinusZ = 1;
                }
                else
                {
                    nPlusMinusZ = -1;
                }

                D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX*nPlusMinusX, fSpeedY, fSpeedZ*nPlusMinusZ);
                SetEffect(g_Player.pos, move, EFFECT_SIZE_EXPLODE_PLAYER, EFFECTTYPE_EXPLOSION); //爆発エフェクト
            }

            //バトルロイヤルなら
            if (pSetting->nMaxPlayer >= 2)
            {
                //乱数でアイテムを散りばめる用の変数
                float posX = 0.0f;
                float posZ = 0.0f;
                int nCntFire = 0; //炎
                int nCntBomb = 0; //ボム
                int nCntSpeed = 0;//スピード
                int nCntKick = 0; //キック
                D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                int nCntLoop = 0;   //無限ループ回避

                //炎を獲得しているなら
                if (g_Player.nFire > PLAYER_FIRST_FIRE)
                {
                    //ランダムに、炎アイテムを設置
                    while (nCntFire != g_Player.nFire - PLAYER_FIRST_FIRE)//設置しきるまでループ
                    {
                        //ランダムにX、Z座標を決める
                        posX = float(rand() % 1200 + 100);
                        posZ = float(rand() % 1000 + 100);
                        pos = D3DXVECTOR3(posX, 0.0f, posZ);

                        //グリッド座標に変換する
                        pos = ConversionGrid(&pos);

                        //そのグリッドに配置物がないなら配置
                        if (ConfirmationGrid(pos) == false)
                        {
                            //炎アイテムを配置
                            SetItem(D3DXVECTOR3(pos.x, GRID_SIZE / 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_000);
                            nCntFire++;
                        }

                        //ループ回数加算
                        nCntLoop++;

                        //無限ループから抜ける
                        if (nCntLoop == AVOID_INFINITE_LOOP)
                        {
                            nCntFire = g_Player.nFire - PLAYER_FIRST_FIRE;
                        }
                    }
                }

                //ループ回数リセット
                nCntLoop = 0;

                //ボムを獲得しているなら
                if (g_Player.nMaxBomb > PLAYER_FIRST_BOMB)
                {
                    //ランダムに、ボムアイテムを設置
                    while (nCntBomb != g_Player.nMaxBomb - PLAYER_FIRST_BOMB)//設置しきるまでループ
                    {
                        //ランダムにX、Z座標を決める
                        posX = float(rand() % 1200 + 100);
                        posZ = float(rand() % 1000 + 100);
                        pos = D3DXVECTOR3(posX, 0.0f, posZ);

                        //グリッド座標に変換する
                        pos = ConversionGrid(&pos);

                        //そのグリッドに配置物がないなら配置
                        if (ConfirmationGrid(pos) == false)
                        {
                            //ボムアイテムを配置
                            SetItem(D3DXVECTOR3(pos.x, GRID_SIZE / 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_001);
                            nCntBomb++;
                        }

                        //ループ回数加算
                        nCntLoop++;

                        //無限ループから抜ける
                        if (nCntLoop == AVOID_INFINITE_LOOP)
                        {
                            nCntBomb = g_Player.nMaxBomb - PLAYER_FIRST_BOMB;
                        }
                    }
                }

                //ループ回数リセット
                nCntLoop = 0;

                //スピードを獲得しているなら
                if (g_Player.nSpeedRank > PLAYER_FIRST_SPEEDRANK)
                {
                    //ランダムに、スピードアイテムを設置
                    while (nCntSpeed != g_Player.nSpeedRank - PLAYER_FIRST_SPEEDRANK)//設置しきるまでループ
                    {
                        //ランダムにX、Z座標を決める
                        posX = float(rand() % 1200 + 100);
                        posZ = float(rand() % 1000 + 100);
                        pos = D3DXVECTOR3(posX, 0.0f, posZ);

                        //グリッド座標に変換する
                        pos = ConversionGrid(&pos);

                        //そのグリッドに配置物がないなら配置
                        if (ConfirmationGrid(pos) == false)
                        {
                            //スピードアイテムを配置
                            SetItem(D3DXVECTOR3(pos.x, GRID_SIZE / 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_002);
                            nCntSpeed++;
                        }

                        //ループ回数加算
                        nCntLoop++;

                        //無限ループから抜ける
                        if (nCntLoop == AVOID_INFINITE_LOOP)
                        {
                            nCntSpeed = g_Player.nSpeedRank - PLAYER_FIRST_SPEEDRANK;
                        }
                    }
                }

                //ループ回数リセット
                nCntLoop = 0;

                //キックを獲得しているなら
                if (g_Player.nCanKick > PLAYER_FIRST_KICK)
                {
                    //ランダムに、キックアイテムを設置
                    while (nCntKick != g_Player.nCanKick - PLAYER_FIRST_KICK)//設置しきるまでループ
                    {
                        //ランダムにX、Z座標を決める
                        posX = float(rand() % 1200 + 100);
                        posZ = float(rand() % 1000 + 100);
                        pos = D3DXVECTOR3(posX, 0.0f, posZ);

                        //グリッド座標に変換する
                        pos = ConversionGrid(&pos);

                        //そのグリッドに配置物がないなら配置
                        if (ConfirmationGrid(pos) == false)
                        {
                            //キックアイテムを配置
                            SetItem(D3DXVECTOR3(pos.x, GRID_SIZE / 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_003);
                            nCntKick++;
                        }

                        //ループ回数加算
                        nCntLoop++;

                        //無限ループから抜ける
                        if (nCntLoop == AVOID_INFINITE_LOOP)
                        {
                            nCntKick = g_Player.nCanKick - PLAYER_FIRST_KICK;
                        }
                    }
                }

                //ループ回数リセット
                nCntLoop = 0;

                //位置などを戻す(ストーリーでは、カメラが追尾してしまうため戻さない)
                g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }

            //総数を減らす
            pGame->nCounter--;

            //ストーリーなら
            if (pSetting->nMaxPlayer == 1)
            {
                PlaySound(SOUND_LABEL_SE_FINISH);       //フィニッシュの効果音
                SetGameState(GAMESTATE_FINSIH_GAME);    //ゲームオーバー
            }
        }
    }

    //殺意の波動を設定
    if (g_bEvilMomo == true)
    {
        //生存しているなら常に
        if (g_Player.bDisp == true)
        {
            float fSpeedY = float(rand() % 800 + 300) / 100.0f;
            //0～3.14を出す*(0 or 1)*-1で正負を決める
            float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            float fRadius = float(rand() % 5000 + 0) / 100.0f;
            D3DXVECTOR3 pos = D3DXVECTOR3(g_Player.pos.x - sinf(fAngle)*fRadius, GRID_SIZE / 2, g_Player.pos.z - cosf(fAngle)*fRadius);
            D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
            SetEffect(pos, move, EFFECT_SIZE_EVIL, EFFECTTYPE_EVIL); //殺意の波動
        }
    }

    //バトル終了後
    if (pGame->state == GAMESTATE_FINSIH_ROUND || pGame->state == GAMESTATE_FINSIH_GAME)
    {
        g_Player.rot.y = 0.0f;  //正面を向く

        //引き分け以外で
        if (pGame->nCounter > 0)
        {
            g_MotionState = MOTIONSTATE_PLAYER_PLEASURE;    //喜び
        }

        //引き分け
        else
        {
            g_MotionState = MOTIONSTATE_PLAYER_REGRET;    //悔しがり
        }
    }
    
    //モデルのアニメーション
    UpdatePlayerMotion();
}

//===================================================================================================
// プレイヤー描画処理
// Author:後藤慎之助
//===================================================================================================
void DrawPlayer(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスへのポインタ
    D3DXMATRIX mtxRot, mtxTrans, mtxParent;
    D3DMATERIAL9 matDef;		// 現在のマテリアル保持用
    D3DXMATERIAL* pMat;			// マテリアルデータへのポインタ

    //プレイヤーが存在していて、無敵時間でないなら描画
    if (g_Player.bDisp == true && g_Player.bTransparence == false)
    {
        // ワールドマトリックスの初期化
        D3DXMatrixIdentity(&g_Player.mtxWorld);

        // 向きを反映
        D3DXMatrixRotationYawPitchRoll(&mtxRot,
            g_Player.rot.y,
            g_Player.rot.x,
            g_Player.rot.z);
        D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

        // 位置を反映
        D3DXMatrixTranslation(&mtxTrans,
            g_Player.pos.x,
            g_Player.pos.y,
            g_Player.pos.z);
        D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

        // パーツの設定
        for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
        {
            // ワールドマトリックスの初期化
            D3DXMatrixIdentity(&g_Player.modelParts[nCntParts].mtxWorld);

            // 向きを反映
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_aPlayer[nCntParts].rot.y,
                g_aPlayer[nCntParts].rot.x,
                g_aPlayer[nCntParts].rot.z);

            D3DXMatrixMultiply(
                &g_Player.modelParts[nCntParts].mtxWorld,
                &g_Player.modelParts[nCntParts].mtxWorld, &mtxRot);

            // 位置を反映
            D3DXMatrixTranslation(&mtxTrans,
                g_aPlayer[nCntParts].pos.x,
                g_aPlayer[nCntParts].pos.y,
                g_aPlayer[nCntParts].pos.z);
            D3DXMatrixMultiply(
                &g_Player.modelParts[nCntParts].mtxWorld,
                &g_Player.modelParts[nCntParts].mtxWorld, &mtxTrans);

            // テクスチャの設定
            pDevice->SetTexture(0, NULL);

            // 現在のマテリアルを取得する
            pDevice->GetMaterial(&matDef);

            // マテリアルデータへのポインタを取得
            pMat = (D3DXMATERIAL*)g_Player.modelParts[nCntParts].pBuffMat->GetBufferPointer();

            // 親モデルのインデックスが - 1の時
            if (g_Player.modelParts[nCntParts].nldxModelParent == -1)
            {
                mtxParent = g_Player.mtxWorld;		// プレイヤーのマトリクス
            }
            else
            {
                mtxParent = g_Player.modelParts[g_Player.modelParts[nCntParts].nldxModelParent].mtxWorld;
            }
            // 親モデルのマトリックスの判定
            D3DXMatrixMultiply(
                &g_Player.modelParts[nCntParts].mtxWorld,
                &g_Player.modelParts[nCntParts].mtxWorld, &mtxParent);

            // ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_Player.modelParts[nCntParts].mtxWorld);

            for (int nCntMat = 0; nCntMat < (int)g_Player.modelParts[nCntParts].nNumMat; nCntMat++)
            {
                // マテリアルの設定
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                g_Player.modelParts[nCntParts].pMesh->DrawSubset(nCntMat);
            }

            // 保持していたマテリアルを戻す
            pDevice->SetMaterial(&matDef);
        }
    }
}

//===========================================================================================================================
// プレイヤーの取得
// Author:後藤慎之助
//===========================================================================================================================
PLAYER*GetPlayer(void)
{
    return &g_Player;
}

//===========================================================================================================================
// プレイヤーのダメージ設定
// Author:後藤慎之助
//===========================================================================================================================
void HitPlayer(float fDamage)
{
    //無敵時間でないなら
    if (g_Player.nCntInvincibleFrame == 0)
    {
        GAME *pGame = GetGame();

        //ゲームが終わった後、ダメージを受けない
        if (pGame->state == GAMESTATE_NORMAL)
        {
            //ダメージ計算
            g_Player.fLife -= fDamage;

            //無敵時間をつくる
            g_Player.nCntInvincibleFrame = PLAYER_INVINCIBLE_FRAME;

            //HPがあるなら
            if (g_Player.fLife > 0.0f)
            {
                PlaySound(SOUND_LABEL_SE_TAKE_DAMAGE_VOICE);    //ダメージを受けた時の声
            }
        }
    }

}

//=====================================================
// プレイヤーに対する衝突判定の設定
// Author:後藤慎之助
//=====================================================
bool CollisionPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    //衝突したかどうか
    bool bCollision = false;

    // プレイヤーが使われているなら
    if (g_Player.bDisp == true)
    {
        //衝突判定を計算
        if (BoxCollision(pPos, pPosOld, size, &g_Player.pos,
            D3DXVECTOR3(GRID_SIZE - (GRID_SIZE / 2.0f + g_Player.fSpeed), PLAYER_HEIGHT * 2, GRID_SIZE - (GRID_SIZE / 2.0f + g_Player.fSpeed))) == true)
        {
            bCollision = true;
        }
    }

    return bCollision;
}

//===========================================================================================================================
// アニメーションロード
// Author:小西 優斗
//===========================================================================================================================
void LoadPlayerAnimation(void)
{
    // ファイルポイント
    FILE *pFile;

    // 変数宣言
    int  nCntLoad = 0;
    int	 nCntKey = 0;
    int	 nCntMotion = 0;

    char cReedText[1024];	// 文字として読み取り用
    char cHeadText[1024];	//
    char cDie[1024];

    // ファイル開
    pFile = fopen("TXT/motion.txt", "r");

    // 開けた
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "SCRIPT") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);
        }

        if (strcmp(cHeadText, "SCRIPT") == 0)
        {
            while (strcmp(cHeadText, "END_SCRIPT") != 0)
            {

                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "MOTIONSET") == 0)
                {

                    while (strcmp(cHeadText, "END_MOTIONSET") != 0)
                    {
                        fgets(cReedText, sizeof(cReedText), pFile);
                        sscanf(cReedText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "LOOP") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayerMotion[nCntMotion].nLoop);
                        }

                        if (strcmp(cHeadText, "NUM_KEY") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayerMotion[nCntMotion].nNum_Key);
                        }

                        //			if (strcmp(cHeadText, "COLLISION") == 0)
                        //			{
                        //				sscanf(cReedText, "%s %s %d %f %f %f %f %d %d", &cDie, &cDie, &g_PlayerCollision.nPartsInfo, &g_PlayerCollision.CollisionPos.x, &g_PlayerCollision.CollisionPos.y, &g_PlayerCollision.CollisionPos.z, &g_PlayerCollision.fLength, &g_PlayerCollision.nFirstFlame[nCntMotion], &g_PlayerCollision.nLastFlame);
                        //			}
                        if (strcmp(cHeadText, "KEYSET") == 0)
                        {

                            while (strcmp(cHeadText, "END_KEYSET") != 0)
                            {

                                fgets(cReedText, sizeof(cReedText), pFile);
                                sscanf(cReedText, "%s", &cHeadText);

                                if (strcmp(cHeadText, "FRAME") == 0)
                                {
                                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayerMotion[nCntMotion].KeyInfo[nCntKey].nFrame);
                                }
                                if (strcmp(cHeadText, "KEY") == 0)
                                {
                                    while (strcmp(cHeadText, "END_KEY") != 0)
                                    {
                                        fgets(cReedText, sizeof(cReedText), pFile);
                                        sscanf(cReedText, "%s", &cHeadText);

                                        if (strcmp(cHeadText, "POS") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &g_aPlayerMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].x,
                                                &g_aPlayerMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].y,
                                                &g_aPlayerMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].z);
                                        }

                                        if (strcmp(cHeadText, "ROT") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &g_aPlayerMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].x,
                                                &g_aPlayerMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].y,
                                                &g_aPlayerMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].z);
                                        }
                                    }
                                    nCntLoad++;
                                }
                            }
                            nCntLoad = 0;
                            nCntKey++;
                        }
                    }
                    nCntKey = 0;
                    nCntMotion++;
                }
            }
            // ファイル閉
            fclose(pFile);
        }
        // 開けない
        else
        {
            printf("開けれませんでした\n");
        }
    }
}

//===========================================================================================================================
// モデルロード
// Author:小西 優斗
//===========================================================================================================================
void LoadPlayer(void)
{
    // ファイルポイント
    FILE *pFile;

    // 変数宣言
    int  nCntLoad = 0;
    char cReedText[128];	// 文字として読み取り用
    char cHeadText[256];	//
    char cDie[128];

    // ファイル開
    pFile = fopen("TXT/player.txt", "r");

    // 開けた
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "CHARACTERSET") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);
        }

        if (strcmp(cHeadText, "CHARACTERSET") == 0)
        {
            while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
            {

                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "\n") == 0)
                {
                }
                else if (strcmp(cHeadText, "PARTSSET") == 0)
                {
                    while (strcmp(cHeadText, "END_PARTSSET") != 0)
                    {

                        fgets(cReedText, sizeof(cReedText), pFile);
                        sscanf(cReedText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "INDEX") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer[nCntLoad].nData);
                        }

                        if (strcmp(cHeadText, "PARENT") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer[nCntLoad].nParents);
                        }

                        if (strcmp(cHeadText, "POS") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                &g_aPlayer[nCntLoad].posOrigin.x,
                                &g_aPlayer[nCntLoad].posOrigin.y,
                                &g_aPlayer[nCntLoad].posOrigin.z);

                            g_aPlayer[nCntLoad].pos = g_aPlayer[nCntLoad].posOrigin;
                        }

                        if (strcmp(cHeadText, "ROT") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                &g_aPlayer[nCntLoad].rot.x,
                                &g_aPlayer[nCntLoad].rot.y,
                                &g_aPlayer[nCntLoad].rot.z);
                        }
                    }
                    nCntLoad++;
                }
            }
        }
        // ファイル閉
        fclose(pFile);
    }

    // 開けない
    else
    {
        printf("開けれませんでした\n");
    }
}

//===========================================================================================================================
// モーションの更新
// Author:小西 優斗
//===========================================================================================================================
void UpdatePlayerMotion(void)
{
    //モーションが変わったとき
    if (g_MotionOldState != g_MotionState) {
        g_nFremePlayer = 0;
        g_nCntKeyPlayer = 0;
    }
    for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
    {
        if (g_nFremePlayer == 0)
        {
            g_aNumPlayerPos[nCntMotion].x = ((g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].pos[nCntMotion].x -
                (g_aPlayer[nCntMotion].pos.x - g_aPlayer[nCntMotion].posOrigin.x)) / float(g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].nFrame));

            g_aNumPlayerPos[nCntMotion].y = ((g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].pos[nCntMotion].y -
                (g_aPlayer[nCntMotion].pos.y - g_aPlayer[nCntMotion].posOrigin.y)) / float(g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].nFrame));

            g_aNumPlayerPos[nCntMotion].z = ((g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].pos[nCntMotion].z -
                (g_aPlayer[nCntMotion].pos.z - g_aPlayer[nCntMotion].posOrigin.z)) / float(g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].nFrame));

            g_aNumPlayerRotDest[nCntMotion] =
                g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].rot[nCntMotion] - g_aPlayer[nCntMotion].rot;

            g_aNumPlayerRot[nCntMotion] = (g_aNumPlayerRotDest[nCntMotion] / float(g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].nFrame));
        }

        g_aPlayer[nCntMotion].pos += g_aNumPlayerPos[nCntMotion];
        g_aPlayer[nCntMotion].rot += g_aNumPlayerRot[nCntMotion];
    }
    g_MotionOldState = g_MotionState;
    // ループするとき
    if (g_nFremePlayer >= g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].nFrame && g_aPlayerMotion[g_MotionState].nLoop == 1)
    {
        g_nFremePlayer = 0;
        g_nCntKeyPlayer++;

        if (g_nCntKeyPlayer >= g_aPlayerMotion[g_MotionState].nNum_Key)
        {
            g_nCntKeyPlayer = 0;
        }

    }
    // ループしないとき
    else if (g_nFremePlayer >= g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].nFrame && g_aPlayerMotion[g_MotionState].nLoop == 0)
    {
        g_nFremePlayer = 0;
        g_nCntKeyPlayer++;
        if (g_nCntKeyPlayer >= g_aPlayerMotion[g_MotionState].nNum_Key)
        {
            for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
            {
                g_aNumPlayerPos[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aNumPlayerRot[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
            g_MotionState = MOTIONSTATE_PLAYER_IDLE;
            g_nCntKeyPlayer = 0;
        }
    }
    else
    {
        g_nFremePlayer++;
    }
}

//===========================================================================================================================
// 使用判定の初期化
// Author:後藤慎之助
//===========================================================================================================================
void InitDispPlayer(void)
{
    g_Player.bDisp = false;
}