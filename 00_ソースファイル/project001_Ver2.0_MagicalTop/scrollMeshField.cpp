//============================================================
//
//	�X�N���[�����b�V���t�B�[���h���� [scrollMeshField.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scrollMeshField.h"

//************************************************************
//	�q�N���X [CScrollMeshField] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScrollMeshField::CScrollMeshField()
{
	// �����o�ϐ����N���A
	m_fTexU  = 0.0f;	// �e�N�X�`�������W�̊J�n�ʒu
	m_fTexV  = 0.0f;	// �e�N�X�`���c���W�̊J�n�ʒu
	m_fMoveU = 0.0f;	// �e�N�X�`�������W�̈ړ���
	m_fMoveV = 0.0f;	// �e�N�X�`���c���W�̈ړ���
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CScrollMeshField::CScrollMeshField(const CObject::LABEL label, const int nPriority) : CObjectMeshField(label, nPriority)
{
	// �����o�ϐ����N���A
	m_fTexU  = 0.0f;	// �e�N�X�`�������W�̊J�n�ʒu
	m_fTexV  = 0.0f;	// �e�N�X�`���c���W�̊J�n�ʒu
	m_fMoveU = 0.0f;	// �e�N�X�`�������W�̈ړ���
	m_fMoveV = 0.0f;	// �e�N�X�`���c���W�̈ړ���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CScrollMeshField::~CScrollMeshField()
{

}

//============================================================
//	����������
//============================================================
HRESULT CScrollMeshField::Init(void)
{
	// �����o�ϐ���������
	m_fTexU  = 0.0f;	// �e�N�X�`�������W�̊J�n�ʒu
	m_fTexV  = 0.0f;	// �e�N�X�`���c���W�̊J�n�ʒu
	m_fMoveU = 0.0f;	// �e�N�X�`�������W�̈ړ���
	m_fMoveV = 0.0f;	// �e�N�X�`���c���W�̈ړ���

	if (FAILED(CObjectMeshField::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	���������� (�I�[�o�[���[�h)
//============================================================
HRESULT CScrollMeshField::Init(const float fMoveU, const float fMoveV)
{
	// �����o�ϐ���������
	m_fTexU  = 0.0f;	// �e�N�X�`�������W�̊J�n�ʒu
	m_fTexV  = 0.0f;	// �e�N�X�`���c���W�̊J�n�ʒu
	m_fMoveU = fMoveU;	// �e�N�X�`�������W�̈ړ���
	m_fMoveV = fMoveV;	// �e�N�X�`���c���W�̈ړ���

	if (FAILED(CObjectMeshField::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CScrollMeshField::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObjectMeshField::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CScrollMeshField::Update(void)
{
	// �X�N���[�������Z
	m_fTexU += m_fMoveU;
	m_fTexV += m_fMoveV;

	if (m_fTexU > 1.0f)
	{ // 1.0f���傫���Ȃ����ꍇ

		// �J�n�n�_��␳
		m_fTexU -= 1.0f;
	}

	if (m_fTexV > 1.0f)
	{ // 1.0f���傫���Ȃ����ꍇ

		// �J�n�n�_��␳
		m_fTexV -= 1.0f;
	}

	// �I�u�W�F�N�g2D�̍X�V
	CObjectMeshField::Update();

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�`�揈��
//============================================================
void CScrollMeshField::Draw(void)
{
	// �I�u�W�F�N�g2D�̕`��
	CObjectMeshField::Draw();
}

//============================================================
//	��������
//============================================================
CScrollMeshField *CScrollMeshField::Create
(
	const float fMoveU,			// �����W�̈ړ���
	const float fMoveV,			// �c���W�̈ړ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR2& rSize,	// �傫��
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const D3DCULL cull,			// �J�����O��
	const bool bLight			// ���C�e�B���O��
)
{
	// �|�C���^��錾
	CScrollMeshField *pScrollMeshField = NULL;	// �X�N���[�����b�V���t�B�[���h�����p

	if (UNUSED(pScrollMeshField))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pScrollMeshField = new CScrollMeshField;	// �X�N���[�����b�V���t�B�[���h
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pScrollMeshField))
	{ // �m�ۂɐ������Ă���ꍇ

		// �X�N���[�����b�V���t�B�[���h�̏�����
		if (FAILED(pScrollMeshField->Init(fMoveU, fMoveV)))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pScrollMeshField;
			pScrollMeshField = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pScrollMeshField->SetPosition(rPos);

		// ������ݒ�
		pScrollMeshField->SetRotation(rRot);

		// �傫����ݒ�
		pScrollMeshField->SetScalingBiaxial(rSize);

		// �F��ݒ�
		pScrollMeshField->SetColor(rCol);

		// �J�����O��ݒ�
		pScrollMeshField->SetCulling(cull);

		// ���C�e�B���O��ݒ�
		pScrollMeshField->SetLighting(bLight);

		// ��������ݒ�
		if (FAILED(pScrollMeshField->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pScrollMeshField;
			pScrollMeshField = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pScrollMeshField;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CScrollMeshField::SetPosition(const D3DXVECTOR3& rPos)
{
	// �ʒu�̐ݒ�
	CObjectMeshField::SetPosition(rPos);

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CScrollMeshField::SetRotation(const D3DXVECTOR3& rRot)
{
	// �����̐ݒ�
	CObjectMeshField::SetRotation(rRot);

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CScrollMeshField::SetScalingBiaxial(const D3DXVECTOR2& rSize)
{
	// �傫���̐ݒ�
	CObjectMeshField::SetScalingBiaxial(rSize);

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CScrollMeshField::SetColor(const D3DXCOLOR& rCol)
{
	// �F�̐ݒ�
	CObjectMeshField::SetColor(rCol);

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�������̐ݒ菈��
//============================================================
HRESULT CScrollMeshField::SetPattern(const POSGRID2& rPart)
{
	// �������̐ݒ�
	if (FAILED(CObjectMeshField::SetPattern(rPart)))
	{ // �������̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�����W�̊J�n�ʒu�ݒ菈��
//============================================================
void CScrollMeshField::SetTexU(const float fTexU)
{
	// �����̉����W�̊J�n�ʒu����
	m_fTexU = fTexU;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�c���W�̊J�n�ʒu�ݒ菈��
//============================================================
void CScrollMeshField::SetTexV(const float fTexV)
{
	// �����̏c���W�̊J�n�ʒu����
	m_fTexV = fTexV;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�����W�̈ړ��ʐݒ菈��
//============================================================
void CScrollMeshField::SetMoveU(const float fMoveU)
{
	// �����̉����W�̈ړ��ʂ���
	m_fMoveU = fMoveU;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�c���W�̈ړ��ʐݒ菈��
//============================================================
void CScrollMeshField::SetMoveV(const float fMoveV)
{
	// �����̏c���W�̈ړ��ʂ���
	m_fMoveV = fMoveV;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObjectMeshField::SetScrollTex(m_fTexU, m_fTexV);
}
