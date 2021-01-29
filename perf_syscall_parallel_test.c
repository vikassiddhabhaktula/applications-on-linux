#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <pthread.h>

long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                int cpu, int group_fd, unsigned long flags)
{
        int ret;

        ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,
                       group_fd, flags);
        return ret;
}

void *change_to_sw_group(void *args) {
        int fd;
        struct perf_event_attr pe;

        memset(&pe, 0, sizeof(struct perf_event_attr));
        pe.type = PERF_TYPE_SOFTWARE;
        pe.size = sizeof(struct perf_event_attr);
        pe.config = PERF_COUNT_SW_PAGE_FAULTS;
        pe.disabled = 1;
        pe.exclude_kernel = 1;
        pe.exclude_hv = 1;

        fd = perf_event_open(&pe, 0, -1, *((int *)args), 0);
}

void *change_to_hw_group(void *args) {
        int fd;
        struct perf_event_attr pe;

        memset(&pe, 0, sizeof(struct perf_event_attr));
        pe.type = PERF_TYPE_HARDWARE;
        pe.size = sizeof(struct perf_event_attr);
        pe.config = PERF_COUNT_HW_INSTRUCTIONS;
        pe.disabled = 1;
        pe.exclude_kernel = 1;
        pe.exclude_hv = 1;

        fd = perf_event_open(&pe, 0, -1, *((int *)args), 0);
}

int main(int argc, char **argv)
{
        struct perf_event_attr pe;
        struct perf_event_attr pe2;
        long long count;
        int fd;
        int fd2;
        pthread_t th[2];
        int new_gr = -1;

        memset(&pe, 0, sizeof(struct perf_event_attr));
        pe.type = PERF_TYPE_SOFTWARE;
        pe.size = sizeof(struct perf_event_attr);
        pe.config = PERF_COUNT_SW_PAGE_FAULTS;
        pe.disabled = 1;
        pe.exclude_kernel = 1;
        pe.exclude_hv = 1;

        memset(&pe2, 0, sizeof(struct perf_event_attr));
        pe2.type = PERF_TYPE_HARDWARE;
        pe2.size = sizeof(struct perf_event_attr);
        pe2.config = PERF_COUNT_HW_INSTRUCTIONS;
        pe2.disabled = 1;
        pe2.exclude_kernel = 1;
        pe2.exclude_hv = 1;

        fork();
        fd = perf_event_open(&pe, 0, -1, -1, 0);
        fork();
        pthread_create(&th[0], NULL, change_to_hw_group, &fd);
        fork();
        pthread_create(&th[1], NULL, change_to_sw_group, &fd);
        fork();
        fd2 = perf_event_open(&pe2, 0, -1, fd, 0);
        fork();

        close(fd);
}
