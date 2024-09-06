#include "Player.h"

void Player::Initialize(Model* model) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 0.8f,0.8f,0.8f };
	input_ = Input::GetInstance();
}

void Player::Update() {
	//ImGui
	ImGui::Begin("Player");
	if (isStandingOnHit_ == true) {
		ImGui::Text("isStandingOnHit:True");
	}
	else {
		ImGui::Text("isStandingOnHit:False");
	}
	if (isCollidingFromSide_ == true) {
		ImGui::Text("isCollidingFromSide:True");
	}
	else {
		ImGui::Text("isCollidingFromSide:False");
	}
	if (isReflection_ == true) {
		ImGui::Text("isReflection:True");
	}
	else {
		ImGui::Text("isReflection:False");
	}
	ImGui::End();

	if (isFloorHit_ == false && isBlockHit_ == false) {
		Fall();
	}
	else if (velocity_.num[1] > 0.0f) {

	}
	else {
		velocity_.num[1] = 0.0f;
		worldTransform_.translation_.num[1] = floorTransform_.translation_.num[1] + floorTransform_.scale_.num[1] + worldTransform_.scale_.num[1];
	}

	if (isReflection_ == true) {
		reflectionRecoveryTimer_++;
	}
	if (reflectionRecoveryTimer_ >= reflectionRecoveryTime_) {
		isReflection_ = false;
		reflectionRecoveryTimer_ = 0;
	}

	if (isReflection_ == true && isCollidingFromSide_ == true) {
		antiFreezeTimer_++;
	}
	else {
		isAntiFreeze_ = false;
		antiFreezeTimer_ = 0;
	}
	if (antiFreezeTimer_ >= antiFreezeTime_) {
		isAntiFreeze_ = true;
		antiFreezeTimer_ = 0;
	}

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, Vector4{ 0.2f,1.0f,0.2f,1.0f });
}

void Player::Move() {
	if (isReflection_ == false || isAntiFreeze_ == true) {
		if (input_->PressKey(DIK_A)) {
			velocity_.num[0] = -0.1f;
		}
		if (input_->PressKey(DIK_D)) {
			velocity_.num[0] = 0.1f;
		}
	}

	if (isCollidingFromSide_ == false) {
		if (input_->TriggerKey(DIK_SPACE)) {
			velocity_.num[1] = 0.5f;

		}
	}

	//加速度減衰処理
	const float kGravityAcceleration = 0.01f;

	if (velocity_.num[0] >= 0.01f) {
		velocity_.num[0] -= kGravityAcceleration;
	}
	else if (velocity_.num[0] <= -0.01f) {
		velocity_.num[0] += kGravityAcceleration;
	}
	else {
		velocity_.num[0] = 0.0f;
	}

	if (velocity_.num[2] >= 0.01f) {
		velocity_.num[2] -= kGravityAcceleration;
	}
	else if (velocity_.num[2] <= -0.01f) {
		velocity_.num[2] += kGravityAcceleration;
	}
	else {
		velocity_.num[2] = 0.0f;
	}
}

void Player::Fall() {
	worldTransform_.translation_.num[1] += velocity_.num[1] / 2;
	const float kGravityAcceleration = 0.05f;
	Vector3 accelerationVector = { 0.0f,-kGravityAcceleration,0.0f };
	velocity_.num[1] += accelerationVector.num[1];
}