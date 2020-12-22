//===============================================
//
// アニメーション処理 (animation.cpp)
// Author : 樋宮 匠
//
//===============================================

#define _CRT_SECURE_NO_WARNINGS

//========================
// インクルードファイル
//========================
#include "animation.h"
#include "game.h"

CAnimation::CAnimation()
{
	memset(m_keyInfo, 0, sizeof(m_keyInfo));
	m_animation = ANIM_IDLE;
	m_nFrame = 0;
	m_nKey = 0;

	for (int nCount = 0; nCount < CCharacter::PARTS_MAX; nCount++)
	{
		m_pos[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rot[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

CAnimation::~CAnimation()
{
}

HRESULT CAnimation::Init(CCharacter *pCharacter)
{
	memset(m_keyInfo, 0, sizeof(m_keyInfo));
	m_animation = ANIM_IDLE;
	m_pCharacter = pCharacter;
	m_nFrame = 0;
	m_nKey = 0;

	for (int nCount = 0; nCount < CCharacter::PARTS_MAX; nCount++)
	{
		m_pos[nCount] = CCharacter::GetDefaultPos(nCount);
		m_rot[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_posDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rotDest[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return S_OK;
}

void CAnimation::Uninit(void)
{
}

void CAnimation::Update(void)
{
	//モーションが変わったとき
	if (m_animationOld != m_animation)
	{
		m_nFrame = 0;
		m_nKey = 0;
	}
	for (int nCntParts = 0; nCntParts < CCharacter::PARTS_MAX; nCntParts++)
	{
		if (m_nFrame == 0)
		{
			m_posDest[nCntParts].x = ((m_keyInfo[m_animation].parts[nCntParts].pos[m_nKey].x - (m_pos[nCntParts].x - CCharacter::GetDefaultPos(nCntParts).x)) / float(m_keyInfo[m_animation].nFrame[m_nKey]));

			m_posDest[nCntParts].y = ((m_keyInfo[m_animation].parts[nCntParts].pos[m_nKey].y - (m_pos[nCntParts].y - CCharacter::GetDefaultPos(nCntParts).y)) / float(m_keyInfo[m_animation].nFrame[m_nKey]));

			m_posDest[nCntParts].z = ((m_keyInfo[m_animation].parts[nCntParts].pos[m_nKey].z - (m_pos[nCntParts].z - CCharacter::GetDefaultPos(nCntParts).z)) / float(m_keyInfo[m_animation].nFrame[m_nKey]));

			m_rotDest[nCntParts].x = (m_keyInfo[m_animation].parts[nCntParts].rot[m_nKey].x - m_rot[nCntParts].x) / float(m_keyInfo[m_animation].nFrame[m_nKey]);

			m_rotDest[nCntParts].y = (m_keyInfo[m_animation].parts[nCntParts].rot[m_nKey].y - m_rot[nCntParts].y) / float(m_keyInfo[m_animation].nFrame[m_nKey]);

			m_rotDest[nCntParts].z = (m_keyInfo[m_animation].parts[nCntParts].rot[m_nKey].z - m_rot[nCntParts].z) / float(m_keyInfo[m_animation].nFrame[m_nKey]);
		}

		m_pos[nCntParts] += m_posDest[nCntParts];
		m_rot[nCntParts] += m_rotDest[nCntParts];

		m_pCharacter->SetPos(nCntParts, m_pos[nCntParts]);
		m_pCharacter->SetRot(nCntParts, m_rot[nCntParts]);
	}
	m_animationOld = m_animation;
	// ループするとき
	if (m_nFrame >= m_keyInfo[m_animation].nFrame[m_nKey] && m_keyInfo[m_animation].nLoop == 1)
	{
		m_nFrame = 0;
		m_nKey++;

		if (m_nKey >= m_keyInfo[m_animation].nKeyNum)
		{
			m_nKey = 0;
		}

	}
	// ループしないとき
	else if (m_nFrame >= m_keyInfo[m_animation].nFrame[m_nKey] && m_keyInfo[m_animation].nLoop == 0)
	{
		m_nFrame = 0;
		m_nKey++;
		if (m_nKey >= m_keyInfo[m_animation].nKeyNum)
		{
			for (int nCntMotion = 0; nCntMotion < CCharacter::PARTS_MAX; nCntMotion++)
			{
				m_posDest[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_rotDest[nCntMotion] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			m_animation = ANIM_IDLE;
			m_nKey = 0;
		}
	}
	else
	{
		m_nFrame++;
	}
}

void CAnimation::Draw(void)
{
}

CAnimation * CAnimation::Create(CCharacter *pCharacter)
{
	CAnimation *pAnimation;
	pAnimation = new CAnimation;
	pAnimation->Init(pCharacter);
	pAnimation->LoadAnimation();

	return pAnimation;
}

void CAnimation::LoadAnimation(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad = 0;
	int	 nCntKey = 0;
	int	 nCntMotion = 0;

	char cReedText[1024];	// 文字として読み取り用
	char cHeadText[1024];	//
	char cDie[1024];

	// ファイル開
	pFile = fopen("./data/MODEL/motion_weapon.txt", "r");

	// 開けた
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
							sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_keyInfo[nCntMotion].nLoop);
						}

						if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_keyInfo[nCntMotion].nKeyNum);
						}

						if (strcmp(cHeadText, "KEYSET") == 0)
						{

							while (strcmp(cHeadText, "END_KEYSET") != 0)
							{

								fgets(cReedText, sizeof(cReedText), pFile);
								sscanf(cReedText, "%s", &cHeadText);

								if (strcmp(cHeadText, "FRAME") == 0)
								{
									sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_keyInfo[nCntMotion].nFrame[nCntKey]);
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
												&m_keyInfo[nCntMotion].parts[nCntLoad].pos[nCntKey].x,
												&m_keyInfo[nCntMotion].parts[nCntLoad].pos[nCntKey].y,
												&m_keyInfo[nCntMotion].parts[nCntLoad].pos[nCntKey].z);
										}

										if (strcmp(cHeadText, "ROT") == 0)
										{
											sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie,
												&m_keyInfo[nCntMotion].parts[nCntLoad].rot[nCntKey].x,
												&m_keyInfo[nCntMotion].parts[nCntLoad].rot[nCntKey].y,
												&m_keyInfo[nCntMotion].parts[nCntLoad].rot[nCntKey].z);
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
			// ファイル閉
			fclose(pFile);
		}
		// 開けない
		else
		{
			printf("開けれませんでした\n");
		}
	}
}
