#include "MathCalculation.h"

#pragma region float
float Length(const Vector3& v) {
	return sqrtf(Dot(v, v));
}

float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.num[0] * v2.num[0] + v1.num[1] * v2.num[1] + v1.num[2] * v2.num[2];
}

float Magnitude(const Vector3& v) {
	return sqrt(v.num[0] * v.num[0] + v.num[1] * v.num[1] + v.num[2] * v.num[2]);
}

float LengthQuaternion(const Quaternion& q) {
	return sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}

float Contangent(float b, float a) {
	return(b / tan(a));
}

float Distance(const Vector3& v1, const Vector3& v2) {
	return sqrt((v2.num[0] - v1.num[0]) * (v2.num[0] - v1.num[0]) + (v2.num[1] - v1.num[1]) * (v2.num[1] - v1.num[1]) + (v2.num[2] - v1.num[2]) * (v2.num[2] - v1.num[2]));
}

float Lerp(float a, float b, float t) {
	return a + b * t;
}

float LerpShortAngle(float a, float b, float t) {
	//角度差分を求める
	float diff = b - a;
	diff = std::fmod(diff, 2 * (float)std::numbers::pi);
	if (diff < 2 * -(float)std::numbers::pi) {
		diff += 2 * (float)std::numbers::pi;
	}
	else if (diff >= 2 * (float)std::numbers::pi) {
		diff -= 2 * (float)std::numbers::pi;
	}

	diff = std::fmod(diff, 2 * (float)std::numbers::pi);
	if (diff < -(float)std::numbers::pi) {
		diff += 2 * (float)std::numbers::pi;
	}
	else if (diff >= (float)std::numbers::pi) {
		diff -= 2 * (float)std::numbers::pi;
	}

	return Lerp(a, diff, t);
}
#pragma endregion

#pragma region Vector2
Vector2 operator+(const Vector2& v1, const Vector2& v2) { return Add(v1, v2); }
Vector2 operator-(const Vector2& v1, const Vector2& v2) { return Subtruct(v1, v2); }
Vector2 operator*(float k, const Vector2& v) { return Multiply(k, v); }
Vector2 operator*(const Vector2& v, float k) { return Multiply(k, v); }

Vector2 Add(const Vector2& v1, const Vector2& v2) {
	Vector2 result;
	result.num[0] = v1.num[0] + v2.num[0];
	result.num[1] = v1.num[1] + v2.num[1];
	return result;
}

Vector2 Subtruct(const Vector2& v1, const Vector2& v2) {
	Vector2 v;
	v.num[0] = v1.num[0] - v2.num[0];
	v.num[1] = v1.num[1] - v2.num[1];
	return v;
}

Vector2 Multiply(float scalar, const Vector2& v) {
	Vector2 returnV;
	returnV.num[0] = v.num[0] * scalar;
	returnV.num[1] = v.num[1] * scalar;
	return returnV;
}

float Cross(Vector2 a, Vector2 b) {
	return a.num[0] * b.num[1] - a.num[1] * b.num[0];
}

Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t) { return v1 + Multiply(t, v2 - v1); }

#pragma endregion

#pragma region Vector3
Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Add(v1, v2); }
Vector3 operator-(const Vector3& v) { return { -v.num[0], -v.num[1], -v.num[2] }; }
Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Subtruct(v1, v2); }
Vector3 operator*(float k, const Vector3& v) { return Multiply(k, v); }
Vector3 operator*(const Vector3& v, float k) { return Multiply(k, v); }
Vector3 operator*(const Vector3& v1, const Vector3& v2) { return Multiply(v1, v2); }
Vector3 operator*(const Vector3& v, const Matrix4x4& matrix) { return TransformN(v, matrix); }
Vector3 operator/(const Vector3& v, float k) { return Division(k, v); }
Vector3 operator+=(Vector3& v1, Vector3& v2) { return v1 = Add(v1, v2); }
Vector3 operator+=(Vector3& v1, const Vector3& v2) { return v1 = Add(v1, v2); }
Vector3 operator-=(const Vector3& v1, const Vector3& v2) { return Subtruct(v1, v2); }

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.num[0] = v1.num[0] + v2.num[0];
	result.num[1] = v1.num[1] + v2.num[1];
	result.num[2] = v1.num[2] + v2.num[2];
	return result;
}

Vector3 Subtruct(const Vector3& v1, const Vector3& v2) {
	Vector3 v;
	v.num[0] = v1.num[0] - v2.num[0];
	v.num[1] = v1.num[1] - v2.num[1];
	v.num[2] = v1.num[2] - v2.num[2];
	return v;
}

Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 returnV;
	returnV.num[0] = v.num[0] * scalar;
	returnV.num[1] = v.num[1] * scalar;
	returnV.num[2] = v.num[2] * scalar;
	return returnV;
}

Vector3 Multiply(const Vector3& v1, const Vector3& v2) {
	Vector3 returnV;
	returnV.num[0] = v1.num[0] * v2.num[0];
	returnV.num[1] = v1.num[1] * v2.num[1];
	returnV.num[2] = v1.num[2] * v2.num[2];
	return returnV;
}

Vector3 ScalarMultiply(const Vector3& vec, float scalar) {
	return { vec.num[0] * scalar, vec.num[1] * scalar, vec.num[2] * scalar };
}

Vector3 Division(float scalar, const Vector3& v) {
	Vector3 returnV;
	returnV.num[0] = v.num[0] / scalar;
	returnV.num[1] = v.num[1] / scalar;
	returnV.num[2] = v.num[2] / scalar;
	return returnV;
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
		v.num[0] * m.m[0][0] + v.num[1] * m.m[1][0] + v.num[2] * m.m[2][0],
		v.num[0] * m.m[0][1] + v.num[1] * m.m[1][1] + v.num[2] * m.m[2][1],
		v.num[0] * m.m[0][2] + v.num[1] * m.m[1][2] + v.num[2] * m.m[2][2],
	};
	return result;
}

Vector3 TransformN(const Vector3& v, const Matrix4x4& m) {
	Vector3 result;
	float transformMatrix[4];
	float matrix4[4] = { v.num[0],v.num[1],v.num[2] ,1.0f };
	for (int column = 0; column < 4; column++) {
		transformMatrix[column] = 0.0f;
		for (int i = 0; i < 4; i++) {
			transformMatrix[column] += matrix4[i] * m.m[i][column];
		}
	}
	float w = transformMatrix[3];
	assert(w != 0.0f);
	result.num[0] = transformMatrix[0] / w;
	result.num[1] = transformMatrix[1] / w;
	result.num[2] = transformMatrix[2] / w;
	return result;
}

Vector3 Normalize(const Vector3& v) {
	float len = Length(v);
	if (len != 0) {
		return { v.num[0] / len,v.num[1] / len,v.num[2] / len };
	}
	return v;
}

