set_xmakever("3.0.6")
set_project("aty-general-algorithm-toolbox")

add_rules("mode.debug", "mode.release")
set_config("builddir", "build")
set_config("plat", "mingw")

set_languages("c++latest")

local my_gcc_sdk_dir
if is_host("windows") then
  -- binary from https://gcc-mcf.lhmouse.com/
  my_gcc_sdk_dir = "C:/ucrt64"
end

local my_clang_sdk_dir
if is_host("windows") then
  -- binary from https://github.com/mstorsjo/llvm-mingw
  my_clang_sdk_dir = "C:/llvm-mingw-ucrt-x86_64"
end

toolchain("my_gcc")
  set_kind("standalone")
  set_sdkdir(my_gcc_sdk_dir)
toolchain_end()

toolchain("my_clang")
  set_kind("standalone")
  set_sdkdir(my_clang_sdk_dir)
  set_toolset("cxx", "clang", "clang++")
  set_toolset("ld", "clang++", "clang")
toolchain_end()
add_cxxflags("my_clang::-stdlib=libc++")

if get_config("toolchain") == "my_clang" then
  set_toolchains("my_clang")
  target("std_modules")
    set_kind("object")
    add_includedirs(my_clang_sdk_dir .. "/include/c++/v1")
    add_files(my_clang_sdk_dir .. "/share/libc++/v1/*.cppm", {public = true})
  target_end()
elseif get_config("toolchain") == "my_gcc" then
  set_toolchains("gcc")
  if is_host("windows") then
    add_syslinks("stdc++exp") -- gcc16 still requires this on Windows
  end
end

target("gat")
  set_kind("object")
  if get_config("toolchain") == "my_clang" then
    add_deps("std_modules")
  end
  add_files("src/**.cppm", {public = true})
target_end()

target("test")
  set_kind("binary")
  add_deps("gat")
  add_files("test.cpp")
  set_optimize("faster")
target_end()
