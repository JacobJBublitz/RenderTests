load("@bazel_skylib//rules:run_binary.bzl", "run_binary")

run_binary(
  name = "compile_fixed_triangle_vert",
  outs = ["fixed_triangle.vert.spv"],
  srcs = ["fixed_triangle.vert"],
  tool = "@glslang//:glslangValidator",
  args = ["-V",
  "$(location fixed_triangle.vert)",
  "-o","$(location fixed_triangle.vert.spv)"],
  visibility = ["//visibility:public"],
)

run_binary(
  name = "compile_solid_color_frag",
  outs = ["solid_color.frag.spv"],
  srcs = ["solid_color.frag"],
  tool = "@glslang//:glslangValidator",
  args = ["-V",
  "$(location solid_color.frag)",
  "-o","$(location solid_color.frag.spv)"],
  visibility = ["//visibility:public"],
)
