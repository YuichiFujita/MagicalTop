//============================================================
//
//	���@�}�l�[�W���[���� [magicManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "magicManager.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "sceneGame.h"
#include "object.h"
#include "lockCursor.h"
#include "objectGauge3D.h"
#include "multiModel.h"
#include "player.h"
#include "target.h"
#include "enemy.h"
#include "stage.h"
#include "collision.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_MANA	(100)	// �ő�}�i
#define MANA_FRAME	(10)	// �}�i�ϓ��t���[��

#define GAUGE_PLUS_Y	(120.0f)	// �Q�[�WY�ʒu���Z��
#define GAUGE_GAUGESIZE	(D3DXVECTOR3(110.0f, 15.0f, 0.0f))	// �Q�[�W�傫��
#define GAUGE_FRONTCOL	(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f))	// �\�Q�[�W�F
#define GAUGE_BACKCOL	(D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f))	// ���Q�[�W�F

#define HEAL_CNT	(120)	// �񕜏�ԂɈڍs����܂ł̃J�E���^�[
#define NORMAL_CNT	(40)	// �ʏ��ԂɈڍs����܂ł̃J�E���^�[

#define HEAL_SAFE_PLUS	(2)		// �񕜏�Ԉڍs�J�E���^�[�̃Z�[�t�G���A���̉��Z��
#define HEALCNT_AREAMUL	(10)	// �Z�[�t�G���A�O�ł̉񕜃J�E���^�[�ݒ�p�W��
#define NUM_DEADZONE	(100)	// �f�b�h�]�[���̒l

