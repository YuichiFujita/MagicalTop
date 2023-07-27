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
	// �e�N�X�`����
	typedef enum
	{
		TEXTURE_NORMAL = 0,	// �G���A�\��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	}TEXTURE;

	// �G���A��
	typedef enum
	{
		AREA_NONE = -1,	// �G���A�Ȃ�
		AREA_SAFE,		// �Z�[�t�G���A
		AREA_1,			// ��1�G���A
		AREA_2,			// ��2�G���A
		AREA_3,			// ��3�G���A
		AREA_4,			// ��4�G���A
		AREA_5,			// ��5�G���A
		AREA_MAX		// �G���A�̑���
	}AREA;

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

	// �X�e�[�W�G���A�\����
	typedef struct
	{
		D3DXCOLOR col;	// �G���A�F
		float fRadius;	// �G���A���a
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
	void SetStageArea(const int nID, const StageArea& rArea);	// �X�e�[�W�G���A�ݒ�
	StageArea GetStageArea(const int nID) const;				// �X�e�[�W�G���A�擾
	AREA GetAreaPlayer(void) const;	// �v���C���[�̌��݃G���A�擾

	// �ÓI�����o�֐�
	static CStage *Create(void);				// ����
	static HRESULT Release(CStage *&prStage);	// �j��

private:
	// �ÓI�����o�֐�
	static void LoadSetup(CStage *pStage);	// �Z�b�g�A�b�v

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CObject3D *m_pStageArea;			// �X�e�[�W�G���A�\���̏��
	StageArea  m_aStageArea[AREA_MAX];	// �X�e�[�W�G���A
	StageLimit m_stageLimit;			// �X�e�[�W�͈�
	AREA m_area;	// �v���C���[�̌��݃G���A
};

#endif	// _STAGE_H_
