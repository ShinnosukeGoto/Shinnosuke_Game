//=====================================================
//
// 3Dゲームのひな形の処理[player.h]
// Author:後藤慎之助
//
//=====================================================

//=====================================================
// インクルードファイル
//=====================================================
#include "player.h"
#include "camera.h"
#include "bullet.h"
#include "input.h"
#include "shadow.h"
#include "controller.h"
#include "block.h"
#include "wall.h"
#include "enemy.h"
#include "collision.h"
#include "item.h"
#include "campaign.h"
#include "effect.h"
#include "effect2.h"
#include "enemy.h"
#include "sound.h"
#include "boss.h"

//=====================================================
// グローバル変数定義
//=====================================================
PLAYER g_Player;        //プレイヤーの情報
int g_ShadowIndex;
float g_fAngle;         //スティックの角度記憶用
int g_nCntCharge;       //チャージ時間をカウント
bool g_bChange;         //武器持ち替え用
bool g_bReachBoss;      //ボスにたどり着いたかどうか

int g_FrameCount/* = 0*/;				// 現在のフレーム数

int g_PlayKeySetlndex/* = 0*/;			// 再生中のキーセットインデックス

int g_PlayerCurrentAnimation/* = MOTION_IDLE*/;	// 再生中のアニメーション

int g_nFreme/* = 0*/;					// フレーム数

int g_nCntKey/* = 1*/;					// キー数

int g_nChangeFlame/* = 20*/;			// モーションのフレーム

MOTIONSTATE g_MotionState;			// モーションの構造体
MOTIONSTATE g_MotionOldState;		// 1F前のモーションの構造体

D3DXVECTOR3 g_NumPos[MAX_PARTS];		// posの差分
D3DXVECTOR3 g_NumRot[MAX_PARTS];		// rotの差分
D3DXVECTOR3 g_NumRotDest[MAX_PARTS];	// rotの差分

ANIMATION g_aPMotion[MOTION_MAX];	// モーションの情報
                                    // モデルパーツのデータ
ModelParts g_aPlayer[MAX_PARTS] =
{
    { "MODEL/00_body.x" },
    { "MODEL/01_head.x" },
    { "MODEL/02_armR.x" },
    { "MODEL/03_handR.x" },
    { "MODEL/04_armL.x" },
    { "MODEL/05_handL.x" },
    { "MODEL/06_legR.x" },
    { "MODEL/07_footR.x" },
    { "MODEL/08_legL.x" },
    { "MODEL/09_footL.x" },
    { "MODEL/10_weapon.x" },
    { "MODEL/11_weapon2.x" },
};

//=====================================================
// プレイヤー初期化処理
//=====================================================
void InitPlayer(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //グローバル変数の初期化
    g_MotionState = MOTION_IDLE;
    g_fAngle = 0.0f;
    g_nCntCharge = 0;
    g_Player.bChanceAttack = false;
    g_bChange = false;
    g_bReachBoss = false;

    g_FrameCount = 0;				// 現在のフレーム数

    g_PlayKeySetlndex = 0;			// 再生中のキーセットインデックス

    g_PlayerCurrentAnimation = MOTION_IDLE;	// 再生中のアニメーション

    g_nFreme = 0;					// フレーム数

    g_nCntKey = 1;					// キー数

    g_nChangeFlame = 20;			// モーションのフレーム

    //構造体の初期化
    g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, -3500.0f);		// 初期位置
    g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期位置
    g_Player.rot = D3DXVECTOR3(0.0f, 1800.0f, 0.0f);		// 初期向き
    g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期移動量
    g_Player.bDisp = true;
    g_Player.bJump = false;
    g_Player.bSecondJump = false;
    g_Player.bSkyDash = false;
    g_Player.bLand = false;
    g_Player.bGuard = false;
    g_Player.bBullet = true;
    g_Player.state = PLAYERSTATE_NORMAL;
    g_Player.nCounterState = 0;
    g_Player.nCntSkyDash = 0;
    g_Player.nAttackTime = 0;
    g_Player.nCntEXAttackCD = 0;
    g_Player.fLife = PLAYER_LIFE;	//自機の体力
    g_Player.fEnergy = PLAYER_ENERGY;	//自機のエナジー
    g_Player.fOldEnergy = PLAYER_ENERGY;	//自機の1Fエナジー
    g_Player.fChanceMeter = 0.0f;           //必殺ゲージ
    g_Player.nChance = 0;

    //影の設定
    //g_ShadowIndex = SetShadow(g_Player.pos, g_Player.rot);

    LoadPlayer();
    LoadAnimation();

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

//=====================================================
// プレイヤー終了処理
//=====================================================
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

