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
class CHurricane;	// �n���P�[���N���X
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
		TEXTURE_BARRIER = 0,	// �o���A�\��
		TEXTURE_AREA,			// �G���A�\��
		TEXTURE_MAX				// ���̗񋓌^�̑���
	}TEXTURE;

	// �G���A��
	typedef enum
	{
		AREA_NONE = -1,	// �G���A�Ȃ�
		AREA_SAFE,		// �Z�[�t�G���A
		AREA_1,			// ��1�G���A
		AREA_2,			// ��2�G���A
		AREA_3,			// ��3�G���A
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

	// �X�e�[�W���\����
	typedef struct
	{
		int nCounter;	// ���ʕύX�J�E���^�[
	}StageWind;

	// �X�e�[�W�G���A�\����
	typedef struct
	{
		D3DXCOLOR col;	// �G���A�F
		float fRadius;	// �G���A���a
	}StageArea;

	// �X�e�[�W�͈͍\����
	typedef struct
	{
		LIMIT mode;			// �������[�h
		D3DXVECTOR3 center;	// ���S���W
		float fNear;		// �����ʒu (�O)
		float fFar;			// �����ʒu (��)
		float fRight;		// �����ʒu (�E)
		float fLeft;		// �����ʒu (��)
		float fRadius;		// �����ʒu (���a)
		float fField;		// �����ʒu (�n��)
	}StageLimit;

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

	bool CollisionBarrier(D3DXVECTOR3& rPos, float fRadius);	// �o���A�Ƃ̓����蔻��
	void LimitPosition(D3DXVECTOR3& rPos, const float fRadius);	// �ʒu�␳
	bool LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight);	// �͈͊O���n

	void SetStageLimit(const StageLimit& rLimit);	// �X�e�[�W�͈͐ݒ�
	StageLimit GetStageLimit(void) const;			// �X�e�[�W�͈͎擾

	void SetStageArea(const int nID, const StageArea& rArea);	// �X�e�[�W�G���A�ݒ�
	StageArea GetStageArea(const int nID) const;				// �X�e�[�W�G���A�擾
	void SetEnableDrawArea(const bool bDraw);					// �X�e�[�W�G���A�`��ݒ�
	AREA GetAreaPlayer(void) const;								// �v���C���[�̌��݃G���A�擾

	void SetStageBarrier(const StageArea& rBarrier);	// �X�e�[�W�o���A�ݒ�
	StageArea GetStageBarrier(void) const;				// �X�e�[�W�o���A�擾
	void SetEnableDrawBarrier(const bool bDraw);		// �X�e�[�W�o���A�`��ݒ�
	D3DXVECTOR3 GetStageBarrierPosition(void) const;	// �X�e�[�W�o���A�̈ʒu�擾

	void SetEnableDrawHurricane(const bool bDraw);	// �n���P�[���`��ݒ�
	void SetEnebleCreateWind(const bool bCreate);	// �������ݒ�

	// �ÓI�����o�֐�
	static CStage *Create(void);				// ����
	static HRESULT Release(CStage *&prStage);	// �j��

private:
	// �ÓI�����o�֐�
	static void LoadSetup(CStage *pStage);	// �Z�b�g�A�b�v

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CHurricane	*m_pHurricane;			// �n���P�[���̏��
	CObject3D	*m_pStageBarrier;		// �o���A�\���̏��
	CObject3D	*m_pStageArea;			// �G���A�\���̏��
	StageWind	m_stageWind;			// ����
	StageArea	m_stageBarrier;			// �o���A
	StageArea	m_aStageArea[AREA_MAX];	// �G���A
	StageLimit	m_stageLimit;			// �͈�
	AREA m_area;	// �v���C���[�̌��݃G���A
	bool m_bWind;	// ��������
};

#endif	// _STAGE_H_
