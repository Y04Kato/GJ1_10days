#pragma once

enum class PipelineType {
	Standard3D,
	Standard2D,
	Particle,
	Skinning,
	SkyBox,
	VertexAnimationTexture,
	PostProcess,
	Grayscale,
	Vignette,
	Smoothing,
	Gaussian,
	Outline,
	RadialBlur,
	MaskTexture,
	Random,
	LensDistortion,
	Scanlines,
	HSV,
	PARTICLE_Init,
	PARTICLE_UPDATE,
	PARTICLE_EMIT_BOX,
	PARTICLE_DRAW_ADD
};
