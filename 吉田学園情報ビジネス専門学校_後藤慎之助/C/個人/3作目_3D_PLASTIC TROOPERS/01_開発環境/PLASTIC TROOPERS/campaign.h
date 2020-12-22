//=============================================================================
//
// PLASTIC TROOPERS [campaign.h]の処理 (ボスを倒しに行くモード)
// Author:後藤慎之助
//
//=============================================================================
#ifndef _CAMPAIGN_H_
#define _CAMPAIGN_H_
#define _CRT_SECURE_NO_WARNINGS_

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
// 構造体定義
//=============================================================================
typedef enum
{
	CAMPAIGNSTATE_NONE,
	CAMPAIGNSTATE_NORMAL,
    CAMPAIGNSTATE_PAUSE,
	CAMPAIGNSTATE_NEXTSTAGE,	//次のステージへ
	CAMPAIGNSTATE_GAMEOVER,	    //ゲームオーバー
	CAMPAIGNSTATE_MAX
}CAMPAIGNSTATE;

//=============================================================================
//プロトタイプ宣言
//=============================================================================
void InitCampaign(void);
void UpdateCampaign(void);
void UninitCampaign(void);
void DrawCampaign(void);

void SetCampaignState(CAMPAIGNSTATE state);//ゲームの状態
#endif