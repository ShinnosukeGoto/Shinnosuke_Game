//=============================================================================
//
// ナインストーリーズの処理 [select.cpp] (武器と魔法の選択)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "input.h"
#include "game.h"
#include "select.h"
#include "fade.h"
#include "sound.h"
#include "manual.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureSelect[SELECT_UI] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelect = NULL;//頂点バッファへのポインタ

WSELECT g_nWeapon;	//武器
MSELECT g_nMagic;	//魔法
DIRECT g_nSelectPos;//選択カーソルの位置
//=============================================================================
// セレクト初期化処理
//=============================================================================
void InitSelect(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//選択の初期化
	g_nWeapon = WSELECT_SWORD;	//最初のカーソルは剣
	g_nMagic = MSELECT_FIRE;	//最初のカーソルは炎
	g_nSelectPos = DIRECT_UP;	//最初のカーソルは上

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select999.png", &g_pTextureSelect[0]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select000.png", &g_pTextureSelect[1]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select001.png", &g_pTextureSelect[2]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select002.png", &g_pTextureSelect[3]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select003.png", &g_pTextureSelect[4]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select004.png", &g_pTextureSelect[5]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select005.png", &g_pTextureSelect[6]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select100.png", &g_pTextureSelect[7]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select100.png", &g_pTextureSelect[8]);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX*SELECT_UI,//バッファサイズ
		D3DUSAGE_WRITEONLY,//(固定）
		FVF_VERTEX_2D,//頂点フォーマット
		D3DPOOL_MANAGED,//（固定）
		&g_pVtxBuffSelect,//変数名が変わると変更が必要
		NULL);
	VERTEX_2D*pVtx;//頂点情報のポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

	//頂点座標の設定（右回りで設定する）
	//セレクトの背景
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	//剣
	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(426.6f, 0.0f,0.0f);
	pVtx[6].pos = D3DXVECTOR3(0.0f, 360.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(426.3f, 360.0f, 0.0f);
	//槍
	pVtx[8].pos = D3DXVECTOR3(426.6f, 0.0f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(853.3f, 0.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(426.6f, 360.0f, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(853.3f, 360.0f, 0.0f);
	//弓
	pVtx[12].pos = D3DXVECTOR3(853.3f, 0.0f, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(853.3f, 360.0f, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(SCREEN_WIDTH, 360.0f, 0.0f);
	//炎
	pVtx[16].pos = D3DXVECTOR3(0.0f, 360.0f, 0.0f);
	pVtx[17].pos = D3DXVECTOR3(426.6f, 360.0f, 0.0f);
	pVtx[18].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[19].pos = D3DXVECTOR3(426.3f, SCREEN_HEIGHT, 0.0f);
	//雷
	pVtx[20].pos = D3DXVECTOR3(426.6f, 360.0f, 0.0f);
	pVtx[21].pos = D3DXVECTOR3(853.3f, 360.0f, 0.0f);
	pVtx[22].pos = D3DXVECTOR3(426.6f, SCREEN_HEIGHT, 0.0f);
	pVtx[23].pos = D3DXVECTOR3(853.3f, SCREEN_HEIGHT, 0.0f);
	//吸収
	pVtx[24].pos = D3DXVECTOR3(853.3f, 360.0f, 0.0f);
	pVtx[25].pos = D3DXVECTOR3(SCREEN_WIDTH, 360.0f, 0.0f);
	pVtx[26].pos = D3DXVECTOR3(853.3f, SCREEN_HEIGHT, 0.0f);
	pVtx[27].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	//黒い背景（上）
	pVtx[28].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[29].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[30].pos = D3DXVECTOR3(0.0f, 360, 0.0f);
	pVtx[31].pos = D3DXVECTOR3(SCREEN_WIDTH, 360, 0.0f);
	//黒い背景（下）
	pVtx[32].pos = D3DXVECTOR3(0.0f, 360, 0.0f);
	pVtx[33].pos = D3DXVECTOR3(SCREEN_WIDTH, 360, 0.0f);
	pVtx[34].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[35].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	pVtx[16].rhw = 1.0f;
	pVtx[17].rhw = 1.0f;
	pVtx[18].rhw = 1.0f;
	pVtx[19].rhw = 1.0f;
	pVtx[20].rhw = 1.0f;
	pVtx[21].rhw = 1.0f;
	pVtx[22].rhw = 1.0f;
	pVtx[23].rhw = 1.0f;
	pVtx[24].rhw = 1.0f;
	pVtx[25].rhw = 1.0f;
	pVtx[26].rhw = 1.0f;
	pVtx[27].rhw = 1.0f;
	pVtx[28].rhw = 1.0f;
	pVtx[29].rhw = 1.0f;
	pVtx[30].rhw = 1.0f;
	pVtx[31].rhw = 1.0f;
	pVtx[32].rhw = 1.0f;
	pVtx[33].rhw = 1.0f;
	pVtx[34].rhw = 1.0f;
	pVtx[35].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

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

	pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	pVtx[24].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[25].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[26].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[27].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	pVtx[28].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[29].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[30].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[31].col = D3DCOLOR_RGBA(255, 255, 255, 0);

	pVtx[32].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	pVtx[33].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	pVtx[34].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	pVtx[35].col = D3DCOLOR_RGBA(255, 255, 255, 155);

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
	pVtx[16].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[20].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[21].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[22].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[23].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[24].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[25].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[26].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[27].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[28].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[29].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[30].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[31].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[32].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[33].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[34].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[35].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データアンロック
	g_pVtxBuffSelect->Unlock();
}
//=============================================================================
// セレクト更新処理
//=============================================================================
void UpdateSelect(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nFade = GetFade();	//フェードの情報を獲得

	//頂点バッファをロック
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	//武器選択時
	if (g_nSelectPos == DIRECT_UP)
	{
		//上の黒い背景
		pVtx[28].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[29].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[30].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[31].col = D3DCOLOR_RGBA(255, 255, 255, 0);

		//下の黒い背景
		pVtx[32].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[33].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[34].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[35].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	}

	//魔法選択時
	else if (g_nSelectPos == DIRECT_DOWN)
	{
		//上の黒い背景
		pVtx[28].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[29].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[30].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[31].col = D3DCOLOR_RGBA(255, 255, 255, 155);

		//下の黒い背景
		pVtx[32].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[33].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[34].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[35].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	}

	//剣選択時、剣が明るい
	if (g_nWeapon == WSELECT_SWORD)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	}

	//槍選択時、槍が明るい
	else if (g_nWeapon == WSELECT_LANCE)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	}

	//弓選択時、弓が明るい
	else if (g_nWeapon == WSELECT_BOW)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//炎選択時、炎が明るい
	if (g_nMagic == MSELECT_FIRE)
	{
		pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[24].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[25].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[26].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[27].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	}

	//雷選択時、雷が明るい
	else if (g_nMagic == MSELECT_LIGHTNING)
	{
		pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[24].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[25].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[26].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[27].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	}

	//吸収選択時、吸収が明るい
	else if (g_nMagic == MSELECT_DRAIN)
	{
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
	}


	//頂点データをアンロックする
	g_pVtxBuffSelect->Unlock();

	//Wキーが押された（上移動,武器選択）
	if (GetKeyboardTrigger(DIK_W) && nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_JUMP);
		g_nSelectPos = DIRECT_UP;
	}
	//Sキーが押された（下移動,魔法選択）
	else if (GetKeyboardTrigger(DIK_S) && nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_JUMP);
		g_nSelectPos = DIRECT_DOWN;
	}
	//↑キーが押された（上移動,武器選択）
	else if (GetKeyboardTrigger(DIK_UP) && nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_JUMP);
		g_nSelectPos = DIRECT_UP;
	}
	//↓キーが押された（下移動,魔法選択）
	else if (GetKeyboardTrigger(DIK_DOWN) && nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_JUMP);
		g_nSelectPos = DIRECT_DOWN;
	}

	//武器選択
	if (g_nSelectPos == DIRECT_UP)
	{
		//Aキーが押された（左移動）
		if (GetKeyboardTrigger(DIK_A) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nWeapon == WSELECT_LANCE)
			{
				g_nWeapon = WSELECT_SWORD;
			}
			else if (g_nWeapon == WSELECT_BOW)
			{
				g_nWeapon = WSELECT_LANCE;
			}
		}
		//Dキーが押された（右移動）
		else if (GetKeyboardTrigger(DIK_D) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nWeapon == WSELECT_SWORD)
			{
				g_nWeapon = WSELECT_LANCE;
			}
			else if (g_nWeapon == WSELECT_LANCE)
			{
				g_nWeapon = WSELECT_BOW;
			}
		}
		//←キーが押された（左移動）
		else if (GetKeyboardTrigger(DIK_LEFT) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nWeapon == WSELECT_LANCE)
			{
				g_nWeapon = WSELECT_SWORD;
			}
			else if (g_nWeapon == WSELECT_BOW)
			{
				g_nWeapon = WSELECT_LANCE;
			}
		}
		//→キーが押された（右移動）
		else if (GetKeyboardTrigger(DIK_RIGHT) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nWeapon == WSELECT_SWORD)
			{
				g_nWeapon = WSELECT_LANCE;
			}
			else if (g_nWeapon == WSELECT_LANCE)
			{
				g_nWeapon = WSELECT_BOW;
			}
		}
	}

	//魔法選択
	else if (g_nSelectPos == DIRECT_DOWN)
	{
		//Aキーが押された（左移動）
		if (GetKeyboardTrigger(DIK_A) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nMagic == MSELECT_LIGHTNING)
			{
				g_nMagic = MSELECT_FIRE;
			}
			else if (g_nMagic == MSELECT_DRAIN)
			{
				g_nMagic = MSELECT_LIGHTNING;
			}
		}
		//Dキーが押された（右移動）
		else if (GetKeyboardTrigger(DIK_D) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nMagic == MSELECT_FIRE)
			{
				g_nMagic = MSELECT_LIGHTNING;
			}
			else if (g_nMagic == MSELECT_LIGHTNING)
			{
				g_nMagic = MSELECT_DRAIN;
			}
		}
		//←キーが押された（左移動）
		else if (GetKeyboardTrigger(DIK_LEFT) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nMagic == MSELECT_LIGHTNING)
			{
				g_nMagic = MSELECT_FIRE;
			}
			else if (g_nMagic == MSELECT_DRAIN)
			{
				g_nMagic = MSELECT_LIGHTNING;
			}
		}
		//→キーが押された（右移動）
		else if (GetKeyboardTrigger(DIK_RIGHT) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nMagic == MSELECT_FIRE)
			{
				g_nMagic = MSELECT_LIGHTNING;
			}
			else if (g_nMagic == MSELECT_LIGHTNING)
			{
				g_nMagic = MSELECT_DRAIN;
			}
		}
	}

	//Enterキーを押した時
	//武器選択中は、魔法選択に移行する
	if (g_nSelectPos == DIRECT_UP && GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_JUMP);
		g_nSelectPos = DIRECT_DOWN;
	}

	//魔法選択中に押すと、マニュアル画面へ移行
	else if (g_nSelectPos == DIRECT_DOWN && GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音でプレスエンター音の代用
		InitManual();		//マニュアル画面の初期化
		InitGameSecond();	//ゲームの初期化2
		SetFade(MODE_MANUAL);//マニュアル画面に移動
	}
}
//=============================================================================
// セレクト終了処理
//=============================================================================
void UninitSelect(void)
{
	//テクスチャの開放
	for (int nCntSelect = 0; nCntSelect < SELECT_UI; nCntSelect++)
	{
		if (g_pTextureSelect[nCntSelect] != NULL)
		{
			g_pTextureSelect[nCntSelect]->Release();
			g_pTextureSelect[nCntSelect] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffSelect != NULL)
	{
		g_pVtxBuffSelect->Release();
		g_pVtxBuffSelect = NULL;
	}
}
//=============================================================================
// セレクト描画処理
//=============================================================================
void DrawSelect(void)
{
	//頂点フォーマットの設定
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffSelect, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

	for (int nCntSelect = 0; nCntSelect < SELECT_UI; nCntSelect++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureSelect[nCntSelect]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntSelect, NUM_POLYGON);
	}
}

//=============================================================================
// 武器選択の取得
//=============================================================================
WSELECT GetWSelect(void)
{
	return g_nWeapon;
}

//=============================================================================
// 魔法選択の取得
//=============================================================================
MSELECT GetMSelect(void)
{
	return g_nMagic;
}