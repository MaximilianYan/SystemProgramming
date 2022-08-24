#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "testUI.h"

/**
 * @brief Prints help message to console
 * 
 */
void printHelpMessage() {
    printf(
        "Help message. Command list:\n"
        "-h\t\tshow help message\n"
        "-f [name]\trun tests from [name] file\n");
}

cmdParser::ParserResult cmdParser::processFlags(int argc, char** argv, void* userdata) { // &programMode as userdata
    cmdParser::CmdArgument cmdArguments[MAX_CMD_ARGS_COUNT] = {};
    int cmdFlagC = cmdParser::parse(argc, argv, cmdArguments);

    if (cmdFlagC == cmdParser::BAD_INPUT) {
        printf("Cannot recognize flags. Please use flags from list below.\n");
        printHelpMessage();
        return cmdParser::ParserResult::BAD_INPUT;
    }

    for (int cmdFlagI = 0; cmdFlagI < cmdFlagC; ++cmdFlagI) {
        assert(cmdArguments[cmdFlagI].isActive);

        switch (cmdArguments[cmdFlagI].key) {
        case 'h':
            printHelpMessage();
            break;
        case 'f':
            if (!cmdArguments[cmdFlagI].hasArgument) {
                printf("Cannot recognize file name after '-f'. Please use flags from list below.\n");
                printHelpMessage();
                return cmdParser::ParserResult::BAD_INPUT;
            }

            *(((ProccessFlagsPtrs*)userdata)->programModePtr) |= ProgramMode::EXT_TEST_FILE;
            strcpy(((ProccessFlagsPtrs*)userdata)->userTestFileName, cmdArguments[cmdFlagI].argument);
        default:
            break;
        }
    }
    return cmdParser::ParserResult::GOOD_INPUT;
}