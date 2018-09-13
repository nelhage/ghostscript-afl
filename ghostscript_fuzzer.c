#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "iapi.h"
#include "gserrors.h"

const size_t BUF_SIZE = (1ul << 20);

int main(int argc, char **argv) {
    void *minst = 0;
    int code = gsapi_new_instance(&minst, NULL);
    if (code < 0) {
        abort();
    }

    code = gsapi_set_arg_encoding(minst, GS_ARG_ENCODING_UTF8);
    if (code != 0) {
        return 0;
    }


    const char *gsargv[] = {
        "gs",
        "-q",
        "-dSAFER",
        "-dNODISPLAY",
        "-dOutputFile=/dev/null",
        "-sstdout=/dev/null",
        "-dBATCH",
        "-dNOPAUSE",
        0,
    };
    code = gsapi_init_with_args(minst, sizeof(gsargv)/sizeof(*gsargv)-1, (char**)gsargv);
    if (code != 0) {
        abort();
    }
    int exit_code;
    gsapi_run_string_begin(minst, 0, &exit_code);
    {
        const char *input = "nulldevice";
        gsapi_run_string_continue(minst, input, strlen(input), 0, &exit_code);
    }
    code = gsapi_run_string_end(minst, 0, &exit_code);
    if (code != 0) {
        abort();
    }

#ifdef __AFL_HAVE_MANUAL_CONTROL
  __AFL_INIT();
#endif

    /* read stdin */
    char *buf = malloc(BUF_SIZE);
    size_t nread = fread(buf, 1, BUF_SIZE, stdin);

    int fd = open("/dev/null", O_RDONLY);
    if (fd < 0) {
        abort();
    }
    dup2(fd, 0);
    close(fd);

    gsapi_run_string_with_length(minst, buf, nread, 0, &exit_code);
    free(buf);

    return 0;
}
