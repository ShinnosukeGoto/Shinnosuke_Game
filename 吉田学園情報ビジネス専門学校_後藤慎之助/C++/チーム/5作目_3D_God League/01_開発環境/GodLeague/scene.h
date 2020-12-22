//====================================================================
//
// シーン上のオブジェクト処理 (scene.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// マクロ定義
//================================================
#define MAX_OBJECT 512   // オブジェクトの最大数

//================================================
// クラス宣言
//================================================

// オブジェクトのクラス
class CScene
{
public:
    // オブジェクトの種類(描画順)
    typedef enum
    {
        OBJTYPE_NULL,      // なし
        OBJTYPE_BG,        // 背景
        OBJTYPE_UI,        // UI
        OBJTYPE_BUTTON,    // ボタン
        OBJTYPE_CARD_INFO, // カード情報
        OBJTYPE_CARD_NAME, // カード名
        OBJTYPE_LIFE,      // ライフ
        OBJTYPE_PLAYER,    // プレイヤー
        OBJTYPE_ENEMY,     // エネミー
        OBJTYPE_CARD,      // カード
        OBJTYPE_EFFECT,    // エフェクト
        OBJTYPE_CURSOR,    // カーソル
        OBJTYPE_MANUAL,    // 遊び方
        OBJTYPE_MAX        // オブジェクトの種類の最大数
    }OBJTYPE;

    CScene();
    CScene(OBJTYPE objType);    // イニシャライザでオブジェクトタイプを紐づけ
    virtual ~CScene();  // 親クラスのデストラクタにvirtualをつけることで、メモリリークを抑える
    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size) = 0;
    virtual void Uninit(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
    static void UpdateAll(void);        // 全て更新
    static void DrawAll(void);          // 全て描画
    static void ReleaseAll(void);       // 全て開放
    OBJTYPE GetObjType(void) { return m_objType; }  // オブジェクトタイプを取得
    static CScene *GetScene(int nCntScene);         // シーンを取得

protected:
    void Release(void);                 // 単体を開放

private:
    static CScene *m_apScene[MAX_OBJECT];  // シーン上のオブジェクト情報
    static int m_nNumAll;                   // シーン上のオブジェクトの最大数
    int m_nID;   // 開放する番号を記憶(staticはインスタンス関係なく持っているが、普通のはインスタンスごとにメンバ変数を持っている)
    OBJTYPE m_objType;                      // オブジェクトの種類
};

#endif