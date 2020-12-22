//=============================================================================
//
// �ŋ��̘b�����̏��� [time.cpp] (�������ԁA�T�h���f�X)
// Author:�㓡�T�V��
//
//=============================================================================
#include "time.h"
#include "item.h"
#include "player.h"
#include "ui.h"
#include "game.h"
#include "block.h"
#include "stage.h"
#include "setting.h"
#include "sound.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;//���_�o�b�t�@�ւ̃|�C���^

int g_nCounterTime;	    //���Ԃ𐔂���
int g_nTimeSecond;	    //�������Ԃ̕b
bool g_bTimeUp;         //�^�C���A�b�v�������ǂ���

bool g_bUseSuddenDeath; //�T�h���f�X���g�����ǂ���
bool g_bSuddenDeath;    //�T�h���f�X�ɓ˓��������ǂ���
bool g_bSoundWarning;       //�x���̉����g��
bool g_bSoundSuddenDeath;   //�T�h���f�X�̉����g��
bool g_bSoundFinish;        //�t�B�j�b�V�������g��
int g_nCntSuddenDeath;  //�T�h���f�X�̎��Ԃ𐔂���
float g_fSuddenDeathBlockX;  //�T�h���f�X�u���b�N��X���W
float g_fSuddenDeathBlockZ;  //�T�h���f�X�u���b�N��Z���W
bool g_bAroundTheCorner1;     //�T�h���f�X�u���b�N���p���Ȃ���1
bool g_bAroundTheCorner2;     //�T�h���f�X�u���b�N���p���Ȃ���2
bool g_bAroundTheCorner3;     //�T�h���f�X�u���b�N���p���Ȃ���3
bool g_bAroundTheCorner4;     //�T�h���f�X�u���b�N���p���Ȃ���4
bool g_bAroundTheCorner5;     //�T�h���f�X�u���b�N���p���Ȃ���5
bool g_bAroundTheCorner6;     //�T�h���f�X�u���b�N���p���Ȃ���6
bool g_bAroundTheCorner7;     //�T�h���f�X�u���b�N���p���Ȃ���7
bool g_bAroundTheCorner8;     //�T�h���f�X�u���b�N���p���Ȃ���8
bool g_bAroundTheCorner9;     //�T�h���f�X�u���b�N���p���Ȃ���9
bool g_bAroundTheCorner10;    //�T�h���f�X�u���b�N���p���Ȃ���10
bool g_bAroundTheCorner11;    //�T�h���f�X�u���b�N���p���Ȃ���11
bool g_bSetBlockFinish;       //�T�h���f�X�u���b�N�z�u�I��

