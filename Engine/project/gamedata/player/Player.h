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

private:
	Model* model_;

	WorldTransform worldTransform_;

	Input* input_ = nullptr;

	float moveSpeed_ = 0.5f;

	Vector3 velocity_ = {};

	bool isFloorHit_ = false;
	bool isBlockHit_ = false;
};