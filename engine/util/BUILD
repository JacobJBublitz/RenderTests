load("@rules_cc//cc:defs.bzl", "cc_library", "cc_import")

cc_import(
    name = "dl",
    hdrs = ["dlfcn.h"],
    shared_library = "libdl.so",
    system_provided = True,
)

cc_library(
    name = "library",
    srcs = select({
        "@//platforms:windows": ["library_windows.cc"],
        "//conditions:default": ["library_posix.cc"],
    }),
    deps = select({
        "@//platforms:posix": [":dl"],
        "@//platforms:windows": [],
    }),
    # linkopts = ["-ldl"],
    hdrs = ["library.h"],
)

cc_library(
    name = "util",
    visibility = ["//visibility:public"],
    deps = [":library"],
)
