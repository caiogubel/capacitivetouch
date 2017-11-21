/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * File Name : r_touch_api.h
 * Version : 1.00: [2016-06-13:ONR:15:43:05] Released as Version 1.00 to UL.
 *           2.00: []
 * Description : This module provides the interface to the R_TOUCH middle-ware.
 ***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @file
 ***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @defgroup INTERFACE Interface
 * @brief Structures and functions to estimate touch detection.
 * @{
 * This module implements middle-ware to be used with data generated by the CTSU and allows the user to
 * estimate if a sensor is being touched.
 ***********************************************************************************************************************/
#ifndef R_TOUCH_API_H_
#define R_TOUCH_API_H_

#include "r_ctsu_api.h"
/** API revision number (major) */
#define TOUCH_API_VERSION_MAJOR (2)
/** API revision number (minor) */
#define TOUCH_API_VERSION_MINOR (00)

/**
 * Different error status returned by the API
 */
typedef enum e_touch_err
{
    TOUCH_SUCCESS = 0,                      ///< No Errors.
    TOUCH_ERR_INVALID_PARAM = 3,            ///< Received null pointer for required argument.
    TOUCH_ERR_INSUFFICIENT_MEMORY = 9,      ///< No more control blocks available.
    TOUCH_ERR_LOCKED = 8,                   ///< Control block is in use.
    TOUCH_ERR_CTSU_LOCKED = 10,             ///< Attempted operation while CTSU is scanning.
    TOUCH_ERR_INVALID_CMD = 6,              ///< Command requested is invalid.
    TOUCH_ERR_CTSU_BAD_SCAN = 27,           ///< Error in CTSU scan.
    TOUCH_ERR_AUTO_TUNE_FAILED = 18,        ///< Calibration algorithm failed.
} touch_err_t;                              ///< Refer: @ref e_touch_err

/**
 * Event notifications provided through @ref st_touch_callback_arg::event
 */
typedef enum e_touch_cb_event
{
    TOUCH_EVENT_REQUEST_DELAY = 1,                              ///< Handle has been initialized
    TOUCH_EVENT_SCAN_STARTING = CTSU_EVENT_STARTING_SCAN,       ///< Scan about to start.
    TOUCH_EVENT_SCAN_STARTED,                                   ///< Scan has been started in hardware.
    TOUCH_EVENT_SCAN_COMPLETE = CTSU_EVENT_SCAN_COMPLETED,      ///< Scan cycle is complete. This callback is occurring from an ISR. Check argument information for any scan errors.
    TOUCH_EVENT_PARAMETERS_UPDATED = 8,                         ///< All derived parameters are updated. Info contains count of channels touched.
} touch_event_t;                                                ///< Refer: @ref e_touch_cb_event

/**
 * Commands Supported by @ref R_TOUCH_Read
 */
typedef enum e_touch_result
{
    TOUCH_DATA_BINARY,                         ///< Read binary string of touch determination for selected channels only.
    TOUCH_DATA_CTSU_HW_IDENTIFIER,             ///< Access identifier returned by the lower level
    TOUCH_DATA_HANDLE_LOCK,                    ///< Value of the lock on the handle (Read-only)
    TOUCH_DATA_OPERATION_FLAGS,                ///< Access enable/disable operation flags for the update function
    TOUCH_DATA_SENSOR_OFFSET,                  ///< Offset of the sensor when data is output.
    TOUCH_DATA_COMMON_DRIFT_HOLD_COUNTER,      ///< Access counter measuring drift hold time since last channel touched.
    TOUCH_DATA_COMMON_DRIFT_HOLD_INTERVAL,     ///< Access interval for disabling drift compensation for all channels
    TOUCH_DATA_COMMON_CONTINUOUS_ON_COUNTER,   ///< Access counter measuring the amount of time a sensor is touched
    TOUCH_DATA_COMMON_CONTINUOUS_ON_INTERVAL,  ///< Access the interval after which all touched sensors are released

    /* Items related to Touch Detection*/
    TOUCH_DATA_FILTERED_COUNT,              ///< Access sensor count values output from filter for provided channels.
    TOUCH_DATA_DELTA_COUNT,                 ///< Access out the difference between sensor and baseline count to p_dest array for selected channels.
    TOUCH_DATA_BASELINE_COUNT,              ///< Access the current sensor baseline used to determine if a channel is being touched for selected channels.
    TOUCH_DATA_DELAY_TO_TOUCH_COUNTER,      ///< Access value of delay to touch counter
    TOUCH_DATA_DELAY_TO_RELEASE_COUNTER,    ///< Access value of delay to release counter

    /* Items related to Drift Compensation */
    TOUCH_DATA_AVERAGE_INPUT,           ///< Access average input value during drift interval
    TOUCH_DATA_MINIMUM_INPUT,           ///< Access minimum value input during drift interval
    TOUCH_DATA_MAXIMUM_INPUT,           ///< Access maximum value input during drift interval
    TOUCH_DATA_DRIFT_COUNTER,           ///< Access value of counter counting up to drift interval
    TOUCH_DATA_DRIFT_INTERVAL,          ///< Access value of drift interval

    /* Items related to sensitivity adjustment */
    TOUCH_DATA_AVG_SEN_COUNTER_PRI,     ///< Access average value of primary sensor counter
    TOUCH_DATA_AVG_REF_COUNTER_PRI,     ///< Access average value of primary reference counter
    TOUCH_DATA_RECALIBRATION_COUNTER,   ///< Access value of re-calibration counter

    TOUCH_DATA_LAST,   //!< TOUCH_READ_LAST
} touch_data_t;

/**
 * Forward declared TOUCH Control block
 */
typedef void touch_ctrl_t;

/**
 * Touch Sensor Information
 */
typedef struct st_touch_sensor
{
    uint8_t rx;                         ///< Touch Sensor set as receive electrode
    uint8_t tx;                         ///< Touch Sensor set as transmit electrode (0xff := Not used)
} touch_sensor_t;

/**
 * Structure passed to User notification function as callback.
 */
typedef struct st_touch_callback_arg
{
    touch_ctrl_t * handle_num;    ///< Identifier which generated this event.
    uint32_t event;         ///< Event code. (r_touch_event_t)
    uint32_t info;          ///< Error code or additional information
} touch_callback_arg_t;  ///< Refer @ref st_touch_callback_arg

/**
 * Argument passed to @ref R_TOUCH_Read
 */
typedef struct st_touch_results
{
    touch_data_t read_cmd;          ///< Result type to read
    touch_sensor_t const * sensor;  ///< Channels to read when @ref st_touch_results::sensor_count is non-zero
    uint8_t sensor_count;           ///< Number of sensors
    void * p_dest;                  ///< Location where results must be copied
    size_t size;                    ///< Amount of memory available at location
} touch_read_t;                      ///< Refer @ref st_touch_results

/**
 * Argument passed to @ref R_TOUCH_Control
 */
typedef struct st_touch_control
{
    touch_data_t cmd;               ///< Control Command
    void * p_dest;                  ///< Location where results must be copied
    touch_sensor_t sensor;          ///< sensor channel to get/set information
} touch_control_arg_t;                   ///< Refer @ref st_touch_control

/**
 * Parameters that must be specified for each Touch Sensor
 */
typedef struct st_touch_parameter
{
    uint16_t const threshold;   ///< Threshold count of sensor
    uint16_t const hysteresis;  ///< Hysteresis count of sensor
    uint8_t const dt_limit;     ///< Delay to touch limit (Immediate = 0)
    uint8_t const dr_limit;     ///< Delay to release limit (Immediate = 0)

    uint16_t const drift_rate;          ///< # Scans between drift compensation (Drift Comp. Disabled := 0)
    uint8_t const drift_rate_plus;      ///< Change in drift rate with sensor moving towards a touch (+ve rate change disabled = 0)
    uint8_t const drift_rate_minus;     ///< Change in drift rate with sensor moving away from a touch (-ve rate change disabled = 0)

    uint32_t const recalib_delay;       ///< Delay to perform tuning of un-touched sensor (Disabled = 65535)
    uint16_t const recalib_threshold;   ///< Expressed as raw counts beyond which sample will trigger increase in recalib_counter.
} touch_sensor_parameter_t;             ///< Refer @ref st_touch_parameter

/**
 * Memory location in RAM where volatile parameters associated with Touch
 * Detection can be maintained.
 */
typedef struct st_buffer_memory
{
    volatile uint8_t * const p_start;   ///< Pointer to start of memory
    volatile uint8_t * p_end;           ///< Pointer to end of memory
    size_t size;                        ///< Size of memory
} touch_buffer_t;                       ///< Refer @ref st_buffer_memory

/**
 * Custom Actions which can be specified by the user.
 */
typedef struct st_custom_actions
{
    void (*p_filter)(void*);            ///< Pointer to function to use for filtering CTSU data.
    bool (*p_touch_detect)(void*);      ///< Pointer to function to use for Touch Detection.

    void ** const p_filter_instance;    ///< Pointer to array containing addresses of filter control blocks.
    uint16_t num_filter_instances;      ///< Number of filter blocks.
} touch_custom_actions_t;               ///< Refer @ref st_custom_actions

/**
 * Paremeters shared between Touch Sensors specified by the user.
 */
typedef struct st_common_parameter
{
    uint16_t const drift_hold_limit;    ///< Disable drift compensation for N scans after a sensor is touched.
    uint16_t const on_limit;            ///< Limiter for Touch-ON time (Disabled = 65535)
    uint8_t const max_touched_sensors;  ///< Defines # of sensors allowed to be touched simultaneously.
} touch_common_parameter_t;             ///< Refer @ref st_common_parameter

/**
 * Structure of a callback notifying upper layers.
 * @param p_arg - Pointer to arguments being provided by the TOUCH middle ware.
 */
typedef void (*touch_callback_t)(touch_callback_arg_t const * const p_arg);

/**
 * Touch Configuration (provided by user) passed to Open function.
 */
typedef struct st_touch_cfg_t
{
    ctsu_instance_t const * const p_ctsu;        ///< Underlying CTSU H/W configuration.
    touch_common_parameter_t * const p_common;  ///< Parameters common to all sensors
    touch_sensor_parameter_t * const p_sensor;  ///< Pointer to array containing parameters related to touch.
    uint8_t * const p_binary_result;            ///< Pointer to location holding binary results.
    touch_callback_t p_callback;                ///< Pointer to function to use as notification function.
    uint8_t num_ignored;                        ///< Count of sensors to ignore.
    touch_sensor_t const * const p_ignored;     ///< Sensor combinations to ignore.
    touch_buffer_t buffer;                      ///< Buffer for maintain touch-related parameters
    touch_custom_actions_t custom;              ///< User provided actions to replace default actions
} touch_cfg_t;                                  ///< Refer @ref st_touch_cfg_t

/*******************************************************************************
 Application Programming Interface (Global Functions)
 *******************************************************************************/

/** TOUCH Middleware API structure. Functions implemented at the HAL layer will follow this API. */
typedef struct st_touch_api
{
    /** Saves the configuration information into an empty control block.
     * Enables lower level CTSU hardware module.
     * Initializes necessary data in the handles.
     * @par Implemented as
     *  - R_TOUCH_Open()
     *
     * @param[in]  p_ctrl   Pointer to control handle structure
     * @param[in]  p_cfg    Pointer to Touch configuration data and structures
     */
    touch_err_t (* open)(touch_ctrl_t * const p_ctrl, touch_cfg_t const * const p_cfg);

    /** Close the CTSU by ending any scan in progress, disabling interrupts, and removing power to the peripheral and
     * saving configurations according to options.
     * @par Implemented as
     *  - R_TOUCH_Close()
     *
     * @param[in]  p_ctrl   Pointer to control handle structure
     */
    touch_err_t (* close)(touch_ctrl_t * const p_ctrl);

    /** Start off a single CTSU scan.
     * @par Implemented as
     *  - R_TOUCH_Start_Scan()
     *
     * @param[in]  p_ctrl   Pointer to control handle structure
     */
    touch_err_t (* scan)(touch_ctrl_t * const p_ctrl);

    /** Start off a single CTSU scan.
     * @par Implemented as
     *  - R_TOUCH_Update()
     *
     * @param[in]  p_ctrl   Pointer to control handle structure
     */
    touch_err_t (* update)(touch_ctrl_t * const p_ctrl);

    /** Read the results from the CTSU including raw data, binary data and other derived data according to the
     * selected options.
     * @par Implemented as
     *  - R_TOUCH_Read()
     *
     * @param[in]  p_ctrl   Pointer to control handle structure
     * @param[in] p_arg    Pointer to the instance specifying the read command.
     */
    touch_err_t (* read)(touch_ctrl_t * const  p_ctrl, touch_read_t const * const p_arg);

    /**Calibrate the CTSU handle. I.e. Bring sensor count ~~ reference counts.
     * @par Implemented as
     *  - R_TOUCH_Update_Parameters()
     *
     * @param[in]  p_ctrl   Pointer to control handle structure
     */
    touch_err_t (* calibrate)(touch_ctrl_t * const p_ctrl);

    /** Change operating parameters in the specified control handle structure.
     * @par Implemented as
     *  - R_TOUCH_Control()
     *
     * @param[in]  p_ctrl   Pointer to control handle structure
     * @param[in] p_arg     Pointer to the command specify control operation details.
     */
    touch_err_t (* control)(touch_ctrl_t * const p_ctrl, touch_control_arg_t const * const p_arg);

    /** Retrieve the API version.
     * @par Implemented as
     *  - R_TOUCH_VersionGet()
     *
     * @pre This function retrieves the API version.
     * @param[in]  p_version   Pointer to version structure
     */
    ssp_err_t (* versionGet)(ssp_version_t * const p_version);
}touch_api_t;


/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_touch_instance
{
    touch_ctrl_t         * p_ctrl; ///< Pointer to the control structure for this instance
    touch_cfg_t    const * p_cfg;  ///< Pointer to the configuration structure for this instance
    touch_api_t    const * p_api;  ///< Pointer to the API structure for this instance
} touch_instance_t;

#endif /* R_TOUCH_RX_IF_H_ */
/*******************************************************************************************************************//**
 * @} (end defgroup INTERFACE)
 ***********************************************************************************************************************/
