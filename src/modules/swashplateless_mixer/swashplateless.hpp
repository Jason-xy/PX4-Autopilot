#pragma once

#include <lib/mixer/MixerBase/Mixer.hpp> // Airmode
#include <matrix/matrix/math.hpp>
#include <perf/perf_counter.h>
#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/px4_work_queue/WorkItem.hpp>
#include <uORB/Publication.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/SubscriptionCallback.hpp>
#include <uORB/topics/manual_control_setpoint.h>
#include <uORB/topics/parameter_update.h>
#include <uORB/topics/sensor_motor_encoder.h>
#include <vtol_att_control/vtol_type.h>
#include <lib/ecl/AlphaFilter/AlphaFilter.hpp>
#include "SLMixer.hpp"

#define MAX_SL_MOTOR_NUM 2
using namespace time_literals;

/**
 * SwashplatelessMixer app start / stop handling function
 */
extern "C" __EXPORT int swashplateless_mixer_main(int argc, char *argv[]);


class SwashplatelessMixer : public ModuleBase<SwashplatelessMixer>, public ModuleParams,
	public px4::WorkItem
{
public:
	SwashplatelessMixer();
	~SwashplatelessMixer() override;

	/** @see ModuleBase */
	static int task_spawn(int argc, char *argv[]);

	/** @see ModuleBase */
	static int custom_command(int argc, char *argv[]);

	/** @see ModuleBase */
	static int print_usage(const char *reason = nullptr);

	bool init();
	SLMixer * sl_mixer[MAX_SL_MOTOR_NUM] = {nullptr};
	float calibration_offset[MAX_SL_MOTOR_NUM] = {0};

private:
	void Run() override;

	/**
	 * initialize some vectors/matrices from parameters
	 */
	void		parameters_updated();

	uORB::Subscription _manual_control_setpoint_sub{ORB_ID(manual_control_setpoint)};	/**< manual control setpoint subscription */
	uORB::SubscriptionCallbackWorkItem  _sensor_motor_encoder_sub{this, ORB_ID(sensor_motor_encoder)};	/**< manual control setpoint subscription */
	uORB::SubscriptionInterval _parameter_update_sub{ORB_ID(parameter_update), 1_s};

	sensor_motor_encoder_s v_motor_enc;

	perf_counter_t	_loop_perf;			/**< loop duration performance counter */

	DEFINE_PARAMETERS(
		(ParamFloat<px4::params::SL_GAIN>) _param_sl_gain,
		(ParamFloat<px4::params::SL_CALIB_0>) _param_calib_0,
		(ParamFloat<px4::params::SL_CALIB_1>) _param_calib_1,
		(ParamFloat<px4::params::SL_PHASE_OFF_0>) _param_phase_offset_0,
		(ParamFloat<px4::params::SL_PHASE_OFF_1>) _param_phase_offset_1,
		(ParamInt<px4::params::SL_DIR_0>) _param_dir_0,
		(ParamInt<px4::params::SL_DIR_1>) _param_dir_1
	)
};

