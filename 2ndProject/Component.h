#pragma once
class Component
{
public:
	//コンストラクタ
	//Update order の値が小さければ小さいほど早く呼び出される
	Component(class Actor* owner, int updateOrder = 100);
	//デストラクタ
	virtual ~Component();
	//時間より更新する
	virtual void Update(float deltaTime);
	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	//アクター所有者
	class Actor* mOwner;
	//更新順番
	int mUpdateOrder;
};
