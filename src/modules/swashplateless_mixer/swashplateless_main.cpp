#include "swashplateless.hpp"

#include <mathlib/math/Limits.hpp>
#include <mathlib/math/Functions.hpp>

using namespace matrix;

SwashplatelessMixer::SwashplatelessMixer() :
	ModuleParams(nullptr),
	WorkItem(MODULE_NAME, px4::wq_configurations::nav_and_controllers),
	_loop_perf(perf_alloc(PC_ELAPSED, MODULE_NAME": cycle"))
{
	parameters_updated();
}

SwashplatelessMixer::~SwashplatelessMixer()
{
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

int swashplateless_main(int argc, char *argv[])
{
	return SwashplatelessMixer::main(argc, argv);
}
