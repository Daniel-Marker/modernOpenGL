#pragma once
#include "SceneObject.h"
class Artwork :
	public SceneObject
{
public:
	Artwork(SceneObject* _frameToCopy, Transform newFrameTransform, SceneObject* _pictureToCopy, Texture2D* newImage);
	Artwork(SceneObject* _frameToCopy, Transform newFrameTransform, SceneObject* _pictureToCopy, std::string newImagePath);
	~Artwork();
};

