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
#include "shadow.h"
#include "enemy.h"
#include "bullet.h"
#include "score.h"
#include "campaign.h"
#include "effect.h"
#include "effect2.h"
#include "collision.h"
#include "wall.h"
#include "block.h"
#include "meshfield.h"
#include "item.h"
#include "enemylife.h"
#include "sound.h"

//=====================================================
// グローバル変数定義
//=====================================================
LPD3DXMESH g_pMeshEnemy[ENEMYTYPE_MAX] = {};		// メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatEnemy[ENEMYTYPE_MAX] = {};// マテリアル情報へのポインタ
DWORD g_nNumMatEnemy[ENEMYTYPE_MAX] = {};			// マテリアル情報の数

ENEMY g_Enemy[MAX_ENEMY];	        //エネミーの情報
int g_ShadowIndexEnemy;
int g_RandomAttack = 1;				//弾をランダムに打つときに使用

int g_nCntPhase1;                   //第1フェーズ
int g_nCntPhase2;                   //第2フェーズ
int g_nCntPhase3;                   //第3フェーズ
int g_nCntPhase4;                   //第4フェーズ

//=====================================================
// モデル初期化処理
//=====================================================
void InitEnemy(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    // Xファイルの読み込み
    //キャノン
    D3DXLoadMeshFromX("MODEL/enemy000.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_000],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_000],
        &g_pMeshEnemy[ENEMYTYPE_000]);

    //スパイダー
    D3DXLoadMeshFromX("MODEL/enemy001.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_001],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_001],
        &g_pMeshEnemy[ENEMYTYPE_001]);

    //バタフライ
    D3DXLoadMeshFromX("MODEL/enemy002.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_002],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_002],
        &g_pMeshEnemy[ENEMYTYPE_002]);

    //ゴールデンスパイダー
    D3DXLoadMeshFromX("MODEL/enemy003.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_003],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_003],
        &g_pMeshEnemy[ENEMYTYPE_003]);

    //ジャンプボックス
    D3DXLoadMeshFromX("MODEL/enemy004.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_004],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_004],
        &g_pMeshEnemy[ENEMYTYPE_004]);

    //フィッシュ
    D3DXLoadMeshFromX("MODEL/enemy005.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_005],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_005],
        &g_pMeshEnemy[ENEMYTYPE_005]);

    //グローバル変数の初期化
    g_nCntPhase1 = 1;   //第1フェーズのキーは、キャノン
    g_nCntPhase2 = 1;   //第2フェーズのキーは、バタフライ
    g_nCntPhase3 = 2;   //第3フェーズのキーは、フィッシュ×2

    //敵の情報の初期化
    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        g_Enemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].posMemory = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].rotMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
        g_Enemy[nCntEnemy].type = ENEMYTYPE_000;
        g_Enemy[nCntEnemy].nCounterState = 0;
        g_Enemy[nCntEnemy].nPosX = 0;
        g_Enemy[nCntEnemy].nPosZ = 0;
        g_Enemy[nCntEnemy].nCounterAnim = 0;
        g_Enemy[nCntEnemy].nPatternAnim = 0;
        g_Enemy[nCntEnemy].bUse = false;
        g_Enemy[nCntEnemy].fLife = 0;
        g_Enemy[nCntEnemy].nAttackTime = 0;
        g_Enemy[nCntEnemy].fDistance = 9999.9f;
        g_Enemy[nCntEnemy].nRank = 0;
        g_Enemy[nCntEnemy].bFollow = false;
    }

    //SetShadow(g_aEnemy.pos/* + ENEMYSHADOW_POSITION*/, g_aEnemy.rot);
    //g_ShadowIndex = SetShadow(g_aEnemy.pos, g_aEnemy.rot);
}

//=====================================================
// エネミー終了処理
//=====================================================
void UninitEnemy(void)
{
    // メッシュの破棄
    for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
    {
        if (g_pMeshEnemy[nCntEnemy] != NULL)
        {
            g_pMeshEnemy[nCntEnemy]->Release();
            g_pMeshEnemy[nCntEnemy] = NULL;
        }
        // マテリアルの破棄
        if (g_pBuffMatEnemy[nCntEnemy] != NULL)
        {
            g_pBuffMatEnemy[nCntEnemy]->Release();
            g_pBuffMatEnemy[nCntEnemy] = NULL;
        }
    }
}

