//=============================================================================
//
// �ŋ��̘b�����̏��� [collision.cpp] (�Փ˔���A�����蔻��̋��ʉ�)
// Author:KUDO AYANO
//
//=============================================================================
#include "collision.h"

//=============================================================================
// [BoxCollision] ���ɑ΂���Փ˔���
// Author:KUDO AYANO
//
// ����
//  [in] pPos1     : �Ԃ��鑤�̈ʒu
//  [in] pPos1Old : �Ԃ��鑤��1F�O�̈ʒu
//  [in] size1    : �Ԃ��鑤�̑傫��
//  [in] pPos2    : �Ԃ����鑤�̈ʒu
//  [in] size2    : �Ԃ����鑤�̑傫��
//
// �߂�l : �Փ˂������ǂ���
//=============================================================================
bool BoxCollision(D3DXVECTOR3 * pPos1, D3DXVECTOR3 * pPos1Old, D3DXVECTOR3 size1,
    D3DXVECTOR3 * pPos2, D3DXVECTOR3 size2)
{
    //�Փ˂������ǂ���
    bool bCollision = false;

    D3DXVECTOR3 box1Max = D3DXVECTOR3(size1.x / 2, size1.y, size1.z / 2) + *pPos1;          //�Ԃ��鑤�̍ő�l
    D3DXVECTOR3 box1Min = D3DXVECTOR3(-size1.x / 2, -size1.y, -size1.z / 2) + *pPos1;       //�Ԃ��鑤�̍ŏ��l
    D3DXVECTOR3 box2Max = D3DXVECTOR3(size2.x / 2, size2.y / 2, size2.z / 2) + *pPos2;      //�Ԃ����鑤�̍ő�l
    D3DXVECTOR3 box2Min = D3DXVECTOR3(-size2.x / 2, -size2.y / 2, -size2.z / 2) + *pPos2;   //�Ԃ����鑤�̍ŏ��l

    if (box1Max.y > box2Min.y&&
        box1Min.y < box2Max.y&&
        box1Max.x > box2Min.x&&
        box1Min.x < box2Max.x&&
        box1Max.z > box2Min.z&&
        box1Min.z < box2Max.z)
    {
        bCollision = true;

        if (box1Max.x > box2Min.x&&
            pPos1Old->x + size1.x / 2 <= box2Min.x)
        {//X���̍�
            pPos1->x = box2Min.x - size1.x / 2;
        }
        if (box1Min.x < box2Max.x&&
            pPos1Old->x - size1.x / 2 >= box2Max.x)
        {//x���̉E
            pPos1->x = box2Max.x + size1.x / 2;
        }
        if (box1Max.z > box2Min.z&&
            pPos1Old->z + size1.z / 2 <= box2Min.z)
        {//z���̎�O
            pPos1->z = box2Min.z - size1.z / 2;
        }
        if (box1Min.z < box2Max.z&&
            pPos1Old->z - size1.z / 2 >= box2Max.z)
        {//z���̉�
            pPos1->z = box2Max.z + size1.z / 2;
        }
    }

    return bCollision;
}

//=============================================================================
// [RectangleCollision] ��`�̓����蔻��
// Author:KUDO AYANO
//
// ����
//  [in] pPos1     : �Ԃ��鑤�̈ʒu
//  [in] pPos1Old : �Ԃ��鑤��1F�O�̈ʒu
//  [in] size1    : �Ԃ��鑤�̑傫��
//  [in] pPos2    : �Ԃ����鑤�̈ʒu
//  [in] size2    : �Ԃ����鑤�̑傫��
//
// �߂�l : �������Ă��邩�ǂ���
//=============================================================================
bool RectangleCollision(D3DXVECTOR3 * pPos1, D3DXVECTOR3 * pPos1Old, D3DXVECTOR3 size1,
    D3DXVECTOR3 * pPos2, D3DXVECTOR3 size2)
{
    bool bHit = false;

    D3DXVECTOR3 box1Max = D3DXVECTOR3(size1.x / 2, size1.y, size1.z / 2) + *pPos1;          //�Ԃ��鑤�̍ő�l
    D3DXVECTOR3 box1Min = D3DXVECTOR3(-size1.x / 2, -size1.y, -size1.z / 2) + *pPos1;       //�Ԃ��鑤�̍ŏ��l
    D3DXVECTOR3 box2Max = D3DXVECTOR3(size2.x / 2, size2.y / 2, size2.z / 2) + *pPos2;      //�Ԃ����鑤�̍ő�l
    D3DXVECTOR3 box2Min = D3DXVECTOR3(-size2.x / 2, -size2.y / 2, -size2.z / 2) + *pPos2;   //�Ԃ����鑤�̍ŏ��l

    if (box1Max.y > box2Min.y&&
        box1Min.y < box2Max.y&&
        box1Max.x > box2Min.x&&
        box1Min.x < box2Max.x&&
        box1Max.z > box2Min.z&&
        box1Min.z < box2Max.z)
    {
        bHit = true;
    }
    return bHit;
}

//=============================================================================
// [SphereCollision] ���̓����蔻��
// Author:KUDO AYANO
//
// ����
//  [in] pos1     : �Ԃ��鑤�̈ʒu
//  [in] size1    : �Ԃ��鑤�̒��a
//  [in] pos2     : �Ԃ����鑤�̈ʒu
//  [in] size2    : �Ԃ����鑤�̒��a
//
// �߂�l : �Ԃ��鑤���~�̒��ɓ����Ă��邩�ǂ���
//=============================================================================
bool SphereCollision(D3DXVECTOR3 pos1, float size1, D3DXVECTOR3 pos2, float size2)
{
    float distance = powf(pos1.x - pos2.x, 2.0f) + powf(pos1.y - pos2.y, 2.0f) + powf(pos1.z - pos2.z, 2.0f);      //�ʒu
    float radius = powf(size1 + size2, 2.0f); //���a
    return (distance <= radius);
}