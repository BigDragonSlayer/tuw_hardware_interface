#ifndef TUW_HARDWARE_TRINAMIC_DEFINITIONS_H_
#define TUW_HARDWARE_TRINAMIC_DEFINITIONS_H_

#include <cstdint>

// TODO format properly, in such a way, that it's easier to find stuff

#define DIAMETER 0.108
#define MOTOR_NUMBER 0
#define TYPE_DEFAULT 0
#define TARGET 1
#define VALUE_DEFAULT 0

namespace tmcm1640 {
    enum class tmcm1640_cmd : std::uint8_t {
    // Motion Commands
    ROR     = 1,     // Rotate left
    ROL     = 2,     // Rotate right
    MVP     = 4,     // Move to position
    MST     = 3,     // Motor stop
    // Parameter Commands
    SAP     = 5,     // Set axis parameter
    GAP     = 6,     // Get axis parameter
    STAP    = 7,     // Store axis parameter into EEPROM
    RSAP    = 8,     // Restore axis parameter into EEPROM
    SGP     = 9,     // Set global parameter
    GGP     = 10,    // Get global parameter
    STGP    = 11,    // Store global parameter into EEPROM
    RSGP    = 12,    // Restore global parameter from EEPROM
    // I/O Port Commands
    SIO     = 14,    // Set output
    GIO     = 15,    // Get input
    // Control Commands
    JA      = 22,    // Jump always
    JC      = 21,    // Jump conditional
    COMP    = 20,    // Compare accumulator with constant value
    CSUB    = 23,    // Call subroutine
    RSUB    = 24,    // Return from subroutine
    WAIT    = 27,    // Wait for a specified event
    STOP    = 28,    // End of a TMCL program
    // Calculation Commands
    CALC    = 19,    // Calculate using the accumulator and a constant value
    CALCX   = 33,    // Calculate using the accumulator and the X register
    AAP     = 34,    // Copy accumulator to an axis parameter
    AGP     = 35,    // Copy accumulator to a global parameter
    // I don't need that, but it stays here for completeness
    CMD_Extension   = 26,
    FirmwareVersion = 27
};

    // Caution: only values the TMCM1640's reply will have
    // If the reply itself has a wrong checksum, TMCM1640Communication::check_reply() will return 7
    enum class tmcm1640_status_codes : std::uint8_t {
        OK             = 100,       // Successfully executed, no error
        CMD_LOADED     = 101,       // Command loaded into TMCL program EEPROM
        CHECKSUM_ERROR = 1,         // Wrong checksum
        CMD_ERROR      = 2,         // Invalid command
        TYPE_ERROR     = 3,         // Wrong type
        VALUE_ERROR    = 4,         // Invalid value
        CONFIG_LOCKED  = 5,         // Configuration EEPROM locked
        CMD_NA         = 6          // Command not available
};

