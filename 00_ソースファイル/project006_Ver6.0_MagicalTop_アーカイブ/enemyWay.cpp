//============================================================
//
//	�G�����\������ [enemyWay.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemyWay.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "texture.h"
#include "target.h"

//************************************************************
//	�}�N����`
//************************************************************
#define WAY_PRIO	(2)		// �����\���̗D�揇��
#define FADE_LEVEL	(0.04f)	// �t�F�[�h�̃��l�̉�����

#define RADIUS_MIN_WAY	(420.0f)	// �����\���̑傫���̍ŏ��l
#define RADIUS_MAX_WAY	(750.0f)	// �����\���̑傫���̍ő�l

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CEnemyWay::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\enemyWay000.png",	// �����\���e�N�X�`��
};

//************************************************************
//	�q�N���X [CEnemyWay] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyWay::CEnemyWay(const float fMaxLimitRadius) : CObject3D(CObject::LABEL_WAY, WAY_PRIO), m_fMaxLimitRadius(fMaxLimitRadius)
{
	// �����o�ϐ����N���A
	m_pObject = NULL;		// �����\���I�u�W�F�N�g
	m_state = STATE_SPAWN;	// ���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyWay::~CEnemyWay()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyWay::Init(void)
{
	// �����o�ϐ���������
	m_pObject = NULL;		// �����\���I�u�W�F�N�g
	m_state = STATE_SPAWN;	// ���

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
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
void CEnemyWay::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyWay::Update(void)
{
	// �ϐ���錾
	D3DXCOLOR colWay = GetColor();	// �����\���̐F

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_SPAWN:

		// �����x���グ��
		colWay.a += FADE_LEVEL;

		if (colWay.a >= 1.0f)
		{ // �����x���オ��؂����ꍇ

			// �����x��␳
			colWay.a = 1.0f;

			// ��Ԃ�ݒ�
			m_state = STATE_NORMAL;	// �ʏ���
		}

		break;

	case STATE_NORMAL:

		// ����

		break;

	case STATE_DELETE:

		// �����x��������
		colWay.a -= FADE_LEVEL;

		if (colWay.a <= 0.0f)
		{ // �����x��������؂����ꍇ

			// �����x��␳
			colWay.a = 0.0f;

			// �I�u�W�F�N�g�̏I��
			Uninit();

			// �֐��𔲂���
			return;
		}

		break;

	default:
		assert(false);
		break;
	}

	if (m_state != STATE_DELETE)
	{ // �j����Ԃł͂Ȃ��ꍇ

		// TODO�F�|�C���^�̊m�F�����Ƃ��ꂢ��
		if (CObject::CheckUse(m_pObject) && USED(CScene::GetTarget()))
		{ // �����\���Ώۂ��g�p����Ă��銎�A�^�[�Q�b�g���g�p����Ă���ꍇ

			// �ϐ���錾
			D3DXVECTOR3 posTarget = CScene::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu
			D3DXVECTOR3 posWay = m_pObject->GetPosition();				// �����\���ʒu
			float fDisTarget = sqrtf((posWay.x - posTarget.x) * (posWay.x - posTarget.x) + (posWay.z - posTarget.z) * (posWay.z - posTarget.z)) * 0.5f;	// �^�[�Q�b�g�Ƃ̋���
			float fSizeWay = ((RADIUS_MAX_WAY - RADIUS_MIN_WAY) / m_fMaxLimitRadius) * fDisTarget + RADIUS_MIN_WAY;										// �����\���̑傫��

			// �ʒu��ݒ�
			SetPosition(CScene::GetTarget()->GetPosition());

			// ������ݒ�
			SetRotation(D3DXVECTOR3(0.0f, atan2f(posWay.x - posTarget.x, posWay.z - posTarget.z), 0.0f));

			// �傫����ݒ�
			SetScaling(D3DXVECTOR3(fSizeWay, 0.0, fSizeWay));
		}
	}

	// �F�𔽉f
	SetColor(colWay);

	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyWay::Draw(void)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw();
}

//============================================================
//	��������
//============================================================
CEnemyWay *CEnemyWay::Create(CObject *pObject, const float fMaxLimitRadius)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CEnemyWay *pEnemyWay = NULL;	// �G�����\�������p

	if (UNUSED(pEnemyWay))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pEnemyWay = new CEnemyWay(fMaxLimitRadius);	// �G�����\��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pEnemyWay))
	{ // �m�ۂɐ������Ă���ꍇ

		// �G�����\���̏�����
		if (FAILED(pEnemyWay->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pEnemyWay;
			pEnemyWay = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`����o�^�E����
		pEnemyWay->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_WAY]));

		// ���_��ݒ�
		pEnemyWay->SetOrigin(ORIGIN_CENTER);

		// �ʒu��ݒ�
		pEnemyWay->SetPosition(VEC3_ZERO);

		// ������ݒ�
		pEnemyWay->SetRotation(VEC3_ZERO);

		// �傫����ݒ�
		pEnemyWay->SetScaling(VEC3_ZERO);

		// �F��ݒ�
		pEnemyWay->SetColor(XCOL_AWHITE);

		// �I�u�W�F�N�g��ݒ�
		pEnemyWay->SetObject(pObject);

		// �J�����O��ݒ�
		pEnemyWay->SetCulling(D3DCULL_CCW);

		// ���C�e�B���O��ݒ�
		pEnemyWay->SetLighting(false);

		// Z�e�X�g��ݒ�
		pEnemyWay->SetFunc(D3DCMP_ALWAYS);

		// Z�o�b�t�@�̎g�p�󋵂�ݒ�
		pEnemyWay->SetZEnable(false);

		// �m�ۂ����A�h���X��Ԃ�
		return pEnemyWay;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�폜����
//============================================================
void CEnemyWay::Delete(void)
{
	// �폜��Ԃɂ���
	m_state = STATE_DELETE;
}

//============================================================
//	�I�u�W�F�N�g�̐ݒ菈��
//============================================================
void CEnemyWay::SetObject(CObject *pObject)
{
	// �����̃I�u�W�F�N�g��ݒ�
	m_pObject = pObject;
}
