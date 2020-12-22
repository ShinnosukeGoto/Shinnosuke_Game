//=============================================================================
//
// PLASTIC TROOPERS [ui.cpp]�̏��� (�Q�[����ʂ�UI)
// Author :�㓡�T�V��
//
//=============================================================================
#include "ui.h"
#include "player.h"
#include "camera.h"
#include "title.h"
#include "effect.h"
#include "boss.h"
//#include "title.h"
//#include "select.h"

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonUI[UITYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonUI = NULL;

UI g_aUI[MAX_UI];   //UI�̏��
bool g_bDecrease;   //�G�i�W�[�������Ă��邩�ǂ����̔���
bool g_bOutEnergy;   //�G�i�W�[�؂ꂵ�Ă��邩�ǂ���
int g_nCntHeal;     //�G�i�W�[�񕜂��n�܂�܂ł̃J�E���^�[

//=============================================================================
//UI����������
//=============================================================================
void InitUI(void)
{
    //�錾
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�O���[�o���ϐ��̏�����
    g_bDecrease = false;
    g_bOutEnergy = false;
    g_nCntHeal = 0;

    //g_nUIWtype = GetWSelect();		//����I�����擾
    //g_nUIMtype = GetMSelect();		//���@�I�����擾

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/score000.png", &g_pTexturePolygonUI[0]);	//�X�R�A�̔w�i
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/CAMPAIGN.png", &g_pTexturePolygonUI[1]);	//�L�����y�[��
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/SURVIVAL.png", &g_pTexturePolygonUI[2]);	//�T�o�C�o��
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/VERSUS.png", &g_pTexturePolygonUI[3]);		//�o�[�T�X
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/PRACTICE.png", &g_pTexturePolygonUI[4]);	//�v���N�e�B�X
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI005.png", &g_pTexturePolygonUI[5]);		//�K�E�Q�[�W�̔w�i
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar003.png", &g_pTexturePolygonUI[6]);		//HP/ENERGY�Q�[�W�̔w�i
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ONOFF.png", &g_pTexturePolygonUI[7]);		//ON/OFF
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/HP.png", &g_pTexturePolygonUI[8]);		    //HP�̕���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ENERGY.png", &g_pTexturePolygonUI[9]);	    //ENERGY�̕���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar000.png", &g_pTexturePolygonUI[10]);	    //HP�Q�[�W
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar000.png", &g_pTexturePolygonUI[11]); 	//ENERGY�Q�[�W
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI006.png", &g_pTexturePolygonUI[12]); 	    //�K�E�Q�[�W
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI007.png", &g_pTexturePolygonUI[13]); 	    //�K�E�̐���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/katana.png", &g_pTexturePolygonUI[14]); 	//��
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/zyu.png", &g_pTexturePolygonUI[15]); 	    //�e
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/y.png", &g_pTexturePolygonUI[16]); 	        //Y�{�^��
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/press_R.png", &g_pTexturePolygonUI[17]); 	//�v���XR
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI008.png", &g_pTexturePolygonUI[18]); 	    //�X�s�j���O�u���[�h
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI009.png", &g_pTexturePolygonUI[19]); 	    //�X�v���b�h�t�@�C�A
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar003.png", &g_pTexturePolygonUI[20]); 	//�{�X��HP�o�[�̔w�i
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar000.png", &g_pTexturePolygonUI[21]); 	//�{�X��HP�o�[
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/BOSS.png", &g_pTexturePolygonUI[22]); 	    //BOSS�̕���

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX * MAX_UI, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonUI, NULL);	//�X�R�A�A�^�C���A�o�[�A����A���@�A��Փx
    VERTEX_2D *pVtx;	//�o�b�N�O���E���h�̒��_���ւ̃|�C���^

                        //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffPolygonUI->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        g_aUI[nCntUI].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aUI[nCntUI].fSizeX = 0;
        g_aUI[nCntUI].fSizeY = 0;
        g_aUI[nCntUI].nCounterAnim = 0;
        g_aUI[nCntUI].nPatternAnim = 0;
        g_aUI[nCntUI].type = UITYPE_000;
        g_aUI[nCntUI].bUse = false;

        //���_���W�̐ݒ�i�E���Őݒ肷��j
        pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);

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
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffPolygonUI->Unlock();

}

