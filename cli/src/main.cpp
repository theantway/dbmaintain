#include "DbDeploy.h"

#include <iostream>

#include "SimpleOpt.h"

void usage(){
    cout << "Dbmaintain to update db"<<endl;
}

int main(int argc, char** argv){
    enum { OPT_CLEAR, OPT_CLEAN, OPT_RESET, OPT_HELP, OPT_FOO, OPT_BAR, OPT_HOGE };

    CSimpleOpt::SOption g_rgOptions[] = {
        // ID       TEXT          TYPE
        { OPT_FOO,  "-a",     SO_NONE    }, // "-a"
        { OPT_BAR,  "-b",     SO_NONE    }, // "-b"
        { OPT_HOGE, "-f",     SO_REQ_SEP }, // "-f ARG"
        { OPT_HELP, "-?",     SO_NONE    }, // "-?"
        { OPT_HELP, "--help", SO_NONE    }, // "--help"
        { OPT_CLEAR, "clear", SO_NONE    },
        { OPT_CLEAN, "clean", SO_NONE    },
        { OPT_RESET, "reset", SO_NONE    },
        SO_END_OF_OPTIONS                       // END
    };

    CSimpleOpt args(argc, argv, g_rgOptions);

    while (args.Next()) {
        if (args.LastError() == SO_SUCCESS) {
            if (args.OptionId() == OPT_HELP) {
                usage();
                return 0;
            }

            cout << args.OptionId() << " " << args.OptionText()
                 << (args.OptionArg() ? string(args.OptionArg()) : "") << endl;
        }
        else {
            cout << "Invalid argument: %s\n" << args.OptionText()<<endl;
            return 1;
        }
    }

    for(int i = 0; i< args.FileCount(); i++){
        cout << "arg " << args.File(i) <<endl;
    }

//
//    DbDeploy deploy;
//    deploy.setScriptDirectory("/Users/weixu/projects/msx/mobilestorm/schema/src/main/database/scripts/01_v1.0");
//    deploy.go();
    return 0;
}
