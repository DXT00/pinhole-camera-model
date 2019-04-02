#include"geometry.h"
#include<iostream>
#include<stdio.h>
#include<chrono>
#include"render3d.h"
#include"boat.h"
#include<fstream>
using namespace std;



const Matrix44f worldToCamera = { 0.707107, -0.331295, 0.624695, 0, 0, 0.883452, 0.468521, 0, -0.707107, -0.331295, 0.624695, 0, -1.63871, -5.747777, -40.400412, 1 };
const unsigned int imageWidth = 512, imageHeight = 512;
const float focalLength = 35; //mm

float filmApertureWidth = 0.825;
float filmApertureHeight = 0.446;
static const float inchToMm = 25.4;
float nearClippingPlane = 0.1;
float farClippingPlane = 1000;
float angleOfViewHorizontal = 0;
float angleOfViewVertical = 0;
float filmAspectRatio;
fitMethod fit = OVERSCAN;





int main() {
	ofstream ofs;
	ofs.open("./pinhole.svg");
	ofs << "<svg version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" width=\"" << imageWidth << "\" height=\"" << imageHeight << "\">" << std::endl;

	Matrix44f cameraToWorld = worldToCamera.inverse();
	float t, b, l, r;
	Aperture_Image_RatioFit(filmApertureWidth, filmApertureHeight, imageWidth, imageHeight, fit);
	computeCanvasSize(focalLength, filmApertureWidth, filmApertureHeight, inchToMm, nearClippingPlane, farClippingPlane, l, r, t, b, angleOfViewHorizontal, angleOfViewVertical, filmAspectRatio);
	bool visible = true;
	for (int i = 0; i < numTris; i++)
	{
		visible = true;
		vec3f pWorld0 = verts[tris[i * 3]];
		vec3f pWorld1 = verts[tris[i * 3 + 1]];
		vec3f pWorld2 = verts[tris[i * 3 + 2]];
		vec2f pNDC0,pNDC1,pNDC2;
		vec2i pRaster0,pRaster1,pRaster2;
		compute_Screen_RasterSpace(l, r, t, b, nearClippingPlane, imageWidth, imageHeight, pWorld0, worldToCamera, pNDC0, pRaster0,visible);
		compute_Screen_RasterSpace(l, r, t, b, nearClippingPlane, imageWidth, imageHeight, pWorld1, worldToCamera, pNDC1, pRaster1, visible);
		compute_Screen_RasterSpace(l, r, t, b, nearClippingPlane, imageWidth, imageHeight, pWorld2, worldToCamera, pNDC2, pRaster2, visible);
		
		int val = visible ? 100 : 255;
		ofs << "<line x1=\"" << pRaster0.x << "\" y1=\"" << pRaster0.y << "\" x2=\"" << pRaster1.x << "\" y2=\"" << pRaster1.y << "\" style=\"stroke:rgb(" << val << ",0,0);stroke-width:1\" />\n";
		ofs << "<line x1=\"" << pRaster1.x << "\" y1=\"" << pRaster1.y << "\" x2=\"" << pRaster2.x << "\" y2=\"" << pRaster2.y << "\" style=\"stroke:rgb(" << val << ",0,0);stroke-width:1\" />\n";
		ofs << "<line x1=\"" << pRaster2.x << "\" y1=\"" << pRaster2.y << "\" x2=\"" << pRaster0.x << "\" y2=\"" << pRaster0.y << "\" style=\"stroke:rgb(" << val << ",0,0);stroke-width:1\" />\n";

	}	
	

	ofs << "</svg>\n";
	ofs.close();




	system("pause");
	return 0;
}