//=============================================================================
// UI�I������
//=============================================================================
void UninitUI(void)
{
    //�e�N�X�`���̊J��
    for (int nCntUI = 0; nCntUI < UITYPE_MAX; nCntUI++)
    {
        if (g_pTexturePolygonUI[nCntUI] != NULL)
        {
            g_pTexturePolygonUI[nCntUI]->Release();
            g_pTexturePolygonUI[nCntUI] = NULL;
        }
    }

    //���_�o�b�t�@�̊J��
    if (g_pVtxBuffPolygonUI != NULL)
    {
        g_pVtxBuffPolygonUI->Release();
        g_pVtxBuffPolygonUI = NULL;
    }
}

//=============================================================================
// UI�X�V����
//=============================================================================
void UpdateUI(void)
{
    VERTEX_2D *pVtx;	//�o�b�N�O���E���h�̒��_���ւ̃|�C���^
    PLAYER*pPlayer = GetPlayer();	//���@���擾
    CAMERA*pCamera = GetCamera();   //�J�������擾
    BOSS*pBoss = GetBoss();         //�{�X���擾

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffPolygonUI->Lock(0, 0, (void**)&pVtx, 0);	//���̏����͕ς��Ȃ�����

    //�G�i�W�[�����������ǂ����̔���
    if (pPlayer->fOldEnergy > pPlayer->fEnergy)
    {
        g_bDecrease = true;
    }
    else
    {
        g_bDecrease = false;
    }

    //�G�i�W�[�񕜃J�E���^�|
    if (g_bDecrease == false && pPlayer->fEnergy != PLAYER_ENERGY)
    {
        g_nCntHeal++;

        //�G�i�W�[�؂ꒆ
        if (g_bOutEnergy == true)
        {
            //�G�i�W�[�؂�}�[�N
            EFFECT*pEffect = GetEffect();
            for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
            {
                //���g�p�Ȃ�
                if (pEffect->bUse == false)
                {
                        SetEffect(D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + PLAYER_HEIGHT * 2, pPlayer->pos.z), ENERGYOUT_SIZE
                            , D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECTTYPE_001);
                }
            }

            //�G�i�W�[�񕜊J�n
            if (g_nCntHeal >= START_HEAL_OUT_ENERGY)
            {
                g_nCntHeal = START_HEAL_OUT_ENERGY;
                pPlayer->fEnergy += VALUE_HEAL_ENERGY;   //�G�i�W�[��
            }
        }

        //�G�i�W�[�؂ꂵ�Ă��Ȃ�
        else
        {
            //�G�i�W�[�񕜊J�n
            if (g_nCntHeal >= START_HEAL_ENERGY)
            {
                g_nCntHeal = START_HEAL_ENERGY;
                pPlayer->fEnergy += VALUE_HEAL_ENERGY;   //�G�i�W�[��
            }
        }
    }

    //�G�i�W�[�������Ȃ�A�G�i�W�[�񕜃J�E���^�[�����Z�b�g
    else if (g_bDecrease == true)
    {
        g_nCntHeal = 0;
    }

    //HP��250�ȏ�ɂȂ�Ȃ��悤�ɂ���
    if (pPlayer->fLife > PLAYER_LIFE)
    {
        pPlayer->fLife = PLAYER_LIFE;
    }

    //�G�i�W�[��101�ȏ�ɂȂ�Ȃ��悤�ɂ���
    if (pPlayer->fEnergy > PLAYER_ENERGY)
    {
        pPlayer->fEnergy = PLAYER_ENERGY;
    }

    //�G�i�W�[��0�ȉ��ɂȂ�Ȃ��悤�ɂ���
    if (pPlayer->fEnergy <= 0.0f)
    {
        pPlayer->fEnergy = 0.0f;
        g_bOutEnergy = true;    //�G�i�W�[�؂ꔻ��
    }
    else
    {
        g_bOutEnergy = false;
    }

    //�K�E�Q�[�W�̍ő�
    if (pPlayer->fChanceMeter > PLAYER_CHANCE_METER_MAX)
    {
        //�`�����X�l�̍ő�
        if (pPlayer->nChance > PLAYER_CHANCE_MAX-2)
        {
            pPlayer->fChanceMeter = PLAYER_CHANCE_METER_MAX;    //���[�^�[�}�b�N�X
            pPlayer->nChance = PLAYER_CHANCE_MAX;               //�`�����X�l�}�b�N�X
        }

        //�`�����X�l���ő�łȂ��Ȃ�A�`�����X�l1�A�b�v
        else
        {
            pPlayer->fChanceMeter = 0.0f;   //���Z�b�g
            pPlayer->nChance += 1;          //�`�����X�l�A�b�v
        }
    }

    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        if (g_aUI[nCntUI].bUse == true)
        {
            //HP�Q�[�W
            if (g_aUI[nCntUI].type == UITYPE_010)
            {
                //���_���W�̐ݒ�i���Q�[�W�j
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fLife / PLAYER_LIFE)),
                    g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fLife / PLAYER_LIFE)),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //�K�E�g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pPlayer->bChanceAttack == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                else
                {
                    //HP�ɉ����ăJ���[��ς���i���^�����甼���܂ŗ΁j
                    if (pPlayer->fLife <= PLAYER_LIFE && pPlayer->fLife > PLAYER_LIFE / 2)
                    {
                        pVtx[0].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                    }

                    //HP�ɉ����ăJ���[��ς���i��������4����1�܂ŉ��F�j
                    else if (pPlayer->fLife <= PLAYER_LIFE / 2 && pPlayer->fLife > PLAYER_LIFE / 4)
                    {
                        pVtx[0].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                    }

                    //HP�ɉ����ăJ���[��ς���i4����1����͐ԐF�j
                    else if (pPlayer->fLife <= PLAYER_LIFE / 4)
                    {
                        pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                    }
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2((pPlayer->fLife / PLAYER_LIFE), 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2((pPlayer->fLife / PLAYER_LIFE), 1.0f);
            }

            //ENERGY�Q�[�W
            else if (g_aUI[nCntUI].type == UITYPE_011)
            {
                //���_���W�̐ݒ�i���Q�[�W�j
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fEnergy / PLAYER_ENERGY)),
                    g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fEnergy / PLAYER_ENERGY)),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //�K�E�g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pPlayer->bChanceAttack == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j(��)
                    pVtx[0].col = D3DCOLOR_RGBA(77, 224, 254, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(77, 224, 254, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(77, 224, 254, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(77, 224, 254, 255);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2((pPlayer->fEnergy/ PLAYER_ENERGY), 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2((pPlayer->fEnergy / PLAYER_ENERGY), 1.0f);
            }

            //�K�E�Q�[�W
            else if (g_aUI[nCntUI].type == UITYPE_012)
            {
                //���_���W�̐ݒ�i�c�Q�[�W�j
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2) - (g_aUI[nCntUI].fSizeY)* (pPlayer->fChanceMeter / PLAYER_CHANCE_METER_MAX), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2) - (g_aUI[nCntUI].fSizeY)* (pPlayer->fChanceMeter / PLAYER_CHANCE_METER_MAX), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2) , 0.0f);

                //�K�E�g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pPlayer->bChanceAttack == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j(��)
                    pVtx[0].col = D3DCOLOR_RGBA(255, 84, 59, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 84, 59, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 84, 59, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 84, 59, 255);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f - (pPlayer->fChanceMeter / PLAYER_CHANCE_METER_MAX));
                pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f - (pPlayer->fChanceMeter / PLAYER_CHANCE_METER_MAX));
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //�K�E�̐���
            else if (g_aUI[nCntUI].type == UITYPE_013)
            {
                if (pPlayer->nChance == 0)
                {
                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.0f + 0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.0f + 0.25f, 1.0f);
                }
                else if(pPlayer->nChance == 1)
                {
                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.25f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.25f + 0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.25f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f, 1.0f);
                }
                else if (pPlayer->nChance == 2)
                {
                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f + 0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f + 0.25f, 1.0f);
                }
                else if (pPlayer->nChance == 3)
                {
                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.75f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.75f + 0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.75f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.75f + 0.25f, 1.0f);
                }

                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //�K�E�g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pPlayer->bChanceAttack == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                
            }

            //��
            else if (g_aUI[nCntUI].type == UITYPE_014)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // �K�E�g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pPlayer->bChanceAttack == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //���g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                    if (pPlayer->bBullet == true)
                    {
                        //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                        pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                        pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                        pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                        pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                    }
                    else
                    {
                        //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                        pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    }
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //�e
            else if (g_aUI[nCntUI].type == UITYPE_015)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // �K�E�g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pPlayer->bChanceAttack == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //�e�g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                    if (pPlayer->bBullet == false)
                    {
                        //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                        pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                        pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                        pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                        pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 105);
                    }
                    else
                    {
                        //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                        pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    }
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //�v���XR
            else if (g_aUI[nCntUI].type == UITYPE_017)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // �K�E�g�p�\���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pPlayer->nChance > 0 && pPlayer->bChanceAttack == false 
                    && pPlayer->nCntEXAttackCD == 0 && pPlayer->fEnergy > 0)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
 
                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //�X�s�j���O�u���[�h
            else if (g_aUI[nCntUI].type == UITYPE_018)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //���K�E�g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pPlayer->bChanceAttack == true && pPlayer->bBullet == false)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //�X�v���b�h�t�@�C�A
            else if (g_aUI[nCntUI].type == UITYPE_019)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //���K�E�g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pPlayer->bChanceAttack == true && pPlayer->bBullet == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //ON/OFF
            else if (g_aUI[nCntUI].type == UITYPE_007)
            {
                if (pCamera->bLockOn == true)
                {
                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.0f + 0.5f, 1.0f);
                }
                else
                {
                    //�e�N�X�`�����W�̐ݒ�
                    pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f + 0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f + 0.5f, 1.0f);
                }

                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
            }

            //�K�E�Q�[�W�̔w�i
            else if (g_aUI[nCntUI].type == UITYPE_005)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // �K�E�g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pPlayer->bChanceAttack == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //�Q�[�W�̔w�i
            else if (g_aUI[nCntUI].type == UITYPE_006)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // �K�E�g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pPlayer->bChanceAttack == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //HP
            else if (g_aUI[nCntUI].type == UITYPE_008)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // �K�E�g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pPlayer->bChanceAttack == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //�G�i�W�[
            else if (g_aUI[nCntUI].type == UITYPE_009)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // �K�E�g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pPlayer->bChanceAttack == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //Y
            else if (g_aUI[nCntUI].type == UITYPE_016)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                // �K�E�g�p���Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pPlayer->bChanceAttack == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //�{�X��HP�Q�[�W
            else if (g_aUI[nCntUI].type == UITYPE_021)
            {
                //���_���W�̐ݒ�i���Q�[�W�j
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pBoss->fLife / BOSS_LIFE)),
                    g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pBoss->fLife / BOSS_LIFE)),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //�{�X�펞�Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pCamera->bBossMode == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j(��)
                    pVtx[0].col = D3DCOLOR_RGBA(188, 78, 189, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(188, 78, 189, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(188, 78, 189, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(188, 78, 189, 255);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2((pBoss->fLife / BOSS_LIFE), 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2((pBoss->fLife / BOSS_LIFE), 1.0f);
            }

            //�{�X��HP�o�[�̔w�i
            else if (g_aUI[nCntUI].type == UITYPE_020)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //�{�X�펞�Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pCamera->bBossMode == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //BOSS�̕���
            else if (g_aUI[nCntUI].type == UITYPE_022)
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //�{�X�펞�Ƃ����łȂ��Ƃ��œ����x��؂�ւ���
                if (pCamera->bBossMode == true)
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            else
            {
                //���_���W�̐ݒ�
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //���_�J���[��ݒ�i0�`255�̒l�Őݒ肷��j
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //�e�N�X�`�����W�̐ݒ�
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

        }
        pVtx += 4;
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffPolygonUI->Unlock();
}

//=============================================================================
// UI�`�揈��
//=============================================================================
void DrawUI(void)
{
    //�錾
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�A���t�@�e�X�g��L����
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //�A���t�@�e�X�g��l�̐ݒ�
    pDevice->SetRenderState(D3DRS_ALPHAREF, 30);
    //�A���t�@�e�X�g�̔�r���@�̐ݒ�iGREATER�͊�l���傫���ꍇ)
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffPolygonUI, 0, sizeof(VERTEX_2D));
    //���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF(FVF_VERTEX_2D);

    //�|���S���̕`��
    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        if (g_aUI[nCntUI].bUse == true)//�G���g�p���Ȃ�`��
        {
            //�e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, g_pTexturePolygonUI[g_aUI[nCntUI].type]);

            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntUI, NUM_POLYGON);
        }
    }

    //�A���t�@�e�X�g�𖳌���
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    //�e�N�X�`���̐ݒ�̉���
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
//UI�̐ݒ菈��
//=============================================================================
void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, UITYPE type)
{
    //�ϐ��錾
    VERTEX_2D *pVtx;

    //���_�o�b�t�@�����b�N
    g_pVtxBuffPolygonUI->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        if (g_aUI[nCntUI].bUse == false)
        {
            //�ʒu��ݒ�
            g_aUI[nCntUI].pos = pos;
            g_aUI[nCntUI].fSizeX = fSizeX;
            g_aUI[nCntUI].fSizeY = fSizeY;
            g_aUI[nCntUI].type = type;
            g_aUI[nCntUI].bUse = true;
            break;
        }
        pVtx += 4;
    }

    //���_�f�[�^���A�����b�N����
    g_pVtxBuffPolygonUI->Unlock();

}