//=============================================================================
//
// �ŋ��̘b�����̏���[player2.cpp]�i���@2�j
// Author:�㓡�T�V��
//
//===================================================================================================

//===================================================================================================
// �C���N���[�h�t�@�C��
//===================================================================================================
#include "player2.h"
#include "camera.h"
#include "input.h"
#include "controller.h"
#include "block.h"
#include "sound.h"
#include "stage.h"
#include "bomb.h"
#include "fire.h"
#include "game.h"
#include "item.h"
#include "collision.h"
#include "effect.h"
#include "shadow.h"
#include "manual.h"

//===================================================================================================
// �O���[�o���ϐ���`
//===================================================================================================
PLAYER2 g_Player2;        //�v���C���[�̏��

int g_nFrameCountPlayer2;				// ���݂̃t���[����

int g_nPlayKeySetlndexPlayer2;			// �Đ����̃L�[�Z�b�g�C���f�b�N�X

int g_nPlayer2CurrentAnimationPlayer2;	// �Đ����̃A�j���[�V����

int g_nFremePlayer2;					// �t���[����

int g_nCntKeyPlayer2;					// �L�[��

int g_nChangeFlamePlayer2;			// ���[�V�����̃t���[��

MOTIONSTATE_PLAYER2 g_MotionState;			// ���[�V�����̍\����
MOTIONSTATE_PLAYER2 g_MotionOldState;		// 1F�O�̃��[�V�����̍\����

D3DXVECTOR3 g_aNumPlayer2Pos[MAX_PARTS];		// pos�̍���
D3DXVECTOR3 g_aNumPlayer2Rot[MAX_PARTS];		// rot�̍���
D3DXVECTOR3 g_aNumPlayer2RotDest[MAX_PARTS];	// rot�̍���

ANIMATION_PLAYER2 g_aPlayer2Motion[MOTIONSTATE_PLAYER2_MAX];	// ���[�V�����̏��
                                        // ���f���p�[�c�̃f�[�^
ModelParts_Player2 g_aPlayer2[MAX_PARTS] =
{
    { "MODEL/player2/00_body.x" },
    { "MODEL/player2/01_head.x" },
    { "MODEL/player2/02_armR.x" },
    { "MODEL/player2/03_handR.x" },
    { "MODEL/player2/04_armL.x" },
    { "MODEL/player2/05_handL.x" },
    { "MODEL/player2/06_legR.x" },
    { "MODEL/player2/07_footR.x" },
    { "MODEL/player2/08_legL.x" },
    { "MODEL/player2/09_footL.x" },
};

bool g_bEvilUra;   //�E�ӂ̔g���ɖڊo�߂Ă��邩�ǂ���

//===================================================================================================
// �v���C���[����������
// Author:�㓡�T�V��
//===================================================================================================
void InitPlayer2(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�Q�[����Ԃ̎擾
    GAME *pGame = GetGame();

    //�B���R�}���h�g�p������擾
    g_bEvilUra = GetHiddenCommand2();

    //�O���[�o���ϐ��̏�����
    g_MotionState = MOTIONSTATE_PLAYER2_IDLE;

    g_nFrameCountPlayer2 = 0;				// ���݂̃t���[����

    g_nPlayKeySetlndexPlayer2 = 0;			// �Đ����̃L�[�Z�b�g�C���f�b�N�X

    g_nPlayer2CurrentAnimationPlayer2 = MOTIONSTATE_PLAYER2_IDLE;	// �Đ����̃A�j���[�V����

    g_nFremePlayer2 = 0;					// �t���[����

    g_nCntKeyPlayer2 = 1;					// �L�[��

    g_nChangeFlamePlayer2 = 20;			// ���[�V�����̃t���[��

    //�\���̂̏�����
    g_Player2.pos = pGame->startPos2;		            // �����ʒu
    g_Player2.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ʒu
    g_Player2.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��������
    g_Player2.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ړ���
    g_Player2.posGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ʒu
    g_Player2.bDisp = true;
    //�B���R�}���h�g�p
    if (g_bEvilUra == true)
    {
        g_Player2.fLife = BOMB_DAMAGE + PLAYER_LIFE;	    //�E�ӂ̔g���͔��������ς���
        g_Player2.nSpeedRank = PLAYER_FIRST_SPEEDRANK + 2;   //�E�ӂ̔g����2�����N����
        g_Player2.nBomb = PLAYER_FIRST_BOMB + 1;             //�E�ӂ̔g���̓{����2������
        g_Player2.nCanKick = PLAYER_FIRST_KICK + 1;          //�E�ӂ̔g���̓L�b�N�ł���
    }

    //�B���R�}���h���g�p
    else
    {
        g_Player2.fLife = PLAYER_LIFE;
        g_Player2.nSpeedRank = PLAYER_FIRST_SPEEDRANK;
        g_Player2.nBomb = PLAYER_FIRST_BOMB;
        g_Player2.nCanKick = PLAYER_FIRST_KICK;
    }
    g_Player2.fSpeed = VALUE_MOVE;
    g_Player2.nFire = PLAYER_FIRST_FIRE;
    g_Player2.nShadow = SetShadow(g_Player2.pos, g_Player2.rot, GRID_SIZE, g_Player2.pos.y,
        D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_ALPHA_MAX));//�e�̐ݒ�
    g_Player2.nCntInvincibleFrame = 0;
    g_Player2.bTransparence = false;
    g_Player2.nMaxBomb = g_Player2.nBomb;

    LoadPlayer2();
    LoadPlayer2Animation();

    // ���f���p�[�c�̏�����
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        //		g_Player2.modelParts[nCntParts].pos = g_aPlayer2[nCntParts].pos;			// �����ʒu
        //		g_Player2.modelParts[nCntParts].rot = g_aPlayer2[nCntParts].rot;			// �����p�x

        g_Player2.modelParts[nCntParts].nldxModelParent = 0;
    }
    // �t�@�C���̓ǂݍ���
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        g_Player2.modelParts[nCntParts].nldxModelParent = g_aPlayer2[nCntParts].nParents;

        // X�t�@�C���̓ǂݍ���
        D3DXLoadMeshFromX(g_aPlayer2[nCntParts].pFileName,
            D3DXMESH_SYSTEMMEM,
            pDevice,
            NULL,
            &g_Player2.modelParts[nCntParts].pBuffMat,
            NULL,
            &g_Player2.modelParts[nCntParts].nNumMat,
            &g_Player2.modelParts[nCntParts].pMesh);

    }
}

