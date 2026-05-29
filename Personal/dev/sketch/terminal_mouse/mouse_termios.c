#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

static struct termios g_old_termios;
static int g_mouse_enabled = 0;

static void disable_mouse(void)
{
    if (g_mouse_enabled) {
        /* Disable 1002 (button-event tracking) and 1006 (SGR extended mode). */
        write(STDOUT_FILENO, "\x1b[?1002l\x1b[?1006l", 16);
        g_mouse_enabled = 0;
    }
}

static void restore_terminal(void)
{
    disable_mouse();
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_old_termios);
}

static int enable_raw_mode(void)
{
    struct termios raw;

    if (tcgetattr(STDIN_FILENO, &g_old_termios) == -1) {
        return -1;
    }

    raw = g_old_termios;

    /* Minimal non-canonical mode. */
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_oflag &= ~(OPOST);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        return -1;
    }

    return 0;
}

static int enable_mouse(void)
{
    /* 1002 = report drag while button pressed
       1006 = SGR mouse format: CSI < b;x;y M/m */
    char const* seq = "\x1b[?1002h\x1b[?1006h";

    if (write(STDOUT_FILENO, seq, (size_t) strlen(seq)) < 0) {
        return -1;
    }

    g_mouse_enabled = 1;
    return 0;
}

static int read_byte(char* c)
{
    ssize_t n = read(STDIN_FILENO, c, 1);
    if (n <= 0) {
        return 0;
    }
    return 1;
}

/* Parse: ESC [ < b ; x ; y M|m */
static int read_sgr_mouse(int* b, int* x, int* y, char* final)
{
    char buf[128];
    int i = 0;
    char c = 0;

    /* We assume ESC has already been read. */
    if (!read_byte(&c) || c != '[') {
        return 0;
    }
    if (!read_byte(&c) || c != '<') {
        return 0;
    }

    while (i < (int) sizeof(buf) - 1) {
        if (!read_byte(&c)) {
            return 0;
        }

        if (c == 'M' || c == 'm') {
            buf[i] = '\0';
            *final = c;
            break;
        }

        buf[i++] = c;
    }

    if (i == 0 || (*final != 'M' && *final != 'm')) {
        return 0;
    }

    if (sscanf(buf, "%d;%d;%d", b, x, y) != 3) {
        return 0;
    }

    return 1;
}

int main(void)
{
    int dragging = 0;

    int err = enable_raw_mode();
    if (err) {
        perror("tcsetattr");
        return 1;
    }

    atexit(restore_terminal);

    err = enable_mouse();
    if (err) {
        perror("enable_mouse");
        return 1;
    }

    write(
        STDOUT_FILENO,
        "Mouse test.\r\n"
        "Left click + drag inside the terminal.\r\n"
        "Press q to quit.\r\n\r\n",
        79
    );

    for (;;) {
        char c = 0;

        if (!read_byte(&c)) {
            continue;
        }

        if (c == 'q') {
            break;
        }

        if (c == '\x1b') {
            int b = 0;
            int x = 0;
            int y = 0;
            char final = 0;

            if (!read_sgr_mouse(&b, &x, &y, &final)) {
                continue;
            }

            /* In xterm mouse protocols, top-left is 1,1. */
            if (final == 'm') {
                dragging = 0;
                printf("RELEASE  b=%d  x=%d  y=%d\r\n", b, x, y);
                fflush(stdout);
                continue;
            }

            if (b & 32) {
                if (dragging) {
                    printf("DRAG     b=%d  x=%d  y=%d\r\n", b, x, y);
                    fflush(stdout);
                }
                continue;
            }

            if ((b & 3) == 0) {
                dragging = 1;
                printf("PRESS    button=1  x=%d  y=%d\r\n", x, y);
                fflush(stdout);
                continue;
            }

            printf("EVENT    b=%d  x=%d  y=%d\r\n", b, x, y);
            fflush(stdout);
        }
    }

    return 0;
}
