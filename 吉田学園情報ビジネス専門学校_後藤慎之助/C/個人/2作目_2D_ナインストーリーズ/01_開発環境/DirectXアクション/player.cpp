//=============================================================================
//
// �i�C���X�g�[���[�Y [hunter.cpp]�̏��� (���@)
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "explosion.h"
#include "game.h"
#include "sound.h"
#include "attack.h"
#include "bg.h"
#include "magic.h"
#include "enemy.h"
#include "select.h"
#include "block.h"
#include "effect2.h"
#include "game2.h"
#include "game3.h"

//=============================================================================
//�}�N����`
//=============================================================================

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygon[ATTACKTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygon = NULL;
int g_nCounterAnimHunt;		//�A�j���[�V�����J�E���^�[
int g_nPatternAnimHunt;		//�A�j���[�V�����p�^�[��
int g_nAttackPatternAnim;	//�U���̃A�j���[�V�����p�^�[��
PLAYER g_Player;			//���@�̏��
int g_nSnowCD;				//��̃N�[���^�C��
int g_nDirect;				//���͂��ꂽ����
int g_nChange;				//������ς���
bool g_bBgX;				//�X�e�[�W1�̉����������ǂ���
bool g_bHitPlayer;			//�v���C���[���U�����󂯂����ǂ���
bool g_bEndPlayer;			//�v���C���[���w�i�֔��ōs�������ǂ���
MODE g_ModePlayer;			//�Q�[���̃��[�h���擾

//=============================================================================
//�v���C���[����������
//=============================================================================
void InitPlayer(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_nCounterAnimHunt = 0;
	g_nPatternAnimHunt = 0;
	g_nAttackPatternAnim = 0;

	//�v���C���[�̍\���̂̏�����
	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.posOld = D3DXVECTOR3(SCREEN_WIDTH / 4, 590, 0.0f);
	g_Player.pos = D3DXVECTOR3(SCREEN_WIDTH / 4, 590, 0.0f);
	g_Player.nLife = PLAYER_LIFE;
	g_Player.nMp = PLAYER_MP;
	g_Player.nCounterState = 0;
	g_Player.bLeft = false;
	g_Player.bDisp = false;
	g_Player.nStock = 0;	//�c�@�̑����̏�����
	g_Player.Wtype = GetWSelect();
	g_Player.Mtype = GetMSelect();
	g_Player.nAttackTime = 0;
	//�O���[�o���ϐ��̏�����
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nSnowCD = SNOW_CD;
	g_Player.bSecondJump = false;
	g_nDirect = 0;
	g_nChange = 0;
	g_Player.bJump = false;
	g_bBgX = false;
	g_bHitPlayer = false;
	g_bEndPlayer = false;
	g_ModePlayer = MODE_GAME;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/player000.png", &g_pTexturePolygon[0]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/player001.png", &g_pTexturePolygon[1]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/player003.png", &g_pTexturePolygon[2]);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygon, NULL);
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

	//���_���W�̐ݒ�i�E���Őݒ肷��j
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - (PLAYER_SIZE / 2), g_Player.pos.y - (PLAYER_SIZE / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + (PLAYER_SIZE / 2), g_Player.pos.y - (PLAYER_SIZE / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - (PLAYER_SIZE / 2), g_Player.pos.y + (PLAYER_SIZE / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + (PLAYER_SIZE / 2), g_Player.pos.y + (PLAYER_SIZE / 2), 0.0f);

	//rhw�̐ݒ�i�l��1.0�Őݒ肷��j
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.0f);
	pVtx[1 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.0f);
	pVtx[2 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.5f);
	pVtx[3 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.5f);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// �v���C���[�I������
//=============================================================================
void UninitPlayer(void)
{
	//�e�N�X�`���̊J��
	for (int nCntPlayer = 0; nCntPlayer < ATTACKTYPE_MAX; nCntPlayer++)
	{
		if (g_pTexturePolygon[nCntPlayer] != NULL)
		{
			g_pTexturePolygon[nCntPlayer]->Release();
			g_pTexturePolygon[nCntPlayer] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//=============================================================================
// �v���C���[�X�V����
//=============================================================================
void UpdatePlayer(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	ENEMY*pEnemy;
	int nCntEnemy;

	//�Q�[���̃X�e�[�W���擾
	g_ModePlayer = GetMode();

	int nAttack = 0;		//�U�����Ă��邩�ǂ���

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//�v���C���[�̏��
	switch (g_Player.state)
	{
	case PLAYERSTATE_NORMAL:	//�ʏ�̏��
		break;

	case PLAYERSTATE_DAMAGE:	//�_���[�W���
		g_Player.nCounterState--;
		
		g_bHitPlayer = true;

		if (g_Player.nCounterState <= 0)
		{
			g_Player.state = PLAYERSTATE_NORMAL;

			g_bHitPlayer = false;

			//�ʏ�`��ɖ߂�����
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		break;

	case PLAYERSTATE_APPEAR:	//�o���܂ł̏��
		g_Player.nCounterState--;
		g_Player.bDisp = false;	//�o���܂ŁA�\�������Ȃ�
		if (g_Player.nCounterState <= 0)
		{
			PlaySound(SOUND_LABEL_SE_ITEM);	//������

			g_Player.state = PLAYERSTATE_NORMAL;

			//�ʏ�`��ɖ߂�����
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�e�N�X�`�����W���X�V
			pVtx[0 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.0f);
			pVtx[1 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.0f);
			pVtx[2 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.5f);
			pVtx[3 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.5f);

			g_Player.bDisp = true;	//�\��������
		}
		break;
	}

	//�v���C���[���\������Ă���Ȃ�X�V
	if (g_Player.bDisp == true)
	{
		//�G���擾
		pEnemy = GetEnemy();

		//�w�i�̉����������ǂ����擾
		g_bBgX = GetBg();

		//�O�̈ʒu��ۑ�
		g_Player.posOld = g_Player.pos;

		//A��D���������͖���
		if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_D))
		{

		}

		//D�L�[�������ꂽ�i�E�ړ��j
		else if (g_Player.nAttackTime == 0&&GetKeyboardPress(DIK_D))
		{
			g_nDirect++;

			//��ʂ̒[�ɍs�����瓮����
			if (g_bBgX == false)
			{
				g_Player.pos.x += VALUE_MOVE;
			}
		}

		//A�L�[�������ꂽ�i���ړ��j
		else if (g_Player.nAttackTime == 0 && GetKeyboardPress(DIK_A))
		{
			g_nDirect--;
			
			//��ʂ̒[�ɍs�����瓮����
			if (g_bBgX == false)
			{
				g_Player.pos.x -= VALUE_MOVE;
			}
		}

		//��i�ڂ̃W�����v����
		if (GetKeyboardTrigger(DIK_SPACE)&& g_Player.bJump==false)
		{
			g_Player.move.y = 0;
			g_Player.move.y -= VALUE_JUMP;	//�W�����v
			SetEffect2(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 20, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 110, EFFECTTYPE_003);
			PlaySound(SOUND_LABEL_SE_JUMP);	//�W�����v��
			g_Player.bJump = true;	//��i�ڂ̃W�����v�����Ă���
			g_Player.bSecondJump = true;	//��i�W�����v���\�ɂ���
		}

		//��i�W�����v
		else if (g_Player.bSecondJump == true)
		{
			//�W�����v����
			if (GetKeyboardTrigger(DIK_SPACE))
			{
				g_Player.move.y = 0;	//��xmove��0�ɂ��āA��i�ڂ̍����𒲐�
				g_Player.move.y -= VALUE_JUMP2;	//��i�ڂ̃W�����v
				SetEffect2(g_Player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_001);
				PlaySound(SOUND_LABEL_SE_JUMP);	//�W�����v��
				g_Player.bSecondJump = false;	//��i�W�����v���I��点��
			}
		}

		//�X���b�V���L�[�ōU��
		if (g_Player.state == PLAYERSTATE_NORMAL && g_Player.nAttackTime == 0)	//�ʏ펞���U���̃N�[���^�C�����Ȃ��Ȃ����Ƃ�
		{
			//��U��
			if (GetKeyboardPress(DIK_W) && GetKeyboardTrigger(DIK_SLASH))
			{
				g_nAttackPatternAnim = 2;
				nAttack++;

				//��
				if (g_Player.Wtype == WSELECT_SWORD)
				{
					if (g_Player.bLeft == false)	//�E����
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x + 25.5f, g_Player.pos.y - 15.5, 0.0f), D3DXVECTOR3(0, -10, 0), 19, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_SWORD, 56, DIRECT_UP);
						PlaySound(SOUND_LABEL_SE_SWORD);	//���̉�
					}
					if (g_Player.bLeft == true)	//������
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x - 25.5f, g_Player.pos.y - 15.5, 0.0f), D3DXVECTOR3(0, -10, 0), 19, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_SWORD, 56, DIRECT_UP);
						PlaySound(SOUND_LABEL_SE_SWORD);	//���̉�
					}
				}

				//��
				if (g_Player.Wtype == WSELECT_LANCE)
				{
					if (g_Player.bLeft == false)	//�E����
					{
						//�n��ł͈�x�㏸����
						if (g_Player.bJump == false)
						{
							g_Player.move.y = 0;	//��xmove��0�ɂ��āA��i�ڂ̍����𒲐�
							g_Player.move.y -= VALUE_LANCEJUMP;	//�����X�̏�U���p�̃W�����v�̍���
							g_Player.bJump = true;	//�W�����v������
							g_Player.bSecondJump = true;	//��i�W�����v���\�ɂ���
							SetAttack(D3DXVECTOR3(g_Player.pos.x + 17.5f, g_Player.pos.y - 15.5, 0.0f), D3DXVECTOR3(0, -17.5, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE, 50, DIRECT_UP);
						}
						//��
						else
						{
							SetAttack(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y - 5.5, 0.0f), D3DXVECTOR3(0, -12, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE, 46, DIRECT_UP);
						}
						PlaySound(SOUND_LABEL_SE_SWORD);	//���̉�
					}
					if (g_Player.bLeft == true)	//������
					{
						//�n��ł͈�x�㏸����
						if (g_Player.bJump == false)
						{
							g_Player.move.y = 0;	//��xmove��0�ɂ��āA��i�ڂ̍����𒲐�
							g_Player.move.y -= VALUE_LANCEJUMP;//�����X�̏�U���p�̃W�����v�̍���
							g_Player.bJump = true;	//�W�����v������
							g_Player.bSecondJump = true;	//��i�W�����v���\�ɂ���
							SetAttack(D3DXVECTOR3(g_Player.pos.x - 17.5f, g_Player.pos.y - 15.5, 0.0f), D3DXVECTOR3(0, -17.5, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE, 50, DIRECT_UP);
						}
						//��
						else
						{
							SetAttack(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y - 5.5, 0.0f), D3DXVECTOR3(0, -10, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE, 46, DIRECT_UP);
						}
						PlaySound(SOUND_LABEL_SE_SWORD);	//���̉�
					}
				}

				//�|
				if (g_Player.Wtype == WSELECT_BOW)
				{
					if (g_Player.bLeft == false)	//�E����
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0, -15.0, 0), 24, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_BOW, 58, DIRECT_UP);
						PlaySound(SOUND_LABEL_SE_SHOT);	//�|�̉�
					}
					if (g_Player.bLeft == true)	//������
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0, -15.0, 0), 24, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_BOW, 58, DIRECT_UP);
						PlaySound(SOUND_LABEL_SE_SHOT);	//�|�̉�
					}
				}
			}

			//���U��
			else if (GetKeyboardPress(DIK_S) && GetKeyboardTrigger(DIK_SLASH))
			{
				//��
				if (g_Player.Wtype == WSELECT_SWORD)
				{
					//�󒆂̂�
					if (g_Player.bJump == true)
					{
						g_nAttackPatternAnim = 3;
						nAttack++;

						if (g_Player.bLeft == false)	//�E����
						{
							g_Player.move.y = 0;	//��xmove��0�ɂ��āA��i�ڂ̍����𒲐�
							g_Player.move.y -= VALUE_SMALLJUMP;	//���U���̏��W�����v
							SetAttack(D3DXVECTOR3(g_Player.pos.x + 15.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0, 10, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_SWORD, 60, DIRECT_DOWN);
							PlaySound(SOUND_LABEL_SE_SWORD);	//���̉�
						}
						if (g_Player.bLeft == true)	//������
						{
							g_Player.move.y = 0;	//��xmove��0�ɂ��āA��i�ڂ̍����𒲐�
							g_Player.move.y -= VALUE_SMALLJUMP;	//���U���̏��W�����v
							SetAttack(D3DXVECTOR3(g_Player.pos.x - 15.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0, 10, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_SWORD, 60, DIRECT_DOWN);
							PlaySound(SOUND_LABEL_SE_SWORD);	//���̉�
						}
					}
				}

				//��
				if (g_Player.Wtype == WSELECT_LANCE)
				{
					//�󒆂̂�
					if (g_Player.bJump == true)
					{
						g_nAttackPatternAnim = 3;
						nAttack++;

						if (g_Player.bLeft == false)	//�E����
						{
							g_Player.move.y = 0;	//��xmove��0�ɂ��āA��i�ڂ̍����𒲐�
							g_Player.move.y += GRAVITY*3.5f;	//���U���̋}�~��
							SetAttack(D3DXVECTOR3(g_Player.pos.x + 1.0f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0, 20, 0), 23, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE2, 55, DIRECT_DOWN);
							PlaySound(SOUND_LABEL_SE_SWORD);	//���̉�
						}
						if (g_Player.bLeft == true)	//������
						{
							g_Player.move.y = 0;	//��xmove��0�ɂ��āA��i�ڂ̍����𒲐�
							g_Player.move.y += GRAVITY * 3.5f;	//���U���̋}�~��
							SetAttack(D3DXVECTOR3(g_Player.pos.x - 1.0f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0, 20, 0), 23, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE2, 55, DIRECT_DOWN);
							PlaySound(SOUND_LABEL_SE_SWORD);	//���̉�
						}
					}
				}

				//�|
				if (g_Player.Wtype == WSELECT_BOW)
				{
					//�󒆂̂�
					if (g_Player.bJump == true)
					{
						g_nAttackPatternAnim = 3;
						nAttack++;

						if (g_Player.bLeft == false)	//�E����
						{
							g_Player.move.y = 0;	//��xmove��0�ɂ��āA��i�ڂ̍����𒲐�
							g_Player.move.y -= VALUE_SMALLJUMP;	//���U���̏��W�����v
							SetAttack(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(11.0, 11.0, 0), 24, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_BOW, 58, DIRECT_DOWN);
							PlaySound(SOUND_LABEL_SE_SHOT);	//�|�̉�
						}
						if (g_Player.bLeft == true)	//������
						{
							g_Player.move.y = 0;	//��xmove��0�ɂ��āA��i�ڂ̍����𒲐�
							g_Player.move.y -= VALUE_SMALLJUMP;	//���U���̏��W�����v
							SetAttack(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(-11.0, 11.0, 0), 24, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_BOW2, 58, DIRECT_DOWN);
							PlaySound(SOUND_LABEL_SE_SHOT);	//�|�̉�
						}
					}
				}
			}

			//���E�U��
			else if (GetKeyboardTrigger(DIK_SLASH))
			{
				g_nAttackPatternAnim = 0;
				nAttack++;

				//��
				if (g_Player.Wtype == WSELECT_SWORD)
				{
					if (g_Player.bLeft == false)	//�E�U��
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x + 25.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(10.0, 0, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_SWORD, 60, DIRECT_RIGHT);
						PlaySound(SOUND_LABEL_SE_SWORD);	//���̉�
					}
					if (g_Player.bLeft == true)	//���U��
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x - 25.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(-10.0, 0, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_SWORD, 60, DIRECT_LEFT);
						PlaySound(SOUND_LABEL_SE_SWORD);	//���̉�
					}
				}

				//��
				if (g_Player.Wtype == WSELECT_LANCE)
				{
					if (g_Player.bLeft == false)	//�E�U��
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y + 15.5f, 0.0f), D3DXVECTOR3(13.5, 0, 0), 21, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE, 60, DIRECT_RIGHT);
						PlaySound(SOUND_LABEL_SE_SWORD);	//���̉�
					}
					if (g_Player.bLeft == true)	//���U��
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y + 15.5f, 0.0f), D3DXVECTOR3(-13.5, 0, 0), 21, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE, 60, DIRECT_LEFT);
						PlaySound(SOUND_LABEL_SE_SWORD);	//���̉�
					}
				}

				//�|
				if (g_Player.Wtype == WSELECT_BOW)
				{
					if (g_Player.bLeft == false)	//�E�U��
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(15.0, 0, 0), 24, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_BOW, 60, DIRECT_RIGHT);
						PlaySound(SOUND_LABEL_SE_SHOT);	//�|�̉�
					}
					if (g_Player.bLeft == true)	//���U��
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(-15.0, 0, 0), 24, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_BOW, 60, DIRECT_LEFT);
						PlaySound(SOUND_LABEL_SE_SHOT);	//�|�̉�
					}
				}
			}

			//���@�U��
			else if (GetKeyboardTrigger(DIK_RETURN))
			{
				//��
				if (g_Player.Mtype == MSELECT_FIRE)
				{
					if (g_Player.nMp >= FIRE_MP)	//MP����25
					{
						//�U�����̃A�j���[�V����
						g_nAttackPatternAnim = 4;
						nAttack++;

						g_Player.nMp -= FIRE_MP;
						if (g_Player.bLeft == false)	//�E�U��
						{
							SetMagic(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(12.0, -15.0, 0), 32, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_FIRE, 60);
							SetMagic(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(12.0, -16.5, 0), 32, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_FIRE, 60);
							SetMagic(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(12.0, -18.0, 0), 32, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_FIRE, 60);
						}
						if (g_Player.bLeft == true)	//���U��
						{
							SetMagic(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(-12.0, -15.0, 0), 32, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_FIRE, 60);
							SetMagic(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(-12.0, -16.5, 0), 32, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_FIRE, 60);
							SetMagic(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(-12.0, -18.0, 0), 32, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_FIRE, 60);
						}
						PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��
					}
				}

				//��
				if (g_Player.Mtype == MSELECT_LIGHTNING)
				{
					if (g_Player.nMp >= LIGHTNING_MP)	//MP����40
					{
						//�U�����̃A�j���[�V����
						g_nAttackPatternAnim = 4;
						nAttack++;

						g_Player.nMp -= LIGHTNING_MP;
						if (g_Player.bLeft == false)	//�E�U��
						{
							SetMagic(D3DXVECTOR3(g_Player.pos.x + 285.5f, 0, 0.0f), D3DXVECTOR3(0.0, 19.5, 0), 37, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_LIGHTNING, 70);
						}
						if (g_Player.bLeft == true)	//���U��
						{
							SetMagic(D3DXVECTOR3(g_Player.pos.x - 285.5f, 0, 0.0f), D3DXVECTOR3(0.0, 19.5, 0), 37, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_LIGHTNING, 70);
						}
						PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��
					}
				}

				//�z��
				if (g_Player.Mtype == MSELECT_DRAIN)
				{
					if (g_Player.nMp >= DRAIN_MP)	//MP����
					{
						//�U�����̃A�j���[�V����
						g_nAttackPatternAnim = 4;
						nAttack++;

						g_Player.nMp -= DRAIN_MP;
						if (g_Player.bLeft == false)	//�E�U��
						{
							SetMagic(D3DXVECTOR3(g_Player.pos.x + 225.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0.0, 0.0, 0), 37, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_DRAIN, 70);
						}
						if (g_Player.bLeft == true)	//���U��
						{
							SetMagic(D3DXVECTOR3(g_Player.pos.x - 225.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0.0, 0.0, 0), 37, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_DRAIN, 70);
						}
						PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾��
					}
				}
			}
		}

		//�v���C���[�ւ̏d��
		g_Player.move.y += GRAVITY;	//�d��

		//�ʒu�X�V
		if (g_ModePlayer == MODE_GAME3)	//�Q�[��3�ł����g�p
		{
			g_Player.pos.x += g_Player.move.x;	//���p
		}
		g_Player.pos.y += g_Player.move.y;

		//�Q�[��1�ƃQ�[��3�̂Ƃ��̍��E�̕�
		if (g_ModePlayer == MODE_GAME || g_ModePlayer == MODE_GAME3)
		{
			//���̕�
			if (g_Player.pos.x < PLAYER_SIZE - (PLAYER_SIZE / 2))
			{
				g_Player.pos.x = PLAYER_SIZE - (PLAYER_SIZE / 2);
			}

			//�E�̕�
			if (g_Player.pos.x > SCREEN_WIDTH - PLAYER_SIZE + (PLAYER_SIZE / 2))
			{
				g_Player.pos.x = SCREEN_WIDTH - PLAYER_SIZE + (PLAYER_SIZE / 2);
			}
		}

		//�Q�[��2�̂Ƃ��̍��E�̕�
		if (g_ModePlayer == MODE_GAME2)
		{
			//���̕�
			if (g_Player.pos.x < PLAYER_SIZE - (PLAYER_SIZE / 2) + 200)
			{
				g_Player.pos.x = PLAYER_SIZE - (PLAYER_SIZE / 2) + 200;
			}

			//�E�̕�
			if (g_Player.pos.x > SCREEN_WIDTH - PLAYER_SIZE + (PLAYER_SIZE / 2) -200)
			{
				g_Player.pos.x = SCREEN_WIDTH - PLAYER_SIZE + (PLAYER_SIZE / 2) -200;
			}
		}

		//�n�ʂɂ���Ƃ�
		if (g_Player.pos.y > SCREEN_HEIGHT - PLAYER_SIZE + (PLAYER_SIZE / 2) - 80)
		{
			//���n�̉�
			if (g_Player.bJump == true)
			{
				SetEffect2(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 20, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 110, EFFECTTYPE_003);
			}

			g_Player.pos.y = SCREEN_HEIGHT - PLAYER_SIZE + (PLAYER_SIZE / 2) - 80;

			g_Player.move.y = 0;	//�n�ʂɂ���Ƃ��͏d�͂�^���Ȃ�

								//�W�����v�𕜊�
			g_Player.bJump = false;

			g_Player.bSecondJump = false;	//��i�W�����v���I��点��

			//A��D���������͖���
			if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_D))
			{
				g_nPatternAnimHunt = 0;
			}

			//A�L�[�܂���D�L�[�������ꂽ�i���E�ړ��j
			else if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
			{
				//�A�j���[�V����
				g_nCounterAnimHunt++;	//�J�E���^���Z
				if (g_nCounterAnimHunt == 9)	//fps
				{
					g_nCounterAnimHunt = 0;
					g_nPatternAnimHunt++;
					if (g_nPatternAnimHunt == 4)//����
					{
						g_nPatternAnimHunt = 0;
					}
				}

				//�Q�[��1(��̕���)
				if (g_ModePlayer == MODE_GAME)
				{
					if (g_nSnowCD % SNOW_CD == 0 && g_Player.nAttackTime == 0)
					{
						PlaySound(SOUND_LABEL_SE_SNOW);	//��̉�
					}

					g_nSnowCD++;	//���������Ԋu�Ŗ炷���߂ɉ��Z

					if (g_nSnowCD >= SNOW_CD * 1000)//�I�[�o�[�t���[�h�~
					{
						g_nSnowCD = 0;
					}
				}
			}

			//�n�ʂ̏�ŗ����~�܂��Ă���Ƃ�
			else
			{
				g_nPatternAnimHunt = 0;
			}

			//�e�N�X�`�����W���X�V
			pVtx[0 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.0f);
			pVtx[1 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.0f);
			pVtx[2 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.5f);
			pVtx[3 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.5f);
		}
		//�󒆂ɂ���Ƃ�
		else
		{
			g_Player.bJump = true;		//�u���b�N����~�肽���A2�i�W�����v�����Ȃ�������

			//�e�N�X�`�����W���X�V
			pVtx[0 + g_nChange].tex = D3DXVECTOR2(0.22f, 0.5f);
			pVtx[1 - g_nChange].tex = D3DXVECTOR2(0.4f, 0.5f);
			pVtx[2 + g_nChange].tex = D3DXVECTOR2(0.22f, 1.0f);
			pVtx[3 - g_nChange].tex = D3DXVECTOR2(0.4f, 1.0f);
		}

		// �u���b�N�̏�ɂ���Ƃ�
		if (CollisionBlock(&g_Player.pos, &g_Player.posOld, PLAYER_SIZE) == true)
		{
			g_Player.move.y = 0;
			g_Player.bJump = false;		//�W�����v�𕜊�
			g_Player.bSecondJump = true;		//��i�W�����v�𕜊�

			//���̏�ɂ���Ƃ�
			if (g_ModePlayer == MODE_GAME3)
			{
				//���E�L�[�������Ɗ���
				if (GetKeyboardPress(DIK_A))//������
				{
					g_Player.move.x -= 0.25;
				}

				//���E�L�[�������Ɗ���
				if (GetKeyboardPress(DIK_D))//�E����
				{
					g_Player.move.x += 0.25;
				}

				//������
				if (g_Player.move.x > 2.5)
				{
					g_Player.move.x = 2.5;
				}

				//���鉺��
				if (g_Player.move.x < -2.5)
				{
					g_Player.move.x = -2.5;
				}
			}

			//A��D���������͖���
			if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_D))
			{
				g_nPatternAnimHunt = 0;
			}

			//A�L�[�܂���D�L�[�������ꂽ�i���E�ړ��j
			else if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
			{
				//�A�j���[�V����
				g_nCounterAnimHunt++;	//�J�E���^���Z
				if (g_nCounterAnimHunt == 9)	//fps
				{
					g_nCounterAnimHunt = 0;
					g_nPatternAnimHunt++;
					if (g_nPatternAnimHunt == 4)//����
					{
						g_nPatternAnimHunt = 0;
					}
				}

				//if (g_nSnowCD % SNOW_CD == 0)
				//{
				//	PlaySound(SOUND_LABEL_SE_SNOW);	//��̉�
				//}
			}

			//�n�ʂ̏�ŗ����~�܂��Ă���Ƃ�
			else
			{
				g_nPatternAnimHunt = 0;
			}

			//�e�N�X�`�����W���X�V
			pVtx[0 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.0f);
			pVtx[1 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.0f);
			pVtx[2 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.5f);
			pVtx[3 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.5f);
		}
		else
		{
			//�Q�[��3
			if (g_ModePlayer == MODE_GAME3)
			{
				//���̏�ɂ��Ȃ��Ƃ��́A����Ȃ�
				g_Player.move.x = 0;
			}
		}

		//�������ɕς���
		if (g_nDirect < 0)
		{
			g_nDirect = 0;
			g_nChange = 1;
			g_Player.bLeft = true;
		}

		//�E�����ɕς���
		if (g_nDirect > 0)
		{
			g_nDirect = 0;
			g_nChange = 0;
			g_Player.bLeft = false;
		}

		//�U���̃A�j���[�V����
		if (nAttack > 0)
		{
			nAttack = 0;
			g_Player.nAttackTime = ATTACK_CD;	//���̍U�����ł���܂ł̎���
		}

		//���@���G�ɓ����������̔���
		for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{
				//���@���m�[�}����ԂȂ�
				if (g_Player.state == PLAYERSTATE_NORMAL)
				{

					//���@���r�[�g���ɓ����������̓����蔻��
					if (pEnemy->type == ENEMYTYPE_000)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MOS / 2) &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MOS / 2) &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MOS / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MOS / 2))
						{
							//���@�Ƀ_���[�W
							HitPlayer(7);
						}
					}

					//���@���J���X�ɓ����������̓����蔻��
					if (pEnemy->type == ENEMYTYPE_001)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET / 2) &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET / 2) &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET / 2))
						{
							//���@�Ƀ_���[�W
							HitPlayer(15);
						}
					}

					//���@���J���X����ɓ����������̓����蔻��
					if (pEnemy->type == ENEMYTYPE_002)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET�U / 2) &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET�U / 2) &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET�U / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET�U / 2))
						{
							//���@�Ƀ_���[�W
							HitPlayer(20);
						}
					}

					//���@���K�C�R�c�ɓ����������̓����蔻��
					if (pEnemy->type == ENEMYTYPE_003 || pEnemy->type == ENEMYTYPE_008)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_OCT / 2) &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_OCT / 2) &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_OCT / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_OCT / 2))
						{
							//���@�Ƀ_���[�W
							HitPlayer(17);
						}
					}

					//���@���C�m�V�V�ɓ����������̓����蔻��
					if (pEnemy->type == ENEMYTYPE_004)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GHO / 2) &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GHO / 2) &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GHO / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GHO / 2))
						{
							//���@�Ƀ_���[�W
							HitPlayer(22);
						}
					}

					//���@���R�E�����ɓ����������̓����蔻��
					if (pEnemy->type == ENEMYTYPE_005)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GSO / 2) &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GSO / 2) &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GSO / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GSO / 2))
						{
							//���@�Ƀ_���[�W
							HitPlayer(5);
						}
					}

					//���@���A�C�X�h���S���ɓ����������̓����蔻��
					if (pEnemy->type == ENEMYTYPE_006)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_BOSS_X / 2) - 70 &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_BOSS_X / 2) + 70 &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_BOSS_Y / 2) + 80)
						{
							//�������Ȃ獶�ɔ�΂�
							if (pEnemy->bLeft == true)
							{
								g_Player.pos.x -= 50.0f;
							}
							//�E�����Ȃ�E�ɔ�΂�
							if (pEnemy->bLeft == false)
							{
								g_Player.pos.x += 50.0f;
							}
							//���@�Ƀ_���[�W
							HitPlayer(25);
						}
					}

					//���@���s���j�A�ɓ����������̓����蔻��
					if (pEnemy->type == ENEMYTYPE_007)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_PIR / 2) &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_PIR / 2) &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_PIR / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_PIR / 2))
						{
							//���@�Ƀ_���[�W
							HitPlayer(21);
						}
					}
				}
			}
		}

		//�e�N�X�`�����U���p�ɍX�V
		if (g_Player.nAttackTime > 0)
		{
			g_Player.nAttackTime--;
			pVtx[0 + g_nChange].tex = D3DXVECTOR2(g_nAttackPatternAnim * 0.2f, 0.5f);
			pVtx[1 - g_nChange].tex = D3DXVECTOR2(g_nAttackPatternAnim * 0.2f + 0.2f, 0.5f);
			pVtx[2 + g_nChange].tex = D3DXVECTOR2(g_nAttackPatternAnim * 0.2f, 1.0f);
			pVtx[3 - g_nChange].tex = D3DXVECTOR2(g_nAttackPatternAnim * 0.2f + 0.2f, 1.0f);
		}

		//�e�N�X�`�����_���[�W��ԂɍX�V
		if (g_Player.nCounterState > 0)
		{
			//�e�N�X�`�����W���X�V
			pVtx[0 + g_nChange].tex = D3DXVECTOR2(0.8f, 0.0f);
			pVtx[1 - g_nChange].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2 + g_nChange].tex = D3DXVECTOR2(0.8f, 0.5f);
			pVtx[3 - g_nChange].tex = D3DXVECTOR2(1.0f, 0.5f);
		}

		//���̒�
		if (g_ModePlayer == MODE_GAME3)
		{
			//��̉�ʊO�ɂ�������Q�[���I�[�o�[
			if (g_Player.pos.y < -325)
			{
				//���x���G�t�F�N�g���łȂ��悤�ɂ���
				if (g_bEndPlayer == false)
				{
					g_Player.bDisp = false;
					SetEffect2(D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/4, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_HEIGHT / 2, EFFECTTYPE_004);
					g_bEndPlayer = true;
				}
			}
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - (PLAYER_SIZE / 2), g_Player.pos.y - (PLAYER_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + (PLAYER_SIZE / 2), g_Player.pos.y - (PLAYER_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - (PLAYER_SIZE / 2), g_Player.pos.y + (PLAYER_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + (PLAYER_SIZE / 2), g_Player.pos.y + (PLAYER_SIZE / 2), 0.0f);
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// �v���C���[�`�揈��
//=============================================================================
void DrawPlayer(void)
{
	//�錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�v���C���[���\������Ă���Ȃ�`��
	if (g_Player.bDisp == true)
	{
		if (g_Player.Wtype == WSELECT_SWORD)	//��
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexturePolygon[0]);
		}

		if (g_Player.Wtype == WSELECT_LANCE)	//��
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexturePolygon[1]);
		}

		if (g_Player.Wtype == WSELECT_BOW)		//�|
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexturePolygon[2]);
		}

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// �v���C���[�̐ݒ菈��
//=============================================================================
void SetPlayer(D3DXVECTOR3 pos)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//�v���C���[���\������Ă��Ȃ����A�X�g�b�N�������Ԃ̂Ƃ��ɐݒ�\
	if (g_Player.bDisp == false && g_Player.nStock >-1)
	{
		g_Player.bDisp = true;
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - (PLAYER_SIZE / 2), g_Player.pos.y - (PLAYER_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + (PLAYER_SIZE / 2), g_Player.pos.y - (PLAYER_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - (PLAYER_SIZE / 2), g_Player.pos.y + (PLAYER_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + (PLAYER_SIZE / 2), g_Player.pos.y + (PLAYER_SIZE / 2), 0.0f);

		//�ʒu��ݒ�
		g_Player.pos = pos;
		g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�v���C���[�̗̑�
		g_Player.nLife = PLAYER_LIFE;
		g_Player.nMp = PLAYER_MP;
	}

	//�H��������Ƃ��̃J���[��ʏ�̐F�ɖ߂�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// �v���C���[�̎擾
//=============================================================================
PLAYER*GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// �v���C���[�̃_���[�W�ݒ�
//=============================================================================
void HitPlayer(int nDamage)
{
	//�錾
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//�w�i�֋z�����܂ꂽ�Ƃ��́A����[��
	if (nDamage == 1000)
	{
		PlaySound(SOUND_LABEL_SE_STAR);	//����-��
	}
	else
	{
		PlaySound(SOUND_LABEL_SE_EXPLOSION);	//������
	}

											//���_�o�b�t�@�����b�N
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	g_Player.nLife -= nDamage;
	if (g_Player.nLife <= 0)
	{
		//�w�i�֋z�����܂ꂽ�Ƃ��́A�����������Ȃ�
		if (nDamage == 1000)
		{

		}
		else
		{
			//��������
			SetExplosion(g_Player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE);
		}

		//�v���C���[�𖢎g�p��
		g_Player.bDisp = false;

		//���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�ݒu��ɁA���@�̑��������炷
		g_Player.nStock--;
	}

	else
	{
		//�_���[�W��ԂɑJ��
		g_Player.state = PLAYERSTATE_DAMAGE;
		g_Player.nCounterState = INVINCIBLE_FRAME;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
	}

	//���_�f�[�^���A�����b�N����
	g_pVtxBuffPolygon->Unlock();

	//�Q�[���I�[�o�[
	if (g_Player.nStock < 0)
	{
		g_Player.bDisp = false;

		//�Q�[��1(��̕���)
		if (g_ModePlayer == MODE_GAME)
		{
			SetGameState(GAMESTATE_GAMEOVER);
		}

		//�Q�[��2(������)
		if (g_ModePlayer == MODE_GAME2)
		{
			SetGameState2(GAMESTATE2_GAMEOVER);
		}

		//�Q�[��3(���̒�)
		if (g_ModePlayer == MODE_GAME3)
		{
			SetGameState3(GAMESTATE3_GAMEOVER);
		}
	}
}

//=============================================================================
// �v���C���[�̃_���[�W�ݒ�2
//=============================================================================
bool HitPlayer2(void)
{
	return g_bHitPlayer;
}
