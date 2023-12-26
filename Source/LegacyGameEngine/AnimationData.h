#pragma once
#ifndef ANIMATIONDATA_H
#define ANIMATIONDATA_H
#include"AnimFrameData.h"
#include<GL/glew.h>
#include<vector>

struct AnimationData{
	inline AnimationData(){}
	inline AnimationData(std::vector<GLuint> &imgs, std::vector<AnimFrameData> &frmInfo):images(imgs), frameInfo(frmInfo){}
	//Array of images for all the animations
	std::vector<GLuint> images;
	// The fram data for all the different animations
	std::vector<AnimFrameData> frameInfo;
};
#endif