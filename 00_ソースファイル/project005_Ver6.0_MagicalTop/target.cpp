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
#include "sound.h"
#include "model.h"
#include "stage.h"
#include "field.h"
#include "scene.h"
#include "sceneGame.h"
#include "waveManager.h"
#include "objectMeshCube.h"
#include "objectGauge3D.h"
#include "shadow.h"
#include "warningAttack.h"
#include "flower.h"
#include "particle3D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define TARG_SHADOW_SIZE	(D3DXVECTOR3(200.0f, 0.0f, 200.0f))	// �e�̑傫��

#define CUBE_COL	(D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f))	// �L���[�u�̐F
#define CUBE_SIZE	(D3DXVECTOR3(25.0f, 25.0f, 25.0f))	// �L���[�u�̑傫��
#define CUBE_BORD	(2.5f)		// �L���[�u�̉����̑���
#define TARG_RADIUS	(100.0f)	// �^�[�Q�b�g���a
#define ADD_POS_Y	(92.0f)		// ����ʒu����̃L���[�u���Έʒu�̉��Z��
#define MUL_SIN_POS	(10.0f)		// �T�C���J�[�u�̕␳�W��

#define ADD_SIN_ROT	(D3DXToRadian(1))	// ���V�����̉��Z��
#define ADD_CUBEROT	(D3DXVECTOR3(0.01f, 0.001f, 0.01f))	// �L���[�u�̉�]��

#define TARG_LIFE		(2000)		// �^�[�Q�b�g�̗̑�
#define GAUGE_PLUS_Y	(160.0f)	// �^�[�Q�b�g��Y�ʒu�̉��Z��
#define GAUGE_GAUGESIZE	(D3DXVECTOR3(100.0f, 20.0f, 0.0f))	// �Q�[�W�T�C�Y
#define GAUGE_FRAMESIZE	(D3DXVECTOR3(102.5f, 22.5f, 0.0f))	// �Q�[�W�t���[���T�C�Y
#define GAUGE_FRONTCOL	(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	// �\�Q�[�W�F
#define GAUGE_BACKCOL	(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	// ���Q�[�W�F

#define WARNING_POS		(D3DXVECTOR3(250.0f, 610.0f, 0.0f))	// �x���\���̈ʒu
#define WARNING_SIZE	(D3DXVECTOR3(400.0f, 200.0f, 0.0f))	// �x���\���̑傫��

