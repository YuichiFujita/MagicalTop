//============================================================
//
//	������ [wind.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "wind.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"
#include "scene.h"
#include "sceneGame.h"
#include "stage.h"
#include "field.h"

//************************************************************
//	�}�N����`
//************************************************************
#define WIND_COL	(D3DXCOLOR(0.6f, 0.9f, 1.0f, 0.6f))	// ���̃|���S���F
#define WIND_PRIO	(5)	// ���̗D�揇��

#define MOVE_BACK	(16.0f)		// ���̉��ړ���
#define MOVE_SIDE	(15.5f)		// ���̉��ړ���
#define PLUS_POSY	(20.0f)		// ���̏c���W���Z��
#define COLL_RADIUS	(50.0f)		// ���̓����蔻��̔��a

#define WIND_TEXPART	(1)		// ���̃e�N�X�`��������
#define WIND_PART		(40)	// ���̕�����

//************************************************************
//	�q�N���X [CWind] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CWind::CWind() : CObjectOrbit(CObject::LABEL_WIND, WIND_PRIO)
{
	// �����o�ϐ����N���A
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ���[���h�}�g���b�N�X
	m_pos = VEC3_ZERO;		// �ʒu
	m_state = STATE_NORMAL;	// ���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CWind::~CWind()
{

}

//============================================================
//	����������
//============================================================
HRESULT CWind::Init(void)
{
	// �����o�ϐ���������
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ���[���h�}�g���b�N�X
	m_pos = VEC3_ZERO;		// �ʒu
	m_state = STATE_NORMAL;	// ���

	// �O�Ղ̏�����
	if (FAILED(CObjectOrbit::Init()))
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
void CWind::Uninit(void)
{
	// �O�Ղ̏I��
	CObjectOrbit::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CWind::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 vecTarg, vecSide;	// �^�[�Q�b�g�����x�N�g���E�������x�N�g��

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NORMAL:

		// �^�[�Q�b�g�����̃x�N�g�����v�Z
		vecTarg = CScene::GetStage()->GetStageLimit().center - m_pos;
		vecTarg.y = 0.0f;						// �x�N�g���̏c�����𖳎�
		D3DXVec3Normalize(&vecTarg, &vecTarg);	// �x�N�g���̐��K��

		// �������x�N�g�����v�Z
		vecSide = D3DXVECTOR3(vecTarg.z, 0.0f, -vecTarg.x);

		// ���ړ��ʂ����Z
		m_pos += vecTarg * MOVE_BACK;

		// ���ړ��ʂ����Z
		m_pos += vecSide * MOVE_SIDE;

		// �c���W��n�ʂɓY�킹��
		if (CScene::GetField()->IsPositionRange(m_pos))
		{ // �n�ʂ͈͓̔��̏ꍇ

			// �c���W��n�`�ɓY�킹��
			m_pos.y = CScene::GetField()->GetPositionHeight(m_pos);
			m_pos.y += PLUS_POSY;
		}

		// �o���A�Ƃ̓����蔻��
		if (collision::Circle2D(CScene::GetStage()->GetStageBarrierPosition(), m_pos, COLL_RADIUS, 0.0f))
		{ // �������Ă����ꍇ

			// ��Ԃ�ݒ�
			m_state = STATE_DELETE;	// �������

			// �O�Ղ�������Ԃɐݒ�
			SetState(CObjectOrbit::STATE_VANISH);
		}

		break;

	case STATE_DELETE:

		if (GetState() == CObjectOrbit::STATE_NONE)
		{ // �O�Ղ��Ȃ��Ȃ�؂����ꍇ

			// �I�u�W�F�N�g�̏I��
			Uninit();

			// �֐��𔲂���
			return;
		}

		break;
	}

	// �O�Ղ̍X�V
	CObjectOrbit::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CWind::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �O�Ղ̕`��
	CObjectOrbit::Draw();
}

//============================================================
//	��������
//============================================================
CWind *CWind::Create(const D3DXVECTOR3& rPos)
{
	// �|�C���^��錾
	CWind *pWind = NULL;	// �������p

	if (UNUSED(pWind))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pWind = new CWind;	// ��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pWind))
	{ // �m�ۂɐ������Ă���ꍇ

		// ���̏�����
		if (FAILED(pWind->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pWind;
			pWind = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`��������
		pWind->BindTexture(NONE_IDX);

		// �ʒu��ݒ�
		pWind->SetPosition(rPos);

		// �e�̃}�g���b�N�X��ݒ�
		pWind->SetMatrixParent(pWind->GetPtrMtxWorld());

		// �F��ݒ�
		pWind->SetColor(WIND_COL);

		// �I�t�Z�b�g��ݒ�
		pWind->SetOffset(CObjectOrbit::OFFSET_WIND);

		// �e�N�X�`����������ݒ�
		pWind->SetTexPart(WIND_TEXPART);

		// �������󋵂�ݒ�
		pWind->SetEnableAlpha(true);

		// ������ݒ�
		if (FAILED(pWind->SetLength(WIND_PART)))
		{ // �����̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pWind;
			pWind = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pWind;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CWind::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�}�g���b�N�X�|�C���^�擾����
//============================================================
D3DXMATRIX *CWind::GetPtrMtxWorld(void)
{
	// ���[���h�}�g���b�N�X�̃|�C���^��Ԃ�
	return &m_mtxWorld;
}
