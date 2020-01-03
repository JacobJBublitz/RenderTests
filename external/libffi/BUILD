load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "libffi",
    srcs = [
        "src/closures.c",
        "src/java_raw_api.c",
        "src/prep_cif.c",
        "src/raw_api.c",
        "src/types.c",
        "include/ffi_common.h",
        "include/ffi_cfi.h",
        "x86_64-pc-linux-gnu/fficonfig.h",
        "src/dlmalloc.inl",
    ] + [
        "src/x86/asmnames.h",
        "src/x86/ffi.c",
        "src/x86/ffi64.c",
        "src/x86/ffiw64.c",
        "src/x86/ffitarget.h",
        "src/x86/internal.h",
        "src/x86/internal64.h",
        "src/x86/sysv.S",
        "src/x86/unix64.S",
        "src/x86/win64.S",
    ],
    hdrs = [
        "x86_64-pc-linux-gnu/include/ffi.h",
        "x86_64-pc-linux-gnu/include/ffitarget.h",
    ],
    includes = [
        "include",
        "x86_64-pc-linux-gnu",
        "x86_64-pc-linux-gnu/include",
    ],
    restricted_to = ["@platforms//os:posix"],
    visibility = ["//visibility:public"],
)
