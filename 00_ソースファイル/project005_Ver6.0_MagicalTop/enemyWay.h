//============================================================
//
//	�G�����\���w�b�_�[ [enemyWay.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_WAY_H_
#define _ENEMY_WAY_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object3D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�����\���N���X
class CEnemyWay : public CObject3D
{
public:
	// �e�N�X�`����
	typedef enum
	{
		TEXTURE_WAY = 0,	// �����\��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	}TEXTURE;

	// ��ԗ�
	typedef enum
	{
		STATE_SPAWN = 0,	// �������
		STATE_NORMAL,		// �ʏ���
		STATE_DELETE,		// �j�����
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	// �R���X�g���N�^
	CEnemyWay(const float fMaxLimitRadius);

	// �f�X�g���N�^
	~CEnemyWay();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CEnemyWay *Create(CObject *pObject, const float fMaxLimitRadius);	// ����

	// �����o�֐�
	void Delete(void);	// �폜
	void SetObject(CObject *pObject);	// �I�u�W�F�N�g�ݒ�

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CObject *m_pObject;	// �����\���I�u�W�F�N�g
	STATE m_state;		// ���
	const float m_fMaxLimitRadius;	// �I�u�W�F�N�g�̍ő吧���ʒu
};

#endif	// _ENEMY_WAY_H_
