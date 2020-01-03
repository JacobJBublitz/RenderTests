workspace(name = "engine")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "bazel_skylib",
    sha256 = "97e70364e9249702246c0e9444bccdc4b847bed1eb03c5a3ece4f83dfe6abc44",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz",
    ],
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")

bazel_skylib_workspace()

git_repository(
    name = "com_github_gflags_gflags",
    remote = "https://github.com/gflags/gflags.git",
    tag = "v2.2.2",
)

git_repository(
    name = "com_github_google_glog",
    commit = "445af7e",
    remote = "https://github.com/google/glog.git",
)

new_git_repository(
    name = "fmt",
    build_file = "fmt/BUILD",
    remote = "https://github.com/fmtlib/fmt.git",
    tag = "6.1.2",
)

http_archive(
    name = "libffi",
    build_file = "libffi/BUILD",
    patches = ["libffi/x86_64-pc-linux-gnu-config.patch"],
    strip_prefix = "libffi-3.3",
    url = "https://github.com/libffi/libffi/releases/download/v3.3/libffi-3.3.tar.gz",
)

git_repository(
    name = "platforms",
    commit = "46993ef",
    remote = "https://github.com/bazelbuild/platforms.git",
)

new_git_repository(
    name = "tinyxml2",
    build_file = "tinyxml2/BUILD",
    remote = "https://github.com/leethomason/tinyxml2.git",
    tag = "7.1.0",
)

new_git_repository(
    name = "wayland",
    build_file = "wayland/BUILD",
    patches = ["wayland/fix_includes.patch"],
    remote = "https://gitlab.freedesktop.org/wayland/wayland.git",
    tag = "1.17.0",
)

new_git_repository(
    name = "wayland_protocols",
    build_file = "wayland_protocols/BUILD",
    remote = "https://gitlab.freedesktop.org/wayland/wayland-protocols.git",
    tag = "1.18",
)
