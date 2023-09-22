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
#define PTRN_WIDTH	(5)	// �e�N�X�`���̉�������
#define PTRN_HEIGHT	(2)	// �e�N�X�`���̏c������

#define ICON_SIZE	(D3DXVECTOR3(120.0f, 120.0f, 0.0f))	// �A�C�R���̑傫��
#define DEFAULT_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))	// ��I�����̐F

#define EXPLAIN_SIZE	(D3DXVECTOR3(320.0f, 270.0f, 0.0f))	// �����̑傫��
#define ADD_POS_EXPLAIN	(D3DXVECTOR3(0.0f, 210.0f, 0.0f))	// �����̕\���ʒu�̑��΍��W�ւ̉��Z��

#define LV_VALUE_DIGIT	(2)	// ���x���̐����̌���
#define LV_SPACE		(D3DXVECTOR3(114.0f, -3.0f, 0.0f))	// ���x���̍s��
#define LV_TITLE_SIZE	(D3DXVECTOR3(180.0f, 50.0f, 0.0f))	// ���x���̃^�C�g���傫��
#define LV_VALUE_SIZE	(D3DXVECTOR3(50.0f, 50.0f, 0.0f))	// ���x���̐����傫��
#define LV_VALUE_SPACE	(D3DXVECTOR3(38.0f, 0.0f, 0.0f))	// ���x���̐����̍s��
#define ADD_POS_LEVEL	(D3DXVECTOR3(-40.0f, 130.0f, 0.0f))	// ���x���̕\���ʒu�̑��΍��W�ւ̉��Z��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CShop::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\shop003.png",	// �K�v���x���e�N�X�`��
	"data\\TEXTURE\\shop004.png",	// �A�C�R���e�N�X�`��
	"data\\TEXTURE\\shop005.png",	// �����e�N�X�`��
	"data\\TEXTURE\\shop006.png",	// ����؂�A�C�R���e�N�X�`��
	"data\\TEXTURE\\shop007.png",	// ����؂�����e�N�X�`��
};

