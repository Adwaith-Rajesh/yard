#include <string.h>

#define NOBUILD_IMPLEMENTATION
#include "nobuild.h"

#define CC "gcc"
#define CFLAGS "-Wall", "-Wextra", "-g", "-O3", "-std=c11", "-I./src", "-lcrypto"

#define SRC_DIR "./src"
#define TEST_DIR "./test"
#define BUILD_DIR "./build"
#define OUT_DIR BUILD_DIR "/out"
#define BIN_DIR BUILD_DIR "/bin"
#define TEST_BIN_DIR BUILD_DIR "/test"

#define SERVER_BIN "yard-server"
#define CLIENT_BIN "yard-client"

void build_file(const char *file, const char *filepath) {
    CMD(CC, CFLAGS, "-o", PATH(OUT_DIR, CONCAT(NOEXT(file), ".o")), "-c", filepath);
}

void build_server() {
    // create the object files
    FOREACH_FILE_IN_DIR(f, PATH(SRC_DIR, "server"), {
        if (ENDS_WITH(f, ".c")) {
            build_file(f, PATH("src", "server", f));
        }
    });

    // build the binary
    Cstr_Array cmd_line = cstr_array_make(CC, CFLAGS, "-o", PATH(BIN_DIR, SERVER_BIN), NULL);

    FOREACH_FILE_IN_DIR(f, PATH(SRC_DIR, "server"), {
        if (ENDS_WITH(f, ".c")) {
            cmd_line = cstr_array_append(cmd_line, PATH(OUT_DIR, CONCAT(NOEXT(f), ".o")));
        }
    });

    Cmd cmd = {
        .line = cmd_line,
    };

    INFO("CMD: %s", cmd_show(cmd));
    cmd_run_sync(cmd);
}

void build_client() {
    // create the object files
    FOREACH_FILE_IN_DIR(f, PATH(SRC_DIR, "client"), {
        if (ENDS_WITH(f, ".c")) {
            build_file(f, PATH("src", "client", f));
        }
    });

    // build the binary
    Cstr_Array cmd_line = cstr_array_make(CC, CFLAGS, "-o", PATH(BIN_DIR, CLIENT_BIN), NULL);

    FOREACH_FILE_IN_DIR(f, PATH(SRC_DIR, "client"), {
        if (ENDS_WITH(f, ".c")) {
            cmd_line = cstr_array_append(cmd_line, PATH(OUT_DIR, CONCAT(NOEXT(f), ".o")));
        }
    });

    Cmd cmd = {
        .line = cmd_line,
    };

    INFO("CMD: %s", cmd_show(cmd));
    cmd_run_sync(cmd);
}

void compile_ds() {
    FOREACH_FILE_IN_DIR(f, PATH(SRC_DIR, "core", "ds"), {
        if (ENDS_WITH(f, ".c")) {
            build_file(f, PATH(SRC_DIR, "core", "ds", f));
        }
    });
}

void compile_utils() {
    FOREACH_FILE_IN_DIR(f, PATH(SRC_DIR, "core", "utils"), {
        if (ENDS_WITH(f, ".c")) {
            build_file(f, PATH(SRC_DIR, "core", "utils", f));
        }
    });
}

void make_dirs() {
    MKDIRS(BUILD_DIR);
    MKDIRS(OUT_DIR);
    MKDIRS(BIN_DIR);
    MKDIRS(TEST_BIN_DIR);
}

void build_test() {
    Cstr_Array cmd_line = cstr_array_make(CC, CFLAGS, "-I./test", "-o", PATH(TEST_BIN_DIR, "yard_test"),
                                          PATH(TEST_DIR, "main_test.c"), NULL);

    FOREACH_FILE_IN_DIR(f, OUT_DIR, {
        if (!(ENDS_WITH(f, "client.o") || ENDS_WITH(f, "server.o") || ENDS_WITH(f, ".") || ENDS_WITH(f, ".."))) {
            cmd_line = cstr_array_append(cmd_line, PATH(OUT_DIR, f));
        }
    });

    Cmd cmd = {
        .line = cmd_line,
    };

    INFO("CMD: %s", cmd_show(cmd));
    cmd_run_sync(cmd);
}

void run_test() {
    CMD(TEST_BIN_DIR "/yard_test");
}

int main(int argc, char **argv) {
    GO_REBUILD_URSELF(argc, argv);

    make_dirs();
    compile_utils();
    compile_ds();
    build_server();
    build_client();

    if (argc == 2 && strncmp(argv[1], "test", 4) == 0) {
        build_test();
        run_test();
    }
}