//===================================================================================================
// �v���C���[�I������
// Author:�㓡�T�V��
//===================================================================================================
void UninitPlayer2(void)
{
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        // ���b�V���̔j��
        if (g_Player2.modelParts[nCntParts].pMesh != NULL)
        {
            g_Player2.modelParts[nCntParts].pMesh->Release();
            g_Player2.modelParts[nCntParts].pMesh = NULL;
        }
        // �}�e���A���̔j��
        if (g_Player2.modelParts[nCntParts].pBuffMat != NULL)
        {
            g_Player2.modelParts[nCntParts].pBuffMat->Release();
            g_Player2.modelParts[nCntParts].pBuffMat = NULL;
        }
    }
}

//===================================================================================================
// �v���C���[�X�V����
// Author:�㓡�T�V��
//===================================================================================================
void UpdatePlayer2(void)
{
    //�R���g���[���[�̏����擾����
    DIJOYSTATE2 Controller = GetController(PLAYER_2);
    //�J�����̏����擾����
    CAMERA*pCamera = GetCamera();
    //�Q�[���̏󋵂��擾����
    GAME *pGame = GetGame();

    //�ړ������N�ɉ������A�����𓾂�
    switch (g_Player2.nSpeedRank)
    {
    case PLAYER_FIRST_SPEEDRANK:
        g_Player2.fSpeed = VALUE_MOVE;
        break;

    case PLAYER_FIRST_SPEEDRANK + 1:
        g_Player2.fSpeed = VALUE_MOVE + VALUE_UPMOVE;
        break;

    case PLAYER_FIRST_SPEEDRANK + 2:
        g_Player2.fSpeed = VALUE_MOVE + VALUE_UPMOVE * 2.0f;
        break;

    case PLAYER_FIRST_SPEEDRANK + 3:
        g_Player2.fSpeed = VALUE_MOVE + VALUE_UPMOVE * 3.0f;
        break;

    case PLAYER_FIRST_SPEEDRANK + 4:
        g_Player2.fSpeed = VALUE_MOVE + VALUE_UPMOVE * 4.0f;
        break;
    }

    //���G����
    if (g_Player2.nCntInvincibleFrame > 0)
    {
        g_Player2.nCntInvincibleFrame--;                     //���G���Ԃ𐔂���
        g_Player2.bTransparence = !g_Player2.bTransparence;   //���G���́A�_�ł�����
    }
    else
    {
        g_Player2.bTransparence = false; //����������
    }

    //�������Ă���Ȃ�
    if (g_Player2.bDisp == true && pGame->state == GAMESTATE_NORMAL && pCamera->state == CAMERASTATE_NORMAL)
    {
        // �v���C���[�������Ă��Ȃ��Ƃ�
        D3DXVECTOR3 lengthCheckVec = g_Player2.move;
        lengthCheckVec.y = 0;
        if (D3DXVec3Length(&lengthCheckVec) <= 1.5f)
        {
            g_MotionState = MOTIONSTATE_PLAYER2_IDLE;    //���[�V�������A�C�h���ɂ���
        }

        //1F�O�̈ʒu���L��
        g_Player2.posOld = g_Player2.pos;

        //�O���b�h�̒��S�̒l�𓾂�
        g_Player2.posGrid = ConversionGrid(&g_Player2.pos);

        //�X�e�B�b�N���X���Ă���Ȃ�ړ�
        if (Controller.lY != 0 || Controller.lX != 0)
        {
            float fAngle = atan2(Controller.lX, Controller.lY*-1);//�X�e�B�b�N�̊p�x�����߂�
            float fPlayer2Angle = atan2(Controller.lX*-1, Controller.lY);//�X�e�B�b�N�̊p�x�����߂�

            g_Player2.pos.x += sinf((pCamera->rot.y + fAngle))*g_Player2.fSpeed;
            g_Player2.pos.z += cosf((pCamera->rot.y + fAngle))*g_Player2.fSpeed;
            g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
                                        //�L�����̌�����ς���
            g_Player2.rot.y = pCamera->rot.y + fPlayer2Angle;
        }
        //�\���L�[�ł̈ړ�
        else
        {
            //��
            if (Controller.rgdwPOV[0] == BUTTON_LEFT)
            {
                g_Player2.pos.x -= g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x + D3DXToRadian(90.0f);
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
            }

            //����
            if (Controller.rgdwPOV[0] == BUTTON_LEFT_UP)
            {
                g_Player2.pos.x -= sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.pos.z += cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x + D3DXToRadian(135.0f);
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
            }

            //����
            if (Controller.rgdwPOV[0] == BUTTON_DOWN_LEFT)
            {
                g_Player2.pos.x -= sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.pos.z -= cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x + D3DXToRadian(45.0f);
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
            }

            //�E
            if (Controller.rgdwPOV[0] == BUTTON_RIGHT)
            {
                g_Player2.pos.x += g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x - D3DXToRadian(90.0f);
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
            }

            //�E��
            if (Controller.rgdwPOV[0] == BUTTON_UP_RIGHT)
            {
                g_Player2.pos.x += sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.pos.z += cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x - D3DXToRadian(135.0f);
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
            }

            //�E��
            if (Controller.rgdwPOV[0] == BUTTON_RIGHT_DOWN)
            {
                g_Player2.pos.x += sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.pos.z -= cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x - D3DXToRadian(45.0f);
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
            }

            //��
            if (Controller.rgdwPOV[0] == BUTTON_UP)
            {
                g_Player2.pos.z += g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x + D3DX_PI;
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
            }

            //��
            if (Controller.rgdwPOV[0] == BUTTON_DOWN)
            {
                g_Player2.pos.z -= g_Player2.fSpeed;
                g_Player2.rot.y = pCamera->rot.x;
                g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
            }
        }

        //���̑���===========================================================================================
        if (Controller.lY == 0 && Controller.lX == 0 && Controller.rgdwPOV[0] == BUTTON_NEUTRAL)
        {
            //WASD�L�[�ňړ�
            //A�L�[�������ꂽ(���ړ�)
            if (GetKeyboardPress(DIK_LEFT))
            {
                if (GetKeyboardPress(DIK_UP))//A��W�������ɉ����ꂽ
                {
                    g_Player2.pos.x -= sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.pos.z += cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x + D3DXToRadian(135.0f);
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
                }
                else if (GetKeyboardPress(DIK_DOWN))//A��S�������ɉ����ꂽ
                {
                    g_Player2.pos.x -= sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.pos.z -= cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x + D3DXToRadian(45.0f);
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
                }
                else if (GetKeyboardPress(DIK_RIGHT))
                {

                }
                else				//������������
                {
                    g_Player2.pos.x -= g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x + D3DXToRadian(90.0f);
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
                }

            }

            //D�L�[�������ꂽ(�E�ړ�)
            else if (GetKeyboardPress(DIK_RIGHT))
            {
                if (GetKeyboardPress(DIK_UP))//D��W�������ɉ����ꂽ
                {
                    g_Player2.pos.x += sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.pos.z += cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x - D3DXToRadian(135.0f);
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���

                }
                else if (GetKeyboardPress(DIK_DOWN))//D��S�������ɉ����ꂽ
                {
                    g_Player2.pos.x += sinf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.pos.z -= cosf(D3DX_PI / 4) * g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x - D3DXToRadian(45.0f);
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
                }
                else if (GetKeyboardPress(DIK_LEFT))
                {

                }
                else				//�E����������
                {
                    g_Player2.pos.x += g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x - D3DXToRadian(90.0f);
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
                }

            }

            //W�L�[�������ꂽ(��ړ�)
            else if (GetKeyboardPress(DIK_UP))
            {
                if (GetKeyboardPress(DIK_DOWN))
                {

                }
                else
                {
                    g_Player2.pos.z += g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x + D3DX_PI;
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
                }
            }

            //S�L�[�������ꂽ(���ړ�)
            else if (GetKeyboardPress(DIK_DOWN))
            {
                if (GetKeyboardPress(DIK_UP))
                {

                }
                else
                {
                    g_Player2.pos.z -= g_Player2.fSpeed;
                    g_Player2.rot.y = pCamera->rot.x;
                    g_MotionState = MOTIONSTATE_PLAYER2_WALK;//���[�V����������ɂ���
                }
            }
        }

        //���e��u��(��)
        if (GetKeyboardTrigger(DIK_RETURN) && g_Player2.nBomb > 0 && g_Player2.nCntInvincibleFrame == 0)
        {
            //���e�Əd�Ȃ��Ă��Ȃ��Ȃ�A�u����
            if (HitBomb(&g_Player2.pos, &g_Player2.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BOMB + 1) == false)
            {
                PlaySound(SOUND_LABEL_SE_SET_BOMB);   //�{���ݒu��
                SetBomb(g_Player2.posGrid, g_Player2.nFire, TIME_BOMB, SELECT_URA);
                g_Player2.nBomb--;
            }

        }
        //===============================================================================================

        //���e��u��
        if (GetJoypadTrigger(PLAYER_2, BUTTON_B) && g_Player2.nBomb > 0 && g_Player2.nCntInvincibleFrame == 0)
        {
            //�E�ӂ̔g���ɖڊo�߂ĂȂ��Ȃ�
            if (g_bEvilUra == false)
            {
                //���e�Əd�Ȃ��Ă��Ȃ��Ȃ�A3�b�Ŕ�������{����u����
                if (HitBomb(&g_Player2.pos, &g_Player2.posOld,
                    D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BOMB + 1) == false)
                {
                    PlaySound(SOUND_LABEL_SE_SET_BOMB);   //�{���ݒu��
                    SetBomb(g_Player2.posGrid, g_Player2.nFire, TIME_BOMB, SELECT_URA);
                    g_Player2.nBomb--;
                }
            }

            //�E�ӂ̔g���ɖڊo�߂Ă���Ȃ�
            else
            {
                //���e�Əd�Ȃ��Ă��Ȃ��Ȃ�A2�b�Ŕ�������{����u����
                if (HitBomb(&g_Player2.pos, &g_Player2.posOld,
                    D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BOMB + 1) == false)
                {
                    PlaySound(SOUND_LABEL_SE_SET_BOMB);   //�{���ݒu��
                    SetBomb(g_Player2.posGrid, g_Player2.nFire, TIME_BOMB - 60, SELECT_URA);
                    g_Player2.nBomb--;
                }
            }
        }

        //�N���b�V������
        if (CrushingBlock(&g_Player2.pos, &g_Player2.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BLOCK + 1) == true)
        {
            HitPlayer2(BOMB_DAMAGE*4);
        }

        //�u���b�N�̓����蔻��
        CollisionBlock(&g_Player2.pos, &g_Player2.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID));

        //�L�b�N�ł��Ȃ�
        if (g_Player2.nCanKick <= 0)
        {
            //���e�̓����蔻��
            CollisionBomb(&g_Player2.pos, &g_Player2.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID));
        }

        //�L�b�N�ł���
        else
        {
            //���e�̃L�b�N����
            KickBomb(&g_Player2.pos, &g_Player2.posOld,
                D3DXVECTOR3(GRID_SIZE - (GRID_SIZE / 2.0f + g_Player2.fSpeed)
                    , PLAYER_HEIGHT * 2, GRID_SIZE - (GRID_SIZE / 2.0f + g_Player2.fSpeed)), SELECT_URA);

            //���e�̓����蔻��
            CollisionBomb(&g_Player2.pos, &g_Player2.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID));
        }

        //���̓����蔻��
        if (HitFire(&g_Player2.pos, &g_Player2.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID - g_Player2.fSpeed
                , PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID - g_Player2.fSpeed)) == true)
        {
            HitPlayer2(BOMB_DAMAGE);
        }

        //==============================================================================================================================================================
        //�A�C�e���̓����蔻��
        //��
        if (HitItem(&g_Player2.pos, &g_Player2.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_000) == true)
        {
            g_Player2.nFire++;
        }

        //�{��
        if (HitItem(&g_Player2.pos, &g_Player2.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_001) == true)
        {
            g_Player2.nBomb++;
            g_Player2.nMaxBomb++;
        }

        //�X�s�[�h
        if (HitItem(&g_Player2.pos, &g_Player2.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_002) == true)
        {
            g_Player2.nSpeedRank++;
        }

        //�L�b�N
        if (HitItem(&g_Player2.pos, &g_Player2.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_003) == true)
        {
            g_Player2.nCanKick++;
        }
        //==============================================================================================================================================================

        //�e��ݒ�
        SetPositionShadow(g_Player2.nShadow, D3DXVECTOR3(g_Player2.pos.x, 0, g_Player2.pos.z), g_Player2.pos.y);

        //���S����
        if (g_Player2.fLife <= 0.0f)
        {
            //�e�̍폜
            SHADOW *pShadow = GetShadow();//�e�̏����擾
            pShadow[g_Player2.nShadow].use = false;

            g_Player2.bDisp = false;    //���@�𖢎g�p��

            PlaySound(SOUND_LABEL_SE_MAN_VOICE2);   //���ꂽ���̐�

            //�v���C���[�����G�t�F�N�g
            float fSpeedX = 0.0f;   //����X
            float fSpeedY = 0.0f;   //����Y
            float fSpeedZ = 0.0f;   //����Z
            int nPlusMinusX = 0;    //����X
            int nPlusMinusZ = 0;    //����Z
            for (int nCnt = 0; nCnt < COUNT_EFFECT_EXPLODE_PLAYER; nCnt++)
            {
                fSpeedX = float(rand() % 500) / 100.0f;
                fSpeedY = float(rand() % 800 + 300) / 100.0f;
                fSpeedZ = float(rand() % 500) / 100.0f;
                nPlusMinusX = int(rand() % 2 + 1);
                nPlusMinusZ = int(rand() % 2 + 1);
                if (nPlusMinusX == 1)
                {
                    nPlusMinusX = 1;
                }
                else
                {
                    nPlusMinusX = -1;
                }

                if (nPlusMinusZ == 1)
                {
                    nPlusMinusZ = 1;
                }
                else
                {
                    nPlusMinusZ = -1;
                }

                D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX*nPlusMinusX, fSpeedY, fSpeedZ*nPlusMinusZ);
                SetEffect(g_Player2.pos, move, EFFECT_SIZE_EXPLODE_PLAYER, EFFECTTYPE_EXPLOSION); //�����G�t�F�N�g
            }

            //�����ŃA�C�e�����U��΂߂�p�̕ϐ�
            float posX = 0.0f;
            float posZ = 0.0f;
            int nCntFire = 0; //��
            int nCntBomb = 0; //�{��
            int nCntSpeed = 0;//�X�s�[�h
            int nCntKick = 0; //�L�b�N
            D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            int nCntLoop = 0;   //�������[�v���

            //�����l�����Ă���Ȃ�
            if (g_Player2.nFire > PLAYER_FIRST_FIRE)
            {
                //�����_���ɁA���A�C�e����ݒu
                while (nCntFire != g_Player2.nFire - PLAYER_FIRST_FIRE)//�ݒu������܂Ń��[�v
                {
                    //�����_����X�AZ���W�����߂�
                    posX = float(rand() % 1200 + 100);
                    posZ = float(rand() % 1000 + 100);
                    pos = D3DXVECTOR3(posX, 0.0f, posZ);

                    //�O���b�h���W�ɕϊ�����
                    pos = ConversionGrid(&pos);

                    //���̃O���b�h�ɔz�u�����Ȃ��Ȃ�z�u
                    if (ConfirmationGrid(pos) == false)
                    {
                        //���A�C�e����z�u
                        SetItem(D3DXVECTOR3(pos.x, GRID_SIZE / 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_000);
                        nCntFire++;
                    }

                    //���[�v�񐔉��Z
                    nCntLoop++;

                    //�������[�v���甲����
                    if (nCntLoop == AVOID_INFINITE_LOOP)
                    {
                        nCntFire = g_Player2.nFire - PLAYER_FIRST_FIRE;
                    }
                }
            }

            //���[�v�񐔃��Z�b�g
            nCntLoop = 0;

            //�{�����l�����Ă���Ȃ�
            if (g_Player2.nMaxBomb > PLAYER_FIRST_BOMB)
            {
                //�����_���ɁA�{���A�C�e����ݒu
                while (nCntBomb != g_Player2.nMaxBomb - PLAYER_FIRST_BOMB)//�ݒu������܂Ń��[�v
                {
                    //�����_����X�AZ���W�����߂�
                    posX = float(rand() % 1200 + 100);
                    posZ = float(rand() % 1000 + 100);
                    pos = D3DXVECTOR3(posX, 0.0f, posZ);

                    //�O���b�h���W�ɕϊ�����
                    pos = ConversionGrid(&pos);

                    //���̃O���b�h�ɔz�u�����Ȃ��Ȃ�z�u
                    if (ConfirmationGrid(pos) == false)
                    {
                        //�{���A�C�e����z�u
                        SetItem(D3DXVECTOR3(pos.x, GRID_SIZE / 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_001);
                        nCntBomb++;
                    }

                    //���[�v�񐔉��Z
                    nCntLoop++;

                    //�������[�v���甲����
                    if (nCntLoop == AVOID_INFINITE_LOOP)
                    {
                        nCntBomb = g_Player2.nMaxBomb - PLAYER_FIRST_BOMB;
                    }
                }
            }

            //���[�v�񐔃��Z�b�g
            nCntLoop = 0;

            //�X�s�[�h���l�����Ă���Ȃ�
            if (g_Player2.nSpeedRank > PLAYER_FIRST_SPEEDRANK)
            {
                //�����_���ɁA�X�s�[�h�A�C�e����ݒu
                while (nCntSpeed != g_Player2.nSpeedRank - PLAYER_FIRST_SPEEDRANK)//�ݒu������܂Ń��[�v
                {
                    //�����_����X�AZ���W�����߂�
                    posX = float(rand() % 1200 + 100);
                    posZ = float(rand() % 1000 + 100);
                    pos = D3DXVECTOR3(posX, 0.0f, posZ);

                    //�O���b�h���W�ɕϊ�����
                    pos = ConversionGrid(&pos);

                    //���̃O���b�h�ɔz�u�����Ȃ��Ȃ�z�u
                    if (ConfirmationGrid(pos) == false)
                    {
                        //�X�s�[�h�A�C�e����z�u
                        SetItem(D3DXVECTOR3(pos.x, GRID_SIZE / 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_002);
                        nCntSpeed++;
                    }

                    //���[�v�񐔉��Z
                    nCntLoop++;

                    //�������[�v���甲����
                    if (nCntLoop == AVOID_INFINITE_LOOP)
                    {
                        nCntSpeed = g_Player2.nSpeedRank - PLAYER_FIRST_SPEEDRANK;
                    }
                }
            }

            //���[�v�񐔃��Z�b�g
            nCntLoop = 0;

            //�L�b�N���l�����Ă���Ȃ�
            if (g_Player2.nCanKick > PLAYER_FIRST_KICK)
            {
                //�����_���ɁA�L�b�N�A�C�e����ݒu
                while (nCntKick != g_Player2.nCanKick - PLAYER_FIRST_KICK)//�ݒu������܂Ń��[�v
                {
                    //�����_����X�AZ���W�����߂�
                    posX = float(rand() % 1200 + 100);
                    posZ = float(rand() % 1000 + 100);
                    pos = D3DXVECTOR3(posX, 0.0f, posZ);

                    //�O���b�h���W�ɕϊ�����
                    pos = ConversionGrid(&pos);

                    //���̃O���b�h�ɔz�u�����Ȃ��Ȃ�z�u
                    if (ConfirmationGrid(pos) == false)
                    {
                        //�L�b�N�A�C�e����z�u
                        SetItem(D3DXVECTOR3(pos.x, GRID_SIZE / 2.0f, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_003);
                        nCntKick++;
                    }

                    //���[�v�񐔉��Z
                    nCntLoop++;

                    //�������[�v���甲����
                    if (nCntLoop == AVOID_INFINITE_LOOP)
                    {
                        nCntKick = g_Player2.nCanKick - PLAYER_FIRST_KICK;
                    }
                }
            }

            //���[�v�񐔃��Z�b�g
            nCntLoop = 0;

            //�ʒu��������
            g_Player2.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

            //���������炷
            pGame->nCounter--;
        }
    }

    //�E�ӂ̔g����ݒ�
    if (g_bEvilUra == true)
    {
        //�������Ă���Ȃ���
        if (g_Player2.bDisp == true)
        {
            float fSpeedY = float(rand() % 800 + 300) / 100.0f;
            //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
            float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            float fRadius = float(rand() % 5000 + 0) / 100.0f;
            D3DXVECTOR3 pos = D3DXVECTOR3(g_Player2.pos.x - sinf(fAngle)*fRadius, GRID_SIZE / 2, g_Player2.pos.z - cosf(fAngle)*fRadius);
            D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
            SetEffect(pos, move, EFFECT_SIZE_EVIL, EFFECTTYPE_EVIL); //�E�ӂ̔g��
        }
    }

    //�o�g���I����
    if (pGame->state == GAMESTATE_FINSIH_ROUND || pGame->state == GAMESTATE_FINSIH_GAME)
    {
        g_Player2.rot.y = 0.0f;  //���ʂ�����

        //���������ȊO��
        if (pGame->nCounter > 0)
        {
            g_MotionState = MOTIONSTATE_PLAYER2_PLEASURE;    //���
        }

        //��������
        else
        {
            g_MotionState = MOTIONSTATE_PLAYER2_REGRET;    //��������
        }        
    }

    //���f���̃A�j���[�V����
    UpdatePlayer2Motion();
}

//===================================================================================================
// �v���C���[�`�揈��
// Author:�㓡�T�V��
//===================================================================================================
void DrawPlayer2(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^
    D3DXMATRIX mtxRot, mtxTrans, mtxParent;
    D3DMATERIAL9 matDef;		// ���݂̃}�e���A���ێ��p
    D3DXMATERIAL* pMat;			// �}�e���A���f�[�^�ւ̃|�C���^

    //�v���C���[�����݂��Ă���Ȃ�`��
    if (g_Player2.bDisp == true && g_Player2.bTransparence == false)
    {
        // ���[���h�}�g���b�N�X�̏�����
        D3DXMatrixIdentity(&g_Player2.mtxWorld);

        // �����𔽉f
        D3DXMatrixRotationYawPitchRoll(&mtxRot,
            g_Player2.rot.y,
            g_Player2.rot.x,
            g_Player2.rot.z);
        D3DXMatrixMultiply(&g_Player2.mtxWorld, &g_Player2.mtxWorld, &mtxRot);

        // �ʒu�𔽉f
        D3DXMatrixTranslation(&mtxTrans,
            g_Player2.pos.x,
            g_Player2.pos.y,
            g_Player2.pos.z);
        D3DXMatrixMultiply(&g_Player2.mtxWorld, &g_Player2.mtxWorld, &mtxTrans);

        // �p�[�c�̐ݒ�
        for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
        {
            // ���[���h�}�g���b�N�X�̏�����
            D3DXMatrixIdentity(&g_Player2.modelParts[nCntParts].mtxWorld);

            // �����𔽉f
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_aPlayer2[nCntParts].rot.y,
                g_aPlayer2[nCntParts].rot.x,
                g_aPlayer2[nCntParts].rot.z);

            D3DXMatrixMultiply(
                &g_Player2.modelParts[nCntParts].mtxWorld,
                &g_Player2.modelParts[nCntParts].mtxWorld, &mtxRot);

            // �ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans,
                g_aPlayer2[nCntParts].pos.x,
                g_aPlayer2[nCntParts].pos.y,
                g_aPlayer2[nCntParts].pos.z);
            D3DXMatrixMultiply(
                &g_Player2.modelParts[nCntParts].mtxWorld,
                &g_Player2.modelParts[nCntParts].mtxWorld, &mtxTrans);

            // �e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, NULL);

            // ���݂̃}�e���A�����擾����
            pDevice->GetMaterial(&matDef);

            // �}�e���A���f�[�^�ւ̃|�C���^���擾
            pMat = (D3DXMATERIAL*)g_Player2.modelParts[nCntParts].pBuffMat->GetBufferPointer();

            // �e���f���̃C���f�b�N�X�� - 1�̎�
            if (g_Player2.modelParts[nCntParts].nldxModelParent == -1)
            {
                mtxParent = g_Player2.mtxWorld;		// �v���C���[�̃}�g���N�X
            }
            else
            {
                mtxParent = g_Player2.modelParts[g_Player2.modelParts[nCntParts].nldxModelParent].mtxWorld;
            }
            // �e���f���̃}�g���b�N�X�̔���
            D3DXMatrixMultiply(
                &g_Player2.modelParts[nCntParts].mtxWorld,
                &g_Player2.modelParts[nCntParts].mtxWorld, &mtxParent);

            // ���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_Player2.modelParts[nCntParts].mtxWorld);

            for (int nCntMat = 0; nCntMat < (int)g_Player2.modelParts[nCntParts].nNumMat; nCntMat++)
            {
                // �}�e���A���̐ݒ�
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                g_Player2.modelParts[nCntParts].pMesh->DrawSubset(nCntMat);
            }

            // �ێ����Ă����}�e���A����߂�
            pDevice->SetMaterial(&matDef);
        }
    }
}

