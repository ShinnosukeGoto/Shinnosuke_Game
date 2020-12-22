//=====================================================
//
// 最強昔話決定戦の処理[boss.h](ストーリーモードのボス)
// Author:後藤慎之助
//
//=====================================================

//=====================================================
// インクルードファイル
//=====================================================
#include "boss.h"
#include "camera.h"
#include "sound.h"
#include "effect.h"
#include "collision.h"
#include "game.h"
#include "bullet.h"
#include "player.h"
#include "camera.h"
#include "score.h"
#include "time.h"
#include "shadow.h"
#include "block.h"
#include "fire.h"
#include "ui.h"
#include "setting.h"

//=====================================================
// グローバル変数定義
//=====================================================
BOSS g_Boss;        //ボスの情報

int g_FrameCountBoss;				// 現在のフレーム数

int g_PlayKeySetIndexBoss;			// 再生中のキーセットインデックス

int g_BossCurrentAnimation;	// 再生中のアニメーション

int g_nFremeBoss;					// フレーム数

int g_nCntKeyBoss;					// キー数

int g_nChangeFlameBoss;			// モーションのフレーム

MOTIONSTATE_BOSS g_MotionOldStateBoss;		// 1F前のモーションの構造体

D3DXVECTOR3 g_NumPosBoss[MAX_BOSS_PARTS];		// posの差分
D3DXVECTOR3 g_NumRotBoss[MAX_BOSS_PARTS];		// rotの差分
D3DXVECTOR3 g_NumRotBossDest[MAX_BOSS_PARTS];	// rotの差分

ANIMATION_BOSS g_aBossMotion[MOTION_MAX_BOSS];	// モーションの情報
                                                // モデルパーツのデータ
ModelParts_BOSS g_aBoss[MAX_BOSS_PARTS] =
{
    { "MODEL/boss/00_body.x" },
    { "MODEL/boss/01_head.x" },
    { "MODEL/boss/02_armR.x" },
    { "MODEL/boss/03_handR.x" },
    { "MODEL/boss/04_armL.x" },
    { "MODEL/boss/05_handL.x" },
    { "MODEL/boss/06_legR.x" },
    { "MODEL/boss/07_footR.x" },
    { "MODEL/boss/08_legL.x" },
    { "MODEL/boss/09_footL.x" },
};

int g_nTimeScore;				        //残り時間に対応したスコア

//=====================================================
// ボス初期化処理
// Author:後藤慎之助
//=====================================================
void InitBoss(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //グローバル変数の初期化
    g_nTimeScore = 0;
    g_FrameCountBoss = 0;				// 現在のフレーム数
    g_PlayKeySetIndexBoss = 0;			// 再生中のキーセットインデックス
    g_BossCurrentAnimation = MOTION_IDLE_BOSS;	// 再生中のアニメーション
    g_nFremeBoss = 0;					// フレーム数
    g_nCntKeyBoss = 1;					// キー数
    g_nChangeFlameBoss = 20;			// モーションのフレーム

    //構造体の初期化
    g_Boss.pos = BOSS_FIRST_POS;		// 初期位置
    g_Boss.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期位置
    g_Boss.size = BOSS_SIZE;
    g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 初期向き
    g_Boss.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期向き
    g_Boss.rotMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期向き
    g_Boss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期移動量
    g_Boss.bUse = true;
    g_Boss.bStun = false;
    g_Boss.MotionState = MOTION_IDLE_BOSS;
    g_Boss.state = BOSSSTATE_STAY;
    g_Boss.nCounterRigid = 0;
    g_Boss.nStayTime = NORMAL_STAY_TIME;
    g_Boss.nAttackTime = 0;
    g_Boss.nChanceTime = 0;
    g_Boss.nCntFlinch = 0;
    g_Boss.nStunTime = FLINCH_TIME;
    g_Boss.fDistance = 9999.0f;
    g_Boss.fLife = BOSS_LIFE;	//自機の体力

    g_Boss.nShadow = SetShadow(g_Boss.pos, g_Boss.rot, g_Boss.size.x,
        g_Boss.pos.y, D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_ALPHA_MAX));//影の設定
    g_Boss.nCntInvincibleFrame = 0;
    g_Boss.bTransparence = false;

    g_Boss.rot.y = D3DXToRadian(90);

    LoadBoss();
    LoadBossAnimation();

    // モデルパーツの初期化
    for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
    {
        g_Boss.modelParts[nCntParts].nldxModelParent = 0;
    }
    // ファイルの読み込み
    for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
    {
        g_Boss.modelParts[nCntParts].nldxModelParent = g_aBoss[nCntParts].nParents;

        // Xファイルの読み込み
        D3DXLoadMeshFromX(g_aBoss[nCntParts].pFileName,
            D3DXMESH_SYSTEMMEM,
            pDevice,
            NULL,
            &g_Boss.modelParts[nCntParts].pBuffMat,
            NULL,
            &g_Boss.modelParts[nCntParts].nNumMat,
            &g_Boss.modelParts[nCntParts].pMesh);

    }
}

//=====================================================
// ボス終了処理
// Author:後藤慎之助
//=====================================================
void UninitBoss(void)
{
    for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
    {
        // メッシュの破棄
        if (g_Boss.modelParts[nCntParts].pMesh != NULL)
        {
            g_Boss.modelParts[nCntParts].pMesh->Release();
            g_Boss.modelParts[nCntParts].pMesh = NULL;
        }
        // マテリアルの破棄
        if (g_Boss.modelParts[nCntParts].pBuffMat != NULL)
        {
            g_Boss.modelParts[nCntParts].pBuffMat->Release();
            g_Boss.modelParts[nCntParts].pBuffMat = NULL;
        }
    }
}

