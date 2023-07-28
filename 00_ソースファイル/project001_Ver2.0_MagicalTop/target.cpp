//============================================================
//
//	�^�[�Q�b�g���� [target.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "target.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "objectMeshCube.h"
#include "lifeGauge3D.h"
#include "shadow.h"
#include "flower.h"
#include "particle3D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define TARG_SHADOW_SIZE	(D3DXVECTOR3(200.0f, 0.0f, 200.0f))	// �e�̑傫��

#define CUBE_SIZE	(D3DXVECTOR3(25.0f, 25.0f, 25.0f))	// �L���[�u�̑傫��
#define CUBE_BORD	(2.5f)		// �L���[�u�̉����̑���
#define TARG_RADIUS	(100.0f)	// �^�[�Q�b�g���a
#define ADD_POS_Y	(92.0f)		// ����ʒu����̃L���[�u���Έʒu�̉��Z��
#define MUL_SIN_POS	(10.0f)		// �T�C���J�[�u�̕␳�W��

#define ADD_SIN_ROT	(D3DXToRadian(1))	// ���V�����̉��Z��
#define ADD_CUBEROT	(D3DXVECTOR3(0.01f, 0.001f, 0.01f))	// �L���[�u�̉�]��

#define TARG_LIFE	(1000)		// �^�[�Q�b�g�̗̑�
#define TARG_POSUP	(160.0f)	// �^�[�Q�b�g��Y�ʒu�̉��Z��
#define STATE_HEAL_CNT	(240)	// �񕜏�ԂɈڍs����܂ł̃J�E���^�[
#define NORMAL_CNT		(60)	// �ʏ��ԂɈڍs����܂ł̃J�E���^�[
#define WAIT_HEAL_CNT	(60)	// �񕜂܂ł̃J�E���^�[

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CTarget::mc_apModelFile[] =	// ���f���萔
{
	"data\\MODEL\\TARGET\\pedestal000.x",	// ������f��
};

