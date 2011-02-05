project "unittest"
  language "C++"
  kind     "StaticLib"
  files  {  "UnitTest++/src/*.cpp", 
            "UnitTest++/src/*.h",
            "UnitTest++/src/Posix/*.h",
            "UnitTest++/src/Posix/*.cpp",
         }

  excludes{
	  "UnitTest++/src/tests/Main.cpp"
  }

  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }

  configuration { "Release*" }
    defines { "NDEBUG" }


include "UnitTest++/src/tests"