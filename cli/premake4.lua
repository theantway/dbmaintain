project "cli"
  language "C++"
  kind     "ConsoleApp"
  files  { "include/*.h", "src/*.cpp" }
  includedirs { "include", "../core/include" }
  
  libdirs {"/opt/local/lib/postgresql90"}
  links { "pq", "core", "crypto", "unittest", "basetest" }

  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
 
  configuration { "Release*" }
    defines { "NDEBUG" }
    