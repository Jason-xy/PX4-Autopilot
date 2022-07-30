#pragma once

#include <matrix/matrix/math.hpp>
#include <matrix/matrix/Matrix.hpp>

using matrix::Vector3f;
class SLMixer {
	int _direction = 1; //1 clockwise, -1 counterclockwise
	float _amp = 0.2; //amplitude of the sin wave
	float _phrase_offset = 0;
public:
	SLMixer();
	~SLMixer();
	float mix(float calibed_angle, float rpm, const Vector3f & thrust, const Vector3f &torque);
	void set_direction(int direction);
	void set_amp(float amp);
	void set_phrase_offset(float phrase_offset);
};
