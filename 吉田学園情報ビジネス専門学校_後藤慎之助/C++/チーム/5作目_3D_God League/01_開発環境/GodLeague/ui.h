//====================================================================
//
// UIの処理 (ui.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _UI_H_
#define _UI_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================

// バージョン表記
#define VERSION_POS D3DXVECTOR3(1150.0f, 675.0f, 0.0f)
#define VERSION_SIZE D3DXVECTOR3(150.0f,75.0f,0.0f)           

// ゲームの下の緑のやつ
#define GAME_UNDER_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 80.0f, 0.0f))
#define GAME_UNDER_SIZE (D3DXVECTOR3(SCREEN_WIDTH, 160.0f , 0.0f))

// 自分のHP表記
#define UI_HP_PLAYER_POS (D3DXVECTOR3(220.0f, 80.0f, 0.0f))
#define UI_HP_PLAYER_SIZE (D3DXVECTOR3(300.0f, 100.0f, 0.0f))

// 敵のHP表記
#define UI_HP_ENEMY_POS (D3DXVECTOR3(SCREEN_WIDTH - 220.0f, 80.0f, 0.0f))
#define UI_HP_ENEMY_SIZE (D3DXVECTOR3(300.0f, 100.0f, 0.0f))

// あなた表記
#define UI_YOU_POS (D3DXVECTOR3(450.0f, 80.0f, 0.0f))
#define UI_YOU_SIZE (D3DXVECTOR3(100.0f, 100.0f, 0.0f))

// あいて表記
#define UI_ENEMY_POS (D3DXVECTOR3(SCREEN_WIDTH - 450.0f, 80.0f, 0.0f))
#define UI_ENEMY_SIZE (D3DXVECTOR3(100.0f, 100.0f, 0.0f))

// 先攻後攻
#define UI_ORDER_POS (D3DXVECTOR3(0.0f, 75.0f, 0.0f))   // ずらす値
#define UI_ORDER_SIZE (D3DXVECTOR3(100.0f, 80.0f, 0.0f))

// YOUR ATTACK / DEFENCE TURN表記
#define UI_YOUR_TURN_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 25.0f, 0.0f))  // YOUR TURNの位置
#define UI_YOUR_TURN_MAX_SIZE_X (400.0f)                                                   // YOUR TURNの横の最大サイズ
#define UI_YOUR_TURN_MAX_SIZE_Y (200.0f)                                                   // YOUR TURNの縦の最大サイズ
#define UI_YOUR_TURN_EXTEND_RATE_X (10.0f)                                                 // YOUR TURNの横の拡大割合
#define UI_YOUR_TURN_EXTEND_RATE_Y (5.0f)                                                  // YOUR TURNの縦の拡大割合
#define UI_YOUR_TURN_FADE_OUT_COUNT (180)                                                  // YOUR TURNのフェードが始まるカウンタ
#define UI_YOUR_TURN_FADE_OUT_RATE (0.05f)                                                 // YOUR TURNのフェードアウト割合

// 切断表記
#define UI_DISCONNECT_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100.0f, 0.0f)) // 切断の位置
#define UI_DISCONNECT_MAX_SIZE_X (600.0f)                                                   // 切断の横の最大サイズ
#define UI_DISCONNECT_MAX_SIZE_Y (300.0f)                                                   // 切断の縦の最大サイズ
#define UI_DISCONNECT_EXTEND_RATE_X (15.0f)                                                 // 切断の横の拡大割合
#define UI_DISCONNECT_EXTEND_RATE_Y (7.5f)                                                  // 切断の縦の拡大割合

// ロード中の回転
#define UI_NOW_LOADING_IN_GAME_POS (D3DXVECTOR3(SCREEN_WIDTH / 2 + 180.0f, SCREEN_HEIGHT / 2, 0.0f))
#define UI_NOW_LOADING_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)
#define UI_NOW_LOADING_ROT_SPEED 0.03f  // 回転速度

// カード情報の背景
#define UI_CARD_INFO_POS (D3DXVECTOR3(150.0f, 340.0f, 0.0f))
#define UI_CARD_INFO_SIZE (D3DXVECTOR3(225.0f, 325.0f, 0.0f))

// WIN / LOSE 表記
#define UI_JUDGMENT_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))
#define UI_JUDGMENT_SIZE (D3DXVECTOR3(560.0f , 80.0f, 0.0f))

// 敵の使用カード
#define UI_ENEMY_USE_POS (D3DXVECTOR3(SCREEN_WIDTH - 150.0f, 340.0f, 0.0f))
#define UI_ENEMY_USE_SIZE (D3DXVECTOR3(225.0f, 200.0f, 0.0f))

