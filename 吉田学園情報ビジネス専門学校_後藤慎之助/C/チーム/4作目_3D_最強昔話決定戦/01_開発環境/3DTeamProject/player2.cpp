//=============================================================================
//
// 最強昔話決定戦の処理[player2.cpp]（自機2）
// Author:後藤慎之助
//
//===================================================================================================

//===================================================================================================
// インクルードファイル
//===================================================================================================
#include "player2.h"
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
#include "manual.h"

//===================================================================================================
// グローバル変数定義
//===================================================================================================
PLAYER2 g_Player2;        //プレイヤーの情報

int g_nFrameCountPlayer2;				// 現在のフレーム数

int g_nPlayKeySetlndexPlayer2;			// 再生中のキーセットインデックス

int g_nPlayer2CurrentAnimationPlayer2;	// 再生中のアニメーション

int g_nFremePlayer2;					// フレーム数

int g_nCntKeyPlayer2;					// キー数

int g_nChangeFlamePlayer2;			// モーションのフレーム

MOTIONSTATE_PLAYER2 g_MotionState;			// モーションの構造体
MOTIONSTATE_PLAYER2 g_MotionOldState;		// 1F前のモーションの構造体

D3DXVECTOR3 g_aNumPlayer2Pos[MAX_PARTS];		// posの差分
D3DXVECTOR3 g_aNumPlayer2Rot[MAX_PARTS];		// rotの差分
D3DXVECTOR3 g_aNumPlayer2RotDest[MAX_PARTS];	// rotの差分

ANIMATION_PLAYER2 g_aPlayer2Motion[MOTIONSTATE_PLAYER2_MAX];	// モーションの情報
                                        // モデルパーツのデータ
ModelParts_Player2 g_aPlayer2[MAX_PARTS] =
{
    { "MODEL/player2/00_body.x" },
    { "MODEL/player2/01_head.x" },
    { "MODEL/player2/02_armR.x" },
    { "MODEL/player2/03_handR.x" },
    { "MODEL/player2/04_armL.x" },
    { "MODEL/player2/05_handL.x" },
    { "MODEL/player2/06_legR.x" },
    { "MODEL/player2/07_footR.x" },
    { "MODEL/player2/08_legL.x" },
    { "MODEL/player2/09_footL.x" },
};

bool g_bEvilUra;   //殺意の波動に目覚めているかどうか

