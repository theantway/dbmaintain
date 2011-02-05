project "unittest_tests"
  language "C++"
  kind     "ConsoleApp"
  files  {  "*.cpp",
			"../*.cpp",
		    "../Posix/*.cpp",
			}
  includedirs { "..", 
                ".",
              }
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }

  configuration { "Release*" }
    defines { "NDEBUG" }
