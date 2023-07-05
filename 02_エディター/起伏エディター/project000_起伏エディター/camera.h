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
	// �R���X�g���N�^
	CCamera();

	// �f�X�g���N�^
	~CCamera();

	// �J�����\����
	typedef struct
	{
		D3DXVECTOR3  posV;			// ���݂̎��_
		D3DXVECTOR3  posR;			// ���݂̒����_
		D3DXVECTOR3  destPosV;		// �ڕW�̎��_
		D3DXVECTOR3  destPosR;		// �ڕW�̒����_
		D3DXVECTOR3  vecU;			// ������x�N�g��
		D3DXVECTOR3  rot;			// ���݂̌���
		D3DXVECTOR3  destRot;		// �ڕW�̌���
		float        fDis;			// ���_�ƒ����_�̋���
		D3DXMATRIX   mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
		D3DXMATRIX   mtxView;		// �r���[�}�g���b�N�X
		D3DVIEWPORT9 viewport;		// �r���[�|�[�g
	}Camera;

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void SetCamera(void);		// �J�����ݒ�
	void SetDestCamera(void);	// �J�����ڕW�ʒu�ݒ�

	void SetRotation(const D3DXVECTOR3& rRot);		// ���݌����ݒ�
	void SetDestRotation(const D3DXVECTOR3& rRot);	// �ڕW�����ݒ�
	Camera GetCamera(void);							// �J�����擾
	D3DXVECTOR3 GetRotation(void) const;			// ���݌����擾
	D3DXVECTOR3 GetDestRotation(void) const;		// �ڕW�����擾

	// �ÓI�����o�֐�
	static CCamera *Create(void);				// ����
	static HRESULT Release(CCamera *&prCamera);	// �j��

private:
	// �����o�֐�
	void MoveFollowCamera(void);	// �ʒu�̍X�V (�Ǐ])
	void MoveCamera(void);	// �ʒu�̍X�V (����)
	void DisCamera(void);	// �����̍X�V (����)
	void RotCamera(void);	// �����̍X�V (����)

	// �����o�ϐ�
	Camera m_camera;	// �J�����̏��
};

#endif	// _CAMERA_H_
