//============================================================
//
//	����֐����� [collision.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "collision.h"

//============================================================
//	3���̋�`�̓����蔻��
//============================================================
bool collision::Box3D
(
	D3DXVECTOR3 centerPos,		// ����ʒu
	D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
	D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
	D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
	D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
	D3DXVECTOR3 targetSizeDown	// ����ڕW�T�C�Y(���E���E�O)
)
{
	if (centerPos.x + centerSizeUp.x   > targetPos.x - targetSizeDown.x
	&&  centerPos.y + centerSizeUp.y   > targetPos.y - targetSizeDown.y
	&&  centerPos.z + centerSizeUp.z   > targetPos.z - targetSizeDown.z
	&&  centerPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x
	&&  centerPos.y - centerSizeDown.y < targetPos.y + targetSizeUp.y
	&&  centerPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // ������̏ꍇ

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	XZ���ʂ̉~�̓����蔻��
//============================================================
bool collision::Circle2D
(
	D3DXVECTOR3 centerPos,	// ����ʒu
	D3DXVECTOR3 targetPos,	// ����ڕW�ʒu
	float fCenterRadius,	// ���蔼�a
	float fTargetRadius		// ����ڕW���a
)
{
	// �ϐ���錾
	float fLength  = 0.0f;	// ����ʒu�Ɣ���ڕW�ʒu�̊Ԃ̋���

	// ����ʒu�Ɣ���ڕW�ʒu�̋��������߂�
	fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
			+ (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (fLength < ((fCenterRadius + fTargetRadius) * (fCenterRadius + fTargetRadius)))
	{ // ������̏ꍇ

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	3���̉~�̓����蔻��
//============================================================
bool collision::Circle3D
(
	D3DXVECTOR3 centerPos,	// ����ʒu
	D3DXVECTOR3 targetPos,	// ����ڕW�ʒu
	float fCenterRadius,	// ���蔼�a
	float fTargetRadius		// ����ڕW���a
)
{
	// �ϐ���錾
	float fLength  = 0.0f;	// ����ʒu�Ɣ���ڕW�ʒu�̊Ԃ̋���

	// ����ʒu�Ɣ���ڕW�ʒu�̋��������߂�
	fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
			+ (centerPos.y - targetPos.y) * (centerPos.y - targetPos.y)
			+ (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (fLength < ((fCenterRadius + fTargetRadius) * (fCenterRadius + fTargetRadius)))
	{ // ������̏ꍇ

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	XZ���ʂ̐�`�̓����蔻��
//============================================================
bool collision::Sector
(
	D3DXVECTOR3 centerPos,	// ����ʒu
	D3DXVECTOR3 targetPos,	// ����ڕW�ʒu
	float fCenterRot,		// �������
	float fRadius,			// ���E�͈�
	float fAngle,			// ����p
	float *pLength			// ����ڕW�Ƃ̋���
)
{
	// �ϐ���錾
	bool  bHit = false;		// �����蔻��̌���
	float fLength;			// ���S�ʒu�ƖڕW�ʒu�̋���
	float fHalfAngle;		// �����̊p�x�̔����̒l�̑���p
	D3DXVECTOR3 vecToPos;	// ���[�ƈʒu�̃x�N�g��

	// �ϐ��z���錾
	float fRotEdge[2];		// ��`�̉��̊p�x     [��] 0�F�� 1�F�E
	D3DXVECTOR3 posEdge[2];	// ��`�̉��̐�[�ʒu [��] 0�F�� 1�F�E
	D3DXVECTOR3 vecEdge[2];	// ��`�̉��x�N�g��   [��] 0�F�� 1�F�E

	// ���S�ʒu�ƖڕW�ʒu�̋������߂�
	fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
			+ (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (USED(pLength))
	{ // �|�C���^���g�p����Ă���ꍇ

		// ����ڕW�Ƃ̋�������
		*pLength = fLength;
	}

	if (fLength < fRadius * fRadius)
	{ // �~�͈͓̔��̏ꍇ

		// �����̊p�x�̔����̒l�����߂�
		fHalfAngle = fAngle * 0.5f;

		// ��`�̍����̊p�x�����߂�
		fRotEdge[0] = fCenterRot + fHalfAngle;	// �p�x�����ɌX����
		useful::NormalizeRot(fRotEdge[0]);		// �����𐳋K��

		// ��`�̉E���̊p�x�����߂�
		fRotEdge[1] = fCenterRot - fHalfAngle;	// �p�x���E�ɌX����
		useful::NormalizeRot(fRotEdge[1]);		// �����𐳋K��

		// ��`�̍����̐�[�ʒu�����߂�
		posEdge[0].x = centerPos.x + sinf(fRotEdge[0]) * 1.0f;
		posEdge[0].y = 0.0f;
		posEdge[0].z = centerPos.z + cosf(fRotEdge[0]) * 1.0f;

		// ��`�̉E���̐�[�ʒu�����߂�
		posEdge[1].x = centerPos.x + sinf(fRotEdge[1]) * 1.0f;
		posEdge[1].y = 0.0f;
		posEdge[1].z = centerPos.z + cosf(fRotEdge[1]) * 1.0f;

		// ��`�̍����̃x�N�g�������߂�
		vecEdge[0] = posEdge[0] - centerPos;
		vecEdge[0].y = 0.0f;

		// ��`�̉E���̃x�N�g�������߂�
		vecEdge[1] = posEdge[1] - centerPos;
		vecEdge[1].y = 0.0f;

		// ���[�ƈʒu�̃x�N�g�������߂�
		vecToPos = targetPos - centerPos;

		if ((vecEdge[0].z * vecToPos.x) - (vecEdge[0].x * vecToPos.z) < 0
		&&  (vecEdge[1].z * vecToPos.x) - (vecEdge[1].x * vecToPos.z) > 0)
		{ // ��`�̗����͈͓̔��̏ꍇ

			// �������Ă����Ԃɂ���
			bHit = true;
		}
	}

	// �����蔻��̌��ʂ�Ԃ�
	return bHit;
}

//============================================================
//	�p���̏Փ˔���
//============================================================
bool collision::BoxPillar
(
	D3DXVECTOR3& rCenterPos,	// ����ʒu
	D3DXVECTOR3& rCenterPosOld,	// ����ߋ��ʒu
	D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
	D3DXVECTOR3 centerSizeUp,	// ����T�C�Y(�E�E��E��)
	D3DXVECTOR3 centerSizeDown,	// ����T�C�Y(���E���E�O)
	D3DXVECTOR3 targetSizeUp,	// ����ڕW�T�C�Y(�E�E��E��)
	D3DXVECTOR3 targetSizeDown	// ����ڕW�T�C�Y(���E���E�O)
)
{
	// �ϐ���錾
	bool bHit = false;	// �Փ˔��茋��

	// �O��̓����蔻��
	if (rCenterPos.x + centerSizeUp.x > targetPos.x - targetSizeDown.x
	&&  rCenterPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x)
	{ // ���E�͈͓̔��̏ꍇ

		if (rCenterPos.z    + centerSizeUp.z >  targetPos.z - targetSizeDown.z
		&&  rCenterPosOld.z + centerSizeUp.z <= targetPos.z - targetSizeDown.z)
		{ // �O����̓����蔻��

			// �ʒu��␳
			rCenterPos.z = targetPos.z - targetSizeDown.z - centerSizeUp.z;

			// �Փˏ�Ԃɂ���
			bHit = true;
		}
		else if (rCenterPos.z    - centerSizeDown.z <  targetPos.z + targetSizeUp.z
		     &&  rCenterPosOld.z - centerSizeDown.z >= targetPos.z + targetSizeUp.z)
		{ // �ォ��̓����蔻��

			// �ʒu��␳
			rCenterPos.z = targetPos.z + targetSizeUp.z + centerSizeDown.z;

			// �Փˏ�Ԃɂ���
			bHit = true;
		}
	}

	// ���E�̓����蔻��
	if (rCenterPos.z + centerSizeUp.z > targetPos.z - targetSizeDown.z
	&&  rCenterPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // �O��͈͓̔��̏ꍇ

		if (rCenterPos.x    + centerSizeUp.x >  targetPos.x - targetSizeDown.x
		&&  rCenterPosOld.x + centerSizeUp.x <= targetPos.x - targetSizeDown.x)
		{ // ������̓����蔻��

			// �ʒu��␳
			rCenterPos.x = targetPos.x - targetSizeDown.x - centerSizeUp.x;

			// �Փˏ�Ԃɂ���
			bHit = true;
		}
		else if (rCenterPos.x    - centerSizeDown.x <  targetPos.x + targetSizeUp.x
		     &&  rCenterPosOld.x - centerSizeDown.x >= targetPos.x + targetSizeUp.x)
		{ // �E����̓����蔻��
			
			// �ʒu��␳
			rCenterPos.x = targetPos.x + targetSizeUp.x + centerSizeDown.x;

			// �Փˏ�Ԃɂ���
			bHit = true;
		}
	}

	// �Փ˔����Ԃ�
	return bHit;
}

//============================================================
//	�~���̏Փ˔���
//============================================================
bool collision::CirclePillar
(
	D3DXVECTOR3& rCenterPos,	// ����ʒu
	D3DXVECTOR3 targetPos,		// ����ڕW�ʒu
	float fCenterRadius,		// ���蔼�a
	float fTargetRadius			// ����ڕW���a
)
{
	// �ϐ���錾
	float fLength = 0.0f;	// ����ʒu�Ɣ���ڕW�ʒu�̊Ԃ̋���

	// ����ʒu�Ɣ���ڕW�ʒu�̋��������߂�
	fLength = (rCenterPos.x - targetPos.x) * (rCenterPos.x - targetPos.x)
			+ (rCenterPos.z - targetPos.z) * (rCenterPos.z - targetPos.z);

	if (fLength < (fCenterRadius + fTargetRadius) * (fCenterRadius + fTargetRadius))
	{ // ������̏ꍇ

		// �ϐ���錾
		float fCenterRot = atan2f(rCenterPos.x - targetPos.x, rCenterPos.z - targetPos.z);	// ����ڕW���猩���������

		// �ʒu��␳
		rCenterPos.x = targetPos.x + sinf(fCenterRot) * (fCenterRadius + fTargetRadius);
		rCenterPos.z = targetPos.z + cosf(fCenterRot) * (fCenterRadius + fTargetRadius);

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	�O�ς̍��E����
//============================================================
//	���E�����猩�č��E�ǂ���ɂ��邩�̔���Ɏg�p
//============================================================
float collision::LineOuterProduct
(
	D3DXVECTOR3 posLeft,	// ���E�������W
	D3DXVECTOR3 posRight,	// ���E���E���W
	D3DXVECTOR3 pos			// ����ʒu
)
{
	// �ϐ���錾
	D3DXVECTOR3 vecLine;	// ���E���x�N�g��
	D3DXVECTOR3 vecToPos;	// ���[�ƈʒu�̃x�N�g��

	// ���E���x�N�g�������߂�
	vecLine = posRight - posLeft;

	// ���[�ƈʒu�̃x�N�g�������߂�
	vecToPos = pos - posLeft;

	// �O�ς̌v�Z���ʂ�Ԃ�
	return (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
}
