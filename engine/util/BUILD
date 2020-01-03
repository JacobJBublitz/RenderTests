load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "library",
    srcs = select({
        "@//platforms:is_windows": ["library_windows.cc"],
        "@//platforms:is_posix": ["library_posix.cc"],
    }),
    hdrs = ["library.h"],
    linkopts = select({
        "@//platforms:is_posix": ["-ldl"],
        "@//platforms:is_windows": [],
    }),
)

cc_library(
    name = "util",
    visibility = ["//visibility:public"],
    deps = [":library"],
)
