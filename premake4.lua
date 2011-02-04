solution "dbmaintain++"
  configurations { "debug", "release" }
  includedirs { "lib/boost" }
  
  output_path = "ide/"
  if _ACTION ~= nil then 
  	output_path = "ide/" .. _ACTION .. "/"
  end
  
  location (output_path)

  links { "stdc++"}
  configuration { "debug" }
    targetdir (output_path .. "bin/debug")
    flags {"Symbols"}
    buildoptions { "-fno-inline -Wall -O0 -D_UNICODE -DUNICODE -D_LIB" }
 
  configuration { "release" }
    targetdir (output_path .. "bin/release")
 
  if _ACTION == "clean" then
    os.rmdir("ide")
  end

include "core"
include "cli"
include "unittest-cpp"
include "test"