//=====================================================
// プレイヤー更新処理
//=====================================================
void UpdatePlayer(void)
{
    //コントローラーの情報を取得する
    DIJOYSTATE2 Controller = GetController();
    //カメラの情報を取得する
    CAMERA*pCamera = GetCamera();
    //敵の情報を取得する
    ENEMY*pEnemy;
    BOSS*pBoss;

    //プレイヤーの状態
    switch (g_Player.state)
    {
    case PLAYERSTATE_NORMAL:	//通常の状態
        break;

    case PLAYERSTATE_DAMAGE:	//ダメージ状態
        g_Player.nCounterState--;

        if (g_Player.nCounterState <= 0)
        {
            g_Player.state = PLAYERSTATE_NORMAL;

        }
        break;
    }

    //プレイヤーが生存しているなら更新
    if (g_Player.bDisp == true)
    {
        // プレイヤーが動いていないとき
        D3DXVECTOR3 lengthCheckVec = g_Player.move;
        lengthCheckVec.y = 0;
        if (D3DXVec3Length(&lengthCheckVec) <= 1.5f && g_Player.bJump == false)
        {
            g_MotionState = MOTION_IDLE;    //モーションをアイドルにする
        }

        //1F前の位置を記憶
        g_Player.posOld = g_Player.pos;

        //1F前のエナジーを記憶
        g_Player.fOldEnergy = g_Player.fEnergy;

        //重力
        g_Player.move.y -= GRAVITY;

        //ノックバックを制御
        if (g_Player.move.x != 0.0f || g_Player.move.z != 0.0f)
        {
            g_Player.move.x *= 0.8f;
            g_Player.move.z *= 0.8f;
        }

        //攻撃していないとき
        if (g_Player.nAttackTime == 0)
        {

            //ジャンプ
            if (g_Player.bSkyDash == false && g_Player.bJump == false && GetJoypadTrigger(BUTTON_A))
            {
                g_Player.move.y = 0;
                g_Player.move.y += VALUE_JUMP;
                g_Player.bJump = true;
                g_Player.bSecondJump = true;
                g_MotionState = MOTION_JUMP;    //モーションをジャンプにする
            }

            //2段目ジャンプ
            else if (g_Player.bSecondJump == true && GetJoypadTrigger(BUTTON_A))
            {
                g_Player.move.y = 0;
                g_Player.move.y += VALUE_JUMP;
                g_Player.bSecondJump = false;
                g_MotionState = MOTION_JUMP;    //モーションをジャンプにする
            }
        }

        //位置更新
        g_Player.pos += g_Player.move;

        //地面にいる
        if (g_Player.pos.y < 0)
        {
            g_Player.move.y = 0;	//重力を切る
            g_Player.pos.y = 0;	//めりこまないようにする
            //g_Player.rot.x = 0;	//向きを直す
            g_Player.bJump = false;
            g_Player.bSecondJump = false;
            g_Player.bSkyDash = false;
            g_Player.nCntSkyDash = 0;

            //攻撃していないとき
            if (g_Player.nAttackTime == 0)
            {

                //ガードをしている
                if (GetJoypadPress(BUTTON_L1) || GetJoypadPress(BUTTON_L2))
                {
                    g_Player.bGuard = true;
                }
                else
                {
                    g_Player.bGuard = false;
                }

                if (Controller.lY != 0 || Controller.lX != 0)
                {
                    float fAngle = atan2(Controller.lX, Controller.lY*-1);//スティックの角度を求める
                    float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//スティックの角度を求める

                    //スティックとカメラの向きを記録する
                    g_fAngle = pCamera->rot.y + fAngle;

                    //ガード中の歩き
                    if (g_Player.bGuard == true)
                    {
                        g_Player.pos.x += sinf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                        g_Player.pos.z += cosf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                    }

                    //ガードしていない
                    else
                    {
                        //ダッシュ
                        if (GetJoypadPress(BUTTON_B) && g_Player.fEnergy != 0.0f)
                        {
                            g_Player.fEnergy -= ENERGY_DASH;
                            g_Player.pos.x += sinf(pCamera->rot.y + fAngle) *VALUE_DASH;
                            g_Player.pos.z += cosf(pCamera->rot.y + fAngle) *VALUE_DASH;
                            g_MotionState = MOTION_WALK;//モーションを歩きにする
                        }

                        //歩き
                        else
                        {
                            g_Player.pos.x += sinf((pCamera->rot.y + fAngle))*VALUE_MOVE;
                            g_Player.pos.z += cosf((pCamera->rot.y + fAngle))*VALUE_MOVE;
                            g_MotionState = MOTION_WALK;//モーションを歩きにする
                            //SetEffect2(g_Player.pos, g_Player.pos, 0.0f,EFFECT2TYPE_RUN);
                        }

                    }
                    //キャラの向きを変える
                    g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
                }
                else
                {
                    //ダッシュ
                    if (GetJoypadPress(BUTTON_B) && g_Player.bGuard == false && g_Player.fEnergy != 0.0f)
                    {
                        g_Player.fEnergy -= ENERGY_DASH;
                        g_Player.pos.x += sinf(g_fAngle) *VALUE_DASH;
                        g_Player.pos.z += cosf(g_fAngle) *VALUE_DASH;
                        g_MotionState = MOTION_WALK;//モーションを歩きにする
                    }
                }
            }
        }

        //地面より上にいる
        else
        {
            g_Player.bJump = true;
            g_Player.bGuard = false;   //空中はガードできない

            //攻撃していないとき
            if (g_Player.nAttackTime == 0)
            {

                //スティックが倒れている
                if (Controller.lY != 0 || Controller.lX != 0)
                {
                    //空中ダッシュ中は、方向転換不可
                    if (g_Player.bSkyDash == false)
                    {
                        float fAngle = atan2(Controller.lX, Controller.lY*-1);//スティックの角度を求める
                        float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//スティックの角度を求める

                        //スティックとカメラの向きを記録する
                        g_fAngle = pCamera->rot.y + fAngle;

                        //ブロックの上
                        if (g_Player.bLand == true)
                        {
                            //ガードをしている
                            if (GetJoypadPress(BUTTON_L1) || GetJoypadPress(BUTTON_L2))
                            {
                                g_Player.bGuard = true;
                            }
                            else
                            {
                                g_Player.bGuard = false;
                            }

                            //ガード中の歩き
                            if (g_Player.bGuard == true)
                            {
                                g_Player.pos.x += sinf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                                g_Player.pos.z += cosf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                            }

                            //ガードしていない
                            else
                            {
                                //ダッシュ
                                if (GetJoypadPress(BUTTON_B) && g_Player.fEnergy != 0.0f)
                                {
                                    g_Player.bSkyDash = true;
                                }

                                //ブロック上歩き
                                else
                                {
                                    g_Player.pos.x += sinf(g_fAngle) *VALUE_MOVE;
                                    g_Player.pos.z += cosf(g_fAngle) *VALUE_MOVE;
                                    g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
                                    g_MotionState = MOTION_WALK;//モーションを歩きにする
                                }
                            }
                        }

                        //空中
                        else
                        {
                            //空中ダッシュ
                            if (GetJoypadPress(BUTTON_B) && g_Player.fEnergy != 0.0f)
                            {
                                g_Player.bSkyDash = true;
                            }

                            //空中歩き
                            else
                            {
                                g_Player.pos.x += sinf(g_fAngle) *VALUE_MOVE;
                                g_Player.pos.z += cosf(g_fAngle) *VALUE_MOVE;
                                g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
                            }
                        }
                    }

                    //空中ダッシュの移動
                    if (g_Player.bSkyDash == true)
                    {
                        g_Player.nCntSkyDash++;    //空中移動の時間を数える

                        //ジャンプで、空中ダッシュキャンセル
                        if (GetJoypadTrigger(BUTTON_A))
                        {
                            g_Player.nCntSkyDash = SKY_DUSH_TIME;
                        }

                        //空中ダッシュのフレーム数
                        if (g_Player.nCntSkyDash > 0 && g_Player.nCntSkyDash < SKY_DUSH_TIME)
                        {
                            g_Player.fEnergy -= ENERGY_DASH;
                            //一段目のジャンプ時の空中ダッシュ
                            if (g_Player.bSecondJump == true)
                            {
                                g_Player.move.y = GRAVITY;  //空中にとどまるようにする
                            }
                            g_Player.pos.x += sinf(g_fAngle) *VALUE_DASH;
                            g_Player.pos.z += cosf(g_fAngle) *VALUE_DASH;

                            //ブロックの上にいるとき
                            if (g_Player.bLand == true)
                            {
                                g_MotionState = MOTION_WALK;//モーションを歩きにする
                                float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//スティックの角度を求める
                                //キャラの向きを変える
                                g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
                            }
                            else
                            {
                                g_MotionState = MOTION_SKYDASH;//モーションを空中ダッシュにする
                            }
                        }

                        //空中ダッシュのフレームがそれ以上になったら
                        else if (g_Player.nCntSkyDash >= SKY_DUSH_TIME)
                        {
                            g_MotionState = MOTION_WALK;//モーションを歩きにする
                            g_Player.nCntSkyDash = SKY_DUSH_TIME;
                            g_Player.pos.x += sinf(g_fAngle) *VALUE_AFTER_SKYDASH;
                            g_Player.pos.z += cosf(g_fAngle) *VALUE_AFTER_SKYDASH;
                        }
                    }
                }

                //スティックが倒れていない
                else
                {
                    //ダッシュ
                    if (g_Player.bSkyDash == false && GetJoypadPress(BUTTON_B) && g_Player.bGuard == false && g_Player.fEnergy != 0.0f)
                    {
                        g_Player.bSkyDash = true;
                    }

                    //空中ダッシュの移動
                    if (g_Player.bSkyDash == true)
                    {
                        g_Player.nCntSkyDash++;    //空中移動の時間を数える

                        //ジャンプで、空中ダッシュキャンセル
                        if (GetJoypadTrigger(BUTTON_A))
                        {
                            g_Player.nCntSkyDash = SKY_DUSH_TIME;
                        }

                        //空中ダッシュのフレーム数
                        if (g_Player.nCntSkyDash > 0 && g_Player.nCntSkyDash < SKY_DUSH_TIME)
                        {
                            g_Player.fEnergy -= ENERGY_DASH;
                            //一段目のジャンプ時の空中ダッシュ
                            if (g_Player.bSecondJump == true)
                            {
                                g_Player.move.y = GRAVITY;  //空中にとどまるようにする
                            }
                            g_Player.pos.x += sinf(g_fAngle) *VALUE_DASH;
                            g_Player.pos.z += cosf(g_fAngle) *VALUE_DASH;


                            //ブロックの上にいるとき
                            if (g_Player.bLand == true)
                            {
                                g_MotionState = MOTION_WALK;//モーションを歩きにする
                            }
                            else
                            {
                                g_MotionState = MOTION_SKYDASH;//モーションを空中ダッシュにする
                            }
                        }

                        //空中ダッシュのフレームがそれ以上になったら
                        else if (g_Player.nCntSkyDash >= SKY_DUSH_TIME)
                        {
                            g_Player.nCntSkyDash = SKY_DUSH_TIME;
                            g_Player.pos.x += sinf(g_fAngle) *VALUE_AFTER_SKYDASH;
                            g_Player.pos.z += cosf(g_fAngle) *VALUE_AFTER_SKYDASH;
                            g_MotionState = MOTION_WALK;//モーションを歩きにする
                        }
                    }
                }
            }
        }

        //ガード
        if (g_Player.bGuard == true && g_Player.nAttackTime == 0)
        {
            g_MotionState = MOTION_GUARD;//モーションをガードにする

            //通常モード
            if (pCamera->bBossMode == false)
            {
                pEnemy = GetEnemy();
                for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
                {
                    //一番近い敵に向く
                    if (pEnemy->nRank == 1)
                    {

                        float fPposx = g_Player.pos.x, fPposz = g_Player.pos.z;	//自機の座標
                        float fEposx = pEnemy->pos.x, fEposz = pEnemy->pos.z;		//敵の座標
                        float fAngle;											//角度

                        fAngle = atan2f((fPposx - fEposx), (fPposz - fEposz));	//角度を決める

                                                                                //ロックオンした敵のほうを向く
                        if (pCamera->bLockOn == true)
                        {
                            g_Player.rot.y = fAngle;
                        }
                    }
                }
            }

            //ボスモード
            else
            {
                pBoss = GetBoss();
                float fPposx = g_Player.pos.x, fPposz = g_Player.pos.z;	//自機の座標
                float fBposx = pBoss->pos.x, fBposz = pBoss->pos.z;		//敵の座標
                float fAngle;											//角度

                fAngle = atan2f((fPposx - fBposx), (fPposz - fBposz));	//角度を決める

                //ロックオンした敵のほうを向く
                if (pCamera->bLockOn == true)
                {
                    g_Player.rot.y = fAngle;
                }
            }

        }

        //攻撃
        if (GetJoypadTrigger(BUTTON_X) && g_Player.nAttackTime == 0 && g_Player.fEnergy != 0.0f)
        {
            if (g_Player.bBullet == true)
            {
                //銃攻撃のカウンター
                g_Player.nAttackTime = 40;
            }
            else
            {
                //剣攻撃のカウンター
                g_Player.nAttackTime = 45;
            }
            

            //ガードを解除する
            if (g_Player.bGuard == true)
            {
                g_Player.bGuard = false;
            }

            //カメラをロックオンさせる
            pCamera->bLockOn = true;
        }

        //武器切り替え
        if (GetJoypadTrigger(BUTTON_Y) && g_Player.nAttackTime == 0 && g_Player.fEnergy != 0.0f)
        {
            //攻撃のカウンター
            g_Player.nAttackTime = 30;

            //ガードを解除する
            if (g_Player.bGuard == true)
            {
                g_Player.bGuard = false;
            }

            //武器チェンジのフラグ
            g_bChange = true;
        }

        //必殺攻撃
        if (GetJoypadTrigger(BUTTON_R1) && g_Player.nAttackTime == 0 
            && g_Player.fEnergy > 0.0f && g_Player.nChance > 0 && g_Player.nCntEXAttackCD == 0||
            GetJoypadTrigger(BUTTON_R2) && g_Player.nAttackTime == 0 
            && g_Player.fEnergy > 0.0f && g_Player.nChance > 0 && g_Player.nCntEXAttackCD == 0)
        {
            //攻撃のカウンター
            g_Player.nAttackTime = VALUE_EXATTACK_FRAME;

            //ガードを解除する
            if (g_Player.bGuard == true)
            {
                g_Player.bGuard = false;
            }

            //チャンス値3のとき、メーターを0にする
            if (g_Player.nChance == 3)
            {
                g_Player.fChanceMeter = 0.0f;
            }

            //チャンス値を1消費
            g_Player.nChance--;

            //必殺技のクールタイム
            g_Player.nCntEXAttackCD = EXATTACK_CD;

            //必殺中にする
            g_Player.bChanceAttack = true;

            //カメラをロックオンさせる
            pCamera->bLockOn = true;
        }

        //攻撃中のカウンター
        if (g_Player.nAttackTime > 0)
        {
            //攻撃時間をカウント
            g_Player.nAttackTime--;

            //武器持ち替え
            if (g_bChange == true)
            {
                g_MotionState = MOTION_GUARD;//モーションを武器持ち替えにする

                if (g_Player.nAttackTime == 15)
                {
                    //反転
                    g_Player.bBullet = !g_Player.bBullet;
                }

                //武器チェンジを解除
                else if (g_Player.nAttackTime == 0)
                {
                    g_bChange = false;
                }
            }

            //攻撃
            else
            {
                //必殺攻撃
                if (g_Player.bChanceAttack == true)
                {

                    //銃の必殺時
                    if (g_Player.bBullet == true)
                    {
                        g_MotionState = MOTION_BULLET;//モーションを銃攻撃にする

                                                      //ガード時と同じ量、動ける
                                                      //スティックが倒れている
                        if (Controller.lY != 0 || Controller.lX != 0)
                        {
                            float fAngle = atan2(Controller.lX, Controller.lY*-1);//スティックの角度を求める
                            float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//スティックの角度を求める

                                                                                        //スティックとカメラの向きを記録する
                            g_fAngle = pCamera->rot.y + fAngle;

                            g_Player.pos.x += sinf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                            g_Player.pos.z += cosf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                            g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
                        }

                        //炎のエフェクト
                        if (g_Player.nAttackTime > 60)
                        {
                            float fMoveX = 0.0f;    //Yの移動量
                            float fMoveY = 0.0f;    //Yの移動量
                            float fMoveZ = 0.0f;    //Yの移動量
                            int nPlusMinusY = 0;

                            for (int nCnt = 0; nCnt < 4; nCnt++)
                            {
                                nPlusMinusY = int(rand() % 2 + 1);

                                if (nPlusMinusY == 1)
                                {
                                    nPlusMinusY = 1;
                                }
                                else
                                {
                                    nPlusMinusY = -1;
                                }
                                //横方向の移動
                                fMoveX = float(rand() % 800 + 300) / 100.0f;
                                //縦方向の移動
                                fMoveY = float(rand() % 300 + 0) / 100.0f;
                                //奥方向の移動
                                fMoveZ = float(rand() % 800 + 300) / 100.0f;

                                D3DXVECTOR3 move = D3DXVECTOR3(sinf(g_fAngle) *fMoveX, fMoveY*nPlusMinusY, cosf(g_fAngle) *fMoveZ);
                                SetEffect2(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + 80.0f, g_Player.modelParts[10].mtxWorld._43)
                                    , move, EFFECT2_SIZE_HEAL, EFFECT2_INIT_COLOR_HEAL, EFFECT2TYPE_FIRE); //ストームソードエフェクト白
                            }
                        }

                        //チャンスアタックを解除
                        else if (g_Player.nAttackTime == 0)
                        {
                            g_Player.bChanceAttack = false;
                        }
                    }

                    //剣の必殺時
                    else
                    {
                        g_MotionState = MOTION_SWORD2;//モーションを必殺にする

                        //通常モード
                        if (pCamera->bBossMode == false)
                        {
                            //敵を取得
                            pEnemy = GetEnemy();

                            for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
                            {
                                //一番近い敵に向く
                                if (pEnemy->nRank == 1)
                                {

                                    float fPposx = g_Player.pos.x, fPposz = g_Player.pos.z;	//自機の座標
                                    float fEposx = pEnemy->pos.x, fEposz = pEnemy->pos.z;	//敵の座標
                                    float fAngle;											//角度

                                    float fAdjustment;  //符号の調整

                                    float fDistanceY;   //Y軸の差

                                    fAngle = atan2f((fPposx - fEposx), (fPposz - fEposz));	//角度を決める

                                                                                            //高さ調整
                                    if (g_Player.pos.y > pEnemy->pos.y)
                                    {
                                        fAdjustment = -1.0f;

                                        //Y軸
                                        fDistanceY = g_Player.pos.y - pEnemy->pos.y;
                                    }
                                    else if (g_Player.pos.y == pEnemy->pos.y)
                                    {
                                        fAdjustment = 0.0f;

                                        fDistanceY = 0.0f;
                                    }
                                    else if (g_Player.pos.y < pEnemy->pos.y)
                                    {
                                        fAdjustment = 1.0f;

                                        //Y軸
                                        fDistanceY = pEnemy->pos.y - g_Player.pos.y;
                                    }

                                    //当たり判定の発生
                                    if (g_Player.nAttackTime == 165)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 150)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 135)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 120)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 105)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 90)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 75)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 60)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 45)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 30)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }
                                    else if (g_Player.nAttackTime == 15)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(fAngle)*SWORD_SPEED), 8, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                                    }

                                }

                            }
                        }

                        //ボスモード
                        else
                        {
                            pBoss = GetBoss();
                            float fPposx = g_Player.pos.x, fPposz = g_Player.pos.z;	//自機の座標
                            float fBposx = pBoss->pos.x, fBposz = pBoss->pos.z;		//敵の座標
                            float fAngle;											//角度

                            fAngle = atan2f((fPposx - fBposx), (fPposz - fBposz));	//角度を決める

                            float fAdjustment;  //符号の調整

                            float fDistanceY;   //Y軸の差


                            //高さ調整
                            if (g_Player.pos.y > pBoss->pos.y + BOSS_HEIGHT)
                            {
                                fAdjustment = -1.0f;

                                //Y軸
                                fDistanceY = g_Player.pos.y - pBoss->pos.y + BOSS_HEIGHT;
                            }
                            else if (g_Player.pos.y == pBoss->pos.y + BOSS_HEIGHT)
                            {
                                fAdjustment = 0.0f;

                                fDistanceY = 0.0f;
                            }
                            else if (g_Player.pos.y < pBoss->pos.y + BOSS_HEIGHT)
                            {
                                fAdjustment = 1.0f;

                                //Y軸
                                fDistanceY = pBoss->pos.y + BOSS_HEIGHT - g_Player.pos.y;
                            }

                            //当たり判定の発生
                            if (g_Player.nAttackTime == 165)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 150)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 135)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 120)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 105)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 90)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 75)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 60)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 45)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 30)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                            else if (g_Player.nAttackTime == 15)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(fAngle)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(fAngle)*SWORD_SPEED), 9, SWORD_EXRIGID, SWORD_EXDAMAGE, BULLETTYPE_PLAYER);
                            }
                        }
                        

                        //ガード時と同じ量、動ける
                        //スティックが倒れている
                        if (Controller.lY != 0 || Controller.lX != 0)
                        {
                            float fAngle = atan2(Controller.lX, Controller.lY*-1);//スティックの角度を求める
                            float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//スティックの角度を求める

                                                                                        //スティックとカメラの向きを記録する
                            g_fAngle = pCamera->rot.y + fAngle;

                            g_Player.pos.x += sinf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                            g_Player.pos.z += cosf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                        }

                        //ジャンプ
                        if (g_Player.bSkyDash == false && g_Player.bJump == false && GetJoypadTrigger(BUTTON_A))
                        {
                            g_Player.move.y = 0;
                            g_Player.move.y += VALUE_JUMP;
                            g_Player.bJump = true;
                            g_Player.bSecondJump = true;
                        }

                        //モデルを高速回転させる
                        g_Player.rot.y -= 0.4f;
                        if (g_Player.rot.y <= -3600.0f)
                        {
                            g_Player.rot.y = 0.0f;
                        }

                        //チャンスアタックを解除
                        if (g_Player.nAttackTime == 0)
                        {
                            g_Player.bChanceAttack = false;
                        }
                    }

                }

                //通常攻撃
                else
                {
                    //通常攻撃は、エナジーを消費する
                    //銃の時
                    if (g_Player.bBullet == true)
                    {
                        g_MotionState = MOTION_BULLET;//モーションを銃攻撃にする
                        g_Player.fEnergy -= ENERGY_BULLET_ATTACK;
                    }

                    //剣の時
                    else
                    {
                        g_MotionState = MOTION_SWORD1;//モーションを剣攻撃にする
                        g_Player.fEnergy -= ENERGY_SWORD_ATTACK;
                    }

                    
                    //ガード時と同じ量、銃を撃っている間も動ける
                    if (g_Player.bBullet == true)
                    {
                        //スティックが倒れている
                        if (Controller.lY != 0 || Controller.lX != 0)
                        {
                            float fAngle = atan2(Controller.lX, Controller.lY*-1);//スティックの角度を求める
                            float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//スティックの角度を求める

                                                                                        //スティックとカメラの向きを記録する
                            g_fAngle = pCamera->rot.y + fAngle;

                            g_Player.pos.x += sinf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                            g_Player.pos.z += cosf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                            g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
                        }
                    }

                    //剣を振りかざす間、動ける
                    else
                    {
                        if (g_Player.nAttackTime > 20)
                        {
                            //ガード時と同じ量、動ける
                            //スティックが倒れている
                            if (Controller.lY != 0 || Controller.lX != 0)
                            {
                                float fAngle = atan2(Controller.lX, Controller.lY*-1);//スティックの角度を求める
                                float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//スティックの角度を求める

                                                                                            //スティックとカメラの向きを記録する
                                g_fAngle = pCamera->rot.y + fAngle;

                                g_Player.pos.x += sinf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                                g_Player.pos.z += cosf(pCamera->rot.y + fAngle) *VALUE_GUARD;
                                g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
                            }
                        }

                    }

                    //通常モード
                    if (pCamera->bBossMode == false)
                    {
                        //敵を取得
                        pEnemy = GetEnemy();
                        for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
                        {
                            //一番近い敵に向く
                            if (pEnemy->nRank == 1)
                            {

                                float fPposx = g_Player.pos.x, fPposz = g_Player.pos.z;	//自機の座標
                                float fEposx = pEnemy->pos.x, fEposz = pEnemy->pos.z;	//敵の座標
                                float fAngle;											//角度

                                float fAdjustment;  //符号の調整

                                float fDistanceY;   //Y軸の差

                                fAngle = atan2f((fPposx - fEposx), (fPposz - fEposz));	//角度を決める

                                //ロックオンした敵のほうを向く
                                g_Player.rot.y = fAngle;


                                //高さ調整
                                if (g_Player.pos.y > pEnemy->pos.y)
                                {
                                    fAdjustment = -1.0f;

                                    //Y軸
                                    fDistanceY = g_Player.pos.y - pEnemy->pos.y;
                                }
                                else if (g_Player.pos.y == pEnemy->pos.y)
                                {
                                    fAdjustment = 0.0f;

                                    fDistanceY = 0.0f;
                                }
                                else if (g_Player.pos.y < pEnemy->pos.y)
                                {
                                    fAdjustment = 1.0f;

                                    //Y軸
                                    fDistanceY = pEnemy->pos.y - g_Player.pos.y;
                                }

                                //銃攻撃
                                if (g_Player.bBullet == true)
                                {
                                    if (g_Player.nAttackTime == 32)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                            D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                                , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                        PlaySound(SOUND_LABEL_SE_SHOT);
                                    }
                                    else if (g_Player.nAttackTime == 28)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                            D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                                , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                        PlaySound(SOUND_LABEL_SE_SHOT);
                                    }
                                    else if (g_Player.nAttackTime == 24)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                            D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                                , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                        PlaySound(SOUND_LABEL_SE_SHOT);
                                    }
                                    else if (g_Player.nAttackTime == 20)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                            D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                                , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                        PlaySound(SOUND_LABEL_SE_SHOT);
                                    }
                                }

                                //剣攻撃
                                else
                                {
                                    if (g_Player.nAttackTime == 18)
                                    {
                                        SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                            D3DXVECTOR3(-sinf(g_Player.rot.y)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                                , -cosf(g_Player.rot.y)*SWORD_SPEED), 8, SWORD_RIGID, SWORD_DAMAGE, BULLETTYPE_PLAYER);
                                    }
                                }
                            }
                        }
                    }

                    //ボスモード
                    else
                    {
                        pBoss = GetBoss();
                        float fPposx = g_Player.pos.x, fPposz = g_Player.pos.z;	//自機の座標
                        float fBposx = pBoss->pos.x, fBposz = pBoss->pos.z;		//敵の座標
                        float fAngle;											//角度

                        fAngle = atan2f((fPposx - fBposx), (fPposz - fBposz));	//角度を決める

                        float fAdjustment;  //符号の調整

                        float fDistanceY;   //Y軸の差

                        //ロックオンした敵のほうを向く
                        g_Player.rot.y = fAngle;

                                            //高さ調整
                        if (g_Player.pos.y > pBoss->pos.y + BOSS_HEIGHT)
                        {
                            fAdjustment = -1.0f;

                            //Y軸
                            fDistanceY = g_Player.pos.y - pBoss->pos.y + BOSS_HEIGHT;
                        }
                        else if (g_Player.pos.y == pBoss->pos.y + BOSS_HEIGHT)
                        {
                            fAdjustment = 0.0f;

                            fDistanceY = 0.0f;
                        }
                        else if (g_Player.pos.y < pBoss->pos.y + BOSS_HEIGHT)
                        {
                            fAdjustment = 1.0f;

                            //Y軸
                            fDistanceY = pBoss->pos.y + BOSS_HEIGHT - g_Player.pos.y;
                        }

                        //銃攻撃
                        if (g_Player.bBullet == true)
                        {
                            if (g_Player.nAttackTime == 32)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                    D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                        , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                PlaySound(SOUND_LABEL_SE_SHOT);
                            }
                            else if (g_Player.nAttackTime == 28)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                    D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                        , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                PlaySound(SOUND_LABEL_SE_SHOT);
                            }
                            else if (g_Player.nAttackTime == 24)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                    D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                        , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                PlaySound(SOUND_LABEL_SE_SHOT);
                            }
                            else if (g_Player.nAttackTime == 20)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[10].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[10].mtxWorld._43), BULLET_SIZE_PLAYER,
                                    D3DXVECTOR3(-sinf(g_Player.rot.y)*BULLET_SPEED_PLAYER, fDistanceY / BULLET_SPEED_PLAYER * fAdjustment
                                        , -cosf(g_Player.rot.y)*BULLET_SPEED_PLAYER), 28, BULLET_RIGID, BULLET_DAMAGE_PLAYER, BULLETTYPE_PLAYER);

                                PlaySound(SOUND_LABEL_SE_SHOT);
                            }
                        }

                        //剣攻撃
                        else
                        {
                            if (g_Player.nAttackTime == 18)
                            {
                                SetBullet(D3DXVECTOR3(g_Player.modelParts[11].mtxWorld._41, g_Player.pos.y + PLAYER_HEIGHT, g_Player.modelParts[11].mtxWorld._43), SWORD_SIZE,
                                    D3DXVECTOR3(-sinf(g_Player.rot.y)*SWORD_SPEED, fDistanceY / SWORD_SPEED * fAdjustment
                                        , -cosf(g_Player.rot.y)*SWORD_SPEED), 8, SWORD_RIGID, SWORD_DAMAGE, BULLETTYPE_PLAYER);
                            }
                        }
                    }
                }
            }

        }

        //必殺技のクールタイム
        if (g_Player.nCntEXAttackCD > 0)
        {
            g_Player.nCntEXAttackCD--;
        }

        //ブロックの上にいるとき
        //ブロックの上にのっていないときのelseがあるため、共通化していない
        if (LandBlock(&g_Player.pos, &g_Player.posOld, D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH)) == true)
        {
            g_Player.move.y = 0;
            g_Player.bJump = false;
            g_Player.bSecondJump = true;
            g_Player.bSkyDash = false;
            g_Player.nCntSkyDash = 0;
            g_Player.bLand = true;

            //攻撃していないとき
            if (g_Player.nAttackTime == 0)
            {

                //ガードをしている
                if (GetJoypadPress(BUTTON_L1) || GetJoypadPress(BUTTON_L2))
                {
                    g_Player.bGuard = true;
                }
                else
                {
                    g_Player.bGuard = false;
                }
            }
        }
        else
        {
            g_Player.bLand = false;
        }

        //弾の当たり判定
        CollisionBulletPlayer(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT * 2, PLAYER_WIDTH));

        //敵との当たり判定
        CollisionEnemy(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT * 2, PLAYER_WIDTH));

        //ボスとの当たり判定
        CollisionBoss(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT * 2, PLAYER_WIDTH));

        //アイテムとの当たり判定
        CollisionItem(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT * 2, PLAYER_WIDTH));

        //エフェクトとの当たり判定
        CollisionEffectPlayer(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT * 2, PLAYER_WIDTH));

        //影をつける
        //SetPositionShadow(g_ShadowIndex, D3DXVECTOR3(g_Player.pos.x, SHADOW_HEIGHT, g_Player.pos.z));

        //移動制限
        //左
        if (g_Player.pos.x < -WALL_SIZE+ PLAYER_WIDTH)
        {
            g_Player.pos.x = -WALL_SIZE + PLAYER_WIDTH;
        }
        //右
        if (g_Player.pos.x > WALL_SIZE - PLAYER_WIDTH)
        {
            g_Player.pos.x = WALL_SIZE - PLAYER_WIDTH;
        }

        //後ろ
        if (g_Player.pos.z < -WALL_SIZE * 4 + PLAYER_WIDTH)
        {
            g_Player.pos.z = -WALL_SIZE * 4 + PLAYER_WIDTH;
        }
        //一番奥
        if (g_Player.pos.z > WALL_SIZE * 4 - PLAYER_WIDTH)
        {
            g_Player.pos.z = WALL_SIZE * 4 - PLAYER_WIDTH;
        }

        //フェーズごとの壁
        WALL*pWall = GetWall();
        for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
        {
            if (pWall->bUse == true)
            {
                switch (pWall->type)
                {
                case WALLTYPE_RED1:
                    if (g_Player.pos.z > -WALL_SIZE * 2 - PLAYER_WIDTH)
                    {
                        g_Player.pos.z = -WALL_SIZE * 2 - PLAYER_WIDTH;
                    }
                    break;

                case WALLTYPE_RED2:
                    if (g_Player.pos.z > 0.0f - PLAYER_WIDTH)
                    {
                        g_Player.pos.z = 0.0f - PLAYER_WIDTH;
                    }
                    break;

                case WALLTYPE_RED3:
                    if (g_Player.pos.z > WALL_SIZE * 2 - PLAYER_WIDTH)
                    {
                        g_Player.pos.z = WALL_SIZE * 2 - PLAYER_WIDTH;
                    }
                    break;
                }
            }
        }

        //ボスにたどり着いたら、カメラをボスにロックオン
        if (g_bReachBoss == false)
        {
            if (g_Player.pos.z > REACH_BOSS_Z)
            {
                pCamera->state = CAMERASTATE_BOSS;
                g_bReachBoss = true;
            }
        }
            
        //死亡判定
        if (g_Player.fLife <= 0.0f)
        {
            //爆発エフェクト
            float fSpeedX = 0.0f;   //速さX
            float fSpeedY = 0.0f;   //速さY
            float fSpeedZ = 0.0f;   //速さZ
            int nPlusMinusX = 0;
            int nPlusMinusY = 0;
            int nPlusMinusZ = 0;
            for (int nCnt = 0; nCnt < 200; nCnt++)
            {
                fSpeedX = float(rand() % 800 + 300) / 100.0f;
                fSpeedY = float(rand() % 800 + 300) / 100.0f;
                fSpeedZ = float(rand() % 800 + 300) / 100.0f;
                nPlusMinusX = int(rand() % 2 + 1);
                nPlusMinusY = int(rand() % 2 + 1);
                nPlusMinusZ = int(rand() % 2 + 1);
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

                D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX*nPlusMinusX, fSpeedY*nPlusMinusY, fSpeedZ*nPlusMinusZ);
                SetEffect2(g_Player.pos, move, EFFECT2_SIZE_EXPLOSION, EFFECT2_INIT_COLOR_EXPLOSION2, EFFECT2TYPE_EXPLOSION); //爆発エフェクト
            }

            g_Player.bDisp = false;    //自機を未使用に

            PlaySound(SOUND_LABEL_SE_EXPLOSION2);

            SetCampaignState(CAMPAIGNSTATE_GAMEOVER);    //ゲームオーバー画面へ     
        }

        //モデルのアニメーション
        UpdatePlayerMotion();

    }
}

