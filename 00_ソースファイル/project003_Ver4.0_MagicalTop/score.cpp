//============================================================
//
//	�X�R�A���� [score.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "score.h"
#include "manager.h"
#include "renderer.h"
#include "value.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SCO_POS		(D3DXVECTOR3(824.0f, 42.0f, 0.0f))		// �|���S���ʒu
#define SCO_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// �|���S���傫��
#define SCO_SPACE	(D3DXVECTOR3(SCO_SIZE.x, 0.0f, 0.0f))	// �|���S���Ԃ̋�

#define SCO_NUMMIN	(0)			// �ŏ��X�R�A
#define SCO_NUMMAX	(99999999)	// �ő�X�R�A

//************************************************************
//	�q�N���X [CScore] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScore::CScore() : CObject(CObject::LABEL_NUMBER, DEFAULT_PRIO)
{
	// �����o�ϐ����N���A
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// ���l�̏��
	m_nNum = 0;	// �X�R�A
}

//============================================================
//	�f�X�g���N�^
//============================================================
CScore::~CScore()
{

}

//============================================================
//	����������
//============================================================
HRESULT CScore::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// ���l�̏��
	m_nNum = 0;	// �X�R�A

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{ // �X�R�A�̌������J��Ԃ�

		// �����̐���
		m_apValue[nCntScore] = CValue::Create(CValue::TEXTURE_NORMAL, SCO_POS + (SCO_SPACE * (float)nCntScore), SCO_SIZE);
		if (UNUSED(m_apValue[nCntScore]))
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CScore::Uninit(void)
{
	// �����I�u�W�F�N�g��j��
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{ // �X�R�A�̌������J��Ԃ�

		// �����̏I��
		m_apValue[nCntScore]->Uninit();
	}

	// ���g�̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CScore::Update(void)
{
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{ // �X�R�A�̌������J��Ԃ�

		// �����̍X�V
		m_apValue[nCntScore]->Update();
	}
}

//============================================================
//	�`�揈��
//============================================================
void CScore::Draw(void)
{

}

//============================================================
//	��������
//============================================================
CScore *CScore::Create(void)
{
	// �|�C���^��錾
	CScore *pScore = NULL;		// �X�R�A�����p

	if (UNUSED(pScore))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pScore = new CScore;	// �X�R�A
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pScore))
	{ // �m�ۂɐ������Ă���ꍇ

		// �X�R�A�̏�����
		if (FAILED(pScore->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pScore;
			pScore = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pScore;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	���Z����
//============================================================
void CScore::Add(const int nNum)
{
	// �X�R�A�����������Z
	m_nNum += nNum;

	// �X�R�A�̕␳
	useful::LimitNum(m_nNum, SCO_NUMMIN, SCO_NUMMAX);

	// �����̃e�N�X�`�����W�̐ݒ�
	SetTexNum();
}

//============================================================
//	�ݒ菈��
//============================================================
void CScore::Set(const int nNum)
{
	// �X�R�A�������̒l�ɐݒ�
	m_nNum = nNum;

	// �X�R�A�̕␳
	useful::LimitNum(m_nNum, SCO_NUMMIN, SCO_NUMMAX);

	// �����̃e�N�X�`�����W�̐ݒ�
	SetTexNum();
}

//============================================================
//	�擾����
//============================================================
int CScore::Get(void)
{
	// �X�R�A�̒l��Ԃ�
	return m_nNum;
}

//============================================================
//	�����̃e�N�X�`�����W�̐ݒ菈��
//============================================================
void CScore::SetTexNum(void)
{
	// �ϐ���錾
	int aNumDivide[MAX_SCORE];	// �������Ƃ̕���p

	// �X�R�A���������Ƃɕ���
	useful::DivideDigitNum
	( // ����
		&aNumDivide[0],	// �������ʂ̊i�[�z��
		m_nNum,			// �������鐔�l
		MAX_SCORE		// �������鐔���̌���
	);

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{ // �X�R�A�̌������J��Ԃ�

		// �����̐ݒ�
		m_apValue[nCntScore]->SetNumber(aNumDivide[nCntScore]);
	}
}
