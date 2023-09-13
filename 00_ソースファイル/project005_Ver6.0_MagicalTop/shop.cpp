//============================================================
//
//	�V���b�v���� [shop.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shop.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "anim2D.h"
#include "valueUI.h"
#include "multiValue.h"

//************************************************************
//	�}�N����`
//************************************************************
#define PTRN_WIDTH	(BUY_MAX)	// �e�N�X�`���̉�������
#define PTRN_HEIGHT	(1)			// �e�N�X�`���̏c������

#define ICON_SIZE	(D3DXVECTOR3(120.0f, 120.0f, 0.0f))	// �A�C�R���̑傫��
#define ICON_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))	// �A�C�R���̐F

#define LIFE_SPACE			(D3DXVECTOR3(110.0f, -4.0f, 0.0f))	// ���C�t�̍s��
#define LIFE_TITLE_SIZE		(D3DXVECTOR3(180.0f, 50.0f, 0.0f))	// ���C�t�̃^�C�g���傫��
#define LIFE_VALUE_SIZE		(D3DXVECTOR3(40.0f, 50.0f, 0.0f))	// ���C�t�̐����傫��
#define LIFE_VALUE_SPACE	(D3DXVECTOR3(40.0f, 0.0f, 0.0f))	// ���C�t�̐����̍s��
#define LIFE_VALUE_DIGIT	(3)	// ���C�t�̐����̌���

#define LV_SPACE		(D3DXVECTOR3(130.0f, -4.0f, 0.0f))	// ���x���̍s��
#define LV_TITLE_SIZE	(D3DXVECTOR3(180.0f, 50.0f, 0.0f))	// ���x���̃^�C�g���傫��
#define LV_VALUE_SIZE	(D3DXVECTOR3(40.0f, 50.0f, 0.0f))	// ���x���̐����傫��
#define LV_VALUE_SPACE	(D3DXVECTOR3(40.0f, 0.0f, 0.0f))	// ���x���̐����̍s��
#define LV_VALUE_DIGIT	(2)	// ���x���̐����̌���

#define ADD_POS_LIFE	(D3DXVECTOR3(-50.0f, -140.0f, 0.0f))	// �̗͂̕\���ʒu�̑��΍��W�ւ̉��Z��
#define ADD_POS_LEVEL	(D3DXVECTOR3(-50.0f, -90.0f, 0.0f))		// ���x���̕\���ʒu�̑��΍��W�ւ̉��Z��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CShop::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\shop002.png",	// �̗̓e�N�X�`��
	"data\\TEXTURE\\shop003.png",	// ���x���e�N�X�`��
};

//************************************************************
//	�q�N���X [CShop] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CShop::CShop() : CObject(CObject::LABEL_NONE, DEFAULT_PRIO)
{
	// �����o�ϐ����N���A
	m_pIcon = NULL;			// �w���i�A�C�R��
	m_pLife = NULL;			// �K�v�̗�
	m_pLv = NULL;			// �K�v���x��
	m_pos = VEC3_ZERO;		// �ʒu
	m_col = XCOL_WHITE;		// �F
	m_buy = BUY_LV1_FIRE;	// �w���i
}

//============================================================
//	�f�X�g���N�^
//============================================================
CShop::~CShop()
{

}

