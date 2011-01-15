project "cli"
  language "C++"
  kind     "ConsoleApp"
  files  { "include/*.h", "src/*.cpp" }
  includedirs { "include", "../core/include" }
  links { "core" }

  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
 
  configuration { "Release*" }
    defines { "NDEBUG" }
    