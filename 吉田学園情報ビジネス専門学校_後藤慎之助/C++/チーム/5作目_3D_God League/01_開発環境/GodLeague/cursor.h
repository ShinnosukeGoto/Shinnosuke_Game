//====================================================================
//
// カーソルの処理 (cursor.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _CURSOR_H_
#define _CURSOR_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// 前方宣言(不完全なクラスは、cppの方でヘッダを呼び出す)
//================================================

//================================================
// マクロ定義
//================================================
#define CURSOR_POS D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f) // カーソルの位置
#define CURSOR_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)		                  // カーソルのサイズ
#define CURSOR_SPEED 10.0f                                                // カーソルの速さ

//================================================
// クラス宣言
//================================================

// カーソルクラス
class CCursor : public CScene2D
{
public:
    CCursor();
    ~CCursor();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    D3DXVECTOR3 ControlCursor(D3DXVECTOR3 pos);
    static HRESULT Load(void);
    static void Unload(void);
    static CCursor *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;   // 読み込んだテクスチャを保存する
};

#endif