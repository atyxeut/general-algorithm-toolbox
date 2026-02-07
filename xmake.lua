set_xmakever("3.0.6")

set_project("aty-general-algorithm-toolbox")

if is_plat("windows", "linux") then
    set_toolchains("gcc")
end

if is_plat("windows") then
    add_syslinks("stdc++exp")
end

add_rules("mode.debug", "mode.release")

set_config("builddir", "build")
set_languages("c++latest")

target("gat")
    set_kind("shared")
    add_files("src/**.cppm", {public = true})

target("test")
    set_kind("binary")
    add_deps("gat")
    add_files("test.cpp")
    set_optimize("faster")
