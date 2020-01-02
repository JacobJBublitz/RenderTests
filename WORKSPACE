workspace(name = "engine")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

git_repository(
    name = "com_github_gflags_gflags",
    remote = "https://github.com/gflags/gflags.git",
    tag = "v2.2.2",
)

git_repository(
    name = "com_github_google_glog",
    remote = "https://github.com/google/glog.git",
    tag = "v0.4.0",
)

new_git_repository(
    name = "fmt",
    build_file = "BUILD.fmt",
    remote = "https://github.com/fmtlib/fmt.git",
    tag = "6.1.2",
)

new_git_repository(
    name = "glbinding",
    build_file = "BUILD.glbinding",
    remote = "https://github.com/cginternals/glbinding.git",
    tag = "v3.1.0",
)

http_archive(
    name = "libffi",
    build_file = "BUILD.libffi",
    url = "https://github.com/libffi/libffi/releases/download/v3.3/libffi-3.3.tar.gz",
    strip_prefix = "libffi-3.3",
    patches = ["libffi/x86_64-pc-linux-gnu-config.patch"],
)

new_git_repository(
    name = "tinyxml2",
    build_file = "BUILD.tinyxml2",
    remote = "https://github.com/leethomason/tinyxml2.git",
    tag = "7.1.0",
)

new_git_repository(
    name = "wayland",
    build_file = "BUILD.wayland",
    remote = "https://gitlab.freedesktop.org/wayland/wayland.git",
    tag = "1.17.0",
    patches = ["wayland/fix_includes.patch"],
)

new_git_repository(
    name = "wayland_protocols",
    build_file = "BUILD.wayland_protocols",
    remote = "https://gitlab.freedesktop.org/wayland/wayland-protocols.git",
    tag = "1.18",
)

git_repository(
    name = "rules_foreign_cc",
    commit = "ed3db61",
    remote = "https://github.com/bazelbuild/rules_foreign_cc.git",
)

load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()
