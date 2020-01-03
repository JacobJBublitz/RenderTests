load("@bazel_skylib//lib:selects.bzl", "selects")

package(default_visibility = ["//visibility:public"])

config_setting(
    name = "is_windows",
    constraint_values = [
        "@platforms//os:windows",
    ],
)

config_setting(
    name = "is_linux",
    constraint_values = [
        "@platforms//os:linux",
    ],
)

selects.config_setting_group(
    name = "is_posix",
    match_any = [":is_linux"],
)
