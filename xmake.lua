local PLUGIN_NAME<const> = "DynamicCollisionAdjustment" -- dll name
local AUTHOR_NAME<const> = "ersh1"
local DESCRIPTION<const> = "Dynamic Collision Adjustment with True Prone System"
local VERSION<const> = "1.1.1"
local LICENSE<const> = "GPL-3.0-or-later"

set_arch("x64")
set_languages("c++23")
set_encodings("utf-8")
set_warnings("allextra")

add_rules("mode.debug", "mode.releasedbg")
set_defaultmode("releasedbg")

set_version(VERSION)
set_license(LICENSE)

includes("extern/CommonLibSSE_NG")
includes("extern/SKSEMenuFramework")
add_requires("xbyak v7.06")
add_requires("simpleini v4.26")

target(PLUGIN_NAME, function()
    set_kind("shared")

    add_deps("commonlibsse-ng")
    add_deps("SKSEMenuFramework")
    add_packages("xbyak")
    add_packages("simpleini")

    add_defines("NOMINMAX")

    add_includedirs("src")

    add_files("src/**.cpp")
    add_headerfiles("src/**.h")

    set_pcxxheader("src/PCH.h")

    add_cxxflags(
        "cl::/EHsc",
        "cl::/sdl",
        "cl::/utf-8",
        "cl::/permissive-",
        "cl::/Zc:preprocessor",
        "cl::/wd4200"
    )

    add_rules("commonlibsse-ng.plugin", {
        name = PLUGIN_NAME,
        author = AUTHOR_NAME,
        description = DESCRIPTION,
    })
end)