Vector3 GetXAxis(const Matrix4x4& m) {
	return { m.m[0][0],m.m[0][1],m.m[0][2] };
}

Vector3 GetYAxis(const Matrix4x4& m) {
	return { m.m[1][0],m.m[1][1],m.m[1][2] };
}

Vector3 GetZAxis(const Matrix4x4& m) {
	return { m.m[2][0],m.m[2][1],m.m[2][2] };
}

void GetOrientations(const Matrix4x4& m, Vector3 orientations[3]) {
	orientations[0] = GetXAxis(m);
	orientations[1] = GetYAxis(m);
	orientations[2] = GetZAxis(m);
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.num[0] = (v1.num[1] * v2.num[2] - v1.num[2] * v2.num[1]);
	result.num[1] = (v1.num[2] * v2.num[0] - v1.num[0] * v2.num[2]);
	result.num[2] = (v1.num[0] * v2.num[1] - v1.num[1] * v2.num[0]);
	return result;
}

float Angle(Vector3 from, Vector3 to) {
	from = Normalize(from);
	to = Normalize(to);
	Vector2 from2 = { from.num[0],from.num[2] };
	Vector2 to2 = { to.num[0],to.num[2] };
	float dot = Dot(from, to);
	if (dot >= 1.0f) {
		return 0.0f;
	}
	if (dot <= -1.0f) {
		return 180.0f * (2.0f / 180.0f);
	}
	if (Cross(from2, to2) > 0) {
		return -std::acosf(dot);
	}
	else {
		return std::acosf(dot);
	}
}

Vector3 matrixToEulerAngles(const Matrix4x4 mat) {
	float pitch;
	float yaw;
	float roll;
	pitch = asin(-mat.m[2][0]);

	if (cos(pitch) != 0.0f) {
		roll = atan2(mat.m[2][1] / cos(pitch), mat.m[2][2] / cos(pitch));
		yaw = atan2(mat.m[1][0] / cos(pitch), mat.m[0][0] / cos(pitch));
	}
	else {
		//ジンバルロックの場合
		yaw = 0.0f;
		roll = atan2(-mat.m[0][1], mat.m[1][1]);
	}

	//ラジアンから度に変換
	//yaw = yaw * 180.0f / PI;
	//pitch = pitch * 180.0f / PI;
	//roll = roll * 180.0f / PI;
	return{ yaw,pitch,roll };
}

Vector3 extractEulerAnglesFromRotationMatrix(const Matrix4x4& rotationMatrix) {
	Vector3 eulerAngle;

	//回転行列の各要素を抽出
	float m00 = rotationMatrix.m[0][0];
	float m01 = rotationMatrix.m[0][1];
	float m02 = rotationMatrix.m[0][2];
	float m10 = rotationMatrix.m[1][0];
	float m11 = rotationMatrix.m[1][1];
	float m12 = rotationMatrix.m[1][2];
	float m20 = rotationMatrix.m[2][0];
	float m21 = rotationMatrix.m[2][1];
	float m22 = rotationMatrix.m[2][2];

	//Yaw角度の計算
	eulerAngle.num[0] = atan2(m01, m00);

	//Pitch角度の計算
	eulerAngle.num[1] = atan2(-m02, sqrt(m12 * m12 + m22 * m22));

	//Roll角度の計算
	eulerAngle.num[2] = atan2(m12, m22);

	return eulerAngle;
}

Vector3 GetRightVectorFromModelMatrix(const Matrix4x4& modelMatrix) {
	//モデル横方をXとする
	Vector3 rightVector;
	rightVector.num[0] = modelMatrix.m[0][0];
	rightVector.num[1] = modelMatrix.m[1][0];
	rightVector.num[2] = modelMatrix.m[2][0];

	return rightVector;
}

Vector3 GetUpVectorFromModelMatrix(const Matrix4x4& modelMatrix) {
	//モデル上方をYとする
	Vector3 upVector;
	upVector.num[0] = modelMatrix.m[0][1];
	upVector.num[1] = modelMatrix.m[1][1];
	upVector.num[2] = modelMatrix.m[2][1];

	return upVector;
}

Vector3 GetFrontVectorFromModelMatrix(const Matrix4x4& modelMatrix) {
	//モデル前方をZとする
	Vector3 frontVector;
	frontVector.num[0] = modelMatrix.m[0][2];
	frontVector.num[1] = modelMatrix.m[1][2];
	frontVector.num[2] = modelMatrix.m[2][2];

	return frontVector;
}

bool CompereVector3(const Vector3& q1, const Vector3& q2) {
	if (q1.num[0] == q2.num[0] && q1.num[1] == q2.num[1] && q1.num[2] == q2.num[2]) {
		return true;
	}
	else {
		return false;
	}
}

Vector3 MidPoint(const Vector3& v1, const Vector3& v2) {
	Vector3 result;

	result.num[0] = (v1.num[0] + v2.num[0]) / 2.0f;
	result.num[1] = (v1.num[1] + v2.num[1]) / 2.0f;
	result.num[2] = (v1.num[2] + v2.num[2]) / 2.0f;

	return result;
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return v1 + Multiply(t, v2 - v1);
}

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 a = Normalize(v1), b = Normalize(v2);
	float s = (1.0f - t) * Length(a) + t * Length(b);
	Vector3 e1, e2;
	e1 = float(1.0f / Length(a)) * a;
	e2 = float(1.0f / Length(b)) * b;

	float dot = std::clamp(Dot(a, b), 0.0f, 1.0f);
	float theta = std::acos(dot/*/( Length(a)*Length(b))*/);
	if (theta == 0.0f) {
		return Lerp(a, b, t);
	}
	return s * ((std::sinf((1.0f - t) * theta) / std::sinf(theta)) * a + (std::sinf(t * theta) / std::sinf(theta)) * b);
}

Vector3 Project(const Vector3& v, const Vector3 n) {
	float dotProduct = Dot(v, n);
	float ontoLengthSquared = Dot(n, n);
	return ScalarMultiply(n, dotProduct / ontoLengthSquared);
}

std::tuple<Vector3, Vector3, Vector3> ComputeRotationMatrix(const Vector3& rotate) {
	Vector3 result[3];

	float cx = cos(rotate.num[0]);
	float sx = sin(rotate.num[0]);
	float cy = cos(rotate.num[1]);
	float sy = sin(rotate.num[1]);
	float cz = cos(rotate.num[2]);
	float sz = sin(rotate.num[2]);

	// 回転行列の各軸のベクトルを計算
	result[0] = Vector3{ cy * cz, cy * sz, -sy };
	result[1] = Vector3{ sx * sy * cz - cx * sz, sx * sy * sz + cx * cz, sx * cy };
	result[2] = Vector3{ cx * sy * cz + sx * sz, cx * sy * sz - sx * cz, cx * cy };

	return std::make_tuple(result[0], result[1], result[2]);
}

