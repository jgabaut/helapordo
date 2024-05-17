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
#define HLPD_RELEASE_DATA_TOTAL HLPD_LATEST_TAG

/**
 * Array with all releases info.
 */
extern HLPD_Release_Data release_data[HLPD_RELEASE_DATA_TOTAL+1];

#define HLPD_Release_Data_Fmt "Release_Data {\n    date : \"%s\",\n    int_date : %" PRId64 ",\n    version : \"%s\",\n    phase : %s,\n    age_years : %" PRId64 ",\n    age_months : %" PRId64 ",\n    age_days : %" PRId64 ",\n    days_since_last : %" PRId64 ",\n    bin_size_KB : %" PRId64 ",\n    total_slocs : %" PRId64 ",\n    logic_slocs : %" PRId64 ",\n    animation_slocs : %" PRId64 ",\n    logic_slocs_relative_delta : %" PRId64 ",\n    invil_version : \"%s\",\n    amboso_version : \"%s\",\n    s4c_slocs : %" PRId64 ",\n    s4c_tag : \"%s\",\n    kls_slocs : %" PRId64 ",\n    kls_tag : \"%s\",\n    s4c_gui_slocs : %" PRId64 ",\n    s4c_gui_tag : \"%s\",\n    lib_slocs : %" PRId64 ",\n    lib_slocs_relative_delta : %" PRId64 ",\n    ratio_of_lib_slocs_to_logic_slocs : %f,\n    time : \"%s\",\n}"

#define HLPD_Release_Data_Arg(rls) (rls)->date, (rls)->int_date, (rls)->version, (rls)->phase, (rls)->age_years, (rls)->age_months, (rls)->age_days, (rls)->days_since_last, (rls)->bin_size_KB, (rls)->total_slocs, (rls)->logic_slocs, (rls)->animation_slocs, (rls)->logic_slocs_relative_delta, (rls)->invil_version, (rls)->amboso_version, (rls)->s4c_slocs, (rls)->s4c_tag, (rls)->kls_slocs, (rls)->kls_tag, (rls)->s4c_gui_slocs, (rls)->s4c_gui_tag, (rls)->lib_slocs, (rls)->lib_slocs_relative_delta, (rls)->ratio_of_lib_slocs_to_logic_slocs, (rls)->time

#endif // HLPD_RELEASE_DATA_H_
