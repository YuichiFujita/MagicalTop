//============================================================
//
//	�n���P�[������ [hurricane.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "hurricane.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"
#include "sceneGame.h"
#include "target.h"

//************************************************************
//	�}�N����`
//************************************************************
#define HURRICANE_COL	(D3DXCOLOR(0.6f, 0.9f, 1.0f, 0.6f))	// �n���P�[���̃|���S���F
#define HURRICANE_PRIO	(5)	// �n���P�[���̗D�揇��

#define HURRICANE_NUMAROUND		(4)			// �n���P�[���̉Q�̎���
#define HURRICANE_PATTERN		(32)		// �n���P�[���̉Q�̕�����
#define HURRICANE_MOVEROT		(-0.4f)		// �n���P�[���̌����̕ύX��
#define HURRICANE_THICKNESS		(22.5f)		// �n���P�[���̃|���S���̑���
#define HURRICANE_OUTERPLUS		(3.5f)		// �n���P�[���̃|���S���O����Y���W���Z��
#define HURRICANE_SETWIDTH		(100.0f)	// �n���P�[���̐������̉������
#define HURRICANE_SETALPHA		(0.8f)		// �n���P�[���̐������̓����x
#define HURRICANE_ADDWIDTH		(3.0f)		// �n���P�[���̉�����̉��Z��
#define HURRICANE_ADDHEIGHT		(3.8f)		// �n���P�[���̏c����̉��Z��
#define HURRICANE_SUBALPHA		(0.01f)		// �n���P�[���̓����x�̌��Z��
#define HURRICANE_GROWWIDTH		(0.0f)		// �n���P�[���̉�����̐�����
#define HURRICANE_GROWHEIGHT	(0.0f)		// �n���P�[���̏c����̐�����
#define HURRICANE_GROWALPHA		(0.0f)		// �n���P�[���̓����x�̐�����

//************************************************************
//	�q�N���X [CHurricane] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CHurricane::CHurricane() : CObjectTornado(CObject::LABEL_NONE, HURRICANE_PRIO)
{
	// �����o�ϐ����N���A
	m_state = STATE_NORMAL;	// ���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CHurricane::~CHurricane()
{

}

//============================================================
//	����������
//============================================================
HRESULT CHurricane::Init(void)
{
	// �����o�ϐ���������
	m_state = STATE_NORMAL;	// ���

	// �����̏�����
	if (FAILED(CObjectTornado::Init()))
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
void CHurricane::Uninit(void)
{
	// �����̏I��
	CObjectTornado::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CHurricane::Update(void)
{
	// �����̍X�V
	CObjectTornado::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CHurricane::Draw(void)
{
	// �����̕`��
	CObjectTornado::Draw();
}

//============================================================
//	��������
//============================================================
CHurricane *CHurricane::Create(const D3DXVECTOR3& rPos)
{
	// �|�C���^��錾
	CHurricane *pHurricane = NULL;	// �n���P�[�������p

	if (UNUSED(pHurricane))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pHurricane = new CHurricane;	// �n���P�[��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pHurricane))
	{ // �m�ۂɐ������Ă���ꍇ

		// �n���P�[���̏�����
		if (FAILED(pHurricane->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pHurricane;
			pHurricane = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`��������
		pHurricane->BindTexture(NONE_IDX);

		// �ʒu��ݒ�
		pHurricane->SetPosition(rPos);

		// ������ݒ�
		pHurricane->SetDirectionRotation(VEC3_ZERO);

		// �F��ݒ�
		pHurricane->SetColor(HURRICANE_COL);

		// �e�̃}�g���b�N�X��ݒ�
		pHurricane->SetMatrixParent(NULL);

		// �|���S���̑�����ݒ�
		pHurricane->SetThickness(HURRICANE_THICKNESS);

		// �|���S���O����y���W���Z�ʂ�ݒ�
		pHurricane->SetOuterPlus(HURRICANE_OUTERPLUS);

		// �������̉�����ʂ�ݒ�
		pHurricane->SetWidth(HURRICANE_SETWIDTH);

		// �������̓����x��ݒ�
		pHurricane->SetAlpha(HURRICANE_SETALPHA);

		// ������̉��Z�ʂ�ݒ�
		pHurricane->SetAddWidth(HURRICANE_ADDWIDTH);

		// �c����̉��Z�ʂ�ݒ�
		pHurricane->SetAddHeight(HURRICANE_ADDHEIGHT);

		// �����x�̌��Z�ʂ�ݒ�
		pHurricane->SetSubAlpha(HURRICANE_SUBALPHA);

		// �J�����O��ݒ�
		pHurricane->SetCulling(D3DCULL_NONE);

		// ���C�e�B���O��ݒ�
		pHurricane->SetLighting(false);

		// ��������ݒ�
		pHurricane->SetGrow(HURRICANE_MOVEROT, HURRICANE_GROWWIDTH, HURRICANE_GROWHEIGHT, HURRICANE_GROWALPHA);

		// �Q��ݒ�
		if (FAILED(pHurricane->SetVortex(HURRICANE_NUMAROUND, HURRICANE_PATTERN)))
		{ // �Q�̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pHurricane;
			pHurricane = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pHurricane;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
