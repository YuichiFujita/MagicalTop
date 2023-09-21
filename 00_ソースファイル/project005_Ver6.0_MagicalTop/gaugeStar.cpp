//============================================================
//
//	���Q�[�W���� [gaugeStar.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gaugeStar.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "texture.h"
#include "sceneGame.h"
#include "objectBillboard.h"
#include "target.h"

//************************************************************
//	�}�N����`
//************************************************************
#define GAUGE_PRIO	(3)	// ���Q�[�W�̗D�揇��

#define GAUGE_BG_COL	(D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f))	// �Q�[�W�w�i�F
#define GAUGE_OVER_COL	(D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f))		// �Q�[�W�I�[�o�[�q�[�g�F

#define ADD_ROT	(0.05f)	// �������Z�p�����̉��Z��
#define REV_ROT	(0.1f)	// ���Z�����̕␳�W��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CGaugeStar::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\star000.png",	// ��
	"data\\TEXTURE\\star001.png",	// ���g
};

//************************************************************
//	�q�N���X [CGaugeStar] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGaugeStar::CGaugeStar(const int nHealNumGauge, const int nHealWait, const float fMaxRadius) : CObject(CObject::LABEL_GAUGE, GAUGE_PRIO),
m_nHealNumGauge(nHealNumGauge), m_nHealWait(nHealWait), m_fMaxRadius(fMaxRadius)
{
	// �����o�ϐ����N���A
	memset(&m_apStarFrame[0], 0, sizeof(m_apStarFrame));	// ���̘g���
	memset(&m_apStarBG[0], 0, sizeof(m_apStarBG));			// ���̔w�i���
	memset(&m_aStar[0], 0, sizeof(m_aStar));				// ���̏��

	m_state			= STATE_NORMAL;	// ���
	m_nMaxNumGauge	= 0;			// �ő�Q�[�W��
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nCurrentStar	= 0;			// ���ݏ���̐�
	m_bOverheat		= false;		// �I�[�o�[�q�[�g��
	m_pParentObject = NULL;			// �e�I�u�W�F�N�g
	m_posGap		= VEC3_ZERO;	// �\���ʒu�̉��Z��
	m_fDistance		= 0.0f;			// ���S����̋���
	m_fFlicker		= 0.0f;			// �h��߂���
	m_fSinRot		= 0.0f;			// ����������
	m_fAddRot		= 0.0f;			// ����̐������ւ̌��݉��Z��
	m_fDestAddRot	= 0.0f;			// ����̐������ւ̖ڕW���Z��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CGaugeStar::~CGaugeStar()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGaugeStar::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	memset(&m_apStarFrame[0], 0, sizeof(m_apStarFrame));	// ���̘g���
	memset(&m_apStarBG[0], 0, sizeof(m_apStarBG));			// ���̔w�i���
	memset(&m_aStar[0], 0, sizeof(m_aStar));				// ���̏��

	m_state			= STATE_NORMAL;	// ���
	m_nMaxNumGauge	= 0;			// �ő�Q�[�W��
	m_nCounterState = 0;			// ��ԊǗ��J�E���^�[
	m_nCurrentStar	= 0;			// ���ݏ���̐�
	m_bOverheat		= false;		// �I�[�o�[�q�[�g��
	m_pParentObject = NULL;			// �e�I�u�W�F�N�g
	m_posGap		= VEC3_ZERO;	// �\���ʒu�̉��Z��
	m_fDistance		= 0.0f;			// ���S����̋���
	m_fFlicker		= 0.0f;			// �h��߂���
	m_fSinRot		= 0.0f;			// ����������
	m_fAddRot		= 0.0f;			// ����̐������ւ̌��݉��Z��
	m_fDestAddRot	= 0.0f;			// ����̐������ւ̖ڕW���Z��

	for (int nCntStar = 0; nCntStar < MAX_STAR; nCntStar++)
	{ // ���̍ő吔���J��Ԃ�

		// ���̘g�̐���
		m_apStarFrame[nCntStar] = CObjectBillboard::Create
		( // ����
			VEC3_ZERO,	// �ʒu
			VEC3_ZERO	// �傫��
		);
		if (UNUSED(m_apStarFrame[nCntStar]))
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_apStarFrame[nCntStar]->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_FRAME]));

		// �D�揇�ʂ�ݒ�
		m_apStarFrame[nCntStar]->SetPriority(GAUGE_PRIO);

		// �F��ݒ�
		m_apStarFrame[nCntStar]->SetColor(XCOL_BLACK);

		// Z�e�X�g��ݒ�
		m_apStarFrame[nCntStar]->SetFunc(D3DCMP_LESSEQUAL);
	}

	for (int nCntStar = 0; nCntStar < MAX_STAR; nCntStar++)
	{ // ���̍ő吔���J��Ԃ�

		// ���̔w�i�̐���
		m_apStarBG[nCntStar] = CObjectBillboard::Create
		( // ����
			VEC3_ZERO,	// �ʒu
			VEC3_ZERO	// �傫��
		);
		if (UNUSED(m_apStarBG[nCntStar]))
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_apStarBG[nCntStar]->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_STAR]));

		// �D�揇�ʂ�ݒ�
		m_apStarBG[nCntStar]->SetPriority(GAUGE_PRIO);

		// �F��ݒ�
		m_apStarBG[nCntStar]->SetColor(GAUGE_BG_COL);

		// Z�e�X�g��ݒ�
		m_apStarBG[nCntStar]->SetFunc(D3DCMP_LESSEQUAL);

		// ���̐���
		m_aStar[nCntStar].pBillboard = CObjectBillboard::Create
		( // ����
			VEC3_ZERO,	// �ʒu
			VEC3_ZERO	// �傫��
		);
		if (UNUSED(m_aStar[nCntStar].pBillboard))
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_aStar[nCntStar].pBillboard->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_STAR]));

		// �D�揇�ʂ�ݒ�
		m_aStar[nCntStar].pBillboard->SetPriority(GAUGE_PRIO);

		// �F��ݒ�
		m_aStar[nCntStar].pBillboard->SetColor(XCOL_WHITE);

		// Z�e�X�g��ݒ�
		m_aStar[nCntStar].pBillboard->SetFunc(D3DCMP_LESSEQUAL);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGaugeStar::Uninit(void)
{
	for (int nCntStar = 0; nCntStar < MAX_STAR; nCntStar++)
	{ // ���̍ő吔���J��Ԃ�

		// ���̏I��
		m_aStar[nCntStar].pBillboard->Uninit();

		// ���̔w�i�̏I��
		m_apStarBG[nCntStar]->Uninit();

		// ���̘g�̏I��
		m_apStarFrame[nCntStar]->Uninit();
	}

	// ���g�̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CGaugeStar::Update(void)
{
	// �ϐ���錾
	float fDiffRot;	// ��������

	//--------------------------------------------------------
	//	�T�C���J�[�u�p�̌��������Z
	//--------------------------------------------------------
	// �������Z�p�̌��������Z
	m_fSinRot += ADD_ROT;

	// ������␳
	useful::NormalizeRot(m_fSinRot);

	//--------------------------------------------------------
	//	���Z�����̍X�V
	//--------------------------------------------------------
	// �����������v�Z
	fDiffRot = m_fDestAddRot - m_fAddRot;

	// ����������␳
	useful::NormalizeRot(fDiffRot);

	// ���Z�����̍X�V
	m_fAddRot += fDiffRot * REV_ROT;

	// ���Z������␳
	useful::NormalizeRot(m_fAddRot);

	//--------------------------------------------------------
	//	�Q�[�W����̍X�V
	//--------------------------------------------------------
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NORMAL:	// �ʏ���

		if (m_nCurrentStar > 0
		||  m_aStar[m_nCurrentStar].nNumGauge < m_nMaxNumGauge)
		{ // �Q�[�W�������Ă���ꍇ

			if (m_nCounterState < m_nHealWait)
			{ // �J�E���^�[���ҋ@�J�E���g��菬�����ꍇ

				// �J�E���^�[�����Z
				m_nCounterState++;
			}
			else
			{ // �J�E���^�[���ҋ@�J�E���g�ȏ�̏ꍇ

				// �J�E���^�[��������
				m_nCounterState = 0;

				// ��Ԃ�ݒ�
				m_state = STATE_HEAL;	// �Q�[�W�񕜏��
			}
		}

		break;

	case STATE_USAGE:	// �Q�[�W�g�p���

		if (m_aStar[MAX_STAR - 1].nNumGauge > 0)
		{ // �ő��������Ă��Ȃ��ꍇ

			// �Q�[�W�����炷
			m_aStar[m_nCurrentStar].nNumGauge--;

			// ���݂̃Q�[�W�ʂɉ��������̑傫���ɕύX
			m_aStar[m_nCurrentStar].fRadius = (m_fMaxRadius / (float)m_nMaxNumGauge) * m_aStar[m_nCurrentStar].nNumGauge;

			// ���̑傫����ݒ�
			m_aStar[m_nCurrentStar].pBillboard->SetScaling(D3DXVECTOR3(m_aStar[m_nCurrentStar].fRadius, m_aStar[m_nCurrentStar].fRadius, 0.0f));

			if (m_aStar[m_nCurrentStar].nNumGauge <= 0)
			{ // ���݂̐��Q�[�W���g���؂����ꍇ

				if (m_nCurrentStar < MAX_STAR - 1)
				{ // ������ő吔��菬�����ꍇ

					// ���̐��Ɉڍs
					m_nCurrentStar++;

					// �ڕW���Z������ݒ�
					m_fDestAddRot = ((D3DX_PI * 2) / MAX_STAR) * m_nCurrentStar;
					useful::NormalizeRot(m_fDestAddRot);	// ������␳
				}
				else
				{ // ����������ꍇ

					// �I�[�o�[�q�[�g������Ԃɂ���
					m_bOverheat = true;

					for (int nCntStar = 0; nCntStar < MAX_STAR; nCntStar++)
					{ // ���̍ő吔���J��Ԃ�

						// �F��ݒ�
						m_aStar[nCntStar].pBillboard->SetColor(GAUGE_OVER_COL);
					}
				}
			}
		}

		// �ʏ��Ԃɖ߂�
		m_state = STATE_NORMAL;

		break;

	case STATE_HEAL:	// �Q�[�W�񕜏��

		if (m_aStar[0].nNumGauge < m_nMaxNumGauge)
		{ // �ő�񕜂����Ă��Ȃ��ꍇ

			// �Q�[�W�𑝂₷
			m_aStar[m_nCurrentStar].nNumGauge += m_nHealNumGauge;

			// ���݂̃Q�[�W�ʂɉ��������̑傫���ɕύX
			m_aStar[m_nCurrentStar].fRadius = (m_fMaxRadius / (float)m_nMaxNumGauge) * m_aStar[m_nCurrentStar].nNumGauge;

			// ���̑傫����ݒ�
			m_aStar[m_nCurrentStar].pBillboard->SetScaling(D3DXVECTOR3(m_aStar[m_nCurrentStar].fRadius, m_aStar[m_nCurrentStar].fRadius, 0.0f));

			if (m_aStar[m_nCurrentStar].nNumGauge >= m_nMaxNumGauge)
			{ // ���݂̐��Q�[�W�����^���̏ꍇ

				// �Q�[�W��␳
				m_aStar[m_nCurrentStar].nNumGauge = m_nMaxNumGauge;

				if (m_nCurrentStar > 0)
				{ // ������ŏ������傫���ꍇ

					// �O�̐��Ɉڍs
					m_nCurrentStar--;

					// �ڕW���Z������ݒ�
					m_fDestAddRot = ((D3DX_PI * 2) / MAX_STAR) * m_nCurrentStar;
					useful::NormalizeRot(m_fDestAddRot);	// ������␳
				}
				else
				{ // �񕜂��������ꍇ

					// ��Ԃ�ݒ�
					m_state = STATE_NORMAL;	// �ʏ���

					// �I�[�o�[�q�[�g���Ă��Ȃ���Ԃɂ���
					m_bOverheat = false;

					for (int nCntStar = 0; nCntStar < MAX_STAR; nCntStar++)
					{ // ���̍ő吔���J��Ԃ�

						// �F��ݒ�
						m_aStar[nCntStar].pBillboard->SetColor(XCOL_WHITE);
					}
				}
			}
		}

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	//--------------------------------------------------------
	//	�Q�[�W�̕`��ݒ�
	//--------------------------------------------------------
	// �`�����ݒ�
	if (FAILED(SetDrawInfo()))
	{ // �`����̐ݒ�Ɏ��s�����ꍇ

		// �I�u�W�F�N�g�̏I��
		Uninit();

		// �֐��𔲂���
		return;
	}

	for (int nCntStar = 0; nCntStar < MAX_STAR; nCntStar++)
	{ // ���̍ő吔���J��Ԃ�

		// ���̍X�V
		m_aStar[nCntStar].pBillboard->Update();

		// ���̔w�i�̍X�V
		m_apStarBG[nCntStar]->Update();

		// ���̘g�̍X�V
		m_apStarFrame[nCntStar]->Update();
	}
}

//============================================================
//	�`�揈��
//============================================================
void CGaugeStar::Draw(void)
{

}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CGaugeStar::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);	// ���g

	for (int nCntStar = 0; nCntStar < MAX_STAR; nCntStar++)
	{ // ���̍ő吔���J��Ԃ�

		// ���̍X�V�󋵂�ݒ�
		m_aStar[nCntStar].pBillboard->SetEnableUpdate(bUpdate);

		// ���̔w�i�̍X�V�󋵂�ݒ�
		m_apStarBG[nCntStar]->SetEnableUpdate(bUpdate);

		// ���̘g�̍X�V�󋵂�ݒ�
		m_apStarFrame[nCntStar]->SetEnableUpdate(bUpdate);
	}
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CGaugeStar::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);	// ���g

	for (int nCntStar = 0; nCntStar < MAX_STAR; nCntStar++)
	{ // ���̍ő吔���J��Ԃ�

		// ���̕`��󋵂�ݒ�
		m_aStar[nCntStar].pBillboard->SetEnableDraw(bDraw);

		// ���̔w�i�̕`��󋵂�ݒ�
		m_apStarBG[nCntStar]->SetEnableDraw(bDraw);

		// ���̘g�̕`��󋵂�ݒ�
		m_apStarFrame[nCntStar]->SetEnableDraw(bDraw);
	}
}

