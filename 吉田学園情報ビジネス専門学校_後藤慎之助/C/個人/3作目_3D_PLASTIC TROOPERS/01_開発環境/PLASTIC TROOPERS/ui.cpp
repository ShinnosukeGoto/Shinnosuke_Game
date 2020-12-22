//=============================================================================
//
// PLASTIC TROOPERS [ui.cpp]の処理 (ゲーム画面のUI)
// Author :後藤慎之助
//
//=============================================================================
#include "ui.h"
#include "player.h"
#include "camera.h"
#include "title.h"
#include "effect.h"
#include "boss.h"
//#include "title.h"
//#include "select.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonUI[UITYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonUI = NULL;

UI g_aUI[MAX_UI];   //UIの情報
bool g_bDecrease;   //エナジーが減っているかどうかの判定
bool g_bOutEnergy;   //エナジー切れしているかどうか
int g_nCntHeal;     //エナジー回復が始まるまでのカウンター

//=============================================================================
//UI初期化処理
//=============================================================================
void InitUI(void)
{
    //宣言
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //グローバル変数の初期化
    g_bDecrease = false;
    g_bOutEnergy = false;
    g_nCntHeal = 0;

    //g_nUIWtype = GetWSelect();		//武器選択を取得
    //g_nUIMtype = GetMSelect();		//魔法選択を取得

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/score000.png", &g_pTexturePolygonUI[0]);	//スコアの背景
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/CAMPAIGN.png", &g_pTexturePolygonUI[1]);	//キャンペーン
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/SURVIVAL.png", &g_pTexturePolygonUI[2]);	//サバイバル
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/VERSUS.png", &g_pTexturePolygonUI[3]);		//バーサス
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/PRACTICE.png", &g_pTexturePolygonUI[4]);	//プラクティス
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI005.png", &g_pTexturePolygonUI[5]);		//必殺ゲージの背景
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar003.png", &g_pTexturePolygonUI[6]);		//HP/ENERGYゲージの背景
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ONOFF.png", &g_pTexturePolygonUI[7]);		//ON/OFF
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/HP.png", &g_pTexturePolygonUI[8]);		    //HPの文字
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ENERGY.png", &g_pTexturePolygonUI[9]);	    //ENERGYの文字
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar000.png", &g_pTexturePolygonUI[10]);	    //HPゲージ
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar000.png", &g_pTexturePolygonUI[11]); 	//ENERGYゲージ
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI006.png", &g_pTexturePolygonUI[12]); 	    //必殺ゲージ
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI007.png", &g_pTexturePolygonUI[13]); 	    //必殺の数字
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/katana.png", &g_pTexturePolygonUI[14]); 	//剣
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/zyu.png", &g_pTexturePolygonUI[15]); 	    //銃
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/y.png", &g_pTexturePolygonUI[16]); 	        //Yボタン
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/press_R.png", &g_pTexturePolygonUI[17]); 	//プレスR
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI008.png", &g_pTexturePolygonUI[18]); 	    //スピニングブレード
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI009.png", &g_pTexturePolygonUI[19]); 	    //スプレッドファイア
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar003.png", &g_pTexturePolygonUI[20]); 	//ボスのHPバーの背景
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar000.png", &g_pTexturePolygonUI[21]); 	//ボスのHPバー
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/BOSS.png", &g_pTexturePolygonUI[22]); 	    //BOSSの文字

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX * MAX_UI, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonUI, NULL);	//スコア、タイム、バー、武器、魔法、難易度
    VERTEX_2D *pVtx;	//バックグラウンドの頂点情報へのポインタ

                        //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffPolygonUI->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        g_aUI[nCntUI].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aUI[nCntUI].fSizeX = 0;
        g_aUI[nCntUI].fSizeY = 0;
        g_aUI[nCntUI].nCounterAnim = 0;
        g_aUI[nCntUI].nPatternAnim = 0;
        g_aUI[nCntUI].type = UITYPE_000;
        g_aUI[nCntUI].bUse = false;

        //頂点座標の設定（右回りで設定する）
        pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);

        //rhwの設定（値は1.0で設定する）
        pVtx[0].rhw = 1.0f;
        pVtx[1].rhw = 1.0f;
        pVtx[2].rhw = 1.0f;
        pVtx[3].rhw = 1.0f;

        //頂点カラーを設定（0～255の値で設定する）
        pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

        //テクスチャ座標の設定
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;
    }

    //頂点データをアンロックする
    g_pVtxBuffPolygonUI->Unlock();

}

