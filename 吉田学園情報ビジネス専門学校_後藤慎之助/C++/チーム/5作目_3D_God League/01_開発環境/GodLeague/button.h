//====================================================================
//
// BUTTONの処理 (ui.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _BUTTON_H_
#define _BUTTON_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"
#include "card.h"

//================================================
// マクロ定義
//================================================

#define BUTTON_MAX 2

#define BUTTON_FORGIVE_POS (D3DXVECTOR3(SCREEN_WIDTH / 2 - 255.0f, SCREEN_HEIGHT / 2 + 150.0f, 0.0f))
#define BUTTON_PRAY_POS (D3DXVECTOR3(SCREEN_WIDTH / 2 + 255.0f, SCREEN_HEIGHT / 2 + 150.0f, 0.0f))
#define BUTTON_SIZE (D3DXVECTOR3(150.0f, 75.0f, 0.0f))
#define BUTTON_COLLISION_SIZE D3DXVECTOR3(120.0f, 55.0f, 0.0f)	// ボタンの当たり判定のサイズ
#define BUTTON_EXTEND_SIZE D3DXVECTOR3(200.0f, 100.0f, 0.0f)	// ボタンの拡大サイズ

#define FORGIVE_ID (CCard::TYPE_WEAPON_FINAL + (BUTTON_MAX - 1))  // 許すID
#define PRAY_ID (CCard::TYPE_WEAPON_FINAL + (BUTTON_MAX))         // 祈るID

//================================================
// クラス宣言
//================================================

// ボタンクラス
class CButton : public CScene2D
{
public:
    // BUTTONの種類
    typedef enum
    {
        TYPE_FORGIVE = 0,	    // 許す
        TYPE_PRAY,              // 祈る
        TYPE_MAX			    // 種類の最大数
    }TYPE;
    CButton();
    ~CButton();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CButton *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, TYPE type);

    void GameFinishFade(void);                                      // ゲーム終了時に、消すボタン
    D3DXVECTOR3 CollisionCursor(D3DXVECTOR3 pos, D3DXVECTOR3 size); // カーソルの当たり判定
    void SetCardInfo(void);                                         // カード情報の設定
    void Select(void);                                              // 選択

    void DisconnectInAnim(void);                    // アニメーションパートでの、切断対応

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    int m_nID;                                         // ID
    D3DXCOLOR m_col;                                   // 色
    TYPE m_type;                                       // ボタンの種類
};

#endif