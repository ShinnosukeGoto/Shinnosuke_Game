//=============================================================================
//
// �ŋ��̘b�����̏���[player.cpp]�i���@1�j
// Author:�㓡�T�V��
//
//=============================================================================

//===================================================================================================
// �C���N���[�h�t�@�C��
//===================================================================================================
#include "player.h"
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
#include "setting.h"
#include "manual.h"
#include "enemy.h"
#include "bullet.h"
#include "ui.h"
#include "boss.h"

//===================================================================================================
// �O���[�o���ϐ���`
//===================================================================================================
PLAYER g_Player;        //�v���C���[�̏��

int g_nFrameCountPlayer;				// ���݂̃t���[����

int g_nPlayKeySetlndexPlayer;			// �Đ����̃L�[�Z�b�g�C���f�b�N�X

int g_nPlayerCurrentAnimationPlayer;	// �Đ����̃A�j���[�V����

int g_nFremePlayer;					// �t���[����

int g_nCntKeyPlayer;					// �L�[��

int g_nChangeFlamePlayer;			// ���[�V�����̃t���[��

MOTIONSTATE_PLAYER g_MotionState;			// ���[�V�����̍\����
MOTIONSTATE_PLAYER g_MotionOldState;		// 1F�O�̃��[�V�����̍\����

D3DXVECTOR3 g_aNumPlayerPos[MAX_PARTS];		// pos�̍���
D3DXVECTOR3 g_aNumPlayerRot[MAX_PARTS];		// rot�̍���
D3DXVECTOR3 g_aNumPlayerRotDest[MAX_PARTS];	// rot�̍���

ANIMATION_PLAYER g_aPlayerMotion[MOTIONSTATE_PLAYER_MAX];	// ���[�V�����̏��
                                    // ���f���p�[�c�̃f�[�^
ModelParts_Player g_aPlayer[MAX_PARTS] =
{
    { "MODEL/player/00_body.x" },
    { "MODEL/player/01_head.x" },
    { "MODEL/player/02_armR.x" },
    { "MODEL/player/03_handR.x" },
    { "MODEL/player/04_armL.x" },
    { "MODEL/player/05_handL.x" },
    { "MODEL/player/06_legR.x" },
    { "MODEL/player/07_footR.x" },
    { "MODEL/player/08_legL.x" },
    { "MODEL/player/09_footL.x" },
};

bool g_bEvilMomo;       //�E�ӂ̔g���ɖڊo�߂Ă��邩�ǂ���

bool g_bSetSnake;       //�c�`�m�R���Z�b�g�������ǂ���

bool g_bSetFenceBlock;  //����Z�b�g�������ǂ���

int g_nCntStunPlayer;   //�X�^���̎��Ԃ𐔂���

