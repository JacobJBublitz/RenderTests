load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_skylib//rules:run_binary.bzl", "run_binary")
load("@bazel_skylib//rules:write_file.bzl", "write_file")

write_file(
    name = "write_loader_cmake_config_h",
    out = "loader_cmake_config.h",
    content = [
        "#define HAVE_SECURE_GETENV 1",
        "#define HAVE___SECURE_GETENV 1",
    ],
)

cc_library(
    name = "vulkan_loader",
    srcs = [
        "loader/cJSON.c",
        "loader/cJSON.h",
        "loader/debug_utils.c",
        "loader/debug_utils.h",
        "loader/extension_manual.c",
        "loader/gpa_helper.h",
        "loader/loader.c",
        "loader/loader.h",
        "loader/murmurhash.c",
        "loader/murmurhash.h",
        "loader/trampoline.c",
        "loader/vk_loader_layer.h",
        "loader/vk_loader_platform.h",
        "loader/wsi.c",
        "loader/wsi.h",
        "loader/dev_ext_trampoline.c",
        "loader/phys_dev_ext.c",
        "loader/generated/vk_dispatch_table_helper.h",
        "loader/generated/vk_layer_dispatch_table.h",
        "loader/generated/vk_loader_extensions.h",
        "loader/generated/vk_object_types.h",
        "loader_cmake_config.h",
        "loader/extension_manual.h",
        "loader/unknown_ext_chain.c",
    ] + select({
        "@//platforms:is_linux": [
        ],
        "@//platforms:is_windows": [
            "loader/adapters.h",
            "loader/dirent_on_windows.c",
            "loader/dxgi_loader.c",
            "loader/dxgi_loader.h",
            "loader/unknown_ext_chain_masm.asm",
        ],
    }),
    includes = [
        "loader",
        "loader/generated",
    ],
    local_defines = select({
        "@//platforms:is_linux": [
            "VK_USE_PLATFORM_XCB_KHR",
            "VK_USE_PLATFORM_XLIB_KHR",
            "VK_USE_PLATFORM_XLIB_XRANDR_EXT",
            "VK_USE_PLATFORM_WAYLAND_KHR",
            "FALLBACK_CONFIG_DIRS=\\\"/etc/xdg\\\"",
            "FALLBACK_DATA_DIRS=\\\"/usr/local/share:/usr/share\\\"",
            "SYSCONFDIR=\\\"\\\"",
            "HAVE_CET_H",
        ],
        "@//platforms:is_windows": [
            "VK_USE_PLATFORM_WIN32_KHR",
            "WIN32_LEAN_AND_MEAN",
            "API_NAME=\"Vulkan\"",
        ],
    }),
    textual_hdrs = [
        "loader/generated/vk_loader_extensions.c",
    ],
    visibility = ["//visibility:public"],
    deps = ["@vulkan_headers"],
)
