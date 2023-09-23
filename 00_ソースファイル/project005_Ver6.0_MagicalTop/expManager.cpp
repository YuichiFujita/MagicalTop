//============================================================
//
//	�o���l�}�l�[�W���[���� [expManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "expManager.h"
#include "manager.h"
#include "sound.h"
#include "objectGauge2D.h"
#include "multiValue.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_EXP			(100)	// �ő�o���l
#define EXP_FRAME		(10)	// �o���l�ϓ��t���[��
#define GAUGE_POS		(D3DXVECTOR3(SCREEN_CENT.x, 680.0f, 0.0f))	// �ʒu
#define GAUGE_GAUGESIZE	(D3DXVECTOR3(600.0f, 10.0f, 0.0f))			// �Q�[�W�傫��
#define GAUGE_FRONTCOL	(D3DXCOLOR(0.0f, 0.8f, 0.35f, 1.0f))		// �\�Q�[�W�F
#define GAUGE_BACKCOL	(D3DXCOLOR(0.0f, 0.1f, 0.2f, 1.0f))			// ���Q�[�W�F
#define GAUGE_FRAMESIZE	(D3DXVECTOR3(602.0f, 12.0f, 0.0f))			// �Q�[�W�g�傫��

#define FIRST_LV	(0)	// ���x�������l
#define LV_DIGIT	(2)	// ���x������
#define LV_SIZE		(D3DXVECTOR3(55.0f, 65.0f, 0.0f))	// ���l�傫��
#define LV_SPACE	(D3DXVECTOR3(55.0f, 0.0f, 0.0f))	// �s��
#define LV_POS		(D3DXVECTOR3(SCREEN_CENT.x - (LV_SPACE.x * 0.5f), 655.0f, 0.0f))	// �ʒu

//************************************************************
//	�e�N���X [CExpManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CExpManager::CExpManager()
{
	// �����o�ϐ����N���A
	m_pExp = NULL;		// �o���l�̏��
	m_pLevel = NULL;	// ���x���̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CExpManager::~CExpManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CExpManager::Init(void)
{
	// �����o�ϐ���������
	m_pExp = NULL;		// �o���l�̏��
	m_pLevel = NULL;	// ���x���̏��

	// �o���l�̐���
	m_pExp = CObjectGauge2D::Create
	( // ����
		CObject::LABEL_GAUGE,		// �I�u�W�F�N�g���x��
		MAX_EXP,					// �ő�o���l
		EXP_FRAME,					// �o���l�ϓ��t���[��
		GAUGE_POS,					// �ʒu
		GAUGE_GAUGESIZE,			// �Q�[�W�傫��
		GAUGE_FRONTCOL,				// �\�Q�[�W�F
		GAUGE_BACKCOL,				// ���Q�[�W�F
		true,						// �g�`���
		CObjectGauge2D::TYPE_EXP,	// �g���
		GAUGE_FRAMESIZE				// �g�傫��
	);
	if (UNUSED(m_pExp))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �Q�[�W�̐ݒ�
	m_pExp->SetNum(0);

	// ���x���̐���
	m_pLevel = CMultiValue::Create
	( // ����
		CValue::TEXTURE_MAGIC_GREEN,	// �e�N�X�`��
		FIRST_LV,	// ���x�������l
		LV_DIGIT,	// ���x������
		LV_POS,		// �ʒu
		LV_SIZE,	// ���l�傫��
		LV_SPACE	// �s��
	);
	if (UNUSED(m_pLevel))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CExpManager::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CExpManager::Update(void)
{

}

//============================================================
//	���x���̉��Z����
//============================================================
void CExpManager::AddLevel(const int nAdd)
{
	// ���������x�������Z
	m_pLevel->AddNum(nAdd);
}

//============================================================
//	�o���l�̉��Z����
//============================================================
void CExpManager::AddExp(const int nAdd)
{
	// �ϐ���錾
	int nCurrentExp = m_pExp->GetNum();	// ���݂̌o���l

	if (nCurrentExp + nAdd >= MAX_EXP)
	{ // �o���l���I�[�o�[����ꍇ

		// ���x�������Z
		m_pLevel->AddNum(1);

		// �o���l�̂��܂��ݒ�
		m_pExp->SetNum((nCurrentExp + nAdd) - MAX_EXP);

		// �T�E���h�̍Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_LEVELUP);	// ���x���A�b�v��
	}
	else
	{ // �o���l���I�[�o�[���Ȃ��ꍇ

		// �o���l��ݒ�
		m_pExp->SetNum(nCurrentExp + nAdd);
	}

	// ��O����
	assert(nAdd <= MAX_EXP + 1);	// ���Z�ʃI�[�o�[
}

//============================================================
//	���x���擾����
//============================================================
int CExpManager::GetLevel(void) const
{
	// ���x����Ԃ�
	return m_pLevel->GetNum();
}

//============================================================
//	�o���l�`��󋵂̐ݒ菈��
//============================================================
void CExpManager::SetEnableExpDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	m_pExp->SetEnableDraw(bDraw);	// �Q�[�W
	m_pLevel->SetEnableDraw(bDraw);	// ���l
}

//============================================================
//	��������
//============================================================
CExpManager *CExpManager::Create()
{
	// �|�C���^��錾
	CExpManager *pExpManager = NULL;	// �o���l�}�l�[�W���[�����p

	if (UNUSED(pExpManager))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pExpManager = new CExpManager;	// �o���l�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pExpManager))
	{ // �g�p����Ă���ꍇ
		
		// �o���l�}�l�[�W���[�̏�����
		if (FAILED(pExpManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pExpManager;
			pExpManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pExpManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CExpManager::Release(CExpManager *&prExpManager)
{
	if (USED(prExpManager))
	{ // �g�p���̏ꍇ

		// �o���l�}�l�[�W���[�̏I��
		prExpManager->Uninit();

		// �������J��
		delete prExpManager;
		prExpManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}
