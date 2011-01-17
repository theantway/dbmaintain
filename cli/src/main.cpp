#include "DbDeploy.h"

#include <iostream>

#include "SimpleOpt.h"

void usage(){
    cout << "Dbmaintain to update db"<<endl;
}

int main(int argc, char** argv){
    enum {
        OPT_UPDATE,
        OPT_CLEAR,
        OPT_CLEAN,
        OPT_MARK_UPTO_DATE,
        OPT_RESET,
        OPT_HELP,
        OPT_FILE,
        OPT_SCRIPT_LOCATION,
        OPT_RUNNER,
        OPT_DB_URL,
        OPT_DB_DIALECT,

    };

    CSimpleOpt::SOption g_rgOptions[] = {
        // ID       TEXT          TYPE
        { OPT_FILE,   "-f",     SO_REQ_SEP }, // "-f config file, default to dbmaintain.config"
        { OPT_SCRIPT_LOCATION, "-s",     SO_REQ_SEP }, // "-s script location"
        { OPT_RUNNER, "-r",     SO_REQ_SEP }, // "runner"
        { OPT_RUNNER, "--runner",     SO_REQ_SEP }, // "runner"
        { OPT_DB_URL, "--db",     SO_REQ_SEP }, // "db url"
        { OPT_DB_DIALECT, "--type",     SO_REQ_SEP }, // "db url"
        { OPT_HELP,   "-?",     SO_NONE    }, // "-?"
        { OPT_HELP,   "--help", SO_NONE    }, // "--help"
        { OPT_UPDATE, "update", SO_NONE    },
        { OPT_UPDATE, "updateDatabase", SO_NONE    },
        { OPT_CLEAR, "clear", SO_NONE    },
        { OPT_CLEAR, "clearDatabase", SO_NONE    },
        { OPT_CLEAN, "clean", SO_NONE    },
        { OPT_CLEAN, "cleanDatabase", SO_NONE    },
        { OPT_MARK_UPTO_DATE, "markUptoDate", SO_NONE    },
        { OPT_RESET, "reset", SO_NONE    },
        SO_END_OF_OPTIONS                       // END
    };

    CSimpleOpt args(argc, argv, g_rgOptions);

    while (args.Next()) {
        if (args.LastError() == SO_SUCCESS) {
            switch (args.OptionId()) {
            case OPT_HELP:
                if(argc != 2){
                    cout << "invalid usage."<<endl;
                }
                usage();
                break;
            case OPT_FILE:
                break;
            }
            if (args.OptionId() == OPT_HELP) {
                usage();
                return 0;
            }

            cout << args.OptionId() << " " << args.OptionText() << " "
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
