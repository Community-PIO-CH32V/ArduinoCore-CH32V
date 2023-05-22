# Copyright 2023-present Maximilian Gerhardt <maximilian.gerhardt@rub.de>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Arduino

Arduino Wiring-based Framework allows writing cross-platform software to
control devices attached to a wide range of Arduino boards to create all
kinds of creative coding, interactive objects, spaces or physical experiences.

http://arduino.cc/en/Reference/HomePage
"""

from os.path import isdir, join

from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()
platform = env.PioPlatform()
board = env.BoardConfig()
chip_series: str = board.get("build.series", "")[0:-1].lower() + "x"

FRAMEWORK_DIR = platform.get_package_dir("framework-arduinoch32v")
assert isdir(FRAMEWORK_DIR)

def get_startup_filename(board):
    extra_flags = str(board.get("build.extra_flags", "")).split(" ")
    class_to_startup = {
        "CH32V20x_D6": "startup_ch32v20x_D6.S",
        "CH32V20x_D8": "startup_ch32v20x_D8.S",
        "CH32V20x_D8W": "startup_ch32v20x_D8W.S",
        "CH32V30x_D8": "startup_ch32v30x_D8.S",
        "CH32V30x_D8C": "startup_ch32v30x_D8C.S"
    }
    startup_file = None
    for k, v in class_to_startup.items():
        if any([f"-D{k}" == flag for flag in extra_flags]):
            startup_file = v
    if startup_file is None:
        chip_name = str(board.get("build.mcu", "")).lower()
        if chip_name.startswith("ch32v0"):
            return "startup_ch32v00x.S"
        elif chip_name.startswith("ch32v1"):
            return "startup_ch32v10x.S"
        elif chip_name.startswith("ch5"):
            return "startup_" + board.get("build.series").lower()[0:len("ch5xx")] + ".S"
    if startup_file is None:
        print("Failed to find startup file for board " + str(board))
        env.Exit(-1)
    return startup_file

machine_flags = [
    "-march=%s" % board.get("build.march"),
    "-mabi=%s" % board.get("build.mabi"),
]

env.Append(
    ASFLAGS=machine_flags,
    ASPPFLAGS=[
        "-x", "assembler-with-cpp"
    ],

    CFLAGS=[
        "-std=gnu11"
    ],

    CCFLAGS=machine_flags + [
        "-Os",
        "-Wall",
        "-Wno-comment",
        "-ffunction-sections",
        "-fdata-sections",
        #"-flto",
        #"-ffat-lto-objects",
        #"-fuse-linker-plugin",
        "-static-libgcc",
    ],

    CXXFLAGS=[
        "-fno-exceptions",
        "-fno-rtti",
        "-fpermissive",
        "-std=gnu++17"
    ],

    LINKFLAGS=machine_flags + [
        "-Os",
        "-Wl,--gc-sections",
        #"-flto",
        #"-fuse-linker-plugin",
        #"-nostdlib",
        #"-fwhole-program",
        "--specs=nosys.specs",
        "--specs=nano.specs",
        "-nostartfiles",
        '-Wl,-Map="%s"' % join("${BUILD_DIR}", "${PROGNAME}.map")
    ],

    CPPDEFINES= [
        "TINYVECTOR",
        "ARDUINO_ARCH_WCH",
        ("ARDUINO", 10808),
        ("CMSIS_STARTUP_FILE", env.StringifyMacro(get_startup_filename(board))),
        "__PIO_CPP_SUPPORT__",
    ],

    LIBS=[ "gcc" ],

    LIBSOURCE_DIRS=[
        join(FRAMEWORK_DIR, "libraries")
    ],

    CPPPATH=[
        join(FRAMEWORK_DIR, "cores", "arduino"),
        # we excplicitly cannot include the API folder here because "String.h" (Arduino)
        # conflicts with "string.h" (GCC toolchain standard header) on case-insensitive
        # filesystems (Windows)
        #join(FRAMEWORK_DIR, "cores", "arduino", "api"),
        join(FRAMEWORK_DIR, "cores", "arduino", "api", "deprecated"),
        join(FRAMEWORK_DIR, "cores", "arduino", "api", "deprecated-avr-comp"),
        join(FRAMEWORK_DIR, "cores", "arduino", "wch"),
        join(FRAMEWORK_DIR, "system", "None-OS-SDK", "Core", chip_series),
        join(FRAMEWORK_DIR, "system", "None-OS-SDK", "Peripheral", chip_series, "inc"),
        join(FRAMEWORK_DIR, "system", "None-OS-SDK", "Peripheral", chip_series, "src"),
        join(FRAMEWORK_DIR, "system", "None-OS-SDK", "Startup"),
        join(FRAMEWORK_DIR, "system", "None-OS-SDK", "System", chip_series),
    ]
)

#
# Target: Build Core Library
#

libs = []

if "build.variant" in board:
    variants_dir = join(
        "$PROJECT_DIR", board.get("build.variants_dir")) if board.get(
            "build.variants_dir", "") else join(FRAMEWORK_DIR, "variants")

    env.Append(
        CPPPATH=[
            join(variants_dir, board.get("build.variant"))
        ],
        LDSCRIPT_PATH=join(variants_dir, board.get("build.variant"), "ldscript.ld"),

    )
    env.BuildSources(
        join("$BUILD_DIR", "FrameworkArduinoVariant"),
        join(variants_dir, board.get("build.variant"))
    )

# We have build this as sources and not as library
# to get linking to work properly (??? not sure why yet)
env.BuildSources(
    join("$BUILD_DIR", "FrameworkArduino"),
    join(FRAMEWORK_DIR, "cores", "arduino"),
    # Hack: including IPAddress.cpp in the build, although unused,
    # blows up firmware size. Neither gc-sections nor LTO can
    # get rid of this :(
    "+<*> -<api/IPAddress.cpp>"
)

env.Prepend(LIBS=libs)
