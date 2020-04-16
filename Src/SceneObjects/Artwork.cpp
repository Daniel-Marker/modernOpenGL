#include "Artwork.h"

Artwork::Artwork(SceneObject* _frameToCopy, Transform newFrameTransform, SceneObject* _pictureToCopy, Texture2D* newImage):
	SceneObject(_frameToCopy->GetShader(), _frameToCopy->GetTexture(), _frameToCopy->GetMesh(), _frameToCopy->GetMaterial(), _frameToCopy->GetCamera(), newFrameTransform, _frameToCopy->GetRectCollider())
{
	_children.clear();

	SceneObject* picture = new SceneObject(_pictureToCopy->GetShader(), newImage, _pictureToCopy->GetMesh(),
		_pictureToCopy->GetMaterial(), _pictureToCopy->GetCamera(), _pictureToCopy->GetTransform(), _pictureToCopy->GetRectCollider());

	AddChild(picture);
}

Artwork::Artwork(SceneObject* _frameToCopy, Transform newFrameTransform, SceneObject* _pictureToCopy, std::string newImagePath):
	SceneObject(_frameToCopy->GetShader(), _frameToCopy->GetTexture(), _frameToCopy->GetMesh(), _frameToCopy->GetMaterial(), _frameToCopy->GetCamera(), newFrameTransform, _frameToCopy->GetRectCollider())
{
	_children.clear();

	Texture2D* newImage = new Texture2D();
	newImage->Load(newImagePath);

	SceneObject* picture = new SceneObject(_pictureToCopy->GetShader(), newImage, _pictureToCopy->GetMesh(),
		_pictureToCopy->GetMaterial(), _pictureToCopy->GetCamera(), _pictureToCopy->GetTransform(), _pictureToCopy->GetRectCollider());

	AddChild(picture);
}

Artwork::~Artwork()
{
}
