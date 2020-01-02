load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "core",
    srcs = [
        "core.cc",
        "util.cc",
    ],
    hdrs = [
        "core.h",
        "util.h",
    ],
    deps = ["@wayland//:wayland-client"],
)

cc_library(
    name = "egl",
    srcs = ["egl.cc"],
    hdrs = ["egl.h"],
    deps = [
        ":base",
        ":core",
    ],
)

genrule(
    name = "make_wayland_base_protocol",
    srcs = ["@wayland//:base_protocol"],
    outs = [
        "wayland.h",
        "wayland.cc",
    ],
    cmd = "./$(location //tools/wayland-protocol-generator) --cxx_header=$(location wayland.h) --cxx_source=$(location wayland.cc) $(location @wayland//:base_protocol)",
    tools = ["//tools/wayland-protocol-generator"],
)

cc_library(
    name = "base",
    srcs = ["wayland.cc"],
    hdrs = ["wayland.h"],
    deps = [":core"],
)

genrule(
    name = "make_wayland_xdg_decoration_protocol",
    srcs = [
        "@wayland_protocols//:xdg_decoration_unstable_v1",
        "@wayland_protocols//:xdg_shell",
        "@wayland//:base_protocol",
    ],
    outs = [
        "xdg_decoration_unstable_v1.h",
        "xdg_decoration_unstable_v1.cc",
    ],
    cmd = "./$(location //tools/wayland-protocol-generator) " +
          "--cxx_header=$(location xdg_decoration_unstable_v1.h) " +
          "--cxx_source=$(location xdg_decoration_unstable_v1.cc) " +
          "$(location @wayland_protocols//:xdg_decoration_unstable_v1) " +
          "$(location @wayland_protocols//:xdg_shell) " +
          "$(location @wayland//:base_protocol) ",
    tools = ["//tools/wayland-protocol-generator"],
)

cc_library(
    name = "xdg_decoration_unstable_v1",
    srcs = ["xdg_decoration_unstable_v1.cc"],
    hdrs = ["xdg_decoration_unstable_v1.h"],
    deps = [
        ":base",
        ":core",
        ":xdg_shell",
    ],
)

genrule(
    name = "make_wayland_xdg_shell_protocol",
    srcs = [
        "@wayland_protocols//:xdg_shell",
        "@wayland//:base_protocol",
    ],
    outs = [
        "xdg_shell.h",
        "xdg_shell.cc",
    ],
    cmd = "./$(location //tools/wayland-protocol-generator) " +
          "--cxx_header=$(location xdg_shell.h) " +
          "--cxx_source=$(location xdg_shell.cc) " +
          "$(location @wayland_protocols//:xdg_shell) " +
          "$(location @wayland//:base_protocol) ",
    tools = ["//tools/wayland-protocol-generator"],
)

cc_library(
    name = "xdg_shell",
    srcs = ["xdg_shell.cc"],
    hdrs = ["xdg_shell.h"],
    deps = [
        ":base",
        ":core",
    ],
)

cc_library(
    name = "internal",
    visibility = ["//visibility:public"],
    deps = [
        ":base",
        ":core",
        ":egl",
        ":xdg_decoration_unstable_v1",
    ],
)
