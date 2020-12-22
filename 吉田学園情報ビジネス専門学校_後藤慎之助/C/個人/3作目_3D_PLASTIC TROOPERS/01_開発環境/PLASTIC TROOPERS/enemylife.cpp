////=============================================================================
////
//// �̗͏��� [enemylife.cpp]
//// Author : ��@�T��
////
////=============================================================================
//
//#include "main.h"
//#include "enemylife.h"
//#include "player.h"
//#include "enemy.h"
//
////*****************************************************************************
//// �O���[�o���ϐ�
////*****************************************************************************
//LPDIRECT3DTEXTURE9 g_pTextureEnemyLife = NULL;
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemyLife = NULL;
//
//ENEMYLIFE g_aEnemyLife[MAX_ENEMYLIFE];
//
//D3DXCOLOR g_colorEnemyLife[MAX_ENEMYLIFE];
//
////=============================================================================
//// ����������
////=============================================================================
//void InitEnemyLife(void)
//{
//    LPDIRECT3DDEVICE9 pDevice = GetDevice();
//    VERTEX_3D *pVtx;//���_���̃|�C���^
//                    //�e�N�X�`���̓ǂݍ���
//    int nCntEnemyLife;
//
//    D3DXCreateTextureFromFile(pDevice,
//        "TEXTURE/bar000.png",//�t�@�C���̖��O
//        &g_pTextureEnemyLife);
//    //���_�o�b�t�@����
//    pDevice->CreateVertexBuffer(
//        sizeof(VERTEX_3D)*NUM_VERTEX*MAX_ENEMYLIFE,//�o�b�t�@�T�C�Y
//        D3DUSAGE_WRITEONLY,//(�Œ�)
//        FVF_VERTEX_3D,//(�Œ�)
//        D3DPOOL_MANAGED,//(�Œ�)
//        &g_pVtxBuffEnemyLife,//�ϐ������ς��ƕύX���K�v
//        NULL);//���_���ւ̃|�C���^
//
//              //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//    g_pVtxBuffEnemyLife->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�
//
//    for (nCntEnemyLife = 0; nCntEnemyLife < MAX_ENEMYLIFE; nCntEnemyLife++)
//    {
//        g_aEnemyLife[nCntEnemyLife].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//        g_aEnemyLife[nCntEnemyLife].fMaxLife = 0.0f;
//        g_aEnemyLife[nCntEnemyLife].bUse = false;
//        g_aEnemyLife[nCntEnemyLife].nEnemyIndex = -1;
//
//        //���_���W�̐ݒ�(�E���Őݒ肷��)
//        pVtx[0].pos = D3DXVECTOR3((-ENEMYLIFE_SIZE_X / 2), (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//        pVtx[1].pos = D3DXVECTOR3((ENEMYLIFE_SIZE_X / 2), (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//        pVtx[2].pos = D3DXVECTOR3((-ENEMYLIFE_SIZE_X / 2), (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//        pVtx[3].pos = D3DXVECTOR3((ENEMYLIFE_SIZE_X / 2), (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//        //�@���̐ݒ�
//        pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//        pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//        pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//        pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//
//        //�e�N�X�`���̐ݒ�
//        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//        //���_�J���[�̐ݒ�
//        pVtx[0].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f);
//        pVtx[1].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f);
//        pVtx[2].col = D3DXCOLOR(0.3f, 0.0f, 0.7f, 1.0f);
//        pVtx[3].col = D3DXCOLOR(0.3f, 0.0f, 0.7f, 1.0f);
//
//        pVtx += 4;
//    }
//    //���_�f�[�^���A�����b�N����
//    g_pVtxBuffEnemyLife->Unlock();
//}
////=============================================================================
//// �I������
////=============================================================================
//void UninitEnemyLife(void)
//{
//    //�e�N�X�`���̉��
//    if (g_pTextureEnemyLife != NULL)
//    {
//        g_pTextureEnemyLife->Release();
//        g_pTextureEnemyLife = NULL;
//    }
//    //
//    if (g_pVtxBuffEnemyLife != NULL)
//    {
//        g_pVtxBuffEnemyLife->Release();
//        g_pVtxBuffEnemyLife = NULL;
//    }
//}
////=============================================================================
//// �X�V����
////=============================================================================
//void UpdateEnemyLife(void)
//{
//    VERTEX_3D*pVtx;
//    //�G������
//    ENEMY *pEnemy;
//
//    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//    g_pVtxBuffEnemyLife->Lock(0, 0, (void**)&pVtx, 0);
//
//    for (int nCntEnemyLife = 0; nCntEnemyLife < MAX_ENEMYLIFE; nCntEnemyLife++)
//    {
//        if (g_aEnemyLife[nCntEnemyLife].bUse == true)
//        {
//            for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
//            {
//                pEnemy = GetEnemy();
//
//                if (g_aEnemyLife[nCntEnemyLife].nEnemyIndex == nCntEnemy)
//                {//�����ꂽ�C���f�b�N�X����nCntEnemy����v������
//                    if (pEnemy->bUse == false)
//                    {//���C�t�o�[������Ȃ���Ԃ�������
//
//                        g_aEnemyLife[nCntEnemyLife].bUse = false;
//                    }
//                    else
//                    {//���C�t�o�[���g�p����
//
//                        if (pEnemy->type == ENEMYTYPE_000)
//                        {
//                            g_aEnemyLife[nCntEnemyLife].pos = D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y + ENEMYLIFE_POS_Y_000, pEnemy->pos.z);
//
//                            //�ʒu�X�V(�G�̍ő�̗̑͂������Ă���͈̂ʒu�����̂���)
//                            pVtx[0].pos = D3DXVECTOR3(-((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2), (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[1].pos = D3DXVECTOR3(((float)pEnemy->fLife * ENEMYLIFE_SIZE_X / 2) - ((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2),
//                                (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[2].pos = D3DXVECTOR3(-((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2), (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[3].pos = D3DXVECTOR3((pEnemy->fLife * ENEMYLIFE_SIZE_X / 2) - ((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2),
//                                (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//                        }
//                        else if (pEnemy->type == ENEMYTYPE_001)
//                        {
//                            g_aEnemyLife[nCntEnemyLife].pos = D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y + ENEMYLIFE_POS_Y_001, pEnemy->pos.z);
//
//                            //�ʒu�X�V(�G�̍ő�̗̑͂������Ă���͈̂ʒu�����̂���)
//                            pVtx[0].pos = D3DXVECTOR3(-((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2), (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[1].pos = D3DXVECTOR3((pEnemy->fLife * ENEMYLIFE_SIZE_X / 2) - ((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2),
//                                (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[2].pos = D3DXVECTOR3(-((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2), (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[3].pos = D3DXVECTOR3((pEnemy->fLife * ENEMYLIFE_SIZE_X / 2) - ((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2),
//                                (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//                        }
//                        else
//                        {
//                            g_aEnemyLife[nCntEnemyLife].pos = D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y + BOSSLIFE_POS_Y_001, pEnemy->pos.z);
//
//                            //�ʒu�X�V(�G�̍ő�̗̑͂������Ă���͈̂ʒu�����̂���)
//                            pVtx[0].pos = D3DXVECTOR3(-((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2), (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[1].pos = D3DXVECTOR3((pEnemy->fLife * ENEMYLIFE_SIZE_X / 2) - ((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2),
//                                (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[2].pos = D3DXVECTOR3(-((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2), (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[3].pos = D3DXVECTOR3(((float)pEnemy->fLife * ENEMYLIFE_SIZE_X / 2) - ((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2),
//                                (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//                        }
//                    }
//
//                    break;
//                }
//            }
//
//
//        }
//        pVtx += 4;
//    }
//    //���_�f�[�^���A�����b�N����
//    g_pVtxBuffEnemyLife->Unlock();
//}
//
////=============================================================================
//// �`�揈��
////=============================================================================
//void DrawEnemyLife(void)
//{
//    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^���擾
//    D3DXMATRIX mtxRot, mtxTrans;//�s��v�Z�p�̃}�g���N�X
//
//                                //�A���t�@�e�X�g��L����
//    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//    //�A���t�@�e�X�g��l�̐ݒ�
//    pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
//    //�A���t�@�e�X�g�̔�r���@�̐ݒ�iGREATER�͊�l���傫���ꍇ)
//    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
//
//    //���C�g�̉e�����󂯂Ȃ�����
//    DWORD ambient;
//    pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
//    pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
//    pDevice->LightEnable(0, FALSE);
//
//    for (int nCntEnemyLife = 0; nCntEnemyLife < MAX_ENEMYLIFE; nCntEnemyLife++)
//    {
//        if (g_aEnemyLife[nCntEnemyLife].bUse == true)
//        {
//            //���[���h�}�g���N�X�̏�����
//            D3DXMatrixIdentity(&g_aEnemyLife[nCntEnemyLife].mtxWorld);
//            //��]�̋t�s��𐶐�
//            pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
//            D3DXMatrixInverse(&g_aEnemyLife[nCntEnemyLife].mtxWorld, NULL, &mtxRot);
//            g_aEnemyLife[nCntEnemyLife].mtxWorld._41 = 0;//�ړ��͂��������Ȃ��̂�0����
//            g_aEnemyLife[nCntEnemyLife].mtxWorld._42 = 0;//�ړ��͂��������Ȃ��̂�0����
//            g_aEnemyLife[nCntEnemyLife].mtxWorld._43 = 0;//�ړ��͂��������Ȃ��̂�0����
//
//                                                  //�ʒu�𔽉f
//            D3DXMatrixTranslation(&mtxTrans, g_aEnemyLife[nCntEnemyLife].pos.x, g_aEnemyLife[nCntEnemyLife].pos.y, g_aEnemyLife[nCntEnemyLife].pos.z);
//            D3DXMatrixMultiply(&g_aEnemyLife[nCntEnemyLife].mtxWorld, &g_aEnemyLife[nCntEnemyLife].mtxWorld, &mtxTrans);
//            //���[���h�}�g���b�N�X�̐ݒ�
//            pDevice->SetTransform(D3DTS_WORLD, &g_aEnemyLife[nCntEnemyLife].mtxWorld);
//            //�e�N�X�`���̐ݒ�
//            pDevice->SetTexture(0, g_pTextureEnemyLife);
//            //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
//            pDevice->SetStreamSource(0, g_pVtxBuffEnemyLife, 0, sizeof(VERTEX_3D));
//            //���_�t�H�[�}�b�g�̐ݒ�
//            pDevice->SetFVF(FVF_VERTEX_3D);
//            //�|���S���̕`��
//            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemyLife * 4, NUM_POLYGON);
//
//        }
//    }
//
//    //�A���t�@�e�X�g�𖳌���
//    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
//
//    //���C�g�̉e�����󂯂�悤�ɂ���
//    pDevice->SetRenderState(D3DRS_AMBIENT, ambient);
//    pDevice->LightEnable(0, TRUE);
//
//    //�e�N�X�`���̐ݒ�̉���
//    pDevice->SetTexture(0, NULL);
//
//}
////=============================================================================
//// �̗͂̐ݒ�
////=============================================================================
//void SetEnemyLife(float fMaxLife, int nCntEnemy)
//{
//    VERTEX_3D*pVtx;
//    int nCntEnemyLife;
//    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//    g_pVtxBuffEnemyLife->Lock(0, 0, (void**)&pVtx, 0);
//
//    for (nCntEnemyLife = 0; nCntEnemyLife < MAX_ENEMYLIFE; nCntEnemyLife++)
//    {
//        if (g_aEnemyLife[nCntEnemyLife].bUse == false)//���������g�p�̏ꍇ
//        {
//            g_aEnemyLife[nCntEnemyLife].bUse = true;//�g�p���Ă����Ԃɂ���
//            g_aEnemyLife[nCntEnemyLife].fMaxLife = fMaxLife;
//            g_aEnemyLife[nCntEnemyLife].nEnemyIndex = nCntEnemy;
//            break;
//        }
//        pVtx += 4;
//    }
//    //���_�f�[�^���A�����b�N����
//    g_pVtxBuffEnemyLife->Unlock();
//}
//
//void UnsetEnemyLife(int nCntEnemy)
//{
//    for (int nCntEnemyLife = 0; nCntEnemyLife < MAX_ENEMYLIFE; nCntEnemyLife++)
//    {
//        if (g_aEnemyLife[nCntEnemyLife].bUse == true &&
//            g_aEnemyLife[nCntEnemyLife].nEnemyIndex == nCntEnemy)
//        {
//            g_aEnemyLife[nCntEnemyLife].bUse = false;
//
//            break;
//        }
//    }
//}