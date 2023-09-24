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
class CObjectGauge3D;	// �I�u�W�F�N�g�Q�[�W3D�N���X
class CShadow;			// �e�N���X
class CWarningAttack;	// �U���x���\���N���X

//************************************************************
//	�N���X��`
//************************************************************
// �^�[�Q�b�g�N���X
class CTarget : public CObjectModel
{
public:
	// ���f����
	typedef enum
	{
		MODEL_NORMAL = 0,	// �ʏ���
		MODEL_MAX			// ���̗񋓌^�̑���
	}MODEL;

	// ��ԗ�
	typedef enum
	{
		STATE_NORMAL = 0,	// �ʏ���
		STATE_HEAL,			// �񕜏��
		STATE_DESTROY,		// �j����
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	// �R���X�g���N�^
	CTarget();

	// �f�X�g���N�^
	~CTarget();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void Hit(const int nDmg);	// �q�b�g

	// �ÓI�����o�֐�
	static CTarget *Create	// ����
	( // ����
		const MODEL model,			// ���f��
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �傫��
	);

	// �����o�֐�
	void SetEnableUpdate(const bool bUpdate);	// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw);		// �`��󋵐ݒ�
	void SetEnableDrawLife(const bool bDraw);	// �̗͕`��󋵐ݒ�
	void AddLife(const int nAdd);	// �̗͉��Z
	void SetLife(const int nLife);	// �̗͐ݒ�
	int GetLife(void) const;		// �̗͎擾
	int GetMaxLife(void) const;		// �ő�̗͎擾
	int GetState(void) const;		// ��Ԏ擾
	float GetRadius(void) const;	// ���a�擾

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	CObjectMeshCube *m_pMeshCube;	// ���b�V���L���[�u�̏��
	CObjectGauge3D *m_pLife;		// �̗͂̏��
	CShadow *m_pShadow;				// �e�̏��
	CWarningAttack *m_pWarning;		// �U���x���\���̏��

	STATE m_state;			// ���
	float m_fSinRot;		// ���V����
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
	int m_nCounterHeal;		// �񕜊Ǘ��J�E���^�[
};

#endif	// _TARGET_H_
