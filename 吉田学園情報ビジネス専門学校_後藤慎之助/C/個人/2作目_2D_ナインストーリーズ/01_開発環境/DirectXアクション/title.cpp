//=============================================================================
//
// ナインストーリーズの処理 [title.cpp] (タイトル)
// Author:後藤慎之助
//
//=============================================================================
#include "title.h"
#include "input.h"		//キー入力処理
#include "bg.h"
#include "fade.h"
#include "game.h"
#include "sound.h"
#include "result.h"
#include "manual.h"
#include "select.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLE_UI] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//頂点バッファへのポインタ
int g_nCounterTitle;//アニメーションカウンター
int g_nPatternEnter;//アニメーションパターン

int g_nEnter;	//エンターを押した情報
int g_nEnter2;	//エンターを押した情報
GAMELEVEL g_nLevel;	//レベルの情報

//=============================================================================
//タイトル初期化処理
//=============================================================================
void InitTitle(void)
{
	//変数宣言
	VERTEX_2D*pVtx;//頂点情報のポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/title000.png", &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/press_enter.png", &g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/NORMAL000.png", &g_pTextureTitle[2]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/HARD000.png", &g_pTextureTitle[3]);

	//グローバル変数の初期化
	g_nCounterTitle = 0;
	g_nPatternEnter = 0;

	//エンター、レベルの初期化
	g_nEnter = 0;
	g_nEnter2 = 30;		//次のエンターを押せるまでの時間
	g_nLevel = GAMELEVEL_NORMAL;

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
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//プレスエンターの位置
	pVtx[4].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) - (PRESS_ENTER_Y / 2) + 146, 0.0f);
	pVtx[5].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) - (PRESS_ENTER_Y / 2) + 146, 0.0f);
	pVtx[6].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) + (PRESS_ENTER_Y / 2) + 146, 0.0f);
	pVtx[7].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) + (PRESS_ENTER_Y / 2) + 146, 0.0f);

	//NORMALの位置
	pVtx[8].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (NORMAL_HARD_X / 2) - 300, (SCREEN_HEIGHT / 1.5) - (NORMAL_HARD_Y / 2) + 146, 0.0f);
	pVtx[9].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (NORMAL_HARD_X / 2) - 300, (SCREEN_HEIGHT / 1.5) - (NORMAL_HARD_Y / 2) + 146, 0.0f);
	pVtx[10].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (NORMAL_HARD_X / 2) - 300, (SCREEN_HEIGHT / 1.5) + (NORMAL_HARD_Y / 2) + 146, 0.0f);
	pVtx[11].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (NORMAL_HARD_X / 2) - 300, (SCREEN_HEIGHT / 1.5) + (NORMAL_HARD_Y / 2) + 146, 0.0f);

	//HARDの位置
	pVtx[12].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (NORMAL_HARD_X / 2) + 300, (SCREEN_HEIGHT / 1.5) - (NORMAL_HARD_Y / 2) + 146, 0.0f);
	pVtx[13].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (NORMAL_HARD_X / 2) + 300, (SCREEN_HEIGHT / 1.5) - (NORMAL_HARD_Y / 2) + 146, 0.0f);
	pVtx[14].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (NORMAL_HARD_X / 2) + 300, (SCREEN_HEIGHT / 1.5) + (NORMAL_HARD_Y / 2) + 146, 0.0f);
	pVtx[15].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (NORMAL_HARD_X / 2) + 300, (SCREEN_HEIGHT / 1.5) + (NORMAL_HARD_Y / 2) + 146, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	pVtx[12].rhw = 1.0f;
	pVtx[13].rhw = 1.0f;
	pVtx[14].rhw = 1.0f;
	pVtx[15].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[8].col = D3DCOLOR_RGBA(0, 0, 0, 255);
	pVtx[9].col = D3DCOLOR_RGBA(0, 0, 0, 255);
	pVtx[10].col = D3DCOLOR_RGBA(0, 0, 0, 255);
	pVtx[11].col = D3DCOLOR_RGBA(0, 0, 0, 255);

	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 155);

	//テクスチャの座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データアンロック
	g_pVtxBuffTitle->Unlock();

	PlaySound(SOUND_LABEL_BGM000);	//タイトルのBGM
}
//=============================================================================
//更新処理
//=============================================================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nFade = GetFade();	//フェードの情報を獲得

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

	//ノーマル選択時
	if (g_nLevel == GAMELEVEL_NORMAL)
	{
		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	}

	//ハード選択時
	if (g_nLevel == GAMELEVEL_HARD)
	{
		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//頂点データをアンロックする
	g_pVtxBuffTitle->Unlock();

	//Enterキーを押す前
	if (g_nEnter == 0)
	{
		if (GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_ENTER);	//プレスエンター音
			g_nEnter = 1;
		}
	}

	//難易度選択
	if (g_nEnter == 1)
	{
		//Aキーが押された（左移動,ノーマル）
		if (GetKeyboardTrigger(DIK_A) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_nLevel = GAMELEVEL_NORMAL;
		}
		//Dキーが押された（右移動,ハード）
		if (GetKeyboardTrigger(DIK_D) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_nLevel = GAMELEVEL_HARD;
		}
		//←キーが押された（左移動,ノーマル）
		if (GetKeyboardTrigger(DIK_LEFT) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_nLevel = GAMELEVEL_NORMAL;
		}
		//→キーが押された（右移動,ハード）
		if (GetKeyboardTrigger(DIK_RIGHT) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			g_nLevel = GAMELEVEL_HARD;
		}

		//次のエンターを押せるまでの時間
		g_nEnter2--;
		if (g_nEnter2 <= 0)
		{
			g_nEnter2 = 0;
		}

		//Enterキーを押した時
		if (g_nEnter2 == 0 && GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_ENTER);	//プレスエンター音
			InitGameFirst();					//ゲームの初期化1
			InitSelect();						//セレクト画面の初期化
			SetFade(MODE_SELECT);				//セレクト画面に移動
		}
	}
}
//=============================================================================
//終了処理
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
//描画処理
//=============================================================================
void DrawTitle(void)
{
	//頂点フォーマットの設定
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitle[0]);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);

	//Enterキーを押す前
	if (g_nEnter == 0)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[1]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 1, NUM_POLYGON);
	}

	//Enterキーを押した後
	if (g_nEnter == 1)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[2]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 2, NUM_POLYGON);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[3]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 3, NUM_POLYGON);
	}
}

//=============================================================================
// 難易度の取得
//=============================================================================
GAMELEVEL GetTitle(void)
{
	return g_nLevel;
}