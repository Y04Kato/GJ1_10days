#include "RunTimeCounter.h"

RunTimeCounter* RunTimeCounter::GetInstance()
{
	static RunTimeCounter instance;
	return &instance;
}

void RunTimeCounter::Create()
{
	timerBuf_ = std::make_unique<BufferResource<RunTimeCount>>();
	timerBuf_->CreateResource();
}

void RunTimeCounter::Update()
{
	time_.deltaTime += DeltaTimer(time_.flame);
	if (time_.deltaTime >= max_)
	{
		time_.deltaTime = 0;
	}
	timerBuf_->Map();
	timerBuf_->Setbuffer(time_);
	timerBuf_->UnMap();
}

void RunTimeCounter::CommandCall(uint32_t rootParamIndex)
{
	timerBuf_->CommandCall(rootParamIndex);
}

void RunTimeCounter::ComputeCommandCall(uint32_t rootParamIndex)
{
	timerBuf_->ComputeCommandCall(rootParamIndex);
}

uint32_t RunTimeCounter::DeltaTimer(float& flame)
{
	flame += 1.0f / 60.0f;

	if (flame >= 1.0f)
	{
		flame = 0.0f;
		return 1;
	}

	return 0;
}

