#pragma once

#include <glm/glm.hpp>

class Camera;

struct SDL_KeyboardEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseWheelEvent;

class CameraManipulator
{
public:
    CameraManipulator();

    ~CameraManipulator();

	void SetCamera( Camera* _pCamera );
    void Update( float _deltaTime);

	inline void  SetSpeed(glm::vec3 _speed) { m_speed = _speed; }
	inline glm::vec3 GetSpeed() const noexcept { return m_speed; }

	void KeyboardDown(const SDL_KeyboardEvent& key);
	void KeyboardUp(const SDL_KeyboardEvent& key);
	void MouseMove(const SDL_MouseMotionEvent& mouse);
	void MouseWheel(const SDL_MouseWheelEvent& wheel );

private:
    Camera* m_pCamera = nullptr;

	// The u spherical coordinate of the spherical coordinate pair (u,v) denoting the
	// current viewing direction from the view position m_eye. 
	float	m_u = 0.0f;

	// The v spherical coordinate of the spherical coordinate pair (u,v) denoting the
	// current viewing direction from the view position m_eye. 
	float	m_v = 0.0f;

	// The distance of the look at point from the camera. 
	float	m_distance = 0.0f;

	// The point of direction.
	glm::vec3 m_point = glm::vec3( 0.0f );

	// The traversal speed of the camera
	float m_acceleration = 0.1f;
	glm::vec3 m_speed = glm::vec3(0.0);

	// Traveling indicator to different directions.
	float	m_goForward = 0.0f;
	float	m_goRight   = 0.0f;
	float   m_goUp      = 0.0f;
	float	m_break		= 0.0f;
};