//===================================================================================================
// �v���C���[����������
// Author:�㓡�T�V��
//===================================================================================================
void InitPlayer(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�Q�[����Ԃ̎擾
    GAME *pGame = GetGame();

    //�ݒ�̎擾
    SETTING *pSetting = GetSetting();

    //�B���R�}���h�g�p������擾
    g_bEvilMomo = GetHiddenCommand();

    //�O���[�o���ϐ��̏�����
    g_MotionState = MOTIONSTATE_PLAYER_IDLE;

    g_nFrameCountPlayer = 0;				// ���݂̃t���[����

    g_nPlayKeySetlndexPlayer = 0;			// �Đ����̃L�[�Z�b�g�C���f�b�N�X

    g_nPlayerCurrentAnimationPlayer = MOTIONSTATE_PLAYER_IDLE;	// �Đ����̃A�j���[�V����

    g_nFremePlayer = 0;					// �t���[����

    g_nCntKeyPlayer = 1;				// �L�[��

    g_nChangeFlamePlayer = 20;			// ���[�V�����̃t���[��

    //�X�g�[���[�Ɏg���A�O���[�o���ϐ��̏�����
    g_bSetSnake = false;
    g_bSetFenceBlock = false;
    g_nCntStunPlayer = PLAYER_STUN_TIME;

    //�\���̂̏�����
    g_Player.pos = pGame->startPos1;		            // �����ʒu
    g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ʒu
    g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��������
    g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	    // �����ړ���
    g_Player.posGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ʒu
    g_Player.bDisp = true;
    //�X�g�[���[���ǂ���
    if (pSetting->nMaxPlayer == 1)
    {
        //��Փx:�ӂ�
        if (pSetting->nLevel == SETTINGLEVEL_NORMAL)
        {
            g_Player.fLife = PLAYER_STORY_LIFE;	//���@�̗̑�
        }

        //��Փx:�S
        else
        {
            g_Player.fLife = PLAYER_LIFE;	//���@�̗̑�
        }
    }
    else
    {
        //�B���R�}���h�g�p
        if (g_bEvilMomo == true)
        {
            g_Player.fLife = BOMB_DAMAGE + PLAYER_LIFE;	    //�E�ӂ̔g���͔��������ς���
        }

        //�B���R�}���h���g�p
        else
        {
            g_Player.fLife = PLAYER_LIFE;
        }
        
    }
    g_Player.fSpeed = VALUE_MOVE;
    //�B���R�}���h�g�p
    if (g_bEvilMomo == true)
    {
        g_Player.nSpeedRank = PLAYER_FIRST_SPEEDRANK + 2;   //�E�ӂ̔g����2�����N����
        g_Player.nBomb = PLAYER_FIRST_BOMB + 1;             //�E�ӂ̔g���̓{����2������
        g_Player.nCanKick = PLAYER_FIRST_KICK + 1;          //�E�ӂ̔g���̓L�b�N�ł���
    }

    //�B���R�}���h���g�p
    else
    {
        g_Player.nSpeedRank = PLAYER_FIRST_SPEEDRANK;
        g_Player.nBomb = PLAYER_FIRST_BOMB;
        g_Player.nCanKick = PLAYER_FIRST_KICK;
    }
    g_Player.nFire = PLAYER_FIRST_FIRE;
    g_Player.nShadow = SetShadow(g_Player.pos, g_Player.rot, GRID_SIZE, g_Player.pos.y, 
        D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_ALPHA_MAX));//�e�̐ݒ�
    g_Player.nCntInvincibleFrame = 0;
    g_Player.bTransparence = false;
    g_Player.nMaxBomb = g_Player.nBomb;
    g_Player.bStun = false;

    LoadPlayer();
    LoadPlayerAnimation();

    // ���f���p�[�c�̏�����
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        //		g_Player.modelParts[nCntParts].pos = g_aPlayer[nCntParts].pos;			// �����ʒu
        //		g_Player.modelParts[nCntParts].rot = g_aPlayer[nCntParts].rot;			// �����p�x

        g_Player.modelParts[nCntParts].nldxModelParent = 0;
    }
    // �t�@�C���̓ǂݍ���
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        g_Player.modelParts[nCntParts].nldxModelParent = g_aPlayer[nCntParts].nParents;

        // X�t�@�C���̓ǂݍ���
        D3DXLoadMeshFromX(g_aPlayer[nCntParts].pFileName,
            D3DXMESH_SYSTEMMEM,
            pDevice,
            NULL,
            &g_Player.modelParts[nCntParts].pBuffMat,
            NULL,
            &g_Player.modelParts[nCntParts].nNumMat,
            &g_Player.modelParts[nCntParts].pMesh);

    }
}

//===================================================================================================
// �v���C���[�I������
// Author:�㓡�T�V��
//===================================================================================================
void UninitPlayer(void)
{
    for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
    {
        // ���b�V���̔j��
        if (g_Player.modelParts[nCntParts].pMesh != NULL)
        {
            g_Player.modelParts[nCntParts].pMesh->Release();
            g_Player.modelParts[nCntParts].pMesh = NULL;
        }
        // �}�e���A���̔j��
        if (g_Player.modelParts[nCntParts].pBuffMat != NULL)
        {
            g_Player.modelParts[nCntParts].pBuffMat->Release();
            g_Player.modelParts[nCntParts].pBuffMat = NULL;
        }
    }
}

