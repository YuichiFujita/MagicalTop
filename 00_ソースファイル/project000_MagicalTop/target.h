//============================================================
//
//	�^�[�Q�b�g�w�b�_�[ [target.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TARGET_H_
#define _TARGET_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectModel.h"
#include "model.h"

//************************************************************
//	�O���錾
//************************************************************
class CObjectMeshCube;	// �I�u�W�F�N�g���b�V���L���[�u�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �^�[�Q�b�g�N���X
class CTarget : public CObjectModel
{
public:
	// ��ޗ�
	typedef enum
	{
		MODEL_NORMAL = 0,	// �ʏ���
		MODEL_MAX			// ���̗񋓌^�̑���
	}MODEL;

	// �R���X�g���N�^
	CTarget();

	// �f�X�g���N�^
	~CTarget();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CTarget *Create	// ����
	( // ����
		const MODEL model,			// ���f��
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �傫��
	);

	// �����o�֐�
	float GetRadius(void) const;	// ���a�擾

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	CObjectMeshCube *m_pMeshCube;	// ���b�V���L���[�u�̏��
	float m_fSinRot;	// ���V����
};

#endif	// _TARGET_H_
