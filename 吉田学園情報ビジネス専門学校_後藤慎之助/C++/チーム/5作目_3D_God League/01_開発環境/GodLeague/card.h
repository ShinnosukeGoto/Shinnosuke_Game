//====================================================================
//
// �J�[�h�̏��� (card.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _CARD_H_
#define _CARD_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// �O���錾(�s���S�ȃN���X�́Acpp�̕��Ńw�b�_���Ăяo��)
//================================================
class CNumber;

//================================================
// �}�N����`
//================================================
#define CARD_SIZE D3DXVECTOR3(75.0f, 75.0f, 0.0f)	        // �J�[�h�̃T�C�Y
#define CARD_COLLISION_SIZE D3DXVECTOR3(70.0f, 70.0f, 0.0f)	// �J�[�h�̓����蔻��̃T�C�Y
#define CARD_EXTEND_SIZE D3DXVECTOR3(100.0f, 100.0f, 0.0f)	// �J�[�h�̊g��T�C�Y
#define CARD_SORT_TIME 60.0f                                // �J�[�h�̃\�[�g����

#define CARD_CONSIDER_CLOSE_DEST 5.0f                       // �ړI�̈ʒu�ɋ߂��Ƃ݂Ȃ��l

#define CARD_FIRST_POS_X (115.0f)                                       // �J�[�h�̍ŏ���X���W
#define CARD_POS_Y (640.0f)                                             // �J�[�h��Y���W
#define CARD_FIRST_POS D3DXVECTOR3(CARD_FIRST_POS_X, CARD_POS_Y, 0.0f)	// �J�[�h�̍ŏ��̈ʒu
#define CARD_SPACE_X 130.0f                                             // �J�[�h�̉��Ԋu

#define CARD_CREATE_POS (D3DXVECTOR3(500.0f, 370.0f, 0.0f))     // �J�[�h�����������ʒu


//=================================
// �J�[�h�̏��
//=================================

#define ARMOR_PERCENTAGE 45         // 100���̂Ȃ�ڂ��Ŗh���������

// �h��
#define CARD_ARMOR_000 9
#define CARD_ARMOR_001 5
#define CARD_ARMOR_002 4
#define CARD_ARMOR_003 8
#define CARD_ARMOR_004 7
#define CARD_ARMOR_005 13
#define CARD_ARMOR_006 3
#define CARD_ARMOR_007 15
#define CARD_ARMOR_FINAL 20

// ����
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
// �N���X�錾
//================================================

// �J�[�h�N���X
class CCard : public CScene2D
{
public:

    // �X�e�[�^�X
    typedef struct
    {
        int nAttack;    // �U����
        int nDefence;   // �h���
    }STATUS;

    // �J�[�h�̎��
    typedef enum
    {
        TYPE_NONE = 0,          // �Ȃ�
        TYPE_ARMOR_000,         // ���̏�
        TYPE_ARMOR_001,         // ���̃t�[�h
        TYPE_ARMOR_002,         // �S�̊Z
        TYPE_ARMOR_003,         // �X�̏�
        TYPE_ARMOR_004,         // �Ƃ��V�[���h
        TYPE_ARMOR_005,         // �`���̏�
        TYPE_ARMOR_006,         // �؂̏�
        TYPE_ARMOR_007,         // ���_�̊Z
        TYPE_ARMOR_FINAL,       // �h��ŋ�
        //TYPE_ITEM_DAMMY,      // ����_�~�[
        //TYPE_MAGIC,           // ���@�_�~�[ 
        TYPE_WEAPON_000,        // �S�̌�
        TYPE_WEAPON_001,        // ���̌�
        TYPE_WEAPON_002,        // ���̌�
        TYPE_WEAPON_003,        // �X�m�E�\�[�h
        TYPE_WEAPON_004,        // �Ƃ�����ڂ�
        TYPE_WEAPON_005,        // �ł̌�
        TYPE_WEAPON_006,        // ����ڂ�
        TYPE_WEAPON_007,        // �S�̕�
        TYPE_WEAPON_008,        // �v�̃��`
        TYPE_WEAPON_009,        // �u�[������
        TYPE_WEAPON_010,        // ���b�h�N���[
        TYPE_WEAPON_FINAL,      // ����ŋ�
        TYPE_MAX                // ��ނ̍ő吔
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
    D3DXVECTOR3 CollisionCursor(D3DXVECTOR3 pos, D3DXVECTOR3 size);     // �J�[�\���Ƃ̓����蔻��
    void SetCardInfo(void);                                             // �J�[�h���̐ݒ�
    void SelectCard(void);                                              // �g���J�[�h�̑I��
    static void SortCards(void);                                        // �J�[�h�̕��ёւ�(��u)
    static void SortMoveCards(void);                                    // �J�[�h�̕��ёւ�(�ړ��������銴���̂��)
    void SetPosDest(D3DXVECTOR3 posDest) { m_posDest = posDest; }       // �ړI�̈ʒu��ݒ�
    D3DXVECTOR3 Move(D3DXVECTOR3 pos);                                  // �J�[�h�̈ړ�
    int GetType(void) { return m_nType; }                               // �J�[�h�̃^�C�v���擾

    static STATUS List(int nType);                                      // �J�[�h���X�g
    static void DrawCard(int nSheets);                                  // �J�[�h������

    static void CheckDispDefenceCard(void);                             // �h��J�[�h�����邩�ǂ����`�F�b�N
    static void CheckDispAttackCard(void);                              // �U���J�[�h�����邩�ǂ����`�F�b�N
    static bool GetDispDefenceCard(void) { return m_bDispDefenceCard; } // �h��J�[�h�����邩�ǂ����擾
    static bool GetDispAttackCard(void) { return m_bDispAttackCard; }   // �U���J�[�h�����邩�ǂ����擾

    static int GetNumAll(void) { return m_nNumAll; }                    // ��D�̖������擾

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // �ǂݍ��񂾃e�N�X�`����ۑ�����
    static int m_nNumAll;   // ��D�̖���
    D3DXVECTOR3 m_posDest;  // �ړI�̈ʒu
    int m_nType;            // �J�[�h�̎��
    bool m_bUse;            // �g�p���邩�ǂ���
    int m_nMoveTime;        // �ړ��̌o�ߎ���

    int m_nAttackValue;     // �U����
    int m_nDefenceValue;    // �h���

    static bool m_bDispDefenceCard;                    // �h��J�[�h�����邩�ǂ���
    static bool m_bDispAttackCard;                     // �U���J�[�h�����邩�ǂ���
};

#endif