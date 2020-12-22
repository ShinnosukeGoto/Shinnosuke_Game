//=============================================================================
//
// ナインストーリーズの処理 [sound.h] (サウンド)
// Author:後藤慎之助
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイルの構造体定義
//*****************************************************************************
typedef enum
{
    SOUND_LABEL_BGM000 = 0,		// BGM0 タイトル
    SOUND_LABEL_BGM001,			// BGM1 ゲーム
    SOUND_LABEL_BGM002,			// BGM2 ハッカー
    SOUND_LABEL_BGM003,			// BGM3 リザルト
    SOUND_LABEL_BGM004,			// BGM4 ボス
    SOUND_LABEL_SE_ENTER,		// エンター音
    SOUND_LABEL_SE_JUMP,		// ジャンプ音/選択音
    SOUND_LABEL_SE_EXPLOSION2,	// 雪の音
    SOUND_LABEL_SE_SWORD,		// 剣、槍の音
    SOUND_LABEL_SE_SHOT,		// 弓の音
    SOUND_LABEL_SE_EXPLOSION,	// ヒット音
    SOUND_LABEL_SE_STAR,		// きら-ん音
    SOUND_LABEL_SE_ITEM,		// アイテム取得/回復音/魔法音
    SOUND_LABEL_SE_VOICE,		// ボス撃破音
    SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif