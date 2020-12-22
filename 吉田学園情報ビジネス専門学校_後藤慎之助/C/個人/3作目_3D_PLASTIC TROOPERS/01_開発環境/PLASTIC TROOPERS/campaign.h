//=============================================================================
//
// PLASTIC TROOPERS [campaign.h]�̏��� (�{�X��|���ɍs�����[�h)
// Author:�㓡�T�V��
//
//=============================================================================
#ifndef _CAMPAIGN_H_
#define _CAMPAIGN_H_
#define _CRT_SECURE_NO_WARNINGS_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
// �\���̒�`
//=============================================================================
typedef enum
{
	CAMPAIGNSTATE_NONE,
	CAMPAIGNSTATE_NORMAL,
    CAMPAIGNSTATE_PAUSE,
	CAMPAIGNSTATE_NEXTSTAGE,	//���̃X�e�[�W��
	CAMPAIGNSTATE_GAMEOVER,	    //�Q�[���I�[�o�[
	CAMPAIGNSTATE_MAX
}CAMPAIGNSTATE;

//=============================================================================
//�v���g�^�C�v�錾
//=============================================================================
void InitCampaign(void);
void UpdateCampaign(void);
void UninitCampaign(void);
void DrawCampaign(void);

void SetCampaignState(CAMPAIGNSTATE state);//�Q�[���̏��
#endif