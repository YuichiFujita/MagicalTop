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
#include "sceneGame.h"
#include "renderer.h"
#include "texture.h"
#include "multiModel.h"
#include "lifeGauge3D.h"
#include "shadow.h"
#include "objectBillboard.h"
#include "collision.h"
#include "player.h"
#include "target.h"
#include "bullet.h"
#include "stage.h"
#include "score.h"
#include "particle3D.h"
#include "field.h"

//************************************************************
//	�}�N����`
//************************************************************
#define ENEMY_SETUP_TXT	"data\\TXT\\enemy.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

#define ENE_REV		(0.02f)	// �v���C���[�ړ��ʂ̌����W��
#define ENE_GRAVITY	(1.0f)	// �v���C���[�d��

#define ENE_DMG_FRAME	(20)	// �G�̃_���[�W��ԃt���[��

#define ENE_WAR_ADD_POSY	(100.0f)	// �x���\���ʒu y�̉��Z��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CEnemy::StatusInfo CEnemy::m_aStatusInfo[CEnemy::TYPE_MAX] = {};	// �X�e�[�^�X���
CEnemy::PartsInfo CEnemy::m_aPartsInfo[CEnemy::TYPE_MAX] = {};		// �p�[�c���
int CEnemy::m_nNumAll = 0;											// �G�̑���

const char *CEnemyCar::mc_apModelFile[] =	// ��ԃ��f���萔
{
	"data\\MODEL\\ENEMY\\CAR\\caterpillar000.x",	// �L���^�s��
	"data\\MODEL\\ENEMY\\CAR\\cannon000.x",			// �L���m��
};

