//============================================================
//
//	�J�����w�b�_�[ [camera.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�N���X��`
//************************************************************
// �J�����N���X
class CCamera
{
public:
	// �J������
	enum TYPE
	{
		TYPE_MAIN,		// ���C���J����
		TYPE_MODELUI,	// ���f��UI�\���J����
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CCamera();

	// �f�X�g���N�^
	~CCamera();

	// �J�����\����
	typedef struct
	{
		D3DXVECTOR3		posV;			// ���݂̎��_
		D3DXVECTOR3		posR;			// ���݂̒����_
		D3DXVECTOR3		destPosV;		// �ڕW�̎��_
		D3DXVECTOR3		destPosR;		// �ڕW�̒����_
		D3DXVECTOR3		vecU;			// ������x�N�g��
		D3DXVECTOR3		rot;			// ���݂̌���
		D3DXVECTOR3		destRot;		// �ڕW�̌���
		float			fDis;			// ���݂̎��_�ƒ����_�̋���
		float			fDestDis;		// �ڕW�̎��_�ƒ����_�̋���
		D3DXMATRIX		mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
		D3DXMATRIX		mtxView;		// �r���[�}�g���b�N�X
		D3DVIEWPORT9	viewport;		// �r���[�|�[�g
	}Camera;

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void SetCamera(const TYPE type);	// �J�����ݒ�
	Camera GetCamera(const TYPE type);	// �J�����擾
	void SetDestBargainingCamera(void);	// �J�����ڕW�ʒu�ݒ� (������)
	void SetRotation(const D3DXVECTOR3& rRot);		// ���݌����ݒ�
	void SetDestRotation(const D3DXVECTOR3& rRot);	// �ڕW�����ݒ�
	D3DXVECTOR3 GetRotation(void) const;			// ���݌����擾
	D3DXVECTOR3 GetDestRotation(void) const;		// �ڕW�����擾

	// �ÓI�����o�֐�
	static CCamera *Create(void);				// ����
	static HRESULT Release(CCamera *&prCamera);	// �j��

private:
	// �����o�֐�
	void Follow(void);		// �J�����̍X�V (�Ǐ])
	void Bargaining(void);	// �J�����̍X�V (������)
	void Control(void);		// �J�����̍X�V (����)
	void Move(void);		// �ʒu�̍X�V (����)
	void Distance(void);	// �����̍X�V (����)
	void Rotation(void);	// �����̍X�V (����)

	// �����o�ϐ�
	Camera m_aCamera[TYPE_MAX];	// �J�����̏��
};

#endif	// _CAMERA_H_
