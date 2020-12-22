//====================================================================
//
// �J�[�h��񏈗� (card_info.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _CARD_INFO_H_
#define _CARD_INFO_H_

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
#define MAX_CARD_INFO_DIGITS 2                                                      // �J�[�h���̌���
#define MAX_CARD_INFO_NUMBER 6                                                      // ���ʂɂ�����A�����̍ő吔

#define CARD_INFO_NUMBER_SIZE D3DXVECTOR3(40.0f, 40.0f, 0.0f)                       // �J�[�h���̐����̑傫��

#define CARD_INFO_ATTACK_POS (D3DXVECTOR3(SCREEN_WIDTH - 1109.5f, 322.5f, 0.0f))    // �U���̃J�[�h���̈ʒu
#define CARD_INFO_DEFENSE_POS (D3DXVECTOR3(SCREEN_WIDTH - 1109.5f,375.0f, 0.0f))    // �h��̃J�[�h���̈ʒu

#define CARD_INFO_ENEMY_POS (D3DXVECTOR3(SCREEN_WIDTH - 145.0f, 407.5f, 0.0f))      // �U���̃J�[�h���̈ʒu

#define CARD_INFO_ENEMY_TRANSPARENT -1                                              // ����̃J�[�h���𓧖��ɂ���

//================================================
// �N���X�錾
//================================================

// �J�[�h���̃N���X
class CCardInfo : public CScene
{
public:

    CCardInfo();
    ~CCardInfo();
    static CCardInfo *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void ReleaseNumber(int nCntNumber);   // �������J��
    void Update(void);
    void Draw(void);

    void SetCardInfo(int nAttack, int nDefence);    // �J�[�h���̐ݒ�

    void SetEnemyCardInfo(int nValue);              // �G�̃J�[�h���̐ݒ�

private:
    static CNumber *m_apNumber[MAX_CARD_INFO_NUMBER];  // �������
};

#endif