//************************************************************
//	�q�N���X [CTarget] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTarget::CTarget() : CObjectModel(CObject::LABEL_TARGET)
{
	// �����o�ϐ����N���A
	m_pMeshCube		= NULL;			// ���b�V���L���[�u�̏��
	m_pLifeGauge	= NULL;			// �̗͂̏��
	m_pShadow		= NULL;			// �e�̏��
	m_state			= STATE_NORMAL;	// ���
	m_fSinRot		= 0.0f;			// ���V����
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nCounterHeal	= 0;			// �񕜊Ǘ��J�E���^�[
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTarget::~CTarget()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTarget::Init(void)
{
	// �����o�ϐ���������
	m_pMeshCube		= NULL;			// ���b�V���L���[�u�̏��
	m_pLifeGauge	= NULL;			// �̗͂̏��
	m_pShadow		= NULL;			// �e�̏��
	m_state			= STATE_NORMAL;	// ���
	m_fSinRot		= 0.0f;			// ���V����
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nCounterHeal	= 0;			// �񕜊Ǘ��J�E���^�[

	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���b�V���L���[�u�̐���
	m_pMeshCube = CObjectMeshCube::Create
	( // ����
		VEC3_ZERO,	// �ʒu
		VEC3_ZERO,	// ����
		CUBE_SIZE,	// �傫��
		XCOL_WHITE,	// �L���[�u�F
		XCOL_BLACK,	// �����F
		CObjectMeshCube::BORDERSTATE_ON,	// �������
		CUBE_BORD,							// ����葾��
		CObjectMeshCube::TEXSTATE_ONE,		// �e�N�X�`�����
		CObjectMeshCube::FACETEX(-1),		// �e�N�X�`�����
		VEC2_ONE,		// �e�N�X�`�������� x
		VEC2_ONE,		// �e�N�X�`�������� y
		VEC2_ONE,		// �e�N�X�`�������� z
		D3DCULL_CCW,	// �J�����O��
		false			// ���C�e�B���O��
	);
	if (UNUSED(m_pMeshCube))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �̗̓Q�[�W3D�̐���
	m_pLifeGauge = CLifeGauge3D::Create(TARG_LIFE, TARG_LIFE, (int)(NORMAL_CNT * 0.5f), TARG_POSUP, this);
	if (UNUSED(m_pLifeGauge))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�̐���
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, TARG_SHADOW_SIZE, this);
	if (UNUSED(m_pShadow))
	{ // �����Ɏ��s�����ꍇ

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
void CTarget::Uninit(void)
{
	// �I�u�W�F�N�g���b�V���L���[�u��j��
	m_pMeshCube->Uninit();

	// �̗̓Q�[�W3D��j��
	m_pLifeGauge->Uninit();

	// �e��j��
	m_pShadow->Uninit();

	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CTarget::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetPosition();				// ����ʒu
	D3DXVECTOR3 rot = m_pMeshCube->GetRotation();	// �L���[�u����
	int nNumFlower = CFlower::GetNumAll();			// �}�i�t�����[�̑���

	// ��ԊǗ�
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NORMAL:	// �ʏ���

		if (m_pLifeGauge->GetLife() < TARG_LIFE && nNumFlower > 0)
		{ // �̗͂��������Ă��銎�A�}�i�t�����[����{�ł������Ă���ꍇ

			if (m_nCounterState < STATE_HEAL_CNT)
			{ // �J�E���^�[�����l��菬�����ꍇ

				// �J�E���^�[�����Z
				m_nCounterState++;
			}
			else
			{ // �J�E���^�[�����l�ȏ�̏ꍇ

				// �J�E���^�[��������
				m_nCounterState = 0;

				// ��Ԃ�ύX
				m_state = STATE_HEAL;	// �񕜏��
			}
		}

		break;

	case STATE_DAMAGE:	// �_���[�W���

		if (m_nCounterState < NORMAL_CNT)
		{ // �J�E���^�[�����l��菬�����ꍇ

			// �J�E���^�[�����Z
			m_nCounterState++;
		}
		else
		{ // �J�E���^�[�����l�ȏ�̏ꍇ

			// �J�E���^�[��������
			m_nCounterState = 0;

			// ��Ԃ�ύX
			m_state = STATE_NORMAL;	// �ʏ���
		}

		break;

	case STATE_HEAL:	// �񕜏��

		if (m_pLifeGauge->GetLife() < TARG_LIFE)
		{ // �̗͂��ő�l��菭�Ȃ��ꍇ

			if (m_nCounterHeal < WAIT_HEAL_CNT)
			{ // �J�E���^�[�����l��菬�����ꍇ

				// �J�E���^�[�����Z
				m_nCounterHeal++;
			}
			else
			{ // �J�E���^�[�����l�ȏ�̏ꍇ

				// �J�E���^�[��������
				m_nCounterHeal = 0;

				// �̗͂���
				m_pLifeGauge->AddLife(nNumFlower);	// �}�i�t�����[�ʂɉ����ĉ񕜗ʑ���
			}
		}
		else
		{ // �̗͂��S�񕜂����ꍇ

			// ��Ԃ�ύX
			m_state = STATE_NORMAL;	// �ʏ���
		}

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �L���[�u�̏c�ʒu��ύX
	pos.y += ADD_POS_Y + sinf(m_fSinRot) * MUL_SIN_POS;

	// �L���[�u�̌�����ύX
	rot += ADD_CUBEROT;

	// �L���[�u�̈ʒu�E�����𔽉f
	m_pMeshCube->SetPosition(pos);
	m_pMeshCube->SetRotation(rot);

	// ���V���������Z
	m_fSinRot += ADD_SIN_ROT;
	useful::NormalizeRot(m_fSinRot);	// �����␳

	// ���b�V���L���[�u�̍X�V
	m_pMeshCube->Update();

	// �̗̓Q�[�W3D�̍X�V
	m_pLifeGauge->Update();

	// �e�̍X�V
	m_pShadow->Update();

	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CTarget::Draw(void)
{
	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw();
}

//============================================================
//	�q�b�g����
//============================================================
void CTarget::Hit(const int nDmg)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetPosition();	// �^�[�Q�b�g�ʒu

	if (IsDeath() != true)
	{ // ���S�t���O�������Ă��Ȃ��ꍇ

		if (m_state == STATE_NORMAL || m_state == STATE_HEAL)
		{ // �ʏ��Ԃ܂��͉񕜏�Ԃ̏ꍇ

			// �̗͂���_���[�W�����Z
			m_pLifeGauge->AddLife(-nDmg);

			if (m_pLifeGauge->GetLife() > 0)
			{ // �����Ă���ꍇ

				// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
				CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos);

				// �J�E���^�[��������
				m_nCounterState = 0;

				// ��Ԃ�ύX
				m_state = STATE_DAMAGE;		// �_���[�W���
			}
			else
			{ // ����ł���ꍇ

				// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
				CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f));
				CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos, D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f));

				// �X�V�ƕ`����~
				SetEnableUpdate(false);
				SetEnableDraw(false);

				// �J�E���^�[��������
				m_nCounterState = 0;

				// ��Ԃ�ύX
				m_state = STATE_DESTROY;	// �j����
			}
		}
	}
}

