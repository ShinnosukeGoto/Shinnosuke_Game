//=============================================================================
//
// 最強昔話決定戦の処理 [sound.h] (サウンド)
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
    SOUND_LABEL_BGM_TITLE = 0,	    // BGM タイトル
    SOUND_LABEL_BGM_MENU,		    // BGM メニュー
    SOUND_LABEL_BGM_GAME,           // BGM ゲーム
    SOUND_LABEL_BGM_SUDDEN_DEATH,   // BGM サドンデス
    SOUND_LABEL_BGM_RESULT,			// BGM リザルト
    SOUND_LABEL_SE_SELECT,		    // 選択音
    SOUND_LABEL_SE_DECISION,        // 決定音
    SOUND_LABEL_SE_EXPLOSION,	    // 爆発音
    SOUND_LABEL_SE_SET_BOMB, 	    // ボム設置音
    SOUND_LABEL_SE_MAN_VOICE,		// 男性がやられた時のボイス
    SOUND_LABEL_SE_ITEM,	        // アイテム取得音
    SOUND_LABEL_SE_START,		    // 開始音
    SOUND_LABEL_SE_WARNING,		    // 警告音
    SOUND_LABEL_SE_FINISH,		    // 試合終了音
    SOUND_LABEL_SE_KICK,		    // ボムキック音
    SOUND_LABEL_SE_WOMAN_VOICE,		// 女性がやられた時のボイス
    SOUND_LABEL_SE_PINOCCHIO_VOICE,	// ピノキオがやられた時のボイス
    SOUND_LABEL_SE_MAN_VOICE2,		// 男性がやられた時のボイス2
    SOUND_LABEL_SE_START_MAN_VOICE,	    // 開始の掛け声(男)
    SOUND_LABEL_SE_START_MAN_VOICE2,    // 開始の掛け声(男2)
    SOUND_LABEL_SE_START_MAN_VOICE3,    // 開始の掛け声(男3)
    SOUND_LABEL_SE_START_WOMAN_VOICE,   // 開始の掛け声(女)
    SOUND_LABEL_SE_TAKE_DAMAGE_VOICE,   // ダメージを受けた時の声
    SOUND_LABEL_SE_TAKE_DAMAGE_VOICE2,  // ダメージを受けた時の声2
    SOUND_LABEL_SE_TAKE_DAMAGE_VOICE3,  // ダメージを受けた時の声3
    SOUND_LABEL_SE_TAKE_DAMAGE_VOICE4,  // ダメージを受けた時の声4
    SOUND_LABEL_SE_EXPLOSION2,	        // モンスターの爆発音
    SOUND_LABEL_SE_ROAR,	            // ボスの咆哮
    SOUND_LABEL_SE_BOSS_EXPLOSION,	    // ボスの爆発音
    SOUND_LABEL_SE_JUMP,	            // ジャンプ音
    SOUND_LABEL_SE_FALL,	            // 落下音
    SOUND_LABEL_SE_GROUND,	            // 地響き
    SOUND_LABEL_SE_SHOT,                // ショット音
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