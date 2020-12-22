//=============================================================================
//
// 最強昔話決定戦の処理[enemy.cpp]（ゲーム内に出てくる雑魚敵）
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "player.h"
#include "camera.h"
#include "shadow.h"
#include "enemy.h"
#include "bullet.h"
#include "score.h"
#include "effect.h"
#include "collision.h"
#include "block.h"
#include "item.h"
#include "sound.h"
#include "game.h"
#include "fire.h"
#include "bomb.h"

//=============================================================================
// グローバル変数定義
//=============================================================================
LPD3DXMESH g_pMeshEnemy[ENEMYTYPE_MAX] = {};		// メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatEnemy[ENEMYTYPE_MAX] = {};   // マテリアル情報へのポインタ
DWORD g_nNumMatEnemy[ENEMYTYPE_MAX] = {};			// マテリアル情報の数

ENEMY g_aEnemy[MAX_ENEMY];	        //エネミーの情報
int g_RandomAttack = 1;				//弾をランダムに打つときに使用

//=============================================================================
// エネミー初期化処理
// Author:後藤慎之助
//=============================================================================
void InitEnemy(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    // Xファイルの読み込み
    //キャノン
    D3DXLoadMeshFromX("MODEL/enemy000.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_CANNON],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_CANNON],
        &g_pMeshEnemy[ENEMYTYPE_CANNON]);

    //アンブレラ
    D3DXLoadMeshFromX("MODEL/enemy001.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_UMBRELLA],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_UMBRELLA],
        &g_pMeshEnemy[ENEMYTYPE_UMBRELLA]);

    //ゴースト
    D3DXLoadMeshFromX("MODEL/enemy002.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_GHOST],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_GHOST],
        &g_pMeshEnemy[ENEMYTYPE_GHOST]);

    //ツチノコ
    D3DXLoadMeshFromX("MODEL/enemy003.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_SNAKE],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_SNAKE],
        &g_pMeshEnemy[ENEMYTYPE_SNAKE]);

    //レッドゴースト
    D3DXLoadMeshFromX("MODEL/enemy004.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatEnemy[ENEMYTYPE_RED_GHOST],
        NULL,
        &g_nNumMatEnemy[ENEMYTYPE_RED_GHOST],
        &g_pMeshEnemy[ENEMYTYPE_RED_GHOST]);

    //敵の情報の初期化
    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEnemy[nCntEnemy].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEnemy[nCntEnemy].rotMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEnemy[nCntEnemy].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
        g_aEnemy[nCntEnemy].type = ENEMYTYPE_CANNON;
        g_aEnemy[nCntEnemy].bUse = false;
        g_aEnemy[nCntEnemy].fLife = 0.0f;
        g_aEnemy[nCntEnemy].nAttackTime = 0;
        g_aEnemy[nCntEnemy].nShadow = 0;
        g_aEnemy[nCntEnemy].nCntInvincibleFrame = 0;
        g_aEnemy[nCntEnemy].bTransparence = false;
        g_aEnemy[nCntEnemy].fDistance = 0.0f;
        g_aEnemy[nCntEnemy].bFindPlayer = false;
        g_aEnemy[nCntEnemy].nCntInvincibleFrame = 0;
    }
}

//=============================================================================
// エネミー終了処理
// Author:後藤慎之助
//=============================================================================
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

//=============================================================================
// エネミー更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateEnemy(void)
{
    //自機を取得する
    PLAYER *pPlayer = GetPlayer();

    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        //使用中なら
        if (g_aEnemy[nCntEnemy].bUse == true)
        {
            //無敵時間
            if (g_aEnemy[nCntEnemy].nCntInvincibleFrame > 0)
            {
                g_aEnemy[nCntEnemy].nCntInvincibleFrame--;                     //無敵時間を数える
                g_aEnemy[nCntEnemy].bTransparence = !g_aEnemy[nCntEnemy].bTransparence;   //無敵中は、点滅させる
            }
            else
            {
                g_aEnemy[nCntEnemy].bTransparence = false; //透明を解除
            }

            //1F前の位置を記憶
            g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

            //攻撃や移動に関する変数
            float fPposx = pPlayer->pos.x, fPposz = pPlayer->pos.z;	                        //自機の座標
            float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposz = g_aEnemy[nCntEnemy].pos.z;	//敵の座標
            float fAngle = atan2f((fEposx - fPposx), (fEposz - fPposz));	                //角度を決める
            float fAngle2 = atan2f((fPposx - fEposx), (fPposz - fEposz));	                //角度を決める

            //自機と敵の距離
            g_aEnemy[nCntEnemy].fDistance = sqrtf(
                powf((pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x), 2) +
                powf((pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z), 2));

            //エフェクトのための変数
            float fAngleEffect = 0.0f;    //角度
            float fRadius = g_aEnemy[nCntEnemy].size.x; //エフェクト半径
            float fSpeedX = 0.0f;   //速さX
            float fSpeedY = 0.0f;   //速さY
            float fSpeedZ = 0.0f;   //速さZ
            int nPlusMinusX = 0;
            int nPlusMinusY = 0;
            int nPlusMinusZ = 0;

            //各エネミー
            switch (g_aEnemy[nCntEnemy].type)
            {
                //========================
                //キャノン
                //========================
            case ENEMYTYPE_CANNON:

                //状態による場合分け
                switch (g_aEnemy[nCntEnemy].state)
                {
                    //通常時
                case ENEMYSTATE_NORMAL:

                    //位置を移動量と紐づけ
                    g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

                    //視野に入ってかつ、攻撃のクールタイムが終わったら行動
                    if (g_aEnemy[nCntEnemy].fDistance < ENEMY_FIELD_OF_VISION && g_aEnemy[nCntEnemy].nAttackTime == 0)
                    {
                        //自機のほうを徐々に見るようにする
                        g_aEnemy[nCntEnemy].rotDest.y = fAngle;

                        // 回転の制限
                        if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                        {
                            g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                        }
                        if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                        {
                            g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                        }

                        g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y;

                        // 目的の角度の回転を制限
                        if (g_aEnemy[nCntEnemy].rotMin.y > D3DX_PI)
                        {
                            g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2;
                        }
                        if (g_aEnemy[nCntEnemy].rotMin.y < -D3DX_PI)
                        {
                            g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2;
                        }

                        // 回転の制限
                        if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                        {
                            g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                        }
                        if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                        {
                            g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                        }

                        if (fabsf(g_aEnemy[nCntEnemy].rotMin.y) < 0)
                        {
                            g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y;
                        }
                        else
                        {
                            g_aEnemy[nCntEnemy].rot += (g_aEnemy[nCntEnemy].rotDest - g_aEnemy[nCntEnemy].rot) * ENEMY_TURN_SPEED;
                        }

                        //弾の発射
                        if ((rand() % (g_RandomAttack * ENEMY_ATTACK_COOL_DOWN)) == 0)
                        {
                            SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + ENEMY_HEIGHT_CANNON / 2
                                , g_aEnemy[nCntEnemy].pos.z), BULLET_SIZE_ENEMY,
                                D3DXVECTOR3(-sinf(g_aEnemy[nCntEnemy].rot.y)*BULLET_SPEED_ENEMY, 0.0f
                                    , -cosf(g_aEnemy[nCntEnemy].rot.y)*BULLET_SPEED_ENEMY), BULLET_LIFE_ENEMY, BULLET_DAMAGE_ENEMY, BULLETTYPE_ENEMY);

                            g_aEnemy[nCntEnemy].nAttackTime = ENEMY_ATTACK_COOL_DOWN;
                        }
                    }

                    break;

                    //ダメージ時
                case ENEMYSTATE_DAMAGE:

                    if (g_aEnemy[nCntEnemy].nCntInvincibleFrame <= 0)
                    {
                        g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                break;

                //========================
                //アンブレラ
                //========================
            case ENEMYTYPE_UMBRELLA:

                //状態による場合分け
                switch (g_aEnemy[nCntEnemy].state)
                {
                    //通常時
                case ENEMYSTATE_NORMAL:

                    //位置を移動量と紐づけ
                    g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

                    // 移動している方向を向く
                    // 回転の制限
                    if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                    }

                    g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y;

                    // 目的の角度の回転を制限
                    if (g_aEnemy[nCntEnemy].rotMin.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rotMin.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2;
                    }

                    // 回転の制限
                    if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                    }

                    if (fabsf(g_aEnemy[nCntEnemy].rotMin.y) < 0)
                    {
                        g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y;
                    }
                    else
                    {
                        g_aEnemy[nCntEnemy].rot += (g_aEnemy[nCntEnemy].rotDest - g_aEnemy[nCntEnemy].rot) * ENEMY_TURN_SPEED;
                    }

                    //ジャンプ
                    if (g_aEnemy[nCntEnemy].pos.y == 0)
                    {
                        g_aEnemy[nCntEnemy].move.y = ENEMY_JUMP;
                    }

                    break;

                    //ダメージ時
                case ENEMYSTATE_DAMAGE:

                    if (g_aEnemy[nCntEnemy].nCntInvincibleFrame <= 0)
                    {
                        g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                break;

                //========================
                //ゴースト
                //========================
            case ENEMYTYPE_GHOST:

                //状態による場合分け
                switch (g_aEnemy[nCntEnemy].state)
                {
                    //通常時
                case ENEMYSTATE_NORMAL:

                    //位置を移動量と紐づけ
                    g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

                    // 移動している方向を向く
                    // 回転の制限
                    if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                    }

                    g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y;

                    // 目的の角度の回転を制限
                    if (g_aEnemy[nCntEnemy].rotMin.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rotMin.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2;
                    }

                    // 回転の制限
                    if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                    }

                    if (fabsf(g_aEnemy[nCntEnemy].rotMin.y) < 0)
                    {
                        g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y;
                    }
                    else
                    {
                        g_aEnemy[nCntEnemy].rot += (g_aEnemy[nCntEnemy].rotDest - g_aEnemy[nCntEnemy].rot) * ENEMY_TURN_SPEED;
                    }

                    break;

                    //ダメージ時
                case ENEMYSTATE_DAMAGE:

                    if (g_aEnemy[nCntEnemy].nCntInvincibleFrame <= 0)
                    {
                        g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                break;

                //========================
                //レッドゴースト
                //========================
            case ENEMYTYPE_RED_GHOST:

                //状態による場合分け
                switch (g_aEnemy[nCntEnemy].state)
                {
                    //通常時
                case ENEMYSTATE_NORMAL:

                    //位置を移動量と紐づけ
                    g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

                    // 移動している方向を向く
                    // 回転の制限
                    if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                    }

                    g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y;

                    // 目的の角度の回転を制限
                    if (g_aEnemy[nCntEnemy].rotMin.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rotMin.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2;
                    }

                    // 回転の制限
                    if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
                    }
                    if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
                    {
                        g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
                    }

                    if (fabsf(g_aEnemy[nCntEnemy].rotMin.y) < 0)
                    {
                        g_aEnemy[nCntEnemy].rotMin.y = g_aEnemy[nCntEnemy].rotDest.y;
                    }
                    else
                    {
                        g_aEnemy[nCntEnemy].rot += (g_aEnemy[nCntEnemy].rotDest - g_aEnemy[nCntEnemy].rot) * ENEMY_TURN_SPEED;
                    }

                    break;

                    //ダメージ時
                case ENEMYSTATE_DAMAGE:

                    if (g_aEnemy[nCntEnemy].nCntInvincibleFrame <= 0)
                    {
                        g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                break;

                //========================
                //ツチノコ
                //========================
            case ENEMYTYPE_SNAKE:

                //状態による場合分け
                switch (g_aEnemy[nCntEnemy].state)
                {
                    //通常時
                case ENEMYSTATE_NORMAL:

                    //位置を移動量と紐づけ
                    g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

                    //視野に入ったら行動
                    if (g_aEnemy[nCntEnemy].fDistance < ENEMY_FIELD_OF_VISION)
                    {
                        g_aEnemy[nCntEnemy].bFindPlayer = true;
                    }

                    break;

                    //ダメージ時
                case ENEMYSTATE_DAMAGE:

                    if (g_aEnemy[nCntEnemy].nCntInvincibleFrame <= 0)
                    {
                        g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
                    }

                    break;
                }

                break;
            }

            //攻撃中のカウンター
            if (g_aEnemy[nCntEnemy].nAttackTime > 0)
            {
                g_aEnemy[nCntEnemy].nAttackTime--;
            }

            //重力と地面の設定
            if (g_aEnemy[nCntEnemy].type != ENEMYTYPE_SNAKE)
            {
                g_aEnemy[nCntEnemy].move.y -= GRAVITY;
                if (g_aEnemy[nCntEnemy].pos.y < 0)
                {
                    g_aEnemy[nCntEnemy].pos.y = 0;
                    g_aEnemy[nCntEnemy].move.y = 0;
                }
            }
            //ツチノコは地中まで行ったら未使用にする
            else
            {
                //地中まで行ったら
                if (g_aEnemy[nCntEnemy].pos.y < -GRID_SIZE)
                {
                    //未使用に
                    g_aEnemy[nCntEnemy].bUse = false;

                    //影の削除
                    SHADOW *pShadow = GetShadow();//影の情報を取得
                    pShadow[g_aEnemy[nCntEnemy].nShadow].use = false;

                    //初期化
                    g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                    g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                    g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                    g_aEnemy[nCntEnemy].fDistance = 9999.9f;
                }
            }

            //ツチノコが自機を発見した場合
            if (g_aEnemy[nCntEnemy].bFindPlayer == true)
            {
                g_aEnemy[nCntEnemy].rot.y += ENEMY_TURN_SPEED;
                g_aEnemy[nCntEnemy].move.y -= ENEMY_TURN_SPEED;
            }

            //ゴースト、アンブレラ、レッドゴーストの場合
            if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_GHOST || g_aEnemy[nCntEnemy].type == ENEMYTYPE_UMBRELLA
                || g_aEnemy[nCntEnemy].type == ENEMYTYPE_RED_GHOST)
            {
                //ブロックの当たり判定
                if (CollisionBlock(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld,
                    D3DXVECTOR3(GRID_SIZE, GRID_SIZE * 2, GRID_SIZE)) == true)
                {
                    int nRandom = 0;

                    //タイプごとに場合分け
                    switch(g_aEnemy[nCntEnemy].type)
                    {
                        //アンブレラの場合
                    case ENEMYTYPE_UMBRELLA:
                        //右移動中
                        if (g_aEnemy[nCntEnemy].move.x > 0)
                        {
                            nRandom = rand() % 3 + 1;    //1～3を得る

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_UMBRELLA_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_UMBRELLA_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_UMBRELLA_SPEED;
                            }
                        }

                        //左移動中
                        else if (g_aEnemy[nCntEnemy].move.x < 0)
                        {
                            nRandom = rand() % 3 + 1;    //1～3を得る

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_UMBRELLA_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_UMBRELLA_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_UMBRELLA_SPEED;
                            }
                        }

                        //奥移動中
                        else if (g_aEnemy[nCntEnemy].move.z > 0)
                        {
                            nRandom = rand() % 3 + 1;    //1～3を得る

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_UMBRELLA_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_UMBRELLA_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_UMBRELLA_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                        }

                        //手前移動中
                        else if (g_aEnemy[nCntEnemy].move.z < 0)
                        {
                            nRandom = rand() % 3 + 1;    //1～3を得る

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_UMBRELLA_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_UMBRELLA_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_UMBRELLA_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                        }

                        break;

                        //ゴーストの場合
                    case ENEMYTYPE_GHOST:
                        //右移動中
                        if (g_aEnemy[nCntEnemy].move.x > 0)
                        {
                            nRandom = rand() % 3 + 1;    //1～3を得る

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_GHOST_SPEED;
                            }
                        }

                        //左移動中
                        else if (g_aEnemy[nCntEnemy].move.x < 0)
                        {
                            nRandom = rand() % 3 + 1;    //1～3を得る

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_GHOST_SPEED;
                            }
                        }

                        //奥移動中
                        else if (g_aEnemy[nCntEnemy].move.z > 0)
                        {
                            nRandom = rand() % 3 + 1;    //1～3を得る

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                        }

                        //手前移動中
                        else if (g_aEnemy[nCntEnemy].move.z < 0)
                        {
                            nRandom = rand() % 3 + 1;    //1～3を得る

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                        }

                        break;

                        //レッドゴーストの場合
                    case ENEMYTYPE_RED_GHOST:
                        //右移動中
                        if (g_aEnemy[nCntEnemy].move.x > 0)
                        {
                            nRandom = rand() % 3 + 1;    //1～3を得る

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_RED_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_RED_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_RED_GHOST_SPEED;
                            }
                        }

                        //左移動中
                        else if (g_aEnemy[nCntEnemy].move.x < 0)
                        {
                            nRandom = rand() % 3 + 1;    //1～3を得る

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_RED_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_RED_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_RED_GHOST_SPEED;
                            }
                        }

                        //奥移動中
                        else if (g_aEnemy[nCntEnemy].move.z > 0)
                        {
                            nRandom = rand() % 3 + 1;    //1～3を得る

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = -ENEMY_RED_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_RED_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_RED_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                        }

                        //手前移動中
                        else if (g_aEnemy[nCntEnemy].move.z < 0)
                        {
                            nRandom = rand() % 3 + 1;    //1～3を得る

                            if (nRandom == 1)
                            {
                                g_aEnemy[nCntEnemy].move.x = 0.0f;
                                g_aEnemy[nCntEnemy].move.z = ENEMY_RED_GHOST_SPEED;
                            }
                            else if (nRandom == 2)
                            {
                                g_aEnemy[nCntEnemy].move.x = ENEMY_RED_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                            else if (nRandom == 3)
                            {
                                g_aEnemy[nCntEnemy].move.x = -ENEMY_RED_GHOST_SPEED;
                                g_aEnemy[nCntEnemy].move.z = 0.0f;
                            }
                        }

                        break;
                    }  
                }

                //爆弾をすり抜けられる時間のカウンター
                if (g_aEnemy[nCntEnemy].nCntDisabledBomb > 0)
                {
                    g_aEnemy[nCntEnemy].nCntDisabledBomb--;
                }

                //爆弾の当たり判定
                if (g_aEnemy[nCntEnemy].nCntDisabledBomb == 0)
                {
                    if (CollisionBomb(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld,
                        D3DXVECTOR3(GRID_SIZE, GRID_SIZE * 2, GRID_SIZE)) == true)
                    {
                        g_aEnemy[nCntEnemy].nCntDisabledBomb = ENEMY_DISABLED_BOMB_FRAME;   //爆弾をすり抜けられるフレーム数

                        int nRandom = 0;

                        //タイプごとに場合分け
                        switch (g_aEnemy[nCntEnemy].type)
                        {
                            //アンブレラの場合
                        case ENEMYTYPE_UMBRELLA:
                            //右移動中
                            if (g_aEnemy[nCntEnemy].move.x > 0)
                            {
                                nRandom = rand() % 3 + 1;    //1～3を得る

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_UMBRELLA_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_UMBRELLA_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_UMBRELLA_SPEED;
                                }
                            }

                            //左移動中
                            else if (g_aEnemy[nCntEnemy].move.x < 0)
                            {
                                nRandom = rand() % 3 + 1;    //1～3を得る

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_UMBRELLA_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_UMBRELLA_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_UMBRELLA_SPEED;
                                }
                            }

                            //奥移動中
                            else if (g_aEnemy[nCntEnemy].move.z > 0)
                            {
                                nRandom = rand() % 3 + 1;    //1～3を得る

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_UMBRELLA_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_UMBRELLA_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_UMBRELLA_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                            }

                            //手前移動中
                            else if (g_aEnemy[nCntEnemy].move.z < 0)
                            {
                                nRandom = rand() % 3 + 1;    //1～3を得る

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_UMBRELLA_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_UMBRELLA_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_UMBRELLA_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                            }

                            break;

                            //ゴーストの場合
                        case ENEMYTYPE_GHOST:
                            //右移動中
                            if (g_aEnemy[nCntEnemy].move.x > 0)
                            {
                                nRandom = rand() % 3 + 1;    //1～3を得る

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_GHOST_SPEED;
                                }
                            }

                            //左移動中
                            else if (g_aEnemy[nCntEnemy].move.x < 0)
                            {
                                nRandom = rand() % 3 + 1;    //1～3を得る

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_GHOST_SPEED;
                                }
                            }

                            //奥移動中
                            else if (g_aEnemy[nCntEnemy].move.z > 0)
                            {
                                nRandom = rand() % 3 + 1;    //1～3を得る

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                            }

                            //手前移動中
                            else if (g_aEnemy[nCntEnemy].move.z < 0)
                            {
                                nRandom = rand() % 3 + 1;    //1～3を得る

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                            }

                            break;

                            //レッドゴーストの場合
                        case ENEMYTYPE_RED_GHOST:
                            //右移動中
                            if (g_aEnemy[nCntEnemy].move.x > 0)
                            {
                                nRandom = rand() % 3 + 1;    //1～3を得る

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_RED_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_RED_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_RED_GHOST_SPEED;
                                }
                            }

                            //左移動中
                            else if (g_aEnemy[nCntEnemy].move.x < 0)
                            {
                                nRandom = rand() % 3 + 1;    //1～3を得る

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_RED_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_RED_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_RED_GHOST_SPEED;
                                }
                            }

                            //奥移動中
                            else if (g_aEnemy[nCntEnemy].move.z > 0)
                            {
                                nRandom = rand() % 3 + 1;    //1～3を得る

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = -ENEMY_RED_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_RED_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_RED_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                            }

                            //手前移動中
                            else if (g_aEnemy[nCntEnemy].move.z < 0)
                            {
                                nRandom = rand() % 3 + 1;    //1～3を得る

                                if (nRandom == 1)
                                {
                                    g_aEnemy[nCntEnemy].move.x = 0.0f;
                                    g_aEnemy[nCntEnemy].move.z = ENEMY_RED_GHOST_SPEED;
                                }
                                else if (nRandom == 2)
                                {
                                    g_aEnemy[nCntEnemy].move.x = ENEMY_RED_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                                else if (nRandom == 3)
                                {
                                    g_aEnemy[nCntEnemy].move.x = -ENEMY_RED_GHOST_SPEED;
                                    g_aEnemy[nCntEnemy].move.z = 0.0f;
                                }
                            }

                            break;
                        }
                    }
                }

                //向きの目的値を、移動の向きと合わせる
                //右
                if (g_aEnemy[nCntEnemy].move.x > 0.0f)
                {
                    g_aEnemy[nCntEnemy].rotDest.y = D3DXToRadian(270);
                }
                //左
                if (g_aEnemy[nCntEnemy].move.x < 0.0f)
                {
                    g_aEnemy[nCntEnemy].rotDest.y = D3DXToRadian(90);
                }
                //奥
                if (g_aEnemy[nCntEnemy].move.z > 0.0f)
                {
                    g_aEnemy[nCntEnemy].rotDest.y = D3DXToRadian(180);
                }
                //手前
                if (g_aEnemy[nCntEnemy].move.z < 0.0f)
                {
                    g_aEnemy[nCntEnemy].rotDest.y = D3DXToRadian(0);
                }
            }

            //炎の当たり判定
            if (HitFire(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, g_aEnemy[nCntEnemy].size.y,
                    GRID_SIZE - COLLISION_AVOID)) == true)
            {
                HitEnemy(nCntEnemy, BOMB_DAMAGE);
            }

            //影を設定
            SetPositionShadow(g_aEnemy[nCntEnemy].nShadow
                , D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 0, g_aEnemy[nCntEnemy].pos.z), g_aEnemy[nCntEnemy].pos.y);

            //死亡判定
            if (g_aEnemy[nCntEnemy].fLife <= 0.0f)
            {
                //やられた時の声
                PlaySound(SOUND_LABEL_SE_EXPLOSION2);

                //種類ごとに、スコア加算
                switch (g_aEnemy[nCntEnemy].type)
                {
                case ENEMYTYPE_CANNON:
                    ScoreCalculation(ENEMY_CANNON_SCORE);
                    break;
                case ENEMYTYPE_UMBRELLA:
                    ScoreCalculation(ENEMY_UMBRELLA_SCORE);
                    break;
                case ENEMYTYPE_GHOST:
                    ScoreCalculation(ENEMY_GHOST_SCORE);
                    break;
                case ENEMYTYPE_SNAKE:
                    ScoreCalculation(ENEMY_SNAKE_SCORE);
                    break;
                case ENEMYTYPE_RED_GHOST:
                    ScoreCalculation(ENEMY_RED_GHOST_SCORE);
                    break;
                }

                //爆発エフェクト
                float fSpeedX = 0.0f;   //速さX
                float fSpeedY = 0.0f;   //速さY
                float fSpeedZ = 0.0f;   //速さZ
                int nPlusMinusX = 0;    //符号X
                int nPlusMinusZ = 0;    //符号Z
                for (int nCnt = 0; nCnt < COUNT_EFFECT_EXPLODE_ITEM; nCnt++)
                {
                    fSpeedX = float(rand() % 200) / 100.0f;
                    fSpeedY = float(rand() % 800 + 300) / 100.0f;
                    fSpeedZ = float(rand() % 200) / 100.0f;
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
                    SetEffect(g_aEnemy[nCntEnemy].pos, move, EFFECT_SIZE_EXPLODE_ITEM, EFFECTTYPE_EXPLOSION);
                }

                //未使用に
                g_aEnemy[nCntEnemy].bUse = false;

                //影の削除
                SHADOW *pShadow = GetShadow();//影の情報を取得
                pShadow[g_aEnemy[nCntEnemy].nShadow].use = false;

                //初期化
                g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aEnemy[nCntEnemy].fDistance = 9999.9f;
            }

        }
    }
}

