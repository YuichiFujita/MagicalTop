//============================================================
//
//	�o�u������ [bubble.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "bubble.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "camera.h"
#include "model.h"

//************************************************************
//	�}�N����`
//************************************************************
#define BUBBLE_PRIO	(4)	// �o�u���\���̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CBubble::mc_apModelFile[] =	// ���f���萔
{
	"data\\MODEL\\BUBBLE\\bubble000.x",	// �o�u�����f��
};

//************************************************************
//	�q�N���X [CBubble] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBubble::CBubble(const int nMaxNum, const D3DXVECTOR3& rMaxScale) : CObjectModel(CObject::LABEL_NONE, BUBBLE_PRIO), m_maxScale(rMaxScale), m_nMaxNum(nMaxNum)
{
	// �����o�ϐ����N���A
	m_pParentObject = NULL;	// �e�I�u�W�F�N�g
	m_nHitNum = 0;			// �o�u���q�b�g��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CBubble::~CBubble()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBubble::Init(void)
{
	// �����o�ϐ���������
	m_pParentObject = NULL;	// �e�I�u�W�F�N�g
	m_nHitNum = 0;			// �o�u���q�b�g��

	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

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
void CBubble::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CBubble::Update(void)
{
	// TODO�F�|�C���^�̊m�F�����Ƃ��ꂢ��
	if (CObject::CheckUse(m_pParentObject))
	{ // �e�I�u�W�F�N�g���g�p����Ă����ꍇ

		// �ϐ���錾
		D3DXVECTOR3 posParent = m_pParentObject->GetPosition();	// �e�I�u�W�F�N�g�ʒu
		D3DXVECTOR3 posBubble = VEC3_ZERO;		// �o�u���ʒu
		D3DXVECTOR3 scaleBubble = VEC3_ZERO;	// �o�u���g�嗦

		// �o�u���̈ʒu�����߂�
		posBubble = posParent;	// �e�I�u�W�F�N�g�̍��W���
		posBubble.y += m_pParentObject->GetHeight() * 0.5f;	// �I�u�W�F�N�g�̏c���̔��������Z

		// �o�u���̊g�嗦�����߂�
		scaleBubble.x = m_nHitNum * (m_maxScale.x / (float)m_nMaxNum);
		scaleBubble.y = m_nHitNum * (m_maxScale.y / (float)m_nMaxNum);
		scaleBubble.z = m_nHitNum * (m_maxScale.z / (float)m_nMaxNum);

		// �ʒu��ݒ�
		SetPosition(posBubble);

		// �g�嗦��ݒ�
		SetScaling(scaleBubble);
	}

	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CBubble::Draw(void)
{
	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw();
}

//============================================================
//	��������
//============================================================
CBubble *CBubble::Create
( // ����
	CObject *pObject,				// �e�I�u�W�F�N�g
	const int nMaxNum,				// �ő�l
	const D3DXVECTOR3& rMaxScale,	// �ő�g�嗦
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rRot,		// ����
	const D3DXVECTOR3& rScale		// �g�嗦
)
{
	// �|�C���^��錾
	CModel *pModel = CManager::GetModel();	// ���f���ւ̃|�C���^
	CBubble *pBubble = NULL;	// �o�u�������p

	if (UNUSED(pBubble))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pBubble = new CBubble(nMaxNum, rMaxScale);	// �o�u��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pBubble))
	{ // �g�p����Ă���ꍇ
		
		// �o�u���̏�����
		if (FAILED(pBubble->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pBubble;
			pBubble = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// ���f����o�^�E����
		pBubble->BindModel(pModel->GetModel(pModel->Regist(mc_apModelFile[MODEL_NORMAL])));

		// �ʒu��ݒ�
		pBubble->SetPosition(rPos);

		// ������ݒ�
		pBubble->SetRotation(rRot);

		// �傫����ݒ�
		pBubble->SetScaling(rScale);

		// �e�I�u�W�F�N�g��ݒ�
		pBubble->SetParentObject(pObject);

		// �m�ۂ����A�h���X��Ԃ�
		return pBubble;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�q�b�g�����Z����
//============================================================
void CBubble::AddHitNum(const int nAdd)
{
	// �q�b�g�������������Z
	m_nHitNum += nAdd;

	// �͈͓�����
	useful::LimitNum(m_nHitNum, 0, m_nMaxNum);
}

//============================================================
//	�q�b�g���ݒ菈��
//============================================================
void CBubble::SetHitNum(const int nNum)
{
	// �����̒l���q�b�g���ɐݒ�
	m_nHitNum += nNum;

	// �͈͓�����
	useful::LimitNum(m_nHitNum, 0, m_nMaxNum);
}

//============================================================
//	�q�b�g���擾����
//============================================================
int CBubble::GetHitNum(void) const
{
	// �q�b�g����Ԃ�
	return m_nHitNum;
}

//============================================================
//	�e�I�u�W�F�N�g�̐ݒ菈��
//============================================================
void CBubble::SetParentObject(CObject *pObject)
{
	// �����̃I�u�W�F�N�g��ݒ�
	m_pParentObject = pObject;
}