//===========================================================================================================================
// �v���C���[�̎擾
// Author:�㓡�T�V��
//===========================================================================================================================
PLAYER2*GetPlayer2(void)
{
    return &g_Player2;
}

//===========================================================================================================================
// �v���C���[�̃_���[�W�ݒ�
// Author:�㓡�T�V��
//===========================================================================================================================
void HitPlayer2(float fDamage)
{
    //���G���ԂłȂ��Ȃ�
    if (g_Player2.nCntInvincibleFrame == 0)
    {
        GAME *pGame = GetGame();

        //�Q�[�����I�������A�_���[�W���󂯂Ȃ�
        if (pGame->state == GAMESTATE_NORMAL)
        {
            //�_���[�W�v�Z
            g_Player2.fLife -= fDamage;

            //���G���Ԃ�����
            g_Player2.nCntInvincibleFrame = PLAYER_INVINCIBLE_FRAME;

            //HP������Ȃ�
            if (g_Player2.fLife > 0.0f)
            {
                PlaySound(SOUND_LABEL_SE_TAKE_DAMAGE_VOICE2);    //�_���[�W���󂯂����̐�
            }
        }
    }
}

//=====================================================
// �v���C���[�ɑ΂���Փ˔���̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool CollisionPlayer2(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    //�Փ˂������ǂ���
    bool bCollision = false;

    // �v���C���[���g���Ă���Ȃ�
    if (g_Player2.bDisp == true)
    {
        //�Փ˔�����v�Z
        if (BoxCollision(pPos, pPosOld, size, &g_Player2.pos,
            D3DXVECTOR3(GRID_SIZE - (GRID_SIZE / 2.0f + g_Player2.fSpeed), PLAYER_HEIGHT * 2, GRID_SIZE - (GRID_SIZE / 2.0f + g_Player2.fSpeed))) == true)
        {
            bCollision = true;
        }
    }

    return bCollision;
}

