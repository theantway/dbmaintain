solution "dbmaintain++"
  configurations { "debug", "release" }
  includedirs { "lib/boost" }
  
  output_path = "ide/" .. _ACTION .. "/"
  location (output_path)

  configuration { "debug" }
    targetdir (output_path .. "bin/debug")
    flags {"Symbols"}
    buildoptions { "-g -fno-inline -Wall -O0 -D_UNICODE -DUNICODE -D_LIB" }
 
  configuration { "release" }
    targetdir (output_path .. "bin/release")
 
  if _ACTION == "clean" then
    os.rmdir("ide")
  end

include "core"
include "cli"
include "unittest-cpp"
include "test"