#include "DbDeploy.h"

#include <iostream>

#include "config/FileConfig.h"
#include "runner/ScriptRunner.h"
#include "runner/PostgresSqlScriptRunner.h"
#include "SimpleOpt.h"

class CmdOpt{
public:
    CmdOpt(int optionId, string optionArg):m_optionId(optionId), m_optionArg(optionArg){}
    ~CmdOpt(){}
private:
    int m_optionId;
    string m_optionArg;
};

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

    ScriptRunnerFactory::Register("postgres", &PostgresSqlScriptRunner::createInstance);

    Config config;

    DbDeploy deploy;
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
                FileConfig(args.OptionArg()).applyTo(config);
                break;
            case OPT_UPDATE:
                deploy.update(config);
                break;
            case OPT_CLEAR:
                deploy.clear(config);
                break;
            case OPT_CLEAN:
                deploy.clean(config);
                break;
            }

            cout << args.OptionId() << " " << args.OptionText() << " "
                 << (args.OptionArg() ? string(args.OptionArg()) : "") << endl;
        }else{
            cout << "Invalid argument: %s\n" << args.OptionText()<<endl;
            return 1;
        }
    }

    return 0;
}