//=====================================================
// エネミー描画処理
// Author:後藤慎之助
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
        //使用中かつ透明でないなら
        if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].bTransparence == false)
        {
            // ワールドマトリックスの初期化
            D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

            // スケールを反映
            D3DXMatrixScaling(&mtxScale,
                g_aEnemy[nCntEnemy].size.x / g_aEnemy[nCntEnemy].size.x,
                g_aEnemy[nCntEnemy].size.y / g_aEnemy[nCntEnemy].size.y,
                g_aEnemy[nCntEnemy].size.z / g_aEnemy[nCntEnemy].size.z);
            D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxScale);
            // 向きを反映
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_aEnemy[nCntEnemy].rot.y,
                g_aEnemy[nCntEnemy].rot.x,
                g_aEnemy[nCntEnemy].rot.z);
            D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);
            // 位置を反映
            D3DXMatrixTranslation(&mtxTrans,
                g_aEnemy[nCntEnemy].pos.x,
                g_aEnemy[nCntEnemy].pos.y,
                g_aEnemy[nCntEnemy].pos.z);
            D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);


            // ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);
            // 現在のマテリアルを取得する
            pDevice->GetMaterial(&matDef);
            // マテリアルデータへのポインタを取得
            pMat = (D3DXMATERIAL*)g_pBuffMatEnemy[g_aEnemy[nCntEnemy].type]->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)g_nNumMatEnemy[g_aEnemy[nCntEnemy].type]; nCntMat++)
            {
                // マテリアルの設定
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                // モデルパーツ
                g_pMeshEnemy[g_aEnemy[nCntEnemy].type]->DrawSubset(nCntMat);
            }
            // 保持していたマテリアルを戻す
            pDevice->SetMaterial(&matDef);
        }
    }
}

