//====================================================================
//
// 背景の処理 (bg.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _BG_H_
#define _BG_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// マクロ定義
//================================================
#define BG_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))   // 背景の位置
#define BG_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))          // 背景の大きさ

//================================================
// クラス宣言
//================================================

// 背景クラス
class CBg : public CScene2D
{
public:
    // 背景の種類
    typedef enum
    {
        TYPE_TITLE_BG,          // タイトルの背景
        TYPE_MAX			    // 種類の最大数
    }TYPE;
    CBg();
    ~CBg();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CBg *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, TYPE type);

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    D3DXCOLOR m_col;    // 色
    TYPE m_type;        // 背景の種類
};

#endif