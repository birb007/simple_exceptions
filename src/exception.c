#include "exception.h"

#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdio.h>

static struct sig_table_ {
    jmp_buf resume;
    uint_least32_t lut;
    siginfo_t info;
} sig_table;


jmp_buf *get_table_resume(void)
{
    return &sig_table.resume;
}

int get_table_lut(void)
{
    return sig_table.lut;
}

void reset_table(void)
{
    sig_table.lut = 0;
}

void get_table_info(siginfo_t **info)
{
    *info = &sig_table.info;
}

static void handler(int signum, siginfo_t *info, void *ctx)
{
    (void) ctx; /* unused parameter warning */
    sig_table.lut |= 1 << signum;
    memcpy(&sig_table.info, info, sizeof(sig_table.info));
    siglongjmp(sig_table.resume, 1);
}

void __attribute__((constructor)) init(void)
{
    _Bool err = 0;
    struct sigaction sa = {
        .sa_flags = SA_SIGINFO,
        .sa_sigaction = handler
    };
    if (sigfillset(&sa.sa_mask) < 0) {
        abort();
    }
    err |= sigaction(SIGFPE, &sa, NULL) < 0;
    err |= sigaction(SIGILL, &sa, NULL) < 0;
    err |= sigaction(SIGSEGV, &sa, NULL) < 0;
    err |= sigaction(SIGSYS, &sa, NULL) < 0;
    err |= sigaction(SIGTRAP, &sa, NULL) < 0;
    if (err) {
        abort();
    }
}

