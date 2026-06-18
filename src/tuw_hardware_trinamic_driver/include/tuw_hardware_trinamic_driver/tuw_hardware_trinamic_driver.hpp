#ifndef TUW_HARDWARE_TRINAMIC_DRIVER_HPP
#define TUW_HARDWARE_TRINAMIC_DRIVER_HPP

namespace tmcm1640 {
    enum class tmcm1640_cmd : std::uint8_t {
    // Motion Commands
    ROR     = 1;     // Rotate left
    ROL     = 2;     // Rotate right
    MVP     = 4;     // Move to position
    MST     = 3;     // Motor stop
    // Parameter Commands
    SAP     = 5;     // Set axis parameter
    GAP     = 6;     // Get axis parameter
    STAP    = 7;     // Store axis parameter into EEPROM
    RSAP    = 8;     // Restore axis parameter into EEPROM
    SGP     = 9;     // Set global parameter
    GGP     = 10;    // Get global parameter
    STGP    = 11;    // Store global parameter into EEPROM
    RSGP    = 12;    // Restore global parameter from EEPROM
    // I/O Port Commands
    SIO     = 14;    // Set output
    GIO     = 15;    // Get input
    // Control Commands
    JA      = 22;    // Jump always
    JC      = 21;    // Jump conditional
    COMP    = 20;    // Compare accumulator with constant value
    CSUB    = 23;    // Call subroutine
    RSUB    = 24;    // Return from subroutine
    WAIT    = 27;    // Wait for a specified event
    STOP    = 28;    // End of a TMCL program
    // Calculation Commands
    CALC    = 19;    // Calculate using the accumulator and a constant value
    CALCX   = 33;    // Calculate using the accumulator and the X register
    AAP     = 34;    // Copy accumulator to an axis parameter
    AGP     = 35;    // Copy accumulator to a global parameter
    // I don't need that, but it stays here for completeness
    CMD_Extension   = 26;
    FirmwareVersion = 27;
};

    enum class tmcm1640_status_codes : std::uint8_t {
        OK             = 100        // Successfully executed, no error
        CMD_LOADED     = 101        // Command loaded into TMCL program EEPROM
        CHECKSUM_ERROR = 1          // Wrong checksum
        CMD_ERROR      = 2          // Invalid command
        TYPE_ERROR     = 3          // Wrong type
        VALUE_ERROR    = 4          // Invalid value
        CONFIG_LOCKED  = 5          // Configuration EEPROM locked
        CMD_NA         = 6          // Command not available
};

    struct CmdFormat {
    };

    struct ReplyFormat {

    };
}

#endif // TUW_HARDWARE_TRINAMIC_DRIVER_H_
