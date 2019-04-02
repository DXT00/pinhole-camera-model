#include "geometry.h"
#include<fstream>
#include<chrono>
#include<math.h>
#include"cow.h"
#include"render3d.h"

void Aperture_Image_RatioFit(float &filmApertureWidth, float &filmApertureHeight,
	const float &imageWidth, const float &imageHeight, const int fitMethod) {

	float filmRatio = filmApertureWidth / filmApertureHeight;
	float imageRatio = imageWidth / imageHeight;
	vec2f scale(1.0, 1.0);
	switch (fitMethod)
	{
	case(FILL):
		if (filmRatio > imageRatio) {
			scale.x = imageRatio / filmRatio;
		}
		else {
			scale.y = filmRatio / imageRatio;

		}
		break;
	case(OVERSCAN):
		if (filmRatio > imageRatio) {
			scale.y = filmRatio / imageRatio;
		}
		else {
			scale.x = imageRatio / filmRatio;
		}

	default:
		break;
	}

	filmApertureWidth *= scale.x;
	filmApertureHeight *= scale.y;

}

void computeCanvasSize(const float focalLength, const float filmApertureWidth,
	const float filmApertureHeight, const float inchToMm,
	const float nearClipingPlane, const float farClipingPlane,
	float &left, float &right, float &top, float &bottom,
	float &angleOfViewHorizontal,
	float &angleOfViewVertical,
	float &filmAspectRatio) {

	float canvasWidth = (filmApertureWidth*inchToMm*nearClipingPlane) / focalLength;
	right = canvasWidth / 2.0;
	left = -right;

	float canvasHeight = (filmApertureHeight*inchToMm*nearClipingPlane) / focalLength;
	top = canvasHeight / 2.0;
	bottom = -top;

	angleOfViewHorizontal = 2 * 180 / PI * atan((filmApertureWidth*inchToMm) / (2.0*focalLength));
	angleOfViewVertical = 2 * 180 / PI * atan((filmApertureHeight*inchToMm) / (2.0*focalLength));

	filmAspectRatio = filmApertureWidth / filmApertureHeight;

	printf("Screen window bottom-left, top-right coordinates ,angleOfViewHorizontal ,angleOfViewVertical %f %f %f %f %f %f\n", bottom, left, top, right, angleOfViewHorizontal, angleOfViewVertical);

}

void compute_Screen_RasterSpace(const float left, const float right,
	const float top, const float bottom, float nearClipingPlane,
	unsigned int imageWidth, unsigned int imageHeight,
	const vec3f pWorld, const Matrix44f worldToCamera, vec2f &pNDC, vec3i &pRaster, bool &visible) {

	vec3f pCamera;
	worldToCamera.multVecMatrix(pWorld, pCamera);

	vec2f pScreen;
	pScreen.x = (pCamera.x / (-pCamera.z))*nearClipingPlane;
	pScreen.y = (pCamera.y / (-pCamera.z))*nearClipingPlane;


	if (pScreen.x > right || pScreen.x<left || pScreen.y>top || pScreen.y < bottom)
		visible = false;

	pNDC.x = 2 * pScreen.x / (right - left) - (right + left) / (right - left);
	pNDC.y = 2 * pScreen.y / (top - bottom) - (top + bottom) / (top - bottom);

	pRaster.x = (pNDC.x + 1) / 2 * imageWidth;
	pRaster.y = (1 - (pNDC.y + 1) / 2)*imageHeight;//NDC y轴与raster y轴相反
	pRaster.z = -pCamera.z;
}

//float edgeFunction() {
//
//}