//=====================================================
// ボス更新処理
// Author:後藤慎之助
//=====================================================
void UpdateBoss(void)
{
    //ボスが生存しているなら
    if (g_Boss.bUse == true)
    {
        //無敵時間
        if (g_Boss.nCntInvincibleFrame > 0)
        {
            g_Boss.nCntInvincibleFrame--;                     //無敵時間を数える
            g_Boss.bTransparence = !g_Boss.bTransparence;   //無敵中は、点滅させる
        }
        else
        {
            g_Boss.bTransparence = false; //透明を解除
        }

        //1F前の位置を記憶
        g_Boss.posOld = g_Boss.pos;

        //自機を取得する
        PLAYER *pPlayer = GetPlayer();
        CAMERA *pCamera = GetCamera();

        //攻撃や移動に関する変数
        float fPposx = pPlayer->pos.x, fPposz = pPlayer->pos.z;	//自機の座標
        float fEposx = g_Boss.pos.x, fEposz = g_Boss.pos.z;		//敵の座標
        float fAngle = atan2f((fEposx - fPposx), (fEposz - fPposz));	//角度を決める
        float fAngle2 = atan2f((fPposx - fEposx), (fPposz - fEposz));	//角度を決める
        int nRandAttack = 0;        //ランダムな攻撃にする

        //位置を移動量と紐づけ
        g_Boss.pos += g_Boss.move;

        //ボス戦なら
        if (pCamera->bBossBattle == true)
        {
            //自機とボスの距離
            g_Boss.fDistance = sqrtf(
                powf((pPlayer->pos.x - g_Boss.pos.x), 2) +
                powf((pPlayer->pos.z - g_Boss.pos.z), 2));

            //状態による場合分け
            switch (g_Boss.state)
            {
                //========================
                //向き取得待機
                //========================
            case BOSSSTATE_STAY:

                //移動量をなくす
                g_Boss.move.x = 0.0f;
                g_Boss.move.z = 0.0f;

                //スタンしていないなら
                if (g_Boss.bStun == false)
                {
                    //待機時間をカウント
                    if (g_Boss.nStayTime > 0)
                    {
                        g_Boss.nStayTime--;
                    }

                    //自機のほうを徐々に見るようにする
                    g_Boss.rotDest.y = fAngle;

                    // 回転の制限
                    if (g_Boss.rot.y > D3DX_PI)
                    {
                        g_Boss.rot.y -= D3DX_PI * 2;
                    }
                    if (g_Boss.rot.y < -D3DX_PI)
                    {
                        g_Boss.rot.y += D3DX_PI * 2;
                    }

                    g_Boss.rotMin.y = g_Boss.rotDest.y - g_Boss.rot.y;

                    // 目的の角度の回転を制限
                    if (g_Boss.rotMin.y > D3DX_PI)
                    {
                        g_Boss.rotDest.y -= D3DX_PI * 2;
                    }
                    if (g_Boss.rotMin.y < -D3DX_PI)
                    {
                        g_Boss.rotDest.y += D3DX_PI * 2;
                    }

                    // 回転の制限
                    if (g_Boss.rot.y > D3DX_PI)
                    {
                        g_Boss.rot.y -= D3DX_PI * 2;
                    }
                    if (g_Boss.rot.y < -D3DX_PI)
                    {
                        g_Boss.rot.y += D3DX_PI * 2;
                    }

                    if (fabsf(g_Boss.rotMin.y) < 0)
                    {
                        g_Boss.rotMin.y = g_Boss.rotDest.y;
                    }
                    else
                    {
                        //残り体力によって、振り向く速さを変える
                        if (g_Boss.fLife > BOSS_ANGER_LIFE)
                        {
                            g_Boss.rot.y += (g_Boss.rotDest.y - g_Boss.rot.y) * NORMAL_ROT_SPEED;
                        }
                        else if (g_Boss.fLife <= BOSS_ANGER_LIFE && g_Boss.fLife > BOSS_EXANGER_LIFE)
                        {
                            g_Boss.rot.y += (g_Boss.rotDest.y - g_Boss.rot.y) * ANGER_ROT_SPEED;
                        }
                        else if (g_Boss.fLife <= BOSS_EXANGER_LIFE)
                        {
                            g_Boss.rot.y += (g_Boss.rotDest.y - g_Boss.rot.y) * EXANGER_ROT_SPEED;
                        }
                    }

                }
                //スタンしたら
                else
                {
                    //残り体力によって、向き取得待機時間を設定
                    if (g_Boss.fLife > BOSS_ANGER_LIFE)
                    {
                        g_Boss.nStayTime = NORMAL_STAY_TIME;
                    }
                    else if (g_Boss.fLife <= BOSS_ANGER_LIFE && g_Boss.fLife > BOSS_EXANGER_LIFE)
                    {
                        g_Boss.nStayTime = ANGER_STAY_TIME;
                    }
                    else if (g_Boss.fLife <= BOSS_EXANGER_LIFE)
                    {
                        g_Boss.nStayTime = EXANGER_STAY_TIME;
                    }
                }

                //次の行動(攻撃)に切り替える
                if (g_Boss.nStayTime == 0)
                {
                    //乱数を初期化
                    nRandAttack = rand() % 100 + 1;

                    //近距離の時
                    if (g_Boss.fDistance < BOSS_CONSIDER_SHORT)
                    {
                        //5%
                        if (nRandAttack <= 5)
                        {
                            //ブレス
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_BREATH;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_BREATH;
                        }

                        //35%
                        else if (nRandAttack > 5 && nRandAttack <= 40)
                        {
                            //通常時
                            if (g_Boss.fLife > BOSS_EXANGER_LIFE)
                            {
                                //タックル
                                //攻撃に切り替える
                                g_Boss.state = BOSSSTATE_ATTACK_TACKLE;
                                //攻撃フレームを設定
                                g_Boss.nAttackTime = ATTACK_TIME_TACKLE;
                            }

                            //激昂時
                            else
                            {
                                //ジャンプ
                                //攻撃に切り替える
                                g_Boss.state = BOSSSTATE_ATTACK_JUMP;
                                //攻撃フレームを設定
                                g_Boss.nAttackTime = ATTACK_TIME_JUMP;
                            }

                        }

                        //60％
                        else if (nRandAttack > 40)
                        {
                            //通常時
                            if (g_Boss.fLife > BOSS_EXANGER_LIFE)
                            {
                                //ジャンプ
                                //攻撃に切り替える
                                g_Boss.state = BOSSSTATE_ATTACK_JUMP;
                                //攻撃フレームを設定
                                g_Boss.nAttackTime = ATTACK_TIME_JUMP;
                            }

                            //激昂時
                            else
                            {
                                //タックル
                                //攻撃に切り替える
                                g_Boss.state = BOSSSTATE_ATTACK_TACKLE;
                                //攻撃フレームを設定
                                g_Boss.nAttackTime = ATTACK_TIME_TACKLE;
                            }

                        }
                    }

                    //中距離の時
                    else if (g_Boss.fDistance >= BOSS_CONSIDER_SHORT && g_Boss.fDistance < BOSS_CONSIDER_MEDIUM)
                    {
                        //20%
                        if (nRandAttack <= 20)
                        {
                            //ジャンプ
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_JUMP;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_JUMP;
                        }

                        //30%
                        else if (nRandAttack > 20 && nRandAttack <= 50)
                        {
                            //ブレス
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_BREATH;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_BREATH;
                        }

                        //50％
                        else if (nRandAttack > 50)
                        {
                            //タックル
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_TACKLE;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_TACKLE;
                        }
                    }

                    //遠距離の時
                    else if (g_Boss.fDistance >= BOSS_CONSIDER_MEDIUM)
                    {
                        //5%
                        if (nRandAttack <= 5)
                        {
                            //ジャンプ
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_JUMP;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_JUMP;
                        }

                        //40%
                        else if (nRandAttack > 5 && nRandAttack <= 45)
                        {
                            //タックル
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_TACKLE;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_TACKLE;
                        }

                        //55％
                        else if (nRandAttack > 45)
                        {
                            //ブレス
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_BREATH;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_BREATH;
                        }
                    }

                }

                g_Boss.MotionState = MOTION_IDLE_BOSS;   //待機

                break;

                //========================
                //突進攻撃中
                //========================
            case BOSSSTATE_ATTACK_TACKLE:

                //スタンしていないなら
                if (g_Boss.bStun == false)
                {
                    //プレイヤーが生存しているなら
                    if (pPlayer->bDisp == true)
                    {
                        //残り体力によって、スピードを変える
                        if (g_Boss.fLife > BOSS_ANGER_LIFE)
                        {
                            //タックルのスピードを最初の1Fのみ取得
                            if (g_Boss.nAttackTime == ATTACK_TIME_TACKLE)
                            {
                                //自機を追いかける
                                g_Boss.move.x = -sinf(g_Boss.rot.y)*NORMAL_TACKLE_SPEED;
                                g_Boss.move.z = -cosf(g_Boss.rot.y)*NORMAL_TACKLE_SPEED;
                            }
                        }
                        //怒り/激昂時、追従させる
                        else
                        {
                            //怒り時
                            if (g_Boss.fLife > BOSS_EXANGER_LIFE)
                            {
                                //タックルのスピードを最初の1Fのみ取得
                                if (g_Boss.nAttackTime == ATTACK_TIME_TACKLE)
                                {
                                    //自機を追従
                                    g_Boss.move.x = -sinf(g_Boss.rot.y)*ANGER_TACKLE_SPEED;
                                    g_Boss.move.z = -cosf(g_Boss.rot.y)*ANGER_TACKLE_SPEED;
                                }
                            }
                            //激昂時
                            else
                            {
                                //タックルのスピードを最初の1Fのみ取得
                                if (g_Boss.nAttackTime == ATTACK_TIME_TACKLE)
                                {
                                    //自機を追いかける
                                    g_Boss.move.x = -sinf(g_Boss.rot.y)*EXANGER_TACKLE_SPEED;
                                    g_Boss.move.z = -cosf(g_Boss.rot.y)*EXANGER_TACKLE_SPEED;
                                }
                            }
                        }
                    }

                    //攻撃中のカウンター
                    if (g_Boss.nAttackTime > 0)
                    {
                        g_Boss.nAttackTime--;
                    }
                }

                //スタンしたら
                else
                {
                    //移動量をなくす
                    g_Boss.move.x = 0.0f;
                    g_Boss.move.z = 0.0f;

                    //スタン明けに、次の行動をさせるために1にする
                    g_Boss.nAttackTime = 1;
                }

                //次の行動に切り替える
                if (g_Boss.nAttackTime == 0)
                {
                    //怒り時
                    if (g_Boss.fLife < BOSS_ANGER_LIFE)
                    {
                        //距離が近いなら、ジャンプに派生
                        if (g_Boss.fDistance < BOSS_CONSIDER_SHORT)
                        {
                            //ジャンプ
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_JUMP;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_JUMP;
                        }

                        //距離が遠いなら、向き固定待機
                        else
                        {
                            //向き固定待機にする
                            g_Boss.state = BOSSSTATE_CHANCE;

                            //向き固定待機時間を設定
                            g_Boss.nChanceTime = CHANCE_TIME_TACKLE;
                        }
                    }

                    //通常時
                    else
                    {
                        //向き固定待機にする
                        g_Boss.state = BOSSSTATE_CHANCE;

                        //向き固定待機時間を設定
                        g_Boss.nChanceTime = CHANCE_TIME_TACKLE;
                    }

                }

                g_Boss.MotionState = MOTION_WALK_BOSS;   //歩き

                break;

                //========================
                //ジャンプ攻撃中
                //========================
            case BOSSSTATE_ATTACK_JUMP:

                //移動量をなくす
                g_Boss.move.x = 0.0f;
                g_Boss.move.z = 0.0f;

                //スタンしていないなら
                if (g_Boss.bStun == false)
                {
                    //ジャンプ量を最初の1Fのみ取得
                    if (g_Boss.nAttackTime == ATTACK_TIME_JUMP)
                    {
                        PlaySound(SOUND_LABEL_SE_JUMP);
                        //ジャンプ
                        g_Boss.pos.y = 1.0f;
                        g_Boss.move.y = VALUE_BOSS_JUMP;
                    }
                    //落下音
                    else if (g_Boss.nAttackTime == 90)
                    {
                        StopSound(SOUND_LABEL_SE_JUMP);
                        PlaySound(SOUND_LABEL_SE_FALL);
                    }
                    //落下を加速させる
                    else if (g_Boss.nAttackTime < 90)
                    {
                        //重力を強める
                        g_Boss.move.y -= GRAVITY * 2;
                    }

                    //着地した瞬間に、近くのプレイヤーをスタンさせる
                    if (g_Boss.nAttackTime == 30)
                    {
                        StopSound(SOUND_LABEL_SE_FALL);
                        PlaySound(SOUND_LABEL_SE_GROUND);

                        //通常時
                        if (g_Boss.fLife > BOSS_ANGER_LIFE)
                        {
                            //地ならしエフェクト(外側)
                            //エフェクト関連の変数
                            float fAngle = 0.0f;    //角度
                            float fSpeedY = 0.0f;   //速さ
                            float fRadius = RANGE_STUN_XZ; //エフェクト半径
                            for (int nCnt = 0; nCnt < COUNT_EFFECT_ITEM; nCnt++)
                            {
                                fSpeedY = float(rand() % 800 + 300) / 100.0f;
                                //0～3.14を出す*(0 or 1)*-1で正負を決める
                                fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                                D3DXVECTOR3 pos = D3DXVECTOR3(g_Boss.pos.x - sinf(fAngle)*fRadius, g_Boss.pos.y
                                    , g_Boss.pos.z - cosf(fAngle)*fRadius);
                                D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                                SetEffect(pos, move, EFFECT_SIZE_EXPLODE_PLAYER, EFFECTTYPE_EXPLOSION); //地ならし
                            }

                            //地ならしエフェクト(内側)
                            fRadius = RANGE_STUN_XZ / 3 * 2; //エフェクト半径
                            for (int nCnt = 0; nCnt < COUNT_EFFECT_ITEM; nCnt++)
                            {
                                fSpeedY = float(rand() % 800 + 300) / 100.0f;
                                //0～3.14を出す*(0 or 1)*-1で正負を決める
                                fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                                D3DXVECTOR3 pos = D3DXVECTOR3(g_Boss.pos.x - sinf(fAngle)*fRadius, g_Boss.pos.y
                                    , g_Boss.pos.z - cosf(fAngle)*fRadius);
                                D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                                SetEffect(pos, move, EFFECT_SIZE_EXPLODE_PLAYER, EFFECTTYPE_EXPLOSION); //地ならし
                            }

                            //地ならしで自機をスタンさせる
                            if (g_Boss.fDistance < RANGE_STUN_XZ && g_Boss.pos.y <= RANGE_STUN_Y - pPlayer->pos.y)
                            {
                                pPlayer->bStun = true;
                            }
                        }

                        //怒り時
                        else
                        {
                            //地ならしエフェクト(外側)
                            //エフェクト関連の変数
                            float fAngle = 0.0f;    //角度
                            float fSpeedY = 0.0f;   //速さ
                            float fRadius = ANGER_RANGE_STUN_XZ; //エフェクト半径
                            for (int nCnt = 0; nCnt < COUNT_EFFECT_ITEM; nCnt++)
                            {
                                fSpeedY = float(rand() % 800 + 300) / 100.0f;
                                //0～3.14を出す*(0 or 1)*-1で正負を決める
                                fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                                D3DXVECTOR3 pos = D3DXVECTOR3(g_Boss.pos.x - sinf(fAngle)*fRadius, g_Boss.pos.y
                                    , g_Boss.pos.z - cosf(fAngle)*fRadius);
                                D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                                SetEffect(pos, move, EFFECT_SIZE_EXPLODE_PLAYER, EFFECTTYPE_EXPLOSION); //地ならし
                            }

                            //地ならしエフェクト(内側)
                            fRadius = ANGER_RANGE_STUN_XZ / 3 * 2; //エフェクト半径
                            for (int nCnt = 0; nCnt < COUNT_EFFECT_ITEM; nCnt++)
                            {
                                fSpeedY = float(rand() % 800 + 300) / 100.0f;
                                //0～3.14を出す*(0 or 1)*-1で正負を決める
                                fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                                D3DXVECTOR3 pos = D3DXVECTOR3(g_Boss.pos.x - sinf(fAngle)*fRadius, g_Boss.pos.y
                                    , g_Boss.pos.z - cosf(fAngle)*fRadius);
                                D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                                SetEffect(pos, move, EFFECT_SIZE_EXPLODE_PLAYER, EFFECTTYPE_EXPLOSION); //地ならし
                            }

                            //地ならしで自機をスタンさせる
                            if (g_Boss.fDistance < ANGER_RANGE_STUN_XZ && g_Boss.pos.y <= RANGE_STUN_Y - pPlayer->pos.y)
                            {
                                pPlayer->bStun = true;
                            }
                        }
                    }

                    //攻撃中のカウンター
                    if (g_Boss.nAttackTime > 0)
                    {
                        g_Boss.nAttackTime--;
                    }
                }

                //スタンしたら
                else
                {
                    //スタン明けに、次の行動をさせるために1にする
                    g_Boss.nAttackTime = 1;
                }

                //次の行動(向き固定待機)に切り替える
                if (g_Boss.nAttackTime == 0)
                {
                    //向き固定待機にする
                    g_Boss.state = BOSSSTATE_CHANCE;

                    //向き固定待機時間を設定
                    g_Boss.nChanceTime = CHANCE_TIME_JUMP;
                }

                g_Boss.MotionState = MOTION_JUMP_BOSS;   //ジャンプ

                break;

                //========================
                //ブレス攻撃中
                //========================
            case BOSSSTATE_ATTACK_BREATH:

                //移動量をなくす
                g_Boss.move.x = 0.0f;
                g_Boss.move.z = 0.0f;

                //スタンしていないなら
                if (g_Boss.bStun == false)
                {
                    //溜めている間、自機を見るようにする
                    if (g_Boss.nAttackTime > BREATH_MOMENT)
                    {
                        //自機のほうを徐々に見るようにする
                        g_Boss.rotDest.y = fAngle;

                        // 回転の制限
                        if (g_Boss.rot.y > D3DX_PI)
                        {
                            g_Boss.rot.y -= D3DX_PI * 2;
                        }
                        if (g_Boss.rot.y < -D3DX_PI)
                        {
                            g_Boss.rot.y += D3DX_PI * 2;
                        }

                        g_Boss.rotMin.y = g_Boss.rotDest.y - g_Boss.rot.y;

                        // 目的の角度の回転を制限
                        if (g_Boss.rotMin.y > D3DX_PI)
                        {
                            g_Boss.rotDest.y -= D3DX_PI * 2;
                        }
                        if (g_Boss.rotMin.y < -D3DX_PI)
                        {
                            g_Boss.rotDest.y += D3DX_PI * 2;
                        }

                        // 回転の制限
                        if (g_Boss.rot.y > D3DX_PI)
                        {
                            g_Boss.rot.y -= D3DX_PI * 2;
                        }
                        if (g_Boss.rot.y < -D3DX_PI)
                        {
                            g_Boss.rot.y += D3DX_PI * 2;
                        }

                        if (fabsf(g_Boss.rotMin.y) < 0)
                        {
                            g_Boss.rotMin.y = g_Boss.rotDest.y;
                        }
                        else
                        {
                            //残り体力によって、振り向く速さを変える
                            if (g_Boss.fLife > BOSS_ANGER_LIFE)
                            {
                                g_Boss.rot.y += (g_Boss.rotDest.y - g_Boss.rot.y) * NORMAL_ROT_SPEED;
                            }
                            else if (g_Boss.fLife <= BOSS_ANGER_LIFE && g_Boss.fLife > BOSS_EXANGER_LIFE)
                            {
                                g_Boss.rot.y += (g_Boss.rotDest.y - g_Boss.rot.y) * ANGER_ROT_SPEED;
                            }
                            else if (g_Boss.fLife <= BOSS_EXANGER_LIFE)
                            {
                                g_Boss.rot.y += (g_Boss.rotDest.y - g_Boss.rot.y) * EXANGER_ROT_SPEED;
                            }
                        }
                    }

                    //弾を発射
                    else if (g_Boss.nAttackTime == BREATH_MOMENT)
                    {

                        //通常時
                        if (g_Boss.fLife > BOSS_ANGER_LIFE)
                        {
                            SetBullet(D3DXVECTOR3(g_Boss.modelParts[1].mtxWorld._41, g_Boss.pos.y + BOSS_HEIGHT
                                , g_Boss.modelParts[1].mtxWorld._43), BULLET_SIZE_BOSS,
                                D3DXVECTOR3(-sinf(g_Boss.rot.y)*BULLET_SPEED_BOSS, 0.0f
                                    , -cosf(g_Boss.rot.y)*BULLET_SPEED_BOSS), BULLET_LIFE_BOSS, BULLET_DAMAGE_BOSS, BULLETTYPE_BOSS);
                        }

                        //怒り時
                        else
                        {
                            SetBullet(D3DXVECTOR3(g_Boss.modelParts[1].mtxWorld._41, g_Boss.pos.y + BOSS_HEIGHT
                                , g_Boss.modelParts[1].mtxWorld._43), BULLET_SIZE_BOSS,
                                D3DXVECTOR3(-sinf(g_Boss.rot.y)*BULLET_SPEED_BOSS + 1.5f, 0.0f
                                    , -cosf(g_Boss.rot.y)*BULLET_SPEED_BOSS + 1.5f), BULLET_LIFE_BOSS, BULLET_DAMAGE_BOSS, BULLETTYPE_BOSS);

                            SetBullet(D3DXVECTOR3(g_Boss.modelParts[1].mtxWorld._41, g_Boss.pos.y + BOSS_HEIGHT
                                , g_Boss.modelParts[1].mtxWorld._43), BULLET_SIZE_BOSS,
                                D3DXVECTOR3(-sinf(g_Boss.rot.y - (float)D3DXToRadian(22.5))*BULLET_SPEED_BOSS + 1.5f, 0.0f
                                    , -cosf(g_Boss.rot.y - (float)D3DXToRadian(22.5))*BULLET_SPEED_BOSS + 1.5f), BULLET_LIFE_BOSS, BULLET_DAMAGE_BOSS, BULLETTYPE_BOSS);

                            SetBullet(D3DXVECTOR3(g_Boss.modelParts[1].mtxWorld._41, g_Boss.pos.y + BOSS_HEIGHT
                                , g_Boss.modelParts[1].mtxWorld._43), BULLET_SIZE_BOSS,
                                D3DXVECTOR3(-sinf(g_Boss.rot.y + (float)D3DXToRadian(22.5))*BULLET_SPEED_BOSS + 1.5f, 0.0f
                                    , -cosf(g_Boss.rot.y + (float)D3DXToRadian(22.5))*BULLET_SPEED_BOSS + 1.5f), BULLET_LIFE_BOSS, BULLET_DAMAGE_BOSS, BULLETTYPE_BOSS);
                        }
                    }

                    //発射音
                    else if (g_Boss.nAttackTime == BREATH_MOMENT - 5)
                    {
                        PlaySound(SOUND_LABEL_SE_SHOT); //ショット音
                    }

                    //攻撃中のカウンター
                    if (g_Boss.nAttackTime > 0)
                    {
                        g_Boss.nAttackTime--;
                    }
                }

                //スタンしたら
                else
                {
                    //スタン明けに、次の行動をさせるために1にする
                    g_Boss.nAttackTime = 1;
                }

                //次の行動(向き固定待機)に切り替える
                if (g_Boss.nAttackTime == 0)
                {
                    //移動量をなくす
                    g_Boss.move.x = 0.0f;
                    g_Boss.move.z = 0.0f;

                    //向き固定待機にする
                    g_Boss.state = BOSSSTATE_CHANCE;

                    //向き固定待機時間を設定
                    g_Boss.nChanceTime = CHANCE_TIME_BREATH;
                }

                g_Boss.MotionState = MOTION_BREATH_BOSS;   //銃攻撃

                break;

                //========================
                //向き固定待機
                //========================
            case BOSSSTATE_CHANCE:

                //移動量をなくす
                g_Boss.move.x = 0.0f;
                g_Boss.move.z = 0.0f;

                //スタンしていないなら
                if (g_Boss.bStun == false)
                {
                    //向き固定待機のカウンター
                    if (g_Boss.nChanceTime > 0)
                    {
                        g_Boss.nChanceTime--;
                    }
                }

                //次の行動(向き取得待機)に切り替える
                if (g_Boss.nChanceTime == 0)
                {
                    //向き取得待機にする
                    g_Boss.state = BOSSSTATE_STAY;

                    //残り体力によって、向き取得待機時間を設定
                    if (g_Boss.fLife > BOSS_ANGER_LIFE)
                    {
                        g_Boss.nStayTime = NORMAL_STAY_TIME;
                    }
                    else if (g_Boss.fLife <= BOSS_ANGER_LIFE && g_Boss.fLife > BOSS_EXANGER_LIFE)
                    {
                        g_Boss.nStayTime = ANGER_STAY_TIME;
                    }
                    else if (g_Boss.fLife <= BOSS_EXANGER_LIFE)
                    {
                        g_Boss.nStayTime = EXANGER_STAY_TIME;
                    }
                }

                g_Boss.MotionState = MOTION_IDLE_BOSS;   //待機

                break;
            }
            
            //桃太郎の炎の当たり判定
            if (HitMomoFire(&g_Boss.pos, &g_Boss.posOld, g_Boss.size) == true)
            {
                //炎の強さによって、与えるダメージと怯み値を変える
                switch (pPlayer->nFire)
                {
                case PLAYER_FIRST_FIRE:
                    HitBoss(BOMB_FLINCH, BOMB_DAMAGE);
                    break;

                case PLAYER_FIRST_FIRE + 1:
                    HitBoss(BOMB_FLINCH + 10, BOMB_DAMAGE + 5.0f);
                    break;

                case PLAYER_FIRST_FIRE + 2:
                    HitBoss(BOMB_FLINCH + 20, BOMB_DAMAGE + 10.0f);
                    break;

                case PLAYER_FIRST_FIRE + 3:
                    HitBoss(BOMB_FLINCH + 30, BOMB_DAMAGE + 15.0f);
                    break;

                case PLAYER_FIRST_FIRE + 4:
                    HitBoss(BOMB_FLINCH * 2, BOMB_DAMAGE * 2);
                    break;
                }
            }

            //怯み/スタンの処理
            if (g_Boss.bStun == false && g_Boss.nCounterRigid > BOSS_REACH_FLINCH)
            {
                g_Boss.bStun = true;
                g_Boss.nCntFlinch++;        //怯み回数を加算
            }

            //怯み/スタン中
            else if (g_Boss.bStun == true)
            {
                //スタンのエフェクト
                if (g_Boss.nStunTime == STUN_TIME)
                {
                    SetEffect(D3DXVECTOR3(g_Boss.modelParts[1].mtxWorld._41, BOSS_HEIGHT * 5, g_Boss.modelParts[1].mtxWorld._43)
                        , D3DXVECTOR3(0.0f,0.0f,0.0f), EFFECT_SIZE_STUN, EFFECTTYPE_BOSS_STUN); //ボススタンエフェクト
                }

                g_Boss.nCounterRigid = 0;    //怯み/スタンまでの値をリセット
                g_Boss.nStunTime--;          //怯み/スタン時間

                //怯み
                if (g_Boss.nCntFlinch == 1)
                {
                    g_Boss.MotionState = MOTION_FLINCH_BOSS;   //怯み

                    //後ずさりする
                    g_Boss.move.x = sinf(g_Boss.rot.y)*FLINCH_MOVE;
                    g_Boss.move.z = cosf(g_Boss.rot.y)*FLINCH_MOVE;

                    //怯み解除
                    if (g_Boss.nStunTime < 0)
                    {
                        g_Boss.nStunTime = FLINCH_TIME; //次は怯み
                        g_Boss.bStun = false;
                    }
                }

                //怯み(次スタン)
                else if (g_Boss.nCntFlinch == 2)
                {
                    g_Boss.MotionState = MOTION_FLINCH_BOSS;   //怯み

                    //後ずさりする
                    g_Boss.move.x = sinf(g_Boss.rot.y)*FLINCH_MOVE;
                    g_Boss.move.z = cosf(g_Boss.rot.y)*FLINCH_MOVE;

                    //怯み解除
                    if (g_Boss.nStunTime < 0)
                    {
                        g_Boss.nStunTime = STUN_TIME; //次はスタン
                        g_Boss.bStun = false;
                    }
                }

                //スタン(次は怯み)
                else if (g_Boss.nCntFlinch == 3)
                {
                    g_Boss.MotionState = MOTION_STUN_BOSS;   //スタン

                    //移動量をなくす
                    g_Boss.move.x = 0.0f;
                    g_Boss.move.z = 0.0f;

                    //怯み解除
                    if (g_Boss.nStunTime < 0)
                    {
                        g_Boss.nStunTime = FLINCH_TIME; //次は怯み
                        g_Boss.bStun = false;
                        g_Boss.nCntFlinch = 0;  //怯み回数をリセット
                    }
                }
            }

            //重力
            g_Boss.move.y -= GRAVITY;
            if (g_Boss.pos.y < 0)
            {
                g_Boss.pos.y = 0;
                g_Boss.move.y = 0;
            }

            //ブロックの当たり判定
            CollisionBlock(&g_Boss.pos, &g_Boss.posOld, g_Boss.size);

            //影を設定
            SetPositionShadow(g_Boss.nShadow
                , D3DXVECTOR3(g_Boss.pos.x, 0, g_Boss.pos.z), g_Boss.pos.y);

            //死亡判定(プレイヤーが生存しているなら)
            if (g_Boss.fLife <= 0.0f && pPlayer->bDisp == true)
            {
                //残りの秒数を数える
                g_nTimeScore = g_nTimeMinute * 60 + g_nTimeSecond;

                //難易度によって、入るスコアを変える
                SETTING *pSetting = GetSetting();
                //ノーマル
                if (pSetting->nLevel == SETTINGLEVEL_NORMAL)
                {
                    ScoreCalculation((int)pPlayer->fLife * 30 + g_nTimeScore * 100 + 10000);
                }
                //鬼
                else
                {
                    ScoreCalculation(g_nTimeScore * 100 + 67500);
                }

                //影の削除
                SHADOW *pShadow = GetShadow();//影の情報を取得
                pShadow[g_Boss.nShadow].use = false;

                g_Boss.bUse = false;    //自機を未使用に

                PlaySound(SOUND_LABEL_SE_BOSS_EXPLOSION);   //ボスがやられた時の声

                //爆発エフェクト
                float fSpeedX = 0.0f;   //速さX
                float fSpeedY = 0.0f;   //速さY
                float fSpeedZ = 0.0f;   //速さZ
                int nPlusMinusX = 0;    //符号X
                int nPlusMinusZ = 0;    //符号Z
                for (int nCnt = 0; nCnt < COUNT_EFFECT_EXPLODE_PLAYER * 2; nCnt++)
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
                    SetEffect(g_Boss.pos, move, EFFECT_SIZE_EXPLODE_PLAYER, EFFECTTYPE_EXPLOSION); //爆発エフェクト
                }

                //初期化
                g_Boss.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_Boss.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_Boss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_Boss.fDistance = 9999.9f;

                PlaySound(SOUND_LABEL_SE_FINISH);       //フィニッシュの効果音

                //ゲームクリアの背景を呼び出すための設定
                GAME *pGame = GetGame();
                pGame->winner = SELECT_BOSS;

                SetGameState(GAMESTATE_FINSIH_GAME);    //ゲームクリア 
            }

            //怒り状態で、オーラをまとう
            if (g_Boss.fLife <= BOSS_ANGER_LIFE)
            {
                float fSpeedY = float(rand() % 800 + 300) / 100.0f;
                //0～3.14を出す*(0 or 1)*-1で正負を決める
                float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                float fRadius = float(rand() % 17500 + 0) / 100.0f;
                D3DXVECTOR3 pos = D3DXVECTOR3(g_Boss.pos.x - sinf(fAngle)*fRadius, g_Boss.pos.y + GRID_SIZE * 2, g_Boss.pos.z - cosf(fAngle)*fRadius);
                D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                SetEffect(pos, move, EFFECT_SIZE_EVIL, EFFECTTYPE_EVIL); //殺意の波動
            }

        }
        //モデルのアニメーション
        UpdateBossMotion();
    }
}

