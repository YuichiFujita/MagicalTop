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
#include "target.h"
#include "player.h"
#include "stage.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SHOP_PRIO	(6)	// �V���b�v�\���̗D�揇��

#define POS_SHOP	(D3DXVECTOR3(285.0f, 270.0f, 0.0f))	// �V���b�v�̈ʒu
#define SPACE_SHOP	(D3DXVECTOR3(350.0f, 0.0f, 0.0f))	// �V���b�v�̋�

#define CHOICE_COL	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// �I�𒆃J���[
#define DEFAULT_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))	// ��I�𒆃J���[

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CShopManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\icon000.png",	// �^�[�Q�b�g�A�C�R���e�N�X�`��
	"data\\TEXTURE\\icon001.png",	// �o���l�A�C�R���e�N�X�`��
	"data\\TEXTURE\\shop000.png",	// �̗̓e�N�X�`��
	"data\\TEXTURE\\shop001.png",	// ���x���e�N�X�`��
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
	m_pIconTarget	= NULL;	// �^�[�Q�b�g�A�C�R�����
	m_pIconExp		= NULL;	// �o���l�A�C�R�����
	m_pLife			= NULL;	// �^�[�Q�b�g�̗͏��
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
	m_pIconTarget	= NULL;	// �^�[�Q�b�g�A�C�R�����
	m_pIconExp		= NULL;	// �o���l�A�C�R�����
	m_pLife			= NULL;	// �^�[�Q�b�g�̗͏��
	m_pLv			= NULL;	// �v���C���[���x�����
	m_nSelect		= 0;	// ���݂̑I��ԍ�
	m_nOldSelect	= 0;	// �ߋ��̑I��ԍ�

	//--------------------------------------------------------
	//	�w�i�̐����E�ݒ�
	//--------------------------------------------------------
	// �w�i���̐���
	m_pBg = CObject2D::Create	// TODO�F�萔
	( // ����
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.85f, SCREEN_HEIGHT * 0.78f, 0.0f),
		VEC3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.55f)
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
	//	�^�[�Q�b�g�A�C�R���̐����E�ݒ�
	//--------------------------------------------------------
	// �^�[�Q�b�g�A�C�R�����̐���
	m_pIconTarget = CObject2D::Create	// TODO�F�萔
	( // ����
		D3DXVECTOR3(160.0f, 150.0f, 0.0f),
		D3DXVECTOR3(120.0f, 120.0f, 0.0f)
	);
	if (UNUSED(m_pIconTarget))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`���������E�ݒ�
	m_pIconTarget->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_TARGET]));

	// �D�揇�ʂ�ݒ�
	m_pIconTarget->SetPriority(SHOP_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pIconTarget->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�o���l�A�C�R���̐����E�ݒ�
	//--------------------------------------------------------
	// �o���l�A�C�R�����̐���
	m_pIconExp = CObject2D::Create	// TODO�F�萔
	( // ����
		D3DXVECTOR3(750.0f, 145.0f, 0.0f),
		D3DXVECTOR3(120.0f, 120.0f, 0.0f)
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
	//	�^�[�Q�b�g�̗͂̐����E�ݒ�
	//--------------------------------------------------------
	// �^�[�Q�b�g�̗̑͏��̐���
	m_pLife = CValueUI::Create
	( // ����
		D3DXVECTOR3(280.0f, 160.0f, 0.0f),	// 
		D3DXVECTOR3(110.0f, -10.0f, 0.0f),	// 
		D3DXVECTOR3(280.0f, 90.0f, 0.0f),	// 
		D3DXVECTOR3(80.0f, 90.0f, 0.0f),	// 
		pTexture->Regist(mc_apTextureFile[TEXTURE_LIFE])
	);
	if (UNUSED(m_pLife))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����̍s�Ԃ�ݒ�
	m_pLife->GetMultiValue()->SetSpace(D3DXVECTOR3(80.0f, 0.0f, 0.0f));

	// �����̌�����ݒ�
	m_pLife->GetMultiValue()->SetDigit(4);

	// �D�揇�ʂ�ݒ�
	m_pLife->SetPriority(SHOP_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pLife->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�v���C���[���x���̐����E�ݒ�
	//--------------------------------------------------------
	// �v���C���[�̃��x�����̐���
	m_pLv = CValueUI::Create
	( // ����
		D3DXVECTOR3(900.0f, 160.0f, 0.0f),	// 
		D3DXVECTOR3(140.0f, -8.0f, 0.0f),	// 
		D3DXVECTOR3(280.0f, 90.0f, 0.0f),	// 
		D3DXVECTOR3(80.0f, 90.0f, 0.0f),	// 
		pTexture->Regist(mc_apTextureFile[TEXTURE_LV])
	);
	if (UNUSED(m_pLv))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����̍s�Ԃ�ݒ�
	m_pLv->GetMultiValue()->SetSpace(D3DXVECTOR3(80.0f, 0.0f, 0.0f));

	// �����̌�����ݒ�
	m_pLv->GetMultiValue()->SetDigit(2);

	// �D�揇�ʂ�ݒ�
	m_pLv->SetPriority(SHOP_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pLv->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�V���b�v���̐����E�ݒ�
	//--------------------------------------------------------
	for (int nCntShop = 0; nCntShop < MAX_SHOP; nCntShop++)
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
	m_pIconTarget->Uninit();	// �^�[�Q�b�g�A�C�R�����
	m_pIconExp->Uninit();		// �o���l�A�C�R�����

	// ����UI����j��
	m_pLife->Uninit();	// �^�[�Q�b�g�̗͏��
	m_pLv->Uninit();	// �v���C���[���x�����

	for (int nCntShop = 0; nCntShop < MAX_SHOP; nCntShop++)
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
	m_pLife->GetMultiValue()->SetNum(CScene::GetTarget()->GetLife());

	// ���l�Ƀ��x����ݒ�
	m_pLv->GetMultiValue()->SetNum(CScene::GetPlayer()->GetLevel());

	// �w���i�I���̍X�V
	UpdateSelect();

	// �I�u�W�F�N�g2D���̍X�V
	m_pBg->Update();			// �w�i���
	m_pIconTarget->Update();	// �^�[�Q�b�g�A�C�R�����
	m_pIconExp->Update();		// �o���l�A�C�R�����

	// ����UI���̍X�V
	m_pLife->Update();	// �^�[�Q�b�g�̗͏��
	m_pLv->Update();	// �v���C���[���x�����
}

//============================================================
//	�V���b�v�S�ύX����
//============================================================
void CShopManager::AllRandomShop(void)
{
	// �ϐ���錾
	int nNumOmit = 0;	// �V���b�t�����Ȃ���

	// �ϐ��z���錾
	int aRandomHit[MAX_SHOP] = {};		// �����_���ɓ��������w���i
	bool aOmitBuy[CShop::BUY_MAX] = {};	// �Ȃ����̏��

	// �|�C���^��錾
	int *pShuffle = NULL;	// �V���b�t�����Ȃ��f�[�^�̕ێ��p

	// TODO�F�����ɏȂ��w���i��ݒ肷�鏈��

#if 0
	aOmitBuy[0] = true;
	nNumOmit++;
#endif

	for (int nCntShop = 0; nCntShop < MAX_SHOP; nCntShop++)
	{ // �V���b�v�̕i�ڐ����J��Ԃ�

		// �ϐ���錾
		int nOmitID = 0;	// ���݂̃V���b�t�����Ȃ��z��̗v�f

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

		// �V���b�v�̒P�ύX
		aRandomHit[nCntShop] = RandomShop(nCntShop, pShuffle, nNumOmit + nCntShop);
		if (aRandomHit[nCntShop] > NONE_IDX)
		{ // ����؂�ł͂Ȃ������ꍇ

			// �؂�ւ�����w���i��������Ȃ��悤�ɐݒ�
			aOmitBuy[aRandomHit[nCntShop]] = true;
		}

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

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CShopManager::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	m_pBg->SetEnableDraw(bDraw);			// �w�i���
	m_pIconTarget->SetEnableDraw(bDraw);	// �^�[�Q�b�g�A�C�R�����
	m_pIconExp->SetEnableDraw(bDraw);		// �o���l�A�C�R�����
	m_pLife->SetEnableDraw(bDraw);			// �^�[�Q�b�g�̗͏��
	m_pLv->SetEnableDraw(bDraw);			// �v���C���[���x�����

	for (int nCntShop = 0; nCntShop < MAX_SHOP; nCntShop++)
	{ // �V���b�v�̕i�ڐ����J��Ԃ�

		// �����̕`��󋵂�ݒ�
		m_apShop[nCntShop]->SetEnableDraw(bDraw);	// �V���b�v���
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

	if (nNumShuffle <= 0)
	{ // �w���ł�����̂����݂��Ȃ��ꍇ

		// �w���i��ύX
		m_apShop[nShop]->SetBuy((CShop::BUY)NONE_IDX);

		// ����؂��Ԃ�
		return NONE_IDX;
	}

	if (UNUSED(pShuffle))
	{ // �V���b�t���f�[�^���g���Ă��Ȃ��ꍇ

		// �V���b�t���f�[�^�K�v�����̃���������
		pShuffle = new int[nNumShuffle];

		// ��O����
		assert(USED(pShuffle));	// �������s
	}
	else { assert(false); }	// �g�p��

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

	// �v�f���V���b�t������
	useful::Shuffle(pShuffle, nNumShuffle);

	// �V���b�t�������f�[�^�̂ǂ̗v�f���g�p���邩��ݒ�
	nHitID = rand() % nNumShuffle;

	// �����_���ȃC���f�b�N�X�̗v�f���w���i�Ƃ���
	nRandomHit = pShuffle[nHitID];

	// �w���i��ύX
	m_apShop[nShop]->SetBuy((CShop::BUY)nRandomHit);

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
