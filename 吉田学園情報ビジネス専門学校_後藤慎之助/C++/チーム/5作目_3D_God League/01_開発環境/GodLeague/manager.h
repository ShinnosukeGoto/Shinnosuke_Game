//====================================================================
//
// �����̊Ǘ����� (manager.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"

//================================================
// �O���錾(�s���S�ȃN���X�́Acpp�̕��Ńw�b�_���Ăяo��)
//================================================
class CRenderer;
class CInputKeyboard;
class CInputJoypad;
class CSound;
class CMode;
class CFade;
class CCamera;
class CLight;

//================================================
// �N���X�錾
//================================================

// �}�l�[�W���[�N���X
class CManager
{
public:

    // ���[�h�̎��
    typedef enum
    {
        MODE_NONE = 0,
        MODE_TITLE,
        MODE_GAME,
        MODE_RESULT,
        MODE_MAX
    }MODE;

	// �^�[���̎��
	typedef enum
	{
		TURN_ATTACK = 0,
		TURN_DEFENCE,
		TURN_MAX
	}TURN;

    static void ResetAll(void);     // �S�Ă̒ʐM�ɂ��������̂̐ÓI�����o�ϐ���������
    CManager();
    ~CManager();
    HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static CRenderer *GetRenderer(void) { return m_pRenderer; }
    static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
    static CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; }
    static CSound *GetSound(void) { return m_pSound; }
    static MODE GetMode(void);
    static void SetMode(MODE mode);
	static void ConnectionThread(void);
	static void SetStartMatching(bool bMatching);
	static void SendCard(int nType);
	static void RecvCard(int nTypeCard, TURN Turn);
	static int GetEnemyUseCard(void) { return m_nEnemyUseCard; }

    static bool GetMatching(void) { return m_bMatching; }       // �}�b�`���O�������Ƃ��擾
    static int GetAttackOrder(void) { return m_nAttackOrder; }  // ��U��U���擾

    //static int GetEnemyUseCard(void) { return m_nEnemyUseCard; }    // �G���g�p�����J�[�h���擾
    static int GetPlayerUseCard(void) { return m_nPlayerUseCard; }    // �G���g�p�����J�[�h���擾

    static bool GetEnemyDisconnect(void) { return m_bDisconnect; }    // �G���ؒf�������ǂ���

private:
    static CRenderer *m_pRenderer;              // �����_���[�̃|�C���^
    static CInputKeyboard *m_pInputKeyboard;    // �L�[�{�[�h�̃|�C���^
    static CInputJoypad *m_pInputJoypad;        // �R���g���[���[�̃|�C���^
    static CSound *m_pSound;                    // �T�E���h�̃|�C���^
    static CMode *m_pMode;                      // ���[�h�̃|�C���^
    static CFade *m_pFade;                      // �t�F�[�h�̃|�C���^
    static CCamera *m_pCamera;                  // �J�����̃|�C���^
    static CLight *m_pLight;                    // ���C�g�̃|�C���^
	static bool m_bStartMatching;               // �}�b�`���O�J�n��ʒm
	static bool m_bMatching;                    // �}�b�`���O�ł������ǂ���
	static bool m_bAttackTurn;                  // �U���������ǂ���
	static bool m_bDefenceTurn;                 // �h��^�[�����ǂ���
	static bool m_bDefenceCardUse;              // �h��J�[�h���g����
	static bool m_bRecvCard;                    // ���肩��J�[�h���󂯎�ꂽ���ǂ���
	static bool m_bDisconnect;                  // �ؒf�t���O
	static bool m_bGameEnd;                     // �Q�[���I���t���O
	static int m_nAttackOrder;                  // ��U��U�̏���ۑ��A1���������U�A2���������U
	static int m_nEnemyUseCard;                 // �G���g�p�����J�[�h���
	static int m_nPlayerUseCard;                // �v���C���[���g�p�����J�[�h
};

#endif