//===================================================================================================
// プレイヤー初期化処理
// Author:後藤慎之助
//===================================================================================================
void InitPlayer2(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //ゲーム状態の取得
    GAME *pGame = GetGame();

    //隠しコマンド使用判定を取得
    g_bEvilUra = GetHiddenCommand2();

    //グローバル変数の初期化
    g_MotionState = MOTIONSTATE_PLAYER2_IDLE;

    g_nFrameCountPlayer2 = 0;				// 現在のフレーム数

    g_nPlayKeySetlndexPlayer2 = 0;			// 再生中のキーセットインデックス

    g_nPlayer2CurrentAnimationPlayer2 = MOTIONSTATE_PLAYER2_IDLE;	// 再生中のアニメーション

    g_nFremePlayer2 = 0;					// フレーム数

    g_nCntKeyPlayer2 = 1;					// キー数

    g_nChangeFlamePlayer2 = 20;			// モーションのフレーム

    //構造体の初期化
    g_Player2.pos = pGame->startPos2;		            // 初期位置
    g_Player2.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期位置
    g_Player2.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 初期向き
    g_Player2.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期移動量
    g_Player2.posGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期位置
    g_Player2.bDisp = true;
    //隠しコマンド使用
    if (g_bEvilUra == true)
    {
        g_Player2.fLife = BOMB_DAMAGE + PLAYER_LIFE;	    //殺意の波動は爆風を一回耐える
        g_Player2.nSpeedRank = PLAYER_FIRST_SPEEDRANK + 2;   //殺意の波動は2ランク速い
        g_Player2.nBomb = PLAYER_FIRST_BOMB + 1;             //殺意の波動はボムを2つおける
        g_Player2.nCanKick = PLAYER_FIRST_KICK + 1;          //殺意の波動はキックできる
    }

    //隠しコマンド未使用
    else
    {
        g_Player2.fLife = PLAYER_LIFE;
        g_Player2.nSpeedRank = PLAYER_FIRST_SPEEDRANK;
        g_Player2.nBomb = PLAYER_FIRST_BOMB;
        g_Player2.nCanKick = PLAYER_FIRST_KICK;
    }
    g_Player2.fSpeed = VALUE_MOVE;
    g_Player2.nFire = PLAYER_FIRST_FIRE;
    g_Player2.nShadow = SetShadow(g_Player2.pos, g_Player2.rot, GRID_SIZE, g_Player2.pos.y,
        D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_ALPHA_MAX));//影の設定
    g_Player2.nCntInvincibleFrame = 0;
    g_Player2.bTransparence = false;
    g_Player2.nMaxBomb = g_Player2.nBomb;

    LoadPlayer2();
    LoadPlayer2Animation();

    // モデルパーツの初期化
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        //		g_Player2.modelParts[nCntParts].pos = g_aPlayer2[nCntParts].pos;			// 初期位置
        //		g_Player2.modelParts[nCntParts].rot = g_aPlayer2[nCntParts].rot;			// 初期角度

        g_Player2.modelParts[nCntParts].nldxModelParent = 0;
    }
    // ファイルの読み込み
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        g_Player2.modelParts[nCntParts].nldxModelParent = g_aPlayer2[nCntParts].nParents;

        // Xファイルの読み込み
        D3DXLoadMeshFromX(g_aPlayer2[nCntParts].pFileName,
            D3DXMESH_SYSTEMMEM,
            pDevice,
            NULL,
            &g_Player2.modelParts[nCntParts].pBuffMat,
            NULL,
            &g_Player2.modelParts[nCntParts].nNumMat,
            &g_Player2.modelParts[nCntParts].pMesh);

    }
}

//===================================================================================================
// プレイヤー終了処理
// Author:後藤慎之助
//===================================================================================================
void UninitPlayer2(void)
{
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        // メッシュの破棄
        if (g_Player2.modelParts[nCntParts].pMesh != NULL)
        {
            g_Player2.modelParts[nCntParts].pMesh->Release();
            g_Player2.modelParts[nCntParts].pMesh = NULL;
        }
        // マテリアルの破棄
        if (g_Player2.modelParts[nCntParts].pBuffMat != NULL)
        {
            g_Player2.modelParts[nCntParts].pBuffMat->Release();
            g_Player2.modelParts[nCntParts].pBuffMat = NULL;
        }
    }
}

