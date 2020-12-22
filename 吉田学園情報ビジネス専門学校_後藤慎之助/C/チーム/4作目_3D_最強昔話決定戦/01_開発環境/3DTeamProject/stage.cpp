//=============================================================================
//
// �ŋ��̘b�����̏��� [stage.cpp](�X�e�[�W���O���b�h�ŊǗ�)
// Author:KUDO AYANO
//
//=============================================================================
#include "stage.h"
#include "block.h"
#include "setting.h"

//*****************************************************************************
//�O���[�o���ϐ�
//*****************************************************************************
OBJECT_PLACED_INFO objectPlacedInfo[MAX_GRID];    //�O���b�h�̐����̏��

//=============================================================================
// [InitStage] �X�e�[�W���̏�����
// Author:KUDO AYANO
//=============================================================================
void InitStage(void)
{
    for (int nCntstage = 0; nCntstage < MAX_GRID; nCntstage++)
    {
        objectPlacedInfo[nCntstage].isPlaced = false;
    }
}

//=============================================================================
// [ConversionGrid] �O���b�g��̍��W�����Ƃ߂�
// Author:KUDO AYANO
// [in / out] pos : ���ݍ��W���󂯎��A�O���b�g���W�ɕϊ����ēn��
//=============================================================================
D3DXVECTOR3 ConversionGrid(D3DXVECTOR3* pos)
{
    D3DXVECTOR3 posGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // ��U���W��10�Ŋ���
    float conversionX = pos->x / GRID_SIZE;
    float conversionY = pos->y / GRID_SIZE;
    float conversionZ = pos->z / GRID_SIZE;

    // ���W���l�̌ܓ�����
    conversionX = (float)round(conversionX);
    conversionZ = (float)round(conversionZ);
    // y���͐؂�̂�
    conversionY = (float)floor(conversionY);

    posGrid = D3DXVECTOR3((float)conversionX * GRID_SIZE,
        (float)conversionY * GRID_SIZE,
        (float)conversionZ * GRID_SIZE);

    return posGrid;
}

//=============================================================================
// [ConfirmationGrid] �I�u�W�F�N�g���u����Ă��邩�ǂ��������m�F
// Author:KUDO AYANO
// [in] pos : ���ׂ������W
// �Ԃ�l�F���W�̈ʒu�ɃI�u�W�F�N�g���u����Ă��邩�ǂ�����Ԃ�
//=============================================================================
bool ConfirmationGrid(D3DXVECTOR3 pos)
{
    D3DXVECTOR3 Grid = D3DXVECTOR3(pos.x / GRID_SIZE, pos.y / GRID_SIZE, pos.z / GRID_SIZE);
    return objectPlacedInfo[(int)Grid.z * MESH_BLOCK_X + (int)Grid.x].isPlaced;
}

//=============================================================================
// [SetGridInfo] �X�e�[�W�̃}�X��̔z�u����ύX
// Author:KUDO AYANO
// [in] pos    : �I�u�W�F�N�g�̍��W
// [Placed]    : �ύX��̔z�u���
//=============================================================================
void SetGridInfo(D3DXVECTOR3 pos, bool Placed)
{
    //�ݒ�̏��擾
    SETTING *pSetting = GetSetting();

    //�݂�ȂŗV�ԂȂ�
    if (pSetting->nMaxPlayer >= 2)
    {
        D3DXVECTOR3 Grid = D3DXVECTOR3(pos.x / GRID_SIZE, pos.y / GRID_SIZE, pos.z / GRID_SIZE);
        int GridNum = (int)Grid.z * MESH_BLOCK_X + (int)Grid.x;
        objectPlacedInfo[GridNum].isPlaced = Placed;
    }
}

//=============================================================================
// [InputStageData] �t�@�C���̓ǂݍ���
// Author:KUDO AYANO
//=============================================================================
void InputStageData(void)
{
    char cRead[MAX_BLOCK];	//�����ǂݎ��p
    char cHead[MAX_BLOCK];	//��r�p
    D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    BLOCKTYPE type = BLOCKTYPE_000;
    FILE *pFile = NULL;//�t�@�C���I�[�v��

    //�ݒ�̏��擾
    SETTING *pSetting = GetSetting();

    //�݂�ȂŗV�ԂȂ�
    if (pSetting->nMaxPlayer != 1)
    {
        //�m�[�}���X�e�[�W�ǂݍ���
        if (pSetting->nStage == SETTINGSTAGE_NORMAL)
        {
            pFile = fopen("TXT/stage.txt", "r");//�t�@�C���I�[�v��
        }

        //�����X�e�[�W�ǂݍ���
        else if (pSetting->nStage == SETTINGSTAGE_PLAIN)
        {
            pFile = fopen("TXT/stage2.txt", "r");//�t�@�C���I�[�v��
        }

        //��ՃX�e�[�W�ǂݍ���
        else if (pSetting->nStage == SETTINGSTAGE_RUIN)
        {
            pFile = fopen("TXT/stage3.txt", "r");//�t�@�C���I�[�v��
        }

        //�N���X�X�e�[�W�ǂݍ���
        else if (pSetting->nStage == SETTINGSTAGE_CROSS)
        {
            pFile = fopen("TXT/stage4.txt", "r");//�t�@�C���I�[�v��
        }

        //�����_���X�e�[�W�ǂݍ���
        else if (pSetting->nStage == SETTINGSTAGE_RANDOM)
        {
            int nRandom = 0;

            nRandom = rand() % 4 + 1;    //1�`4�𓾂�

            if (nRandom == 1)
            {
                pFile = fopen("TXT/stage.txt", "r");//�t�@�C���I�[�v��
            }
            else if (nRandom == 2)
            {
                pFile = fopen("TXT/stage2.txt", "r");//�t�@�C���I�[�v��
            }
            else if (nRandom == 3)
            {
                pFile = fopen("TXT/stage3.txt", "r");//�t�@�C���I�[�v��
            }
            else if (nRandom == 4)
            {
                pFile = fopen("TXT/stage4.txt", "r");//�t�@�C���I�[�v��
            }
        }
    }

    //�ЂƂ�ŗV�ԂȂ�
    else
    {
        pFile = fopen("TXT/story.txt", "r");//�t�@�C���I�[�v��
    }

    if (pFile == NULL)
    {
        return;//�t�@�C�����J���Ȃ�������I��
    }
    while (strcmp(cHead, "STAGE_DATA"))//STAGE_DATA��ǂݍ��ނ܂Ń��[�v
    {
        fgets(cRead, sizeof(cRead), pFile);
        sscanf(cRead, "%s", &cHead);//�ǂݍ��񂾃e�L�X�g���r�p�ϐ��ɑ��
    }
    if (strcmp(cHead, "STAGE_DATA") == 0)//SCRIPT��ǂݍ��񂾎�
    {
        while (strcmp(cHead, "END_STAGE_DATA") != 0)//END_STAGE_DATA��ǂݍ��ނ܂Ń��[�v
        {
            fgets(cRead, sizeof(cRead), pFile);//�ʒu�s�P�ʂŕ������ǂݍ���
            sscanf(cRead, "%s", &cHead);//�ǂݍ��񂾃e�L�X�g���r�p�ϐ��ɑ��
            sscanf(cRead, "%g,%g,%g,%d", &pos.x, &pos.y, &pos.z, &type);

            //�u���b�N��z�u
            SetBlock(pos, type);
        }
        fclose(pFile);//�t�@�C�������
    }
}