//============================================================
//	��������
//============================================================
CTarget *CTarget::Create
(
	const MODEL model,			// ���f��
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR3& rScale	// �傫��
)
{
	// �ϐ���錾
	int nModelID;	// ���f���C���f�b�N�X

	// �|�C���^��錾
	CModel *pModel = CManager::GetModel();	// ���f���ւ̃|�C���^
	CTarget *pTarget = NULL;	// �^�[�Q�b�g�����p

	if (UNUSED(pTarget))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pTarget = new CTarget;	// �^�[�Q�b�g
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pTarget))
	{ // �g�p����Ă���ꍇ
		
		// �^�[�Q�b�g�̏�����
		if (FAILED(pTarget->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pTarget;
			pTarget = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// ���f����o�^
		nModelID = pModel->Regist(mc_apModelFile[model]);

		// ���f��������
		pTarget->BindModel(pModel->GetModel(nModelID));

		// �ʒu��ݒ�
		pTarget->SetPosition(rPos);

		// ������ݒ�
		pTarget->SetRotation(rRot);

		// �傫����ݒ�
		pTarget->SetScaling(rScale);

		// ���b�V���L���[�u�̈ʒu��ݒ�
		pTarget->m_pMeshCube->SetPosition(D3DXVECTOR3(rPos.x, rPos.y + ADD_POS_Y, rPos.z));

		// �m�ۂ����A�h���X��Ԃ�
		return pTarget;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�̗͂̉��Z����
//============================================================
void CTarget::AddLife(const int nAdd)
{
	// �ϐ���錾
	int nLife = m_pLifeGauge->GetLife();	// ���݂̗̑�

	// �̗͂Ɉ����̒l�����Z
	nLife += nAdd;

	if (nLife < 1)
	{ // ����ł��܂����ꍇ

		// ���ʐ��O�̗̑͂ɕ␳
		nLife = 1;
	}

	// �̗͂�ݒ�
	m_pLifeGauge->SetLife(nLife);
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CTarget::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);		// ���g
	m_pMeshCube->SetEnableUpdate(bUpdate);	// ���b�V���L���[�u
	m_pLifeGauge->SetEnableUpdate(bUpdate);	// �̗̓Q�[�W
	m_pShadow->SetEnableUpdate(bUpdate);	// �e
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CTarget::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);			// ���g
	m_pMeshCube->SetEnableDraw(bDraw);		// ���b�V���L���[�u
	m_pLifeGauge->SetEnableDraw(bDraw);		// �̗̓Q�[�W
	m_pShadow->SetEnableDraw(bDraw);		// �e
}

//============================================================
//	��Ԏ擾����
//============================================================
int CTarget::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	���a�擾����
//============================================================
float CTarget::GetRadius(void) const
{
	// ���a��Ԃ�
	return TARG_RADIUS;
}
