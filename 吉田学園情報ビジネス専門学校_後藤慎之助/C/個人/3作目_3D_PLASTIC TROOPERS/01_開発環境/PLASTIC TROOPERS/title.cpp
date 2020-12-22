//=============================================================================
//
// PLASTIC TROOPERSの処理 [title.cpp] (タイトル)
// Author:後藤慎之助
//
//=============================================================================
#include "title.h"
#include "input.h"		//キー入力処理
#include "fade.h"
#include "campaign.h"
#include "sound.h"
#include "result.h"
#include "manual.h"
#include "select.h"
#include "controller.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLE_UI] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//頂点バッファへのポインタ
int g_nCounterTitle;//アニメーションカウンター
int g_nPatternEnter;//アニメーションパターン

int g_nEnter;	//エンターを押した情報
int g_nEnter2;	//エンターを押した情報
int g_nCntUpDown;//上下にカーソルを動かせるまでの時間
bool g_bUpDown; //上下にカーソルを動かせる
MODE g_nMode;	//モードの情報
LEVEL g_nLevel;		//キャンペーンの難易度
SURVIVALTIME g_nSurvivalTime;	//サバイバル時間

//=============================================================================
//タイトル初期化処理
//=============================================================================
void InitTitle(void)
{
	//変数宣言
	VERTEX_2D *pVtx = NULL;//頂点情報のポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/title000.png", &g_pTextureTitle[0]);			//タイトルの背景
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/PRESS_ANY_BUTTON.png", &g_pTextureTitle[1]);	//プレスボタン
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/CAMPAIGN.png", &g_pTextureTitle[2]);			//キャンペーン
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/はてな.png", &g_pTextureTitle[3]);			//サバイバル
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/はてな.png", &g_pTextureTitle[4]);				//バーサス
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/はてな.png", &g_pTextureTitle[5]);			//プラクティス
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/RANKING.png", &g_pTextureTitle[6]);				//ランキング
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/EXIT.png", &g_pTextureTitle[7]);				//EXIT

	//グローバル変数の初期化
	g_nCounterTitle = 0;
	g_nPatternEnter = 0;
    g_nCntUpDown = 0;
    g_bUpDown = true;

	//エンター、レベルの初期化
	g_nEnter = 0;
	g_nEnter2 = 30;			//次のエンターを押せるまでの時間
	g_nMode = MODE_CAMPAIGN;	//最初のカーソルは、キャンペーン
	g_nLevel = LEVEL_NORMAL;	//最初のカーソルはノーマル
	g_nSurvivalTime = SURVIVALTIME_1m;	//最初のカーソルは1分

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * TITLE_UI,//バッファサイズ
		D3DUSAGE_WRITEONLY,//(固定）
		FVF_VERTEX_2D,//頂点フォーマット
		D3DPOOL_MANAGED,//（固定）
		&g_pVtxBuffTitle,//変数名が変わると変更が必要
		NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

	//頂点座標の設定（右回りで設定する）
	//タイトルの背景
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//プレスボタン
	pVtx[4].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) - (PRESS_ENTER_Y / 2) + 146, 0.0f);
	pVtx[5].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) - (PRESS_ENTER_Y / 2) + 146, 0.0f);
	pVtx[6].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) + (PRESS_ENTER_Y / 2) + 146, 0.0f);
	pVtx[7].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) + (PRESS_ENTER_Y / 2) + 146, 0.0f);

	//キャンペーン
	pVtx[8].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 0.0f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 120.0f, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(SCREEN_WIDTH, 120.0f, 0.0f);

	//サバイバル
	pVtx[12].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 120.0f, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(SCREEN_WIDTH, 120.0f, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 240.0f, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(SCREEN_WIDTH, 240.0f, 0.0f);

	//バーサス
	pVtx[16].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 240.0f, 0.0f);
	pVtx[17].pos = D3DXVECTOR3(SCREEN_WIDTH, 240.0f, 0.0f);
	pVtx[18].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 360.0f, 0.0f);
	pVtx[19].pos = D3DXVECTOR3(SCREEN_WIDTH, 360.0f, 0.0f);

	//プラクティス
	pVtx[20].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 360.0f, 0.0f);
	pVtx[21].pos = D3DXVECTOR3(SCREEN_WIDTH, 360.0f, 0.0f);
	pVtx[22].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 480.0f, 0.0f);
	pVtx[23].pos = D3DXVECTOR3(SCREEN_WIDTH, 480.0f, 0.0f);

	//ランキング
	pVtx[24].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 480.0f, 0.0f);
	pVtx[25].pos = D3DXVECTOR3(SCREEN_WIDTH, 480.0f, 0.0f);
	pVtx[26].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 600.0f, 0.0f);
	pVtx[27].pos = D3DXVECTOR3(SCREEN_WIDTH, 600.0f, 0.0f);

	//EXIT
	pVtx[28].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 600.0f, 0.0f);
	pVtx[29].pos = D3DXVECTOR3(SCREEN_WIDTH, 600.0f, 0.0f);
	pVtx[30].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 3), 720.0f, 0.0f);
	pVtx[31].pos = D3DXVECTOR3(SCREEN_WIDTH, 720.0f, 0.0f);

	//各種共通の設定
	for (int nCntTitle = 0; nCntTitle < TITLE_UI; nCntTitle++)
	{
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャの座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点データアンロック
	g_pVtxBuffTitle->Unlock();

    srand((unsigned int)time(0));	//乱数の種の初期化

	PlaySound(SOUND_LABEL_BGM000);	//タイトルのBGM
}