//===================================================================================================
// �v���C���[�X�V����
// Author:�㓡�T�V��
//===================================================================================================
void UpdatePlayer(void)
{
    //�R���g���[���[�̏����擾����
    DIJOYSTATE2 Controller = GetController(PLAYER_1);
    //�J�����̏����擾����
    CAMERA *pCamera = GetCamera();
    //�Q�[���̏󋵂��擾����
    GAME *pGame = GetGame();
    //�ݒ�̏����擾����
    SETTING *pSetting = GetSetting();

    //�ړ������N�ɉ������A�����𓾂�
    switch (g_Player.nSpeedRank)
    {
    case PLAYER_FIRST_SPEEDRANK:
        g_Player.fSpeed = VALUE_MOVE;
        break;

    case PLAYER_FIRST_SPEEDRANK + 1:
        g_Player.fSpeed = VALUE_MOVE + VALUE_UPMOVE;
        break;

    case PLAYER_FIRST_SPEEDRANK + 2:
        g_Player.fSpeed = VALUE_MOVE + VALUE_UPMOVE * 2.0f;
        break;

    case PLAYER_FIRST_SPEEDRANK + 3:
        g_Player.fSpeed = VALUE_MOVE + VALUE_UPMOVE * 3.0f;
        break;

    case PLAYER_FIRST_SPEEDRANK + 4:
        g_Player.fSpeed = VALUE_MOVE + VALUE_UPMOVE * 4.0f;
        break;
    }

    //���G����
    if (g_Player.nCntInvincibleFrame > 0)
    {
        g_Player.nCntInvincibleFrame--;                     //���G���Ԃ𐔂���
        g_Player.bTransparence = !g_Player.bTransparence;   //���G���́A�_�ł�����
    }
    else
    {
        g_Player.bTransparence = false; //����������
    }

    //�������Ă���Ȃ�
    if (g_Player.bDisp == true && pGame->state == GAMESTATE_NORMAL && pCamera->state == CAMERASTATE_NORMAL ||
        g_Player.bDisp == true && pGame->state == GAMESTATE_NORMAL && pCamera->state == CAMERASTATE_STORY)
    {
        // �v���C���[�������Ă��Ȃ��Ƃ�
        D3DXVECTOR3 lengthCheckVec = g_Player.move;
        lengthCheckVec.y = 0;
        if (D3DXVec3Length(&lengthCheckVec) <= 1.5f)
        {
            g_MotionState = MOTIONSTATE_PLAYER_IDLE;    //���[�V�������A�C�h���ɂ���
        }

        //1F�O�̈ʒu���L��
        g_Player.posOld = g_Player.pos;

        //�O���b�h�̒��S�̒l�𓾂�
        g_Player.posGrid = ConversionGrid(&g_Player.pos);

        //�m�b�N�o�b�N�𐧌�
        if (g_Player.move.x != 0.0f || g_Player.move.z != 0.0f)
        {
            g_Player.move.x *= 0.8f;
            g_Player.move.z *= 0.8f;
        }

        //�ړ��ʂƈʒu��R�Â�
        g_Player.pos += g_Player.move;

        //�X�^�����Ă��Ȃ��Ȃ�
        if (g_Player.bStun == false)
        {
            //�X�e�B�b�N���|��Ă���Ȃ�ړ�
            if (Controller.lY != 0 || Controller.lX != 0)
            {
                float fAngle = atan2(Controller.lX, Controller.lY*-1);//�X�e�B�b�N�̊p�x�����߂�
                float fPlayerAngle = atan2(Controller.lX*-1, Controller.lY);//�X�e�B�b�N�̊p�x�����߂�

                g_Player.pos.x += sinf((pCamera->rot.y + fAngle))*g_Player.fSpeed;
                g_Player.pos.z += cosf((pCamera->rot.y + fAngle))*g_Player.fSpeed;
                g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                                                        //�L�����̌�����ς���
                g_Player.rot.y = pCamera->rot.y + fPlayerAngle;
            }

            //�\���L�[�ł̈ړ�
            else
            {
                //��
                if (Controller.rgdwPOV[0] == BUTTON_LEFT)
                {
                    g_Player.pos.x -= g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x + D3DXToRadian(90.0f);
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                }

                //����
                if (Controller.rgdwPOV[0] == BUTTON_LEFT_UP)
                {
                    g_Player.pos.x -= sinf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.pos.z += cosf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x + D3DXToRadian(135.0f);
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                }

                //����
                if (Controller.rgdwPOV[0] == BUTTON_DOWN_LEFT)
                {
                    g_Player.pos.x -= sinf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.pos.z -= cosf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x + D3DXToRadian(45.0f);
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                }

                //�E
                if (Controller.rgdwPOV[0] == BUTTON_RIGHT)
                {
                    g_Player.pos.x += g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x - D3DXToRadian(90.0f);
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                }

                //�E��
                if (Controller.rgdwPOV[0] == BUTTON_UP_RIGHT)
                {
                    g_Player.pos.x += sinf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.pos.z += cosf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x - D3DXToRadian(135.0f);
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                }

                //�E��
                if (Controller.rgdwPOV[0] == BUTTON_RIGHT_DOWN)
                {
                    g_Player.pos.x += sinf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.pos.z -= cosf(D3DX_PI / 4) * g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x - D3DXToRadian(45.0f);
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                }

                //��
                if (Controller.rgdwPOV[0] == BUTTON_UP)
                {
                    g_Player.pos.z += g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x + D3DX_PI;
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                }

                //��
                if (Controller.rgdwPOV[0] == BUTTON_DOWN)
                {
                    g_Player.pos.z -= g_Player.fSpeed;
                    g_Player.rot.y = pCamera->rot.x;
                    g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                }
            }

            //�X�g�[���[���[�h�̉��̑���===========================================================================================
            if (Controller.lY == 0 && Controller.lX == 0 && Controller.rgdwPOV[0] == BUTTON_NEUTRAL && pSetting->nMaxPlayer == 1)
            {
                //WASD�L�[�ňړ�
                //A�L�[�������ꂽ(���ړ�)
                if (GetKeyboardPress(DIK_A))
                {
                    if (GetKeyboardPress(DIK_W))//A��W�������ɉ����ꂽ
                    {
                        g_Player.pos.x -= sinf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.pos.z += cosf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x + D3DXToRadian(135.0f);
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                    }
                    else if (GetKeyboardPress(DIK_S))//A��S�������ɉ����ꂽ
                    {
                        g_Player.pos.x -= sinf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.pos.z -= cosf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x + D3DXToRadian(45.0f);
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                    }
                    else if (GetKeyboardPress(DIK_D))
                    {

                    }
                    else				//������������
                    {
                        g_Player.pos.x -= g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x + D3DXToRadian(90.0f);
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                    }

                }

                //D�L�[�������ꂽ(�E�ړ�)
                else if (GetKeyboardPress(DIK_D))
                {
                    if (GetKeyboardPress(DIK_W))//D��W�������ɉ����ꂽ
                    {
                        g_Player.pos.x += sinf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.pos.z += cosf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x - D3DXToRadian(135.0f);
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���

                    }
                    else if (GetKeyboardPress(DIK_S))//D��S�������ɉ����ꂽ
                    {
                        g_Player.pos.x += sinf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.pos.z -= cosf(D3DX_PI / 4) * g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x - D3DXToRadian(45.0f);
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                    }
                    else if (GetKeyboardPress(DIK_A))
                    {

                    }
                    else				//�E����������
                    {
                        g_Player.pos.x += g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x - D3DXToRadian(90.0f);
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                    }

                }

                //W�L�[�������ꂽ(��ړ�)
                else if (GetKeyboardPress(DIK_W))
                {
                    if (GetKeyboardPress(DIK_S))
                    {

                    }
                    else
                    {
                        g_Player.pos.z += g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x + D3DX_PI;
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                    }
                }

                //S�L�[�������ꂽ(���ړ�)
                else if (GetKeyboardPress(DIK_S))
                {
                    if (GetKeyboardPress(DIK_W))
                    {

                    }
                    else
                    {
                        g_Player.pos.z -= g_Player.fSpeed;
                        g_Player.rot.y = pCamera->rot.x;
                        g_MotionState = MOTIONSTATE_PLAYER_WALK;//���[�V����������ɂ���
                    }
                }

                //���e��u��(��)
                if (GetKeyboardTrigger(DIK_RETURN) && g_Player.nBomb > 0 && g_Player.nCntInvincibleFrame == 0)
                {
                    //���e�Əd�Ȃ��Ă��Ȃ��Ȃ�A�u����
                    if (HitBomb(&g_Player.pos, &g_Player.posOld,
                        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BOMB + 1) == false)
                    {
                        PlaySound(SOUND_LABEL_SE_SET_BOMB);   //�{���ݒu��
                        SetBomb(g_Player.posGrid, g_Player.nFire, TIME_BOMB, SELECT_MOMO);
                        g_Player.nBomb--;
                    }

                }
            }
            //===============================================================================================

            //���e��u��
            if (GetJoypadTrigger(PLAYER_1, BUTTON_B) && g_Player.nBomb > 0 && g_Player.nCntInvincibleFrame == 0)
            {
                //�E�ӂ̔g���ɖڊo�߂ĂȂ��Ȃ�
                if (g_bEvilMomo == false)
                {
                    //���e�Əd�Ȃ��Ă��Ȃ��Ȃ�A3�b�Ŕ�������{����u����
                    if (HitBomb(&g_Player.pos, &g_Player.posOld,
                        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BOMB + 1) == false)
                    {
                        PlaySound(SOUND_LABEL_SE_SET_BOMB);   //�{���ݒu��
                        SetBomb(g_Player.posGrid, g_Player.nFire, TIME_BOMB, SELECT_MOMO);
                        g_Player.nBomb--;
                    }
                }

                //�E�ӂ̔g���ɖڊo�߂Ă���Ȃ�
                else
                {
                    //���e�Əd�Ȃ��Ă��Ȃ��Ȃ�A2�b�Ŕ�������{����u����
                    if (HitBomb(&g_Player.pos, &g_Player.posOld,
                        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BOMB + 1) == false)
                    {
                        PlaySound(SOUND_LABEL_SE_SET_BOMB);   //�{���ݒu��
                        SetBomb(g_Player.posGrid, g_Player.nFire, TIME_BOMB - 60, SELECT_MOMO);
                        g_Player.nBomb--;
                    }
                }
            }
        }

        //�N���b�V������
        if (CrushingBlock(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), MAX_BLOCK + 1) == true)
        {
            HitPlayer(BOMB_DAMAGE*4);
        }

        //�u���b�N�̓����蔻��
        CollisionBlock(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID));

        //�L�b�N�ł��Ȃ�
        if (g_Player.nCanKick <= 0)
        {
            //���e�̓����蔻��
            CollisionBomb(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID));
        }

        //�L�b�N�ł���
        else
        {
            //���e�̃L�b�N����
            KickBomb(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - (GRID_SIZE/2.0f + g_Player.fSpeed)
                    , PLAYER_HEIGHT * 2, GRID_SIZE -(GRID_SIZE / 2.0f + g_Player.fSpeed)), SELECT_MOMO);

            //���e�̓����蔻��
            CollisionBomb(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID));
        }

        //���̓����蔻��
        if (HitFire(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed, PLAYER_HEIGHT * 2,
                GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed)) == true)
        {
            HitPlayer(BOMB_DAMAGE);
        }

        //�X�g�[���[�Ȃ�
        if (pSetting->nMaxPlayer == 1)
        {
            //�G�l�~�[�Ƃ̓����蔻��
            if (CollisionEnemy(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed
                    , PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed), true) == true)
            {
                HitPlayer(HIT_ENEMY_DAMAGE);
            }

            //�{�X�Ƃ̓����蔻��
            if (CollisionBoss(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed
                    , PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed), true) == true)
            {
                HitPlayer(HIT_ENEMY_DAMAGE);
            }

            //�o���b�g�Ƃ̓����蔻��
            CollisionBullet(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed
                    , PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID - g_Player.fSpeed));

            //�n�[�g�Ƃ̓����蔻��
            if (HitItem(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_004) == true)
            {
                g_Player.fLife += PLAYER_STORY_LIFE / 2;    //������
            }

            //�^�C�}�[�Ƃ̓����蔻��
            if (HitItem(&g_Player.pos, &g_Player.posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_005) == true)
            {
                g_nTimeMinute += 1;     //1���ǉ�
            }

            //�c�`�m�R�o��
            if (g_Player.pos.x >= 2300.0f && g_bSetSnake == false)
            {
                g_bSetSnake = true;
                SetEnemy(D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z / 2 + GRID_SIZE / 2), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_SNAKE);  //�v���C���[�̏����ʒu�ɏo��
            }

            //��o���i�{�X��ڍs�j
            if (g_Player.pos.x >= 4550.0f && g_bSetFenceBlock == false)
            {
                g_Player.pos.x = 4600.0f;           //��ɂ߂荞�ނ̂��������
                StopSound(SOUND_LABEL_BGM_GAME);    //�Q�[�������~�߂�
                g_bSetFenceBlock = true;
                SetBlock(D3DXVECTOR3(4500.0f, -GRID_SIZE, 600.0f), BLOCKTYPE_007);  //��u���b�N�z�u
                pCamera->state = CAMERASTATE_REACH_BOSS;    //�{�X��J�b�g�C��
            }

            //�X�^�����Ă���Ȃ�
            if (g_Player.bStun == true)
            {
                //�X�^���̃G�t�F�N�g
                if (g_nCntStunPlayer == PLAYER_STUN_TIME)
                {
                    SetEffect(D3DXVECTOR3(g_Player.pos.x, PLAYER_HEIGHT * 3, g_Player.pos.z)
                        , D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECT_SIZE_STUN, EFFECTTYPE_PLAYER_STUN); //�v���C���[�X�^���G�t�F�N�g
                }

                g_MotionState = MOTIONSTATE_PLAYER_REGRET;    //��������

                g_nCntStunPlayer--; //�X�^�����Ԃ𐔂���

                //�X�^������
                if (g_nCntStunPlayer <= 0)
                {
                    g_nCntStunPlayer = PLAYER_STUN_TIME;
                    g_Player.bStun = false;
                }
            }
        }

        //==============================================================================================================================================================
        //�A�C�e���̓����蔻��
        //��
        if (HitItem(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true ,ITEMTYPE_000) == true)
        {
            g_Player.nFire++;
        }

        //�{��
        if (HitItem(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_001) == true)
        {
            g_Player.nBomb++;
            g_Player.nMaxBomb++;
        }

        //�X�s�[�h
        if (HitItem(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_002) == true)
        {
            g_Player.nSpeedRank++;
        }

        //�L�b�N
        if (HitItem(&g_Player.pos, &g_Player.posOld,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), true, ITEMTYPE_003) == true)
        {
            g_Player.nCanKick++;
        }
        //==============================================================================================================================================================

        //�e��ݒ�
        SetPositionShadow(g_Player.nShadow, D3DXVECTOR3(g_Player.pos.x, 0, g_Player.pos.z), g_Player.pos.y);

        //���S����
        if (g_Player.fLife <= 0.0f)
        {
            //�e�̍폜
            SHADOW *pShadow = GetShadow();//�e�̏����擾
            pShadow[g_Player.nShadow].use = false;

            g_Player.bDisp = false;    //���@�𖢎g�p��

            PlaySound(SOUND_LABEL_SE_MAN_VOICE);   //���ꂽ���̐�

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
                SetEffect(g_Player.pos, move, EFFECT_SIZE_EXPLODE_PLAYER, EFFECTTYPE_EXPLOSION); //�����G�t�F�N�g
            }

            //�o�g�����C�����Ȃ�
            if (pSetting->nMaxPlayer >= 2)
            {
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
                if (g_Player.nFire > PLAYER_FIRST_FIRE)
                {
                    //�����_���ɁA���A�C�e����ݒu
                    while (nCntFire != g_Player.nFire - PLAYER_FIRST_FIRE)//�ݒu������܂Ń��[�v
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
                            nCntFire = g_Player.nFire - PLAYER_FIRST_FIRE;
                        }
                    }
                }

                //���[�v�񐔃��Z�b�g
                nCntLoop = 0;

                //�{�����l�����Ă���Ȃ�
                if (g_Player.nMaxBomb > PLAYER_FIRST_BOMB)
                {
                    //�����_���ɁA�{���A�C�e����ݒu
                    while (nCntBomb != g_Player.nMaxBomb - PLAYER_FIRST_BOMB)//�ݒu������܂Ń��[�v
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
                            nCntBomb = g_Player.nMaxBomb - PLAYER_FIRST_BOMB;
                        }
                    }
                }

                //���[�v�񐔃��Z�b�g
                nCntLoop = 0;

                //�X�s�[�h���l�����Ă���Ȃ�
                if (g_Player.nSpeedRank > PLAYER_FIRST_SPEEDRANK)
                {
                    //�����_���ɁA�X�s�[�h�A�C�e����ݒu
                    while (nCntSpeed != g_Player.nSpeedRank - PLAYER_FIRST_SPEEDRANK)//�ݒu������܂Ń��[�v
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
                            nCntSpeed = g_Player.nSpeedRank - PLAYER_FIRST_SPEEDRANK;
                        }
                    }
                }

                //���[�v�񐔃��Z�b�g
                nCntLoop = 0;

                //�L�b�N���l�����Ă���Ȃ�
                if (g_Player.nCanKick > PLAYER_FIRST_KICK)
                {
                    //�����_���ɁA�L�b�N�A�C�e����ݒu
                    while (nCntKick != g_Player.nCanKick - PLAYER_FIRST_KICK)//�ݒu������܂Ń��[�v
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
                            nCntKick = g_Player.nCanKick - PLAYER_FIRST_KICK;
                        }
                    }
                }

                //���[�v�񐔃��Z�b�g
                nCntLoop = 0;

                //�ʒu�Ȃǂ�߂�(�X�g�[���[�ł́A�J�������ǔ����Ă��܂����ߖ߂��Ȃ�)
                g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }

            //���������炷
            pGame->nCounter--;

            //�X�g�[���[�Ȃ�
            if (pSetting->nMaxPlayer == 1)
            {
                PlaySound(SOUND_LABEL_SE_FINISH);       //�t�B�j�b�V���̌��ʉ�
                SetGameState(GAMESTATE_FINSIH_GAME);    //�Q�[���I�[�o�[
            }
        }
    }

    //�E�ӂ̔g����ݒ�
    if (g_bEvilMomo == true)
    {
        //�������Ă���Ȃ���
        if (g_Player.bDisp == true)
        {
            float fSpeedY = float(rand() % 800 + 300) / 100.0f;
            //0�`3.14���o��*(0 or 1)*-1�Ő��������߂�
            float fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
            float fRadius = float(rand() % 5000 + 0) / 100.0f;
            D3DXVECTOR3 pos = D3DXVECTOR3(g_Player.pos.x - sinf(fAngle)*fRadius, GRID_SIZE / 2, g_Player.pos.z - cosf(fAngle)*fRadius);
            D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
            SetEffect(pos, move, EFFECT_SIZE_EVIL, EFFECTTYPE_EVIL); //�E�ӂ̔g��
        }
    }

    //�o�g���I����
    if (pGame->state == GAMESTATE_FINSIH_ROUND || pGame->state == GAMESTATE_FINSIH_GAME)
    {
        g_Player.rot.y = 0.0f;  //���ʂ�����

        //���������ȊO��
        if (pGame->nCounter > 0)
        {
            g_MotionState = MOTIONSTATE_PLAYER_PLEASURE;    //���
        }

        //��������
        else
        {
            g_MotionState = MOTIONSTATE_PLAYER_REGRET;    //��������
        }
    }
    
    //���f���̃A�j���[�V����
    UpdatePlayerMotion();
}