// 遊び方表記
#define UI_MANUAL_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 530.0f, 0.0f))
#define UI_MANUAL_SIZE (D3DXVECTOR3(300.0f, 70.0f, 0.0f))

// 待機中表記
#define UI_WAIT_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))
#define UI_WAIT_SIZE (D3DXVECTOR3(300.0f, 70.0f, 0.0f))

// タイトルロゴ
#define TITLE_LOGO_FIRST_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, -150.0f, 0.0f))  // タイトルロゴの最初の位置
#define TITLE_LOGO_TARGET_POS_Y 150.0f                                       // タイトルロゴの目標位置 
#define TITLE_LOGO_SIZE (D3DXVECTOR3(500.0f, 250.0f, 0.0f))                  // タイトルロゴのサイズ
#define TITLE_LOGO_FLOW_DOWN_SPEED 2.0f                                      // タイトルロゴの流れ落ちる速度

// PLEASE PRESS START
#define PRESS_START_POS D3DXVECTOR3(SCREEN_WIDTH / 2, 650.0f, 0.0f)                 // スタートボタンを押してねの位置
#define PRESS_START_SIZE D3DXVECTOR3(500.0f,100.0f,0.0f)                            // スタートボタンを押してねのサイズ
#define PRESS_START_INACTIVE_COLOR D3DXCOLOR(0.431f, 0.431f, 0.388f, 1.0f)          // スタートボタンを押してねの非アクティブ色
#define PRESS_START_FADE_RATE 0.01f                                                 // スタートボタンを押してねの点滅割合
#define PRESS_START_FADE_OUT_MIN 0.6f                                               // スタートボタンを押してねのフェードアウトの最小値

//================================================
// クラス宣言
//================================================

// UIクラス
class CUI : public CScene2D
{
public:
    // UIの種類
    typedef enum
    {
        TYPE_NONE = 0,	        // 無し
        TYPE_VERSION,           // バージョン
        TYPE_TITLE_LOGO,        // タイトルのロゴ
        TYPE_PRESS_START,       // スタートボタンを押してね
        TYPE_GAME_UNDER,        // ゲームの下のバー
        TYPE_HP,                // HPバー
        TYPE_YOU,               // あなた
        TYPE_ENEMY,             // あいて
        TYPE_FIRST,             // 先攻
        TYPE_SECOND,            // 後攻
        TYPE_GAME_START,        // ゲーム開始
        TYPE_ATTACK_START,      // 攻撃開始
        TYPE_DEFENCE_START,     // 防御開始
        TYPE_CARD_INFO_BG,      // カード情報の背景
        TYPE_YOU_WIN,           // 勝ち
        TYPE_YOU_LOSE,          // 負け
        TYPE_TITLE_UI00,		// 対戦相手を探しています・・・
        TYPE_TITLE_UI01,		// 対戦相手が見つかりました！
        TYPE_TITLE_UI02,		// 対戦相手が見つかりませんでした
        TYPE_TITLE_UI03,		// 吹き出し
        TYPE_TITLE_UI04,        // ロード中に回転するやつ
        TYPE_ENEMY_USE_ATTACK,  // 敵の攻撃
        TYPE_ENEMY_USE_DEFENCE, // 敵の防御
        TYPE_MANUAL,            // 遊び方表記
        TYPE_WAIT,              // 相手を待機中
        TYPE_DISCONNECT,        // 切断
        TYPE_MAX			    // 種類の最大数
    }TYPE;
    CUI();
    ~CUI();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CUI *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, TYPE type);

    void GameFinishFade(void);              // ゲーム終了時に、消すUI
    void Wait(void);                        // 相手を待機中のUI
    D3DXVECTOR3 Extend(D3DXVECTOR3 size);   // 拡大するUI

    TYPE GetType(void) { return m_type; }           // タイプの取得
    void SetDontUse(void) { m_bUse = false; }       // 使わないものの破棄
    
    void DisconnectInAnim(void);                    // アニメーションパートでの、切断対応

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    D3DXVECTOR3 m_move;                                // 移動量
    D3DXCOLOR m_col;                                   // 色
    TYPE m_type;                                       // UIの種類
    float m_fAngle;                                    // 角度
    bool m_bFadeOut;                                   // フェードアウトかどうか
    bool m_bBlinking;                                  // 点滅中かどうか
    int m_nCntTime;                                    // 時間をカウント
    bool m_bUse;                                       // 使用するかどうか
};

#endif