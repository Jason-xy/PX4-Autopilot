#include "SLMixer.hpp"

SLMixer::SLMixer() {

}

SLMixer::~SLMixer() {

}

float SLMixer::mix(float calibed_angle, float rpm, const Vector3f & thrust, const Vector3f & torque) {
//Mixer of swashplateless
	float output = 0;
	output = _amp * sinf(calibed_angle + _phrase_offset);
	return output;
}


void SLMixer::set_direction(int direction) {
	_direction = direction;
}

void SLMixer::set_amp(float amp) {
	_amp = amp;
}
void SLMixer::set_phrase_offset(float phrase_offset) {
	_phrase_offset = phrase_offset;
}