//============================================================
//	��������
//============================================================
CGaugeStar *CGaugeStar::Create
(
	const int nMaxNumGauge,		// �ő�Q�[�W��
	const float fMaxRadius,		// ���a
	const int nHealNumGauge,	// �Q�[�W�񕜗�
	const int nHealWait,		// �񕜑ҋ@�J�E���g
	CObject *pObject,			// �e�I�u�W�F�N�g
	const D3DXVECTOR3& rGap,	// �\���ʒu�̉��Z��
	const float fDistance,		// ���S����̋���
	const float fFlicker		// �h��߂���
)
{
	// �|�C���^��錾
	CGaugeStar *pGaugeStar = NULL;	// ���Q�[�W�����p

	if (UNUSED(pGaugeStar))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pGaugeStar = new CGaugeStar(nHealNumGauge, nHealWait, fMaxRadius);	// ���Q�[�W
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pGaugeStar))
	{ // �m�ۂɐ������Ă���ꍇ

		// ���Q�[�W�̏�����
		if (FAILED(pGaugeStar->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return NULL;
		}

		// �e�I�u�W�F�N�g��ݒ�
		pGaugeStar->SetParentObject(pObject);

		// �ő�Q�[�W�ʂ�ݒ�
		pGaugeStar->SetMaxNumGauge(nMaxNumGauge);

		// �\���ʒu�̉��Z�ʂ�ݒ�
		pGaugeStar->SetGapPosition(rGap);

		// ������ݒ�
		pGaugeStar->SetStar(nMaxNumGauge, fMaxRadius);

		// ���S����̋�����ݒ�
		pGaugeStar->SetDistance(fDistance);

		// �h��߂��ʂ�ݒ�
		pGaugeStar->SetFlicker(fFlicker);

		// �m�ۂ����A�h���X��Ԃ�
		return pGaugeStar;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�Q�[�W�̎g�p����
//============================================================
bool CGaugeStar::UseGauge(void)
{
	if (!m_bOverheat)
	{ // �I�[�o�[�q�[�g���Ă��Ȃ��ꍇ

		if (m_aStar[MAX_STAR - 1].nNumGauge > 0)
		{ // �Q�[�W�c�ʂ�����ꍇ

			// �J�E���^�[��������
			m_nCounterState = 0;

			// ��Ԃ�ݒ�
			m_state = STATE_USAGE;	// �Q�[�W�g�p���

			// �g�p������Ԃ�Ԃ�
			return true;
		}
		else
		{ // �Q�[�W�c�ʂ��Ȃ��ꍇ

			// �g�p���Ă��Ȃ���Ԃ�Ԃ�
			return false;
		}
	}
	else
	{ // �I�[�o�[�q�[�g���Ă����ꍇ

		// �g�p���Ă��Ȃ���Ԃ�Ԃ�
		return false;
	}
}

//============================================================
//	�Q�[�W�̑S�񕜏���
//============================================================
void CGaugeStar::HealNumGauge(void)
{
	// �ڕW���Z������ݒ�
	m_fDestAddRot = ((D3DX_PI * 2) / MAX_STAR) * 0;
	useful::NormalizeRot(m_fDestAddRot);	// ������␳

	// �����擪�ɐݒ�
	m_nCurrentStar = 0;

	// ��ԊǗ��J�E���^�[��������
	m_nCounterState = 0;

	for (int nCntStar = 0; nCntStar < MAX_STAR; nCntStar++)
	{ // ���̍ő吔���J��Ԃ�

		// �Q�[�W���ő�l�ɕ␳
		m_aStar[nCntStar].nNumGauge = m_nMaxNumGauge;

		// ���݂̃Q�[�W�ʂɉ��������̑傫���ɕύX
		m_aStar[nCntStar].fRadius = (m_fMaxRadius / (float)m_nMaxNumGauge) * m_aStar[nCntStar].nNumGauge;

		// ���̑傫����ݒ�
		m_aStar[nCntStar].pBillboard->SetScaling(D3DXVECTOR3(m_aStar[nCntStar].fRadius, m_aStar[nCntStar].fRadius, 0.0f));

		// �F��ݒ�
		m_aStar[nCntStar].pBillboard->SetColor(XCOL_WHITE);
	}

	// ��Ԃ�ݒ�
	m_state = STATE_NORMAL;	// �ʏ���

	// �I�[�o�[�q�[�g���Ă��Ȃ���Ԃɂ���
	m_bOverheat = false;
}

//============================================================
//	�ő�Q�[�W�ʂ̐ݒ菈��
//============================================================
void CGaugeStar::SetMaxNumGauge(const int nMax)
{
	// �����̍ő�Q�[�W�ʂ�ݒ�
	m_nMaxNumGauge = nMax;

	// �Q�[�W�̑S��
	HealNumGauge();
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CGaugeStar::SetStar(const int nNumGauge, const float fRadius)
{
	for (int nCntStar = 0; nCntStar < MAX_STAR; nCntStar++)
	{ // ���̍ő吔���J��Ԃ�

		// ���̃Q�[�W�ʂ�ݒ�
		m_aStar[nCntStar].nNumGauge = nNumGauge;

		// ���̔��a��ݒ�
		m_aStar[nCntStar].fRadius = fRadius;

		// ���̑傫����ݒ�
		m_aStar[nCntStar].pBillboard->SetScaling(D3DXVECTOR3(fRadius, fRadius, 0.0f));

		// ���̔w�i�̑傫����ݒ�
		m_apStarBG[nCntStar]->SetScaling(D3DXVECTOR3(fRadius, fRadius, 0.0f));

		// ���̘g�̑傫����ݒ�
		m_apStarFrame[nCntStar]->SetScaling(D3DXVECTOR3(fRadius, fRadius, 0.0f));
	}
}

//============================================================
//	�\���ʒu�̉��Z�ʂ̐ݒ菈��
//============================================================
void CGaugeStar::SetGapPosition(const D3DXVECTOR3& rGap)
{
	// �����̕\���ʒu�̉��Z�ʂ�ݒ�
	m_posGap = rGap;
}

//============================================================
//	���S����̋����̐ݒ菈��
//============================================================
void CGaugeStar::SetDistance(const float fDistance)
{
	// �����̒��S����̋�����ݒ�
	m_fDistance = fDistance;
}

//============================================================
//	�h��߂��ʂ̐ݒ菈��
//============================================================
void CGaugeStar::SetFlicker(const float fFlicker)
{
	// �����̗h��߂��ʂ�ݒ�
	m_fFlicker = fFlicker;
}

//============================================================
//	�`����̐ݒ菈��
//============================================================
HRESULT CGaugeStar::SetDrawInfo(void)
{
	// TODO�F�|�C���^�̊m�F�����Ƃ��ꂢ��
	if (CObject::CheckUse(m_pParentObject))
	{ // �e�I�u�W�F�N�g���g�p����Ă����ꍇ

		// �ϐ���錾
		D3DXVECTOR3 posParent = m_pParentObject->GetPosition();		// �e�I�u�W�F�N�g�ʒu
		D3DXVECTOR3 posTarget = CScene::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu
		float fPartRot = (D3DX_PI * 2) / MAX_STAR;	// ��������

		// ���̕`�����ݒ�
		for (int nCntStar = 0; nCntStar < MAX_STAR; nCntStar++)
		{ // ���̍ő吔���J��Ԃ�

			// �ϐ���錾
			D3DXVECTOR3 posStar = posParent + m_posGap;	// �\���ʒu
			float fRot = (fPartRot * nCntStar);			// �\������

			// �J�����̌��������Z
			fRot += CManager::GetCamera()->GetRotation().y;
			useful::NormalizeRot(fRot);	// ������␳

			// ����̐��𐳖ʂɂ���
			fRot -= m_fAddRot;
			useful::NormalizeRot(fRot);	// ������␳

			// �\���ʒu�ɉ��Z
			posStar.x += sinf(fRot) * (m_fDistance + sinf(m_fSinRot) * m_fFlicker);
			posStar.z += cosf(fRot) * (m_fDistance + sinf(m_fSinRot) * m_fFlicker);

			// ���̈ʒu�ݒ�
			m_aStar[nCntStar].pBillboard->SetPosition(posStar);
			m_apStarBG[nCntStar]->SetPosition(posStar);
			m_apStarFrame[nCntStar]->SetPosition(posStar);
		}

		// ������Ԃ�
		return S_OK;
	}
	else
	{ // �e�I�u�W�F�N�g���g�p����Ă��Ȃ������ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}
}

//============================================================
//	�e�I�u�W�F�N�g�̐ݒ菈��
//============================================================
void CGaugeStar::SetParentObject(CObject *pObject)
{
	// �����̐e�I�u�W�F�N�g��ݒ�
	m_pParentObject = pObject;
}
