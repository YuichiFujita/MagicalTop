//============================================================
//
//	���w�b�_�[ [weed.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _WEED_H_
#define _WEED_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object3D.h"

//************************************************************
//	�O���錾
//************************************************************
class CShadow;	// �e�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���N���X
class CWeed : public CObject3D
{
public:
	// ��ޗ�
	typedef enum
	{
		TYPE_NORMAL = 0,	// �ʏ�
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	// �R���X�g���N�^
	CWeed();

	// �f�X�g���N�^
	~CWeed();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CWeed *Create	// ����
	( // ����
		const TYPE type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rSize	// �傫��
	);
	static void RandomSpawn	// �����_������
	( // ����
		const int nNum,				// ������
		const TYPE type,			// ���
		const D3DXVECTOR3& rSize	// �傫��
	);

private:
	// �����o�֐�
	bool CollisionPlayer(void);	// �v���C���[�Ƃ̓����蔻��
	bool CollisionEnemy(void);	// �G�Ƃ̓����蔻��

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CShadow *m_pShadow;	// �e�̏��
	TYPE m_type;		// ���
};

#endif	// _WEED_H_