//=====================================================
// エネミー更新処理
//=====================================================
void UpdateEnemy(void)
{
    PLAYER*pPlayer;

    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        //ランク初期化
        g_Enemy[nCntEnemy].nRank = 0;

        //使用中なら
        if (g_Enemy[nCntEnemy].bUse == true)
        {
            //1F前の位置を記憶
            g_Enemy[nCntEnemy].posOld = g_Enemy[nCntEnemy].pos;

            //自機を取得する
            pPlayer = GetPlayer();

            //攻撃や移動に関する変数
            float fPposx = pPlayer->pos.x, fPposz = pPlayer->pos.z;	//自機の座標
            float fEposx = g_Enemy[nCntEnemy].pos.x, fEposz = g_Enemy[nCntEnemy].pos.z;		//敵の座標
            float fAngle = atan2f((fEposx - fPposx), (fEposz - fPposz));	//角度を決める											//角度
            float fAngle2 = atan2f((fPposx - fEposx), (fPposz - fEposz));	//角度を決める
            float fAngle3 = atan2f((fEposx - g_Enemy[nCntEnemy].posMemory.x), (fEposz - g_Enemy[nCntEnemy].posMemory.z));	//角度を決める
            float fAdjustment = 0.0f;  //符号調整
            float fDistanceY = 0.0f;   //自機と敵の高さの差

            //高さ調整
            if (pPlayer->pos.y < g_Enemy[nCntEnemy].pos.y)
            {
                fAdjustment = -1.0f;
                fDistanceY = g_Enemy[nCntEnemy].pos.y - pPlayer->pos.y;
            }
            else if (pPlayer->pos.y == g_Enemy[nCntEnemy].pos.y)
            {
                fAdjustment = 0.0f;
                fDistanceY = 0.0f;
            }
            else if (pPlayer->pos.y > g_Enemy[nCntEnemy].pos.y)
            {
                fAdjustment = 1.0f;
                fDistanceY = pPlayer->pos.y - g_Enemy[nCntEnemy].pos.y;
            }

            //自機と敵の距離
            g_Enemy[nCntEnemy].fDistance = sqrtf(
                powf((pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x), 2) +
                powf((pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z), 2));

            //エフェクトのための変数
            float fAngleEffect = 0.0f;    //角度
            float fRadius = g_Enemy[nCntEnemy].size.x; //エフェクト半径
            float fSpeedX = 0.0f;   //速さX
            float fSpeedY = 0.0f;   //速さY
            float fSpeedZ = 0.0f;   //速さZ
            int nPlusMinusX = 0;
            int nPlusMinusY = 0;
            int nPlusMinusZ = 0;

            //各エネミー
            switch (g_Enemy[nCntEnemy].type)
            {
                //========================
                //キャノン
                //========================
            case ENEMYTYPE_000:

                //状態による場合分け
                switch (g_Enemy[nCntEnemy].state)
                {
                    //通常時
                case ENEMYSTATE_NORMAL:

                    //位置を移動量と紐づけ
                    g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

                    //視野に入ったら行動
                    if (g_Enemy[nCntEnemy].fDistance < 1000.0f)
                    {
                        //クールタイムが終わってから攻撃可能
                        if (g_Enemy[nCntEnemy].nAttackTime == 0)
                        {
                            //弾の発射
                            if ((rand() % (g_RandomAttack * 90)) == 0)
                            {
                                SetBullet(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + ENEMY_HEIGHT_CANNON / 2
                                    , g_Enemy[nCntEnemy].pos.z), BULLET_SIZE_ENEMY,
                                    D3DXVECTOR3(-sinf(g_Enemy[nCntEnemy].rot.y)*BULLET_SPEED_ENEMY, 0.0f
                                        , -cosf(g_Enemy[nCntEnemy].rot.y)*BULLET_SPEED_ENEMY), 40, BULLET_RIGID, BULLET_DAMAGE_ENEMY, BULLETTYPE_ENEMY);

                                g_Enemy[nCntEnemy].nAttackTime = 20;
                            }
                        }
                    }

                    break;

                    //ダメージ時
                case ENEMYSTATE_DAMAGE:
                    g_Enemy[nCntEnemy].nCounterState--;

                    //打ち上げ時
                    g_Enemy[nCntEnemy].pos.y += g_Enemy[nCntEnemy].move.y;

                    if (g_Enemy[nCntEnemy].nCounterState <= 0)
                    {
                        g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                //自機のほうを徐々に見るようにする
                g_Enemy[nCntEnemy].rotDest.y = fAngle;

                // 回転の制限
                if (g_Enemy[nCntEnemy].rot.y > D3DX_PI)
                {
                    g_Enemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                }
                if (g_Enemy[nCntEnemy].rot.y < -D3DX_PI)
                {
                    g_Enemy[nCntEnemy].rot.y += D3DX_PI * 2;
                }

                g_Enemy[nCntEnemy].rotMin.y = g_Enemy[nCntEnemy].rotDest.y - g_Enemy[nCntEnemy].rot.y;
                
                // 目的の角度の回転を制限
                if (g_Enemy[nCntEnemy].rotMin.y > D3DX_PI)
                {
                    g_Enemy[nCntEnemy].rotDest.y -= D3DX_PI * 2;
                }
                if (g_Enemy[nCntEnemy].rotMin.y < -D3DX_PI)
                {
                    g_Enemy[nCntEnemy].rotDest.y += D3DX_PI * 2;
                }

                // 回転の制限
                if (g_Enemy[nCntEnemy].rot.y > D3DX_PI)
                {
                    g_Enemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                }
                if (g_Enemy[nCntEnemy].rot.y < -D3DX_PI)
                {
                    g_Enemy[nCntEnemy].rot.y += D3DX_PI * 2;
                }

                if (fabsf(g_Enemy[nCntEnemy].rotMin.y) < 0)
                {
                    g_Enemy[nCntEnemy].rotMin.y = g_Enemy[nCntEnemy].rotDest.y;
                }
                else
                {
                    g_Enemy[nCntEnemy].rot += (g_Enemy[nCntEnemy].rotDest - g_Enemy[nCntEnemy].rot) * NORMAL_ROT_SPEED;
                }

                break;
                //========================
                //スパイダー
                //========================
            case ENEMYTYPE_001:

                //状態による場合分け
                switch (g_Enemy[nCntEnemy].state)
                {
                    //通常時
                case ENEMYSTATE_NORMAL:

                    //位置を移動量と紐づけ
                    g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

                    //スパイダーの移動
                    g_Enemy[nCntEnemy].nPosX += 1;

                    //スパイダーの移動範囲
                    if (g_Enemy[nCntEnemy].nPosX > 50)
                    {
                        g_Enemy[nCntEnemy].move.x *= -1;
                        g_Enemy[nCntEnemy].nPosX *= -1;
                    }

                    break;

                    //ダメージ時
                case ENEMYSTATE_DAMAGE:
                    g_Enemy[nCntEnemy].nCounterState--;

                    //打ち上げ時
                    g_Enemy[nCntEnemy].pos.y += g_Enemy[nCntEnemy].move.y;

                    if (g_Enemy[nCntEnemy].nCounterState <= 0)
                    {
                        g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                //移動の向きによる、方向転換
                if (g_Enemy[nCntEnemy].move.x < 0)
                {
                    g_Enemy[nCntEnemy].rot.y = 900; //右向き
                }
                else if (g_Enemy[nCntEnemy].move.x > 0)
                {
                    g_Enemy[nCntEnemy].rot.y = 2700;//左向き
                }

                break;
                //========================
                //バタフライ
                //========================
            case ENEMYTYPE_002:

                //状態による場合分け
                switch (g_Enemy[nCntEnemy].state)
                {
                    //通常時
                case ENEMYSTATE_NORMAL:

                    //位置を移動量と紐づけ
                    g_Enemy[nCntEnemy].pos.x += g_Enemy[nCntEnemy].move.x;
                    g_Enemy[nCntEnemy].pos.z += g_Enemy[nCntEnemy].move.z;

                    //バタフライの移動
                    g_Enemy[nCntEnemy].nPosX += 1;
                    g_Enemy[nCntEnemy].nPosZ += 1;

                    //バタフライの移動範囲
                    //横
                    if (g_Enemy[nCntEnemy].nPosX > 60)
                    {
                        g_Enemy[nCntEnemy].move.x *= -1;
                        g_Enemy[nCntEnemy].nPosX *= -1;
                    }
                    //縦
                    if (g_Enemy[nCntEnemy].nPosZ > 80)
                    {
                        g_Enemy[nCntEnemy].move.z *= -1;
                        g_Enemy[nCntEnemy].nPosZ *= -1;
                    }

                    //クールタイムが終わってから攻撃可能
                    if (g_Enemy[nCntEnemy].nAttackTime == 0)
                    {
                        //弾の発射
                        if ((rand() % (g_RandomAttack * 90)) == 0)
                        {
                            SetBullet(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + ENEMY_HEIGHT_BUTTERFLY / 2, g_Enemy[nCntEnemy].pos.z), BULLET_SIZE_ENEMY,
                                D3DXVECTOR3(-sinf(fAngle)*BULLET_SPEED_ENEMY, fDistanceY / BULLET_SPEED_ENEMY * fAdjustment
                                    , -cosf(fAngle)*BULLET_SPEED_ENEMY), 40,BULLET_RIGID, BULLET_DAMAGE_ENEMY, BULLETTYPE_ENEMY);

                            g_Enemy[nCntEnemy].nAttackTime = 20;
                        }
                    }

                    break;

                    //ダメージ時
                case ENEMYSTATE_DAMAGE:
                    g_Enemy[nCntEnemy].nCounterState--;

                    if (g_Enemy[nCntEnemy].nCounterState <= 0)
                    {
                        g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                //自機のほうを見るようにする
                g_Enemy[nCntEnemy].rot.y = fAngle;

                break;

                //========================
                //ゴールデンスパイダー
                //========================
            case ENEMYTYPE_003:

                //状態による場合分け
                switch (g_Enemy[nCntEnemy].state)
                {
                    //通常時
                case ENEMYSTATE_NORMAL:

                    //位置を移動量と紐づけ
                    g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

                    //視野に入ったら逃げ出す
                    if (g_Enemy[nCntEnemy].fDistance < 500.0f)
                    {
                        g_Enemy[nCntEnemy].move.x = sinf(fAngle)*6.0f;
                        g_Enemy[nCntEnemy].move.z = cosf(fAngle)*6.0f;
                        //自機と逆を見るようにする
                        g_Enemy[nCntEnemy].rot.y = fAngle2;
                    }
                    else
                    {
                        //一度逃げ出したら、逃げるようにする
                        if (g_Enemy[nCntEnemy].move.x == 0)
                        {
                            //自機のほうを見るようにする
                            g_Enemy[nCntEnemy].rot.y = fAngle;
                        }
                        else
                        {
                            //自機と逆を見るようにする
                            g_Enemy[nCntEnemy].rot.y = fAngle2;
                        }
                    }

                    break;

                    //ダメージ時
                case ENEMYSTATE_DAMAGE:
                    g_Enemy[nCntEnemy].nCounterState--;

                    //打ち上げ時
                    g_Enemy[nCntEnemy].pos.y += g_Enemy[nCntEnemy].move.y;

                    if (g_Enemy[nCntEnemy].nCounterState <= 0)
                    {
                        g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                //ブロックに対する当たり判定
                EnemyHitBlock(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld,
                g_Enemy[nCntEnemy].size);

                //メッシュフィールドの範囲外に行ったら、消滅
                //左
                if (g_Enemy[nCntEnemy].pos.x < -FIELD_WIDTH_SIZE + g_Enemy[nCntEnemy].size.x)
                {
                    g_Enemy[nCntEnemy].bUse = false;
                }
                //右
                if (g_Enemy[nCntEnemy].pos.x > FIELD_WIDTH_SIZE - g_Enemy[nCntEnemy].size.x)
                {
                    g_Enemy[nCntEnemy].bUse = false;
                }
                //後ろ
                if (g_Enemy[nCntEnemy].pos.z < -FIELD_WIDTH_SIZE + g_Enemy[nCntEnemy].size.z)
                {
                    g_Enemy[nCntEnemy].bUse = false;
                }
                //前
                if (g_Enemy[nCntEnemy].pos.z > FIELD_WIDTH_SIZE + g_Enemy[nCntEnemy].size.z)
                {
                    g_Enemy[nCntEnemy].bUse = false;
                }

                ////キラキラ
                //for (int nCnt = 0; nCnt < 1; nCnt++)
                //{
                //    fSpeedX = float(rand() % 200 + 100) / 100.0f;
                //    fSpeedY = float(rand() % 250 + 50) / 100.0f;
                //    fSpeedZ = float(rand() % 200 + 100) / 100.0f;
                //    nPlusMinusX = int(rand() % 2 + 1);
                //    nPlusMinusY = int(rand() % 2 + 1);
                //    nPlusMinusZ = int(rand() % 2 + 1);
                //    if (nPlusMinusX == 1)
                //    {
                //        nPlusMinusX = 1;
                //    }
                //    else
                //    {
                //        nPlusMinusX = -1;
                //    }

                //    if (nPlusMinusY == 1)
                //    {
                //        nPlusMinusY = 1;
                //    }
                //    else
                //    {
                //        nPlusMinusY = 1;
                //    }

                //    if (nPlusMinusZ == 1)
                //    {
                //        nPlusMinusZ = 1;
                //    }
                //    else
                //    {
                //        nPlusMinusZ = -1;
                //    }

                //    //0～3.14を出す*(0 or 1)*-1で正負を決める
                //    fAngleEffect = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                //    D3DXVECTOR3 pos = D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x - sinf(fAngleEffect)*fRadius, g_Enemy[nCntEnemy].pos.y,
                //        g_Enemy[nCntEnemy].pos.z - cosf(fAngleEffect)*fRadius);

                //    D3DXVECTOR3 move = D3DXVECTOR3(/*fSpeedX*nPlusMinusX*/0.0f, fSpeedY*nPlusMinusY, /*fSpeedZ*nPlusMinusZ*/0.0f);
                //    SetEffect2(pos, move, EFFECT2_SIZE_HEAL, EFFECT2_INIT_COLOR_EXPLOSION, EFFECT2TYPE_RARE);
                //}

                break;
                //========================
                //ジャンプボックス
                //========================
            case ENEMYTYPE_004:

                //状態による場合分け
                switch (g_Enemy[nCntEnemy].state)
                {
                    //通常時
                case ENEMYSTATE_NORMAL:

                    //位置を移動量と紐づけ
                    g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

                    //ジャンプ
                    if (g_Enemy[nCntEnemy].pos.y == 0)
                    {
                        g_Enemy[nCntEnemy].move.y += VALUE_JUMP;
                    }

                    //ジャンプボックスの移動
                    g_Enemy[nCntEnemy].nPosX += 1;

                    //ジャンプボックスの移動範囲
                    if (g_Enemy[nCntEnemy].nPosX > 160)
                    {
                        g_Enemy[nCntEnemy].move.x *= -1;
                        g_Enemy[nCntEnemy].nPosX *= -1;
                    }

                    break;

                    //ダメージ時
                case ENEMYSTATE_DAMAGE:
                    g_Enemy[nCntEnemy].nCounterState--;

                    //打ち上げ時
                    g_Enemy[nCntEnemy].pos.y += g_Enemy[nCntEnemy].move.y;

                    if (g_Enemy[nCntEnemy].nCounterState <= 0)
                    {
                        g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                //移動の向きによる、方向転換
                if (g_Enemy[nCntEnemy].move.x < 0)
                {
                    g_Enemy[nCntEnemy].rot.y = 900; //右向き
                }
                else if (g_Enemy[nCntEnemy].move.x > 0)
                {
                    g_Enemy[nCntEnemy].rot.y = 2700;//左向き
                }

                break;
                //========================
                //フィッシュ
                //========================
            case ENEMYTYPE_005:

                //状態による場合分け
                switch (g_Enemy[nCntEnemy].state)
                {
                    //通常時
                case ENEMYSTATE_NORMAL:

                    //位置を移動量と紐づけ
                    g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;

                    //視野に入ったら追いかける
                    if (g_Enemy[nCntEnemy].fDistance < 700.0f)
                    {
                        g_Enemy[nCntEnemy].bFollow = true;
                    }
                    //視野から出たら
                    else
                    {
                        ////元の場所に帰る
                        //if (sqrtf(
                        //    powf((g_Enemy[nCntEnemy].pos.x - g_Enemy[nCntEnemy].posMemory.x), 2) +
                        //    powf((pPlayer->pos.z - g_Enemy[nCntEnemy].posMemory.z), 2))>50.0f)
                        //{
                        //    if (g_Enemy[nCntEnemy].move.x != 0.0f)
                        //    {
                        //        g_Enemy[nCntEnemy].move.x = -sinf(fAngle3)*5.0f;
                        //        g_Enemy[nCntEnemy].move.z = -cosf(fAngle3)*5.0f;
                        //        g_Enemy[nCntEnemy].rot.y = fAngle3;
                        //    }
                        //}
                        //else
                        //{
                            g_Enemy[nCntEnemy].bFollow = false;
                            //g_Enemy[nCntEnemy].rot.y = 0;
                        /*}*/
                    }

                    //視野外から攻撃を受けても、動き出す
                    if (g_Enemy[nCntEnemy].fLife != ENEMY_LIFE_FISH)
                    {
                        g_Enemy[nCntEnemy].bFollow = true;
                    }

                    //追従の移動量
                    if (g_Enemy[nCntEnemy].bFollow == true && pPlayer->bDisp == true)   //プレイヤー撃破後のぐるぐるをなくす
                    {
                        //プレイヤーのY軸が高いと、追従をだんだん遅くさせる
                        if (pPlayer->pos.y > 0)
                        {
                            g_Enemy[nCntEnemy].move.x *= 0.99f;
                            g_Enemy[nCntEnemy].move.z *= 0.99f;
                        }
                        else
                        {
                            g_Enemy[nCntEnemy].move.x = -sinf(fAngle)*5.0f;
                            g_Enemy[nCntEnemy].move.z = -cosf(fAngle)*5.0f;
                        }
                    }
                    //追従していないときの移動量
                    else
                    {
                        g_Enemy[nCntEnemy].move.x = 0.0f;
                        g_Enemy[nCntEnemy].move.z = 0.0f;
                    }

                    //プレイヤーが生存しているなら、
                    if (pPlayer->bDisp == true)
                    {
                        //自機を見るようにする
                        g_Enemy[nCntEnemy].rot.y = fAngle;
                    }

                    break;

                    //ダメージ時
                case ENEMYSTATE_DAMAGE:
                    g_Enemy[nCntEnemy].nCounterState--;

                    //打ち上げ用
                    g_Enemy[nCntEnemy].pos.y += g_Enemy[nCntEnemy].move.y;

                    //追従を鈍らせる
                    g_Enemy[nCntEnemy].pos.x += g_Enemy[nCntEnemy].move.x * 0.5f;
                    g_Enemy[nCntEnemy].pos.z += g_Enemy[nCntEnemy].move.z * 0.5f;

                    if (g_Enemy[nCntEnemy].nCounterState <= 0)
                    {
                        g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                //ブロックに対する当たり判定
                EnemyHitBlock(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld,
                    g_Enemy[nCntEnemy].size);

                //砂埃
                //for (int nCnt = 0; nCnt < 1; nCnt++)
                //{
                //    fSpeedX = float(rand() % 200 + 100) / 100.0f;
                //    fSpeedY = float(rand() % 250 + 50) / 100.0f;
                //    fSpeedZ = float(rand() % 200 + 100) / 100.0f;
                //    nPlusMinusX = int(rand() % 2 + 1);
                //    nPlusMinusY = int(rand() % 2 + 1);
                //    nPlusMinusZ = int(rand() % 2 + 1);
                //    if (nPlusMinusX == 1)
                //    {
                //        nPlusMinusX = 1;
                //    }
                //    else
                //    {
                //        nPlusMinusX = -1;
                //    }

                //    if (nPlusMinusY == 1)
                //    {
                //        nPlusMinusY = 1;
                //    }
                //    else
                //    {
                //        nPlusMinusY = 1;
                //    }

                //    if (nPlusMinusZ == 1)
                //    {
                //        nPlusMinusZ = 1;
                //    }
                //    else
                //    {
                //        nPlusMinusZ = -1;
                //    }

                //    //0～3.14を出す*(0 or 1)*-1で正負を決める
                //    fAngleEffect = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                //    D3DXVECTOR3 pos = D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x - sinf(fAngleEffect)*fRadius, g_Enemy[nCntEnemy].pos.y,
                //        g_Enemy[nCntEnemy].pos.z - cosf(fAngleEffect)*fRadius);

                //    D3DXVECTOR3 move = D3DXVECTOR3(/*fSpeedX*nPlusMinusX*/0.0f, fSpeedY*nPlusMinusY, /*fSpeedZ*nPlusMinusZ*/0.0f);
                //    SetEffect2(pos, move, EFFECT2_SIZE_HEAL, EFFECT2_INIT_COLOR_EXPLOSION, EFFECT2TYPE_RARE);
                //}
                
                break;
            }

            //攻撃中のカウンター
            if (g_Enemy[nCntEnemy].nAttackTime > 0)
            {
                g_Enemy[nCntEnemy].nAttackTime--;
            }

            //重力と地面の設定
            g_Enemy[nCntEnemy].move.y -= GRAVITY;
            if (g_Enemy[nCntEnemy].pos.y < 0)
            {
                g_Enemy[nCntEnemy].pos.y = 0;
                g_Enemy[nCntEnemy].move.y = 0;
            }

            //弾の当たり判定
            CollisionBulletEnemy(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld,
                D3DXVECTOR3(g_Enemy[nCntEnemy].size.x, g_Enemy[nCntEnemy].size.y * 2, g_Enemy[nCntEnemy].size.z) , nCntEnemy);

            //エフェクトの当たり判定
            CollisionEffectEnemy(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld,
                D3DXVECTOR3(g_Enemy[nCntEnemy].size.x, g_Enemy[nCntEnemy].size.y * 2, g_Enemy[nCntEnemy].size.z), nCntEnemy);

            //影をつける
            //SetPositionShadow(g_ShadowIndex, D3DXVECTOR3(g_aEnemy.pos.x, SHADOW_HEIGHT, g_aEnemy.pos.z));

            //死亡判定
            if (g_Enemy[nCntEnemy].fLife <= 0.0f)
            {

                //爆発エフェクト
                float fSpeedX = 0.0f;   //速さX
                float fSpeedY = 0.0f;   //速さY
                float fSpeedZ = 0.0f;   //速さZ
                int nPlusMinusX = 0;
                int nPlusMinusZ = 0;
                for (int nCnt = 0; nCnt < 200; nCnt++)
                {
                    fSpeedX = float(rand() % 800 + 300) / 100.0f;
                    fSpeedY = float(rand() % 800 + 300) / 100.0f;
                    fSpeedZ = float(rand() % 800 + 300) / 100.0f;
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
                    SetEffect2(g_Enemy[nCntEnemy].pos, move, EFFECT2_SIZE_EXPLOSION, EFFECT2_INIT_COLOR_EXPLOSION, EFFECT2TYPE_EXPLOSION); //爆発エフェクト
                }

                //敵の種類によって入るスコアを変える
                switch (g_Enemy[nCntEnemy].type)
                {
                    //キャノン
                case ENEMYTYPE_000:
                    ScoreCalculation(2000);
                    g_nCntPhase1--;
                    break;

                    //スパイダー
                case ENEMYTYPE_001:
                    ScoreCalculation(1000);
                    break;

                    //バタフライ
                case ENEMYTYPE_002:
                    ScoreCalculation(3000);
                    g_nCntPhase2--;
                    break;

                    //ゴールデンスパイダー
                case ENEMYTYPE_003:
                    ScoreCalculation(10000);
                    break;

                    //ジャンプボックス
                case ENEMYTYPE_004:
                    ScoreCalculation(5000);
                    //ハートを生成
                    SetItem(g_Enemy[nCntEnemy].pos, ITEM_SIZE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_000);
                    break;

                    //フィッシュ
                case ENEMYTYPE_005:
                    ScoreCalculation(3500);
                    g_nCntPhase3--;
                    break;
                }

                //第1フェーズの壁
                if (g_nCntPhase1 == 0)
                {
                    WALL*pWall = GetWall();
                    for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
                    {
                        if (pWall->type == WALLTYPE_RED1)
                        {
                            //使用中なら未使用にする
                            if (pWall->bUse == true)
                            {
                                pWall->bUse = false;

                                //ロック解除マーク
                                SetEffect(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + g_Enemy[nCntEnemy].size.y / 2
                                            , g_Enemy[nCntEnemy].pos.z), UNLOCK_SIZE,D3DXVECTOR3(0.0f, 1.0f, 0.0f), EFFECTTYPE_002);
                            }
                        }
                    }
                }

                //第2フェーズの壁
                if (g_nCntPhase2 == 0)
                {
                    WALL*pWall = GetWall();
                    for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
                    {
                        if (pWall->type == WALLTYPE_RED2)
                        {
                            //使用中なら未使用にする
                            if (pWall->bUse == true)
                            {
                                pWall->bUse = false;

                                //ロック解除マーク
                                SetEffect(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + g_Enemy[nCntEnemy].size.y / 2
                                            , g_Enemy[nCntEnemy].pos.z), UNLOCK_SIZE, D3DXVECTOR3(0.0f, 1.0f, 0.0f), EFFECTTYPE_002);
                            }
                        }
                    }
                }

                //第3フェーズの壁
                if (g_nCntPhase3 == 0)
                {
                    WALL*pWall = GetWall();
                    for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
                    {
                        if (pWall->type == WALLTYPE_RED3)
                        {
                            //使用中なら未使用にする
                            if (pWall->bUse == true)
                            {
                                pWall->bUse = false;

                                //ロック解除マーク
                                SetEffect(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + g_Enemy[nCntEnemy].size.y / 2
                                            , g_Enemy[nCntEnemy].pos.z), UNLOCK_SIZE, D3DXVECTOR3(0.0f, 1.0f, 0.0f), EFFECTTYPE_002);
                            }
                        }
                    }
                }

                //バグ回避
                g_Enemy[nCntEnemy].fDistance = 9999.9f;
                g_Enemy[nCntEnemy].bUse = false;

                //爆発音
                PlaySound(SOUND_LABEL_SE_EXPLOSION2);
            }

        }
    }

    //ランク付けの内部処理
    int nNum01, nNum02;		//距離同士を比較する際に使用
    for (nNum01 = 0; nNum01 < MAX_ENEMY; nNum01++)
    {
        for (nNum02 = 0; nNum02 < MAX_ENEMY; nNum02++)
        {
            //距離が一番近いものがRank1になるようにする
            if (g_Enemy[nNum01].fDistance >= g_Enemy[nNum02].fDistance)
            {
                g_Enemy[nNum01].nRank++;
            }
        }
    }
}

//=====================================================
// エネミー描画処理
//=====================================================
void DrawEnemy(void)
{
    int nCntEnemy;
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスへのポインタ
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;
    D3DMATERIAL9 matDef;		// 現在のマテリアル保持用
    D3DXMATERIAL* pMat;			// マテリアルデータへのポインタ


    for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        if (g_Enemy[nCntEnemy].bUse == true)
        {
            // ワールドマトリックスの初期化
            D3DXMatrixIdentity(&g_Enemy[nCntEnemy].mtxWorld);

            // スケールを反映
            D3DXMatrixScaling(&mtxScale,
                g_Enemy[nCntEnemy].size.x / g_Enemy[nCntEnemy].size.x,
                g_Enemy[nCntEnemy].size.y / g_Enemy[nCntEnemy].size.y,
                g_Enemy[nCntEnemy].size.z / g_Enemy[nCntEnemy].size.z);
            D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorld, &g_Enemy[nCntEnemy].mtxWorld, &mtxScale);
            // 向きを反映
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_Enemy[nCntEnemy].rot.y,
                g_Enemy[nCntEnemy].rot.x,
                g_Enemy[nCntEnemy].rot.z);
            D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorld, &g_Enemy[nCntEnemy].mtxWorld, &mtxRot);
            // 位置を反映
            D3DXMatrixTranslation(&mtxTrans,
                g_Enemy[nCntEnemy].pos.x,
                g_Enemy[nCntEnemy].pos.y,
                g_Enemy[nCntEnemy].pos.z);
            D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorld, &g_Enemy[nCntEnemy].mtxWorld, &mtxTrans);


            // ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].mtxWorld);
            // 現在のマテリアルを取得する
            pDevice->GetMaterial(&matDef);
            // マテリアルデータへのポインタを取得
            pMat = (D3DXMATERIAL*)g_pBuffMatEnemy[g_Enemy[nCntEnemy].type]->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)g_nNumMatEnemy[g_Enemy[nCntEnemy].type]; nCntMat++)
            {
                // マテリアルの設定
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                // モデルパーツ
                g_pMeshEnemy[g_Enemy[nCntEnemy].type]->DrawSubset(nCntMat);
            }
            // 保持していたマテリアルを戻す
            pDevice->SetMaterial(&matDef);
        }
    }
}

