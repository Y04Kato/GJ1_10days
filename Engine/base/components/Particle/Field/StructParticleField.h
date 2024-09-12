#pragma once
#include"Particle/GpuParticle.h"

namespace ParticleIbu
{
    namespace FieldType
    {

        struct FieldSuction
        {
            Vector3 translate;
            Vector3 scale;
            Vector3 rotate;

            int32_t use;

            Vector3 sizeMin;
            Vector3 sizeMax;

            float suctionPower;
        };
    }
}