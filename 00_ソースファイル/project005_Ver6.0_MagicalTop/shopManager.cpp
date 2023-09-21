//============================================================
//
//	�V���b�v�}�l�[�W���[���� [shopManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shopManager.h"
#include "manager.h"
#include "input.h"
#include "texture.h"
#include "sceneGame.h"
#include "object2D.h"
#include "valueUI.h"
#include "shop.h"
#include "multiValue.h"
#include "player.h"
#include "stage.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SHOP_PRIO	(6)	// �V���b�v�\���̗D�揇��

#define POS_BG	(D3DXVECTOR3(SCREEN_CENT.x, 330.0f, 0.0f))	// �w�i�̈ʒu
#define SIZE_BG	(D3DXVECTOR3(1088.0f, 562.0f, 0.0f))		// �w�i�̑傫��
#define COL_BG	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.55f))		// �w�i�̐F

#define POS_PLAYER_ICON	(D3DXVECTOR3(160.0f, 110.0f, 0.0f))	// �v���C���[�A�C�R���̈ʒu
#define POS_EXP_ICON	(D3DXVECTOR3(785.0f, 110.0f, 0.0f))	// �o���l�A�C�R���̈ʒu
#define SIZE_ICON		(D3DXVECTOR3(100.0f, 100.0f, 0.0f))	// �A�C�R���̑傫��

#define DIGIT_LIFE	(3)	// �̗͂̌���
#define POS_LIFE	(D3DXVECTOR3(270.0f, 120.0f, 0.0f))	// �̗͂̃^�C�g���Ɛ����̈ʒu
#define SPACE_LIFE	(D3DXVECTOR3(100.0f, -2.5f, 0.0f))	// �̗͂̃^�C�g���Ɛ����̍s��

#define DIGIT_LEVEL	(2)	// ���x���̌���
#define POS_LEVEL	(D3DXVECTOR3(920.0f, 120.0f, 0.0f))	// ���x���̃^�C�g���Ɛ����̈ʒu
#define SPACE_LEVEL	(D3DXVECTOR3(130.0f, -2.5f, 0.0f))	// ���x���̃^�C�g���Ɛ����̍s��

#define VALUEUI_SPACE_VALUE	(D3DXVECTOR3(70.0f, 0.0f, 0.0f))	// ����UI�̐����̍s��
#define VALUEUI_SIZE_TITLE	(D3DXVECTOR3(252.0f, 80.0f, 0.0f))	// ����UI�̃^�C�g���̑傫��
#define VALUEUI_SIZE_VALUE	(D3DXVECTOR3(80.0f, 80.0f, 0.0f))	// ����UI�̐����̑傫��

#define POS_MAXLIFE				(D3DXVECTOR3(565.0f, 126.0f, 0.0f))	// �ő�̗͂̃^�C�g���Ɛ����̈ʒu
#define SPACE_MAXLIFE			(D3DXVECTOR3(45.0f, 0.0f, 0.0f))	// �ő�̗͂̃^�C�g���Ɛ����̍s��
#define MAX_LIFE_SPACE_VALUE	(D3DXVECTOR3(45.0f, 0.0f, 0.0f))	// �ő�̗͂̐����̍s��
#define MAX_LIFE_SIZE_TITLE		(D3DXVECTOR3(33.0f, 55.0f, 0.0f))	// �ő�̗͂̃^�C�g���̑傫��
#define MAX_LIFE_SIZE_VALUE		(D3DXVECTOR3(55.0f, 55.0f, 0.0f))	// �ő�̗͂̐����̑傫��

#define POS_SHOP	(D3DXVECTOR3(285.0f, 240.0f, 0.0f))	// �V���b�v�̈ʒu
#define SPACE_SHOP	(D3DXVECTOR3(350.0f, 0.0f, 0.0f))	// �V���b�v�̋�
#define CHOICE_COL	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// �I�𒆃J���[
#define DEFAULT_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))	// ��I�𒆃J���[

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CShopManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\icon000.png",	// �v���C���[�A�C�R���e�N�X�`��
	"data\\TEXTURE\\icon001.png",	// �o���l�A�C�R���e�N�X�`��
	"data\\TEXTURE\\shop000.png",	// �̗̓e�N�X�`��
	"data\\TEXTURE\\shop001.png",	// �ő�̗͋�؂�e�N�X�`��
	"data\\TEXTURE\\shop002.png",	// ���x���e�N�X�`��
};