//=============================================================================
// �^�C������������
// Author:�㓡�T�V��
//=============================================================================
void InitTime(void)
{
    //�ϐ��錾
    VERTEX_2D *pVtx = NULL;//���_���̃|�C���^
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�ݒ�̏��擾
    SETTING *pSetting = GetSetting();

    //�O���[�o���ϐ��̏�����
    g_nCounterTime = 0;
    g_nTimeSecond = TIME_FIREST_SECOND;	//�������Ԃ̍ŏ��̕b�̈ʂ�0
    g_bTimeUp = false;
    g_bUseSuddenDeath = pSetting->bSuddenDeath;
    g_bSuddenDeath = false;
    g_bSoundWarning = false;
    g_bSoundSuddenDeath = false;
    g_bSoundFinish = false;
    g_nCntSuddenDeath = 0;
    g_fSuddenDeathBlockX = 0.0f;
    g_fSuddenDeathBlockZ = 0.0f;
    g_bAroundTheCorner1 = false;
    g_bAroundTheCorner2 = false;
    g_bAroundTheCorner3 = false;
    g_bAroundTheCorner4 = false;
    g_bAroundTheCorner5 = false;
    g_bAroundTheCorner6 = false;
    g_bAroundTheCorner7 = false;
    g_bAroundTheCorner8 = false;
    g_bAroundTheCorner9 = false;
    g_bAroundTheCorner10 = false;
    g_bAroundTheCorner11 = false;
    g_bSetBlockFinish = false;

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/number000.png", &g_pTextureTime);

    //���_�o�b�t�@����
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX * MAX_TIME,//�o�b�t�@�T�C�Y
        D3DUSAGE_WRITEONLY,//(�Œ�j
        FVF_VERTEX_2D,//���_�t�H�[�}�b�g
        D3DPOOL_MANAGED,//�i�Œ�j
        &g_pVtxBuffTime,//�ϐ������ς��ƕύX���K�v
        NULL);

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    for (int nTime = 0; nTime < MAX_TIME; nTime++)
    {
        //���_���W�̐ݒ�i�E���Őݒ肷��j
        pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - (nTime*TIME_SIZE_X + TIME_SIZE_X / 2) - (TIME_SIZE_X / 2) - 565, (SCREEN_HEIGHT - TIME_SIZE_Y / 2) - (TIME_SIZE_Y / 2) - 640, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - (nTime*TIME_SIZE_X + TIME_SIZE_X / 2) + (TIME_SIZE_X / 2) - 565, (SCREEN_HEIGHT - TIME_SIZE_Y / 2) - (TIME_SIZE_Y / 2) - 640, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - (nTime*TIME_SIZE_X + TIME_SIZE_X / 2) - (TIME_SIZE_X / 2) - 565, (SCREEN_HEIGHT - TIME_SIZE_Y / 2) + (TIME_SIZE_Y / 2) - 640, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - (nTime*TIME_SIZE_X + TIME_SIZE_X / 2) + (TIME_SIZE_X / 2) - 565, (SCREEN_HEIGHT - TIME_SIZE_Y / 2) + (TIME_SIZE_Y / 2) - 640, 0.0f);

        //rhw�̐ݒ�
        pVtx[0].rhw = 1.0f;
        pVtx[1].rhw = 1.0f;
        pVtx[2].rhw = 1.0f;
        pVtx[3].rhw = 1.0f;

        //���_�J���[�̐ݒ�
        pVtx[0].col = TIME_COLOR;
        pVtx[1].col = TIME_COLOR;
        pVtx[2].col = TIME_COLOR;
        pVtx[3].col = TIME_COLOR;

        //�e�N�X�`���̍��W
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

        pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffTime->Unlock();

}
//=============================================================================
// �^�C���X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateTime(void)
{
    //�ϐ��錾
    VERTEX_2D *pVtx = NULL; //���_���̃|�C���^
    float fIndex;		    //�w��
    float fRadix = 10.0f;	//�

    //�T�h���f�X�p�̕ϐ�
    float posX = 0.0f;
    float posZ = 0.0f;
    D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    
    //�J�E���^���Z
    if (g_bTimeUp == false)  //�^�C���A�b�v���Ă��Ȃ��Ȃ�
    {
        g_nCounterTime++;    //�J�E���^�X�V

        if (g_nCounterTime == 60)	//60fps��1�b
        {
            //�J�E���g�_�E��
            g_nTimeSecond--;

            //�J�E���^��߂�
            g_nCounterTime = 0;
        }

        //60�b�����I�������A���������A�b��59�ɖ߂�
        if (g_nTimeSecond < 0)
        {
            g_nTimeSecond = 59;
            g_nTimeMinute--;
        }
    }

    //1���Ōx��
    if (g_bUseSuddenDeath == true && g_nTimeSecond == 0 && g_nTimeMinute == 1)
    {
        //�x�����𗬂�
        if (g_bSoundWarning == false)
        {
            g_bSoundWarning = true;

            SetUI(D3DXVECTOR3(1480.0f, SCREEN_HEIGHT / 2, 0.0f), 400.0f, 200.0f, UITYPE_011);  //�}���`
            StopSound(SOUND_LABEL_BGM_GAME);            //�Q�[���̉��y���~�߂�
            PlaySound(SOUND_LABEL_SE_WARNING);          //�x����
        }
    }

    //�������Ԏc��55�b�ŃT�h���f�X�˓�
    if (g_nTimeSecond == 55 && g_nTimeMinute == 0)
    {
        //�ݒ�ŁA�T�h���f�X���g���Ȃ�
        if (g_bUseSuddenDeath == true)
        {
            g_bSuddenDeath = true;

            //�T�h���f�X�̉��y�𗬂�
            if (g_bSoundSuddenDeath == false)
            {
                PlaySound(SOUND_LABEL_BGM_SUDDEN_DEATH);    //�T�h���f�X�̉��y
                g_bSoundSuddenDeath = true;
            }
        }
    }

    //���ƕb��0�Ȃ�A�^�C���A�b�v
    if (g_nTimeSecond == 0 && g_nTimeMinute == 0)
    {
        //�^�C���A�b�v�ɂ���
        g_bTimeUp = true;

        //�t�B�j�b�V���̉��y�𗬂�
        if (g_bSoundFinish == false)
        {
            PlaySound(SOUND_LABEL_SE_FINISH);    //�t�B�j�b�V���̌��ʉ�
            g_bSoundFinish = true;
        }

        //���������ɂ���
        GAME *pGame = GetGame();
        pGame->nCounter = 0;

        //�ݒ�̏��擾
        SETTING *pSetting = GetSetting();
        //�X�g�[���[�Ȃ�
        if (pSetting->nMaxPlayer == 1)
        {
            PlaySound(SOUND_LABEL_SE_FINISH);       //�t�B�j�b�V���̌��ʉ�
            SetGameState(GAMESTATE_FINSIH_GAME);    //�Q�[���I�[�o�[
        }
    }

    //�T�h���f�X���A�u���b�N�z�u���I����Ă��Ȃ��Ȃ�
    if (g_bSuddenDeath == true && g_bSetBlockFinish == false)
    {
        g_nCntSuddenDeath++;

        //�u���b�N��z�u
        if (g_nCntSuddenDeath >= SUDDEN_DEATH_SET_BLOCK)
        {
            g_nCntSuddenDeath = 0;

            //X���W��Z���W�����ɐi�߂�
            posX = 100.0f + 100.0f * g_fSuddenDeathBlockX;
            posZ = 100.0f + 100.0f * g_fSuddenDeathBlockZ;
            pos = D3DXVECTOR3(posX, 0.0f, posZ);

            //�O���b�h���W�ɕϊ�����
            pos = ConversionGrid(&pos);

            //�T�h���f�X�u���b�N��z�u
            SetBlock(D3DXVECTOR3(pos.x, SUDDEN_DEATH_BLOCK_Y, pos.z), BLOCKTYPE_006);

            //�O�ԉ�
            if (g_bAroundTheCorner11 == true)
            {
                g_fSuddenDeathBlockX -= 1.0f;

                if (g_fSuddenDeathBlockX == 2.0f)
                {
                    g_bSetBlockFinish = true;   //�z�u�I��
                }
            }

            //�O�ԉE
            if (g_bAroundTheCorner10 == true && g_bAroundTheCorner11 == false)
            {
                g_fSuddenDeathBlockZ -= 1.0f;

                if (g_fSuddenDeathBlockZ == 2.0f)
                {
                    g_bAroundTheCorner11 = true;
                }
            }

            //�O�ԏ�
            if (g_bAroundTheCorner9 == true && g_bAroundTheCorner10 == false)
            {
                g_fSuddenDeathBlockX += 1.0f;

                if (g_fSuddenDeathBlockX == 10.0f)
                {
                    g_bAroundTheCorner10 = true;
                }
            }

            //�O�ԍ�
            if (g_bAroundTheCorner8 == true && g_bAroundTheCorner9 == false)
            {
                g_fSuddenDeathBlockZ += 1.0f;

                if (g_fSuddenDeathBlockZ == 8.0f)
                {
                    g_bAroundTheCorner9 = true;
                }
            }

            //��ԉ�
            if (g_bAroundTheCorner7 == true && g_bAroundTheCorner8 == false)
            {
                g_fSuddenDeathBlockX -= 1.0f;

                if (g_fSuddenDeathBlockX == 2.0f)
                {
                    g_bAroundTheCorner8 = true;
                }
            }

            //��ԉE
            if (g_bAroundTheCorner6 == true && g_bAroundTheCorner7 == false)
            {
                g_fSuddenDeathBlockZ -= 1.0f;

                if (g_fSuddenDeathBlockZ == 1.0f)
                {
                    g_bAroundTheCorner7 = true;
                }
            }

            //��ԏ�
            if (g_bAroundTheCorner5 == true && g_bAroundTheCorner6 == false)
            {
                g_fSuddenDeathBlockX += 1.0f;

                if (g_fSuddenDeathBlockX == 11.0f)
                {
                    g_bAroundTheCorner6 = true;
                }
            }

            //��ԍ�
            if (g_bAroundTheCorner4 == true && g_bAroundTheCorner5 == false)
            {
                g_fSuddenDeathBlockZ += 1.0f;

                if (g_fSuddenDeathBlockZ == 9.0f)
                {
                    g_bAroundTheCorner5 = true;
                }
            }

            //��ԉ�
            if (g_bAroundTheCorner3 == true && g_bAroundTheCorner4 == false)
            {
                g_fSuddenDeathBlockX -= 1.0f;

                if (g_fSuddenDeathBlockX == 1.0f)
                {
                    g_bAroundTheCorner4 = true;
                }
            }

            //��ԉE
            if (g_bAroundTheCorner2 == true && g_bAroundTheCorner3 == false)
            {
                g_fSuddenDeathBlockZ -= 1.0f;

                if (g_fSuddenDeathBlockZ == 0.0f)
                {
                    g_bAroundTheCorner3 = true;
                }
            }

            //��ԏ�
            if (g_bAroundTheCorner1 == true && g_bAroundTheCorner2 == false)
            {
                g_fSuddenDeathBlockX += 1.0f;

                if (g_fSuddenDeathBlockX == 12.0f)
                {
                    g_bAroundTheCorner2 = true;
                }
            }

            //��ԍ�
            if (g_bAroundTheCorner1 == false)
            {
                g_fSuddenDeathBlockZ += 1.0f;

                if (g_fSuddenDeathBlockZ == 10.0f)
                {
                    g_bAroundTheCorner1 = true;
                }
            }

        }
    }

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    for (fIndex = 0; fIndex < MAX_TIME; fIndex++)
    {
        int nTime = (int)powf(fRadix, MAX_TIME - fIndex);
        int nTime2 = (int)powf(fRadix, MAX_TIME - fIndex - 1);
        int nAnswer = g_nTimeSecond % nTime / nTime2;

        //�e�N�X�`���̍��W
        pVtx[4].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
        pVtx[5].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
        pVtx[6].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
        pVtx[7].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

        pVtx -= 4;//���_�f�[�^�̃|�C���^��4���i�߂�
    }

    //���_�f�[�^�A�����b�N
    g_pVtxBuffTime->Unlock();
}
//=============================================================================
// �^�C���I������
// Author:�㓡�T�V��
//=============================================================================
void UninitTime(void)
{
    //�e�N�X�`���̊J��
    if (g_pTextureTime != NULL)
    {
        g_pTextureTime->Release();
        g_pTextureTime = NULL;
    }

    //���_�o�b�t�@�̊J��
    if (g_pVtxBuffTime != NULL)
    {
        g_pVtxBuffTime->Release();
        g_pVtxBuffTime = NULL;
    }
}

//=============================================================================
// �^�C���`�揈��
// Author:�㓡�T�V��
//=============================================================================
void DrawTime(void)
{

    //���_�t�H�[�}�b�g�̐ݒ�
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //�A���t�@�e�X�g��L����
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //�A���t�@�e�X�g��l�̐ݒ�
    pDevice->SetRenderState(D3DRS_ALPHAREF, 30);
    //�A���t�@�e�X�g�̔�r���@�̐ݒ�iGREATER�͊�l���傫���ꍇ)
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

    //�X�R�A�̐����̕`��
    for (int nTime = 0; nTime < MAX_TIME; nTime++)
    {
        //�e�N�X�`���̐ݒ�
        pDevice->SetTexture(0, g_pTextureTime);

        //�|���S���̕`��
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nTime, NUM_POLYGON);
    }

    //�A���t�@�e�X�g�𖳌���
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    //�e�N�X�`���̐ݒ�
    pDevice->SetTexture(0, NULL);
}