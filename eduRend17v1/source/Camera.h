//
//  Camera.h
//
//	Basic camera class
//

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "vec\vec.h"
#include "vec\mat.h"

using namespace linalg;

class camera_t
{
public:

	float vfov, aspect;	// Aperture attributes
	float zNear, zFar;	// Clip planes
						// zNear should be >0
						// zFar should depend on the size of the scene
	vec3f position;
	float rotationX = 0;
	float rotationY = 0;
	float rotationZ = 0;

	float angle = 0;				// rad
	float angle_vel = fPI / 2;		// rad/s

	camera_t(
		float vfov,
		float aspect,
		float zNear,
		float zFar):
		vfov(vfov), aspect(aspect), zNear(zNear), zFar(zFar)
	{

	}


	// Move to an absolute position
	//
	void moveTo(const vec3f& p)
	{
		position = p;
	}

	// Move relatively
	//
	void moveLocal(const vec3f& v)
	{
		mat4f M = get_ViewToWorldMatrix();
		vec4f move = M * v.xyz0();
		position += move.xyz();
	}


	void moveUpDown(const float& v)
	{
		position.y += v;
	}

	void rotate(const float vX, const float vY, const float vZ)
	{
		rotationX += vY;
		rotationY += vX;
		rotationZ += vZ;
	}

	mat4f get_ViewToWorldMatrix() const
	{
		mat4f M, R, T;
		R = mat4f::rotation(rotationZ, rotationY, rotationX);
		T = mat4f::translation(position);
		M = T * R;
		return M;
	}

	// Return World-to-View matrix for this camera
	//
	mat4f get_WorldToViewMatrix() const
	{
		// Assuming a camera's position and rotation is defined by matrices T(p) and R,
		// the View-to-World transform is T(p)*R (for a first-person style camera)
		// World-to-View then is the inverse of T*R;
		//	inverse(T(p)*R) = inverse(R)*inverse(T(p)) = transpose(R)*T(-p)
		// Since now there is no rotation, this matrix is simply T(-p)

		mat4f M, R, T;
		R = mat4f::rotation(rotationZ, rotationY, rotationX);
		R.transpose();

		T = mat4f::translation(-position);
		M = R * T;
		return M;
	}

	// Matrix transforming from View space to Clip space
	// In a performance sensitive situation this matrix can be precomputed, as long as
	// all parameters are constant (which typically is the case)
	//
	mat4f get_ProjectionMatrix() const
	{
		return mat4f::projection(vfov, aspect, zNear, zFar);
	}
};

#endif