std::pair<Vector3, Vector3> ComputeCollisionVelocities(float mass1, const Vector3& velocity1, float mass2, const Vector3& velocity2, float coefficientOfRestitution, const Vector3& normal) {
	// 法線ベクトルを正規化
	Vector3 n = Normalize(normal);

	// 速度の法線成分と接線成分を計算
	Vector3 v1n = Project(velocity1, n);
	Vector3 v1t = Subtruct(velocity1, v1n);
	Vector3 v2n = Project(velocity2, n);
	Vector3 v2t = Subtruct(velocity2, v2n);

	// 衝突後の法線方向の速度を計算
	float v1n_dot_n = Dot(v1n, n);
	float v2n_dot_n = Dot(v2n, n);

	float new_v1n_dot_n = (v1n_dot_n * (mass1 - coefficientOfRestitution * mass2) + v2n_dot_n * (1 + coefficientOfRestitution) * mass2) / (mass1 + mass2);
	float new_v2n_dot_n = (v2n_dot_n * (mass2 - coefficientOfRestitution * mass1) + v1n_dot_n * (1 + coefficientOfRestitution) * mass1) / (mass1 + mass2);

	Vector3 new_v1n = ScalarMultiply(n, new_v1n_dot_n);
	Vector3 new_v2n = ScalarMultiply(n, new_v2n_dot_n);

	// 衝突後の速度は法線成分と接線成分の和
	Vector3 new_velocity1 = Add(new_v1n, v1t);
	Vector3 new_velocity2 = Add(new_v2n, v2t);

	return std::make_pair(new_velocity1, new_velocity2);
}

Vector3 ComputeSphereVelocityAfterCollisionWithOBB(const StructSphere& sphere, const Vector3& sphereVelocity, const OBB& obb, float restitution) {
	//最近接点を計算
	Vector3 closestPoint = obb.center;
	Vector3 d = sphere.center - obb.center;

	for (int i = 0; i < 3; ++i) {
		float dist = Dot(d, obb.orientation[i]);
		dist = std::fmax(-obb.size.num[i], std::fmin(dist, obb.size.num[i]));
		closestPoint += obb.orientation[i] * dist;
	}

	//衝突法線を計算
	Vector3 collisionNormal = Normalize(sphere.center - closestPoint);

	//速度を衝突面法線方向とその他に分解
	Vector3 velocityNormal = Project(sphereVelocity, collisionNormal);
	Vector3 velocityTangent = sphereVelocity - velocityNormal;

	//反発係数を考慮して新しい速度を計算
	Vector3 newVelocityNormal = -restitution * velocityNormal;
	Vector3 newSphereVelocity = newVelocityNormal + velocityTangent;

	return newSphereVelocity;
}

Vector3 ComputeOBBRepulsion(const OBB& obb1, const Vector3& obb1Velocity, const OBB& obb2, float restitution) {
	//obb1の中心に最も近いobb2上の点を計算
	Vector3 closestPoint = obb2.center;
	Vector3 d = obb1.center - obb2.center;

	//obb2の軸に投影して、obb2上の最も近い点を見つける
	for (int i = 0; i < 3; ++i) {
		float dist = Dot(d, obb2.orientation[i]);
		dist = std::fmax(-obb2.size.num[i], std::fmin(dist, obb2.size.num[i]));
		closestPoint = closestPoint + obb2.orientation[i] * dist;
	}

	//侵入ベクトルを計算
	Vector3 penetrationVector = obb1.center - closestPoint;

	//衝突がないか確認する
	float penetrationDistance = std::sqrt(penetrationVector.num[0] * penetrationVector.num[0] +
		penetrationVector.num[1] * penetrationVector.num[1] +
		penetrationVector.num[2] * penetrationVector.num[2]);

	//貫通ベクトルを正規化して衝突法線を取得
	Vector3 collisionNormal = Normalize(penetrationVector);

	//衝突法線方向の相対速度を計算
	float relativeVelocity = Dot(obb1Velocity, collisionNormal);

	//スカラーを計算
	float impulseScalar = -(1.0f + restitution) * relativeVelocity;

	//obb1に適用して新しい速度を取得します
	Vector3 newVelocity = obb1Velocity + collisionNormal * impulseScalar;

	//新しい速度を返す
	return newVelocity;
}

Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframe, float time) {
	assert(!keyframe.empty());//キーがない物は返す値がないのでダメ
	if (keyframe.size() == 1 || time <= keyframe[0].time) {
		return keyframe[0].value;
	}

	for (size_t index = 0; index < keyframe.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		//indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframe[index].time <= time && time <= keyframe[nextIndex].time) {
			//範囲内の補完
			float t = (time - keyframe[index].time) / (keyframe[nextIndex].time - keyframe[index].time);
			return Lerp(keyframe[index].value, keyframe[nextIndex].value, t);
		}
	}

	//ここまで来た場合、一番後の時刻より後ろになっているので最後の値を返す
	return (*keyframe.rbegin()).value;
}

Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframe, float time) {
	assert(!keyframe.empty());//キーがない物は返す値がないのでダメ
	if (keyframe.size() == 1 || time <= keyframe[0].time) {
		return keyframe[0].value;
	}

	for (size_t index = 0; index < keyframe.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		//indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframe[index].time <= time && time <= keyframe[nextIndex].time) {
			//範囲内の補完
			float t = (time - keyframe[index].time) / (keyframe[nextIndex].time - keyframe[index].time);
			return Slerp(t, keyframe[index].value, keyframe[nextIndex].value);
		}
	}

	//ここまで来た場合、一番後の時刻より後ろになっているので最後の値を返す
	return (*keyframe.rbegin()).value;
}

#pragma endregion

