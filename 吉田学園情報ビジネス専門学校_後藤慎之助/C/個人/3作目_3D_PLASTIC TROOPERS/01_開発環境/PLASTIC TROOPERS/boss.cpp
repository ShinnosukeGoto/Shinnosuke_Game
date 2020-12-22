//=====================================================
//
// 3Dゲームのひな形の処理[boss.h]
// Author:後藤慎之助
//
//=====================================================

//=====================================================
// インクルードファイル
//=====================================================
#include "boss.h"
#include "camera.h"
#include "sound.h"
#include "effect2.h"
#include "collision.h"
#include "campaign.h"
#include "bullet.h"
#include "player.h"
#include "camera.h"
#include "score.h"
#include "time.h"

//=====================================================
// グローバル変数定義
//=====================================================
BOSS g_Boss;        //ボスの情報
int g_ShadowIndexBossBoss;
int g_RandomAttackBoss = 1;				//弾をランダムに打つときに使用
int g_nTimeScore;				//残り時間に対応したスコア

int g_FrameCountBoss/* = 0*/;				// 現在のフレーム数

int g_PlayKeySetIndexBoss/* = 0*/;			// 再生中のキーセットインデックス

int g_BossCurrentAnimation/* = MOTION_IDLE*/;	// 再生中のアニメーション

int g_nFremeBoss/* = 0*/;					// フレーム数

int g_nCntKeyBoss/* = 1*/;					// キー数

int g_nChangeFlameBoss/* = 20*/;			// モーションのフレーム

MOTIONSTATE_BOSS g_MotionStateBoss;			// モーションの構造体
MOTIONSTATE_BOSS g_MotionOldStateBoss;		// 1F前のモーションの構造体

D3DXVECTOR3 g_NumPosBoss[MAX_BOSS_PARTS];		// posの差分
D3DXVECTOR3 g_NumRotBoss[MAX_BOSS_PARTS];		// rotの差分
D3DXVECTOR3 g_NumRotBossDest[MAX_BOSS_PARTS];	// rotの差分

ANIMATION_BOSS g_aBossMotion[MOTION_MAX_BOSS];	// モーションの情報
                                    // モデルパーツのデータ
ModelParts_BOSS g_aBoss[MAX_BOSS_PARTS] =
{
    { "MODEL/00_karada_mae.x" },
    { "MODEL/01_karada_usiro.x" },
    { "MODEL/02_atama.x" },
    { "MODEL/03_L_foot.x" },
    { "MODEL/04_L_sune.x" },
    { "MODEL/05_L_leg.x" },
    { "MODEL/06_R_foot.x" },
    { "MODEL/07_R_sune.x" },
    { "MODEL/08_R_leg.x" },
    { "MODEL/09_L_ude.x" },
    { "MODEL/10_R_ude.x" },
    { "MODEL/11_L_wing.x" },
    { "MODEL/12_R_wing.x" },
    { "MODEL/13_tail.x" },
};

