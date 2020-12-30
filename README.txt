Simple exception handling - published for convenience of access. To include into your application, include `include/exception.h` then link against `src/exception.c`. If you want to change what exceptions are caught, modify `src/exception.c` to enable/disable exceptions.

Example:

/* example.c */
#include <stdio.h>
#include "exception.h"

int main() {
    F_TRY({asm("rdmsr");}, SIGSEGV) {
        puts("SIGSEGV");
    } else {
        puts("Nothing");
    }
}

$ gcc example.c src/exception.c -I include/exception.h -o example
$ ./example
SIGSEGV

F_TRY and TRY introduce an exception context variable `siginfo_t sig_info` which can be used for inspecting exception information (there is no exposure of signal context). If you are using multiple exception handlers in the same scope, do:

void foo(void)
{
    DECL_EX_VARS;
    TRY({...}, <signal>) {
        /* occured: sig_info defined */
    else
        /* did not occur */

    TRY({...}, <signal>) {
        ...
    ...
}

or

void foo(void)
{
    F_TRY({...}, <signal>)
        ...

    TRY({...}, <signal>)
        ...
}

If you find this inflexible and bad, then write your own; this is a trivial library for proof of concepts.
