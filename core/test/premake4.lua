project "core_tests"
  language "C++"
  kind     "ConsoleApp"
  files  { "*.cpp"}
  includedirs { "../include", 
                "/opt/local/include/postgresql90/",
                "../../test/include",
                "../../unittest-cpp/UnitTest++/src"
              }
  libdirs {"/opt/local/lib/postgresql90"}
  links { "pq", "core", "crypto", "unittest", "basetest" }
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }

  configuration { "Release*" }
    defines { "NDEBUG" }
