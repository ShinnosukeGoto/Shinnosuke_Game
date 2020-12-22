//====================================================================
//
// カードの処理 (card.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _CARD_H_
#define _CARD_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// 前方宣言(不完全なクラスは、cppの方でヘッダを呼び出す)
//================================================
class CNumber;

//================================================
// マクロ定義
//================================================
#define CARD_SIZE D3DXVECTOR3(75.0f, 75.0f, 0.0f)	        // カードのサイズ
#define CARD_COLLISION_SIZE D3DXVECTOR3(70.0f, 70.0f, 0.0f)	// カードの当たり判定のサイズ
#define CARD_EXTEND_SIZE D3DXVECTOR3(100.0f, 100.0f, 0.0f)	// カードの拡大サイズ
#define CARD_SORT_TIME 60.0f                                // カードのソート時間

#define CARD_CONSIDER_CLOSE_DEST 5.0f                       // 目的の位置に近いとみなす値

#define CARD_FIRST_POS_X (115.0f)                                       // カードの最初のX座標
#define CARD_POS_Y (640.0f)                                             // カードのY座標
#define CARD_FIRST_POS D3DXVECTOR3(CARD_FIRST_POS_X, CARD_POS_Y, 0.0f)	// カードの最初の位置
#define CARD_SPACE_X 130.0f                                             // カードの横間隔

#define CARD_CREATE_POS (D3DXVECTOR3(500.0f, 370.0f, 0.0f))     // カードが複製される位置


//=================================
// カードの情報
//=================================

#define ARMOR_PERCENTAGE 45         // 100分のなんぼかで防具を引ける

// 防具
#define CARD_ARMOR_000 9
#define CARD_ARMOR_001 5
#define CARD_ARMOR_002 4
#define CARD_ARMOR_003 8
#define CARD_ARMOR_004 7
#define CARD_ARMOR_005 13
#define CARD_ARMOR_006 3
#define CARD_ARMOR_007 15
#define CARD_ARMOR_FINAL 20

// 武器
#define CARD_WEAPON_000 5
#define CARD_WEAPON_001 12
#define CARD_WEAPON_002 8
#define CARD_WEAPON_003 11
#define CARD_WEAPON_004 7
#define CARD_WEAPON_005 14
#define CARD_WEAPON_006 3
#define CARD_WEAPON_007 4
#define CARD_WEAPON_008 2
#define CARD_WEAPON_009 1
#define CARD_WEAPON_010 13
#define CARD_WEAPON_FINAL 25

//================================================
// クラス宣言
//================================================

// カードクラス
class CCard : public CScene2D
{
public:

    // ステータス
    typedef struct
    {
        int nAttack;    // 攻撃力
        int nDefence;   // 防御力
    }STATUS;

    // カードの種類
    typedef enum
    {
        TYPE_NONE = 0,          // なし
        TYPE_ARMOR_000,         // 光の盾
        TYPE_ARMOR_001,         // 水のフード
        TYPE_ARMOR_002,         // 鉄の鎧
        TYPE_ARMOR_003,         // 氷の盾
        TYPE_ARMOR_004,         // とげシールド
        TYPE_ARMOR_005,         // 伝承の盾
        TYPE_ARMOR_006,         // 木の盾
        TYPE_ARMOR_007,         // 魔神の鎧
        TYPE_ARMOR_FINAL,       // 防具最強
        //TYPE_ITEM_DAMMY,      // 道具ダミー
        //TYPE_MAGIC,           // 魔法ダミー 
        TYPE_WEAPON_000,        // 鉄の剣
        TYPE_WEAPON_001,        // 光の剣
        TYPE_WEAPON_002,        // 炎の剣
        TYPE_WEAPON_003,        // スノウソード
        TYPE_WEAPON_004,        // とげこんぼう
        TYPE_WEAPON_005,        // 闇の剣
        TYPE_WEAPON_006,        // こんぼう
        TYPE_WEAPON_007,        // 鉄の斧
        TYPE_WEAPON_008,        // 革のムチ
        TYPE_WEAPON_009,        // ブーメラン
        TYPE_WEAPON_010,        // レッドクロー
        TYPE_WEAPON_FINAL,      // 武器最強
        TYPE_MAX                // 種類の最大数
    }TYPE;
    CCard();
    ~CCard();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CCard *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int type);
    D3DXVECTOR3 CollisionCursor(D3DXVECTOR3 pos, D3DXVECTOR3 size);     // カーソルとの当たり判定
    void SetCardInfo(void);                                             // カード情報の設定
    void SelectCard(void);                                              // 使うカードの選択
    static void SortCards(void);                                        // カードの並び替え(一瞬)
    static void SortMoveCards(void);                                    // カードの並び替え(移動が見える感じのやつ)
    void SetPosDest(D3DXVECTOR3 posDest) { m_posDest = posDest; }       // 目的の位置を設定
    D3DXVECTOR3 Move(D3DXVECTOR3 pos);                                  // カードの移動
    int GetType(void) { return m_nType; }                               // カードのタイプを取得

    static STATUS List(int nType);                                      // カードリスト
    static void DrawCard(int nSheets);                                  // カードを引く

    static void CheckDispDefenceCard(void);                             // 防御カードがあるかどうかチェック
    static void CheckDispAttackCard(void);                              // 攻撃カードがあるかどうかチェック
    static bool GetDispDefenceCard(void) { return m_bDispDefenceCard; } // 防御カードがあるかどうか取得
    static bool GetDispAttackCard(void) { return m_bDispAttackCard; }   // 攻撃カードがあるかどうか取得

    static int GetNumAll(void) { return m_nNumAll; }                    // 手札の枚数を取得

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // 読み込んだテクスチャを保存する
    static int m_nNumAll;   // 手札の枚数
    D3DXVECTOR3 m_posDest;  // 目的の位置
    int m_nType;            // カードの種類
    bool m_bUse;            // 使用するかどうか
    int m_nMoveTime;        // 移動の経過時間

    int m_nAttackValue;     // 攻撃力
    int m_nDefenceValue;    // 防御力

    static bool m_bDispDefenceCard;                    // 防御カードがあるかどうか
    static bool m_bDispAttackCard;                     // 攻撃カードがあるかどうか
};

#endif