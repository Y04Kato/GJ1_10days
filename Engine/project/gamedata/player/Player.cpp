#include "Player.h"

void Player::Initialize(Model* model) {
	model_ = model;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
}

void Player::Update() {

	//ImGui
	ImGui::Begin("Player");
	if (isFloorHit_ == true) {
		ImGui::Text("isFloorHit:True");
	}
	else {
		ImGui::Text("isFloorHit:False");
	}
	if (isBlockHit_ == true) {
		ImGui::Text("isBlockHit:True");
	}
	else {
		ImGui::Text("isBlockHit:False");
	}
	ImGui::End();

	if (isFloorHit_ == false && isBlockHit_ == false) {
		Fall();
	}
	else if(velocity_.num[1] > 0.0f) {
		
	}
	else {
		velocity_.num[1] = 0.0f;
	}

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, Vector4{ 0.2f,1.0f,0.2f,1.0f });
}

void Player::Move() {
	if (input_->PressKey(DIK_A)) {
		worldTransform_.translation_.num[0] -= 0.1f;
	}
	if (input_->PressKey(DIK_D)) {
		worldTransform_.translation_.num[0] += 0.1f;
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		velocity_.num[1] = 0.5f;
	}
}

void Player::Fall() {
	worldTransform_.translation_.num[1] += velocity_.num[1] / 2;
	const float kGravityAcceleration = 0.05f;
	Vector3 accelerationVector = { 0.0f,-kGravityAcceleration,0.0f };
	velocity_.num[1] += accelerationVector.num[1];
}