//************************************************************
//	�q�N���X [CEnemy] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemy::CEnemy(const TYPE type) : CObjectChara(CObject::LABEL_ENEMY), m_status(m_aStatusInfo[type]), m_parts(m_aPartsInfo[type])
{
	// �����o�ϐ����N���A
	m_pLifeGauge = NULL;		// �̗͂̏��
	m_pShadow  = NULL;			// �e�̏��
	m_pWarning = NULL;			// �x���̏��
	m_oldPos   = VEC3_ZERO;		// �ߋ��ʒu
	m_movePos  = VEC3_ZERO;		// �ʒu�ړ���
	m_moveRot  = VEC3_ZERO;		// �����ύX��
	m_state    = STATE_SPAWN;	// ���
	m_nCounterAtk = 0;			// �U���Ǘ��J�E���^�[

	// �G�̑��������Z
	m_nNumAll++;
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemy::~CEnemy()
{
	// �G�̑��������Z
	m_nNumAll--;
}

//============================================================
//	����������
//============================================================
HRESULT CEnemy::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	m_pLifeGauge = NULL;		// �̗͂̏��
	m_pShadow  = NULL;			// �e�̏��
	m_pWarning = NULL;			// �x���̏��
	m_oldPos   = VEC3_ZERO;		// �ߋ��ʒu
	m_movePos  = VEC3_ZERO;		// �ʒu�ړ���
	m_moveRot  = VEC3_ZERO;		// �����ύX��
	m_state    = STATE_SPAWN;	// ���
	m_nCounterAtk = 0;			// �U���Ǘ��J�E���^�[

	// �̗̓Q�[�W3D�̐���
	m_pLifeGauge = CLifeGauge3D::Create(m_status.nLife, m_status.nLife, (int)(ENE_DMG_FRAME * 0.5f), m_status.fLifeUp, this);
	if (UNUSED(m_pLifeGauge))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�̐���
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, D3DXVECTOR3(m_status.fShadowRadius, 0.0f, m_status.fShadowRadius), this);
	if (UNUSED(m_pShadow))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �I�u�W�F�N�g�r���{�[�h�̐���
	m_pWarning = CObjectBillboard::Create(VEC3_ZERO, D3DXVECTOR3(160.0f, 160.0f, 0.0f));
	if (UNUSED(m_pWarning))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pWarning->BindTexture(pTexture->Regist("data\\TEXTURE\\warning000.png"));

	// �I�u�W�F�N�g�L�����N�^�[�̏�����
	if (FAILED(CObjectChara::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	for (int nCntEnemy = 0; nCntEnemy < m_parts.nNumParts; nCntEnemy++)
	{ // �p�[�c�����J��Ԃ�

		// �p�[�c���̐ݒ�
		CObjectChara::SetPartsInfo
		( // ����
			nCntEnemy,							// �p�[�c�C���f�b�N�X
			m_parts.aInfo[nCntEnemy].nParentID,	// �e�C���f�b�N�X
			m_parts.aInfo[nCntEnemy].pos,		// �ʒu
			m_parts.aInfo[nCntEnemy].rot,		// ����
			GetModelFileName(nCntEnemy)			// �t�@�C����
		);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemy::Uninit(void)
{
	// �̗̓Q�[�W3D��j��
	m_pLifeGauge->Uninit();

	// �e��j��
	m_pShadow->Uninit();

	// �x����j��
	m_pWarning->Uninit();

	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	CObjectChara::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemy::Update(void)
{
	// �̗̓Q�[�W3D�̍X�V
	m_pLifeGauge->Update();

	// �e�̍X�V
	m_pShadow->Update();

	// �x���̍X�V
	m_pWarning->Update();

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEnemy::Draw(void)
{
	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw();
}

//============================================================
//	�q�b�g����
//============================================================
void CEnemy::Hit(const int nDmg)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetPosition();	// �G�ʒu

	if (IsDeath() != true)
	{ // ���S�t���O�������Ă��Ȃ��ꍇ

		// �̗͂���_���[�W�����Z
		m_pLifeGauge->AddLife(-nDmg);

		if (m_pLifeGauge->GetLife() > 0)
		{ // �����Ă���ꍇ

			// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
			CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos);
		}
		else
		{ // ����ł���ꍇ

			// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
			CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f));
			CParticle3D::Create(CParticle3D::TYPE_DAMAGE, pos, D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f));

			// �X�R�A�����Z
			CSceneGame::GetScore()->Add(m_status.nScore);

			// �G�I�u�W�F�N�g�̏I��
			Uninit();
		}
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
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�G�����_����������
//============================================================
void CEnemy::RandomSpawn
(
	const int nNum,	// ������
	const TYPE type	// ���
)
{
	// �ϐ���錾
	D3DXVECTOR3 pos, rot;	// �ʒu�E�����ݒ�p
	D3DXVECTOR3 posTarget;	// �^�[�Q�b�g�ʒu
	int nLimit = (int)CSceneGame::GetStage()->GetStageLimit().fRadius;	// �X�e�[�W�͈�

	// �|�C���^��錾
	CTarget *pTarget = CSceneGame::GetTarget();	// �^�[�Q�b�g���

	if (USED(CSceneGame::GetTarget()))
	{ // �^�[�Q�b�g���g�p����Ă���ꍇ

		for (int nCntGrow = 0; nCntGrow < nNum; nCntGrow++)
		{ // ���������J��Ԃ�

			// �^�[�Q�b�g�ʒu���擾
			posTarget = pTarget->GetPosition();

			// �����ʒu��ݒ�
			pos.x = (float)(rand() % (nLimit * 2) - nLimit + 1);
			pos.y = 3000.0f;	// TODO�F�萔����
			pos.z = (float)(rand() % (nLimit * 2) - nLimit + 1);

			// ����������ݒ�
			rot = VEC3_ZERO;	// ������������
			rot.y = atan2f(pos.x - posTarget.x, pos.z - posTarget.z);	// �^�[�Q�b�g�̕�������������

			// �ʒu��␳
			pos.x = sinf(rot.y) * (CSceneGame::GetStage()->GetStageLimit().fRadius - m_aStatusInfo[type].fRadius);
			pos.z = cosf(rot.y) * (CSceneGame::GetStage()->GetStageLimit().fRadius - m_aStatusInfo[type].fRadius);

			// �G�I�u�W�F�N�g�̐���
			CEnemy::Create(type, pos, rot);
		}
	}
}

//============================================================
//	�����擾����
//============================================================
int CEnemy::GetNumAll(void)
{
	// ���݂̓G�̑�����Ԃ�
	return m_nNumAll;
}

//============================================================
//	�ߋ��ʒu�̍X�V����
//============================================================
void CEnemy::UpdateOldPosition(void)
{
	// �ߋ��ʒu���X�V
	m_oldPos = GetPosition();
}

//============================================================
//	�ʒu�ړ��ʂ̐ݒ菈��
//============================================================
void CEnemy::SetMovePosition(const D3DXVECTOR3& rMove)
{
	// �����̈ʒu�ړ��ʂ�ݒ�
	m_movePos = rMove;
}

//============================================================
//	�����ύX�ʂ̐ݒ菈��
//============================================================
void CEnemy::SetMoveRotation(const D3DXVECTOR3& rMove)
{
	// �����̌�����ݒ�
	m_moveRot = rMove;

	// �����̐��K��
	useful::NormalizeRot(m_moveRot.x);
	useful::NormalizeRot(m_moveRot.y);
	useful::NormalizeRot(m_moveRot.z);
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CEnemy::SetState(const STATE state)
{
	// �����̏�Ԃ�ݒ�
	m_state = state;
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CEnemy::GetMtxWorld(void) const
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;	// �v�Z�p�}�g���b�N�X
	D3DXVECTOR3 posEnemy = GetPosition();	// �G�ʒu
	D3DXVECTOR3 rotEnemy = GetRotation();	// �G����

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotEnemy.y, rotEnemy.x, rotEnemy.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, posEnemy.x, posEnemy.y, posEnemy.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X��Ԃ�
	return mtxWorld;
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
//	�ʒu�ړ��ʎ擾����
//============================================================
D3DXVECTOR3 CEnemy::GetMovePosition(void) const
{
	// �ʒu�ړ��ʂ�Ԃ�
	return m_movePos;
}

//============================================================
//	�����ύX�ʎ擾����
//============================================================
D3DXVECTOR3 CEnemy::GetMoveRotation(void) const
{
	// �����ύX�ʂ�Ԃ�
	return m_moveRot;
}

//============================================================
//	��Ԏ擾����
//============================================================
int CEnemy::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
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
//	�X�|�[������
//============================================================
void CEnemy::Spawn(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetPosition();		// �G�ʒu
	D3DXVECTOR3 moveEnemy = GetMovePosition();	// �G�ړ���
	D3DXVECTOR3 posWarning = posEnemy;			// �x���ʒu

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// �X�|�[����Ԃ̓G�Ƃ̓����蔻��
	CollisionSpawnEnemy(posEnemy);

	// �d�͂����Z
	moveEnemy.y -= ENE_GRAVITY;

	// �ړ��ʂ����Z
	posEnemy += moveEnemy;

	// ���n����
	if (CSceneGame::GetField()->LandPosition(posEnemy, moveEnemy))
	{ // ���n�����ꍇ

		// �`������Ȃ��ݒ�ɂ���
		m_pWarning->SetEnableDraw(false);

		// ��Ԃ�ݒ�
		m_state = STATE_NORMAL;	// �ʏ���
	}

	// �ʒu�𔽉f
	SetPosition(posEnemy);

	// �ʒu�ړ��ʂ𔽉f
	SetMovePosition(moveEnemy);

	// �x���ʒu��n�`�ɓY�킹��
	posWarning.y = CSceneGame::GetField()->GetPositionHeight(posWarning);

	// �x���\���ʒu��ݒ�
	m_pWarning->SetPosition(D3DXVECTOR3(posWarning.x, posWarning.y + ENE_WAR_ADD_POSY, posWarning.z));
}

//============================================================
//	���m�͈͂̓����蔻��
//============================================================
void CEnemy::CollisionFind(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posLook;	// ���F�Ώۈʒu
	D3DXVECTOR3 posEnemy = GetPosition();	// �G�ʒu
	D3DXVECTOR3 rotEnemy = GetRotation();	// �G����
	float fPlayerRadius = CSceneGame::GetPlayer()->GetRadius();	// �v���C���[���a

	if (USED(CSceneGame::GetPlayer()) && USED(CSceneGame::GetTarget()))	// TODO�FGETPLAYER
	{ // �v���C���[�E�^�[�Q�b�g���g�p����Ă���ꍇ

		// ���F�Ώۂ̐ݒ�
		if (collision::Circle2D(CSceneGame::GetPlayer()->GetPosition(), posEnemy, fPlayerRadius, m_status.fFindRadius) == false)
		{ // �G�̌��m�͈͊O�̏ꍇ

			// ���F�Ώۈʒu��ݒ�
			posLook = CSceneGame::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu
		}
		else
		{ // �G�̌��m�͈͓��̏ꍇ

			// ���F�Ώۈʒu��ݒ�
			posLook = CSceneGame::GetPlayer()->GetPosition();	// �v���C���[�ʒu
		}

		// �Ώۂ̕�������������
		Look(posLook, posEnemy, rotEnemy);

		if (collision::Circle2D(posLook, posEnemy, fPlayerRadius, m_status.fAttackRadius) == false)
		{ // �G�̍U���͈͊O�̏ꍇ

			// �Ώۂ̕����Ɉړ� (�O�i)
			posEnemy.x -= sinf(rotEnemy.y) * m_status.fForwardMove;
			posEnemy.z -= cosf(rotEnemy.y) * m_status.fForwardMove;

			// �X�e�[�W�͈͊O�̕␳
			CSceneGame::GetStage()->LimitPosition(posEnemy, m_status.fRadius);
		}
		else
		{ // �G�̍U���͈͓��̏ꍇ

			if (collision::Circle2D(posLook, posEnemy, fPlayerRadius, m_status.fBackwardRadius) == true && m_status.bBackward == true)
			{ // �G�̌�ޔ͈͓����A��ނ�ON�̏ꍇ

				// �Ώۂ̋t�����Ɉړ� (���)
				posEnemy.x += sinf(rotEnemy.y) * m_status.fBackwardMove;
				posEnemy.z += cosf(rotEnemy.y) * m_status.fBackwardMove;

				// �X�e�[�W�͈͊O�̕␳
				CSceneGame::GetStage()->LimitPosition(posEnemy, m_status.fRadius);
			}

			// �U��
			Attack(posLook);
		}
	}

	// �ʒu���X�V
	SetPosition(posEnemy);

	// �������X�V
	SetRotation(rotEnemy);
}

//============================================================
//	�Ώێ��F����
//============================================================
void CEnemy::Look(const D3DXVECTOR3& rPosLook, const D3DXVECTOR3& rPosEnemy, D3DXVECTOR3& rRotEnemy)
{
	// �ϐ���錾
	float fDestRot = 0.0f;	// �ڕW����
	float fDiffRot = 0.0f;	// ����

	// ���F�Ώۂ̕�������
	fDestRot = atan2f(rPosEnemy.x - rPosLook.x, rPosEnemy.z - rPosLook.z);	// �ڕW����

	// �������������߂�
	fDiffRot = fDestRot - rRotEnemy.y;
	useful::NormalizeRot(fDiffRot);		// ���������̐��K��

	// �����̍X�V
	rRotEnemy.y += fDiffRot * m_status.fLookRevision;
	useful::NormalizeRot(rRotEnemy.y);	// �����̐��K��
}

//============================================================
//	�U������
//============================================================
void CEnemy::Attack(const D3DXVECTOR3& rTarget)
{
	// �J�E���^�[�����Z
	m_nCounterAtk++;

	if (m_nCounterAtk >= m_status.nCounterAttack)
	{ // �J�E���^�[�����l�ȏ�̏ꍇ

		// �ϐ���錾
		D3DXVECTOR3 posBull;			// ���ˈʒu
		D3DXVECTOR3 vecBull;			// ���˕���
		D3DXMATRIX  mtxTrans, mtxWorld;	// �v�Z�p�}�g���b�N�X

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_status.bullPos.x, m_status.bullPos.y, m_status.bullPos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// �}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &GetMultiModel(m_status.nBullParts)->GetMtxWorld());

		// �}�g���b�N�X����ʒu�����߂�
		posBull = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

		// �e�̈ړ����������߂�
		vecBull = rTarget - posBull;
		D3DXVec3Normalize(&vecBull, &vecBull);		// �ړ������𐳋K��
		useful::LimitNum(vecBull.y, -0.1f, 0.1f);	// �ړ����� y�𐧌�	// TODO�F�萔�}�N����	// TODO�F�����͂���ꂽ�Ƃ��������Ⴄ�̖h�~

		// �e�I�u�W�F�N�g�̐���
		CBullet::Create
		( // ����
			CBullet::TYPE_ENEMY,			// ���
			posBull,						// �ʒu
			VEC3_ALL(m_status.fBullRadius),	// �傫��
			XCOL_WHITE,						// �F
			vecBull,						// �ړ�����
			m_status.fBullMove,				// �ړ����x
			m_status.nBullLife,				// ����
			m_status.nBullDamage			// �U����
		);

		// �p�[�e�B�N��3D�I�u�W�F�N�g�̐���
		CParticle3D::Create(CParticle3D::TYPE_DAMAGE, posBull);

		// �J�E���^�[��������
		m_nCounterAtk = 0;
	}
}

//============================================================
//	�^�[�Q�b�g�Ƃ̓����蔻��
//============================================================
void CEnemy::CollisionTarget(D3DXVECTOR3& rPos)
{
	// �|�C���^��錾
	CTarget *pTarget = CSceneGame::GetTarget();	// �^�[�Q�b�g���

	if (USED(pTarget))
	{ // �^�[�Q�b�g���g�p����Ă���ꍇ

		// �^�[�Q�b�g�Ƃ̏Փ˔���
		collision::CirclePillar
		( // ����
			rPos,					// ����ʒu
			pTarget->GetPosition(),	// ����ڕW�ʒu
			m_status.fRadius,		// ���蔼�a
			pTarget->GetRadius()	// ����ڕW���a
		);
	}
}

//============================================================
//	�X�|�[�����̓G�Ƃ̓����蔻��
//============================================================
void CEnemy::CollisionSpawnEnemy(D3DXVECTOR3& rPos)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (USED(pObjectTop))
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (USED(pObjCheck))
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_ENEMY || pObjCheck == GetObject())
				{ // �I�u�W�F�N�g���x�����G�ł͂Ȃ��A�܂��̓I�u�W�F�N�g���������g�������ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (pObjCheck->GetState() != CEnemy::STATE_SPAWN)
				{ // �G�̏�Ԃ��X�|�[����Ԃł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �^�[�Q�b�g�Ƃ̏Փ˔���
				collision::CirclePillar
				( // ����
					rPos,						// ����ʒu
					pObjCheck->GetPosition(),	// ����ڕW�ʒu
					m_status.fRadius,			// ���蔼�a
					pObjCheck->GetRadius()		// ����ڕW���a
				);

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	�ʏ펞�̓G�Ƃ̓����蔻��
//============================================================
void CEnemy::CollisionNormalEnemy(D3DXVECTOR3& rPos)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (USED(pObjectTop))
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (USED(pObjCheck))
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_ENEMY || pObjCheck == GetObject())
				{ // �I�u�W�F�N�g���x�����G�ł͂Ȃ��A�܂��̓I�u�W�F�N�g���������g�������ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (pObjCheck->GetState() != CEnemy::STATE_NORMAL)
				{ // �G�̏�Ԃ��ʏ��Ԃł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �^�[�Q�b�g�Ƃ̏Փ˔���
				collision::CirclePillar
				( // ����
					rPos,						// ����ʒu
					pObjCheck->GetPosition(),	// ����ڕW�ʒu
					m_status.fRadius,			// ���蔼�a
					pObjCheck->GetRadius()		// ����ڕW���a
				);

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	�x���̕`��󋵂̐ݒ菈��
//============================================================
void CEnemy::SetDrawWarning(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	m_pWarning->SetEnableDraw(bDraw);
}

//============================================================
//	�x���̈ʒu�̐ݒ菈��
//============================================================
void CEnemy::SetPositionWarning(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pWarning->SetPosition(rPos);
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
	m_spawn = SPAWN_WAIT;	// �X�|�[�����
	m_nCounterSpawn = 0;	// �X�|�[���Ǘ��J�E���^�[
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
	// �����o�ϐ���������
	m_spawn = SPAWN_WAIT;	// �X�|�[�����
	m_nCounterSpawn = 0;	// �X�|�[���Ǘ��J�E���^�[

	// �G�̏�����
	if (FAILED(CEnemy::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyCar::Uninit(void)
{
	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyCar::Update(void)
{
	switch (GetState())
	{ // ��Ԃ��Ƃ̏���
	case STATE_SPAWN:

		// �G�̃X�|�[������̍X�V
		Spawn();

		break;

	case STATE_NORMAL:

		// �G�̓���̍X�V
		CollisionFind();

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �G�̍X�V
	CEnemy::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyCar::Draw(void)
{
	// �G�̕`��
	CEnemy::Draw();
}

//============================================================
//	���f���t�@�C���擾����
//============================================================
const char* CEnemyCar::GetModelFileName(const int nModel) const
{
	if (nModel < MODEL_MAX)
	{ // �g�p�ł���C���f�b�N�X�̏ꍇ

		// �����̃C���f�b�N�X�̃��f����Ԃ�
		return mc_apModelFile[nModel];
	}
	else { assert(false); return NONE_STRING; }	// �͈͊O
}

//============================================================
//	�X�|�[������
//============================================================
void CEnemyCar::Spawn(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posEnemy = GetPosition();		// �G�ʒu
	D3DXVECTOR3 moveEnemy = GetMovePosition();	// �G�ړ���
	D3DXVECTOR3 posWarning = posEnemy;			// �x���ʒu

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// �X�|�[����Ԃ̓G�Ƃ̓����蔻��
	CollisionSpawnEnemy(posEnemy);

	switch (m_spawn)
	{ // �X�|�[����Ԃ��Ƃ̏���
	case STATE_SPAWN:

		if (m_nCounterSpawn < 40)	// TODO�F�萔
		{ // �J�E���^�[�����l��菬�����ꍇ

			// �J�E���^�[�����Z
			m_nCounterSpawn++;
		}
		else
		{ // �J�E���^�[�����l�ȏ�̏ꍇ

			// �J�E���^�[��������
			m_nCounterSpawn = 0;

			// ��Ԃ�ύX
			m_spawn = SPAWN_FALL;	// �������
		}

		break;

	case STATE_NORMAL:
	
		// �d�͂����Z
		moveEnemy.y -= ENE_GRAVITY;
	
		// �ړ��ʂ����Z
		posEnemy += moveEnemy;
	
		// ���n����
		if (CSceneGame::GetField()->LandPosition(posEnemy, moveEnemy))
		{ // ���n�����ꍇ

			// �x���̕`��󋵂̐ݒ�
			SetDrawWarning(false);	// �`�悵�Ȃ�
	
			// ��Ԃ̐ݒ�
			SetState(STATE_NORMAL);	// �ʏ���
		}

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �ʒu�𔽉f
	SetPosition(posEnemy);

	// �ʒu�ړ��ʂ𔽉f
	SetMovePosition(moveEnemy);

	// �x���ʒu��n�`�ɓY�킹��
	posWarning.y = CSceneGame::GetField()->GetPositionHeight(posWarning);

	// �x���̈ʒu�̐ݒ�
	SetPositionWarning(D3DXVECTOR3(posWarning.x, posWarning.y + ENE_WAR_ADD_POSY, posWarning.z));
}

//============================================================
//	���m�͈͂̓����蔻��
//============================================================
void CEnemyCar::CollisionFind(void)
{
	// �ϐ���錾
	StatusInfo  status		= GetStatusInfo();		// �G�X�e�[�^�X
	D3DXVECTOR3 posEnemy	= GetPosition();		// �G�ʒu
	D3DXVECTOR3 moveEnemy	= GetMovePosition();	// �G�ړ���
	D3DXVECTOR3 rotEnemy	= GetRotation();		// �G����
	D3DXVECTOR3 posLook		= VEC3_ZERO;			// ���F�Ώۈʒu
	D3DXVECTOR3 rotCannon	= VEC3_ZERO;			// �L���m������
	float fPlayerRadius = CSceneGame::GetPlayer()->GetRadius();	// �v���C���[���a

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	if (USED(CSceneGame::GetPlayer()) && USED(CSceneGame::GetTarget()))	// TODO�FGETPLAYER
	{ // �v���C���[�E�^�[�Q�b�g���g�p����Ă���ꍇ

		// ���F�Ώۂ̌��m����
		if (collision::Circle2D(CSceneGame::GetPlayer()->GetPosition(), posEnemy, fPlayerRadius, status.fFindRadius) == false)
		{ // �G�̌��m�͈͊O�̏ꍇ

			// ���F�Ώۈʒu��ݒ�
			posLook = CSceneGame::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu
		}
		else
		{ // �G�̌��m�͈͓��̏ꍇ

			// ���F�Ώۈʒu��ݒ�
			posLook = CSceneGame::GetPlayer()->GetPosition();	// �v���C���[�ʒu
		}

		// ���F�Ώۂ̍U������
		if (collision::Circle2D(posLook, posEnemy, fPlayerRadius, status.fAttackRadius) == false)
		{ // �G�̍U���͈͊O�̏ꍇ

			// �Ώۂ̕�������������
			Look(posLook, posEnemy, rotEnemy);

			// �Ώۂ̕����Ɉړ� (�O�i)
			moveEnemy.x -= sinf(rotEnemy.y) * status.fForwardMove;
			moveEnemy.z -= cosf(rotEnemy.y) * status.fForwardMove;

			// �d�͂����Z
			moveEnemy.y -= ENE_GRAVITY;

			// �ړ��ʂ����Z
			posEnemy += moveEnemy;

			// �ړ��ʂ�����
			moveEnemy.x += (0.0f - moveEnemy.x) * ENE_REV;
			moveEnemy.z += (0.0f - moveEnemy.z) * ENE_REV;

			// �^�[�Q�b�g�Ƃ̓����蔻��
			CollisionTarget(posEnemy);

			// �ʏ��Ԃ̓G�Ƃ̓����蔻��
			CollisionNormalEnemy(posEnemy);

			// ���n����
			CSceneGame::GetField()->LandPosition(posEnemy, moveEnemy);

			// �X�e�[�W�͈͊O�̕␳
			CSceneGame::GetStage()->LimitPosition(posEnemy, status.fRadius);
		}
		else
		{ // �G�̍U���͈͓��̏ꍇ

			if (collision::Circle2D(posLook, posEnemy, fPlayerRadius, status.fBackwardRadius) == true && status.bBackward == true)
			{ // �G�̌�ޔ͈͓����A��ނ�ON�̏ꍇ

				// �Ώۂ̕�������������
				Look(posLook, posEnemy, rotEnemy);

				// �Ώۂ̋t�����Ɉړ� (���)
				moveEnemy.x += sinf(rotEnemy.y) * status.fBackwardMove;
				moveEnemy.z += cosf(rotEnemy.y) * status.fBackwardMove;
			}

			// �d�͂����Z
			moveEnemy.y -= ENE_GRAVITY;

			// �ړ��ʂ����Z
			posEnemy += moveEnemy;

			// �ړ��ʂ�����
			moveEnemy.x += (0.0f - moveEnemy.x) * ENE_REV;
			moveEnemy.z += (0.0f - moveEnemy.z) * ENE_REV;

			// �^�[�Q�b�g�Ƃ̓����蔻��
			CollisionTarget(posEnemy);

			// �ʏ��Ԃ̓G�Ƃ̓����蔻��
			CollisionNormalEnemy(posEnemy);

			// ���n����
			CSceneGame::GetField()->LandPosition(posEnemy, moveEnemy);

			// �X�e�[�W�͈͊O�̕␳
			CSceneGame::GetStage()->LimitPosition(posEnemy, status.fRadius);

			// �L���m�������̐ݒ�
			if (SetRotationCannon(posLook, posEnemy, rotEnemy))
			{ // ���ˉ\��Ԃ̏ꍇ

				// �U��
				Attack(posLook);
			}
		}
	}

	// �ʒu�𔽉f
	SetPosition(posEnemy);

	// �ʒu�ړ��ʂ𔽉f
	SetMovePosition(moveEnemy);

	// �����𔽉f
	SetRotation(rotEnemy);
}

//============================================================
//	�L���m�������̐ݒ菈��
//============================================================
bool CEnemyCar::SetRotationCannon(const D3DXVECTOR3& rLookPos, const D3DXVECTOR3& rEnemyPos, const D3DXVECTOR3& rEnemyRot)
{
	// �ϐ���錾
	D3DXVECTOR3 cannonRot;	// ��C����
	float fDestRot = 0.0f;	// �ڕW����
	float fDiffRot = 0.0f;	// ����

	// ��C�������擾
	cannonRot = GetMultiModel(MODEL_CANNON)->GetRotation() + rEnemyRot;	// �{�̂̌��������Z
	useful::NormalizeRot(cannonRot.y);	// ��C�����̐��K��

	// �ڕW�����̌v�Z
	fDestRot = atan2f(rEnemyPos.x - rLookPos.x, rEnemyPos.z - rLookPos.z);
	useful::NormalizeRot(fDestRot);		// �ڕW�����̐��K��

	// ���������̌v�Z
	fDiffRot = fDestRot - cannonRot.y;
	useful::NormalizeRot(fDiffRot);		// ���������̐��K��

	// �����̍X�V
	cannonRot.y += fDiffRot * 0.03f;	// TODO�Fcannon�̌����␳�l���
	useful::NormalizeRot(cannonRot.y);	// �L���m�������̐��K��

	// ������ݒ�
	GetMultiModel(MODEL_CANNON)->SetRotation(cannonRot - rEnemyRot);	// �{�̂̌��������Z

	if (fDestRot + D3DX_PI <= cannonRot.y + D3DX_PI + 0.25f
	&&  fDestRot + D3DX_PI >= cannonRot.y + D3DX_PI - 0.25f)	// TODO�Fcannon�̌덷�X�e�[�^�X���
	{ // ��C�����ƖڕW�����̌덷�����Ȃ��ꍇ

		// ���ˉ\��Ԃ�Ԃ�
		return true;
	}
	else
	{ // ��C�����ƖڕW�����̌덷�������ꍇ

		// ���˕s�\��Ԃ�Ԃ�
		return false;
	}
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

	// �ÓI�����o�ϐ��̏����N���A
	memset(&m_aStatusInfo[0], 0, sizeof(m_aStatusInfo));	// �X�e�[�^�X���
	memset(&m_aPartsInfo[0], 0, sizeof(m_aPartsInfo));		// �p�[�c���

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
							else if (strcmp(&aString[0], "BULLET_DAMAGE") == 0)
							{ // �ǂݍ��񂾕����� BULLET_DAMAGE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nBullDamage);	// �e�̍U���͂�ǂݍ���
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
							else if (strcmp(&aString[0], "BULLET_PARTS") == 0)
							{ // �ǂݍ��񂾕����� BULLET_PARTS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nBullParts);	// �e�̔��˃p�[�c��ǂݍ���
							}
							else if (strcmp(&aString[0], "BULLET_POS") == 0)
							{ // �ǂݍ��񂾕����� BULLET_POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].bullPos.x);	// �e�̔��ˈʒuX��ǂݍ���
								fscanf(pFile, "%f", &m_aStatusInfo[nType].bullPos.y);	// �e�̔��ˈʒuY��ǂݍ���
								fscanf(pFile, "%f", &m_aStatusInfo[nType].bullPos.z);	// �e�̔��ˈʒuZ��ǂݍ���
							}
							else if (strcmp(&aString[0], "SHADOW_RADIUS") == 0)
							{ // �ǂݍ��񂾕����� SHADOW_RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fShadowRadius);	// �e�̔��a��ǂݍ���
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
							else if (strcmp(&aString[0], "LIFE_POSUP") == 0)
							{ // �ǂݍ��񂾕����� LIFE_POSUP �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);							// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fLifeUp);			// �̗͕\����Y�ʒu���Z�ʂ�ǂݍ���
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