//=====================================================
// ボス描画処理
// Author:後藤慎之助
//=====================================================
void DrawBoss(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスへのポインタ
    D3DXMATRIX mtxRot, mtxTrans, mtxParent;
    D3DMATERIAL9 matDef;		// 現在のマテリアル保持用
    D3DXMATERIAL* pMat;			// マテリアルデータへのポインタ

    //ボスが存在していて、透明でないなら描画
    if (g_Boss.bUse == true && g_Boss.bTransparence == false)
    {
        // ワールドマトリックスの初期化
        D3DXMatrixIdentity(&g_Boss.mtxWorld);

        // 向きを反映
        D3DXMatrixRotationYawPitchRoll(&mtxRot,
            g_Boss.rot.y,
            g_Boss.rot.x,
            g_Boss.rot.z);
        D3DXMatrixMultiply(&g_Boss.mtxWorld, &g_Boss.mtxWorld, &mtxRot);

        // 位置を反映
        D3DXMatrixTranslation(&mtxTrans,
            g_Boss.pos.x,
            g_Boss.pos.y,
            g_Boss.pos.z);
        D3DXMatrixMultiply(&g_Boss.mtxWorld, &g_Boss.mtxWorld, &mtxTrans);

        // パーツの設定
        for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
        {
            // ワールドマトリックスの初期化
            D3DXMatrixIdentity(&g_Boss.modelParts[nCntParts].mtxWorld);

            // 向きを反映
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_aBoss[nCntParts].rot.y,
                g_aBoss[nCntParts].rot.x,
                g_aBoss[nCntParts].rot.z);

            D3DXMatrixMultiply(
                &g_Boss.modelParts[nCntParts].mtxWorld,
                &g_Boss.modelParts[nCntParts].mtxWorld, &mtxRot);

            // 位置を反映
            D3DXMatrixTranslation(&mtxTrans,
                g_aBoss[nCntParts].pos.x,
                g_aBoss[nCntParts].pos.y,
                g_aBoss[nCntParts].pos.z);
            D3DXMatrixMultiply(
                &g_Boss.modelParts[nCntParts].mtxWorld,
                &g_Boss.modelParts[nCntParts].mtxWorld, &mtxTrans);

            // テクスチャの設定
            pDevice->SetTexture(0, NULL);

            // 現在のマテリアルを取得する
            pDevice->GetMaterial(&matDef);

            // マテリアルデータへのポインタを取得
            pMat = (D3DXMATERIAL*)g_Boss.modelParts[nCntParts].pBuffMat->GetBufferPointer();

            // 親モデルのインデックスが - 1の時
            if (g_Boss.modelParts[nCntParts].nldxModelParent == -1)
            {
                mtxParent = g_Boss.mtxWorld;		// ボスのマトリクス
            }
            else
            {
                mtxParent = g_Boss.modelParts[g_Boss.modelParts[nCntParts].nldxModelParent].mtxWorld;
            }
            // 親モデルのマトリックスの判定
            D3DXMatrixMultiply(
                &g_Boss.modelParts[nCntParts].mtxWorld,
                &g_Boss.modelParts[nCntParts].mtxWorld, &mtxParent);

            // ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_Boss.modelParts[nCntParts].mtxWorld);

            for (int nCntMat = 0; nCntMat < (int)g_Boss.modelParts[nCntParts].nNumMat; nCntMat++)
            {
                // マテリアルの設定
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                g_Boss.modelParts[nCntParts].pMesh->DrawSubset(nCntMat);
            }

            // 保持していたマテリアルを戻す
            pDevice->SetMaterial(&matDef);
        }
    }
}

