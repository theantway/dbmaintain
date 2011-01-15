project "core"
  language "C++"
  kind     "StaticLib"
  files  { "include/*.h", "src/*.cpp" }
  includedirs { "include", "/opt/local/include/postgresql90/" }
 
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
 
  configuration { "Release*" }
    defines { "NDEBUG" }
