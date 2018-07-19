#ifndef _LEPTON_CONFIG_H
#define _LEPTON_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// Uncomment this define if wanting to exclude extended i2c functions from compilation.
// #define LEPFLIR_EXCLUDE_EXT_I2C_FUNCS   1

// Uncomment this define if wanting to exclude SYS functions from compilation.
// #define LEPFLIR_EXCLUDE_SYS_FUNCS   1

// Uncomment this define if wanting to exclude VID functions from compilation.
// #define LEPFLIR_EXCLUDE_VID_FUNCS   1

// Uncomment this define if wanting to exclude AGC functions from compilation.
// #define LEPFLIR_EXCLUDE_AGC_FUNCS   1

// Uncomment this define if wanting to exclude misc. functions from compilation.
// #define LEPFLIR_EXCLUDE_MISC_FUNCS   1

// Explicitly suppress warning about LEPFLIR_EXCLUDE_MISC_FUNCS
// #define LEPFLIR_EXCLUDE_MISC_FUNCS_SUPPRESS_WARNING 1

// Uncomment this define if wanting to exclude image statistics functions from compilation.
// #define LEPFLIR_EXCLUDE_IMAGE_FUNCS   1

/* Do not use locking instruction in case of single threading */
// #define LEPFLIR_EXCLUDE_LOCKING

// Uncomment this define to enable debug output functions.
#define LEPFLIR_ENABLE_DEBUG_OUTPUT     1

#ifdef __cplusplus
}
#endif
#endif /* _LEPTON_CONFIG_H */
