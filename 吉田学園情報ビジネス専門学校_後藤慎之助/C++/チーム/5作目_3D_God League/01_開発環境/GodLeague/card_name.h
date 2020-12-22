//====================================================================
//
// カード名Eの処理 (card_name.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _CARD_NAME_H_
#define _CARD_NAME_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"
#include "card.h"
#include "button.h"

//================================================
// マクロ定義
//================================================

// カード名
#define CARD_NAME_POS (D3DXVECTOR3(160.0f, 262.5f, 0.0f))
#define CARD_NAME_SIZE (D3DXVECTOR3(220.0f, 72.5f, 0.0f))

#define CARD_NAME_ENEMY_POS (D3DXVECTOR3(SCREEN_WIDTH - 139.5f, 335.5f, 0.0f))

//================================================
// クラス宣言
//================================================

// CARD_NAMEクラス
class CCardName : public CScene2D
{
public:

    // プレイヤーか敵か
    typedef enum
    {
        USETYPE_PLAYER = 0,          // プレイヤー
        USETYPE_ENEMY,               // 敵
        USETYPE_MAX
    }USETYPE;

    CCardName();
    ~CCardName();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CCardName *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int type, USETYPE useType);

    int GetType(void) { return m_nType; }          // タイプの取得
    USETYPE GetUseType(void) { return m_useType; } // 使った人の種類を取得
    void SetDontUse(void) { m_bUse = false; }      // 使わないものの破棄

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[CCard::TYPE_MAX + BUTTON_MAX];   // 読み込んだテクスチャを保存する
    USETYPE m_useType;                                                     // 使った人の種類
    int m_nType;                                                           // CARD_NAMEの種類
    bool m_bUse;                                                           // 使用するかどうか
};

#endif