//===========================================================================================================================
// �A�j���[�V�������[�h
// Author:���� �D�l
//===========================================================================================================================
void LoadPlayer2Animation(void)
{
    // �t�@�C���|�C���g
    FILE *pFile;

    // �ϐ��錾
    int  nCntLoad = 0;
    int	 nCntKey = 0;
    int	 nCntMotion = 0;

    char cReedText[1024];	// �����Ƃ��ēǂݎ��p
    char cHeadText[1024];	//
    char cDie[1024];

    // �t�@�C���J
    pFile = fopen("TXT/motion2.txt", "r");

    // �J����
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "SCRIPT") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);
        }

        if (strcmp(cHeadText, "SCRIPT") == 0)
        {
            while (strcmp(cHeadText, "END_SCRIPT") != 0)
            {

                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "MOTIONSET") == 0)
                {

                    while (strcmp(cHeadText, "END_MOTIONSET") != 0)
                    {
                        fgets(cReedText, sizeof(cReedText), pFile);
                        sscanf(cReedText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "LOOP") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer2Motion[nCntMotion].nLoop);
                        }

                        if (strcmp(cHeadText, "NUM_KEY") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer2Motion[nCntMotion].nNum_Key);
                        }

                        //			if (strcmp(cHeadText, "COLLISION") == 0)
                        //			{
                        //				sscanf(cReedText, "%s %s %d %f %f %f %f %d %d", &cDie, &cDie, &g_Player2Collision.nPartsInfo, &g_Player2Collision.CollisionPos.x, &g_Player2Collision.CollisionPos.y, &g_Player2Collision.CollisionPos.z, &g_Player2Collision.fLength, &g_Player2Collision.nFirstFlame[nCntMotion], &g_Player2Collision.nLastFlame);
                        //			}
                        if (strcmp(cHeadText, "KEYSET") == 0)
                        {

                            while (strcmp(cHeadText, "END_KEYSET") != 0)
                            {

                                fgets(cReedText, sizeof(cReedText), pFile);
                                sscanf(cReedText, "%s", &cHeadText);

                                if (strcmp(cHeadText, "FRAME") == 0)
                                {
                                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer2Motion[nCntMotion].KeyInfo[nCntKey].nFrame);
                                }
                                if (strcmp(cHeadText, "KEY") == 0)
                                {
                                    while (strcmp(cHeadText, "END_KEY") != 0)
                                    {
                                        fgets(cReedText, sizeof(cReedText), pFile);
                                        sscanf(cReedText, "%s", &cHeadText);

                                        if (strcmp(cHeadText, "POS") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &g_aPlayer2Motion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].x,
                                                &g_aPlayer2Motion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].y,
                                                &g_aPlayer2Motion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].z);
                                        }

                                        if (strcmp(cHeadText, "ROT") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &g_aPlayer2Motion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].x,
                                                &g_aPlayer2Motion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].y,
                                                &g_aPlayer2Motion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].z);
                                        }
                                    }
                                    nCntLoad++;
                                }
                            }
                            nCntLoad = 0;
                            nCntKey++;
                        }
                    }
                    nCntKey = 0;
                    nCntMotion++;
                }
            }
            // �t�@�C����
            fclose(pFile);
        }
        // �J���Ȃ�
        else
        {
            printf("�J����܂���ł���\n");
        }
    }
}

