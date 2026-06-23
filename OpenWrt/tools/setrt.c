#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdint.h>

struct sched_attr {
    uint32_t size;
    uint32_t sched_policy;
    uint64_t sched_flags;
    int32_t  sched_nice;
    uint32_t sched_priority;
    uint64_t sched_runtime;
    uint64_t sched_deadline;
    uint64_t sched_period;
};

#ifndef __NR_sched_setattr
#define __NR_sched_setattr 314
#endif

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <PID> <PRIORITY(1-99)>\n", argv[0]);
        return 1;
    }

    int pid = atoi(argv[1]);
    int priority = atoi(argv[2]);

    struct sched_attr attr;
    __builtin_memset(&attr, 0, sizeof(attr));
    attr.size = sizeof(attr);
    attr.sched_policy = 1; // 1 = SCHED_FIFO
    attr.sched_priority = priority;

    if (syscall(__NR_sched_setattr, pid, &attr, 0) == -1) {
        perror("System call sched_setattr failed");
        return 1;
    }

    printf("Successfully set PID %d to maximum RT via syscall!\n", pid);
    return 0;
}