#define STATE_HEAL_CNT	(480)	// �񕜏�ԂɈڍs����܂ł̃J�E���^�[
#define NORMAL_CNT		(60)	// �ʏ��ԂɈڍs����܂ł̃J�E���^�[
#define WAIT_HEAL_CNT	(180)	// �񕜂܂ł̃J�E���^�[
#define HEAL_EFF_COL	(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	// �񕜎��̃G�t�F�N�g�F

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
	m_pLife			= NULL;			// �̗͂̏��
	m_pShadow		= NULL;			// �e�̏��
	m_pWarning		= NULL;			// �U���x���\���̏��
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
	m_pLife			= NULL;			// �̗͂̏��
	m_pShadow		= NULL;			// �e�̏��
	m_pWarning		= NULL;			// �U���x���\���̏��
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
		CUBE_COL,	// �L���[�u�F
		XCOL_BLACK,	// �����F
		CObjectMeshCube::BORDERSTATE_ON,	// �������
		CUBE_BORD,							// ����葾��
		CObjectMeshCube::TEXSTATE_ONE,		// �e�N�X�`�����
		CObjectMeshCube::FACETEX(NONE_IDX),	// �e�N�X�`�����
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
	m_pLife = CObjectGauge3D::Create
	( // ����
		CObject::LABEL_GAUGE,			// �I�u�W�F�N�g���x��
		this,							// �Q�[�W�\���I�u�W�F�N�g
		TARG_LIFE,						// �ő�\���l
		(int)(NORMAL_CNT * 0.5f),		// �\���l�ϓ��t���[��
		GAUGE_PLUS_Y,					// �\��Y�ʒu�̉��Z��
		GAUGE_GAUGESIZE,				// �Q�[�W�傫��
		GAUGE_FRONTCOL,					// �\�Q�[�W�F
		GAUGE_BACKCOL,					// ���Q�[�W�F
		true,							// �g�`���
		CObjectGauge3D::TYPE_TARGET,	// �g���
		GAUGE_FRAMESIZE					// �g�傫��
	);
	if (UNUSED(m_pLife))
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

	// �U���x���\���̐���
	m_pWarning = CWarningAttack::Create(WARNING_POS, WARNING_SIZE);
	if (UNUSED(m_pWarning))
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
	bool bUpdate = true;	// �X�V��

	if (CManager::GetMode() == CScene::MODE_GAME)
	{ // ���[�h���Q�[���̏ꍇ

		if (CSceneGame::GetWaveManager()->GetState() != CWaveManager::STATE_PROGRESSION)
		{ // �E�F�[�u�i�s��Ԃł͂Ȃ��ꍇ

			// �X�V�ł��Ȃ���Ԃɂ���
			bUpdate = false;
		}
	}

	if (bUpdate)
	{ // �X�V�ł����Ԃ̏ꍇ

		// ��ԊǗ�
		switch (m_state)
		{ // ��Ԃ��Ƃ̏���
		case STATE_NORMAL:	// �ʏ���

			if (m_pLife->GetNum() < TARG_LIFE && nNumFlower > 0)
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

		case STATE_HEAL:	// �񕜏��

			if (m_pLife->GetNum() < TARG_LIFE)
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
					m_pLife->AddNum(nNumFlower * 2);	// �}�i�t�����[�ʂɉ����ĉ񕜗ʑ���

					// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
					CParticle3D::Create(CParticle3D::TYPE_HEAL, pos, HEAL_EFF_COL);
				}
			}
			else
			{ // �̗͂��S�񕜂����ꍇ

				// ��Ԃ�ύX
				m_state = STATE_NORMAL;	// �ʏ���
			}

			break;

		case STATE_DESTROY:	// �j����

			// ����

			break;

		default:	// ��O����
			assert(false);
			break;
		}
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
			m_pLife->AddNum(-nDmg);

			if (m_pLife->GetNum() > 0)
			{ // �����Ă���ꍇ

				// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
				CParticle3D::Create(CParticle3D::TYPE_SMALL_EXPLOSION, pos);

				// �J�E���^�[��������
				m_nCounterState = 0;

				// �U���x����\��������
				m_pWarning->SetDisp();
			}
			else
			{ // ����ł���ꍇ

				// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
				CParticle3D::Create(CParticle3D::TYPE_BIG_EXPLOSION, pos);

				// �`����~
				m_pMeshCube->SetEnableDraw(false);

				// �J�E���^�[��������
				m_nCounterState = 0;

				// ��Ԃ�ύX
				m_state = STATE_DESTROY;	// �j����

				// �T�E���h�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_BREAK);	// �j��
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
	D3DXVECTOR3 pos = rPos;	// ���W�ݒ�p
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
		CScene::GetStage()->LimitPosition(pos, TARG_RADIUS);	// �X�e�[�W�͈͊O�␳
		pos.y = CScene::GetField()->GetPositionHeight(pos);		// ������n�ʂɐݒ�
		pTarget->SetPosition(pos);

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
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CTarget::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);		// ���g
	m_pMeshCube->SetEnableUpdate(bUpdate);	// ���b�V���L���[�u
	m_pLife->SetEnableUpdate(bUpdate);		// �̗�
	m_pShadow->SetEnableUpdate(bUpdate);	// �e
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CTarget::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);		// ���g
	m_pMeshCube->SetEnableDraw(bDraw);	// ���b�V���L���[�u
	m_pLife->SetEnableDraw(bDraw);		// �̗�
	m_pShadow->SetEnableDraw(bDraw);	// �e
}

//============================================================
//	�̗͂̕`��󋵂̐ݒ菈��
//============================================================
void CTarget::SetEnableDrawLife(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	m_pLife->SetEnableDraw(bDraw);	// �̗�
}

//============================================================
//	�̗͂̉��Z����
//============================================================
void CTarget::AddLife(const int nAdd)
{
	// �ϐ���錾
	int nLife = m_pLife->GetNum();	// ���݂̗̑�

	// �̗͂Ɉ����̒l�����Z
	nLife += nAdd;

	if (nLife < 1)
	{ // ����ł��܂����ꍇ

		// ���ʐ��O�̗̑͂ɕ␳
		nLife = 1;
	}

	// �̗͂�ݒ�
	m_pLife->SetNum(nLife);
}

//============================================================
//	�̗͐ݒ�̐ݒ菈��
//============================================================
void CTarget::SetLife(const int nLife)
{
	// �����̗̑͂�ݒ�
	m_pLife->SetNum(nLife);
}

//============================================================
//	�̗͎擾����
//============================================================
int CTarget::GetLife(void) const
{
	// �̗͂�Ԃ�
	return m_pLife->GetNum();
}

//============================================================
//	�ő�̗͎擾����
//============================================================
int CTarget::GetMaxLife(void) const
{
	// �ő�̗͂�Ԃ�
	return m_pLife->GetMaxNum();
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
