#pragma once
const int cRefreshRate = 16;
const int cInitWindowWidth = 1920;
const int cInitWindowHeight = 1080;
const int cInitWindowPositionX = 0;
const int cInitWindowPositionY = 0;

const float cFOV = 45.0f;
const float cNearClippingPlaneDist = 0.1f;
const float cFarClippingPlaneDist = 1000.0f;
const float cCameraMoveSpeed = 5.0f;
const float cCameraMax = 45.0f;
const float cCameraMin = -45.0f;

const float cMouseSensitivity = 1.0f;

const float cUISpaceRight = 1.0f;
const float cUISpaceTop = 1.0f;
const float cTextSpacingMultiplier = 500.0f;
const float cTextUVOffset = 1.0f / 50.0f;

#define MAX_LIGHTS 2