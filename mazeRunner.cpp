/** CONTRACT: THE NAME OF THE EXECUTABLE PROGRAM MUST BE 'mazeRunner' FOR FLAG
 * CHECKING TO WORK CORRECTLY. AUTHORS: RAHUL GARG (s4008144), ARMAAN SINGH GILL
 * (s4051372), ISHPREET SINGH ARORA (s4093910)
 * **/

#include </usr/local/include/mcpp/mcpp.h>

#include <chrono>
#include <memory>
#include <thread>

#include "Menu.h"

int main(int argc, const char* argv[])
{
    // Checking for valid flags
    int testCount = 0;
    int fastCount = 0;
    int progCount = 0;
    bool testmode = false;
    bool fastmode = false;

// Accept paramters and flags for the program, make sure they are only one of
// each, and that they are valid.
    if (argc > 1) {
        for (int i = 0; i < argc; ++i) {
            if (std::string(argv[i]) == "-testmode") {
                testmode = true;
                ++testCount;
                if (testCount > 1) {
                    std::cout << std::endl
                              << "-testmode flag can only be used once in the "
                                 "command!"
                              << std::endl
                              << std::endl;
                    exit(1);
                }
            } else if (std::string(argv[i]) == "-fastmode") {
                fastmode = true;
                ++fastCount;
                if (fastCount > 1) {
                    std::cout << std::endl
                              << "-fastmode flag can only be used once in the "
                                 "command!"
                              << std::endl
                              << std::endl;
                    exit(1);
                }
            } else if (std::string(argv[i]) == "./mazeRunner") {
                // it is the name of the program
                ++progCount;
                if (progCount > 1) {
                    std::cout
                        << std::endl
                        << "./mazeRunner can only be used once in the command!"
                        << std::endl
                        << std::endl;
                    exit(1);
                }
            } else {
                std::cout << std::endl
                          << argv[i]
                          << " is an invalid flag. -testmode and -fastmode are "
                             "the only available flags!"
                          << std::endl
                          << std::endl;
                exit(1);
            }
        }
    }

    // make 5 attempts to connect to server, if all failed, then
    // exit the program, raising an error
    bool connectionMade = false;
    int conCounter = 0;

    while (!connectionMade && conCounter < 5) {
        try {
            std::shared_ptr<mcpp::MinecraftConnection> mc =
                std::make_shared<mcpp::MinecraftConnection>();
            Menu menu(mc, testmode, fastmode);
            menu.start();
            connectionMade = true;
        } catch (std::runtime_error&) {
            ++conCounter;
            std::cout << std::endl
                      << "Please make sure the server is running and there is "
                         "a player in the Minecraft world! (Connection "
                         "attempts remaining: "
                      << 5 - conCounter << ")" << std::endl;
            connectionMade = false;
        }
        std::this_thread::sleep_for(std::chrono::seconds(9));
    }

    std::cout << std::endl
              << "PROGRAM TIMED OUT: SERVER NOT RUNNING!" << std::endl
              << std::endl;

    exit(1);
}
