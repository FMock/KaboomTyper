#pragma once
#ifndef ANIMFRAMEDATA_H
#define ANIMFRAMEDATA_H

struct AnimFrameData{
	inline AnimFrameData(){}
	inline AnimFrameData(int f, int n): startFrame(f), numFrames(n){}
	// The index of the first frame of an animation
	int startFrame;
	// The total number of frames for said animation
	int numFrames;
};

#endif