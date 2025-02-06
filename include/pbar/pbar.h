#ifndef PBAR_H
#define PBAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

typedef struct {
#ifdef _WIN32
    long long counter;
#else
    struct timespec ts;
#endif
} pbar_time_point;

typedef enum PBAR_FORMAT {
    PCT = 0,
    ARG,
    TIME,
    PCT_BAR_ARG,
    PCT_TIME,
    PCT_BAR_ARG_TIME,
} PBAR_FORMAT;

typedef struct {
    PBAR_FORMAT format;
    double arg_end;
    double last_arg;
    double last_rate;
    pbar_time_point start_time;
    pbar_time_point last_time;
    int initialized;
} pbar;

void pbar_init(pbar *pb, double arg_end, PBAR_FORMAT format);

void pbar_update(pbar *pb, double arg_now, int stage_num);

void pbar_update_newline(pbar *pb, double arg_now, int stage_num);

double pbar_time_cost(pbar *pb);

#ifdef __cplusplus
}
#endif

#endif  // PBAR_H