//=====================================================
// ボス初期化処理
//=====================================================
void InitBoss(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //グローバル変数の初期化
    g_MotionStateBoss = MOTION_IDLE_BOSS;
    g_nTimeScore = 0;

    g_FrameCountBoss = 0;				// 現在のフレーム数

    g_PlayKeySetIndexBoss = 0;			// 再生中のキーセットインデックス

    g_BossCurrentAnimation = MOTION_IDLE_BOSS;	// 再生中のアニメーション

    g_nFremeBoss = 0;					// フレーム数

    g_nCntKeyBoss = 1;					// キー数

    g_nChangeFlameBoss = 20;			// モーションのフレーム

                                    //構造体の初期化
    g_Boss.pos = D3DXVECTOR3(0.0f, 0.0f, BOSS_FIRST_POS_Z);		// 初期位置
    g_Boss.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期位置
    g_Boss.size = BOSS_SIZE;
    g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 初期向き
    g_Boss.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期向き
    g_Boss.rotMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期向き
    g_Boss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 初期移動量
    g_Boss.bUse = true;
    g_Boss.bStun = false;
    g_Boss.state = BOSSSTATE_STAY;
    g_Boss.nCounterRigid = 0;
    g_Boss.nStayTime = NORMAL_STAY_TIME;
    g_Boss.nAttackTime = 0;
    g_Boss.nChanceTime = 0;
    g_Boss.nCntFlinch = 0;
    g_Boss.nStunTime = FLINCH_TIME;
    g_Boss.fDistance = 9999.0f;
    g_Boss.fLife = BOSS_LIFE;	//自機の体力

    //影の設定
    //g_ShadowIndexBoss = SetShadow(g_Boss.pos, g_Boss.rot);

    LoadBoss();
    LoadBossAnimation();

    // モデルパーツの初期化
    for (int nCntParts = 0; nCntParts < MAX_BOSS_PARTS; nCntParts++)
    {
        //		g_Boss.modelParts[nCntParts].pos = g_aBoss[nCntParts].pos;			// 初期位置
        //		g_Boss.modelParts[nCntParts].rot = g_aBoss[nCntParts].rot;			// 初期角度

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
//=====================================================
void UpdateBoss(void)
{
    //ボスが生存しているなら
    if (g_Boss.bUse == true)
    {
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
        float fAdjustment = 0.0f;  //符号調整
        float fDistanceY = 0.0f;   //自機と敵の高さの差
        float fKnockback = KNOCKBACK_SPIN;  //スピン攻撃のノックバック
        int nRandAttack = 0;        //ランダムな攻撃にする
        float fCircle = 0.0f;    //円状にする
        float fSpeed = 0.0f;   //速さ
        float fRadius = 0.0f; //エフェクト発生半径

        //高さ調整
        if (pPlayer->pos.y < g_Boss.pos.y + BOSS_HEIGHT)
        {
            fAdjustment = -1.0f;
            fDistanceY = g_Boss.pos.y + BOSS_HEIGHT - pPlayer->pos.y;
        }
        else if (pPlayer->pos.y == g_Boss.pos.y + BOSS_HEIGHT)
        {
            fAdjustment = 0.0f;
            fDistanceY = 0.0f;
        }
        else if (pPlayer->pos.y > g_Boss.pos.y + BOSS_HEIGHT)
        {
            fAdjustment = 1.0f;
            fDistanceY = pPlayer->pos.y - g_Boss.pos.y + BOSS_HEIGHT;
        }

        //位置を移動量と紐づけ
        g_Boss.pos += g_Boss.move;

        //ボス戦なら
        if (pCamera->bBossMode == true)
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
                            //バレット
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_BULLET;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_BULLET;
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
                                //スピン
                                //攻撃に切り替える
                                g_Boss.state = BOSSSTATE_ATTACK_SPIN;
                                //攻撃フレームを設定
                                g_Boss.nAttackTime = ATTACK_TIME_SPIN;
                            }

                        }

                        //60％
                        else if (nRandAttack > 40)
                        {
                            //通常時
                            if (g_Boss.fLife > BOSS_EXANGER_LIFE)
                            {
                                //スピン
                                //攻撃に切り替える
                                g_Boss.state = BOSSSTATE_ATTACK_SPIN;
                                //攻撃フレームを設定
                                g_Boss.nAttackTime = ATTACK_TIME_SPIN;
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
                        //5%
                        if (nRandAttack <= 5)
                        {
                            //スピン
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_SPIN;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_SPIN;
                        }

                        //20%
                        else if (nRandAttack > 5 && nRandAttack <= 25)
                        {
                            //バレット
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_BULLET;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_BULLET;
                        }

                        //75％
                        else if (nRandAttack > 25)
                        {
                            //タックル
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_TACKLE;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_TACKLE;
                        }
                    }

                    //遠距離の時
                    else if(g_Boss.fDistance >= BOSS_CONSIDER_MEDIUM)
                    {
                        //5%
                        if (nRandAttack <= 5)
                        {
                            //スピン
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_SPIN;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_SPIN;
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
                            //バレット
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_BULLET;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_BULLET;
                        }
                    }

                }

                g_MotionStateBoss = MOTION_IDLE_BOSS;   //待機

                break;

                //========================
                //突進攻撃中
                //========================
            case BOSSSTATE_ATTACK_TACKLE:

                //スタンしていないなら
                if (g_Boss.bStun == false)
                {
                    //残り体力によって、追従かどうかを変える
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
                            //自機を追従
                            g_Boss.move.x = -sinf(fAngle)*ANGER_TACKLE_SPEED;
                            g_Boss.move.z = -cosf(fAngle)*ANGER_TACKLE_SPEED;
                        }
                        //激昂時
                        else
                        {
                            //自機を追いかける
                            g_Boss.move.x = -sinf(g_Boss.rot.y)*EXANGER_TACKLE_SPEED;
                            g_Boss.move.z = -cosf(g_Boss.rot.y)*EXANGER_TACKLE_SPEED;
                        }

                        //自機のほうを見る
                        g_Boss.rot.y = fAngle;
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
                        //距離が近いなら、スピンに派生
                        if (g_Boss.fDistance < RANGE_SPIN_XZ)
                        {
                            //攻撃に切り替える
                            g_Boss.state = BOSSSTATE_ATTACK_SPIN;
                            //攻撃フレームを設定
                            g_Boss.nAttackTime = ATTACK_TIME_SPIN;
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

                g_MotionStateBoss = MOTION_WALK_BOSS;   //歩き

                break;

                //========================
                //スピン攻撃中
                //========================
            case BOSSSTATE_ATTACK_SPIN:

                //移動量をなくす
                g_Boss.move.x = 0.0f;
                g_Boss.move.z = 0.0f;

                //スタンしていないなら
                if (g_Boss.bStun == false)
                {
                    //溜めが終わった後、一回転させる
                    if (g_Boss.nAttackTime < CHARGE_SPIN)
                    {
                        g_Boss.rot.y += D3DX_PI / 10.0f;    //1回転させる
                    }

                    //攻撃判定発生
                    if (g_Boss.nAttackTime == 10)
                    {
                        if (g_Boss.fDistance < RANGE_SPIN_XZ && g_Boss.pos.y <= RANGE_SPIN_Y - pPlayer->pos.y)
                        {
                            //必殺中は無敵
                            if (pPlayer->bChanceAttack == false)
                            {
                                //通常時
                                if (g_Boss.fLife > BOSS_ANGER_LIFE)
                                {
                                    HitPlayer(50, DAMAGE_SPIN);
                                }
                                //怒り時
                                else
                                {
                                    HitPlayer(50, DAMAGE_SPIN*1.5f);
                                }     

                                //ノックバック
                                pPlayer->move.x = pPlayer->pos.x - g_Boss.posOld.x;
                                pPlayer->move.z = pPlayer->pos.z - g_Boss.posOld.z;
                                D3DXVec3Normalize(&pPlayer->move, &pPlayer->move);  //ベクトルを正規化
                                pPlayer->move.x = pPlayer->move.x*fKnockback;
                                pPlayer->move.z = pPlayer->move.z*fKnockback;

                                //Y軸上昇
                                pPlayer->move.y += UPPER_SPIN;    //上昇

                                                                  //ヒットエフェクト
                                float fAngle = 0.0f;    //角度
                                float fSpeed = 10.0f;   //速さ
                                CAMERA*pCamera = GetCamera();
                                for (int nCnt = 0; nCnt < 50; nCnt++)
                                {
                                    //0～3.14を出す*(0 or 1)*-1で正負を決める
                                    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                                    D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->rot.y)*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->rot.y)*fSpeed);
                                    SetEffect2(pPlayer->pos, move, EFFECT2_SIZE_HIT, EFFECT2_INIT_COLOR_HIT, EFFECT2TYPE_HIT);
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
                    //スタン明けに、次の行動をさせるために1にする
                    g_Boss.nAttackTime = 1;
                }

                //次の行動(向き固定待機)に切り替える
                if (g_Boss.nAttackTime == 0)
                {
                    //向き固定待機にする
                    g_Boss.state = BOSSSTATE_CHANCE;

                    //向き固定待機時間を設定
                    g_Boss.nChanceTime = CHANCE_TIME_SPIN;
                }

                g_MotionStateBoss = MOTION_ATTACK_SPIN_BOSS;   //スピン

                break;

                //========================
                //バレット攻撃中
                //========================
            case BOSSSTATE_ATTACK_BULLET:

                //移動量をなくす
                g_Boss.move.x = 0.0f;
                g_Boss.move.z = 0.0f;

                //スタンしていないなら
                if (g_Boss.bStun == false)
                {
                    //溜めている間、自機を見るようにする
                    if (g_Boss.nAttackTime > BULLET_MOMENT)
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
                    else if (g_Boss.nAttackTime == BULLET_MOMENT)
                    {
                        //通常時
                        if (g_Boss.fLife > BOSS_ANGER_LIFE)
                        {
                            SetBullet(D3DXVECTOR3(g_Boss.pos.x, g_Boss.pos.y + BOSS_HEIGHT
                                , g_Boss.pos.z), BULLET_SIZE_BOSS,
                                D3DXVECTOR3(-sinf(g_Boss.rot.y)*BULLET_SPEED_BOSS, fDistanceY / BULLET_SMASH * fAdjustment
                                    , -cosf(g_Boss.rot.y)*BULLET_SPEED_BOSS), 80, BULLET_RIGID, BULLET_DAMAGE_BOSS, BULLETTYPE_BOSS);
                        }

                        //怒り時
                        else
                        {

                            for (int nCnt = 0; nCnt < 350; nCnt++)
                            {
                                fSpeed = float(rand() % 2000 + 1000) / 100.0f;
                                //0～3.14を出す*(0 or 1)*-1で正負を決める
                                fCircle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                                fRadius = float(rand() % 8000 + 0) / 100.0f;
                                D3DXVECTOR3 pos = D3DXVECTOR3(g_Boss.pos.x - sinf(fCircle + pCamera->rot.y)*fRadius
                                    , g_Boss.pos.y + BOSS_HEIGHT / 2 + cosf(fCircle)*fRadius
                                    , g_Boss.pos.z - cosf(fCircle + pCamera->rot.y)*fRadius);
                                D3DXVECTOR3 move = D3DXVECTOR3(-sinf(g_Boss.rot.y)*fSpeed, 0.0f, -cosf(g_Boss.rot.y)*fSpeed);
                                SetEffect2(pos, move, EFFECT2_SIZE_HEAL, EFFECT2_INIT_COLOR_HEAL, EFFECT2TYPE_FIRE2); //炎エフェクト
                            }
                        }

                    }

                    //後ずさり
                    else if (g_Boss.nAttackTime < BULLET_MOMENT)
                    {
                        //後ずさりする
                        g_Boss.move.x = sinf(g_Boss.rot.y)*FLINCH_MOVE;
                        g_Boss.move.z = cosf(g_Boss.rot.y)*FLINCH_MOVE;
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
                    g_Boss.nChanceTime = CHANCE_TIME_BULLET;
                }

                g_MotionStateBoss = MOTION_ATTACK_BULLET_BOSS;   //銃攻撃

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

                g_MotionStateBoss = MOTION_IDLE_BOSS;   //待機

                break;
            }

        }

        //弾の当たり判定
        CollisionBulletBoss(&g_Boss.pos, &g_Boss.posOld,
            D3DXVECTOR3(g_Boss.size.x, g_Boss.size.y * 2, g_Boss.size.z));

        //弾の当たり判定
        CollisionEffectBoss(&g_Boss.pos, &g_Boss.posOld,
            D3DXVECTOR3(g_Boss.size.x, g_Boss.size.y * 2, g_Boss.size.z));

        //怯み/スタンの処理
        if (g_Boss.bStun == false && g_Boss.nCounterRigid > BOSS_REACH_RIGID)
        {
            g_Boss.bStun = true;
            g_Boss.nCntFlinch++;        //怯み回数を加算
        }

        //怯み/スタン中
        else if (g_Boss.bStun == true)
        {
            g_Boss.nCounterRigid = 0;    //怯み/スタンまでの値をリセット
            g_Boss.nStunTime--;         //怯み/スタン時間

            //怯み
            if (g_Boss.nCntFlinch == 1)
            {
                g_MotionStateBoss = MOTION_FLINCH_BOSS;   //怯み

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
                g_MotionStateBoss = MOTION_FLINCH_BOSS;   //怯み

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
                g_MotionStateBoss = MOTION_STUN_BOSS;   //スタン

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

        //死亡判定
        if (g_Boss.fLife <= 0.0f)
        {
            g_nTimeScore = GetTime();

            ScoreCalculation((int)pPlayer->fLife * 30 + g_nTimeScore * 100 + 10000);

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
                SetEffect2(g_Boss.pos, move, EFFECT2_SIZE_EXPLOSION, EFFECT2_INIT_COLOR_EXPLOSION2, EFFECT2TYPE_EXPLOSION); //爆発エフェクト
            }

            g_Boss.bUse = false;    //自機を未使用に

            PlaySound(SOUND_LABEL_SE_VOICE);

            SetCampaignState(CAMPAIGNSTATE_GAMEOVER);    //ゲームクリア画面へ     
        }

        //モデルのアニメーション
        UpdateBossMotion();
    }
}

//=====================================================
// ボス描画処理
//=====================================================
void DrawBoss(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスへのポインタ
    D3DXMATRIX mtxRot, mtxTrans, mtxParent;
    D3DMATERIAL9 matDef;		// 現在のマテリアル保持用
    D3DXMATERIAL* pMat;			// マテリアルデータへのポインタ

    //ボスが存在しているなら描画
    if (g_Boss.bUse == true)
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
//=============================================================================
BOSS*GetBoss(void)
{
    return &g_Boss;
}

//=============================================================================
// ボスのダメージ設定
//=============================================================================
void HitBoss(int nCntRigid, float fDamage)
{
    //残り体力によって、スタン値の上昇を設定
    if (g_Boss.fLife > BOSS_ANGER_LIFE)
    {
        g_Boss.nCounterRigid += nCntRigid; //スタン値をあげる
    }
    //残り体力半分から、スタン値があがりずらくなる
    else
    {
        g_Boss.nCounterRigid += nCntRigid / 2; //スタン値をあげる
    }

    //残り体力によって、受けるダメージを設定
    if (g_Boss.fLife > BOSS_EXANGER_LIFE)
    {
        //ダメージ計算
        g_Boss.fLife -= fDamage;
    }
    //残り体力4分の1から、硬化する
    else
    {
        //ダメージ計算
        g_Boss.fLife -= fDamage / 2;
    }

    //必殺ゲージ上昇
    PLAYER*pPlayer = GetPlayer();
    if (pPlayer->bChanceAttack == false)    //必殺攻撃中以外で
    {
        //銃の場合
        if (pPlayer->bBullet == true)
        {
            pPlayer->fChanceMeter += HIT_BULLET_CHARGE;
        }

        //剣の場合
        else
        {
            pPlayer->fChanceMeter += HIT_SWORD_CHARGE;
        }
    }
}

//=====================================================
// 当たり判定の設定
//=====================================================
bool CollisionBoss(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bHit = false;  //当たったかどうか

    //ボス使われたら
    if (g_Boss.bUse == true)
    {

        //当たり判定を計算
        if (RectangleCollision(pPos, pPosOld, size, &g_Boss.pos, g_Boss.size) == true)
        {
            PLAYER*pPlayer = GetPlayer();

            //必殺中は無敵
            if (pPlayer->bChanceAttack == false)
            {

                HitPlayer(5, HIT_BOSS_DAMAGE);

                //ノックバック
                float fKnockback = VALUE_KNOCKBACK;
                pPlayer->move.x = pPlayer->pos.x - g_Boss.posOld.x;
                pPlayer->move.z = pPlayer->pos.z - g_Boss.posOld.z;
                D3DXVec3Normalize(&pPlayer->move, &pPlayer->move);  //ベクトルを正規化
                pPlayer->move.x = pPlayer->move.x*fKnockback;
                pPlayer->move.z = pPlayer->move.z*fKnockback;

                //ヒットエフェクト
                float fAngle = 0.0f;    //角度
                float fSpeed = 10.0f;   //速さ
                CAMERA*pCamera = GetCamera();
                for (int nCnt = 0; nCnt < 50; nCnt++)
                {
                    //0～3.14を出す*(0 or 1)*-1で正負を決める
                    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                    D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->rot.y)*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->rot.y)*fSpeed);
                    SetEffect2(pPlayer->pos, move, EFFECT2_SIZE_HIT, EFFECT2_INIT_COLOR_HIT, EFFECT2TYPE_HIT);
                }
            }

            bHit = true;    //当たった
        }
    }

    return bHit;    //当たったかどうかを返す
}

