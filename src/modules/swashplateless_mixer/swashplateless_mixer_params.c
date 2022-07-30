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
PARAM_DEFINE_INT32(SL_DIR_1, 1);

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
