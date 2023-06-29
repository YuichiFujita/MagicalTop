//============================================================
//
//	�G���� [enemy.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "multiModel.h"
#include "debugproc.h"
#include "collision.h"
#include "player.h"
#include "target.h"
#include "bullet.h"
#include "score.h"
#include "particle3D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define ENEMY_SETUP_TXT	"data\\TXT\\enemy.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CEnemy::StatusInfo CEnemy::m_aStatusInfo[CEnemy::TYPE_MAX] = {};	// �X�e�[�^�X���
CEnemy::PartsInfo CEnemy::m_aPartsInfo[CEnemy::TYPE_MAX] = {};		// �p�[�c���

const char *CEnemyCar::mc_apModelFile[] =	// ��ԃ��f���萔
{
	"data\\MODEL\\caterpillar000.x",	// �L���^�s��
	"data\\MODEL\\cannon000.x",			// �L���m��
};

//************************************************************
//	�q�N���X [CEnemy] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemy::CEnemy(const TYPE type) : CObject(CObject::LABEL_ENEMY), m_status(m_aStatusInfo[type]), m_parts(m_aPartsInfo[type])
{
	// �����o�ϐ����N���A
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ���[���h�}�g���b�N�X
	m_pos		= VEC3_ZERO;	// ���݈ʒu
	m_oldPos	= VEC3_ZERO;	// �ߋ��ʒu
	m_movePos	= VEC3_ZERO;	// �ʒu�ړ���
	m_rot		= VEC3_ZERO;	// ����
	m_moveRot	= VEC3_ZERO;	// �����ύX��
	m_nLife		= 0;			// �̗�
	m_nCounterAtk = 0;			// �U���Ǘ��J�E���^�[
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemy::~CEnemy()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemy::Init(void)
{
	// �����o�ϐ���������
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ���[���h�}�g���b�N�X
	m_pos		= VEC3_ZERO;		// ���݈ʒu
	m_oldPos	= VEC3_ZERO;		// �ߋ��ʒu
	m_movePos	= VEC3_ZERO;		// �ʒu�ړ���
	m_rot		= VEC3_ZERO;		// ����
	m_moveRot	= VEC3_ZERO;		// �����ύX��
	m_nLife		= m_status.nLife;	// �̗�
	m_nCounterAtk = 0;				// �U���Ǘ��J�E���^�[

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemy::Uninit(void)
{
	// �G��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CEnemy::Update(void)
{
	// �ߋ��ʒu���X�V
	m_oldPos = m_pos;
}

//============================================================
//	�`�揈��
//============================================================
void CEnemy::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//============================================================
//	�q�b�g����
//============================================================
void CEnemy::Hit(const int nDmg)
{
	// �̗͂���_���[�W�����Z
	m_nLife -= nDmg;

	if (m_nLife > 0)
	{ // �����Ă���ꍇ

		// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, m_pos);
	}
	else
	{ // ����ł���ꍇ

		// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, m_pos, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f));
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, m_pos, D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f));

		// �X�R�A�����Z
		CManager::GetScore()->Add(m_status.nScore);

		// �G�I�u�W�F�N�g�̏I��
		Uninit();
	}
}

//============================================================
//	��������
//============================================================
CEnemy *CEnemy::Create(const TYPE type, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot)
{
	// �|�C���^��錾
	CEnemy *pEnemy = NULL;	// �G�����p

	if (UNUSED(pEnemy))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		switch (type)
		{ // ��ނ��Ƃ̏���
		case TYPE_CAR:	// ���

			// ��Ԃ𐶐�
			pEnemy = new CEnemyCar(type);

			break;

		default:	// ��O����
			assert(false);
			break;
		}
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pEnemy))
	{ // �g�p����Ă���ꍇ
		
		if (SUCCEEDED(pEnemy->GetResult()))
		{ // �I�u�W�F�N�g�̏��ݒ�ɐ��������ꍇ

			// �G�̏�����
			if (FAILED(pEnemy->Init()))
			{ // �������Ɏ��s�����ꍇ

				// �������J��
				delete pEnemy;
				pEnemy = NULL;

				// ���s��Ԃ�
				return NULL;
			}

			// �ʒu��ݒ�
			pEnemy->SetPosition(rPos);

			// ������ݒ�
			pEnemy->SetRotation(rRot);

			// �m�ۂ����A�h���X��Ԃ�
			return pEnemy;
		}
		else
		{ // �I�u�W�F�N�g�̏��ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pEnemy;
			pEnemy = NULL;

			// ���s��Ԃ�
			return NULL;
		}
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CEnemy::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CEnemy::SetRotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::NormalizeRot(m_rot.x);
	useful::NormalizeRot(m_rot.y);
	useful::NormalizeRot(m_rot.z);
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CEnemy::GetPosition(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�ߋ��ʒu�擾����
//============================================================
D3DXVECTOR3 CEnemy::GetOldPosition(void) const
{
	// �ߋ��ʒu��Ԃ�
	return m_oldPos;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CEnemy::GetRotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	���a�擾����
//============================================================
float CEnemy::GetRadius(void) const
{
	// ���a��Ԃ�
	return m_status.fRadius;
}

//============================================================
//	�X�e�[�^�X���擾����
//============================================================
CEnemy::StatusInfo CEnemy::GetStatusInfo(void) const
{
	// �X�e�[�^�X����Ԃ�
	return m_status;
}

//============================================================
//	�p�[�c���擾����
//============================================================
CEnemy::PartsInfo CEnemy::GetPartsInfo(void) const
{
	// �p�[�c����Ԃ�
	return m_parts;
}

//============================================================
//	���m�͈͂̓����蔻��
//============================================================
void CEnemy::CollisionFind(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posLook;	// ���F�Ώۈʒu
	float fPlayerRadius = CManager::GetPlayer()->GetRadius();	// �v���C���[���a

	// TODO�F�v���C���[���񂾂�Manager��Player��NULL�ɂ���
	if (USED(CManager::GetPlayer()) && USED(CManager::GetTarget()))
	{ // �v���C���[�E�^�[�Q�b�g���g�p����Ă���ꍇ

		// ���F�Ώۂ̐ݒ�
		if (collision::Circle(CManager::GetPlayer()->GetPosition(), m_pos, fPlayerRadius, m_status.fFindRadius) == false)
		{ // �G�̌��m�͈͊O�̏ꍇ

			// ���F�Ώۈʒu��ݒ�
			posLook = CManager::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu
		}
		else
		{ // �G�̌��m�͈͓��̏ꍇ

			// ���F�Ώۈʒu��ݒ�
			posLook = CManager::GetPlayer()->GetPosition();	// �v���C���[�ʒu
		}

		// �Ώۂ̕�������������
		Look(posLook);

		if (collision::Circle(posLook, m_pos, fPlayerRadius, m_status.fAttackRadius) == false)
		{ // �G�̍U���͈͊O�̏ꍇ

			// �Ώۂ̕����Ɉړ� (�O�i)
			m_pos.x -= sinf(m_rot.y) * m_status.fForwardMove;
			m_pos.z -= cosf(m_rot.y) * m_status.fForwardMove;

			// �ʒu�␳
			Limit(m_pos);
		}
		else
		{ // �G�̍U���͈͓��̏ꍇ

			if (collision::Circle(posLook, m_pos, fPlayerRadius, m_status.fBackwardRadius) == true && m_status.bBackward == true)
			{ // �G�̌�ޔ͈͓����A��ނ�ON�̏ꍇ

				// �Ώۂ̋t�����Ɉړ� (���)
				m_pos.x += sinf(m_rot.y) * m_status.fBackwardMove;
				m_pos.z += cosf(m_rot.y) * m_status.fBackwardMove;

				// �ʒu�␳
				Limit(m_pos);
			}

			// �U��
			Attack(m_pos, D3DXVECTOR3(80.0f, 80.0f, 0.0f), D3DXVECTOR3(m_rot.x + (-D3DX_PI * 0.5f), m_rot.y, 0.0f));
		}
	}
}

//============================================================
//	�Ώێ��F����
//============================================================
void CEnemy::Look(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	float fDestRot = 0.0f;	// �ڕW����
	float fDiffRot = 0.0f;	// ����

	// �v���C���[�̌�������
	fDestRot = atan2f(m_pos.x - rPos.x, m_pos.z - rPos.z);	// �ڕW����

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = fDestRot - m_rot.y;

	// ���������̐��K��
	useful::NormalizeRot(fDiffRot);

	// �����̍X�V
	m_rot.y += fDiffRot * m_status.fLookRevision;

	// �����̐��K��
	useful::NormalizeRot(m_rot.y);
}

//============================================================
//	�ʒu��������
//============================================================
void CEnemy::Limit(D3DXVECTOR3& rPos)
{
	// �͈͊O�␳
	// TODO�F�͈͊O�␳����
	useful::LimitNum(rPos.x, -2000.0f, 2000.0f);
	useful::LimitNum(rPos.z, -2000.0f, 2000.0f);
}

//============================================================
//	�U������
//============================================================
void CEnemy::Attack(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const D3DXVECTOR3& rRot)
{
	// �J�E���^�[�����Z
	m_nCounterAtk++;

	if (m_nCounterAtk >= m_status.nCounterAttack)
	{ // �J�E���^�[�����l�ȏ�̏ꍇ

		// �J�E���^�[��������
		m_nCounterAtk = 0;

		// �e�I�u�W�F�N�g�̐���
		CBullet::Create
		( // ����
			CBullet::TYPE_ENEMY,	// ���
			rPos,					// �ʒu
			rSize,					// �傫��
			XCOL_WHITE,				// �F
			rRot,					// �ˌ�����
			m_status.fBullMove,		// �ړ����x
			m_status.nBullLife		// ����
		);
	}
}

//============================================================
//	�G�Ƃ̓����蔻��
//============================================================
void CEnemy::CollisionEnemy(D3DXVECTOR3& rPos, D3DXVECTOR3& rOldPos)
{
	// TODO�F�����̊m�F
	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{ // �I�u�W�F�N�g�̑������J��Ԃ�

			// �ϐ���錾
			D3DXVECTOR3 posEnemy;	// �G�̈ʒu
			D3DXVECTOR3 sizeEnemy;	// �G�̑傫��
			D3DXVECTOR3 sizeSelf;	// ���g�̑傫��

			// �|�C���^��錾
			CObject *pObject = CObject::GetObject(nCntPri, nCntObject);	// �I�u�W�F�N�g

			if (UNUSED(pObject)
			||  pObject->GetLabel() != CObject::LABEL_ENEMY)
			{ // �I�u�W�F�N�g����g�p���E���x�����G�ł͂Ȃ��ꍇ

				// ���̌J��Ԃ��Ɉڍs
				continue;
			}

			if (pObject->GetID() == GetID()
			&&  pObject->GetPriority() == GetPriority())
			{ // �I�u�W�F�N�g���������g�������ꍇ

				// ���̌J��Ԃ��Ɉڍs
				continue;
			}

			// �G�̏����擾
			posEnemy  = pObject->GetPosition();			// �G�̈ʒu
			sizeEnemy = VEC3_ALL(pObject->GetRadius());	// �G�̑傫��
			sizeSelf  = VEC3_ALL(m_status.fRadius);		// ���g�̑傫��

			// �Փ˔���
			collision::Pillar
			( // ����
				rPos,		// ����ʒu
				rOldPos,	// ����ߋ��ʒu
				posEnemy,	// ����ڕW�ʒu
				sizeSelf,	// ����T�C�Y(�E�E��E��)
				sizeSelf,	// ����T�C�Y(���E���E�O)
				sizeEnemy,	// ����ڕW�T�C�Y(�E�E��E��)
				sizeEnemy	// ����ڕW�T�C�Y(���E���E�O)
			);
		}
	}
}

//************************************************************
//	�q�N���X [CEnemyCar] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyCar::CEnemyCar(const TYPE type) : CEnemy(type)
{
	// �����o�ϐ����N���A
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// ���f���̏��
	m_nNumModel = 0;	// �p�[�c�̑���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyCar::~CEnemyCar()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyCar::Init(void)
{
	// �ϐ���錾
	PartsInfo parts = GetPartsInfo();	// �p�[�c���
	int nModelID;	// ���f���C���f�b�N�X

	// �|�C���^��錾
	CModel *pModel = CManager::GetModel();	// ���f���ւ̃|�C���^

	// �G�̏�����
	if (FAILED(CEnemy::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �����o�ϐ���������
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// ���f���̏��
	m_nNumModel = 0;	// �p�[�c�̑���

	// �p�[�c������
	m_nNumModel = parts.nNumParts;

	for (int nCntEnemy = 0; nCntEnemy < m_nNumModel; nCntEnemy++)
	{ // �p�[�c�����J��Ԃ�

		// ���f���̐���
		m_apMultiModel[nCntEnemy] = CMultiModel::Create(parts.aInfo[nCntEnemy].pos, parts.aInfo[nCntEnemy].rot);

		// ���f���̓o�^�E����
		nModelID = pModel->Regist(mc_apModelFile[(MODEL)nCntEnemy]);
		m_apMultiModel[nCntEnemy]->BindModel(pModel->GetModel(nModelID));

		// �e���f���̐ݒ�
		if (parts.aInfo[nCntEnemy].nParentID == NONE_IDX)
		{ // �e���Ȃ��ꍇ

			// NULL��ݒ�
			m_apMultiModel[nCntEnemy]->SetParent(NULL);
		}
		else
		{ // �e������ꍇ

			// �e�̃A�h���X��ݒ�
			m_apMultiModel[nCntEnemy]->SetParent(m_apMultiModel[parts.aInfo[nCntEnemy].nParentID]);
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyCar::Uninit(void)
{
	// �}���`���f����j��
	for (int nCntEnemy = 0; nCntEnemy < MAX_PARTS; nCntEnemy++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		if (USED(m_apMultiModel[nCntEnemy]))
		{ // �p�[�c���g�p���̏ꍇ

			// �j������
			if (FAILED(m_apMultiModel[nCntEnemy]->Release(m_apMultiModel[nCntEnemy])))
			{ // �j���Ɏ��s�����ꍇ

				// ��O����
				assert(false);
			}
		}
	}

	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyCar::Update(void)
{
	// �G�̍X�V
	CEnemy::Update();

	// �G�̓���̍X�V
	CollisionFind();
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyCar::Draw(void)
{
	// �G�̕`��
	CEnemy::Draw();

	for (int nCntEnemy = 0; nCntEnemy < m_nNumModel; nCntEnemy++)
	{ // �p�[�c�̑������J��Ԃ�

		// �p�[�c�̕`��
		m_apMultiModel[nCntEnemy]->Draw();
	}
}

//============================================================
//	���m�͈͂̓����蔻��
//============================================================
void CEnemyCar::CollisionFind(void)
{
	// �ϐ���錾
	StatusInfo  status		= GetStatusInfo();	// �G�X�e�[�^�X
	D3DXVECTOR3 posEnemy	= GetPosition();	// �G�ʒu
	D3DXVECTOR3 rotEnemy	= GetRotation();	// �G����
	D3DXVECTOR3 posLook		= VEC3_ZERO;		// ���F�Ώۈʒu
	D3DXVECTOR3 rotCannon	= VEC3_ZERO;		// �L���m������
	float fPlayerRadius = CManager::GetPlayer()->GetRadius();	// �v���C���[���a

	// TODO�F�ړ��̊m�F�E�ړ��ʁA�����ύX�ʂ��g��
	if (USED(CManager::GetPlayer()) && USED(CManager::GetTarget()))
	{ // �v���C���[�E�^�[�Q�b�g���g�p����Ă���ꍇ

		// ���F�Ώۂ̌��m����
		if (collision::Circle(CManager::GetPlayer()->GetPosition(), posEnemy, fPlayerRadius, status.fFindRadius) == false)
		{ // �G�̌��m�͈͊O�̏ꍇ

			// ���F�Ώۈʒu��ݒ�
			posLook = CManager::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu
		}
		else
		{ // �G�̌��m�͈͓��̏ꍇ

			// ���F�Ώۈʒu��ݒ�
			posLook = CManager::GetPlayer()->GetPosition();	// �v���C���[�ʒu
		}

		// ���F�Ώۂ̍U������
		if (collision::Circle(posLook, posEnemy, fPlayerRadius, status.fAttackRadius) == false)
		{ // �G�̍U���͈͊O�̏ꍇ

			// �Ώۂ̕�������������
			Look(posLook);

			// �������擾
			rotEnemy = GetRotation();

			// �Ώۂ̕����Ɉړ� (�O�i)
			posEnemy.x -= sinf(rotEnemy.y) * status.fForwardMove;
			posEnemy.z -= cosf(rotEnemy.y) * status.fForwardMove;
		}
		else
		{ // �G�̍U���͈͓��̏ꍇ

			if (collision::Circle(posLook, posEnemy, fPlayerRadius, status.fBackwardRadius) == true && status.bBackward == true)
			{ // �G�̌�ޔ͈͓����A��ނ�ON�̏ꍇ

				// �Ώۂ̕�������������
				Look(posLook);

				// �������擾
				rotEnemy = GetRotation();

				// �Ώۂ̋t�����Ɉړ� (���)
				posEnemy.x += sinf(rotEnemy.y) * status.fBackwardMove;
				posEnemy.z += cosf(rotEnemy.y) * status.fBackwardMove;
			}
			else
			{ // ��ޏ�Ԃł͂Ȃ��ꍇ

				// �ϐ���錾
				float fDestRot = 0.0f;	// �ڕW����
				float fDiffRot = 0.0f;	// ����

				// �L���m���̌������擾
				rotCannon = m_apMultiModel[MODEL_CANNON]->GetRotation() + GetRotation();	// �{�̂̌��������Z
				useful::NormalizeRot(rotCannon.y);	// �L���m�������̐��K��

				// �ڕW�����̌v�Z
				fDestRot = atan2f(posEnemy.x - posLook.x, posEnemy.z - posLook.z);
				useful::NormalizeRot(fDestRot);		// �ڕW�����̐��K��

				// ���������̌v�Z
				fDiffRot = fDestRot - rotCannon.y;
				useful::NormalizeRot(fDiffRot);		// ���������̐��K��

				// �����̍X�V
				rotCannon.y += fDiffRot * 0.03f;	// TODO�Fcannon�̌����␳�l���
				useful::NormalizeRot(rotCannon.y);	// �L���m�������̐��K��

				// ������ݒ�
				m_apMultiModel[MODEL_CANNON]->SetRotation(rotCannon - GetRotation());		// �{�̂̌��������Z
			}
		}

		// �ʒu�␳
		Limit(posEnemy);

		// �G�Ƃ̓����蔻��
		CollisionEnemy(posEnemy, GetOldPosition());

		// �U��
		Attack
		( // ����
			posEnemy,						// �e�̔��ˈʒu
			VEC3_ALL(status.fBullRadius),	// �e�̑傫��
			D3DXVECTOR3						// �e�̔��ˌ���
			( // ����
				rotCannon.x + (-D3DX_PI * 0.5f),	// x
				rotCannon.y,						// y
				0.0f								// z
			)
		);
	}

	// �ʒu�𔽉f
	SetPosition(posEnemy);

	// �����𔽉f
	SetRotation(rotEnemy);
}

//************************************************************
//	�q�N���X [CEnemy] �̃Z�b�g�A�b�v�֐�
//************************************************************
//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CEnemy::LoadSetup(void)
{
	// �ϐ���錾
	int nType		= 0;	// ��ނ̑���p
	int nID			= 0;	// �C���f�b�N�X�̑���p
	int nBackward	= 0;	// ��ނ�ON/OFF�̕ϊ��p
	int nEnd		= 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(ENEMY_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �L�����N�^�[�̐ݒ�
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // �ǂݍ��񂾕����� CHARACTERSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "ENEMYSET") == 0)
					{ // �ǂݍ��񂾕����� ENEMYSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_ENEMYSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
							}
							else if (strcmp(&aString[0], "PARTSSET") == 0)
							{ // �ǂݍ��񂾕����� PARTSSET �̏ꍇ

								do
								{ // �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

									// �t�@�C�����當�����ǂݍ���
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "INDEX") == 0)
									{ // �ǂݍ��񂾕����� INDEX �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%d", &nID);			// ���f���̃C���f�b�N�X��ǂݍ���

										// �p�[�c�������Z
										m_aPartsInfo[nType].nNumParts++;
									}
									else if (strcmp(&aString[0], "PARENT") == 0)
									{ // �ǂݍ��񂾕����� PARENT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%d", &m_aPartsInfo[nType].aInfo[nID].nParentID);	// ���f���̐e�̃C���f�b�N�X��ǂݍ���
									}
									else if (strcmp(&aString[0], "POS") == 0)
									{ // �ǂݍ��񂾕����� POS �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].pos.x);		// X���W��ǂݍ���
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].pos.y);		// Y���W��ǂݍ���
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].pos.z);		// Z���W��ǂݍ���
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // �ǂݍ��񂾕����� ROT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);								// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].rot.x);		// X������ǂݍ���
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].rot.y);		// Y������ǂݍ���
										fscanf(pFile, "%f", &m_aPartsInfo[nType].aInfo[nID].rot.z);		// Z������ǂݍ���
									}
								} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v
							}
						} while (strcmp(&aString[0], "END_ENEMYSET") != 0);	// �ǂݍ��񂾕����� END_ENEMYSET �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v
			}

			// �X�e�[�^�X�̐ݒ�
			else if (strcmp(&aString[0], "STATUSSET") == 0)
			{ // �ǂݍ��񂾕����� STATUSSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_STATUSSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "ENEMYSET") == 0)
					{ // �ǂݍ��񂾕����� ENEMYSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_ENEMYSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
							}
							else if (strcmp(&aString[0], "LIFE") == 0)
							{ // �ǂݍ��񂾕����� LIFE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nLife);	// �̗͂�ǂݍ���
							}
							else if (strcmp(&aString[0], "SCORE") == 0)
							{ // �ǂݍ��񂾕����� SCORE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nScore);	// �X�R�A���Z�ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fRadius);	// ���a��ǂݍ���
							}
							else if (strcmp(&aString[0], "HEIGHT") == 0)
							{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fHeight);	// �c����ǂݍ���
							}
							else if (strcmp(&aString[0], "BULLET_LIFE") == 0)
							{ // �ǂݍ��񂾕����� BULLET_LIFE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nBullLife);	// �e�̎�����ǂݍ���
							}
							else if (strcmp(&aString[0], "BULLET_MOVE") == 0)
							{ // �ǂݍ��񂾕����� BULLET_MOVE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fBullMove);	// �e�̈ړ��ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "BULLET_RADIUS") == 0)
							{ // �ǂݍ��񂾕����� BULLET_RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fBullRadius);	// �e�̔��a��ǂݍ���
							}
							else if (strcmp(&aString[0], "ATTACK_CNT") == 0)
							{ // �ǂݍ��񂾕����� ATTACK_CNT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nCounterAttack);	// �U���p�x��ǂݍ���
							}
							else if (strcmp(&aString[0], "FORWARD_MOVE") == 0)
							{ // �ǂݍ��񂾕����� FORWARD_MOVE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fForwardMove);	// �O�i�̈ړ��ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "BACKWARD_MOVE") == 0)
							{ // �ǂݍ��񂾕����� BACKWARD_MOVE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fBackwardMove);	// ��ނ̈ړ��ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "LOOK_REV") == 0)
							{ // �ǂݍ��񂾕����� LOOK_REV �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fLookRevision);	// �U������␳�W����ǂݍ���
							}
							else if (strcmp(&aString[0], "FIND_RADIUS") == 0)
							{ // �ǂݍ��񂾕����� FIND_RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fFindRadius);		// ���m�͈͂�ǂݍ���
							}
							else if (strcmp(&aString[0], "ATTACK_RADIUS") == 0)
							{ // �ǂݍ��񂾕����� ATTACK_RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fAttackRadius);	// �U���͈͂�ǂݍ���
							}
							else if (strcmp(&aString[0], "BACKWARD_RADIUS") == 0)
							{ // �ǂݍ��񂾕����� BACKWARD_RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fBackwardRadius);	// ��ޔ͈͂�ǂݍ���
							}
							else if (strcmp(&aString[0], "BACWARD_ENABLE") == 0)
							{ // �ǂݍ��񂾕����� BACWARD_ENABLE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nBackward);	// ��ނ�ON/OFF��ǂݍ���

								// �ǂݍ��񂾒l��bool�^�ɕϊ�
								m_aStatusInfo[nType].bBackward = (nBackward == 0) ? true : false;
							}
						} while (strcmp(&aString[0], "END_ENEMYSET") != 0);	// �ǂݍ��񂾕����� END_ENEMYSET �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_STATUSSET") != 0);		// �ǂݍ��񂾕����� END_STATUSSET �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�G�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