//===================================================================================================
// プレイヤー更新処理
// Author:後藤慎之助
//===================================================================================================
void UpdatePlayer2(void)
{
    //コントローラーの情報を取得する
    DIJOYSTATE2 Controller = GetController(PLAYER_2);
    //カメラの情報を取得する
    CAMERA*pCamera = GetCamera();
    //ゲームの状況を取得する
    GAME *pGame = GetGame();

    //移動ランクに応じた、速さを得る
    switch (g_Player2.nSpeedRank)
    {
    case PLAYER_FIRST_SPEEDRANK:
        g_Player2.fSpeed = VALUE_MOVE;
        break;

    case PLAYER_FIRST_SPEEDRANK + 1:
        g_Player2.fSpeed = VALUE_MOVE + VALUE_UPMOVE;
        break;

    case PLAYER_FIRST_SPEEDRANK + 2:
        g_Player2.fSpeed = VALUE_MOVE + VALUE_UPMOVE * 2.0f;
        break;

    case PLAYER_FIRST_SPEEDRANK + 3:
        g_Player2.fSpeed = VALUE_MOVE + VALUE_UPMOVE * 3.0f;
        break;

    case PLAYER_FIRST_SPEEDRANK + 4:
        g_Player2.fSpeed = VALUE_MOVE + VALUE_UPMOVE * 4.0f;
        break;
    }

    //無敵時間
    if (g_Player2.nCntInvincibleFrame > 0)
    {
        g_Player2.nCntInvincibleFrame--;                     //無敵時間を数える
        g_Player2.bTransparence = !g_Player2.bTransparence;   //無敵中は、点滅させる
    }
    else
    {
        g_Player2.bTransparence = false; //透明を解除
    }

    //生存しているなら
    if (g_Player2.bDisp == true && pGame->state == GAMESTATE_NORMAL && pCamera->state == CAMERASTATE_NORMAL)
    {
        // プレイヤーが動いていないとき
        D3DXVECTOR3 lengthCheckVec = g_Player2.move;
        lengthCheckVec.y = 0;
        if (D3DXVec3Length(&lengthCheckVec) <= 1.5f)
        {
            g_MotionState = MOTIONSTATE_PLAYER2_IDLE;    //モーションをアイドルにする
        }

        //1F前の位置を記憶
        g_Player2.posOld = g_Player2.pos;

        //グリッドの中心の値を得る
        g_Player2.posGrid = ConversionGrid(&g_Player2.pos);

        //スティックが傾いているなら移動
        if (Controller.lY != 0 || Controller.lX != 0)
        {
            float fAngle = atan2(Controller.lX, Controller.lY*-1);//スティックの角度を求める
            float fPlayer2Angle = atan2(Controller.lX*-1, Controller.lY);//スティックの角度を求める

            g_Player2.pos.x += sinf((pCamera->rot.y + fAngle))*g_Player2.fSpeed;
            g_Player2.pos.z += cosf((pCamera->rot.y + fAngle))*g_Player2.fSpeed;
            g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
                                        //キャラの向きを変える
            g_Player2.rot.y = pCamera->rot.y + fPlayer2Angle;
        }
        //十字キーでの移動
        else
        {
            //左
            if (Controller.rgdwPOV[0] == BUTTON_LEFT)
            {
                g_Player2.pos.x -= g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x + D3DXToRadian(90.0f);
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
            }

            //左上
            if (Controller.rgdwPOV[0] == BUTTON_LEFT_UP)
            {
                g_Player2.pos.x -= sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.pos.z += cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x + D3DXToRadian(135.0f);
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
            }

            //左下
            if (Controller.rgdwPOV[0] == BUTTON_DOWN_LEFT)
            {
                g_Player2.pos.x -= sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.pos.z -= cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x + D3DXToRadian(45.0f);
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
            }

            //右
            if (Controller.rgdwPOV[0] == BUTTON_RIGHT)
            {
                g_Player2.pos.x += g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x - D3DXToRadian(90.0f);
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
            }

            //右上
            if (Controller.rgdwPOV[0] == BUTTON_UP_RIGHT)
            {
                g_Player2.pos.x += sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.pos.z += cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x - D3DXToRadian(135.0f);
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
            }

            //右下
            if (Controller.rgdwPOV[0] == BUTTON_RIGHT_DOWN)
            {
                g_Player2.pos.x += sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.pos.z -= cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x - D3DXToRadian(45.0f);
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
            }

            //上
            if (Controller.rgdwPOV[0] == BUTTON_UP)
            {
                g_Player2.pos.z += g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x + D3DX_PI;
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
            }

            //下
            if (Controller.rgdwPOV[0] == BUTTON_DOWN)
            {
                g_Player2.pos.z -= g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x;
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
            }
        }

        //仮の操作===========================================================================================
        if (Controller.lY == 0 && Controller.lX == 0 && Controller.rgdwPOV[0] == BUTTON_NEUTRAL)
        {
            //WASDキーで移動
            //Aキーが押された(左移動)
            if (GetKeyboardPress(DIK_LEFT))
            {
                if (GetKeyboardPress(DIK_UP))//AとWが同時に押された
                {
                    g_Player2.pos.x -= sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.pos.z += cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x + D3DXToRadian(135.0f);
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
                }
                else if (GetKeyboardPress(DIK_DOWN))//AとSが同時に押された
                {
                    g_Player2.pos.x -= sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.pos.z -= cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x + D3DXToRadian(45.0f);
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
                }
                else if (GetKeyboardPress(DIK_RIGHT))
                {

                }
                else				//左だけ押した
                {
                    g_Player2.pos.x -= g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x + D3DXToRadian(90.0f);
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
                }

            }

            //Dキーが押された(右移動)
            else if (GetKeyboardPress(DIK_RIGHT))
            {
                if (GetKeyboardPress(DIK_UP))//DとWが同時に押された
                {
                    g_Player2.pos.x += sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.pos.z += cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x - D3DXToRadian(135.0f);
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする

                }
                else if (GetKeyboardPress(DIK_DOWN))//DとSが同時に押された
                {
                    g_Player2.pos.x += sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.pos.z -= cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x - D3DXToRadian(45.0f);
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
                }
                else if (GetKeyboardPress(DIK_LEFT))
                {

                }
                else				//右だけ押した
                {
                    g_Player2.pos.x += g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x - D3DXToRadian(90.0f);
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
                }

            }

            //Wキーが押された(上移動)
            else if (GetKeyboardPress(DIK_UP))
            {
                if (GetKeyboardPress(DIK_DOWN))
                {

                }
                else
                {
                    g_Player2.pos.z += g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x + D3DX_PI;
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
                }
            }

            //Sキーが押された(下移動)
            else if (GetKeyboardPress(DIK_DOWN))
            {
                if (GetKeyboardPress(DIK_UP))
                {

                }
                else
                {
                    g_Player2.pos.z -= g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x;
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//モーションを歩きにする
                }
            }
        }

        //爆弾を置く(仮)
        if (GetKeyboardTrigger(DIK_RETURN) && g_Player2.nBomb > 0 && g_Player2.nCntInvincibleFrame == 0)
        {
            //爆弾と重なっていないなら、置ける
            if (HitBomb(&g_Player2.pos, &g_Player2.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BOMB + 1) == false)
            {
                PlaySound(SOUND_LABEL_SE_SET_BOMB);   //ボム設置音
                SetBomb(g_Player2.posGrid, g_Player2.nFire, TIME_BOMB, SELECT_URA);
                g_Player2.nBomb--;
            }

        }
        //===============================================================================================

        //爆弾を置く
        if (GetJoypadTrigger(PLAYER_2, BUTTON_B) && g_Player2.nBomb > 0 && g_Player2.nCntInvincibleFrame == 0)
        {
            //殺意の波動に目覚めてないなら
            if (g_bEvilUra == false)
            {
                //爆弾と重なっていないなら、3秒で爆発するボムを置ける
                if (HitBomb(&g_Player2.pos, &g_Player2.posOld,
                    D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BOMB + 1) == false)
                {
                    PlaySound(SOUND_LABEL_SE_SET_BOMB);   //ボム設置音
                    SetBomb(g_Player2.posGrid, g_Player2.nFire, TIME_BOMB, SELECT_URA);
                    g_Player2.nBomb--;
                }
            }

            //殺意の波動に目覚めているなら
            else
            {
                //爆弾と重なっていないなら、2秒で爆発するボムを置ける
                if (HitBomb(&g_Player2.pos, &g_Player2.posOld,
                    D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BOMB + 1) == false)
                {
                    PlaySound(SOUND_LABEL_SE_SET_BOMB);   //ボム設置音
                    SetBomb(g_Player2.posGrid, g_Player2.nFire, TIME_BOMB - 60, SELECT_URA);
                    g_Player2.nBomb--;
                }
            }
        }

        //クラッシュ判定
        if (CrushingBlock(&g_Player2.pos, &g_Player2.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BLOCK + 1) == true)
        {
            HitPlayer2(BOMB_DAMAGE*4);
        }

        //ブロックの当たり判定
        CollisionBlock(&g_Player2.pos, &g_Player2.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID));

        //キックできない
        if (g_Player2.nCanKick <= 0)
        {
            //爆弾の当たり判定
            CollisionBomb(&g_Player2.pos, &g_Player2.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID));
        }

        //キックできる
        else
        {
            //爆弾のキック判定
            KickBomb(&g_Player2.pos, &g_Player2.posOld,
                D3DXVECTOR3(GRID_SIZE - (GRID_SIZE / 2.0f + g_Player2.fSpeed)
                    , PLAYER_HEIGHT * 2, GRID_SIZE - (GRID_SIZE / 2.0f + g_Player2.fSpeed)), SELECT_URA);

            //爆弾の当たり判定
            CollisionBomb(&g_Player2.pos, &g_Player2.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID));
        }

        //炎の当たり判定
        if (HitFire(&g_Player2.pos, &g_Player2.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID - g_Player2.fSpeed
                , PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID - g_Player2.fSpeed)) == true)
        {
            HitPlayer2(BOMB_DAMAGE);
        }

        //==============================================================================================================================================================
        //アイテムの当たり判定
        //炎
        if (HitItem(&g_Player2.pos, &g_Player2.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_000) == true)
        {
            g_Player2.nFire++;
        }

        //ボム
        if (HitItem(&g_Player2.pos, &g_Player2.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_001) == true)
        {
            g_Player2.nBomb++;
            g_Player2.nMaxBomb++;
        }

        //スピード
        if (HitItem(&g_Player2.pos, &g_Player2.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_002) == true)
        {
            g_Player2.nSpeedRank++;
        }

        //キック
        if (HitItem(&g_Player2.pos, &g_Player2.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_003) == true)
        {
            g_Player2.nCanKick++;
        }
        //==============================================================================================================================================================

        //影を設定
        SetPositionShadow(g_Player2.nShadow, D3DXVECTOR3(g_Player2.pos.x, 0, g_Player2.pos.z), g_Player2.pos.y);

        //死亡判定
        if (g_Player2.fLife <= 0.0f)
        {
            //影の削除
            SHADOW *pShadow = GetShadow();//影の情報を取得
            pShadow[g_Player2.nShadow].use = false;

            g_Player2.bDisp = false;    //自機を未使用に

            PlaySound(SOUND_LABEL_SE_MAN_VOICE2);   //やられた時の声

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
                SetEffect(g_Player2.pos, move, EFFECT_SIZE_EXPLODE_PLAYER, EFFECTTYPE_EXPLOSION); //爆発エフェクト
            }

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
            if (g_Player2.nFire > PLAYER_FIRST_FIRE)
            {
                //ランダムに、炎アイテムを設置
                while (nCntFire != g_Player2.nFire - PLAYER_FIRST_FIRE)//設置しきるまでループ
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
                        nCntFire = g_Player2.nFire - PLAYER_FIRST_FIRE;
                    }
                }
            }

            //ループ回数リセット
            nCntLoop = 0;

            //ボムを獲得しているなら
            if (g_Player2.nMaxBomb > PLAYER_FIRST_BOMB)
            {
                //ランダムに、ボムアイテムを設置
                while (nCntBomb != g_Player2.nMaxBomb - PLAYER_FIRST_BOMB)//設置しきるまでループ
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
                        nCntBomb = g_Player2.nMaxBomb - PLAYER_FIRST_BOMB;
                    }
                }
            }

            //ループ回数リセット
            nCntLoop = 0;

            //スピードを獲得しているなら
            if (g_Player2.nSpeedRank > PLAYER_FIRST_SPEEDRANK)
            {
                //ランダムに、スピードアイテムを設置
                while (nCntSpeed != g_Player2.nSpeedRank - PLAYER_FIRST_SPEEDRANK)//設置しきるまでループ
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
                        nCntSpeed = g_Player2.nSpeedRank - PLAYER_FIRST_SPEEDRANK;
                    }
                }
            }

            //ループ回数リセット
            nCntLoop = 0;

            //キックを獲得しているなら
            if (g_Player2.nCanKick > PLAYER_FIRST_KICK)
            {
                //ランダムに、キックアイテムを設置
                while (nCntKick != g_Player2.nCanKick - PLAYER_FIRST_KICK)//設置しきるまでループ
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
                        nCntKick = g_Player2.nCanKick - PLAYER_FIRST_KICK;
                    }
                }
            }

            //ループ回数リセット
            nCntLoop = 0;

            //位置を初期化
            g_Player2.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

            //総数を減らす
            pGame->nCounter--;
        }
    }

    //殺意の波動を設定
    if (g_bEvilUra == true)
    {
        //生存しているなら常に
        if (g_Player2.bDisp == true)
        {
            float fSpeedY = float(rand() % 800 + 300) / 100.0f;
            //0〜3.14を出す*(0 or 1)*-1で正負を決める
            float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            float fRadius = float(rand() % 5000 + 0) / 100.0f;
            D3DXVECTOR3 pos = D3DXVECTOR3(g_Player2.pos.x - sinf(fAngle)*fRadius, GRID_SIZE / 2, g_Player2.pos.z - cosf(fAngle)*fRadius);
            D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
            SetEffect(pos, move, EFFECT_SIZE_EVIL, EFFECTTYPE_EVIL); //殺意の波動
        }
    }

    //バトル終了後
    if (pGame->state == GAMESTATE_FINSIH_ROUND || pGame->state == GAMESTATE_FINSIH_GAME)
    {
        g_Player2.rot.y = 0.0f;  //正面を向く

        //引き分け以外で
        if (pGame->nCounter > 0)
        {
            g_MotionState = MOTIONSTATE_PLAYER2_PLEASURE;    //喜び
        }

        //引き分け
        else
        {
            g_MotionState = MOTIONSTATE_PLAYER2_REGRET;    //悔しがり
        }        
    }

    //モデルのアニメーション
    UpdatePlayer2Motion();
}

