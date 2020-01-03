load("@bazel_skylib//lib:selects.bzl", "selects")

package(default_visibility = ["//visibility:public"])

config_setting(
    name = "windows",
    constraint_values = [
        "@platforms//os:windows",
    ]
)

config_setting(
    name = "linux",
    constraint_values = [
        "@platforms//os:linux",
    ],
)

selects.config_setting_group(
    name = "posix",
    match_any = [":linux"],
)
