//====================================================================
//
// ���C�t���� (life.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene.h"

//================================================
// �O���錾(�s���S�ȃN���X�́Acpp�̕��Ńw�b�_���Ăяo��)
//================================================
class CNumber;

//================================================
// �}�N����`
//================================================
#define MAX_LIFE_DIGITS 2     // ���C�t�̌���
#define MAX_LIFE_NUMBER 4     // ���ʂɂ�����A�����̍ő吔

#define FIRST_LIFE 40   // �J�nHP
#define MAX_LIFE 99     // �ő�HP

#define LIFE_SIZE D3DXVECTOR3(70.0f, 70.0f, 0.0f)      // �v���C���[�̃��C�t�̑傫��

#define LIFE_PLAYER_POS (D3DXVECTOR3(230.0f, 77.5f, 0.0f))       // �v���C���[�̃��C�t�̈ʒu
#define LIFE_ENEMY_POS (D3DXVECTOR3(SCREEN_WIDTH - 210.0f, 77.5f, 0.0f))      // �G�l�~�[�̃��C�t�̈ʒu

#define BORDER_HIT_SOUND 10     // �q�b�g�����؂�ւ�鋫�E�̒l

//================================================
// �N���X�錾
//================================================

// ���C�t�̃N���X
class CLife : public CScene
{
public:
    // �v���C���[���G��
    typedef enum
    {
        TYPE_PLAYER = 0,          // �v���C���[
        TYPE_ENEMY,               // �G
        TYPE_MAX
    }TYPE;

    CLife();
    ~CLife();
    static CLife *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void ReleaseNumber(int nCntNumber);   // �������J��
    void Update(void);
    void Draw(void);
    int GetPlayerLife(void) { return m_nPlayerLife; }   // �v���C���[�̃��C�t���擾
    int GetEnemyLife(void) { return m_nEnemyLife; }     // �G�̃��C�t���擾

    void PlayerDamageCalculation(void);      // �v���C���[�̃_���[�W�v�Z
    void EnemyDamageCalculation(void);       // �G�̃��C�t�̃_���[�W�v�Z
    int GetAttackValue(const TYPE type);     // �U���͂��擾
    int GetDefenceValue(const TYPE type);    // �h��͂��擾

    void AddPlayerLife(const int nValue);    // �v���C���[�̉�
    void AddEnemyLife(const int nValue);     // �G�̉�

    void PlayAttackSound(int nDamage);       // �U�����Đ�

private:
    static CNumber *m_apNumber[MAX_LIFE_NUMBER];  // �������
    int m_nPlayerLife;                            // �v���C���[�̃��C�t
    int m_nEnemyLife;                             // �G�̃��C�t
};

#endif