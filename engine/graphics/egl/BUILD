load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "context",
    srcs = ["context.cc"],
    hdrs = ["context.h"],
    deps = [
        ":display",
        ":internal",
        ":surface",
    ],
)

cc_library(
    name = "display",
    srcs = ["display.cc"],
    hdrs = ["display.h"],
    deps = [
        ":internal",
        "@com_github_google_glog//:glog",
    ],
)

cc_library(
    name = "internal",
    srcs = ["internal.cc"],
    hdrs = ["internal.h"],
    deps = ["//engine/util"],
)

cc_library(
    name = "surface",
    srcs = ["surface.cc"],
    hdrs = ["surface.h"],
    deps = [
        ":display",
        ":internal",
    ],
)

cc_library(
    name = "egl",
    visibility = ["//visibility:public"],
    deps = [
        ":context",
        ":display",
        ":surface",
    ],
)
