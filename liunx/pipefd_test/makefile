cc := gcc
unnamed_pipe_test: unnamed_pipe_test.c
    $(CC) -o $@ $<
    ./$@ "test"
    rm ./$@
