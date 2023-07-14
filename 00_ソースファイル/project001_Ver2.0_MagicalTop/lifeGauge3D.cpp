//============================================================
//
//	�̗̓Q�[�W3D���� [lifeGauge3D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "lifeGauge3D.h"
#include "manager.h"
#include "renderer.h"
#include "objectBillboard.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define LIFEGAUGE3D_PRIO	(4)	// �̗̓Q�[�W3D�̗D�揇��
#define GAUGE_WIDTH	(100.0f)	// �̗̓Q�[�W�̉���

#define GAUGE_SIZE	(D3DXVECTOR3(100.0f, 20.0f, 0.0f))	// �Q�[�W�T�C�Y
#define FRAME_SIZE	(D3DXVECTOR3(102.5f, 22.5f, 0.0f))	// �Q�[�W�t���[���T�C�Y
#define BG_COL		(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	// �Q�[�W�w�i�F
#define GAUGE_COL	(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	// �Q�[�W�F

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CLifeGauge3D::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	NULL,	// �e�N�X�`���Ȃ�
	NULL,	// �e�N�X�`���Ȃ�
	"data\\TEXTURE\\lifeGauge000.png",	// �Q�[�W�t���[��
};

//************************************************************
//	�q�N���X [CLifeGauge3D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLifeGauge3D::CLifeGauge3D(const int nMax, const int nFrame) : CObject(CObject::LABEL_LOCK, LIFEGAUGE3D_PRIO), m_nMaxLife(nMax), m_nFrame(nFrame)	// TODO�F�������b�N�ɂȂ��Ă�I�IRelease�������I
{
	// �����o�ϐ����N���A
	memset(&m_apBilboard[0], 0, sizeof(m_apBilboard));	// �r���{�[�h�̏��
	m_pLife = NULL;			// �̗̓Q�[�W3D�\���I�u�W�F�N�g�̏��
	m_state = STATE_NONE;	// ���
	m_fChange = 0.0f;		// �̗͕ϓ���
	m_fCurrentLife = 0.0f;	// ���ݑ̗�
	m_fPosUp = 0.0f;		// �̗͕\����Y�ʒu���Z��
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[
	m_nLife = 0;			// �̗�
}

//============================================================
//	�f�X�g���N�^
//============================================================
CLifeGauge3D::~CLifeGauge3D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CLifeGauge3D::Init(void)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �ϐ��z���錾
	D3DXVECTOR3 aSize[] =	// �傫��
	{ // �����l
		GAUGE_SIZE,	// �̗͔w�i
		GAUGE_SIZE,	// �̗̓Q�[�W
		FRAME_SIZE,	// �̗̓Q�[�W�t���[��
	};
	D3DXCOLOR aCol[] =		// �F
	{ // �����l
		BG_COL,		// �̗͔w�i
		GAUGE_COL,	// �̗̓Q�[�W
		XCOL_WHITE,	// �̗̓Q�[�W�t���[��
	};

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	memset(&m_apBilboard[0], 0, sizeof(m_apBilboard));	// �r���{�[�h�̏��
	m_pLife = NULL;			// �̗̓Q�[�W3D�\���I�u�W�F�N�g�̏��
	m_state = STATE_NONE;	// ���
	m_fChange = 0.0f;		// �̗͕ϓ���
	m_fCurrentLife = 0.0f;	// ���ݑ̗�
	m_fPosUp = 0.0f;		// �̗͕\����Y�ʒu���Z��
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[
	m_nLife = 0;			// �̗�

	for (int nCntLife = 0; nCntLife < TEXTURE_MAX; nCntLife++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �r���{�[�h�̐���
		m_apBilboard[nCntLife] = CObjectBillboard::Create
		( // ����
			VEC3_ZERO,			// �ʒu
			aSize[nCntLife],	// �傫��
			VEC3_ZERO,			// ����
			aCol[nCntLife]		// �F
		);
		if (UNUSED(m_apBilboard[nCntLife]))
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^
		nTextureID = pTexture->Regist(mc_apTextureFile[nCntLife]);

		// �e�N�X�`��������
		m_apBilboard[nCntLife]->BindTexture(nTextureID);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLifeGauge3D::Uninit(void)
{
	// �r���{�[�h��j��
	for (int nCntLife = 0; nCntLife < TEXTURE_MAX; nCntLife++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �r���{�[�h�̏I��
		m_apBilboard[nCntLife]->Uninit();
	}

	// ���g�̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CLifeGauge3D::Update(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxLife;	// �̗͕\���I�u�W�F�N�g�̃}�g���b�N�X

	// �Q�[�W�̐ݒ�
	if (m_state == STATE_CHANGE)
	{ // �̗͂��ϓ����̏ꍇ

		// �ϐ���錾
		D3DXVECTOR3 size = m_apBilboard[TEXTURE_GAUGE]->GetScaling();	// �傫��

		// �J�E���^�[�����Z
		m_nCounterState--;

		// ���݂̗̑̓Q�[�W����󂯂��_���[�W�������Z
		m_fCurrentLife += m_fChange;

		// �傫����ݒ�
		useful::LimitNum(m_fCurrentLife, 0.0f, (float)m_nMaxLife);		// ���ݑ̗͂�␳
		size.x = m_fCurrentLife * (GAUGE_WIDTH / (float)m_nMaxLife);	// �Q�[�W�̉�����ݒ�
		m_apBilboard[TEXTURE_GAUGE]->SetScaling(size);					// �̗͂�ݒ�

		if (m_nCounterState <= 0)
		{ // �J�E���^�[�� 0�ȉ��ɂȂ����ꍇ

			// �ʏ��Ԃɂ���
			m_state = STATE_NONE;
		}
	}

	// �ʒu�̐ݒ�
	for (int nCntLife = 0; nCntLife < TEXTURE_MAX; nCntLife++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �r���{�[�h�̍X�V
		m_apBilboard[nCntLife]->Update();

		// TODO�F�|�C���^�̊m�F�����Ƃ��ꂢ��
		if (CObject::CheckUse(m_pLife))
		{ // �̗͕\���Ώۂ��g�p����Ă����ꍇ

			// ���[���h�}�g���b�N�X���擾
			mtxLife = m_pLife->GetMtxWorld();

			// �ʒu��ݒ�
			m_apBilboard[nCntLife]->SetPosition(D3DXVECTOR3(mtxLife._41, mtxLife._42 + m_fPosUp, mtxLife._43));
		}
		else
		{ // �̗͕\���Ώۂ��g�p����Ă��Ȃ������ꍇ

			// �̗͕\�����Ă���I�u�W�F�N�g�̏���������
			m_pLife = NULL;

			// �I�u�W�F�N�g�̏I��
			Uninit();

			// �֐��𔲂���
			return;
		}
	}
}

//============================================================
//	�`�揈��
//============================================================
void CLifeGauge3D::Draw(void)
{
	for (int nCntLife = 0; nCntLife < TEXTURE_MAX; nCntLife++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �r���{�[�h�̕`��
		m_apBilboard[nCntLife]->Draw();
	}
}

//============================================================
//	��������
//============================================================
CLifeGauge3D *CLifeGauge3D::Create
(
	const int nLife,	// �̗�
	const int nMax,		// �ő�̗�
	const int nFrame,	// �̗͕ϓ��t���[��
	const float fPosUp,	// �̗͕\����Y�ʒu���Z��
	CObject *pObject	// �̗̓Q�[�W�\���I�u�W�F�N�g
)
{
	// �|�C���^��錾
	CLifeGauge3D *pLifeGauge3D = NULL;	// �̗̓Q�[�W3D�����p

	if (UNUSED(pLifeGauge3D))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pLifeGauge3D = new CLifeGauge3D(nMax, nFrame);	// �̗̓Q�[�W3D
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pLifeGauge3D))
	{ // �m�ۂɐ������Ă���ꍇ

		if (SUCCEEDED(pLifeGauge3D->GetResult()))
		{ // �I�u�W�F�N�g�̏��ݒ�ɐ��������ꍇ

			// �̗̓Q�[�W3D�̏�����
			if (FAILED(pLifeGauge3D->Init()))
			{ // �������Ɏ��s�����ꍇ

				// ���s��Ԃ�
				return NULL;
			}

			// �̗͂̐ݒ�
			pLifeGauge3D->SetLife(nLife);

			// �̗͕\���̐ݒ�
			pLifeGauge3D->SetLifeObject(pObject);

			// Y�ʒu���Z�ʂ̐ݒ�
			pLifeGauge3D->SetPositionUp(fPosUp);

			// �m�ۂ����A�h���X��Ԃ�
			return pLifeGauge3D;
		}
		else
		{ // �I�u�W�F�N�g�̏��ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pLifeGauge3D;
			pLifeGauge3D = NULL;

			// ���s��Ԃ�
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�̗͂̉��Z����
//============================================================
void CLifeGauge3D::AddLife(const int nAdd)
{
	// ���݂̗̑͂�ݒ�
	m_fCurrentLife = (float)m_nLife;

	// �̗͕ϓ��ʂ����߂�
	m_fChange = ((float)nAdd / (float)m_nFrame);

	// ����ݒ�
	m_state = STATE_CHANGE;		// ���l�ϓ����
	m_nCounterState = m_nFrame;	// ��ԊǗ��J�E���^�[

	// �̗͂����������Z
	m_nLife += nAdd;

	// �̗͂̐���
	useful::LimitNum(m_nLife, 0, m_nMaxLife);
}

//============================================================
//	�̗͕\���̐ݒ菈��
//============================================================
void CLifeGauge3D::SetLifeObject(CObject *pObject)
{
	// �����̃I�u�W�F�N�g��ݒ�
	m_pLife = pObject;
}

//============================================================
//	�̗͂̐ݒ菈��
//============================================================
void CLifeGauge3D::SetLife(const int nLife)
{
	// �ϐ���錾
	D3DXVECTOR3 size = m_apBilboard[TEXTURE_GAUGE]->GetScaling();	// �傫��

	// �����̗̑͂�ݒ�
	m_nLife = nLife;

	// �̗͂̐���
	useful::LimitNum(m_nLife, 0, m_nMaxLife);

	// �Q�[�W��ݒ�
	useful::LimitNum(m_nLife, 0, m_nMaxLife);						// �̗͂�␳
	size.x = (float)m_nLife * (GAUGE_WIDTH / (float)m_nMaxLife);	// �Q�[�W�̉�����ݒ�
	m_apBilboard[TEXTURE_GAUGE]->SetScaling(size);					// �̗͂�ݒ�
}

//============================================================
//	Y�ʒu���Z�ʂ̐ݒ菈��
//============================================================
void CLifeGauge3D::SetPositionUp(const float fUp)
{
	// ������Y�ʒu���Z�ʂ�ݒ�
	m_fPosUp = fUp;
}

//============================================================
//	�̗͕\���擾����
//============================================================
const CObject *CLifeGauge3D::GetLifeObject(void) const
{
	// �̗͕\�����̃I�u�W�F�N�g��Ԃ�
	return m_pLife;
}

//============================================================
//	�̗͎擾����
//============================================================
int CLifeGauge3D::GetLife(void) const
{
	// �̗͂�Ԃ�
	return m_nLife;
}

//============================================================
//	Y�ʒu���Z�ʎ擾����
//============================================================
float CLifeGauge3D::GetPositionUp(void) const
{
	// Y�ʒu���Z�ʂ�Ԃ�
	return m_fPosUp;
}
