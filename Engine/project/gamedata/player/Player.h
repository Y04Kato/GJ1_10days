#pragma once
#include "Input.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Model.h>
#include <vector>

class Player {
public:
	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	//移動処理
	void Move();

	//落下処理
	void Fall();

	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	void SetTranslate(const Vector3 translate) { worldTransform_.translation_ = translate; }

	Vector3 GetVelocity() { return velocity_; }
	void SetVelocity(Vector3 velocity) { velocity_ = velocity; }

	bool GetIsFloorHit() { return isFloorHit_; }
	void SetIsFloorHit(bool isFloorHit) { isFloorHit_ = isFloorHit; }

	bool GetIsBlockHit() { return isBlockHit_; }
	void SetIsBlockHit(bool isBlockHit) { isBlockHit_ = isBlockHit; }

	bool GetIsStandingOnHit() { return isStandingOnHit_; }
	void SetIsStandingOnHit(bool isStandingOnHit) {
		isStandingOnHit_ = isStandingOnHit; 
		if (isStandingOnHit_ == true) {
			isJumpOn = true;
		}
	}

	bool GetIsCollidingFromSide() { return isCollidingFromSide_; }
	void SetIsCollidingFromSide(bool isCollidingFromSide) { isCollidingFromSide_ = isCollidingFromSide; }

	void SetIsReflection(bool isReflection) { isReflection_ = isReflection; reflectionRecoveryTimer_ = 0; }

	void SetFloorPos(WorldTransform& worldtransform) { floorTransform_ = worldtransform; }

private:
	Model* model_;

	WorldTransform worldTransform_;
	WorldTransform floorTransform_;

	Input* input_ = nullptr;

	float moveSpeed_ = 0.5f;

	Vector3 velocity_ = {};

	//各種ヒットフラグ
	bool isFloorHit_ = false;
	bool isBlockHit_ = false;
	bool isStandingOnHit_ = false;
	bool isCollidingFromSide_ = false;

	//反射用フラグ
	bool isReflection_ = false;
	const int reflectionRecoveryTime_ = 30;
	int reflectionRecoveryTimer_ = 0;

	//フリーズバグ修正用フラグ
	bool isAntiFreeze_ = false;
	const int antiFreezeTime_ = 3;
	int antiFreezeTimer_ = 0;

	bool isJumpOn = false;
};