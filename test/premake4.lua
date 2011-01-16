project "basetest"
  language "C++"
  kind     "StaticLib"
  files  { "include/*.h", "src/*.cpp" }
  includedirs { "include", "../unittest-cpp/UnitTest++/src" }

  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }

  configuration { "Release*" }
    defines { "NDEBUG" }
