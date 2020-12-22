//====================================================================
//
// 遊び方の処理 (manual.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _MANUAL_H_
#define _MANUAL_H_

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
#define MANUAL_POS D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f) // 遊び方の位置
#define MANUAL_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))	  // 遊び方のサイズ
#define MANUAL_INACTIVE_COLOR D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)           // 遊び方の非アクティブ色

//================================================
// クラス宣言
//================================================

// 遊び方クラス
class CManual : public CScene2D
{
public:
    CManual();
    ~CManual();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CManual *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;   // 読み込んだテクスチャを保存する
    bool m_bDisp;                           // 表示するかどうか
};

#endif