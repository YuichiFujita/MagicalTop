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
class CLifeGauge3D;		// �̗̓Q�[�W3D�N���X
class CShadow;			// �e�N���X

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

	// ��ԗ�
	typedef enum
	{
		STATE_NORMAL = 0,	// �ʏ���
		STATE_DAMAGE,		// �_���[�W���
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
	int GetState(void) const;		// ��Ԏ擾
	float GetRadius(void) const;	// ���a�擾

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	CObjectMeshCube *m_pMeshCube;	// ���b�V���L���[�u�̏��
	CLifeGauge3D *m_pLifeGauge;		// �̗͂̏��
	CShadow *m_pShadow;		// �e�̏��
	STATE m_state;			// ���
	float m_fSinRot;		// ���V����
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
	int m_nCounterHeal;		// �񕜊Ǘ��J�E���^�[
};

#endif	// _TARGET_H_