//=============================================================================
// ボスの取得
// Author:後藤慎之助
//=============================================================================
BOSS*GetBoss(void)
{
    return &g_Boss;
}

//=============================================================================
// ボスのダメージ設定
// Author:後藤慎之助
//=============================================================================
void HitBoss(int nCntFlinch, float fDamage)
{
    //無敵時間でないなら
    if (g_Boss.nCntInvincibleFrame == 0)
    {
        GAME *pGame = GetGame();

        //ゲームが終わった後、ダメージを受けない
        if (pGame->state == GAMESTATE_NORMAL)
        {
            //残り体力によって、スタン値の上昇を設定
            if (g_Boss.fLife > BOSS_ANGER_LIFE)
            {
                g_Boss.nCounterRigid += nCntFlinch; //スタン値をあげる
            }
            //残り体力半分から、スタン値があがりずらくなる
            else
            {
                g_Boss.nCounterRigid += nCntFlinch / 2; //スタン値をあげる
            }

            //残り体力によって、受けるダメージを設定
            if (g_Boss.fLife > BOSS_EXANGER_LIFE)
            {
                //ダメージ計算
                g_Boss.fLife -= nCntFlinch;
            }
            //残り体力4分の1から、硬化する
            else
            {
                //ダメージ計算
                g_Boss.fLife -= nCntFlinch / 2;
            }

            //無敵時間をつくる
            g_Boss.nCntInvincibleFrame = ENEMY_INVINCIBLE_FRAME;
        }
    }
}