//=============================================================================
//タイトル更新処理
//=============================================================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx = NULL;	//頂点情報へのポインタ
    DIJOYSTATE2 Controller = GetController();  //コントローラーの情報を取得
	int nFade = GetFade();	//フェードの情報を取得

	//点滅するアニメーション
	g_nCounterTitle++;
	if (g_nCounterTitle == 6)//速さ
	{
		g_nCounterTitle = 0;
		g_nPatternEnter++;
		if (g_nPatternEnter == 9)//透明度のパターン
		{
			g_nPatternEnter = 0;
		}
	}

	//頂点バッファをロック
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	////頂点カラーを設定（0～255の値で設定する）
	if (g_nPatternEnter == 0)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	if (g_nPatternEnter == 1)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 205);
	}

	if (g_nPatternEnter == 2)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	}

	if (g_nPatternEnter == 3)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 105);
	}

	if (g_nPatternEnter == 4)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	}

	if (g_nPatternEnter == 5)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 105);
	}

	if (g_nPatternEnter == 6)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	}

	if (g_nPatternEnter == 7)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 205);
	}

	if (g_nPatternEnter == 8)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//Enterキーを押す前
	if (g_nEnter == 0)
	{
        if (nFade == FADE_NONE)
        {
            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(BUTTON_A) || GetJoypadTrigger(BUTTON_B) ||
                GetJoypadTrigger(BUTTON_X) || GetJoypadTrigger(BUTTON_Y) || GetJoypadTrigger(BUTTON_START))
            {
                //PlaySound(SOUND_LABEL_SE_ENTER);	//プレスエンター音
                g_nEnter = 1;
            }
        }
	}

	//モード選択（選択しているもの以外、透明度を下げる）
	if (g_nEnter == 1)
	{
        //十字キーやスティックでカーソルを動かせるまでのカウンター
        if (g_bUpDown == false)
        {
            g_nCntUpDown++;
            if (g_nCntUpDown > 12)
            {
                g_nCntUpDown = 0;
                g_bUpDown = true;   //アップダウン可能
            }
        }

		switch (g_nMode)
		{
		case MODE_CAMPAIGN:
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			for (int nCntTitle = 0; nCntTitle < 5; nCntTitle++)
			{
				pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

				pVtx += 4;
			}

            if (nFade == FADE_NONE)
            {
                //↓が押された（下移動）
                if (GetKeyboardTrigger(DIK_DOWN) || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bUpDown == true 
                    || Controller.lY > 0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_SURVIVAL;
                    g_bUpDown = false;
                }

                //Enterキーを押した時
                if (g_nEnter2 == 0)
                {
                    if (GetJoypadTrigger(BUTTON_A) || GetJoypadTrigger(BUTTON_B) || GetKeyboardTrigger(DIK_RETURN))
                    {
                        SetFade(MODE_MANUAL);
                    }
                }
            }

			break;

		case MODE_SURVIVAL:
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			for (int nCntTitle = 0; nCntTitle < 4; nCntTitle++)
			{
				pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

				pVtx += 4;
			}

            if (nFade == FADE_NONE)
            {
                //↑が押された（上移動）
                if (GetKeyboardTrigger(DIK_UP) || Controller.rgdwPOV[0] == BUTTON_UP && g_bUpDown == true 
                    || Controller.lY <0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_CAMPAIGN;
                    g_bUpDown = false;
                }

                //↓が押された（下移動）
                else if (GetKeyboardTrigger(DIK_DOWN) || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bUpDown == true 
                    || Controller.lY >0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_VERSUS;
                    g_bUpDown = false;
                }

			}

			break;

		case MODE_VERSUS:
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			for (int nCntTitle = 0; nCntTitle < 3; nCntTitle++)
			{
				pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 55);

				pVtx += 4;
			}

            if (nFade == FADE_NONE)
            {
                //↑が押された（上移動）
                if (GetKeyboardTrigger(DIK_UP) || Controller.rgdwPOV[0] == BUTTON_UP && g_bUpDown == true 
                    || Controller.lY <0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_SURVIVAL;
                    g_bUpDown = false;
                }

                //↓が押された（下移動）
                else if (GetKeyboardTrigger(DIK_DOWN) || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bUpDown == true 
                    || Controller.lY >0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_PRACTICE;
                    g_bUpDown = false;
                }

            }

			break;

		case MODE_PRACTICE:
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			for (int nCntTitle = 0; nCntTitle < 2; nCntTitle++)
			{
				pVtx[24].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[25].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[26].col = D3DCOLOR_RGBA(255, 255, 255, 55);
				pVtx[27].col = D3DCOLOR_RGBA(255, 255, 255, 55);

				pVtx += 4;
			}

            if (nFade == FADE_NONE)
            {
                //↑が押された（上移動）
                if (GetKeyboardTrigger(DIK_UP) || Controller.rgdwPOV[0] == BUTTON_UP && g_bUpDown == true 
                    || Controller.lY <0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_VERSUS;
                    g_bUpDown = false;
                }

                //↓が押された（下移動）
                else if (GetKeyboardTrigger(DIK_DOWN) || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bUpDown == true 
                    || Controller.lY >0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_RANKING;
                    g_bUpDown = false;
                }

            }

			break;

		case MODE_RANKING:
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[24].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[25].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[26].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[27].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			pVtx[28].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[29].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[30].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[31].col = D3DCOLOR_RGBA(255, 255, 255, 55);

            if (nFade == FADE_NONE)
            {
                //↑が押された（上移動）
                if (GetKeyboardTrigger(DIK_UP) || Controller.rgdwPOV[0] == BUTTON_UP && g_bUpDown == true 
                    || Controller.lY <0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_PRACTICE;
                    g_bUpDown = false;
                }

                //↓が押された（下移動）
                else if (GetKeyboardTrigger(DIK_DOWN) || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bUpDown == true 
                    || Controller.lY >0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_EXIT;
                    g_bUpDown = false;
                }

            }

            //Enterキーを押した時
            if (g_nEnter2 == 0 && nFade == FADE_NONE)
            {
                if (GetJoypadTrigger(BUTTON_A) || GetJoypadTrigger(BUTTON_B) || GetKeyboardTrigger(DIK_RETURN))
                {
                    StopSound(SOUND_LABEL_BGM000);	//タイトルのBGMを止める
                    SetFade(MODE_RANKING);
                }
            }

			break;

		case MODE_EXIT:
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[24].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[25].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[26].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[27].col = D3DCOLOR_RGBA(255, 255, 255, 55);

			pVtx[28].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[29].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[30].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[31].col = D3DCOLOR_RGBA(255, 255, 255, 255);

            if (nFade == FADE_NONE)
            {
                //↑が押された（上移動）
                if (GetKeyboardTrigger(DIK_UP) || Controller.rgdwPOV[0] == BUTTON_UP && g_bUpDown == true 
                    || Controller.lY <0 && g_bUpDown == true)
                {
                    //PlaySound(SOUND_LABEL_SE_JUMP);
                    g_nMode = MODE_RANKING;

                    g_bUpDown = false;
                }

            }

            //Enterキーを押した時
            if (g_nEnter2 == 0 && nFade == FADE_NONE)
            {
                if (GetJoypadTrigger(BUTTON_A) || GetJoypadTrigger(BUTTON_B) || GetKeyboardTrigger(DIK_RETURN))
                {
                    SetFade(MODE_EXIT);
                }
            }

			break;
		}

		//頂点データをアンロックする
		g_pVtxBuffTitle->Unlock();

		//次のエンターを押せるまでの時間
		g_nEnter2--;
		if (g_nEnter2 <= 0)
		{
			g_nEnter2 = 0;
		}

	}

}

