#pragma once

#define RLIMIT_CORE 1

struct rlimit {
     unsigned long   rlim_cur;
     unsigned long   rlim_max;
};

int getrlimit(int resource, struct rlimit *rlim);
int setrlimit(int resource, const struct rlimit *rlim);