const int CShop::mc_aNeedLevel[] =	// �w���K�v���x���萔
{
	1,	// ��
	1,	// �̗̓��x���A�b�v
	1,	// �}�i���x���A�b�v
	1,	// �_�b�V�����x���A�b�v
	2,	// �h��̓��x���A�b�v
	2,	// �f�������x���A�b�v
	6,	// ���@���ː����x���A�b�v
	3,	// ���@�e�����x���A�b�v
	3,	// ���@�A�ˑ��x���x���A�b�v
	2,	// �l���o���l����
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
	m_pIcon = NULL;		// �w���i�A�C�R��
	m_pExplain = NULL;	// �w���i����
	m_pLv = NULL;		// �K�v���x��
	m_pos = VEC3_ZERO;	// �ʒu
	m_col = XCOL_WHITE;	// �F
	m_buy = BUY_HEAL;	// �w���i
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
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`��

	// �����o�ϐ���������
	m_pIcon = NULL;		// �w���i�A�C�R��
	m_pLv = NULL;		// �K�v���x��
	m_pos = VEC3_ZERO;	// �ʒu
	m_col = XCOL_WHITE;	// �F
	m_buy = BUY_HEAL;	// �w���i

	//--------------------------------------------------------
	//	�w���i�A�C�R���̐����E�ݒ�
	//--------------------------------------------------------
	// �w���i�A�C�R���̐���
	m_pIcon = CAnim2D::Create
	( // ����
		PTRN_WIDTH,		// ��������
		PTRN_HEIGHT,	// �c������
		VEC3_ZERO,		// �ʒu
		ICON_SIZE,		// �傫��
		VEC3_ZERO,		// ����
		DEFAULT_COL		// �F
	);
	if (UNUSED(m_pIcon))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	�w���i�����̐����E�ݒ�
	//--------------------------------------------------------
	// �w���i�����̐���
	m_pExplain = CAnim2D::Create
	( // ����
		PTRN_WIDTH,		// ��������
		PTRN_HEIGHT,	// �c������
		VEC3_ZERO,		// �ʒu
		EXPLAIN_SIZE,	// �傫��
		VEC3_ZERO,		// ����
		DEFAULT_COL		// �F
	);
	if (UNUSED(m_pExplain))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	�K�v���x���̐����E�ݒ�
	//--------------------------------------------------------
	// �K�v���x���̐���
	m_pLv = CValueUI::Create
	( // ����
		mc_apTextureFile[TEXTURE_LV],	// �^�C�g���e�N�X�`���p�X
		CValue::TEXTURE_ANGULAR,		// �����e�N�X�`��
		LV_VALUE_DIGIT,	// ����
		VEC3_ZERO,		// �ʒu
		LV_SPACE,		// �s��
		LV_VALUE_SPACE,	// �����s��
		LV_TITLE_SIZE,	// �^�C�g���傫��
		LV_VALUE_SIZE,	// �����傫��
		VEC3_ZERO,		// �^�C�g������
		VEC3_ZERO,		// ��������
		DEFAULT_COL,	// �^�C�g���F
		DEFAULT_COL		// �����F
	);
	if (UNUSED(m_pLv))
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
void CShop::Uninit(void)
{
	// �w���i�A�C�R���̏I��
	m_pIcon->Uninit();

	// �w���i�����̏I��
	m_pExplain->Uninit();

	// �K�v���x���̏I��
	m_pLv->Uninit();

	// ���g�̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CShop::Update(void)
{
	// �w���i�A�C�R���̍X�V
	m_pIcon->Update();

	// �w���i�����̍X�V
	m_pExplain->Update();

	// �K�v���x���̍X�V
	m_pLv->Update();
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
	CObject::SetEnableDraw(bDraw);		// ���g
	m_pIcon->SetEnableDraw(bDraw);		// �w���i�A�C�R��
	m_pExplain->SetEnableDraw(bDraw);	// �w���i����
	m_pLv->SetEnableDraw(bDraw);		// �K�v���x��
}

//============================================================
//	�D�揇�ʂ̐ݒ菈��
//============================================================
void CShop::SetPriority(const int nPriority)
{
	// �����̗D�揇�ʂ�ݒ�
	CObject::SetPriority(nPriority);	// ���g
	m_pIcon->SetPriority(nPriority);	// �w���i�A�C�R��
	m_pExplain->SetPriority(nPriority);	// �w���i����
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
		if (FAILED(pShop->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pShop;
			pShop = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �w���i��ݒ�
		pShop->SetBuy(buy);

		// �ʒu��ݒ�
		pShop->SetPosition(rPos);

		// �m�ۂ����A�h���X��Ԃ�
		return pShop;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�w���K�v���x���擾����
//============================================================
int CShop::GetNeedLevel(const BUY buy)
{
	// �����̍w���i�̍w���K�v���x����Ԃ�
	return mc_aNeedLevel[buy];
}

//============================================================
//	�ݒ菈��
//============================================================
void CShop::SetBuy(const BUY buy)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`��

	// �����̍w���i��ݒ�
	m_buy = buy;

	if (m_buy == NONE_IDX)
	{ // ����؂ꂾ�����ꍇ

		// �e�N�X�`����o�^�E����
		m_pIcon->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_SOLDOUT_ICON]));			// �w���i�A�C�R��
		m_pExplain->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_SOLDOUT_EXPLAIN]));	// �w���i����

		// �w���i�A�C�R���̃e�N�X�`����������ݒ�
		m_pIcon->SetWidthPattern(1);
		m_pIcon->SetHeightPattern(1);

		// �w���i�����̃e�N�X�`����������ݒ�
		m_pExplain->SetWidthPattern(1);
		m_pExplain->SetHeightPattern(1);

		// �w���i�̃p�^�[����ݒ�
		m_pIcon->SetPattern(0);		// �w���i�A�C�R��
		m_pExplain->SetPattern(0);	// �w���i����

		// �w���i�̕K�v���x����ݒ�
		m_pLv->GetMultiValue()->SetNum(0);
	}
	else
	{ // ����؂�ł͂Ȃ������ꍇ

		// �e�N�X�`����o�^�E����
		m_pIcon->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_ICON]));			// �w���i�A�C�R��
		m_pExplain->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_EXPLAIN]));	// �w���i����

		// �w���i�A�C�R���̃e�N�X�`����������ݒ�
		m_pIcon->SetWidthPattern(PTRN_WIDTH);
		m_pIcon->SetHeightPattern(PTRN_HEIGHT);

		// �w���i�����̃e�N�X�`����������ݒ�
		m_pExplain->SetWidthPattern(PTRN_WIDTH);
		m_pExplain->SetHeightPattern(PTRN_HEIGHT);

		// �w���i�̃p�^�[����ݒ�
		m_pIcon->SetPattern(m_buy);		// �w���i�A�C�R��
		m_pExplain->SetPattern(m_buy);	// �w���i����

		// �w���i�̕K�v���x����ݒ�
		m_pLv->GetMultiValue()->SetNum(mc_aNeedLevel[m_buy]);
	}
}

//============================================================
//	�擾����
//============================================================
CShop::BUY CShop::GetBuy(void) const
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
	// �w���i�A�C�R���̈ʒu��ݒ�
	m_pIcon->SetPosition(m_pos);

	// �w���i�����̕\���ʒu��ݒ�
	m_pExplain->SetPosition(m_pos + ADD_POS_EXPLAIN);

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

	// �w���i�A�C�R���̐F��ݒ�
	m_pIcon->SetColor(rCol);

	// �w���i�����̐F��ݒ�
	m_pExplain->SetColor(rCol);

	// �K�v���x���̕\���ʒu��ݒ�
	m_pLv->SetColorTitle(rCol);				// �^�C�g���F
	m_pLv->GetMultiValue()->SetColor(rCol);	// �����F
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