//************************************************************
//	�e�N���X [CShopManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CShopManager::CShopManager()
{
	// �����o�ϐ����N���A
	memset(&m_apShop[0], 0, sizeof(m_apShop));	// �V���b�v���
	m_pBg			= NULL;	// �w�i���
	m_pIconPlayer	= NULL;	// �v���C���[�A�C�R�����
	m_pIconExp		= NULL;	// �o���l�A�C�R�����
	m_pLife			= NULL;	// �v���C���[�̗͏��
	m_pMaxLife		= NULL;	// �v���C���[�ő�̗͏��
	m_pLv			= NULL;	// �v���C���[���x�����
	m_nSelect		= 0;	// ���݂̑I��ԍ�
	m_nOldSelect	= 0;	// �ߋ��̑I��ԍ�
}

//============================================================
//	�f�X�g���N�^
//============================================================
CShopManager::~CShopManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CShopManager::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`��

	// �����o�ϐ���������
	memset(&m_apShop[0], 0, sizeof(m_apShop));	// �V���b�v���
	m_pBg			= NULL;	// �w�i���
	m_pIconPlayer	= NULL;	// �v���C���[�A�C�R�����
	m_pIconExp		= NULL;	// �o���l�A�C�R�����
	m_pLife			= NULL;	// �v���C���[�̗͏��
	m_pMaxLife		= NULL;	// �v���C���[�ő�̗͏��
	m_pLv			= NULL;	// �v���C���[���x�����
	m_nSelect		= 0;	// ���݂̑I��ԍ�
	m_nOldSelect	= 0;	// �ߋ��̑I��ԍ�

	//--------------------------------------------------------
	//	�w�i�̐����E�ݒ�
	//--------------------------------------------------------
	// �w�i���̐���
	m_pBg = CObject2D::Create
	( // ����
		POS_BG,		// �ʒu
		SIZE_BG,	// �傫��
		VEC3_ZERO,	// ����
		COL_BG		// �F
	);
	if (UNUSED(m_pBg))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pBg->SetPriority(SHOP_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pBg->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�v���C���[�A�C�R���̐����E�ݒ�
	//--------------------------------------------------------
	// �v���C���[�A�C�R�����̐���
	m_pIconPlayer = CObject2D::Create
	( // ����
		POS_PLAYER_ICON,	// �ʒu
		SIZE_ICON			// �傫��
	);
	if (UNUSED(m_pIconPlayer))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`���������E�ݒ�
	m_pIconPlayer->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_PLAYER]));

	// �D�揇�ʂ�ݒ�
	m_pIconPlayer->SetPriority(SHOP_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pIconPlayer->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�o���l�A�C�R���̐����E�ݒ�
	//--------------------------------------------------------
	// �o���l�A�C�R�����̐���
	m_pIconExp = CObject2D::Create
	( // ����
		POS_EXP_ICON,	// �ʒu
		SIZE_ICON		// �傫��
	);
	if (UNUSED(m_pIconExp))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`���������E�ݒ�
	m_pIconExp->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_EXP]));

	// �D�揇�ʂ�ݒ�
	m_pIconExp->SetPriority(SHOP_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pIconExp->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�v���C���[�̗͂̐����E�ݒ�
	//--------------------------------------------------------
	// �v���C���[�̗̑͏��̐���
	m_pLife = CValueUI::Create
	( // ����
		mc_apTextureFile[TEXTURE_LIFE],	// �^�C�g���e�N�X�`���p�X
		CValue::TEXTURE_ANGULAR,		// �����e�N�X�`��
		DIGIT_LIFE,				// ����
		POS_LIFE,				// �ʒu
		SPACE_LIFE,				// �s��
		VALUEUI_SPACE_VALUE,	// �����s��
		VALUEUI_SIZE_TITLE,		// �^�C�g���傫��
		VALUEUI_SIZE_VALUE		// �����傫��
	);
	if (UNUSED(m_pLife))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pLife->SetPriority(SHOP_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pLife->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�v���C���[�ő�̗͂̐����E�ݒ�
	//--------------------------------------------------------
	// �v���C���[�̍ő�̗͏��̐���
	m_pMaxLife = CValueUI::Create
	( // ����
		mc_apTextureFile[TEXTURE_SLASH],	// �^�C�g���e�N�X�`���p�X
		CValue::TEXTURE_ANGULAR,			// �����e�N�X�`��
		DIGIT_LIFE,				// ����
		POS_MAXLIFE,			// �ʒu
		SPACE_MAXLIFE,			// �s��
		MAX_LIFE_SPACE_VALUE,	// �����s��
		MAX_LIFE_SIZE_TITLE,	// �^�C�g���傫��
		MAX_LIFE_SIZE_VALUE		// �����傫��
	);
	if (UNUSED(m_pMaxLife))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pMaxLife->SetPriority(SHOP_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pMaxLife->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�v���C���[���x���̐����E�ݒ�
	//--------------------------------------------------------
	// �v���C���[�̃��x�����̐���
	m_pLv = CValueUI::Create
	( // ����
		mc_apTextureFile[TEXTURE_LV],	// �^�C�g���e�N�X�`���p�X
		CValue::TEXTURE_ANGULAR,		// �����e�N�X�`��
		DIGIT_LEVEL,			// ����
		POS_LEVEL,				// �ʒu
		SPACE_LEVEL,			// �s��
		VALUEUI_SPACE_VALUE,	// �����s��
		VALUEUI_SIZE_TITLE,		// �^�C�g���傫��
		VALUEUI_SIZE_VALUE		// �����傫��
	);
	if (UNUSED(m_pLv))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pLv->SetPriority(SHOP_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pLv->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�V���b�v���̐����E�ݒ�
	//--------------------------------------------------------
	for (int nCntShop = 0; nCntShop < SELECT_MAX; nCntShop++)
	{ // �V���b�v�̕i�ڐ����J��Ԃ�

		// �V���b�v���̐���
		m_apShop[nCntShop] = CShop::Create
		( // ����
			CShop::BUY_PLAYLIFE,					// �w���i
			POS_SHOP + (float)nCntShop * SPACE_SHOP	// �ʒu
		);
		if (UNUSED(m_apShop[nCntShop]))
		{ // ��g�p���̏ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apShop[nCntShop]->SetPriority(SHOP_PRIO);

		// �`������Ȃ���Ԃɂ���
		m_apShop[nCntShop]->SetEnableDraw(false);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CShopManager::Uninit(void)
{
	// �I�u�W�F�N�g2D����j��
	m_pBg->Uninit();			// �w�i���
	m_pIconPlayer->Uninit();	// �v���C���[�A�C�R�����
	m_pIconExp->Uninit();		// �o���l�A�C�R�����

	// ����UI����j��
	m_pLife->Uninit();		// �v���C���[�̗͏��
	m_pMaxLife->Uninit();	// �v���C���[�ő�̗͏��
	m_pLv->Uninit();		// �v���C���[���x�����

	for (int nCntShop = 0; nCntShop < SELECT_MAX; nCntShop++)
	{ // �V���b�v�̕i�ڐ����J��Ԃ�

		if (USED(m_apShop[nCntShop]))
		{ // �V���b�v���g�p����Ă���ꍇ

			// �V���b�v����j��
			m_apShop[nCntShop]->Uninit();
		}
	}
}

//============================================================
//	�X�V����
//============================================================
void CShopManager::Update(void)
{
	// ���l�ɑ̗͂�ݒ�
	m_pLife->GetMultiValue()->SetNum(CScene::GetPlayer()->GetLife());

	// ���l�ɍő�̗͂�ݒ�
	m_pMaxLife->GetMultiValue()->SetNum(CScene::GetPlayer()->GetMaxLife());

	// ���l�Ƀ��x����ݒ�
	m_pLv->GetMultiValue()->SetNum(CScene::GetPlayer()->GetLevel());

	// �w���i�I���̍X�V
	UpdateSelect();

	// �I�u�W�F�N�g2D���̍X�V
	m_pBg->Update();			// �w�i���
	m_pIconPlayer->Update();	// �v���C���[�A�C�R�����
	m_pIconExp->Update();		// �o���l�A�C�R�����

	// ����UI���̍X�V
	m_pLife->Update();		// �v���C���[�̗͏��
	m_pMaxLife->Update();	// �v���C���[�ő�̗͏��
	m_pLv->Update();		// �v���C���[���x�����
}

//============================================================
//	�J�X�󋵂̐ݒ菈��
//============================================================
void CShopManager::SetEnableOpen(const bool bOpen)
{
	// �V���b�v�̕i���낦��S�ύX
	AllRandomShop();

	// �V���b�v��\��
	SetEnableDraw(bOpen);

	if (bOpen)
	{ // �J�X����ꍇ

		// ���l�ɑ̗͂�ݒ�
		m_pLife->GetMultiValue()->SetNum(CScene::GetPlayer()->GetLife());

		// ���l�ɍő�̗͂�ݒ�
		m_pMaxLife->GetMultiValue()->SetNum(CScene::GetPlayer()->GetMaxLife());

		// ���l�Ƀ��x����ݒ�
		m_pLv->GetMultiValue()->SetNum(CScene::GetPlayer()->GetLevel());
	}
}

//============================================================
//	��������
//============================================================
CShopManager *CShopManager::Create(void)
{
	// �|�C���^��錾
	CShopManager *pShopManager = NULL;	// �V���b�v�}�l�[�W���[�����p

	if (UNUSED(pShopManager))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pShopManager = new CShopManager;	// �V���b�v�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pShopManager))
	{ // �g�p����Ă���ꍇ
		
		// �V���b�v�}�l�[�W���[�̏�����
		if (FAILED(pShopManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pShopManager;
			pShopManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pShopManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CShopManager::Release(CShopManager *&prShopManager)
{
	if (USED(prShopManager))
	{ // �g�p���̏ꍇ

		// �V���b�v�}�l�[�W���[�̏I��
		prShopManager->Uninit();

		// �������J��
		delete prShopManager;
		prShopManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�w���i�I���̍X�V����
//============================================================
void CShopManager::UpdateSelect(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetPad();		// �p�b�h

	// �ߋ��̑I��ԍ����X�V
	m_nOldSelect = m_nSelect;

	// �I�𑀍�
	if (pKeyboard->GetTrigger(DIK_LEFT) || pPad->GetTrigger(CInputPad::KEY_LEFT))
	{
		// �I�𒆔ԍ������Z
		m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;
	}
	else if (pKeyboard->GetTrigger(DIK_RIGHT) || pPad->GetTrigger(CInputPad::KEY_RIGHT))
	{
		// �I�𒆔ԍ������Z
		m_nSelect = (m_nSelect + 1) % SELECT_MAX;
	}

	// �I�𒆂̃J���[��ݒ�
	m_apShop[m_nOldSelect]->SetColor(DEFAULT_COL);	// �ߋ��̑I�����Â�����
	m_apShop[m_nSelect]->SetColor(CHOICE_COL);		// ���݂̑I���𖾂邭����
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CShopManager::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	m_pBg->SetEnableDraw(bDraw);			// �w�i���
	m_pIconPlayer->SetEnableDraw(bDraw);	// �v���C���[�A�C�R�����
	m_pIconExp->SetEnableDraw(bDraw);		// �o���l�A�C�R�����
	m_pLife->SetEnableDraw(bDraw);			// �v���C���[�̗͏��
	m_pMaxLife->SetEnableDraw(bDraw);		// �v���C���[�ő�̗͏��
	m_pLv->SetEnableDraw(bDraw);			// �v���C���[���x�����

	for (int nCntShop = 0; nCntShop < SELECT_MAX; nCntShop++)
	{ // �V���b�v�̕i�ڐ����J��Ԃ�

		// �����̕`��󋵂�ݒ�
		m_apShop[nCntShop]->SetEnableDraw(bDraw);	// �V���b�v���
	}
}

//============================================================
//	�V���b�v�P�ύX����
//============================================================
int CShopManager::RandomShop
(
	const int nShop,	// �ύX����V���b�v�C���f�b�N�X
	int *pOmit,			// �����_�����Ȃ��w���i�̐擪�A�h���X
	const int nNumOmit	// �����_�����Ȃ��w���i��
)
{
	// �ϐ���錾
	int nNumShuffle = 0;		// �V���b�t���f�[�^�̕K�v��
	int nShuffleID = 0;			// �V���b�t���f�[�^������C���f�b�N�X
	int nHitID = 0;				// �V���b�t���f�[�^�̎Q�ƃC���f�b�N�X
	int nRandomHit = NONE_IDX;	// �ύX��̍w���i

	// �ϐ��z���錾
	bool aCanBuy[CShop::BUY_MAX] = {};	// �w���ł��邩�̏��

	// �|�C���^��錾
	int *pShuffle = NULL;	// �f�[�^�̃V���b�t���p

	//--------------------------------------------------------
	//	�����_������Ȃ��l��ݒ�E�V���b�t������ݒ�
	//--------------------------------------------------------
	for (int nCntShopMenu = 0; nCntShopMenu < CShop::BUY_MAX; nCntShopMenu++)
	{ // �w���i�̍ő吔���J��Ԃ�

		// �ϐ���錾
		bool bOmit = false;	// �����_���Ȃ���

		if (USED(pOmit))
		{ // �Ȃ��w���i������ꍇ

			for (int nCntBuy = 0; nCntBuy < nNumOmit; nCntBuy++)
			{ // �����_�����Ȃ��w���i�����J��Ԃ�

				if (pOmit[nCntBuy] == nCntShopMenu)
				{ // �����_�����Ȃ��w���i�������ꍇ

					// �����_�����Ȃ���Ԃɂ���
					bOmit = true;

					// �����𔲂���
					break;
				}
			}
		}

		if (bOmit)
		{ // �����_�����Ȃ��ꍇ

			// �w���ł��Ȃ���Ԃɂ���
			aCanBuy[nCntShopMenu] = false;
		}
		else
		{ // �����_�����Ȃ��Ȃ��ꍇ

			// �w���ł����Ԃɂ���
			aCanBuy[nCntShopMenu] = true;

			// �V���b�t���f�[�^�K�v�������Z
			nNumShuffle++;
		}
	}

	//--------------------------------------------------------
	//	�V���b�t������l���Ȃ��ꍇ (����؂�)
	//--------------------------------------------------------
	if (nNumShuffle <= 0)
	{ // �w���ł�����̂����݂��Ȃ��ꍇ

		// �w���i��ύX
		m_apShop[nShop]->SetBuy((CShop::BUY)NONE_IDX);

		// ����؂��Ԃ�
		return NONE_IDX;
	}

	//--------------------------------------------------------
	//	�V���b�t���p�z�񃁃����m��
	//--------------------------------------------------------
	if (UNUSED(pShuffle))
	{ // �V���b�t���f�[�^���g���Ă��Ȃ��ꍇ

		// �V���b�t���f�[�^�K�v�����̃���������
		pShuffle = new int[nNumShuffle];

		// ��O����
		assert(USED(pShuffle));	// �������s
	}
	else { assert(false); }	// �g�p��

	//--------------------------------------------------------
	//	�V���b�t���p�z��ɏȂ����l�ȊO�����ɑ��
	//--------------------------------------------------------
	for (int nCntShopMenu = 0; nCntShopMenu < CShop::BUY_MAX; nCntShopMenu++)
	{ // �w���i�̍ő吔���J��Ԃ�

		if (aCanBuy[nCntShopMenu])
		{ // �w���ł���ꍇ

			// ���݂̃C���f�b�N�X���V���b�t���f�[�^�ɑ��
			pShuffle[nShuffleID] = nCntShopMenu;

			// �V���b�t���f�[�^������C���f�b�N�X�����Z
			nShuffleID++;
		}
	}

	//--------------------------------------------------------
	//	�z����V���b�t�����w���i��ݒ�
	//--------------------------------------------------------
	// �v�f���V���b�t������
	useful::Shuffle(pShuffle, nNumShuffle);

	// �V���b�t�������f�[�^�̂ǂ̗v�f���g�p���邩��ݒ�
	nHitID = rand() % nNumShuffle;

	// �����_���ȃC���f�b�N�X�̗v�f���w���i�Ƃ���
	nRandomHit = pShuffle[nHitID];

	// �w���i��ύX
	m_apShop[nShop]->SetBuy((CShop::BUY)nRandomHit);

	//--------------------------------------------------------
	//	�V���b�t���p�z�񃁃����J��
	//--------------------------------------------------------
	if (USED(pShuffle))
	{ // �V���b�t���f�[�^���g���Ă���ꍇ

		// �V���b�t���f�[�^�K�v�����̃������J��
		delete[] pShuffle;
		pShuffle = NULL;

		// ��O����
		assert(UNUSED(pShuffle));	// �J�����s
	}
	else { assert(false); }	// ��g�p��

	// �ύX��̍w���i��Ԃ�
	return nRandomHit;
}

//============================================================
//	�V���b�v�S�ύX����
//============================================================
void CShopManager::AllRandomShop(void)
{
	// �ϐ���錾
	int nNumOmit = 0;	// �V���b�t�����Ȃ���

	// �ϐ��z���錾
	int aRandomHit[SELECT_MAX] = {};	// �����_���ɓ��������w���i
	bool aOmitBuy[CShop::BUY_MAX] = {};	// �Ȃ����̏��

	// �|�C���^��錾
	int *pShuffle = NULL;	// �V���b�t�����Ȃ��f�[�^�̕ێ��p

	//--------------------------------------------------------
	//	�Ȃ��w���i��ݒ�
	//--------------------------------------------------------

	// TODO�F�����ɏȂ��w���i��ݒ肷�鏈��

#if 0
	aOmitBuy[0] = true;
	nNumOmit++;
#endif

	//--------------------------------------------------------
	//	�V���b�v�����ύX����
	//--------------------------------------------------------
	for (int nCntShop = 0; nCntShop < SELECT_MAX; nCntShop++)
	{ // �V���b�v�̕i�ڐ����J��Ԃ�

		// �ϐ���錾
		int nOmitID = 0;	// ���݂̃V���b�t�����Ȃ��z��̗v�f

		//----------------------------------------------------
		//	�V���b�t������Ȃ��l�̐����������m��
		//----------------------------------------------------
		if (nNumOmit + nCntShop > 0)
		{ // �m�ۂ���ꍇ

			if (UNUSED(pShuffle))
			{ // �V���b�t���f�[�^���g���Ă��Ȃ��ꍇ

				// �V���b�t�����Ȃ������̃���������
				pShuffle = new int[nNumOmit + nCntShop];

				// ��O����
				assert(USED(pShuffle));	// �������s
			}
			else { assert(false); }	// �g�p��

			for (int nCntShopMenu = 0; nCntShopMenu < CShop::BUY_MAX; nCntShopMenu++)
			{ // �w���i�̍ő吔���J��Ԃ�

				if (aOmitBuy[nCntShopMenu])
				{ // �Ȃ��ꍇ

					// ���݂̃C���f�b�N�X���Ȃ��f�[�^�ɑ��
					pShuffle[nOmitID] = nCntShopMenu;

					// �Ȃ��f�[�^������C���f�b�N�X�����Z
					nOmitID++;
				}
			}
		}

		//----------------------------------------------------
		//	�V���b�v���e�̕ύX�E�w���i���o���Ȃ��悤�ɐݒ�
		//----------------------------------------------------
		// �V���b�v�̒P�ύX
		aRandomHit[nCntShop] = RandomShop(nCntShop, pShuffle, nNumOmit + nCntShop);
		if (aRandomHit[nCntShop] > NONE_IDX)
		{ // ����؂�ł͂Ȃ������ꍇ

			// �؂�ւ�����w���i��������Ȃ��悤�ɐݒ�
			aOmitBuy[aRandomHit[nCntShop]] = true;
		}

		//----------------------------------------------------
		//	�V���b�t������Ȃ��l�̃������J��
		//----------------------------------------------------
		if (nNumOmit + nCntShop > 0)
		{ // �m�ۂ��Ă���ꍇ

			if (USED(pShuffle))
			{ // �V���b�t���f�[�^���g���Ă���ꍇ

				// �V���b�t�����Ȃ������̃������J��
				delete[] pShuffle;
				pShuffle = NULL;

				// ��O����
				assert(UNUSED(pShuffle));	// �J�����s
			}
			else { assert(false); }	// ��g�p��
		}
	}
}