//===================================================================================================
// �v���C���[�`�揈��
// Author:�㓡�T�V��
//===================================================================================================
void DrawPlayer(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^
    D3DXMATRIX mtxRot, mtxTrans, mtxParent;
    D3DMATERIAL9 matDef;		// ���݂̃}�e���A���ێ��p
    D3DXMATERIAL* pMat;			// �}�e���A���f�[�^�ւ̃|�C���^

    //�v���C���[�����݂��Ă��āA���G���ԂłȂ��Ȃ�`��
    if (g_Player.bDisp == true && g_Player.bTransparence == false)
    {
        // ���[���h�}�g���b�N�X�̏�����
        D3DXMatrixIdentity(&g_Player.mtxWorld);

        // �����𔽉f
        D3DXMatrixRotationYawPitchRoll(&mtxRot,
            g_Player.rot.y,
            g_Player.rot.x,
            g_Player.rot.z);
        D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

        // �ʒu�𔽉f
        D3DXMatrixTranslation(&mtxTrans,
            g_Player.pos.x,
            g_Player.pos.y,
            g_Player.pos.z);
        D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

        // �p�[�c�̐ݒ�
        for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
        {
            // ���[���h�}�g���b�N�X�̏�����
            D3DXMatrixIdentity(&g_Player.modelParts[nCntParts].mtxWorld);

            // �����𔽉f
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_aPlayer[nCntParts].rot.y,
                g_aPlayer[nCntParts].rot.x,
                g_aPlayer[nCntParts].rot.z);

            D3DXMatrixMultiply(
                &g_Player.modelParts[nCntParts].mtxWorld,
                &g_Player.modelParts[nCntParts].mtxWorld, &mtxRot);

            // �ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans,
                g_aPlayer[nCntParts].pos.x,
                g_aPlayer[nCntParts].pos.y,
                g_aPlayer[nCntParts].pos.z);
            D3DXMatrixMultiply(
                &g_Player.modelParts[nCntParts].mtxWorld,
                &g_Player.modelParts[nCntParts].mtxWorld, &mtxTrans);

            // �e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, NULL);

            // ���݂̃}�e���A�����擾����
            pDevice->GetMaterial(&matDef);

            // �}�e���A���f�[�^�ւ̃|�C���^���擾
            pMat = (D3DXMATERIAL*)g_Player.modelParts[nCntParts].pBuffMat->GetBufferPointer();

            // �e���f���̃C���f�b�N�X�� - 1�̎�
            if (g_Player.modelParts[nCntParts].nldxModelParent == -1)
            {
                mtxParent = g_Player.mtxWorld;		// �v���C���[�̃}�g���N�X
            }
            else
            {
                mtxParent = g_Player.modelParts[g_Player.modelParts[nCntParts].nldxModelParent].mtxWorld;
            }
            // �e���f���̃}�g���b�N�X�̔���
            D3DXMatrixMultiply(
                &g_Player.modelParts[nCntParts].mtxWorld,
                &g_Player.modelParts[nCntParts].mtxWorld, &mtxParent);

            // ���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_Player.modelParts[nCntParts].mtxWorld);

            for (int nCntMat = 0; nCntMat < (int)g_Player.modelParts[nCntParts].nNumMat; nCntMat++)
            {
                // �}�e���A���̐ݒ�
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                g_Player.modelParts[nCntParts].pMesh->DrawSubset(nCntMat);
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
PLAYER*GetPlayer(void)
{
    return &g_Player;
}

//===========================================================================================================================
// �v���C���[�̃_���[�W�ݒ�
// Author:�㓡�T�V��
//===========================================================================================================================
void HitPlayer(float fDamage)
{
    //���G���ԂłȂ��Ȃ�
    if (g_Player.nCntInvincibleFrame == 0)
    {
        GAME *pGame = GetGame();

        //�Q�[�����I�������A�_���[�W���󂯂Ȃ�
        if (pGame->state == GAMESTATE_NORMAL)
        {
            //�_���[�W�v�Z
            g_Player.fLife -= fDamage;

            //���G���Ԃ�����
            g_Player.nCntInvincibleFrame = PLAYER_INVINCIBLE_FRAME;

            //HP������Ȃ�
            if (g_Player.fLife > 0.0f)
            {
                PlaySound(SOUND_LABEL_SE_TAKE_DAMAGE_VOICE);    //�_���[�W���󂯂����̐�
            }
        }
    }

}

//=====================================================
// �v���C���[�ɑ΂���Փ˔���̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool CollisionPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    //�Փ˂������ǂ���
    bool bCollision = false;

    // �v���C���[���g���Ă���Ȃ�
    if (g_Player.bDisp == true)
    {
        //�Փ˔�����v�Z
        if (BoxCollision(pPos, pPosOld, size, &g_Player.pos,
            D3DXVECTOR3(GRID_SIZE - (GRID_SIZE / 2.0f + g_Player.fSpeed), PLAYER_HEIGHT * 2, GRID_SIZE - (GRID_SIZE / 2.0f + g_Player.fSpeed))) == true)
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
void LoadPlayerAnimation(void)
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
    pFile = fopen("TXT/motion.txt", "r");

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
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayerMotion[nCntMotion].nLoop);
                        }

                        if (strcmp(cHeadText, "NUM_KEY") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayerMotion[nCntMotion].nNum_Key);
                        }

                        //			if (strcmp(cHeadText, "COLLISION") == 0)
                        //			{
                        //				sscanf(cReedText, "%s %s %d %f %f %f %f %d %d", &cDie, &cDie, &g_PlayerCollision.nPartsInfo, &g_PlayerCollision.CollisionPos.x, &g_PlayerCollision.CollisionPos.y, &g_PlayerCollision.CollisionPos.z, &g_PlayerCollision.fLength, &g_PlayerCollision.nFirstFlame[nCntMotion], &g_PlayerCollision.nLastFlame);
                        //			}
                        if (strcmp(cHeadText, "KEYSET") == 0)
                        {

                            while (strcmp(cHeadText, "END_KEYSET") != 0)
                            {

                                fgets(cReedText, sizeof(cReedText), pFile);
                                sscanf(cReedText, "%s", &cHeadText);

                                if (strcmp(cHeadText, "FRAME") == 0)
                                {
                                    sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayerMotion[nCntMotion].KeyInfo[nCntKey].nFrame);
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
                                                &g_aPlayerMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].x,
                                                &g_aPlayerMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].y,
                                                &g_aPlayerMotion[nCntMotion].KeyInfo[nCntKey].pos[nCntLoad].z);
                                        }

                                        if (strcmp(cHeadText, "ROT") == 0)
                                        {
                                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                                &g_aPlayerMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].x,
                                                &g_aPlayerMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].y,
                                                &g_aPlayerMotion[nCntMotion].KeyInfo[nCntKey].rot[nCntLoad].z);
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
void LoadPlayer(void)
{
    // �t�@�C���|�C���g
    FILE *pFile;

    // �ϐ��錾
    int  nCntLoad = 0;
    char cReedText[128];	// �����Ƃ��ēǂݎ��p
    char cHeadText[256];	//
    char cDie[128];

    // �t�@�C���J
    pFile = fopen("TXT/player.txt", "r");

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
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer[nCntLoad].nData);
                        }

                        if (strcmp(cHeadText, "PARENT") == 0)
                        {
                            sscanf(cReedText, "%s %s %d", &cDie, &cDie, &g_aPlayer[nCntLoad].nParents);
                        }

                        if (strcmp(cHeadText, "POS") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                &g_aPlayer[nCntLoad].posOrigin.x,
                                &g_aPlayer[nCntLoad].posOrigin.y,
                                &g_aPlayer[nCntLoad].posOrigin.z);

                            g_aPlayer[nCntLoad].pos = g_aPlayer[nCntLoad].posOrigin;
                        }

                        if (strcmp(cHeadText, "ROT") == 0)
                        {
                            sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
                                &g_aPlayer[nCntLoad].rot.x,
                                &g_aPlayer[nCntLoad].rot.y,
                                &g_aPlayer[nCntLoad].rot.z);
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
void UpdatePlayerMotion(void)
{
    //���[�V�������ς�����Ƃ�
    if (g_MotionOldState != g_MotionState) {
        g_nFremePlayer = 0;
        g_nCntKeyPlayer = 0;
    }
    for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
    {
        if (g_nFremePlayer == 0)
        {
            g_aNumPlayerPos[nCntMotion].x = ((g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].pos[nCntMotion].x -
                (g_aPlayer[nCntMotion].pos.x - g_aPlayer[nCntMotion].posOrigin.x)) / float(g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].nFrame));

            g_aNumPlayerPos[nCntMotion].y = ((g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].pos[nCntMotion].y -
                (g_aPlayer[nCntMotion].pos.y - g_aPlayer[nCntMotion].posOrigin.y)) / float(g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].nFrame));

            g_aNumPlayerPos[nCntMotion].z = ((g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].pos[nCntMotion].z -
                (g_aPlayer[nCntMotion].pos.z - g_aPlayer[nCntMotion].posOrigin.z)) / float(g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].nFrame));

            g_aNumPlayerRotDest[nCntMotion] =
                g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].rot[nCntMotion] - g_aPlayer[nCntMotion].rot;

            g_aNumPlayerRot[nCntMotion] = (g_aNumPlayerRotDest[nCntMotion] / float(g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].nFrame));
        }

        g_aPlayer[nCntMotion].pos += g_aNumPlayerPos[nCntMotion];
        g_aPlayer[nCntMotion].rot += g_aNumPlayerRot[nCntMotion];
    }
    g_MotionOldState = g_MotionState;
    // ���[�v����Ƃ�
    if (g_nFremePlayer >= g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].nFrame && g_aPlayerMotion[g_MotionState].nLoop == 1)
    {
        g_nFremePlayer = 0;
        g_nCntKeyPlayer++;

        if (g_nCntKeyPlayer >= g_aPlayerMotion[g_MotionState].nNum_Key)
        {
            g_nCntKeyPlayer = 0;
        }

    }
    // ���[�v���Ȃ��Ƃ�
    else if (g_nFremePlayer >= g_aPlayerMotion[g_MotionState].KeyInfo[g_nCntKeyPlayer].nFrame && g_aPlayerMotion[g_MotionState].nLoop == 0)
    {
        g_nFremePlayer = 0;
        g_nCntKeyPlayer++;
        if (g_nCntKeyPlayer >= g_aPlayerMotion[g_MotionState].nNum_Key)
        {
            for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
            {
                g_aNumPlayerPos[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aNumPlayerRot[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
            g_MotionState = MOTIONSTATE_PLAYER_IDLE;
            g_nCntKeyPlayer = 0;
        }
    }
    else
    {
        g_nFremePlayer++;
    }
}

//===========================================================================================================================
// �g�p����̏�����
// Author:�㓡�T�V��
//===========================================================================================================================
void InitDispPlayer(void)
{
    g_Player.bDisp = false;
}