//=============================================================================
// UI終了処理
//=============================================================================
void UninitUI(void)
{
    //テクスチャの開放
    for (int nCntUI = 0; nCntUI < UITYPE_MAX; nCntUI++)
    {
        if (g_pTexturePolygonUI[nCntUI] != NULL)
        {
            g_pTexturePolygonUI[nCntUI]->Release();
            g_pTexturePolygonUI[nCntUI] = NULL;
        }
    }

    //頂点バッファの開放
    if (g_pVtxBuffPolygonUI != NULL)
    {
        g_pVtxBuffPolygonUI->Release();
        g_pVtxBuffPolygonUI = NULL;
    }
}

//=============================================================================
// UI更新処理
//=============================================================================
void UpdateUI(void)
{
    VERTEX_2D *pVtx;	//バックグラウンドの頂点情報へのポインタ
    PLAYER*pPlayer = GetPlayer();	//自機を取得
    CAMERA*pCamera = GetCamera();   //カメラを取得
    BOSS*pBoss = GetBoss();         //ボスを取得

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffPolygonUI->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

    //エナジーが減少中かどうかの判定
    if (pPlayer->fOldEnergy > pPlayer->fEnergy)
    {
        g_bDecrease = true;
    }
    else
    {
        g_bDecrease = false;
    }

    //エナジー回復カウンタ－
    if (g_bDecrease == false && pPlayer->fEnergy != PLAYER_ENERGY)
    {
        g_nCntHeal++;

        //エナジー切れ中
        if (g_bOutEnergy == true)
        {
            //エナジー切れマーク
            EFFECT*pEffect = GetEffect();
            for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
            {
                //未使用なら
                if (pEffect->bUse == false)
                {
                        SetEffect(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + PLAYER_HEIGHT * 2, pPlayer->pos.z), ENERGYOUT_SIZE
                            , D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECTTYPE_001);
                }
            }

            //エナジー回復開始
            if (g_nCntHeal >= START_HEAL_OUT_ENERGY)
            {
                g_nCntHeal = START_HEAL_OUT_ENERGY;
                pPlayer->fEnergy += VALUE_HEAL_ENERGY;   //エナジー回復
            }
        }

        //エナジー切れしていない
        else
        {
            //エナジー回復開始
            if (g_nCntHeal >= START_HEAL_ENERGY)
            {
                g_nCntHeal = START_HEAL_ENERGY;
                pPlayer->fEnergy += VALUE_HEAL_ENERGY;   //エナジー回復
            }
        }
    }

    //エナジー減少中なら、エナジー回復カウンターをリセット
    else if (g_bDecrease == true)
    {
        g_nCntHeal = 0;
    }

    //HPが250以上にならないようにする
    if (pPlayer->fLife > PLAYER_LIFE)
    {
        pPlayer->fLife = PLAYER_LIFE;
    }

    //エナジーが101以上にならないようにする
    if (pPlayer->fEnergy > PLAYER_ENERGY)
    {
        pPlayer->fEnergy = PLAYER_ENERGY;
    }

    //エナジーが0以下にならないようにする
    if (pPlayer->fEnergy <= 0.0f)
    {
        pPlayer->fEnergy = 0.0f;
        g_bOutEnergy = true;    //エナジー切れ判定
    }
    else
    {
        g_bOutEnergy = false;
    }

    //必殺ゲージの最大
    if (pPlayer->fChanceMeter > PLAYER_CHANCE_METER_MAX)
    {
        //チャンス値の最大
        if (pPlayer->nChance > PLAYER_CHANCE_MAX-2)
        {
            pPlayer->fChanceMeter = PLAYER_CHANCE_METER_MAX;    //メーターマックス
            pPlayer->nChance = PLAYER_CHANCE_MAX;               //チャンス値マックス
        }

        //チャンス値が最大でないなら、チャンス値1アップ
        else
        {
            pPlayer->fChanceMeter = 0.0f;   //リセット
            pPlayer->nChance += 1;          //チャンス値アップ
        }
    }

    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        if (g_aUI[nCntUI].bUse == true)
        {
            //HPゲージ
            if (g_aUI[nCntUI].type == UITYPE_010)
            {
                //頂点座標の設定（横ゲージ）
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fLife / PLAYER_LIFE)),
                    g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fLife / PLAYER_LIFE)),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //必殺使用時とそうでないときで透明度を切り替える
                if (pPlayer->bChanceAttack == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                else
                {
                    //HPに応じてカラーを変える（満タンから半分まで緑）
                    if (pPlayer->fLife <= PLAYER_LIFE && pPlayer->fLife > PLAYER_LIFE / 2)
                    {
                        pVtx[0].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                    }

                    //HPに応じてカラーを変える（半分から4分の1まで黄色）
                    else if (pPlayer->fLife <= PLAYER_LIFE / 2 && pPlayer->fLife > PLAYER_LIFE / 4)
                    {
                        pVtx[0].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                    }

                    //HPに応じてカラーを変える（4分の1からは赤色）
                    else if (pPlayer->fLife <= PLAYER_LIFE / 4)
                    {
                        pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                    }
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2((pPlayer->fLife / PLAYER_LIFE), 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2((pPlayer->fLife / PLAYER_LIFE), 1.0f);
            }

            //ENERGYゲージ
            else if (g_aUI[nCntUI].type == UITYPE_011)
            {
                //頂点座標の設定（横ゲージ）
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fEnergy / PLAYER_ENERGY)),
                    g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fEnergy / PLAYER_ENERGY)),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //必殺使用時とそうでないときで透明度を切り替える
                if (pPlayer->bChanceAttack == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）(青)
                    pVtx[0].col = D3DCOLOR_RGBA(77, 224, 254, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(77, 224, 254, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(77, 224, 254, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(77, 224, 254, 255);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2((pPlayer->fEnergy/ PLAYER_ENERGY), 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2((pPlayer->fEnergy / PLAYER_ENERGY), 1.0f);
            }

            //必殺ゲージ
            else if (g_aUI[nCntUI].type == UITYPE_012)
            {
                //頂点座標の設定（縦ゲージ）
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2) - (g_aUI[nCntUI].fSizeY)* (pPlayer->fChanceMeter / PLAYER_CHANCE_METER_MAX), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2) - (g_aUI[nCntUI].fSizeY)* (pPlayer->fChanceMeter / PLAYER_CHANCE_METER_MAX), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2) , 0.0f);

                //必殺使用時とそうでないときで透明度を切り替える
                if (pPlayer->bChanceAttack == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                else
                {
                    //頂点カラーを設定（0～255の値で設定する）(赤)
                    pVtx[0].col = D3DCOLOR_RGBA(255, 84, 59, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 84, 59, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 84, 59, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 84, 59, 255);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f - (pPlayer->fChanceMeter / PLAYER_CHANCE_METER_MAX));
                pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f - (pPlayer->fChanceMeter / PLAYER_CHANCE_METER_MAX));
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //必殺の数字
            else if (g_aUI[nCntUI].type == UITYPE_013)
            {
                if (pPlayer->nChance == 0)
                {
                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.0f + 0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.0f + 0.25f, 1.0f);
                }
                else if(pPlayer->nChance == 1)
                {
                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.25f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.25f + 0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.25f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f, 1.0f);
                }
                else if (pPlayer->nChance == 2)
                {
                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f + 0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f + 0.25f, 1.0f);
                }
                else if (pPlayer->nChance == 3)
                {
                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.75f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.75f + 0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.75f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.75f + 0.25f, 1.0f);
                }

                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //必殺使用時とそうでないときで透明度を切り替える
                if (pPlayer->bChanceAttack == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                
            }

            //剣
            else if (g_aUI[nCntUI].type == UITYPE_014)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // 必殺使用時とそうでないときで透明度を切り替える
                if (pPlayer->bChanceAttack == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //剣使用時とそうでないときで透明度を切り替える
                    if (pPlayer->bBullet == true)
                    {
                        //頂点カラーを設定（0～255の値で設定する）
                        pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                        pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                        pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                        pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                    }
                    else
                    {
                        //頂点カラーを設定（0～255の値で設定する）
                        pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    }
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //銃
            else if (g_aUI[nCntUI].type == UITYPE_015)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // 必殺使用時とそうでないときで透明度を切り替える
                if (pPlayer->bChanceAttack == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //銃使用時とそうでないときで透明度を切り替える
                    if (pPlayer->bBullet == false)
                    {
                        //頂点カラーを設定（0～255の値で設定する）
                        pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                        pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                        pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                        pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                    }
                    else
                    {
                        //頂点カラーを設定（0～255の値で設定する）
                        pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    }
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //プレスR
            else if (g_aUI[nCntUI].type == UITYPE_017)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // 必殺使用可能時とそうでないときで透明度を切り替える
                if (pPlayer->nChance > 0 && pPlayer->bChanceAttack == false 
                    && pPlayer->nCntEXAttackCD == 0 && pPlayer->fEnergy > 0)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
 
                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //スピニングブレード
            else if (g_aUI[nCntUI].type == UITYPE_018)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //剣必殺使用時とそうでないときで透明度を切り替える
                if (pPlayer->bChanceAttack == true && pPlayer->bBullet == false)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //スプレッドファイア
            else if (g_aUI[nCntUI].type == UITYPE_019)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //剣必殺使用時とそうでないときで透明度を切り替える
                if (pPlayer->bChanceAttack == true && pPlayer->bBullet == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //ON/OFF
            else if (g_aUI[nCntUI].type == UITYPE_007)
            {
                if (pCamera->bLockOn == true)
                {
                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.0f + 0.5f, 1.0f);
                }
                else
                {
                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f + 0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f, 1.0f);
                }

                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //頂点カラーを設定（0～255の値で設定する）
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
            }

            //必殺ゲージの背景
            else if (g_aUI[nCntUI].type == UITYPE_005)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // 必殺使用時とそうでないときで透明度を切り替える
                if (pPlayer->bChanceAttack == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //ゲージの背景
            else if (g_aUI[nCntUI].type == UITYPE_006)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // 必殺使用時とそうでないときで透明度を切り替える
                if (pPlayer->bChanceAttack == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //HP
            else if (g_aUI[nCntUI].type == UITYPE_008)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // 必殺使用時とそうでないときで透明度を切り替える
                if (pPlayer->bChanceAttack == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //エナジー
            else if (g_aUI[nCntUI].type == UITYPE_009)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // 必殺使用時とそうでないときで透明度を切り替える
                if (pPlayer->bChanceAttack == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //Y
            else if (g_aUI[nCntUI].type == UITYPE_016)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // 必殺使用時とそうでないときで透明度を切り替える
                if (pPlayer->bChanceAttack == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //ボスのHPゲージ
            else if (g_aUI[nCntUI].type == UITYPE_021)
            {
                //頂点座標の設定（横ゲージ）
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pBoss->fLife / BOSS_LIFE)),
                    g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pBoss->fLife / BOSS_LIFE)),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //ボス戦時とそうでないときで透明度を切り替える
                if (pCamera->bBossMode == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）(紫)
                    pVtx[0].col = D3DCOLOR_RGBA(188, 78, 189, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(188, 78, 189, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(188, 78, 189, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(188, 78, 189, 255);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2((pBoss->fLife / BOSS_LIFE), 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2((pBoss->fLife / BOSS_LIFE), 1.0f);
            }

            //ボスのHPバーの背景
            else if (g_aUI[nCntUI].type == UITYPE_020)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //ボス戦時とそうでないときで透明度を切り替える
                if (pCamera->bBossMode == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //BOSSの文字
            else if (g_aUI[nCntUI].type == UITYPE_022)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //ボス戦時とそうでないときで透明度を切り替える
                if (pCamera->bBossMode == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            else
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //頂点カラーを設定（0～255の値で設定する）
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

        }
        pVtx += 4;
    }

    //頂点データをアンロックする
    g_pVtxBuffPolygonUI->Unlock();
}

//=============================================================================
// UI描画処理
//=============================================================================
void DrawUI(void)
{
    //宣言
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //アルファテストを有効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //アルファテスト基準値の設定
    pDevice->SetRenderState(D3DRS_ALPHAREF, 30);
    //アルファテストの比較方法の設定（GREATERは基準値より大きい場合)
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffPolygonUI, 0, sizeof(VERTEX_2D));
    //頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    //ポリゴンの描画
    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        if (g_aUI[nCntUI].bUse == true)//敵が使用中なら描画
        {
            //テクスチャの設定
            pDevice->SetTexture(0, g_pTexturePolygonUI[g_aUI[nCntUI].type]);

            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntUI, NUM_POLYGON);
        }
    }

    //アルファテストを無効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    //テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
//UIの設定処理
//=============================================================================
void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, UITYPE type)
{
    //変数宣言
    VERTEX_2D *pVtx;

    //頂点バッファをロック
    g_pVtxBuffPolygonUI->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        if (g_aUI[nCntUI].bUse == false)
        {
            //位置を設定
            g_aUI[nCntUI].pos = pos;
            g_aUI[nCntUI].fSizeX = fSizeX;
            g_aUI[nCntUI].fSizeY = fSizeY;
            g_aUI[nCntUI].type = type;
            g_aUI[nCntUI].bUse = true;
            break;
        }
        pVtx += 4;
    }

    //頂点データをアンロックする
    g_pVtxBuffPolygonUI->Unlock();

}