//===========================================================================================================================
// ���f�����[�h
// Author:���� �D�l
//===========================================================================================================================
void LoadPlayer2(void)
{
    // �t�@�C���|�C���g
    FILE *pFile;

    // �ϐ��錾
    int  nCntLoad = 0;
    char cReedText[128];	// �����Ƃ��ēǂݎ��p
    char cHeadText[256];	//
    char cDie[128];

    // �t�@�C���J
    pFile = fopen("TXT/player2.txt", "r");

    // �J����
    if (pFile != NULL)
    {
        while (strcmp(cHeadText, "CHARACTERSET") != 0)
        {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);
        }

        if (strcmp(cHeadText, "CHARACTERSET") == 0)
        {
            while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
            {

                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if (strcmp(cHeadText, "\n") == 0)
                {
                }
                else if (strcmp(cHeadText, "PARTSSET") == 0)
                {
                    while (strcmp(cHeadText, "END_PARTSSET") != 0)
                    {

                        fgets(cReedText, sizeof(cReedText), pFile);
                        sscanf(cReedText, "%s", &cHeadText);

                        if (strcmp(cHeadText, "INDEX") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer2[nCntLoad].nData);
                        }

                        if (strcmp(cHeadText, "PARENT") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer2[nCntLoad].nParents);
                        }

                        if (strcmp(cHeadText, "POS") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                &g_aPlayer2[nCntLoad].posOrigin.x,
                                &g_aPlayer2[nCntLoad].posOrigin.y,
                                &g_aPlayer2[nCntLoad].posOrigin.z);

                            g_aPlayer2[nCntLoad].pos = g_aPlayer2[nCntLoad].posOrigin;
                        }

                        if (strcmp(cHeadText, "ROT") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                &g_aPlayer2[nCntLoad].rot.x,
                                &g_aPlayer2[nCntLoad].rot.y,
                                &g_aPlayer2[nCntLoad].rot.z);
                        }
                    }
                    nCntLoad++;
                }
            }
        }
        // �t�@�C����
        fclose(pFile);
    }

    // �J���Ȃ�
    else
    {
        printf("�J����܂���ł���\n");
    }
}

