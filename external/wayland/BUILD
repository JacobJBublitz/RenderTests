load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@bazel_skylib//rules:run_binary.bzl", "run_binary")

filegroup(
    name = "base_protocol",
    srcs = ["protocol/wayland.xml"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "scanner",
    srcs = [
        "config.h",
        "src/scanner.c",
    ],
    linkopts = ["-lexpat"],  # TODO: Build expat
    deps = [":wayland-util"],
)

genrule(
    name = "generate_config",
    srcs = [],
    outs = ["config.h"],
    cmd = "touch $(location config.h)",
)

run_binary(
    name = "generate_version",
    srcs = ["src/wayland-version.h.in"],
    outs = ["wayland-version.h"],
    args = [
        "$(location src/wayland-version.h.in)",
        "$(location wayland-version.h)",
    ],
    env = {
        "WAYLAND_VERSION_MAJOR": "1",
        "WAYLAND_VERSION_MINOR": "17",
        "WAYLAND_VERSION_MICRO": "0",
        "WAYLAND_VERSION": "1.17.0",
    },
    tool = "@//tools:header_config",
)

genrule(
    name = "make_wayland_client_protocol",
    srcs = ["protocol/wayland.xml"],
    outs = [
        "wayland-client-protocol.h",
    ],
    cmd = "$(location :scanner) " +
          "client-header " +
          "$(location protocol/wayland.xml) " +
          "$(location wayland-client-protocol.h)",
    tools = [":scanner"],
)

genrule(
    name = "make_wayland_protocol",
    srcs = ["protocol/wayland.xml"],
    outs = [
        "wayland-protocol.c",
    ],
    cmd = "$(location :scanner) " +
          "code " +
          "$(location protocol/wayland.xml) " +
          "$(location wayland-protocol.c)",
    tools = [":scanner"],
)

cc_library(
    name = "wayland-util",
    srcs = [
        "src/wayland-private.h",
        "src/wayland-server-core.h",
        "src/wayland-util.c",
        "wayland-version.h",
    ],
    hdrs = ["src/wayland-util.h"],
)

cc_library(
    name = "wayland-private",
    srcs = [
        "src/connection.c",
        "src/wayland-os.c",
        "src/wayland-os.h",
        "src/wayland-private.h",
    ],
    hdrs = [
        "config.h",
        "src/wayland-client.h",
        "src/wayland-client-core.h",
        "src/wayland-server.h",
        "src/wayland-server-core.h",
        "wayland-version.h",
    ],
    deps = [
        ":wayland-util",
        "@libffi",
    ],
)

cc_library(
    name = "wayland-client",
    srcs = [
        "src/wayland-client.c",
        "wayland-protocol.c",
    ],
    hdrs = ["wayland-client-protocol.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":wayland-private",
        ":wayland-util",
    ],
)