//=====================================================
// ボスの衝突判定の設定
// Author:後藤慎之助
//=====================================================
bool CollisionBoss(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, bool bPlayer)
{
    bool bCollision = false;  //衝突したかどうか

    // ボスが使われたら
    if (g_Boss.bUse == true)
    {
        //当たり判定を計算
        if (RectangleCollision(pPos, pPosOld, size, &g_Boss.pos, g_Boss.size) == true)
        {
            bCollision = true;    //衝突

            //プレイヤーなら
            if (bPlayer == true)
            {
                //ノックバック
                PLAYER *pPlayer = GetPlayer();
                if (pPlayer->nCntInvincibleFrame == 0)  //無敵でないなら
                {
                    float fKnockback = VALUE_KNOCKBACK;
                    pPlayer->move.x = pPlayer->pos.x - g_Boss.posOld.x;
                    pPlayer->move.z = pPlayer->pos.z - g_Boss.posOld.z;
                    D3DXVec3Normalize(&pPlayer->move, &pPlayer->move);  //ベクトルを正規化
                    pPlayer->move.x = pPlayer->move.x*fKnockback;
                    pPlayer->move.z = pPlayer->move.z*fKnockback;
                }
            }
        }
    }

    return bCollision;    //衝突したかどうかを返す
}

