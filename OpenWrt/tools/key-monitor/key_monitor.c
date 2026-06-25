#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <linux/input.h>

#define INPUT_DEVICE      "/dev/input/event1"
#define TARGET_KEY_CODE   148  /* KEY_PROG1 */
#define TARGET_KEY_VAL    1    /* 1 = Press, 0 = Release */
#define MAX_EPOLL_EVENTS  1

static const char *const led_paths[] = {
    "/sys/class/leds/led_blue/brightness",
    "/sys/class/leds/led_green/brightness",
    "/sys/class/leds/led_red/brightness",
    "/sys/class/leds/usr_led1/brightness"
};

#define LED_COUNT (sizeof(led_paths) / sizeof(led_paths[0]))

static int led_fds[LED_COUNT];

static void init_led_fds(void) {
    for (size_t i = 0; i < LED_COUNT; ++i) {
        led_fds[i] = open(led_paths[i], O_WRONLY | O_NONBLOCK);
        if (led_fds[i] < 0) {
            fprintf(stderr, "[WARN] Failed to open %s: %s\n", led_paths[i], strerror(errno));
        }
    }
    printf("[INFO] LED file descriptors cached successfully.\n");
}

static void dismiss_alarm_leds(void) {
    for (size_t i = 0; i < LED_COUNT; ++i) {
        if (led_fds[i] >= 0) {
            lseek(led_fds[i], 0, SEEK_SET);
            if (write(led_fds[i], "0\n", 2) < 0) {
                fprintf(stderr, "[WARN] Write error on LED %zu\n", i);
            }
        }
    }
    printf("[EVENT] Alarm dismissed.\n");
}

int main(void) {
    int input_fd = -1;
    int epoll_fd = -1;
    struct epoll_event ev, events[MAX_EPOLL_EVENTS];
    struct input_event input_ev;

    init_led_fds();

    input_fd = open(INPUT_DEVICE, O_RDONLY | O_NONBLOCK);
    if (input_fd < 0) {
        fprintf(stderr, "[ERROR] Failed to open %s: %s\n", INPUT_DEVICE, strerror(errno));
        return EXIT_FAILURE;
    }

    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        fprintf(stderr, "[ERROR] Epoll creation failed\n");
        return EXIT_FAILURE;
    }

    ev.events = EPOLLIN;
    ev.data.fd = input_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, input_fd, &ev) < 0) {
        fprintf(stderr, "[ERROR] Epoll setup failed\n");
        return EXIT_FAILURE;
    }

    printf("[INFO] System armed. Waiting for KEY_PROG1...\n");

    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1);
        if (nfds < 0) {
            if (errno == EINTR) continue;
            break;
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == input_fd) {
                while (read(input_fd, &input_ev, sizeof(struct input_event)) == sizeof(struct input_event)) {
                    if (input_ev.type == EV_KEY && 
                        input_ev.code == TARGET_KEY_CODE && 
                        input_ev.value == TARGET_KEY_VAL) {
                        
                        /* Lightning fast execution */
                        dismiss_alarm_leds();
                    }
                }
            }
        }
    }

    close(epoll_fd);
    close(input_fd);
    for (size_t i = 0; i < LED_COUNT; ++i) {
        if (led_fds[i] >= 0) close(led_fds[i]);
    }
    
    return EXIT_SUCCESS;
}
