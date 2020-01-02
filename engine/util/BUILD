load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "library",
    srcs = ["library.cc"],
    hdrs = ["library.h"],
)

cc_library(
    name = "util",
    linkopts = ["-ldl"],
    visibility = ["//visibility:public"],
    deps = [":library"],
)