//************************************************************
//	�e�N���X [CMagicManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMagicManager::CMagicManager()
{
	// �����o�ϐ����N���A
	m_pMana = NULL;			// �}�i�̏��
	m_state = STATE_NORMAL;	// ���
	m_nCounterMagic = 0;	// ���@�Ǘ��J�E���^�[
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[
	m_nCounterHeal = 0;		// �񕜊Ǘ��J�E���^�[
}

//============================================================
//	�f�X�g���N�^
//============================================================
CMagicManager::~CMagicManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMagicManager::Init(void)
{
	// �����o�ϐ���������
	m_pMana = NULL;			// �}�i�̏��
	m_state = STATE_NORMAL;	// ���
	m_nCounterMagic = 0;	// ���@�Ǘ��J�E���^�[
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[
	m_nCounterHeal = 0;		// �񕜊Ǘ��J�E���^�[

	// �}�i�̐���
	m_pMana = CObjectGauge3D::Create
	( // ����
		CObject::LABEL_GAUGE,			// �I�u�W�F�N�g���x��
		NULL,							// �Q�[�W�\���I�u�W�F�N�g
		MAX_MANA,						// �ő�}�i
		MANA_FRAME,						// �}�i�ϓ��t���[��
		GAUGE_PLUS_Y,					// �\��Y�ʒu�̉��Z��
		GAUGE_GAUGESIZE,				// �Q�[�W�傫��
		GAUGE_FRONTCOL,					// �\�Q�[�W�F
		GAUGE_BACKCOL,					// ���Q�[�W�F
		true,							// �g�`���
		CObjectGauge3D::TYPE_PLAYER,	// �g���
		GAUGE_GAUGESIZE					// �g�傫��
	);
	if (UNUSED(m_pMana))
	{ // ��g�p���̏ꍇ

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
void CMagicManager::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CMagicManager::Update(void)
{
	if (CSceneGame::GetPlayer()->GetState() != CPlayer::STATE_NONE
	&&  CSceneGame::GetPlayer()->GetState() != CPlayer::STATE_FADEIN
	&&  CSceneGame::GetPlayer()->GetState() != CPlayer::STATE_FADEOUT)
	{ // �v���C���[���������Ȃ���Ԃł͂Ȃ��ꍇ

		if (m_nCounterMagic > 0)
		{ // �J�E���^�[�� 0���傫���ꍇ

			// �J�E���^�[�����Z
			m_nCounterMagic--;
		}

		switch (m_state)
		{ // ��Ԃ��Ƃ̏���
		case STATE_NORMAL:	// �ʏ���

			if (m_nCounterState < HEAL_CNT)
			{ // �J�E���^�[�����l��菬�����ꍇ

				if (CSceneGame::GetStage()->GetAreaPlayer() == CStage::AREA_SAFE)
				{ // �Z�[�t�G���A�ɂ���ꍇ

					// �J�E���^�[�����Z
					m_nCounterState += HEAL_SAFE_PLUS;
				}
				else
				{ // �Z�[�t�G���A�ɂ��Ȃ��ꍇ

					// �J�E���^�[�����Z
					m_nCounterState++;
				}
			}
			else
			{ // �J�E���^�[�����l�ȏ�̏ꍇ

				// �J�E���^�[��������
				m_nCounterState = 0;

				// ��Ԃ�ύX
				m_state = STATE_HEAL;	// �񕜏��
			}

			break;

		case STATE_ATTACK:	// �U�����

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

			if (CSceneGame::GetStage()->GetAreaPlayer() == CStage::AREA_SAFE)
			{ // �Z�[�t�G���A�ɂ���ꍇ

				if (m_pMana->GetNum() < MAX_MANA)
				{ // �}�i�������Ă���ꍇ

					// �}�i����
					m_pMana->AddNum(1);
				}
			}
			else
			{ // �Z�[�t�G���A�ɂ��Ȃ��ꍇ

				if (m_nCounterHeal < HEALCNT_AREAMUL * CSceneGame::GetStage()->GetAreaPlayer())
				{ // �J�E���^�[�����l��菬�����ꍇ

					// �J�E���^�[�����Z
					m_nCounterHeal++;
				}
				else
				{ // �J�E���^�[�����l�ȏ�̏ꍇ

					// �J�E���^�[��������
					m_nCounterHeal = 0;

					// �}�i����
					m_pMana->AddNum(1);
				}
			}

			break;

		default:	// ��O����
			assert(false);
			break;
		}
	}
}

//============================================================
//	���@�̔��ˏ���
//============================================================
CMagicManager::Shot CMagicManager::ShotMagic(void)
{
	// �ϐ���錾
	D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetRotation();	// �J��������
	Shot bShot = { 0.0f, false, false };	// �ˌ���
	float fVecRot = 0.0f;	// �ˌ�����

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetPad();		// �p�b�h

	// ���˕����̐ݒ�
	if (pKeyboard->GetPress(DIK_I))
	{
		// ���ˑ����������Ԃɂ���
		bShot.bControl = true;

		if (pKeyboard->GetPress(DIK_J))
		{
			// ���ˌ�����ۑ�
			bShot.fRot = D3DXToRadian(135);

			// �ˌ�������ݒ�
			fVecRot = D3DXToRadian(135) + rotCamera.y;
		}
		else if (pKeyboard->GetPress(DIK_L))
		{
			// ���ˌ�����ݒ�
			bShot.fRot = D3DXToRadian(225);

			// �ˌ�������ݒ�
			fVecRot = D3DXToRadian(225) + rotCamera.y;
		}
		else
		{
			// ���ˌ�����ݒ�
			bShot.fRot = D3DXToRadian(180);

			// �ˌ�������ݒ�
			fVecRot = D3DXToRadian(180) + rotCamera.y;
		}
	}
	else if (pKeyboard->GetPress(DIK_K))
	{
		// ���ˑ����������Ԃɂ���
		bShot.bControl = true;

		if (pKeyboard->GetPress(DIK_J))
		{
			// ���ˌ�����ݒ�
			bShot.fRot = D3DXToRadian(45);

			// �ˌ�������ݒ�
			fVecRot = D3DXToRadian(45) + rotCamera.y;
		}
		else if (pKeyboard->GetPress(DIK_L))
		{
			// ���ˌ�����ݒ�
			bShot.fRot = D3DXToRadian(315);

			// �ˌ�������ݒ�
			fVecRot = D3DXToRadian(315) + rotCamera.y;
		}
		else
		{
			// ���ˌ�����ݒ�
			bShot.fRot = D3DXToRadian(0);

			// �ˌ�������ݒ�
			fVecRot = D3DXToRadian(0) + rotCamera.y;
		}
	}
	else if (pKeyboard->GetPress(DIK_J))
	{
		// ���ˑ����������Ԃɂ���
		bShot.bControl = true;

		// ���ˌ�����ݒ�
		bShot.fRot = D3DXToRadian(90);

		// �ˌ�������ݒ�
		fVecRot = D3DXToRadian(90) + rotCamera.y;
	}
	else if (pKeyboard->GetPress(DIK_L))
	{
		// ���ˑ����������Ԃɂ���
		bShot.bControl = true;

		// ���ˌ�����ݒ�
		bShot.fRot = D3DXToRadian(270);

		// �ˌ�������ݒ�
		fVecRot = D3DXToRadian(270) + rotCamera.y;
	}
	else if (pPad->GetPressRStickX() >  NUM_DEADZONE
	||		 pPad->GetPressRStickX() < -NUM_DEADZONE
	||		 pPad->GetPressRStickY() >  NUM_DEADZONE
	||		 pPad->GetPressRStickY() < -NUM_DEADZONE)
	{
		// ���ˑ����������Ԃɂ���
		bShot.bControl = true;

		// ���ˌ�����ݒ�
		bShot.fRot = pPad->GetPressRStickRot() + D3DXToRadian(270);

		// �ˌ�������ݒ�
		fVecRot = pPad->GetPressRStickRot() + D3DXToRadian(270) + rotCamera.y;
	}

	// ������␳
	useful::NormalizeRot(bShot.fRot);

	if (bShot.bControl)
	{ // ���@�̔��ˑ��삵�Ă����ꍇ

		if (m_pMana->GetNum() > 0 && m_nCounterMagic <= 0)
		{ // �}�i�����銎�A�N�[���^�C�����I�������ꍇ

			// �|�C���^��錾
			CPlayer *pPlayer = CSceneGame::GetPlayer();	// �v���C���[

			// �ϐ���錾
			CMagic::StatusInfo status = CMagic::GetStatusInfo();	// ���@�X�e�[�^�X
			D3DXVECTOR3 magicPos, vecMove;	// ���ˈʒu�E�ړ�����
			D3DXMATRIX  mtxTrans, mtxWorld;	// �v�Z�p�}�g���b�N�X

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorld);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, status.shotPos.x, status.shotPos.y, status.shotPos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

			// �}�g���b�N�X���|�����킹��
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &pPlayer->GetMultiModel(status.nShotParts)->GetMtxWorld());

			// �}�g���b�N�X���甭�ˈʒu�����߂�
			magicPos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

			// �����𐳋K��
			useful::NormalizeRot(fVecRot);

			// �ړ�������ݒ�
			vecMove = D3DXVECTOR3(sinf(fVecRot), 0.0f, cosf(fVecRot));

			// ���@�I�u�W�F�N�g�̐���
			CMagic::Create
			( // ����
				magicPos,	// �ʒu
				VEC3_ZERO,	// ����
				vecMove		// �ړ�����
			);

			// �J�E���^�[��ݒ�
			m_nCounterMagic = status.nCoolTime;

			// �}�i������
			m_pMana->AddNum(-1);

			// �J�E���^�[��������
			m_nCounterState = 0;

			// ��Ԃ�ݒ�
			m_state = STATE_ATTACK;	// �U�����

			// ���˂�����Ԃɂ���
			bShot.bShot = true;
		}
	}

	// �ˌ��󋵂�Ԃ�
	return bShot;
}

//============================================================
//	�}�i�`��󋵂̐ݒ菈��
//============================================================
void CMagicManager::SetEnableManaDraw(const bool bDraw)
{
	// �����̕`��󋵂��}�i�̕`��󋵂ɐݒ�
	m_pMana->SetEnableDraw(bDraw);
}

//============================================================
//	�}�i�c�ʎ擾����
//============================================================
int CMagicManager::GetMana(void) const
{
	// �}�i�̎c�ʂ�Ԃ�
	return m_pMana->GetNum();
}

//============================================================
//	��������
//============================================================
CMagicManager *CMagicManager::Create(CObject *pPlayer)
{
	// �|�C���^��錾
	CMagicManager *pMagicManager = NULL;	// ���@�}�l�[�W���[�����p

	if (UNUSED(pMagicManager))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pMagicManager = new CMagicManager;	// ���@�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pMagicManager))
	{ // �g�p����Ă���ꍇ
		
		// ���@�}�l�[�W���[�̏�����
		if (FAILED(pMagicManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pMagicManager;
			pMagicManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �}�i�\���I�u�W�F�N�g��ݒ�
		pMagicManager->m_pMana->SetGaugeObject(pPlayer);

		// �m�ۂ����A�h���X��Ԃ�
		return pMagicManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CMagicManager::Release(CMagicManager *&prMagicManager)
{
	if (USED(prMagicManager))
	{ // �g�p���̏ꍇ

		// ���@�}�l�[�W���[�̏I��
		prMagicManager->Uninit();

		// �������J��
		delete prMagicManager;
		prMagicManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}
