load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "engine",
    srcs = ["main.cc"],
    deps = [
        "//engine/graphics",
        "//engine/graphics/egl",
        # "//engine/graphics/wayland",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_google_glog//:glog",
        # "@glbinding",
    ],
)
