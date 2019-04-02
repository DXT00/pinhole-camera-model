#pragma once
#define PI 3.14159
enum fitMethod
{
	FILL,
	OVERSCAN
};
void Aperture_Image_RatioFit(float &filmApertureWidth, float &filmApertureHeight,
	const float &imageWidth, const float &imageHeight, const int fitMethod);

void computeCanvasSize(const float focalLength, const float filmApertureWidth,
	const float filmApertureHeight, const float inchToMm,
	const float nearClipingPlane, const float farClipingPlane,
	float &left, float &right, float &top, float &bottom,
	float &angleOfViewHorizontal,
	float &angleOfViewVertical,
	float &filmAspectRatio);

void compute_Screen_RasterSpace(const float left, const float right,
	const float top, const float bottom, float nearClipingPlane,
	unsigned int imageWidth, unsigned int imageHeight,
	const vec3f pWorld, const Matrix44f worldToCamera, vec2f &pNDC, vec3i &pRaster, bool &visible);