//=============================================================================
// エネミーの設定処理
// Author:後藤慎之助
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE type)
{
    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        if (g_aEnemy[nCntEnemy].bUse == false)
        {
            //設定
            g_aEnemy[nCntEnemy].pos = pos;
            g_aEnemy[nCntEnemy].posOld = pos;
            g_aEnemy[nCntEnemy].move = move;
            g_aEnemy[nCntEnemy].type = type;

            //初期化
            g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
            g_aEnemy[nCntEnemy].nAttackTime = 0;
            g_aEnemy[nCntEnemy].nCntInvincibleFrame = 0;
            g_aEnemy[nCntEnemy].bTransparence = false;
            g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aEnemy[nCntEnemy].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aEnemy[nCntEnemy].rotMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aEnemy[nCntEnemy].bFindPlayer = false;
            g_aEnemy[nCntEnemy].nCntInvincibleFrame = 0;

            //種類ごとに、大きさとライフを設定
            switch (g_aEnemy[nCntEnemy].type)
            {
            case ENEMYTYPE_CANNON:
                g_aEnemy[nCntEnemy].rot.y = D3DXToRadian(90);
                g_aEnemy[nCntEnemy].size = ENEMY_SIZE_CANNON;
                g_aEnemy[nCntEnemy].fLife = ENEMY_LIFE_CANNON;
                break;
            case ENEMYTYPE_UMBRELLA:
                g_aEnemy[nCntEnemy].size = ENEMY_SIZE_UMBRELLA;
                g_aEnemy[nCntEnemy].fLife = ENEMY_LIFE_UMBRELLA;
                break;
            case ENEMYTYPE_GHOST:
                g_aEnemy[nCntEnemy].size = ENEMY_SIZE_GHOST;
                g_aEnemy[nCntEnemy].fLife = ENEMY_LIFE_GHOST;
                break;
            case ENEMYTYPE_SNAKE:
                g_aEnemy[nCntEnemy].rot.y = D3DXToRadian(270);
                g_aEnemy[nCntEnemy].size = ENEMY_SIZE_SNAKE;
                g_aEnemy[nCntEnemy].fLife = ENEMY_LIFE_SNAKE;
                break;
            case ENEMYTYPE_RED_GHOST:
                g_aEnemy[nCntEnemy].size = ENEMY_SIZE_GHOST;
                g_aEnemy[nCntEnemy].fLife = ENEMY_LIFE_RED_GHOST;
                break;
            }

            //使用中にする
            g_aEnemy[nCntEnemy].bUse = true;

            //敵の影の設定
            g_aEnemy[nCntEnemy].nShadow = SetShadow(g_aEnemy[nCntEnemy].pos,g_aEnemy[nCntEnemy].rot,
                GRID_SIZE , g_aEnemy[nCntEnemy].pos.y, D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_ALPHA_MAX));

            break;
        }
    }
}