#pragma region Matrix4x4
Matrix4x4 operator+(Matrix4x4 m1, Matrix4x4 m2) { return Add(m1, m2); }
Matrix4x4 operator-(Matrix4x4 m1, Matrix4x4 m2) { return Subtruct(m1, m2); }
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) { return Multiply(m1, m2); }
Matrix4x4 operator+=(Matrix4x4 m1, Matrix4x4 m2) { return Add(m1, m2); }
Matrix4x4 operator-=(Matrix4x4& m1, const Matrix4x4& m2) { return m1 = Subtruct(m1, m2); }
Matrix4x4 operator*=(Matrix4x4& m1, const Matrix4x4& m2) { return m1 = Multiply(m1, m2); }

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 Subtruct(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float Term = 0;
			for (int k = 0; k < 4; k++) {
				Term = Term + m1.m[i][k] * m2.m[k][j];
				result.m[i][j] = Term;
			}
		}
	}
	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = -std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = std::cos(radian);
	result.m[0][1] = 0.0f;
	result.m[0][2] = -std::sin(radian);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = std::sin(radian);
	result.m[2][1] = 0.0f;
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = -std::sin(radian);
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateMatrix(Vector3 theta) {
	Matrix4x4 rotateX = MakeRotateXMatrix(theta.num[0]);
	Matrix4x4 rotateY = MakeRotateYMatrix(theta.num[1]);
	Matrix4x4 rotateZ = MakeRotateZMatrix(theta.num[2]);
	return Multiply(rotateX, Multiply(rotateY, rotateZ));
}

Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotate) {
	Vector3 s = { std::sin(rotate.num[0]), std::sin(rotate.num[1]), std::sin(rotate.num[2]) };
	Vector3 c = { std::cos(rotate.num[0]), std::cos(rotate.num[1]), std::cos(rotate.num[2]) };
	return {
		c.num[1] * c.num[2],c.num[1] * s.num[2],-s.num[1],0.0f,
		s.num[0] * s.num[1] * c.num[2] - c.num[0] * s.num[2],	s.num[0] * s.num[1] * s.num[2] + c.num[0] * c.num[2],	s.num[0] * c.num[1],	0.0f,
		c.num[0] * s.num[1] * c.num[2] + s.num[0] * s.num[2],	c.num[0] * s.num[1] * s.num[2] - s.num[0] * c.num[2],	c.num[0] * c.num[1],	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f };
}

Matrix4x4 MakeTranslateMatrix(Vector3 translate) {
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = translate.num[0];
	result.m[3][1] = translate.num[1];
	result.m[3][2] = translate.num[2];
	result.m[3][3] = 1.0f;

	return result;
};

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;

	result.m[0][0] = scale.num[0];
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.num[1];
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = scale.num[2];
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result;
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.num[0]);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.num[1]);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.num[2]);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	result = Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
	return result;
}

