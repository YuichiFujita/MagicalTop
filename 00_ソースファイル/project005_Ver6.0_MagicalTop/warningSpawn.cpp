//============================================================
//
//	�o���x���\������ [warningSpawn.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "warningSpawn.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define WARNIN_PRIO	(6)		// �x���\���̗D�揇��
#define FADEIN_CNT	(60)	// �t�F�[�h�C���J�n�ҋ@�t���[��
#define SET_SCALE	(18.0f)	// �g�嗦�̏����l
#define SUB_SCALE	(1.5f)	// �g�嗦�̌��Z��
#define SUB_FADE	(0.06f)	// �t�F�[�h�̃��l�̌��Z��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CWarningSpawn::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\warningSpawn000.png",	// �x���\���e�N�X�`��
};

//************************************************************
//	�q�N���X [CWarningSpawn] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CWarningSpawn::CWarningSpawn() : CObject2D(CObject::LABEL_UI, WARNIN_PRIO)
{
	// �����o�ϐ����N���A
	m_size =VEC3_ZERO;		// ���̑傫��
	m_state = STATE_NONE;	// ���
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[
	m_fScale = 0.0f;		// �g�嗦
}

//============================================================
//	�f�X�g���N�^
//============================================================
CWarningSpawn::~CWarningSpawn()
{

}

//============================================================
//	����������
//============================================================
HRESULT CWarningSpawn::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	m_size = VEC3_ZERO;		// ���̑傫��
	m_state = STATE_NONE;	// ���
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[
	m_fScale = 0.0f;		// �g�嗦

	// �I�u�W�F�N�g2D�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_WARNING]));

	// �`����~����
	SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CWarningSpawn::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CWarningSpawn::Update(void)
{
	// �ϐ���錾
	D3DXCOLOR colWarning = GetColor();	// �x���\���̐F

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:

		// ����

		break;

	case STATE_FADEOUT:

		// �g�嗦�����Z
		m_fScale -= SUB_SCALE;

		if (m_fScale <= 1.0f)
		{ // �g�嗦���ŏ��l�ȉ��̏ꍇ

			// �g�嗦��␳
			m_fScale = 1.0f;

			// ��Ԃ�ݒ�
			m_state = STATE_NORMAL;	// �ʏ���
		}

		break;

	case STATE_NORMAL:

		if (m_nCounterState < FADEIN_CNT)
		{ // �J�E���^�[�����l��菬�����ꍇ

			// �J�E���^�[�����Z
			m_nCounterState++;
		}
		else
		{ // �J�E���^�[�����l�ȏ�̏ꍇ

			// �J�E���^�[��������
			m_nCounterState = 0;

			// ��Ԃ�ݒ�
			m_state = STATE_FADEIN;	// �t�F�[�h�C�����
		}

		break;

	case STATE_FADEIN:

		// �����x�����Z
		colWarning.a -= SUB_FADE;

		if (colWarning.a <= 0.0f)
		{ // �����x��������؂����ꍇ

			// �����x��␳
			colWarning.a = 0.0f;

			// �`����~����
			SetEnableDraw(false);

			// ��Ԃ�ݒ�
			m_state = STATE_NONE;	// �������Ȃ����
		}

		break;

	default:
		assert(false);
		break;
	}

	// �傫���𔽉f
	SetScaling(m_size * m_fScale);

	// �F�𔽉f
	SetColor(colWarning);

	// �I�u�W�F�N�g2D�̍X�V
	CObject2D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CWarningSpawn::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���e�X�g�̐ݒ�

	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw();

	// ���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// ���e�X�g�̐ݒ�
}

//============================================================
//	��������
//============================================================
CWarningSpawn *CWarningSpawn::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize	// �傫��
)
{
	// �|�C���^��錾
	CWarningSpawn *pWarningSpawn = NULL;	// �o���x���\�������p

	if (UNUSED(pWarningSpawn))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pWarningSpawn = new CWarningSpawn;	// �o���x���\��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pWarningSpawn))
	{ // �m�ۂɐ������Ă���ꍇ

		// �o���x���\���̏�����
		if (FAILED(pWarningSpawn->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pWarningSpawn;
			pWarningSpawn = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pWarningSpawn->SetPosition(rPos);

		// �傫����ݒ�
		pWarningSpawn->SetScaling(rSize);

		// ���̑傫����ݒ�
		pWarningSpawn->SetScalingOrigin(rSize);

		// �m�ۂ����A�h���X��Ԃ�
		return pWarningSpawn;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�\������
//============================================================
void CWarningSpawn::SetDisp(void)
{
	// �g�嗦��������
	m_fScale = SET_SCALE;

	// �傫����������
	SetScaling(m_size * m_fScale);

	// �F��������
	SetColor(XCOL_WHITE);

	// �`����ĊJ����
	SetEnableDraw(true);

	// ��Ԃ�ݒ�
	m_state = STATE_FADEOUT;	// �t�F�[�h�A�E�g���
}

//============================================================
//	���̑傫���̐ݒ菈��
//============================================================
void CWarningSpawn::SetScalingOrigin(const D3DXVECTOR3& rSize)
{
	// �����̑傫����ݒ�
	m_size = rSize;
}
