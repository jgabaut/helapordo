#ifndef HLPD_RELEASE_DATA_H_
#define HLPD_RELEASE_DATA_H_
#include <stdint.h>

/**
 * Holds info for a release.
 */
typedef struct HLPD_Release_Data {
    char* date;
    int64_t int_date;
    char* version;
    char* phase;
    int64_t age_years;
    int64_t age_months;
    int64_t age_days;
    int64_t days_since_last;
    int64_t bin_size_KB;
    int64_t total_slocs;
    int64_t logic_slocs;
    int64_t animation_slocs;
    int64_t logic_slocs_relative_delta;
    char* invil_version;
    char* amboso_version;
    int64_t s4c_slocs;
    char* s4c_tag;
    int64_t kls_slocs;
    char* kls_tag;
    int64_t s4c_gui_slocs;
    char* s4c_gui_tag;
    int64_t lib_slocs;
    int64_t lib_slocs_relative_delta;
    double ratio_of_lib_slocs_to_logic_slocs;
    char* time;
} HLPD_Release_Data;

/**
 * Indexes for each release info.
 */
typedef enum HLPD_Release_Idx {
    HLPD_01_TAG = 0,
    HLPD_02_TAG,
    HLPD_03_TAG,
    HLPD_04_TAG,
    HLPD_05_TAG,
    HLPD_06_TAG,
    HLPD_07_TAG,
    HLPD_08_TAG,
    HLPD_09_TAG,
    HLPD_10_TAG,
    HLPD_11_TAG,
    HLPD_12_TAG,
    HLPD_13_TAG,
    HLPD_14_TAG,
    HLPD_LATEST_TAG,
} HLPD_Release_Idx;

/**
 * Total number of releases info.
 */
#define HLPD_TOTAL_RELEASE_DATA HLPD_LATEST_TAG

/**
 * Array with all releases info.
 */
extern HLPD_Release_Data release_data[HLPD_TOTAL_RELEASE_DATA+1];

#endif // HLPD_RELEASE_DATA_H_
