#include "swashplateless.hpp"

#include <mathlib/math/Limits.hpp>
#include <mathlib/math/Functions.hpp>

using namespace matrix;

SwashplatelessMixer::SwashplatelessMixer() :
	ModuleParams(nullptr),
	WorkItem(MODULE_NAME, px4::wq_configurations::nav_and_controllers),
	_loop_perf(perf_alloc(PC_ELAPSED, MODULE_NAME": cycle"))
{
	for (int i = 0; i < MAX_SL_MOTOR_NUM; i++) {
		sl_mixer[i] = new SLMixer();
	}
	parameters_updated();
}

SwashplatelessMixer::~SwashplatelessMixer()
{
	for (int i = 0; i < MAX_SL_MOTOR_NUM; i++) {
		delete sl_mixer[i];
	}
	perf_free(_loop_perf);
}

bool
SwashplatelessMixer::init()
{
	if (!_sensor_motor_encoder_sub.registerCallback()) {
		PX4_ERR("sensor_motor_encoder callback registration failed!");
		return false;
	}

	return true;
}

void
SwashplatelessMixer::parameters_updated()
{
	float amp = _param_sl_gain.get();
	for (int i = 0; i < MAX_SL_MOTOR_NUM; i++) {
		sl_mixer[i]->set_amp(amp);
	}
	calibration_offset[0] = _param_calib_0.get();
	calibration_offset[1] = _param_calib_1.get();
	sl_mixer[0]->set_direction(_param_dir_0.get());
	sl_mixer[1]->set_direction(_param_dir_1.get());
	sl_mixer[0]->set_phrase_offset(_param_phase_offset_0.get());
	sl_mixer[1]->set_phrase_offset(_param_phase_offset_1.get());
}

void SwashplatelessMixer::Run()
{
	if (should_exit()) {
		_sensor_motor_encoder_sub.unregisterCallback();
		exit_and_cleanup();
		return;
	}

	perf_begin(_loop_perf);

	// Check if parameters have changed
	if (_parameter_update_sub.updated()) {
		// clear update
		parameter_update_s param_update;
		_parameter_update_sub.copy(&param_update);

		updateParams();
		parameters_updated();
	}

	if (_sensor_motor_encoder_sub.update(&v_motor_enc)) {
		// PX4_INFO("Recv encoder!");
	}

	perf_end(_loop_perf);
}

int SwashplatelessMixer::task_spawn(int argc, char *argv[])
{
	SwashplatelessMixer *instance = new SwashplatelessMixer();

	if (instance) {
		_object.store(instance);
		_task_id = task_id_is_work_queue;

		if (instance->init()) {
			return PX4_OK;
		}

	} else {
		PX4_ERR("alloc failed");
	}

	delete instance;
	_object.store(nullptr);
	_task_id = -1;

	return PX4_ERROR;
}

int SwashplatelessMixer::custom_command(int argc, char *argv[])
{
	return print_usage("unknown command");
}

int SwashplatelessMixer::print_usage(const char *reason)
{
	if (reason) {
		PX4_WARN("%s\n", reason);
	}

	PRINT_MODULE_DESCRIPTION(
		R"DESCR_STR(
### Description
Mixer for swashplateless helicoper

)DESCR_STR");

	PRINT_MODULE_USAGE_NAME("swashplateless_mixer", "controller");
	PRINT_MODULE_USAGE_COMMAND("start");
	PRINT_MODULE_USAGE_DEFAULT_COMMANDS();

	return 0;
}

int swashplateless_mixer_main(int argc, char *argv[])
{
	return SwashplatelessMixer::main(argc, argv);
}