Matrix4x4 Inverse(const Matrix4x4& m1) {
	Matrix4x4 result;
	float deterninant = m1.m[0][0] * m1.m[1][1] * m1.m[2][2] * m1.m[3][3] +
		m1.m[0][0] * m1.m[1][2] * m1.m[2][3] * m1.m[3][1] +
		m1.m[0][0] * m1.m[1][3] * m1.m[2][1] * m1.m[3][2] -

		m1.m[0][0] * m1.m[1][3] * m1.m[2][2] * m1.m[3][1] -
		m1.m[0][0] * m1.m[1][2] * m1.m[2][1] * m1.m[3][3] -
		m1.m[0][0] * m1.m[1][1] * m1.m[2][3] * m1.m[3][2] -

		m1.m[0][1] * m1.m[1][0] * m1.m[2][2] * m1.m[3][3] -
		m1.m[0][2] * m1.m[1][0] * m1.m[2][3] * m1.m[3][1] -
		m1.m[0][3] * m1.m[1][0] * m1.m[2][1] * m1.m[3][2] +

		m1.m[0][3] * m1.m[1][0] * m1.m[2][2] * m1.m[3][1] +
		m1.m[0][2] * m1.m[1][0] * m1.m[2][1] * m1.m[3][3] +
		m1.m[0][1] * m1.m[1][0] * m1.m[2][3] * m1.m[3][2] +

		m1.m[0][1] * m1.m[1][2] * m1.m[2][0] * m1.m[3][3] +
		m1.m[0][2] * m1.m[1][3] * m1.m[2][0] * m1.m[3][1] +
		m1.m[0][3] * m1.m[1][1] * m1.m[2][0] * m1.m[3][2] -

		m1.m[0][3] * m1.m[1][2] * m1.m[2][0] * m1.m[3][1] -
		m1.m[0][2] * m1.m[1][1] * m1.m[2][0] * m1.m[3][3] -
		m1.m[0][1] * m1.m[1][3] * m1.m[2][0] * m1.m[3][2] -

		m1.m[0][1] * m1.m[1][2] * m1.m[2][3] * m1.m[3][0] -
		m1.m[0][2] * m1.m[1][3] * m1.m[2][1] * m1.m[3][0] -
		m1.m[0][3] * m1.m[1][1] * m1.m[2][2] * m1.m[3][0] +

		m1.m[0][3] * m1.m[1][2] * m1.m[2][1] * m1.m[3][0] +
		m1.m[0][2] * m1.m[1][1] * m1.m[2][3] * m1.m[3][0] +
		m1.m[0][1] * m1.m[1][3] * m1.m[2][2] * m1.m[3][0];

	float deterninantRect = 1.0f / deterninant;

	result.m[0][0] =
		(m1.m[1][1] * m1.m[2][2] * m1.m[3][3] +
			m1.m[1][2] * m1.m[2][3] * m1.m[3][1] +
			m1.m[1][3] * m1.m[2][1] * m1.m[3][2] -
			m1.m[1][3] * m1.m[2][2] * m1.m[3][1] -
			m1.m[1][2] * m1.m[2][1] * m1.m[3][3] -
			m1.m[1][1] * m1.m[2][3] * m1.m[3][2]) * deterninantRect;
	result.m[0][1] =
		(-m1.m[0][1] * m1.m[2][2] * m1.m[3][3] -
			m1.m[0][2] * m1.m[2][3] * m1.m[3][1] -
			m1.m[0][3] * m1.m[2][1] * m1.m[3][2] +
			m1.m[0][3] * m1.m[2][2] * m1.m[3][1] +
			m1.m[0][2] * m1.m[2][1] * m1.m[3][3] +
			m1.m[0][1] * m1.m[2][3] * m1.m[3][2]) * deterninantRect;
	result.m[0][2] =
		(m1.m[0][1] * m1.m[1][2] * m1.m[3][3] +
			m1.m[0][2] * m1.m[1][3] * m1.m[3][1] +
			m1.m[0][3] * m1.m[1][1] * m1.m[3][2] -
			m1.m[0][3] * m1.m[1][2] * m1.m[3][1] -
			m1.m[0][2] * m1.m[1][1] * m1.m[3][3] -
			m1.m[0][1] * m1.m[1][3] * m1.m[3][2]) * deterninantRect;
	result.m[0][3] =
		(-m1.m[0][1] * m1.m[1][2] * m1.m[2][3] -
			m1.m[0][2] * m1.m[1][3] * m1.m[2][1] -
			m1.m[0][3] * m1.m[1][1] * m1.m[2][2] +
			m1.m[0][3] * m1.m[1][2] * m1.m[2][1] +
			m1.m[0][2] * m1.m[1][1] * m1.m[2][3] +
			m1.m[0][1] * m1.m[1][3] * m1.m[2][2]) * deterninantRect;

	result.m[1][0] =
		(-m1.m[1][0] * m1.m[2][2] * m1.m[3][3] -
			m1.m[1][2] * m1.m[2][3] * m1.m[3][0] -
			m1.m[1][3] * m1.m[2][0] * m1.m[3][2] +
			m1.m[1][3] * m1.m[2][2] * m1.m[3][0] +
			m1.m[1][2] * m1.m[2][0] * m1.m[3][3] +
			m1.m[1][0] * m1.m[2][3] * m1.m[3][2]) * deterninantRect;
	result.m[1][1] =
		(m1.m[0][0] * m1.m[2][2] * m1.m[3][3] +
			m1.m[0][2] * m1.m[2][3] * m1.m[3][0] +
			m1.m[0][3] * m1.m[2][0] * m1.m[3][2] -
			m1.m[0][3] * m1.m[2][2] * m1.m[3][0] -
			m1.m[0][2] * m1.m[2][0] * m1.m[3][3] -
			m1.m[0][0] * m1.m[2][3] * m1.m[3][2]) * deterninantRect;
	result.m[1][2] =
		(-m1.m[0][0] * m1.m[1][2] * m1.m[3][3] -
			m1.m[0][2] * m1.m[1][3] * m1.m[3][0] -
			m1.m[0][3] * m1.m[1][0] * m1.m[3][2] +
			m1.m[0][3] * m1.m[1][2] * m1.m[3][0] +
			m1.m[0][2] * m1.m[1][0] * m1.m[3][3] +
			m1.m[0][0] * m1.m[1][3] * m1.m[3][2]) * deterninantRect;
	result.m[1][3] =
		(m1.m[0][0] * m1.m[1][2] * m1.m[2][3] +
			m1.m[0][2] * m1.m[1][3] * m1.m[2][0] +
			m1.m[0][3] * m1.m[1][0] * m1.m[2][2] -
			m1.m[0][3] * m1.m[1][2] * m1.m[2][0] -
			m1.m[0][2] * m1.m[1][0] * m1.m[2][3] -
			m1.m[0][0] * m1.m[1][3] * m1.m[2][2]) * deterninantRect;

	result.m[2][0] =
		(m1.m[1][0] * m1.m[2][1] * m1.m[3][3] +
			m1.m[1][1] * m1.m[2][3] * m1.m[3][0] +
			m1.m[1][3] * m1.m[2][0] * m1.m[3][1] -
			m1.m[1][3] * m1.m[2][1] * m1.m[3][0] -
			m1.m[1][1] * m1.m[2][0] * m1.m[3][3] -
			m1.m[1][0] * m1.m[2][3] * m1.m[3][1]) * deterninantRect;
	result.m[2][1] =
		(-m1.m[0][0] * m1.m[2][1] * m1.m[3][3] -
			m1.m[0][1] * m1.m[2][3] * m1.m[3][0] -
			m1.m[0][3] * m1.m[2][0] * m1.m[3][1] +
			m1.m[0][3] * m1.m[2][1] * m1.m[3][0] +
			m1.m[0][1] * m1.m[2][0] * m1.m[3][3] +
			m1.m[0][0] * m1.m[2][3] * m1.m[3][1]) * deterninantRect;
	result.m[2][2] =
		(m1.m[0][0] * m1.m[1][1] * m1.m[3][3] +
			m1.m[0][1] * m1.m[1][3] * m1.m[3][0] +
			m1.m[0][3] * m1.m[1][0] * m1.m[3][1] -
			m1.m[0][3] * m1.m[1][1] * m1.m[3][0] -
			m1.m[0][1] * m1.m[1][0] * m1.m[3][3] -
			m1.m[0][0] * m1.m[1][3] * m1.m[3][1]) * deterninantRect;
	result.m[2][3] =
		(-m1.m[0][0] * m1.m[1][1] * m1.m[2][3] -
			m1.m[0][1] * m1.m[1][3] * m1.m[2][0] -
			m1.m[0][3] * m1.m[1][0] * m1.m[2][1] +
			m1.m[0][3] * m1.m[1][1] * m1.m[2][0] +
			m1.m[0][1] * m1.m[1][0] * m1.m[2][3] +
			m1.m[0][0] * m1.m[1][3] * m1.m[2][1]) * deterninantRect;

	result.m[3][0] =
		(-m1.m[1][0] * m1.m[2][1] * m1.m[3][2] -
			m1.m[1][1] * m1.m[2][2] * m1.m[3][0] -
			m1.m[1][2] * m1.m[2][0] * m1.m[3][1] +
			m1.m[1][2] * m1.m[2][1] * m1.m[3][0] +
			m1.m[1][1] * m1.m[2][0] * m1.m[3][2] +
			m1.m[1][0] * m1.m[2][2] * m1.m[3][1]) * deterninantRect;
	result.m[3][1] =
		(m1.m[0][0] * m1.m[2][1] * m1.m[3][2] +
			m1.m[0][1] * m1.m[2][2] * m1.m[3][0] +
			m1.m[0][2] * m1.m[2][0] * m1.m[3][1] -
			m1.m[0][2] * m1.m[2][1] * m1.m[3][0] -
			m1.m[0][1] * m1.m[2][0] * m1.m[3][2] -
			m1.m[0][0] * m1.m[2][2] * m1.m[3][1]) * deterninantRect;
	result.m[3][2] =
		(-m1.m[0][0] * m1.m[1][1] * m1.m[3][2] -
			m1.m[0][1] * m1.m[1][2] * m1.m[3][0] -
			m1.m[0][2] * m1.m[1][0] * m1.m[3][1] +
			m1.m[0][2] * m1.m[1][1] * m1.m[3][0] +
			m1.m[0][1] * m1.m[1][0] * m1.m[3][2] +
			m1.m[0][0] * m1.m[1][2] * m1.m[3][1]) * deterninantRect;
	result.m[3][3] =
		(m1.m[0][0] * m1.m[1][1] * m1.m[2][2] +
			m1.m[0][1] * m1.m[1][2] * m1.m[2][0] +
			m1.m[0][2] * m1.m[1][0] * m1.m[2][1] -
			m1.m[0][2] * m1.m[1][1] * m1.m[2][0] -
			m1.m[0][1] * m1.m[1][0] * m1.m[2][2] -
			m1.m[0][0] * m1.m[1][2] * m1.m[2][1]) * deterninantRect;

	return result;
}

Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}

Matrix4x4 MakeIdentity4x4() {
	return{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
}

//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRadio, float nearClip, float farClip) {
	Matrix4x4 result;
	result.m[0][0] = Contangent((1 / aspectRadio), (fovY / 2));
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = Contangent(1, fovY / 2);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = -nearClip * farClip / (farClip - nearClip);
	result.m[3][3] = 0.0f;

	return result;
}

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result;
	result.m[0][0] = 2 / (right - left);
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = 2 / (top - bottom);
	result.m[1][2] = 0;
	result.m[1][3] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1 / (farClip - nearClip);
	result.m[2][3] = 0;
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1;
	return result;
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = {
	width / 2,0,0,0,
	0,-(height / 2),0,0,
	0,0,maxDepth - minDepth,0,
	left + (width / 2),top + (height / 2),minDepth,1
	};
	return result;
}