//=============================================================================
//タイトル終了処理
//=============================================================================
void UninitTitle(void)
{
	//テクスチャの開放
	for (int nCntTitle = 0; nCntTitle < TITLE_UI; nCntTitle++)
	{
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}
//=============================================================================
//タイトル描画処理
//=============================================================================
void DrawTitle(void)
{
	//頂点フォーマットの設定
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

	//テクスチャの設定
	//タイトルの背景
	pDevice->SetTexture(0, g_pTextureTitle[0]);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);

	//Enterキーを押す前
	if (g_nEnter == 0)
	{
		//プレスボタン
		pDevice->SetTexture(0, g_pTextureTitle[1]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 1, NUM_POLYGON);
	}

	//Enterキーを押した後
	if (g_nEnter == 1)
	{
		//各選択肢の描画
		for (int nCntTitle = 2; nCntTitle < 8; nCntTitle++)
		{
			//キャンペーン
			pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTitle, NUM_POLYGON);
		}
	}
}

//=============================================================================
// モード選択の取得
//=============================================================================
MODE GetModeTitle(void)
{
	return g_nMode;
}

//=============================================================================
// キャンペーンの難易度選択の取得
//=============================================================================
LEVEL GetLevelTitle(void)
{
	return g_nLevel;
}

//=============================================================================
// サバイバル時間選択の取得
//=============================================================================
SURVIVALTIME GetSurvivalTitle(void)
{
	return g_nSurvivalTime;
}