//=============================================================================
// エネミーの取得
// Author:後藤慎之助
//=============================================================================
ENEMY*GetEnemy(void)
{
    return &g_aEnemy[0];
}

//=============================================================================
// エネミーのダメージ設定
// Author:後藤慎之助
//=============================================================================
void HitEnemy(int nCntEnemy, float fDamage)
{
    //無敵時間でないなら
    if (g_aEnemy[nCntEnemy].nCntInvincibleFrame == 0)
    {
        GAME *pGame = GetGame();

        //ゲームが終わった後、ダメージを受けない
        if (pGame->state == GAMESTATE_NORMAL)
        {
            //ダメージ状態に遷移
            g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;

            //ダメージ計算
            g_aEnemy[nCntEnemy].fLife -= fDamage;

            //無敵時間をつくる
            g_aEnemy[nCntEnemy].nCntInvincibleFrame = ENEMY_INVINCIBLE_FRAME;

            //HPがあるなら
            if (g_aEnemy[nCntEnemy].fLife > 0.0f)
            {
                PlaySound(SOUND_LABEL_SE_TAKE_DAMAGE_VOICE3);    //ダメージを受けた時の声
            }
        }
    }
}

//=====================================================
// エネミーの衝突判定の設定
// Author:後藤慎之助
//=====================================================
bool CollisionEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, bool bPlayer)
{
    bool bCollision = false;  //衝突したかどうか

    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        // 敵が使われたら
        if (g_aEnemy[nCntEnemy].bUse == true)
        {
            //当たり判定を計算
            if (RectangleCollision(pPos, pPosOld, size, &g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].size) == true)
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
                        pPlayer->move.x = pPlayer->pos.x - g_aEnemy[nCntEnemy].posOld.x;
                        pPlayer->move.z = pPlayer->pos.z - g_aEnemy[nCntEnemy].posOld.z;
                        D3DXVec3Normalize(&pPlayer->move, &pPlayer->move);  //ベクトルを正規化
                        pPlayer->move.x = pPlayer->move.x*fKnockback;
                        pPlayer->move.z = pPlayer->move.z*fKnockback;
                    }
                }
            }
        }
    }

    return bCollision;    //衝突したかどうかを返す
}

//=====================================================
// 爆風を止めることができる敵
// Author:後藤慎之助
//=====================================================
bool StopFireEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bCollision = false;  //衝突したかどうか

    for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
    {
        // 敵が使われたら
        if (g_aEnemy[nCntEnemy].bUse == true)
        {
            //爆風を止めることができる敵なら
            if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_CANNON)
            {
                //当たり判定を計算
                if (RectangleCollision(pPos, pPosOld, size, &g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].size) == true)
                {
                    bCollision = true;    //衝突
                }
            }
        }
    }

    return bCollision;    //衝突したかどうかを返す
}