//=============================================================================
// エネミーの設定処理
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos,D3DXVECTOR3 move, ENEMYTYPE type)
{
    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        if (g_Enemy[nCntEnemy].bUse == false)
        {
            //位置を設定
            g_Enemy[nCntEnemy].pos = pos;

            //配置位置を記憶
            g_Enemy[nCntEnemy].posMemory = pos;

            g_Enemy[nCntEnemy].move = move;

            g_Enemy[nCntEnemy].type = type;

            switch (g_Enemy[nCntEnemy].type)
            {
            case ENEMYTYPE_000:
                g_Enemy[nCntEnemy].size = ENEMY_SIZE_CANNON;
                g_Enemy[nCntEnemy].fLife = ENEMY_LIFE_CANNON;
                break;
            case ENEMYTYPE_001:
                g_Enemy[nCntEnemy].size = ENEMY_SIZE_SPIDER;
                g_Enemy[nCntEnemy].fLife = ENEMY_LIFE_SPIDER;
                break;
            case ENEMYTYPE_002:
                g_Enemy[nCntEnemy].size = ENEMY_SIZE_BUTTERFLY;
                g_Enemy[nCntEnemy].fLife = ENEMY_LIFE_BUTTERFLY;
                break;
            case ENEMYTYPE_003:
                g_Enemy[nCntEnemy].size = ENEMY_SIZE_SPIDER;
                g_Enemy[nCntEnemy].fLife = ENEMY_LIFE_GOLDENSPIDER;
                break;
            case ENEMYTYPE_004:
                g_Enemy[nCntEnemy].size = ENEMY_SIZE_JUMPBOX;
                g_Enemy[nCntEnemy].fLife = ENEMY_LIFE_JUMPBOX;
                break;
            case ENEMYTYPE_005:
                g_Enemy[nCntEnemy].size = ENEMY_SIZE_FISH;
                g_Enemy[nCntEnemy].fLife = ENEMY_LIFE_FISH;
                break;
            }

            g_Enemy[nCntEnemy].bUse = true;

            //敵のHPバーの設定
            //SetEnemyLife(g_Enemy[nCntEnemy].fLife, nCntEnemy);

            break;
        }
    }
}

