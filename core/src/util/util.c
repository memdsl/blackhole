#include <util/util.h>

char *strrpc(char *str, char *str_old, char *str_new) {
    char str_buf[strlen(str)];
    memset(str_buf, 0, sizeof(str_buf));
    for (int i = 0; i < strlen(str); i++) {
        if (!strncmp(str + i, str_old, strlen(str_old))) {
            strcat(str_buf, str_new);
            i += strlen(str_old) - 1;
        }
        else {
            strncat(str_buf, str + i, 1);
        }
    }
    strcpy(str, str_buf);
    return str;
}

IFDEF(CONFIG_TIMER_CLOCK_GETTIME,
      static_assert(CLOCKS_PER_SEC == 1000000, "CLOCKS_PER_SEC != 1000000"));
IFDEF(CONFIG_TIMER_CLOCK_GETTIME,
      static_assert(sizeof(clock_t) == 8, "sizeof(clock_t) != 8"));

static uint64_t boot_time = 0;

static uint64_t getUtilTimerInternal() {
#if defined(CONFIG_TIMER_GETTIMEOFDAY)
    struct timeval now;
    gettimeofday(&now, NULL);
    uint64_t us = now.tv_sec * 1000000 + now.tv_usec;
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_COARSE, &now);
    uint64_t us = now.tv_sec * 1000000 + now.tv_nsec / 1000;
#endif
    return us;
}

uint64_t getUtilTimerData() {
    if (boot_time == 0) boot_time = getUtilTimerInternal();
    uint64_t curr_time = getUtilTimerInternal();
    return curr_time - boot_time;
}
