//============================================================
//
//	�p�[�e�B�N��3D�w�b�_�[ [particle3D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PARTICLE3D_H_
#define _PARTICLE3D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �p�[�e�B�N��3D�N���X
class CParticle3D : CObject
{
public:
	// �R���X�g���N�^
	CParticle3D();

	// �f�X�g���N�^
	~CParticle3D();

	// �p�[�e�B�N��3D��ޗ�
	typedef enum
	{
		TYPE_NONE = 0,		// �Ȃ�
		TYPE_DAMAGE,		// �_���[�W
		TYPE_MUZZLE_FLASH,	// �}�Y���t���b�V��
		TYPE_TANK_FIRE,		// �^���N�t�@�C�A
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �I�[�o�[���[�h�����I�[�o�[���C�h�֐�
	HRESULT Init	// ������
	( // ����
		const TYPE type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXCOLOR& rCol		// �F
	);

	// �ÓI�����o�֐�
	static CParticle3D *Create	// ����
	( // ����
		const TYPE type,					// ���
		const D3DXVECTOR3& rPos,			// �ʒu
		const D3DXCOLOR& rCol = XCOL_WHITE	// �F
	);

private:
	// �����o�֐�
	void Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);	// �_���[�W
	void MuzzleFlash(const D3DXVECTOR3& rPos);	// �}�Y���t���b�V��
	void TankFire(const D3DXVECTOR3& rPos);		// �^���N�t�@�C�A

	// �ÓI�����o�ϐ�
	static const int mc_aLife[TYPE_MAX];	// �����萔

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXCOLOR m_col;	// �F
	TYPE m_type;		// ���
	int  m_nLife;		// ����
};

#endif	// _PARTICLE3D_H_
