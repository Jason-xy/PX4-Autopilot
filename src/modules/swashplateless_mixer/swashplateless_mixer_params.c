/**
 * Swashplateless gain
 *
 * Swashplateless gain, i.e..
 *
 * @min 0.0
 * @max 10
 * @decimal 2
 * @increment 0.1
 * @group Swashplate Mixer
 */
PARAM_DEFINE_FLOAT(SL_GAIN, 1.0f);

/**
 * Swashplateless calib 0
 *
 * Swashplateless calib 0, i.e..
 *
 * @min 0.0
 * @max 360.0
 * @decimal 2
 * @increment 0.1
 * @group Swashplate Mixer
 */
PARAM_DEFINE_FLOAT(SL_CALIB_0, 0.0f);


/**
 * Swashplateless calib 1
 *
 * Swashplateless calib 1, i.e..
 *
 * @min 0.0
 * @max 360.0
 * @decimal 2
 * @increment 0.1
 * @group Swashplate Mixer
 */
PARAM_DEFINE_FLOAT(SL_CALIB_1, 0.0f);


/**
 * Motor 0 mix direction
 *
 * Motor 0 mix direction
 *
 * @min -1
 * @max 1
 * @increment 1
 * @group Swashplate Mixer
 */
PARAM_DEFINE_INT32(SL_DIR_0, 1);

/**
 * Motor 1 mix direction
 *
 * Motor 1 mix direction
 *
 * @min -1
 * @max 1
 * @increment 1
 * @group Swashplate Mixer
 */
PARAM_DEFINE_INT32(SL_DIR_1, -1);

/**
 * Swashplateless phase offset 0
 *
 * Swashplateless  phase offset 0, i.e..
 *
 * @min -180
 * @max 180.0
 * @decimal 2
 * @increment 0.1
 * @group Swashplate Mixer
 */
PARAM_DEFINE_FLOAT(SL_PHASE_OFF_0, 0.0f);

/**
 * Swashplateless phase offset 1
 *
 * Swashplateless  phase offset 1, i.e..
 *
 * @min -180
 * @max 180.0
 * @decimal 2
 * @increment 0.1
 * @group Swashplate Mixer
 */
PARAM_DEFINE_FLOAT(SL_PHASE_OFF_1, 0.0f);

/**
 * Swashplateless gain force
 *
 * Swashplateless gain for force, i.e..
 *
 * @min 0.0
 * @max 10
 * @decimal 3
 * @increment 0.001
 * @group Swashplate Mixer
 */
PARAM_DEFINE_FLOAT(SL_GAIN_FORCE, 1.0f);

/**
 * Swashplateless gain torque
 *
 * Swashplateless TPP gain for torque, i.e..
 *
 * @min 0.0
 * @max 10
 * @decimal 3
 * @increment 0.001
 * @group Swashplate Mixer
 */
PARAM_DEFINE_FLOAT(SL_GAIN_TORQUE, 0.1f);

/**
 * Swashplateless position of first motor
 *
 * Swashplateless  position of first motor. + is above
 *
 * @min -100
 * @max 100.0
 * @decimal 2
 * @increment 0.1
 * @group Swashplate Mixer
 */
PARAM_DEFINE_FLOAT(SL_PROP_POS_0, 0.05f);

/**
 * Swashplateless position of second motor
 *
 * Swashplateless  position of second motor. + is above
 *
 * @min -100
 * @max 100.0
 * @decimal 2
 * @increment 0.1
 * @group Swashplate Mixer
 */
PARAM_DEFINE_FLOAT(SL_PROP_POS_1, -0.05f);
