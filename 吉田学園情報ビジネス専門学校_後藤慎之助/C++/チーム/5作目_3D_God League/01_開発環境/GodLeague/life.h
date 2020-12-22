//====================================================================
//
// ライフ処理 (life.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

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
#define MAX_LIFE_DIGITS 2     // ライフの桁数
#define MAX_LIFE_NUMBER 4     // 一画面における、数字の最大数

#define FIRST_LIFE 40   // 開始HP
#define MAX_LIFE 99     // 最大HP

#define LIFE_SIZE D3DXVECTOR3(70.0f, 70.0f, 0.0f)      // プレイヤーのライフの大きさ

#define LIFE_PLAYER_POS (D3DXVECTOR3(230.0f, 77.5f, 0.0f))       // プレイヤーのライフの位置
#define LIFE_ENEMY_POS (D3DXVECTOR3(SCREEN_WIDTH - 210.0f, 77.5f, 0.0f))      // エネミーのライフの位置

#define BORDER_HIT_SOUND 10     // ヒット音が切り替わる境界の値

//================================================
// クラス宣言
//================================================

// ライフのクラス
class CLife : public CScene
{
public:
    // プレイヤーか敵か
    typedef enum
    {
        TYPE_PLAYER = 0,          // プレイヤー
        TYPE_ENEMY,               // 敵
        TYPE_MAX
    }TYPE;

    CLife();
    ~CLife();
    static CLife *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void ReleaseNumber(int nCntNumber);   // 数字情報開放
    void Update(void);
    void Draw(void);
    int GetPlayerLife(void) { return m_nPlayerLife; }   // プレイヤーのライフを取得
    int GetEnemyLife(void) { return m_nEnemyLife; }     // 敵のライフを取得

    void PlayerDamageCalculation(void);      // プレイヤーのダメージ計算
    void EnemyDamageCalculation(void);       // 敵のライフのダメージ計算
    int GetAttackValue(const TYPE type);     // 攻撃力を取得
    int GetDefenceValue(const TYPE type);    // 防御力を取得

    void AddPlayerLife(const int nValue);    // プレイヤーの回復
    void AddEnemyLife(const int nValue);     // 敵の回復

    void PlayAttackSound(int nDamage);       // 攻撃音再生

private:
    static CNumber *m_apNumber[MAX_LIFE_NUMBER];  // 数字情報
    int m_nPlayerLife;                            // プレイヤーのライフ
    int m_nEnemyLife;                             // 敵のライフ
};

#endif