//===========================================================================================================================
// ���[�V�����̍X�V
// Author:���� �D�l
//===========================================================================================================================
void UpdatePlayer2Motion(void)
{
    //���[�V�������ς�����Ƃ�
    if (g_MotionOldState != g_MotionState) {
        g_nFremePlayer2 = 0;
        g_nCntKeyPlayer2 = 0;
    }
    for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
    {
        if (g_nFremePlayer2 == 0)
        {
            g_aNumPlayer2Pos[nCntMotion].x = ((g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].pos[nCntMotion].x -
                (g_aPlayer2[nCntMotion].pos.x - g_aPlayer2[nCntMotion].posOrigin.x)) / float(g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].nFrame));

            g_aNumPlayer2Pos[nCntMotion].y = ((g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].pos[nCntMotion].y -
                (g_aPlayer2[nCntMotion].pos.y - g_aPlayer2[nCntMotion].posOrigin.y)) / float(g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].nFrame));

            g_aNumPlayer2Pos[nCntMotion].z = ((g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].pos[nCntMotion].z -
                (g_aPlayer2[nCntMotion].pos.z - g_aPlayer2[nCntMotion].posOrigin.z)) / float(g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].nFrame));

            g_aNumPlayer2RotDest[nCntMotion] =
                g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].rot[nCntMotion] - g_aPlayer2[nCntMotion].rot;

            g_aNumPlayer2Rot[nCntMotion] = (g_aNumPlayer2RotDest[nCntMotion] / float(g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].nFrame));
        }

        g_aPlayer2[nCntMotion].pos += g_aNumPlayer2Pos[nCntMotion];
        g_aPlayer2[nCntMotion].rot += g_aNumPlayer2Rot[nCntMotion];
    }
    g_MotionOldState = g_MotionState;
    // ���[�v����Ƃ�
    if (g_nFremePlayer2 >= g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].nFrame && g_aPlayer2Motion[g_MotionState].nLoop == 1)
    {
        g_nFremePlayer2 = 0;
        g_nCntKeyPlayer2++;

        if (g_nCntKeyPlayer2 >= g_aPlayer2Motion[g_MotionState].nNum_Key)
        {
            g_nCntKeyPlayer2 = 0;
        }

    }
    // ���[�v���Ȃ��Ƃ�
    else if (g_nFremePlayer2 >= g_aPlayer2Motion[g_MotionState].KeyInfo[g_nCntKeyPlayer2].nFrame && g_aPlayer2Motion[g_MotionState].nLoop == 0)
    {
        g_nFremePlayer2 = 0;
        g_nCntKeyPlayer2++;
        if (g_nCntKeyPlayer2 >= g_aPlayer2Motion[g_MotionState].nNum_Key)
        {
            for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
            {
                g_aNumPlayer2Pos[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aNumPlayer2Rot[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
            g_MotionState = MOTIONSTATE_PLAYER2_IDLE;
            g_nCntKeyPlayer2 = 0;
        }
    }
    else
    {
        g_nFremePlayer2++;
    }
}

//===========================================================================================================================
// �g�p����̏�����
// Author:�㓡�T�V��
//===========================================================================================================================
void InitDispPlayer2(void)
{
    g_Player2.bDisp = false;
}