//===================================================================================================
// プレイヤー描画処理
// Author:後藤慎之助
//===================================================================================================
void DrawPlayer2(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスへのポインタ
    D3DXMATRIX mtxRot, mtxTrans, mtxParent;
    D3DMATERIAL9 matDef;		// 現在のマテリアル保持用
    D3DXMATERIAL* pMat;			// マテリアルデータへのポインタ

    //プレイヤーが存在しているなら描画
    if (g_Player2.bDisp == true && g_Player2.bTransparence == false)
    {
        // ワールドマトリックスの初期化
        D3DXMatrixIdentity(&g_Player2.mtxWorld);

        // 向きを反映
        D3DXMatrixRotationYawPitchRoll(&mtxRot,
            g_Player2.rot.y,
            g_Player2.rot.x,
            g_Player2.rot.z);
        D3DXMatrixMultiply(&g_Player2.mtxWorld, &g_Player2.mtxWorld, &mtxRot);

        // 位置を反映
        D3DXMatrixTranslation(&mtxTrans,
            g_Player2.pos.x,
            g_Player2.pos.y,
            g_Player2.pos.z);
        D3DXMatrixMultiply(&g_Player2.mtxWorld, &g_Player2.mtxWorld, &mtxTrans);

        // パーツの設定
        for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
        {
            // ワールドマトリックスの初期化
            D3DXMatrixIdentity(&g_Player2.modelParts[nCntParts].mtxWorld);

            // 向きを反映
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_aPlayer2[nCntParts].rot.y,
                g_aPlayer2[nCntParts].rot.x,
                g_aPlayer2[nCntParts].rot.z);

            D3DXMatrixMultiply(
                &g_Player2.modelParts[nCntParts].mtxWorld,
                &g_Player2.modelParts[nCntParts].mtxWorld, &mtxRot);

            // 位置を反映
            D3DXMatrixTranslation(&mtxTrans,
                g_aPlayer2[nCntParts].pos.x,
                g_aPlayer2[nCntParts].pos.y,
                g_aPlayer2[nCntParts].pos.z);
            D3DXMatrixMultiply(
                &g_Player2.modelParts[nCntParts].mtxWorld,
                &g_Player2.modelParts[nCntParts].mtxWorld, &mtxTrans);

            // テクスチャの設定
            pDevice->SetTexture(0, NULL);

            // 現在のマテリアルを取得する
            pDevice->GetMaterial(&matDef);

            // マテリアルデータへのポインタを取得
            pMat = (D3DXMATERIAL*)g_Player2.modelParts[nCntParts].pBuffMat->GetBufferPointer();

            // 親モデルのインデックスが - 1の時
            if (g_Player2.modelParts[nCntParts].nldxModelParent == -1)
            {
                mtxParent = g_Player2.mtxWorld;		// プレイヤーのマトリクス
            }
            else
            {
                mtxParent = g_Player2.modelParts[g_Player2.modelParts[nCntParts].nldxModelParent].mtxWorld;
            }
            // 親モデルのマトリックスの判定
            D3DXMatrixMultiply(
                &g_Player2.modelParts[nCntParts].mtxWorld,
                &g_Player2.modelParts[nCntParts].mtxWorld, &mtxParent);

            // ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_Player2.modelParts[nCntParts].mtxWorld);

            for (int nCntMat = 0; nCntMat < (int)g_Player2.modelParts[nCntParts].nNumMat; nCntMat++)
            {
                // マテリアルの設定
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                g_Player2.modelParts[nCntParts].pMesh->DrawSubset(nCntMat);
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
PLAYER2*GetPlayer2(void)
{
    return &g_Player2;
}

//===========================================================================================================================
// プレイヤーのダメージ設定
// Author:後藤慎之助
//===========================================================================================================================
void HitPlayer2(float fDamage)
{
    //無敵時間でないなら
    if (g_Player2.nCntInvincibleFrame == 0)
    {
        GAME *pGame = GetGame();

        //ゲームが終わった後、ダメージを受けない
        if (pGame->state == GAMESTATE_NORMAL)
        {
            //ダメージ計算
            g_Player2.fLife -= fDamage;

            //無敵時間をつくる
            g_Player2.nCntInvincibleFrame = PLAYER_INVINCIBLE_FRAME;

            //HPがあるなら
            if (g_Player2.fLife > 0.0f)
            {
                PlaySound(SOUND_LABEL_SE_TAKE_DAMAGE_VOICE2);    //ダメージを受けた時の声
            }
        }
    }
}

//=====================================================
// プレイヤーに対する衝突判定の設定
// Author:後藤慎之助
//=====================================================
bool CollisionPlayer2(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    //衝突したかどうか
    bool bCollision = false;

    // プレイヤーが使われているなら
    if (g_Player2.bDisp == true)
    {
        //衝突判定を計算
        if (BoxCollision(pPos, pPosOld, size, &g_Player2.pos,
            D3DXVECTOR3(GRID_SIZE - (GRID_SIZE / 2.0f + g_Player2.fSpeed), PLAYER_HEIGHT * 2, GRID_SIZE - (GRID_SIZE / 2.0f + g_Player2.fSpeed))) == true)
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
void LoadPlayer2Animation(void)
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
    pFile = fopen("TXT/motion2.txt", "r");

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
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer2Motion[nCntMotion].nLoop);
                        }

                        if (strcmp(cHeadText, "NUM_KEY") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer2Motion[nCntMotion].nNum_Key);
                        }

                        //			if (strcmp(cHeadText, "COLLISION") == 0)
                        //			{
                        //				sscanf(cReedText, "%s %s %d %f %f %f %f %d %d", &cDie, &cDie, &g_Player2Collision.nPartsInfo, &g_Player2Collision.CollisionPos.x, &g_Player2Collision.CollisionPos.y, &g_Player2Collision.CollisionPos.z, &g_Player2Collision.fLength, &g_Player2Collision.nFirstFlame[nCntMotion], &g_Player2Collision.nLastFlame);
                        //			}
                        if (strcmp(cHeadText, "KEYSET") == 0)
                        {

                            while (strcmp(cHeadText, "END_KEYSET") != 0)
                            {

                                fgets(cReedText, sizeof(cReedText), pFile);
                                sscanf(cReedText, "%s", &cHeadText);

                                if (strcmp(cHeadText, "FRAME") == 0)
                                {
                                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer2Motion[nCntMotion].KeyInfo[nCntKey].nFrame);
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
                                                &g_aPlayer2Motion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].x,
                                                &g_aPlayer2Motion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].y,
                                                &g_aPlayer2Motion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].z);
                                        }

                                        if (strcmp(cHeadText, "ROT") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &g_aPlayer2Motion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].x,
                                                &g_aPlayer2Motion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].y,
                                                &g_aPlayer2Motion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].z);
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
void LoadPlayer2(void)
{
    // ファイルポイント
    FILE *pFile;

    // 変数宣言
    int  nCntLoad = 0;
    char cReedText[128];	// 文字として読み取り用
    char cHeadText[256];	//
    char cDie[128];

    // ファイル開
    pFile = fopen("TXT/player2.txt", "r");

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
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer2[nCntLoad].nData);
                        }

                        if (strcmp(cHeadText, "PARENT") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer2[nCntLoad].nParents);
                        }

                        if (strcmp(cHeadText, "POS") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                &g_aPlayer2[nCntLoad].posOrigin.x,
                                &g_aPlayer2[nCntLoad].posOrigin.y,
                                &g_aPlayer2[nCntLoad].posOrigin.z);

                            g_aPlayer2[nCntLoad].pos = g_aPlayer2[nCntLoad].posOrigin;
                        }

                        if (strcmp(cHeadText, "ROT") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                &g_aPlayer2[nCntLoad].rot.x,
                                &g_aPlayer2[nCntLoad].rot.y,
                                &g_aPlayer2[nCntLoad].rot.z);
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
void UpdatePlayer2Motion(void)
{
    //モーションが変わったとき
    if (g_MotionOldState != g_MotionState) {
        g_nFremePlayer2 = 0;
        g_nCntKeyPlayer2 = 0;
    }
    for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
    {
        if (g_nFremePlayer2 == 0)
        {
            g_aNumPlayer2Pos[nCntMotion].x = ((g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].pos[nCntMotion].x -
                (g_aPlayer2[nCntMotion].pos.x - g_aPlayer2[nCntMotion].posOrigin.x)) / float(g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].nFrame));

            g_aNumPlayer2Pos[nCntMotion].y = ((g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].pos[nCntMotion].y -
                (g_aPlayer2[nCntMotion].pos.y - g_aPlayer2[nCntMotion].posOrigin.y)) / float(g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].nFrame));

            g_aNumPlayer2Pos[nCntMotion].z = ((g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].pos[nCntMotion].z -
                (g_aPlayer2[nCntMotion].pos.z - g_aPlayer2[nCntMotion].posOrigin.z)) / float(g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].nFrame));

            g_aNumPlayer2RotDest[nCntMotion] =
                g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].rot[nCntMotion] - g_aPlayer2[nCntMotion].rot;

            g_aNumPlayer2Rot[nCntMotion] = (g_aNumPlayer2RotDest[nCntMotion] / float(g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].nFrame));
        }

        g_aPlayer2[nCntMotion].pos += g_aNumPlayer2Pos[nCntMotion];
        g_aPlayer2[nCntMotion].rot += g_aNumPlayer2Rot[nCntMotion];
    }
    g_MotionOldState = g_MotionState;
    // ループするとき
    if (g_nFremePlayer2 >= g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].nFrame && g_aPlayer2Motion[g_MotionState].nLoop == 1)
    {
        g_nFremePlayer2 = 0;
        g_nCntKeyPlayer2++;

        if (g_nCntKeyPlayer2 >= g_aPlayer2Motion[g_MotionState].nNum_Key)
        {
            g_nCntKeyPlayer2 = 0;
        }

    }
    // ループしないとき
    else if (g_nFremePlayer2 >= g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].nFrame && g_aPlayer2Motion[g_MotionState].nLoop == 0)
    {
        g_nFremePlayer2 = 0;
        g_nCntKeyPlayer2++;
        if (g_nCntKeyPlayer2 >= g_aPlayer2Motion[g_MotionState].nNum_Key)
        {
            for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
            {
                g_aNumPlayer2Pos[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aNumPlayer2Rot[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
            g_MotionState = MOTIONSTATE_PLAYER2_IDLE;
            g_nCntKeyPlayer2 = 0;
        }
    }
    else
    {
        g_nFremePlayer2++;
    }
}

//===========================================================================================================================
// 使用判定の初期化
// Author:後藤慎之助
//===========================================================================================================================
void InitDispPlayer2(void)
{
    g_Player2.bDisp = false;
}