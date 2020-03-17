load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "vulkan",
    srcs = [
        "device.cc",
        "pipeline.cc",
        "renderer.cc",
    ],
    hdrs = [
        "device.h",
        "pipeline.h",
        "renderer.h",
        "surface.h",
        "render_pass.h",
    ],
    data = [
        "//engine/graphics/vulkan/shaders:fixed_triangle.vert.spv",
        "//engine/graphics/vulkan/shaders:solid_color.frag.spv",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@com_github_google_glog//:glog",
        "@fmt",
        "@glslang",
        "@vulkan_headers",
        "@vulkan_loader",
    ],
)
