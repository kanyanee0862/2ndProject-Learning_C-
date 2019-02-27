#pragma once
class Component
{
public:
	//�R���X�g���N�^
	//Update order �̒l����������Ώ������قǑ����Ăяo�����
	Component(class Actor* owner, int updateOrder = 100);
	//�f�X�g���N�^
	virtual ~Component();
	//���Ԃ��X�V����
	virtual void Update(float deltaTime);
	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	//�A�N�^�[���L��
	class Actor* mOwner;
	//�X�V����
	int mUpdateOrder;
};