//=============================================================================
// アニメーションロード
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
    pFile = fopen("motionboss.txt", "r");

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
    pFile = fopen("boss.txt", "r");

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
//=============================================================================
void UpdateBossMotion(void)
{
    //モーションが変わったとき
    if (g_MotionOldStateBoss != g_MotionStateBoss) {
        g_nFremeBoss = 0;
        g_nCntKeyBoss = 0;
    }
    for (int nCntMotion = 0; nCntMotion < MAX_BOSS_PARTS; nCntMotion++)
    {
        if (g_nFremeBoss == 0)
        {
            g_NumPosBoss[nCntMotion].x = ((g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].pos[nCntMotion].x -
                (g_aBoss[nCntMotion].pos.x - g_aBoss[nCntMotion].posOrigin.x)) / float(g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].nFrame));

            g_NumPosBoss[nCntMotion].y = ((g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].pos[nCntMotion].y -
                (g_aBoss[nCntMotion].pos.y - g_aBoss[nCntMotion].posOrigin.y)) / float(g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].nFrame));

            g_NumPosBoss[nCntMotion].z = ((g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].pos[nCntMotion].z -
                (g_aBoss[nCntMotion].pos.z - g_aBoss[nCntMotion].posOrigin.z)) / float(g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].nFrame));

            g_NumRotBossDest[nCntMotion] =
                g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].rot[nCntMotion] - g_aBoss[nCntMotion].rot;

            g_NumRotBoss[nCntMotion] = (g_NumRotBossDest[nCntMotion] / float(g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].nFrame));
        }

        g_aBoss[nCntMotion].pos += g_NumPosBoss[nCntMotion];
        g_aBoss[nCntMotion].rot += g_NumRotBoss[nCntMotion];
    }
    g_MotionOldStateBoss = g_MotionStateBoss;
    // ループするとき
    if (g_nFremeBoss >= g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].nFrame && g_aBossMotion[g_MotionStateBoss].nLoop == 1)
    {
        g_nFremeBoss = 0;
        g_nCntKeyBoss++;

        if (g_nCntKeyBoss >= g_aBossMotion[g_MotionStateBoss].nNum_Key)
        {
            g_nCntKeyBoss = 0;
        }

    }
    // ループしないとき
    else if (g_nFremeBoss >= g_aBossMotion[g_MotionStateBoss].KeyInfo[g_nCntKeyBoss].nFrame && g_aBossMotion[g_MotionStateBoss].nLoop == 0)
    {
        g_nFremeBoss = 0;
        g_nCntKeyBoss++;
        if (g_nCntKeyBoss >= g_aBossMotion[g_MotionStateBoss].nNum_Key)
        {
            for (int nCntMotion = 0; nCntMotion < MAX_BOSS_PARTS; nCntMotion++)
            {
                g_NumPosBoss[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_NumRotBoss[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
            g_MotionStateBoss = MOTION_IDLE_BOSS;
            g_nCntKeyBoss = 0;
        }
    }
    else
    {
        g_nFremeBoss++;
    }
}