    enum class tmcm1640_axis_params : std::uint8_t {
    TARGET_POS         = 0,        // Target position
    ACTUAL_POS         = 1,        // Actual position
    TARGET_VEL         = 2,        // Target speed
    ACTUAL_VEL         = 3,        // Actual speed
    MAX_RAMP_VEL       = 4,        // Max. absolute ramp velocity
    MAX_CURR           = 6,        // Max current
    MVP_REACHED_VEL    = 7,        // MVP Target reached velocity
    MOTOR_HALT_VEL     = 9,        // Motor halted velocity
    MVP_REACHED_DIST   = 10,       // MVP target reached distance
    ACC                = 11,       // Acceleration
    RAMP_GEN_SPEED     = 13,       // Ramp generator speed
    THERM_WIND         = 25,       // Thermal winding time constant
    I_SQ_T_LIMIT       = 26,       // I^2t limit
    I_SQ_T_SUM         = 27,       // I^2t sum
    I_SQ_T_EXC_CNT     = 28,       // I^2t exceed counter
    CLEAR_I_SQ_T       = 29,       // Clear I^2t exceeded flag
    MIN_CNT            = 30,       // Minute counter
    BLDC_RE_INIT       = 31,       // BLDC re-initialization
    PID_REG_DEL        = 133,      // PID regulation loop delay
    CURR_REG_DEL       = 134,      // Current regulation loop delay
    ACTIV_RAMP         = 146,      // Activate ramp
    ACTUAL_CURR        = 150,      // Actual motor current
    ACTUAL_VOLT        = 151,      // Actual voltage
    ACTUAL_TEMP        = 152,      // Actual driver temperature
    TARGET_CURR        = 155,      // Target current
    ERR_FLAG           = 156,      // Error/Status flags
    COMM_MODE          = 159,      // Commutation mode
    ENC_SET_NULL       = 161,      // Encoder set NULL
    SWITCH_SET_NULL    = 162,      // Switch set NULL
    ENC_CLEAR_SET_NULL = 163,      // Encoder clear set NULL
    ACTIV_STOP         = 164,      // Activate stop switch
    ENC_COMM_OFF       = 165,      // Actual encoder commutation offset
    STOP_SW_POL        = 166,      // Stop switch polarity
    CURR_P_PID         = 172,      // P parameter for current PID
    CURR_I_PID         = 173,      // I parameter for current PID
    START_CURR         = 177,      // Start current
    CURR_PID_ERR       = 200,      // Current PID error
    CURR_PID_ERR_SUM   = 201,      // Current PID error sum
    ACTUAL_HALL        = 210,      // Actual hall angle
    ACTUAL_ENC         = 211,      // Actual encoder angle
    ACTUAL_CONTR       = 212,      // Actual controlled angle
    POS_PID_ERR        = 226,      // Position PID error
    VEL_PID_ERR        = 228,      // Velocity PID error
    VEL_PID_ERR_SUM    = 229,      // Velocity PID error sum
    POS_P_PID          = 230,      // P parameter for position PID
    VEL_P_PID          = 234,      // P parameter for velocity PID
    VEL_I_PID          = 235,      // I parameter for velocity PID
    SINE_INIT_SPEED    = 241,      // Sine initialization speed
    INIT_SINE_DELAY    = 244,      // Init sine delay
    OV_PROTECT         = 245,      // Overvoltage protection
    INIT_SINE_MODE     = 249,      // Init sine mode
    ENC_STEPS          = 250,      // Encoder steps
    ENC_DIR            = 251,      // Encoder direction
    NO_MOTOR_POLES     = 253,      // Number of motor poles
    HALL_INV           = 254,      // Hall sensor invert
    EN_DRIVER          = 255       // Enable driver
};

    enum class tmcm1640_cmd_format {
    CMD_TARGET    = 0,     // Module address
    CMD       = 1,     // Command number
    TYPE      = 2,     // Type number
    MOT_BANK  = 3,     // Motor or Bank number
    VALUE_MSB = 4,     // MSB of the value (Value is MSB first!)
    VALUE3    = 4,     // value[3]
    VALUE2    = 5,     // value[2]
    VALUE1    = 6,     // value[1]
    VALUE0    = 7,     // value[0]
    VALUE_LSB = 7,     // LSB of the value (Value is MSB first!)
    CHECKSUM  = 8      // Checksum
    };

    enum class tmcm1640_reply_format {
    REPLY      = 0,    // Reply address
    REPLY_TARGET     = 1,    // Module address
    STATUS     = 2,    // Status
    CMD        = 3,    // Command number
    VALUE_MSB  = 4,    // MSB of the value (Value is MSB first!)
    VALUE3     = 4,    // value[3]
    VALUE2     = 5,    // value[2]
    VALUE1     = 6,    // value[1]
    VALUE0     = 7,    // value[0]
    VALUE_LSB  = 7,    // LSB of the value (Value is MSB first!)
    CHECKSUM   = 8     // Checksum
    };

    //#define BAUDRATE 9600
}

#endif // TUW_HARDWARE_TRINAMIC_DEFINITIONS_H_