//=============================================================================
// アニメーションロード
// Author:小西 優斗
//=============================================================================
void LoadBossAnimation(void)
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
    pFile = fopen("TXT/motionboss.txt", "r");

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
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aBossMotion[nCntMotion].nLoop);
                        }

                        if (strcmp(cHeadText, "NUM_KEY") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aBossMotion[nCntMotion].nNum_Key);
                        }

                        //			if (strcmp(cHeadText, "COLLISION") == 0)
                        //			{
                        //				sscanf(cReedText, "%s %s %d %f %f %f %f %d %d", &cDie, &cDie, &g_BossCollision.nPartsInfo, &g_BossCollision.CollisionPos.x, &g_BossCollision.CollisionPos.y, &g_BossCollision.CollisionPos.z, &g_BossCollision.fLength, &g_BossCollision.nFirstFlame[nCntMotion], &g_BossCollision.nLastFlame);
                        //			}
                        if (strcmp(cHeadText, "KEYSET") == 0)
                        {

                            while (strcmp(cHeadText, "END_KEYSET") != 0)
                            {

                                fgets(cReedText, sizeof(cReedText), pFile);
                                sscanf(cReedText, "%s", &cHeadText);

                                if (strcmp(cHeadText, "FRAME") == 0)
                                {
                                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aBossMotion[nCntMotion].KeyInfo[nCntKey].nFrame);
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
                                                &g_aBossMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].x,
                                                &g_aBossMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].y,
                                                &g_aBossMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].z);
                                        }

                                        if (strcmp(cHeadText, "ROT") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &g_aBossMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].x,
                                                &g_aBossMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].y,
                                                &g_aBossMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].z);
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
// Author:小西 優斗
//=============================================================================
void LoadBoss(void)
{
    // ファイルポイント
    FILE *pFile;

    // 変数宣言
    int  nCntLoad = 0;
    char cReedText[128];	// 文字として読み取り用
    char cHeadText[256];	//
    char cDie[128];

    // ファイル開
    pFile = fopen("TXT/boss.txt", "r");

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
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aBoss[nCntLoad].nData);
                        }

                        if (strcmp(cHeadText, "PARENT") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aBoss[nCntLoad].nParents);
                        }

                        if (strcmp(cHeadText, "POS") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                &g_aBoss[nCntLoad].posOrigin.x,
                                &g_aBoss[nCntLoad].posOrigin.y,
                                &g_aBoss[nCntLoad].posOrigin.z);

                            g_aBoss[nCntLoad].pos = g_aBoss[nCntLoad].posOrigin;
                        }

                        if (strcmp(cHeadText, "ROT") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                &g_aBoss[nCntLoad].rot.x,
                                &g_aBoss[nCntLoad].rot.y,
                                &g_aBoss[nCntLoad].rot.z);
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
// Author:小西 優斗
//=============================================================================
void UpdateBossMotion(void)
{
    //モーションが変わったとき
    if (g_MotionOldStateBoss != g_Boss.MotionState) {
        g_nFremeBoss = 0;
        g_nCntKeyBoss = 0;
    }
    for (int nCntMotion = 0; nCntMotion < MAX_BOSS_PARTS; nCntMotion++)
    {
        if (g_nFremeBoss == 0)
        {
            g_NumPosBoss[nCntMotion].x = ((g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].pos[nCntMotion].x -
                (g_aBoss[nCntMotion].pos.x - g_aBoss[nCntMotion].posOrigin.x)) / float(g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].nFrame));

            g_NumPosBoss[nCntMotion].y = ((g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].pos[nCntMotion].y -
                (g_aBoss[nCntMotion].pos.y - g_aBoss[nCntMotion].posOrigin.y)) / float(g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].nFrame));

            g_NumPosBoss[nCntMotion].z = ((g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].pos[nCntMotion].z -
                (g_aBoss[nCntMotion].pos.z - g_aBoss[nCntMotion].posOrigin.z)) / float(g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].nFrame));

            g_NumRotBossDest[nCntMotion] =
                g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].rot[nCntMotion] - g_aBoss[nCntMotion].rot;

            g_NumRotBoss[nCntMotion] = (g_NumRotBossDest[nCntMotion] / float(g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].nFrame));
        }

        g_aBoss[nCntMotion].pos += g_NumPosBoss[nCntMotion];
        g_aBoss[nCntMotion].rot += g_NumRotBoss[nCntMotion];
    }
    g_MotionOldStateBoss = g_Boss.MotionState;
    // ループするとき
    if (g_nFremeBoss >= g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].nFrame && g_aBossMotion[g_Boss.MotionState].nLoop == 1)
    {
        g_nFremeBoss = 0;
        g_nCntKeyBoss++;

        if (g_nCntKeyBoss >= g_aBossMotion[g_Boss.MotionState].nNum_Key)
        {
            g_nCntKeyBoss = 0;
        }

    }
    // ループしないとき
    else if (g_nFremeBoss >= g_aBossMotion[g_Boss.MotionState].KeyInfo[g_nCntKeyBoss].nFrame && g_aBossMotion[g_Boss.MotionState].nLoop == 0)
    {
        g_nFremeBoss = 0;
        g_nCntKeyBoss++;
        if (g_nCntKeyBoss >= g_aBossMotion[g_Boss.MotionState].nNum_Key)
        {
            for (int nCntMotion = 0; nCntMotion < MAX_BOSS_PARTS; nCntMotion++)
            {
                g_NumPosBoss[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_NumRotBoss[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
            g_Boss.MotionState = MOTION_IDLE_BOSS;
            g_nCntKeyBoss = 0;
        }
    }
    else
    {
        g_nFremeBoss++;
    }
}