Matrix4x4& SetTranslate(Matrix4x4& m, const Vector3& v) {
	m.m[3][0] = v.num[0], m.m[3][1] = v.num[1], m.m[3][2] = v.num[2];
	return m;
}

Matrix4x4 MakeInverseMatrix(const Matrix4x4& rotate, const Vector3& translate) {
	Matrix4x4 RT = Transpose(rotate);
	return SetTranslate(RT, -translate * RT);
}

Matrix4x4 MakeRotateMatrixFromOrientations(const Vector3 orientations[3]) {
	return {
		orientations[0].num[0],orientations[0].num[1],orientations[0].num[2],0.0f,
		orientations[1].num[0],orientations[1].num[1],orientations[1].num[2],0.0f,
		orientations[2].num[0],orientations[2].num[1],orientations[2].num[2],0.0f,
		0.0f,0.0f,0.0f,1.0f };
}

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {
	const float cosTheta = std::cos(angle);
	const float sinTheta = std::sin(angle);

	const float minusCosTheta = 1.0f - cosTheta;

	return Matrix4x4{
		std::powf(axis.num[0], 2) * minusCosTheta + cosTheta,axis.num[0] * axis.num[1] * minusCosTheta + axis.num[2] * sinTheta,axis.num[0] * axis.num[2] * minusCosTheta - axis.num[1] * sinTheta,0.0f,
		axis.num[0] * axis.num[1] * minusCosTheta - axis.num[2] * sinTheta,std::powf(axis.num[1], 2) * minusCosTheta + cosTheta,axis.num[1] * axis.num[2] * minusCosTheta + axis.num[0] * sinTheta,0.0f,
		axis.num[0] * axis.num[2] * minusCosTheta + axis.num[1] * sinTheta, axis.num[1] * axis.num[2] * minusCosTheta - axis.num[0] * sinTheta,std::powf(axis.num[2],2) * minusCosTheta + cosTheta,0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, const float cos, const float sin) {

	const float minusCosTheta = 1.0f - cos;

	return Matrix4x4{
		std::powf(axis.num[0], 2) * minusCosTheta + cos,axis.num[0] * axis.num[1] * minusCosTheta + axis.num[2] * sin,axis.num[0] * axis.num[2] * minusCosTheta - axis.num[1] * sin,0.0f,
		axis.num[0] * axis.num[1] * minusCosTheta - axis.num[2] * sin,std::powf(axis.num[1], 2) * minusCosTheta + cos,axis.num[1] * axis.num[2] * minusCosTheta + axis.num[0] * sin,0.0f,
		axis.num[0] * axis.num[2] * minusCosTheta + axis.num[1] * sin, axis.num[1] * axis.num[2] * minusCosTheta - axis.num[0] * sin,std::powf(axis.num[2],2) * minusCosTheta + cos,0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {
	Vector3 u = Normalize(from);
	Vector3 v = Normalize(to);

	const float dot = Dot(u, v);
	const Vector3 cross = Cross(u, v);
	Vector3 axis = Normalize(cross);

	if (dot == -1.0f) {
		if (u.num[0] != 0.0f || u.num[1] != 0.0f) {
			axis = Vector3{ u.num[1], -u.num[0], 0.0f };
		}
		else if (u.num[0] != 0.0f || u.num[2] != 0.0f) {
			axis = Vector3{ u.num[2], 0.f, -u.num[0] };
		}
	}

	return MakeRotateAxisAngle(Normalize(axis), dot, Length(cross));
}

#pragma endregion

#pragma region Quaternion
Quaternion operator+(const Quaternion& q1, const Quaternion& q2) { return { q1.x + q2.x, q1.y + q2.y, q1.z + q2.z,q1.w + q2.w }; }
Quaternion operator-(const Quaternion& q1, const Quaternion& q2) { return { q1.x - q2.x, q1.y - q2.y, q1.z - q2.z,q1.w - q2.w }; }
Quaternion operator*(const float t, const Quaternion& q) { return { q.x * t,q.y * t,q.z * t,q.w * t }; }
Quaternion operator/(const Quaternion& q, const float t) { return { q.x / t,q.y / t,q.z / t,q.w / t }; }

Vector4 MakeQuaternion(Vector3 axis, float radian) {
	Vector4 quaternion;
	//動かす角度の半分のsin,cos
	float halfSin, halfCos;
	float normal;

	quaternion = { 0,0,0,0 };
	//回転軸の長さを求める
	//λ2x+λ2y+λ2z=1方向が重要、ノルムを１に統一
	normal = axis.num[0] * axis.num[0] + axis.num[1] * axis.num[1] + axis.num[2] * axis.num[2];
	if (normal <= 0.0f) return quaternion;

	//方向ベクトルへ（単位ベクトル：長さ1）
	//ノルムは１で固定
	//sqrtfは平方根
	normal = 1.0f / sqrtf(normal);
	axis.num[0] = axis.num[0] * normal;
	axis.num[1] = axis.num[1] * normal;
	axis.num[2] = axis.num[2] * normal;

	//四次元ベクトル (λ.x*sinθ/2,λ.y*sinθ/2,λ.z*sinθ/2,cosθ/2)
	halfSin = sinf(radian * 0.5f);
	halfCos = cosf(radian * 0.5f);

	quaternion.num[0] = axis.num[0] * halfSin;
	quaternion.num[1] = axis.num[1] * halfSin;
	quaternion.num[2] = axis.num[2] * halfSin;
	quaternion.num[3] = halfCos;

	return quaternion;
}

Quaternion MakeRotateAxisAngleQuaternion(Vector3 axis, float Radian) {
	Quaternion quat;
	float halfAngle = Radian * 0.5f;
	float sinHalfAngle = sin(halfAngle);

	quat.w = cos(halfAngle);
	quat.x = axis.num[0] * sinHalfAngle;
	quat.y = axis.num[1] * sinHalfAngle;
	quat.z = axis.num[2] * sinHalfAngle;

	return quat;
}

Vector3 QuaternionToEulerAngles(const Quaternion& quat) {
	Vector3 euler;
	//X軸回転
	euler.num[0] = atan2(2 * (quat.y * quat.z + quat.w * quat.x), quat.w * quat.w - quat.x * quat.x - quat.y * quat.y + quat.z * quat.z);

	//Y軸回転
	euler.num[1] = asin(2 * (quat.x * quat.z - quat.w * quat.y));

	//Z軸の回転
	euler.num[2] = atan2(2 * (quat.x * quat.y + quat.w * quat.z), quat.w * quat.w + quat.x * quat.x - quat.y * quat.y - quat.z * quat.z);

	return euler;
}

Matrix4x4 MakeQuatAffineMatrix(const Vector3& scale, const Matrix4x4& rotate, const Vector3& translate) {
	Matrix4x4 result;

	result.m[0][0] = scale.num[0] * rotate.m[0][0];
	result.m[0][1] = scale.num[0] * rotate.m[0][1];
	result.m[0][2] = scale.num[0] * rotate.m[0][2];
	result.m[0][3] = 0;
	result.m[1][0] = scale.num[0] * rotate.m[1][0];
	result.m[1][1] = scale.num[0] * rotate.m[1][1];
	result.m[1][2] = scale.num[0] * rotate.m[1][2];
	result.m[1][3] = 0;
	result.m[2][0] = scale.num[1] * rotate.m[2][0];
	result.m[2][1] = scale.num[1] * rotate.m[2][1];
	result.m[2][2] = scale.num[1] * rotate.m[2][2];
	result.m[2][3] = 0;
	result.m[3][0] = translate.num[0];
	result.m[3][1] = translate.num[1];
	result.m[3][2] = translate.num[2];
	result.m[3][3] = 1;
	return result;
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quat) {
	Matrix4x4 result;
	float xx = quat.x * quat.x;
	float xy = quat.x * quat.y;
	float xz = quat.x * quat.z;
	float xw = quat.x * quat.w;

	float yy = quat.y * quat.y;
	float yz = quat.y * quat.z;
	float yw = quat.y * quat.w;

	float zz = quat.z * quat.z;
	float zw = quat.z * quat.w;

	result.m[0][0] = 1.0f - 2.0f * (yy + zz);
	result.m[0][1] = 2.0f * (xy + zw);
	result.m[0][2] = 2.0f * (xz - yw);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 2.0f * (xy - zw);
	result.m[1][1] = 1.0f - 2.0f * (xx + zz);
	result.m[1][2] = 2.0f * (yz + xw);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 2.0f * (xz + yw);
	result.m[2][1] = 2.0f * (yz - xw);
	result.m[2][2] = 1.0f - 2.0f * (xx + yy);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Vector3 RotateVector(const Quaternion& q, const Vector3& v) {
	Quaternion result = { v.num[0],v.num[1],v.num[2],0 };

	result = Multiply(q, Multiply(result, Conjugate(q)));
	return { result.x,result.y,result.z };
}

Vector3 RotateVectorAndQuaternion(const Quaternion& q, const Vector3& v) {
	// クォータニオンの回転行列を取得
	Matrix4x4 rotationMatrix = MakeRotateMatrix(q);

	// ベクトルを4次元ベクトルに変換
	float vector4[4] = { v.num[0], v.num[1], v.num[2], 1.0f };

	// ベクトルを回転行列で変換
	float rotatedVector4[4] = { 0.0f };
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			rotatedVector4[i] += rotationMatrix.m[i][j] * vector4[j];
		}
	}

	// 3次元ベクトルに戻す
	Vector3 rotatedVector = { rotatedVector4[0], rotatedVector4[1], rotatedVector4[2] };

	return rotatedVector;
}

Quaternion Multiply(const Quaternion& q1, const Quaternion& q2) {
	Quaternion result;
	result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
	result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	return result;
}

Quaternion IdentityQuaternion() {
	return { 0.0f,0.0f,0.0f,1.0f };
}

Quaternion Conjugate(const Quaternion& q) {
	Quaternion result;
	result = { -q.x,-q.y,-q.z,q.w };
	return result;
}

float Norm(const Quaternion& q) {
	float result;
	result = LengthQuaternion(q);
	return result;
}

Quaternion Normalize(const Quaternion& q) {
	float len = LengthQuaternion(q);
	Quaternion result;
	if (len != 0.0f) {
		result.w = q.w / len;
		result.x = q.x / len;
		result.y = q.y / len;
		result.z = q.z / len;
		return result;
	}
	else {
		return q;
	}
}

Quaternion Inverse(const Quaternion& q) {
	Quaternion result;
	result = Conjugate(q) / std::pow(Norm(q), 2.0f);


	return result;
}

bool CompereQuaternion(const Quaternion& q1, const Quaternion& q2) {
	if (q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w) {
		return true;
	}
	else {
		return false;
	}
}

Quaternion Lerp(float t, const Quaternion& s, const Quaternion& e) {
	Quaternion result;
	Quaternion es = e - s;
	result = s + t * es;
	return result;
}

Quaternion Slerp(float t, const Quaternion& s, const Quaternion& e) {
	Quaternion ns = Normalize(s);
	Quaternion ne = Normalize(e);
	float dot = ns.x * ne.x + ns.y * ne.y + ns.z * ne.z + ns.w * ne.w;
	if (std::abs(dot) > 0.999f) {
		return Lerp(t, ns, ne);
	}
	if (dot < 0.0f) {
		ns = -1 * ns;
		dot *= -1.0f;
	}

	float theta = std::acos(dot);
	float sinTheta = std::sin(theta);
	float t1 = std::sin((1.0f - t) * theta) / sinTheta;
	float t2 = std::sin(t * theta) / sinTheta;

	return (t1 * ns + t2 * ne);
}

#pragma endregion

EulerTransform operator+(const EulerTransform& v1, const EulerTransform& v2) {
	EulerTransform result;

	result.translate = v1.translate + v2.translate;
	result.rotate = v1.rotate + v2.rotate;
	result.scale = v1.scale + v2.scale;

	return result;
}

EulerTransform operator-(const EulerTransform& v1, const EulerTransform& v2) {
	EulerTransform result;

	result.translate = v1.translate - v2.translate;
	result.rotate = v1.rotate - v2.rotate;
	result.scale = v1.scale - v2.scale;

	return result;
}

OBB CreateOBBFromEulerTransform(const EulerTransform& transform) {
	OBB obb;
	obb.center = transform.translate;
	obb.size = transform.scale;

	auto [o1, o2, o3] = ComputeRotationMatrix(transform.rotate);
	obb.orientation[0] = o1;
	obb.orientation[1] = o2;
	obb.orientation[2] = o3;

	return obb;
}

bool IsCollision(const AABB& aabb, const StructSphere& sphere) {
	Vector3 clossestPoint{
		std::clamp(sphere.center.num[0], aabb.min.num[0], aabb.max.num[0]),
		std::clamp(sphere.center.num[1], aabb.min.num[1], aabb.max.num[1]),
		std::clamp(sphere.center.num[2], aabb.min.num[2], aabb.max.num[2])
	};

	float distance = Length(Subtruct(clossestPoint, sphere.center));

	return distance <= sphere.radius;
}

bool IsCollision(const AABB& aabb, const Vector3& point) {
	if ((aabb.min.num[0] <= point.num[0] && point.num[0] <= aabb.max.num[0]) &&
		(aabb.min.num[1] <= point.num[1] && point.num[1] <= aabb.max.num[1]) &&
		(aabb.min.num[2] <= point.num[2] && point.num[1] <= aabb.max.num[2])) {
		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	if ((aabb1.min.num[0] <= aabb2.max.num[0] && aabb1.max.num[0] >= aabb2.min.num[0]) &&
		(aabb1.min.num[1] <= aabb2.max.num[1] && aabb1.max.num[1] >= aabb2.min.num[1]) &&
		(aabb1.min.num[2] <= aabb2.max.num[2] && aabb1.max.num[2] >= aabb2.min.num[2])) {
		return true;
	}
	else {
		return false;
	}
}

bool IsCollision(const AABB& aabb, const Segment& segment) {
	float tXmin = (aabb.min.num[0] - segment.origin.num[0]) / segment.diff.num[0];
	float tXmax = (aabb.max.num[0] - segment.origin.num[0]) / segment.diff.num[0];
	float tYmin = (aabb.min.num[1] - segment.origin.num[1]) / segment.diff.num[1];
	float tYmax = (aabb.max.num[1] - segment.origin.num[1]) / segment.diff.num[1];
	float tZmin = (aabb.min.num[2] - segment.origin.num[2]) / segment.diff.num[2];
	float tZmax = (aabb.max.num[2] - segment.origin.num[2]) / segment.diff.num[2];

	float tNearX = min(tXmin, tXmax);
	float tNearY = min(tYmin, tYmax);
	float tNearZ = min(tZmin, tZmax);

	float tFarX = max(tXmin, tXmax);
	float tFarY = max(tYmin, tYmax);
	float tFarZ = max(tZmin, tZmax);

	float tmin = max(max(tNearX, tNearY), tNearZ);
	float tmax = min(min(tFarX, tFarY), tFarZ);

	if (tmin <= tmax) {
		if (tmin <= 1.0f && tmax >= 0.0f) {
			return true;
		}
	}
	return false;
}

bool IsCollision(const OBB& obb, const StructSphere& sphere) {
	Matrix4x4 obbWorldInverse = MakeInverseMatrix(MakeRotateMatrixFromOrientations(obb.orientation), obb.center);
	Vector3 centerInOBBLocalSpace = sphere.center * obbWorldInverse;
	AABB aabbOBBLocal{ .min = -obb.size, .max = obb.size };
	StructSphere sphereObbLocal{ centerInOBBLocalSpace, sphere.radius };

	return IsCollision(aabbOBBLocal, sphereObbLocal);
}

bool IsCollision(const OBB& obb, const Segment& segment) {
	AABB localAABB{ -1.0f * obb.size,obb.size };
	Matrix4x4 obbWorldInverse = MakeInverseMatrix(MakeRotateMatrixFromOrientations(obb.orientation), obb.center);
	auto localLine = segment;
	localLine.origin = segment.origin * obbWorldInverse;
	Vector3 localLineEnd = (segment.origin + segment.diff) * obbWorldInverse;
	localLine.diff = localLineEnd - localLine.origin;
	return IsCollision(localAABB, localLine);
}

bool IsCollision(const OBB& obb1, const OBB& obb2) {
	// OBBの3つの軸（local X, Y, Z）
	const Vector3& A0 = obb1.orientation[0];
	const Vector3& A1 = obb1.orientation[1];
	const Vector3& A2 = obb1.orientation[2];

	// OBB2の3つの軸（local X, Y, Z）
	const Vector3& B0 = obb2.orientation[0];
	const Vector3& B1 = obb2.orientation[1];
	const Vector3& B2 = obb2.orientation[2];

	// OBB1とOBB2の中心の距離
	Vector3 D = obb2.center - obb1.center;

	// 軸Aと軸Bの交差を計算
	float R[3][3];  // R[i][j]はobb1の軸Aiとobb2の軸Bjのdot product
	float AbsR[3][3];  // 各R[i][j]の絶対値
	float ra, rb, distance;  // 投影されたOBB1とOBB2の半径と軸上の距離

	// 各軸に対する投影を計算
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			R[i][j] = Dot(obb1.orientation[i], obb2.orientation[j]);
			AbsR[i][j] = std::abs(R[i][j]) + 1e-6f;  // 浮動小数点の誤差対策
		}
	}

	// 各軸に対して分離軸定理を適用
	// OBB1の各軸 A0, A1, A2
	for (int i = 0; i < 3; ++i) {
		ra = obb1.size.num[i];
		rb = obb2.size.num[0] * AbsR[i][0] + obb2.size.num[1] * AbsR[i][1] + obb2.size.num[2] * AbsR[i][2];
		distance = std::abs(Dot(D, obb1.orientation[i]));
		if (distance > ra + rb) {
			return false;  // 分離軸が見つかった場合、衝突していない
		}
	}

	// OBB2の各軸 B0, B1, B2
	for (int i = 0; i < 3; ++i) {
		ra = obb1.size.num[0] * AbsR[0][i] + obb1.size.num[1] * AbsR[1][i] + obb1.size.num[2] * AbsR[2][i];
		rb = obb2.size.num[i];
		distance = std::abs(Dot(D, obb2.orientation[i]));
		if (distance > ra + rb) {
			return false;  // 分離軸が見つかった場合、衝突していない
		}
	}

	// OBB1の各軸とOBB2の各軸のクロス積
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			ra = obb1.size.num[(i + 1) % 3] * AbsR[(i + 2) % 3][j] + obb1.size.num[(i + 2) % 3] * AbsR[(i + 1) % 3][j];
			rb = obb2.size.num[(j + 1) % 3] * AbsR[i][(j + 2) % 3] + obb2.size.num[(j + 2) % 3] * AbsR[i][(j + 1) % 3];
			distance = std::abs(Dot(D, Cross(obb1.orientation[i], obb2.orientation[j])));
			if (distance > ra + rb) {
				return false;  // 分離軸が見つかった場合、衝突していない
			}
		}
	}

	// 全ての軸で分離が見つからなければ、衝突している
	return true;
}

bool IsCollision(const StructSphere& sphere1, const StructSphere& sphere2) {
	Vector3 posA = sphere1.center;
	Vector3 posB = sphere2.center;
	float radA = sphere1.radius;
	float radB = sphere2.radius;

	Vector3 Distance = {
		(posB.num[0] - posA.num[0]) * (posB.num[0] - posA.num[0]), (posB.num[1] - posA.num[1]) * (posB.num[1] - posA.num[1]),
		(posB.num[2] - posA.num[2]) * (posB.num[2] - posA.num[2]) };

	if (Distance.num[0] + Distance.num[1] + Distance.num[2] <= (radA + radB) * (radA + radB)) {
		return true;
	}
	else {
		return false;
	}
}