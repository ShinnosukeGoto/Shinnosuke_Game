//====================================================================
//
// カード情報処理 (card_info.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _CARD_INFO_H_
#define _CARD_INFO_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"

//================================================
// 前方宣言(不完全なクラスは、cppの方でヘッダを呼び出す)
//================================================
class CNumber;

//================================================
// マクロ定義
//================================================
#define MAX_CARD_INFO_DIGITS 2                                                      // カード情報の桁数
#define MAX_CARD_INFO_NUMBER 6                                                      // 一画面における、数字の最大数

#define CARD_INFO_NUMBER_SIZE D3DXVECTOR3(40.0f, 40.0f, 0.0f)                       // カード情報の数字の大きさ

#define CARD_INFO_ATTACK_POS (D3DXVECTOR3(SCREEN_WIDTH - 1109.5f, 322.5f, 0.0f))    // 攻撃のカード情報の位置
#define CARD_INFO_DEFENSE_POS (D3DXVECTOR3(SCREEN_WIDTH - 1109.5f,375.0f, 0.0f))    // 防御のカード情報の位置

#define CARD_INFO_ENEMY_POS (D3DXVECTOR3(SCREEN_WIDTH - 145.0f, 407.5f, 0.0f))      // 攻撃のカード情報の位置

#define CARD_INFO_ENEMY_TRANSPARENT -1                                              // 相手のカード情報を透明にする

//================================================
// クラス宣言
//================================================

// カード情報のクラス
class CCardInfo : public CScene
{
public:

    CCardInfo();
    ~CCardInfo();
    static CCardInfo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void ReleaseNumber(int nCntNumber);   // 数字情報開放
    void Update(void);
    void Draw(void);

    void SetCardInfo(int nAttack, int nDefence);    // カード情報の設定

    void SetEnemyCardInfo(int nValue);              // 敵のカード情報の設定

private:
    static CNumber *m_apNumber[MAX_CARD_INFO_NUMBER];  // 数字情報
};

#endif