//=============================================================================
// エネミーの取得
//=============================================================================
ENEMY*GetEnemy(void)
{
    return &g_Enemy[0];
}

//=============================================================================
// エネミーのダメージ設定
//=============================================================================
void HitEnemy(int nCntEnemy, int nCntRigid, float fDamage)
{
    //PlaySound(SOUND_LABEL_SE_EXPLOSION);	//爆発音

    //ダメージ状態に遷移
    g_Enemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
    g_Enemy[nCntEnemy].nCounterState = nCntRigid; //ヒット硬直

    //ダメージ計算
    g_Enemy[nCntEnemy].fLife -= fDamage;

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
bool CollisionEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bHit = false;  //当たったかどうか

    for (int nCntEnemy = 0; nCntEnemy < MAX_BULLET; nCntEnemy++)
    {
        // 弾が使われたら
        if (g_Enemy[nCntEnemy].bUse == true)
        {

            //当たり判定を計算
            if (RectangleCollision(pPos, pPosOld, size, &g_Enemy[nCntEnemy].pos, g_Enemy[nCntEnemy].size) == true)
            {

                PLAYER*pPlayer = GetPlayer();

                //必殺中は無敵
                if (pPlayer->bChanceAttack == false)
                {
                    //タイプごとに分ける
                    switch (g_Enemy[nCntEnemy].type)
                    {
                        //キャノン
                    case ENEMYTYPE_000:
                        //自機にダメージ
                        HitPlayer(5, HIT_ENEMY_DAMAGE);
                        break;

                        //スパイダー
                    case ENEMYTYPE_001:
                        //自機にダメージ
                        HitPlayer(5, HIT_ENEMY_DAMAGE);
                        break;

                        //バタフライ
                    case ENEMYTYPE_002:
                        //自機にダメージ
                        HitPlayer(5, HIT_ENEMY_DAMAGE);
                        break;

                        //ゴールデンスパイダー
                    case ENEMYTYPE_003:
                        //自機にダメージ
                        HitPlayer(5, HIT_ENEMY_DAMAGE);
                        break;

                        //ジャンプボックス
                    case ENEMYTYPE_004:
                        //自機にダメージ
                        HitPlayer(5, HIT_ENEMY_DAMAGE);
                        break;

                        //フィッシュ
                    case ENEMYTYPE_005:
                        //自機にダメージ
                        HitPlayer(5, HIT_ENEMY_DAMAGE);
                        break;
                    }

                    //ノックバック
                    float fKnockback = VALUE_KNOCKBACK;
                    pPlayer->move.x = pPlayer->pos.x - g_Enemy[nCntEnemy].posOld.x;
                    pPlayer->move.z = pPlayer->pos.z - g_Enemy[nCntEnemy].posOld.z;
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
                        SetEffect2(g_Enemy[nCntEnemy].pos, move, EFFECT2_SIZE_HIT, EFFECT2_INIT_COLOR_HIT, EFFECT2TYPE_HIT);
                    }
                }

                bHit = true;    //当たった
            }
        }
    }

    return bHit;    //当たったかどうかを返す
}

//=====================================================
// 打ち上げるための関数
//=====================================================
void UpperEnemy(float fUp, int nCnt)
{
    //打ち上げる
    g_Enemy[nCnt].move.y += fUp;
}