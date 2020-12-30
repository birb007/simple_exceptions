#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <setjmp.h>
#include <signal.h>

#define DECL_EX_VARS \
    siginfo_t *sig_info

#define TRY(blk, signal)                      \
    reset_table();                            \
    if (!sigsetjmp(*get_table_resume(), 1))   \
        blk;                                  \
    get_table_info(&sig_info);                \
    if (get_table_lut() & (1 << signal))

#define F_TRY(blk, signal)  \
    DECL_EX_VARS            \
    TRY(blk, signal)        \

void reset_table(void);
jmp_buf *get_table_resume(void);
int get_table_lut(void);
void get_table_info(siginfo_t **);

#endif /* EXCEPTION_H */
