#pragma once
#include"../../../DirectXCommon.h"
#include"../../math/MathCalculation.h"

namespace ParticleIbu
{
	struct  ParticleEmitControl
	{
		bool useFlag_ = false;
		float frequency = 0.0f;
		float frequencyTime = 0.0f;
		float flame = 0.0f;
	};
	namespace EmitType {

		struct BoxParam
		{
			Vector3 translate = {};
			Vector3 rotate = {};
			Matrix4x4 matWorld = MakeIdentity4x4();
			Matrix4x4 matVPV = MakeIdentity4x4();
			uint32_t count = 0;
			uint32_t emit = 0;
			Vector3 sizeMin = { -1.0f,-1.0f,-1.0f };
			Vector3 sizeMax = { 1.0f,1.0f,1.0f };
			Vector3 velocityMin = { 0.0f,0.0f,0.0f };
			Vector3 velocityMax = { 0.0f,0.0f,0.0f };

			Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
			Vector4 colorDecayMin = { 0.0f,0.0f,0.0f,0.01f };
			Vector4 colorDecayMax = { 0.0f,0.0f,0.0f,0.01f };
			Vector3 scaleVelocityMin = { 0.0f,0.0f,0.0f };
			Vector3 scaleVelocityMax = { 0.0f,0.0f,0.0f };
		};
		struct Circle
		{
			Vector3 translate = {};
			Vector3 rotate = {};
			Matrix4x4 matWorld = MakeIdentity4x4();
			Matrix4x4 matVPV = MakeIdentity4x4();
			uint32_t count = 0;
			uint32_t emit = 0;
			float radious = 1.0f;

			Vector3 velocityMin = {};
			Vector3 velocityMax = {};

			Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
			Vector4 colorDecayMin = {0.0f,0.0f,0.0f,0.01f};
			Vector4 colorDecayMax = {0.0f,0.0f,0.0f,0.01f};

			Vector3 scaleVelocityMin = {};
			Vector3 scaleVelocityMax = {};

			Vector3 scaleSizeMin = { 1.0f,1.0f,1.0f };
			Vector3 scaleSizeMax = { 1.0f,1.0f,1.0f };
		};
		struct SphereParam
		{
			Vector3 translate = {};
			Vector3 rotate = {};
			Matrix4x4 matWorld = MakeIdentity4x4();
			Matrix4x4 matVPV = MakeIdentity4x4();
			uint32_t count = 0;
			uint32_t emit = 0;
			float radious = 1.0f;
		};
	}
	namespace System
	{
		namespace StructData
		{
			struct  ParticleVertexData
			{
				Vector4 position;
				Vector2 texcoord;
				Vector3 normal;
				uint32_t instancedId;
			};

		}

	}
}
