#include "Player.h"

void Player::Initialize(Model* model) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 0.8f,0.8f,0.8f };
	input_ = Input::GetInstance();

	audio_ = Audio::GetInstance();
	set_ = audio_->SoundLoad("project/gamedata/resources/SE/Set.mp3");
	jump_ = audio_->SoundLoad("project/gamedata/resources/SE/Jump.mp3");
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
	if (isFloorHit_ == true) {
		ImGui::Text("isFloorHit:True");
	}
	else {
		ImGui::Text("isFloorHit:False");
	}
	if (isReflection_ == true) {
		ImGui::Text("isReflection:True");
	}
	else {
		ImGui::Text("isReflection:False");
	}
	ImGui::End();

	//床などの処理
	if (isFloorHit_ == false && isBlockHit_ == false) {
		Fall();
	}
	else if (isFloorHit_ == true) {
		worldTransform_.translation_.num[1] = floorTransform_.translation_.num[1] + floorTransform_.scale_.num[1] + worldTransform_.scale_.num[1] + 0.05f;
	}
	else if (velocity_.num[1] > 0.0f) {

	}
	else {
		velocity_.num[1] = 0.0f;
	}

	//反射フラグ
	if (isReflection_ == true) {
		reflectionRecoveryTimer_++;
	}
	if (reflectionRecoveryTimer_ >= reflectionRecoveryTime_) {
		isReflection_ = false;
		reflectionRecoveryTimer_ = 0;
	}

	//ブロック角によるフリーズバグ修正用
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

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, Vector4{ 0.2f,1.0f,0.2f,1.0f });
}

void Player::Move() {
	bool isMoving = false;

	//左右移動
	if (isReflection_ == false || isAntiFreeze_ == true) {
		if (input_->PressKey(DIK_A)) {
			velocity_.num[0] = -0.15f;
			isMoving = true;
		}
		if (input_->PressKey(DIK_D)) {
			velocity_.num[0] = 0.15f;
			isMoving = true;
		}
	}

	isMoving_ = isMoving;

	//ジャンプ
	if (isCollidingFromSide_ == false || isAntiFreeze_ == true) {
		if (isFloorHit_ == true || isJumpOn == true) {
			if (input_->TriggerKey(DIK_SPACE) && isJumpBlockStanding_ == false) {
				velocity_.num[1] = 0.3f;
				isJumpOn = false;
				audio_->SoundPlayWave(jump_, 0.1f, false);
			}
			if (input_->TriggerKey(DIK_SPACE) && isJumpBlockStanding_ == true) {
				velocity_.num[1] = 0.6f;
				isJumpOn = false;
				isJumpBlockStanding_ = false;
				audio_->SoundPlayWave(jump_, 0.2f, false);
			}
		}
	}
}

void Player::Fall() {
	worldTransform_.translation_.num[1] += velocity_.num[1] / 2;
	const float kGravityAcceleration = 0.05f;
	Vector3 accelerationVector = { 0.0f,-kGravityAcceleration,0.0f };
	velocity_.num[1] += accelerationVector.num[1] / 2;
}

void Player::SetIsFloorHit(bool isFloorHit) { 
	isFloorHit_ = isFloorHit; 
}

void Player::SetIsStandingOnHit(bool isStandingOnHit) {
	isStandingOnHit_ = isStandingOnHit;
	if (isStandingOnHit_ == true) {
		if (isJumpOn == false) {
			audio_->SoundPlayWave(set_, 0.1f, false);
		}
		isJumpOn = true;
	}
}