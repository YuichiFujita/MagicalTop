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
//	�N���X��`
//************************************************************
// �X�e�[�W�N���X
class CStage
{
public:
	// �R���X�g���N�^
	CStage();

	// �f�X�g���N�^
	~CStage();

	// �X�e�[�W�͈͍\����
	typedef struct
	{
		float fNear;	// �����ʒu (�O)
		float fFar;		// �����ʒu (��)
		float fRight;	// �����ʒu (�E)
		float fLeft;	// �����ʒu (��)
		float fField;	// �����ʒu (�n��)
	}StageLimit;

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��

	void LimitPosition(D3DXVECTOR3& rPos, const float fRadius);	// �ʒu�␳
	bool LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight);	// �͈͊O���n

	void SetStageLimit(const StageLimit& rLimit);	// �X�e�[�W�͈͐ݒ�
	StageLimit GetStageLimit(void) const;			// �X�e�[�W�͈͎擾

	// �ÓI�����o�֐�
	static CStage *Create(void);				// ����
	static HRESULT Release(CStage *&prStage);	// �j��

private:
	// �ÓI�����o�֐�
	static void LoadSetup(CStage *pStage);	// �Z�b�g�A�b�v

	// �����o�ϐ�
	StageLimit m_stageLimit;	// �X�e�[�W�͈�
};

#endif	// _STAGE_H_