//=====================================================
// プレイヤー描画処理
//=====================================================
void DrawPlayer(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスへのポインタ
    D3DXMATRIX mtxRot, mtxTrans, mtxParent;
    D3DMATERIAL9 matDef;		// 現在のマテリアル保持用
    D3DXMATERIAL* pMat;			// マテリアルデータへのポインタ

    //プレイヤーが存在しているなら描画
    if (g_Player.bDisp == true)
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
                
                if (nCntParts == 10)
                {
                    // モデルパーツ
                    if (g_Player.bBullet == true)
                    {
                        g_Player.modelParts[nCntParts].pMesh->DrawSubset(nCntMat);
                    }
                }
                
                else if (nCntParts == 11)
                {
                    // モデルパーツ
                    if (g_Player.bBullet == false)
                    {
                        g_Player.modelParts[nCntParts].pMesh->DrawSubset(nCntMat);
                    }
                }

                else
                {
                    g_Player.modelParts[nCntParts].pMesh->DrawSubset(nCntMat);
                }
            }

            // 保持していたマテリアルを戻す
            pDevice->SetMaterial(&matDef);
        }
    }
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
PLAYER*GetPlayer(void)
{
    return &g_Player;
}

//=============================================================================
// プレイヤーのダメージ設定
//=============================================================================
void HitPlayer(int nCntRigid, float fDamage)
{
    //PlaySound(SOUND_LABEL_SE_EXPLOSION);	//爆発音

    //ダメージ状態に遷移
    g_Player.state = PLAYERSTATE_DAMAGE;
    g_Player.nCounterState = nCntRigid; //ヒット硬直

    //ダメージ計算
    //ガードをしている
    if (g_Player.bGuard == true)
    {
        g_Player.fLife -= fDamage/2;
    }
    //ガードしていない
    else
    {
        g_Player.fLife -= fDamage;
    }
    
    //必殺ゲージ上昇
    g_Player.fChanceMeter += TAKE_DAMAGE_CHARGE;
}

