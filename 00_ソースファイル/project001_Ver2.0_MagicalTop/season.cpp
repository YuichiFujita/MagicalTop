//============================================================
//
//	�V�[�Y������ [season.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "season.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "model.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SEASON_PRIO	(5)	// �V�[�Y���\���̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CSeason::mc_apModelFile[] =	// ���f���萔
{
	"data\\MODEL\\FONT\\shot000.x",	// ���f��
};

//************************************************************
//	�q�N���X [CSeason] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSeason::CSeason() : CObjectModel(CObject::LABEL_NONE, SEASON_PRIO)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSeason::~CSeason()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSeason::Init(void)
{
	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
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
void CSeason::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSeason::Update(void)
{
	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSeason::Draw(void)
{
	// �ϐ���錾
	D3DVIEWPORT9 viewportDef;	// �J�����̃r���[�|�[�g�ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���݂̃r���[�|�[�g���擾
	pDevice->GetViewport(&viewportDef);

	// �J�����̐ݒ�
	CManager::GetCamera()->SetCamera(CCamera::TYPE_FONT);

	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw();

	// �J�����̐ݒ�����ɖ߂�
	CManager::GetCamera()->SetCamera(CCamera::TYPE_MAIN);

	// �r���[�|�[�g�����ɖ߂�
	pDevice->SetViewport(&viewportDef);
}

//============================================================
//	��������
//============================================================
CSeason *CSeason::Create(const MODEL model)
{
	// �ϐ���錾
	int nModelID;	// ���f���C���f�b�N�X

	// �|�C���^��錾
	CModel *pModel = CManager::GetModel();	// ���f���ւ̃|�C���^
	CSeason *pSeason = NULL;	// �V�[�Y�������p

	if (UNUSED(pSeason))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pSeason = new CSeason;	// �V�[�Y��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pSeason))
	{ // �g�p����Ă���ꍇ
		
		// �V�[�Y���̏�����
		if (FAILED(pSeason->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pSeason;
			pSeason = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// ���f����o�^
		nModelID = pModel->Regist(mc_apModelFile[model]);

		// ���f��������
		pSeason->BindModel(pModel->GetModel(nModelID));

		// �ʒu��ݒ�
		pSeason->SetPosition(VEC3_ZERO);

		// ������ݒ�
		pSeason->SetRotation(VEC3_ZERO);

		// �傫����ݒ�
		pSeason->SetScaling(VEC3_ONE);

		// �m�ۂ����A�h���X��Ԃ�
		return pSeason;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
