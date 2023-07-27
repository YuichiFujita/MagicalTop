//============================================================
//
//	�X�e�[�W�w�b�_�[ [stage.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _STAGE_H_
#define _STAGE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject3D;	// �I�u�W�F�N�g3D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �X�e�[�W�N���X
class CStage
{
public:
	// ������
	typedef enum
	{
		LIMIT_BOX = 0,	// ��`�͈�
		LIMIT_CIRCLE,	// �~�͈�
		LIMIT_MAX		// ���̗񋓌^�̑���
	}LIMIT;

	// �R���X�g���N�^
	CStage();

	// �f�X�g���N�^
	~CStage();

	// �G���A���\����
	typedef struct
	{
		D3DXCOLOR col;	// �G���A�F
		float fRadius;	// �G���A���a
	}AreaInfo;

	// �X�e�[�W�G���A�\����
	typedef struct
	{
		AreaInfo *pInfo;	// �G���A�̏��
		int nNumArea;		// �G���A�̑���
	}StageArea;

	// �X�e�[�W�͈͍\����
	typedef struct
	{
		LIMIT mode;		// �������[�h
		float fNear;	// �����ʒu (�O)
		float fFar;		// �����ʒu (��)
		float fRight;	// �����ʒu (�E)
		float fLeft;	// �����ʒu (��)
		float fRadius;	// �����ʒu (���a)
		float fField;	// �����ʒu (�n��)
	}StageLimit;

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

	void LimitPosition(D3DXVECTOR3& rPos, const float fRadius);	// �ʒu�␳
	bool LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight);	// �͈͊O���n

	void SetStageLimit(const StageLimit& rLimit);	// �X�e�[�W�͈͐ݒ�
	StageLimit GetStageLimit(void) const;			// �X�e�[�W�͈͎擾
	void SetStageArea(const StageArea& rArea);		// �X�e�[�W�G���A�ݒ�
	StageArea GetStageArea(void) const;				// �X�e�[�W�G���A�擾

	// �ÓI�����o�֐�
	static CStage *Create(void);				// ����
	static HRESULT Release(CStage *&prStage);	// �j��

private:
	// �ÓI�����o�֐�
	static void LoadSetup(CStage *pStage);	// �Z�b�g�A�b�v

	// �����o�ϐ�
	CObject3D *m_pStageArea;	// �X�e�[�W�G���A�\���̏��
	StageArea  m_stageArea;		// �X�e�[�W�G���A
	StageLimit m_stageLimit;	// �X�e�[�W�͈�
};

#endif	// _STAGE_H_