//============================================================
//	����������
//============================================================
HRESULT CShop::Init(void)
{
	// �����o�ϐ���������
	m_pIcon = NULL;			// �w���i�A�C�R��
	m_pLife = NULL;			// �K�v�̗�
	m_pLv = NULL;			// �K�v���x��
	m_pos = VEC3_ZERO;		// �ʒu
	m_col = XCOL_WHITE;		// �F
	m_buy = BUY_LV1_FIRE;	// �w���i

	// ��O����
	assert(false);	// �g�p���Ȃ�������

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	���������� (�I�[�o�[���[�h)
//============================================================
HRESULT CShop::Init(const BUY buy)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`��

	// �����o�ϐ���������
	m_pIcon = NULL;		// �w���i�A�C�R��
	m_pLife = NULL;		// �K�v�̗�
	m_pLv = NULL;		// �K�v���x��
	m_pos = VEC3_ZERO;	// �ʒu
	m_col = XCOL_WHITE;	// �F
	m_buy = buy;		// �w���i

	//--------------------------------------------------------
	//	�w���i�A�C�R���̐����E�ݒ�
	//--------------------------------------------------------
	// �w���i�A�C�R���̐���
	m_pIcon = CAnim2D::Create
	( // ����
		PTRN_WIDTH,		// ��������
		PTRN_HEIGHT,	// �c������
		VEC3_ZERO,		// �ʒu
		ICON_SIZE		// �傫��
	);
	if (UNUSED(m_pIcon))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �F��ݒ�
	m_pIcon->SetColor(ICON_COL);

	// �p�^�[����ݒ�
	m_pIcon->SetPattern(m_buy);

	//--------------------------------------------------------
	//	�K�v�̗͂̐����E�ݒ�
	//--------------------------------------------------------
	// �K�v�̗͂̐���
	m_pLife = CValueUI::Create
	( // ����
		VEC3_ZERO,			// �ʒu
		LIFE_SPACE,			// �s��
		LIFE_TITLE_SIZE,	// �^�C�g���傫��
		LIFE_VALUE_SIZE,	// �����傫��
		pTexture->Regist(mc_apTextureFile[TEXTURE_LIFE])
	);
	if (UNUSED(m_pLife))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����̍s�Ԃ�ݒ�
	m_pLife->GetMultiValue()->SetSpace(LIFE_VALUE_SPACE);

	// �����̌�����ݒ�
	m_pLife->GetMultiValue()->SetDigit(LIFE_VALUE_DIGIT);

	//--------------------------------------------------------
	//	�K�v���x���̐����E�ݒ�
	//--------------------------------------------------------
	// �K�v���x���̐���
	m_pLv = CValueUI::Create
	( // ����
		VEC3_ZERO,		// �ʒu
		LV_SPACE,		// �s��
		LV_TITLE_SIZE,	// �^�C�g���傫��
		LV_VALUE_SIZE,	// �����傫��
		pTexture->Regist(mc_apTextureFile[TEXTURE_LV])
	);
	if (UNUSED(m_pLv))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����̍s�Ԃ�ݒ�
	m_pLv->GetMultiValue()->SetSpace(LV_VALUE_SPACE);

	// �����̌�����ݒ�
	m_pLv->GetMultiValue()->SetDigit(LV_VALUE_DIGIT);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CShop::Uninit(void)
{
	// �A�j���[�V����2D����j��
	m_pIcon->Uninit();

	// ����UI����j��
	m_pLife->Uninit();
	m_pLv->Uninit();

	// ���g�̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CShop::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CShop::Draw(void)
{

}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CShop::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);	// ���g
	m_pIcon->SetEnableDraw(bDraw);	// �w���i�A�C�R��
	m_pLife->SetEnableDraw(bDraw);	// �K�v�̗�
	m_pLv->SetEnableDraw(bDraw);	// �K�v���x��
}

//============================================================
//	�D�揇�ʂ̐ݒ菈��
//============================================================
void CShop::SetPriority(const int nPriority)
{
	// �����̗D�揇�ʂ�ݒ�
	CObject::SetPriority(nPriority);	// ���g
	m_pIcon->SetPriority(nPriority);	// �w���i�A�C�R��
	m_pLife->SetPriority(nPriority);	// �K�v�̗�
	m_pLv->SetPriority(nPriority);		// �K�v���x��
}

//============================================================
//	��������
//============================================================
CShop *CShop::Create
(
	const BUY buy,			// �w���i
	const D3DXVECTOR3& rPos	// �ʒu
)
{
	// �|�C���^��錾
	CShop *pShop = NULL;	// �V���b�v�����p

	if (UNUSED(pShop))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pShop = new CShop;	// �V���b�v
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pShop))
	{ // �m�ۂɐ������Ă���ꍇ

		// �V���b�v�̏�����
		if (FAILED(pShop->Init(buy)))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pShop;
			pShop = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pShop->SetPosition(rPos);

		// �m�ۂ����A�h���X��Ԃ�
		return pShop;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�ݒ菈��
//============================================================
void CShop::Set(const BUY buy)
{
	// �����̍w���i��ݒ�
	m_buy = buy;
}

//============================================================
//	�擾����
//============================================================
CShop::BUY CShop::Get(void)
{
	// �w���i��Ԃ�
	return m_buy;
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CShop::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CShop::SetPositionRelative(void)
{
	// �A�C�R���̈ʒu��ݒ�
	m_pIcon->SetPosition(m_pos);

	// �K�v�̗͂̕\���ʒu��ݒ�
	m_pLife->SetPosition(m_pos + ADD_POS_LIFE);

	// �K�v���x���̕\���ʒu��ݒ�
	m_pLv->SetPosition(m_pos + ADD_POS_LEVEL);
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CShop::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F����
	m_col = rCol;

	// �A�C�R���̈ʒu��ݒ�
	m_pIcon->SetColor(rCol);
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CShop::GetPosition(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CShop::GetColor(void) const
{
	// �F��Ԃ�
	return m_col;
}
