//============================================================
//
//	�^�C�}�[�}�l�[�W���[���� [timerManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "timerManager.h"
#include "manager.h"
#include "renderer.h"
#include "value.h"

#ifdef _DEBUG	// �f�o�b�O����

#include "input.h"

#endif	// _DEBUG

//************************************************************
//	�}�N����`
//************************************************************
#define TIME_POS	(D3DXVECTOR3(36.0f, 42.0f, 0.0f))		// �|���S���ʒu
#define TIME_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// �|���S���傫��
#define TIME_SPACE	(D3DXVECTOR3(TIME_SIZE.x, 0.0f, 0.0f))	// �|���S���Ԃ̋�

#define TIME_NUMMIN	((DWORD)0)			// �ŏ��^�C��
#define TIME_NUMMAX	((DWORD)99 * 60000)	// �ő�^�C��

//************************************************************
//	�q�N���X [CTimerManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTimerManager::CTimerManager()
{
	// �����o�ϐ����N���A
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// ���l�̏��
	m_dwStartTime	= 0;			// �J�n����
	m_dwTime		= 0;			// �o�ߎ���
	m_dwStopTime	= 0;			// ��~����
	m_state			= STATE_NONE;	// �v�����
	m_bStop			= true;			// �v����~��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTimerManager::~CTimerManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTimerManager::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// ���l�̏��
	m_dwStartTime	= 0;			// �J�n����
	m_dwTime		= 0;			// �o�ߎ���
	m_dwStopTime	= 0;			// ��~����
	m_state			= STATE_NONE;	// �v�����
	m_bStop			= true;			// �v����~��

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{ // �^�C�}�[�̌������J��Ԃ�

		// �����̐���
		m_apValue[nCntTimer] = CValue::Create(CValue::TEXTURE_NORMAL, TIME_POS + (TIME_SPACE * (float)nCntTimer), TIME_SIZE);
		if (UNUSED(m_apValue[nCntTimer]))
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
void CTimerManager::Uninit(void)
{
	// �����I�u�W�F�N�g��j��
	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{ // �^�C�}�[�̌������J��Ԃ�

		// �����̏I��
		m_apValue[nCntTimer]->Uninit();
	}
}

//============================================================
//	�X�V����
//============================================================
void CTimerManager::Update(void)
{
#ifdef _DEBUG	// �f�o�b�O����

	if (CManager::GetKeyboard()->GetTrigger(DIK_F2))
	{
		// ��~�󋵂��t�]
		EnableStop((m_bStop) ? false : true);
	}

#if 0
	if (CManager::GetKeyboard()->GetTrigger(DIK_F3))
	{
		// �^�C�}�[�����Z
		AddMSec(1);
	}
	if (CManager::GetKeyboard()->GetTrigger(DIK_F4))
	{
		// �^�C�}�[�����Z
		AddSec(1);
	}
	if (CManager::GetKeyboard()->GetTrigger(DIK_F5))
	{
		// �^�C�}�[�����Z
		AddMin(1);
	}
#endif

#endif	// _DEBUG

	switch (m_state)
	{ // �v����Ԃ��Ƃ̏���
	case STATE_NONE:

		break;

	case STATE_MEASURE:

		if (m_bStop == false)
		{ // �v�����̏ꍇ

			// ���݂̌v���~���b��ݒ�
			m_dwTime = timeGetTime() - m_dwStopTime - m_dwStartTime;

			// �^�C�}�[�̕␳
			useful::LimitNum(m_dwTime, TIME_NUMMIN, TIME_NUMMAX);
		}
		else
		{ // �v����~���̏ꍇ

			// ���݂̒�~�~���b��ݒ�
			m_dwStopTime = timeGetTime() - m_dwTime - m_dwStartTime;

			// �^�C�}�[�̕␳
			useful::LimitNum(m_dwTime, TIME_NUMMIN, TIME_NUMMAX);
		}

		break;

	case STATE_END:

		// ����

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �����̃e�N�X�`�����W�̐ݒ�
	SetTexNum();

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{ // �^�C�}�[�̌������J��Ԃ�

		// �����̍X�V
		m_apValue[nCntTimer]->Update();
	}

	// �f�o�b�O�\��
	CManager::GetDebugProc()->Print("�^�C�}�[�F[%d:%d:%d]\n", m_dwTime / 60000, (m_dwTime / 1000) % 60, m_dwTime % 1000);
	CManager::GetDebugProc()->Print("��~�^�C�}�[�F[%d:%d:%d]\n", m_dwStopTime / 60000, (m_dwStopTime / 1000) % 60, m_dwStopTime % 1000);
}

//============================================================
//	��������
//============================================================
CTimerManager *CTimerManager::Create(void)
{
	// �|�C���^��錾
	CTimerManager *pTimerManager = NULL;	// �^�C�}�[�}�l�[�W���[�����p

	if (UNUSED(pTimerManager))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pTimerManager = new CTimerManager;	// �^�C�}�[�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pTimerManager))
	{ // �m�ۂɐ������Ă���ꍇ

		// �^�C�}�[�}�l�[�W���[�̏�����
		if (FAILED(pTimerManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pTimerManager;
			pTimerManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pTimerManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CTimerManager::Release(CTimerManager *&prTimerManager)
{
	if (USED(prTimerManager))
	{ // �g�p���̏ꍇ

		// �^�C�}�[�}�l�[�W���[�̏I��
		prTimerManager->Uninit();

		// �������J��
		delete prTimerManager;
		prTimerManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�v���J�n����
//============================================================
void CTimerManager::Start(void)
{
	if (m_state != STATE_MEASURE)
	{ // �^�C���̌v�����ł͂Ȃ��ꍇ

		// �^�C�}�[��ݒ�
		m_dwStartTime = timeGetTime();	// ���ݎ������擾

		// ���~��Ԃɂ���
		EnableStop(false);

		// �v���J�n��Ԃɂ���
		m_state = STATE_MEASURE;
	}
}

//============================================================
//	�v���I������
//============================================================
void CTimerManager::End(void)
{
	// �v���I����Ԃɂ���
	m_state = STATE_END;
}

//============================================================
//	�v����~�̗L�������̐ݒ菈��
//============================================================
void CTimerManager::EnableStop(const bool bStop)
{
	// �����̒�~�󋵂���
	m_bStop = bStop;
}

#if 0
//============================================================
//	�~���b�̉��Z����
//============================================================
void CTimerManager::AddMSec(const int nMSec)
{
	// �����̃~���b�����Z
	m_dwTime += nMSec;

	// �^�C�}�[�̕␳
	useful::LimitNum(m_dwTime, TIME_NUMMIN, TIME_NUMMAX);

	// �����̃e�N�X�`�����W�̐ݒ�
	SetTexNum();
}

//============================================================
//	�b�̉��Z����
//============================================================
void CTimerManager::AddSec(const int nSec)
{
	// �����̕b�����Z
	m_dwTime += nSec * 1000;	// �~���b�ɕϊ�

	// �^�C�}�[�̕␳
	useful::LimitNum(m_dwTime, TIME_NUMMIN, TIME_NUMMAX);

	// �����̃e�N�X�`�����W�̐ݒ�
	SetTexNum();
}

//============================================================
//	���̉��Z����
//============================================================
void CTimerManager::AddMin(const int nMin)
{
	// �����̕������Z
	m_dwTime += nMin * 60000;	// �~���b�ɕϊ�

	// �^�C�}�[�̕␳
	useful::LimitNum(m_dwTime, TIME_NUMMIN, TIME_NUMMAX);

	// �����̃e�N�X�`�����W�̐ݒ�
	SetTexNum();
}
#endif

//============================================================
//	�~���b�̎擾����
//============================================================
int CTimerManager::GetMSec(void)
{
	// �~���b��Ԃ�
	return m_dwTime % 1000;
}

//============================================================
//	�b�̎擾����
//============================================================
int CTimerManager::GetSec(void)
{
	// �b��Ԃ�
	return (m_dwTime / 1000) % 60;
}

//============================================================
//	���̎擾����
//============================================================
int CTimerManager::GetMin(void)
{
	// ����Ԃ�
	return m_dwTime / 60000;
}

//============================================================
//	�����̃e�N�X�`�����W�̐ݒ菈��
//============================================================
void CTimerManager::SetTexNum(void)
{
	// �ϐ���錾
	int aNumDivide[MAX_TIMER];	// ���̌������Ƃ̕���p

	// �����������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[0], GetMin(), MAX_MIN);

	// �b���������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[MAX_MIN], GetSec(), MAX_SEC);

	// �~���b���������Ƃɕ���
	useful::DivideDigitNum(&aNumDivide[MAX_MIN + MAX_SEC], GetMSec(), MAX_MSEC);

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{ // �^�C�}�[�̌������J��Ԃ�

		// �����̐ݒ�
		m_apValue[nCntTimer]->SetNumber(aNumDivide[nCntTimer]);
	}
}