//=============================================================================
// アニメーションロード
//=============================================================================
void LoadAnimation(void)
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
    pFile = fopen("motion.txt", "r");

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
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPMotion[nCntMotion].nLoop);
                        }

                        if (strcmp(cHeadText, "NUM_KEY") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPMotion[nCntMotion].nNum_Key);
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
                                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPMotion[nCntMotion].KeyInfo[nCntKey].nFrame);
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
                                                &g_aPMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].x,
                                                &g_aPMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].y,
                                                &g_aPMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].z);
                                        }

                                        if (strcmp(cHeadText, "ROT") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &g_aPMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].x,
                                                &g_aPMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].y,
                                                &g_aPMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].z);
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

//=============================================================================
// モデルロード
//=============================================================================
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
    pFile = fopen("player.txt", "r");

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

//=============================================================================
// モーションの更新
//=============================================================================
void UpdatePlayerMotion(void)
{
    //モーションが変わったとき
    if (g_MotionOldState != g_MotionState) {
        g_nFreme = 0;
        g_nCntKey = 0;
    }
    for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
    {
        if (g_nFreme == 0)
        {
            g_NumPos[nCntMotion].x = ((g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].pos[nCntMotion].x -
                (g_aPlayer[nCntMotion].pos.x - g_aPlayer[nCntMotion].posOrigin.x)) / float(g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].nFrame));

            g_NumPos[nCntMotion].y = ((g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].pos[nCntMotion].y -
                (g_aPlayer[nCntMotion].pos.y - g_aPlayer[nCntMotion].posOrigin.y)) / float(g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].nFrame));

            g_NumPos[nCntMotion].z = ((g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].pos[nCntMotion].z -
                (g_aPlayer[nCntMotion].pos.z - g_aPlayer[nCntMotion].posOrigin.z)) / float(g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].nFrame));

            g_NumRotDest[nCntMotion] =
                g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].rot[nCntMotion] - g_aPlayer[nCntMotion].rot;

            g_NumRot[nCntMotion] = (g_NumRotDest[nCntMotion] / float(g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].nFrame));
        }

        g_aPlayer[nCntMotion].pos += g_NumPos[nCntMotion];
        g_aPlayer[nCntMotion].rot += g_NumRot[nCntMotion];
    }
    g_MotionOldState = g_MotionState;
    // ループするとき
    if (g_nFreme >= g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].nFrame && g_aPMotion[g_MotionState].nLoop == 1)
    {
        g_nFreme = 0;
        g_nCntKey++;

        if (g_nCntKey >= g_aPMotion[g_MotionState].nNum_Key)
        {
            g_nCntKey = 0;
        }

    }
    // ループしないとき
    else if (g_nFreme >= g_aPMotion[g_MotionState].KeyInfo[g_nCntKey].nFrame && g_aPMotion[g_MotionState].nLoop == 0)
    {
        g_nFreme = 0;
        g_nCntKey++;
        if (g_nCntKey >= g_aPMotion[g_MotionState].nNum_Key)
        {
            for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
            {
                g_NumPos[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_NumRot[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
            g_MotionState = MOTION_IDLE;
            g_nCntKey = 0;
        }
    }
    else
    {
        g_nFreme++;
    }
}