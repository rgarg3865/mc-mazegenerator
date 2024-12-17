#include "Menu.h"

#include "Maze.h"
// Construtor that prints out the mode the program is being run in
Menu::Menu(std::shared_ptr<mcpp::MinecraftConnection> mc, bool testmode, bool fastmode)
    : mc(mc), testmode(testmode), fastmode(fastmode), terraform(mc, fastmode)
{
// Makes a maze object in the heap so it can be used after the scope of this function
    maze = new Maze(mc, testmode, fastmode);
    if (fastmode && testmode) {
        mode = "-------- TESTMODE + FASTMODE --------";
    }
    else if (testmode) {
        mode = "------------- TEST MODE -------------";
    }
    else if (fastmode) {
        mode = "------------- FAST MODE -------------";
    }
    else {
        mode = "------------ NORMAL MODE ------------";
    }
}

// Functions to print menus and prompts, etc., to reduce the ammount of
// repeating the same code in the Menu::start() function
void Menu::printStartText(void)
{
    std::cout << std::endl;
    std::cout << "Welcome to Minecraft MazeRunner!" << std::endl;
}

void Menu::printMainMenu(void)
{
    std::cout << std::endl << mode << std::endl << std::endl;
    std::cout << "------------- MAIN MENU -------------" << std::endl;
    std::cout << "1) Generate Maze" << std::endl;
    std::cout << "2) Build Maze in Minecraft" << std::endl;
    std::cout << "3) Solve Maze" << std::endl;
    std::cout << "4) Show Team Information" << std::endl;
    std::cout << "5) Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}

void Menu::printGenerateMazeMenu(void)
{
    std::cout << std::endl << mode << std::endl << std::endl;
    std::cout << "------------- GENERATE MAZE -------------" << std::endl;
    std::cout << "1) Read Maze from terminal" << std::endl;
    std::cout << "2) Generate Random Maze for Terraformed Platform"
              << std::endl;
    std::cout << "3) Generate Random Maze for Non - Terraformed Area"
              << std::endl;
    std::cout << "4) Back" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}

void Menu::printSolveMazeMenu(void)
{
    std::cout << std::endl << mode << std::endl << std::endl;
    std::cout << "------------- SOLVE MAZE -------------" << std::endl;
    std::cout << "1) Solve Manually" << std::endl;
    std::cout << "2) Show Escape Route (Wall Follower Algorithm)" << std::endl;
    std::cout << "3) Find Shortest Path (Breadth First Search)" << std::endl;
    std::cout << "4) Back" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}

void Menu::printTeamInfo(void)
{
    std::cout << std::endl;
    std::cout << "Team members:" << std::endl;
    std::cout << "\t [1] Rahul Garg (s4008144@student.rmit.edu.au)"
              << std::endl;
    std::cout << "\t [2] Armaan Singh Gill (s4051372@student.rmit.edu.au)"
              << std::endl;
    std::cout << "\t [3] Ishpreet Singh Arora (s4093910@student.rmit.edu.au)"
              << std::endl;
}

void Menu::printExitMessage(void)
{
    std::cout << std::endl;
    std::cout << "The End!" << std::endl;
}

// Function to clean an input string with regex
void Menu::clean(std::string &toClean)
{
// "select anything that is one or more spaces"
    std::regex r("\\s+");
    toClean = std::regex_replace(toClean, r, "");
}

void Menu::start()
{
// Set defaults for the menu class
    printStartText();
    std::string choice = "";
    std::string subChoice = "";
    int len = 0;
    int wid = 0;
    fastmode = !!fastmode;
    char **maze = nullptr;
    mcpp::Coordinate basePoint(0, 0, 0);
    mcpp::Coordinate carpet(0, 0, 0);
    mcpp::Coordinate testmodePoint(4848, 71, 4369);

    while (true) {
        choice = "";
        subChoice = "";
        printMainMenu();
        std::getline(std::cin, choice);
        ifEndOfFile();
        clean(choice);

        // generate maze branch
        if (choice == "1") {
            printGenerateMazeMenu();
            std::getline(std::cin, subChoice);
            ifEndOfFile();
            clean(subChoice);
            if (subChoice == "1" || subChoice == "2" || subChoice == "3") {
                /*    // NOT ERROR CHECKING ANY SET/GET MCPP CALLS BECAUSE THE
                 * MCPP API DOESN'T THROW AN EXCEPTION IF A PLAYER IS NOT IN THE
                 * WORLD, GIVEN THE FIRST CALL TO THE SET/GET FUNCTION IS
                 * SUCCESSFUL. https://github.com/rozukke/mcpp/issues/99*/

                if (subChoice == "1" && testmode) {
                    mc->setPlayerPosition(mcpp::Coordinate(4848, 71, 4369));
                    std::cout << "\nTeleported the player to (4848, 71, 4369) successfully!" << std::endl;
                }
                bool positionConfirmed =
                    isPositionConfirmed(basePoint, len, wid);

                if (positionConfirmed) {
                    if (subChoice == "1") {
                        do {
                            maze = readMazeFromTerminal(len, wid, carpet);
                        } while (isPassageTooWide(len, wid, maze));
                        validateUserMaze(maze, len, wid, carpet);
                        this->maze->setMazeMask(maze);
                        this->maze->setCanTerraform(true);
                        this->maze->printMaze();
                    } else if (subChoice == "2") {
                        this->maze->setCanTerraform(true);
                        this->maze->generateMaze();
                    } else if (subChoice == "3") {
                        this->maze->setCanTerraform(false);
                        this->maze->generateMaze();
                    }
                } else {
                    std::cout << std::endl
                              << "INVALID INPUT! REDIRECTING TO MAIN MENU..."
                              << std::endl;
                }

            } else if (subChoice == "4") {
            } else {
                std::cout << std::endl
                          << "INVALID INPUT! REDIRECTING TO MAIN MENU..."
                          << std::endl;
            }

        // build maze branch
        } else if (choice == "2") {
            this->maze->buildMaze();

        // solve maze branch
        } else if (choice == "3") {
            printSolveMazeMenu();
            std::getline(std::cin, subChoice);
            ifEndOfFile();
            clean(subChoice);

            if (subChoice == "1") {
                this->maze->teleportPlayer();
            } else if (subChoice == "2") {
                this->maze->solveMaze(RIGHT_HAND_FOLLOW);
            } else if (subChoice == "3") {
                this->maze->solveMaze(BREATH_FIRST_SEARCH);
            } else if (subChoice == "4") {
            } else {
                std::cout << std::endl
                          << "INVALID INPUT! REDIRECTING TO MAIN MENU..."
                          << std::endl;
            }

        // prints team information
        } else if (choice == "4") {
            printTeamInfo();

        // End the program (restoring the maze only if it is built)
        } else if (choice == "5") {
            this->maze->restore();
            printExitMessage();
            exit(0);

        // Invalid input prompt
        } else {
            std::cout << std::endl
                      << "PLEASE ENTER A NUMBER 1-5 TO CONTINUE: " << std::endl;
        }
    }
}

// Catch if the user has input the eof symbol (Ctrl+D)
void Menu::ifEndOfFile()
{
    if (std::cin.eof()) {
        std::cout << std::endl
                  << std::endl
                  << "STANDARD INPUT REACHED END OF FILE!" << std::endl;
        this->maze->restore();
        printExitMessage();
        exit(0);
    }
}

// Check if the user's entered dimensions of the maze are valid
bool Menu::checkDimensions(std::string &value)
{
    std::getline(std::cin, value);
    ifEndOfFile();
    bool isValid = false;
    bool exitFunc = false;
    int num = 0;

    if (value.length() > 3) {
        std::cout << "\nINVALID INPUT!" << std::endl;
        isValid = false;
        exitFunc = true;
    }

    if (!exitFunc) {
        for (unsigned int i = 0; i < value.length(); ++i) {
            if (isdigit(value.at(i)) == 0) {
                std::cout << "\nINVALID INPUT!" << std::endl;
                isValid = false;
                i = value.length() - 1;
            } else {
                isValid = true;
            }
        }

        if (isValid) {
            num = std::stoi(value);
            if (num > 301) {
                isValid = false;
                std::cout << "\nINVALID INPUT!" << std::endl;
            }
            if (isValid) {
                if (num < 3 || (num % 2 != 1)) {
                    isValid = false;
                    std::cout << "\nINVALID INPUT!" << std::endl;
                }
            }
        }
    }

    return isValid;
}
//--------------------------------

// Grab the player's position when the user has entered 'done'
// to set the basepoint of the maze
bool Menu::isPositionConfirmed(mcpp::Coordinate &basePoint, int &len, int &wid)
{
    std::string confirmation = "";
    bool positionConfirmed = false;
    std::cout << std::endl
              << "In Minecraft, navigate to where you need the "
                 "maze to be "
                 "built and type - done: "
              << std::endl;
    std::getline(std::cin, confirmation);
    ifEndOfFile();

    clean(confirmation);

    if (confirmation == "done") {
        basePoint = mc->getPlayerPosition();
        this->maze->setBasePoint(basePoint);
        setValidLengthWidthBP(len, wid);
        positionConfirmed = true;
    }
    return positionConfirmed;
}

// Check if the user entered maze has a path 2 blocks wide
bool Menu::isPassageTooWide(int &len, int &wid, char **maze)
{
    bool isTooWide = false;
    for (int i = 0; i < wid - 1; i++) {
        for (int j = 0; j < len - 1; j++) {
            if (maze[i][j] == '.' && maze[i + 1][j] == '.' &&
                maze[i][j + 1] == '.' && maze[i + 1][j + 1] == '.') {
                isTooWide = true;
            }
        }
    }

    if (isTooWide) {
        std::cout << "\nTHE MAZE'S PASSAGES CAN ONLY BE 1 BLOCK WIDE!" << std::endl;
        std::cout << "MAZE RESET, PLEASE START AGAIN!" << std::endl;
    }
    return isTooWide;
}

// Get a valid length and width for the maze from the user
// Also set the basepoint of the program
void Menu::setValidLengthWidthBP(int &len, int &wid)
{
    bool validLength = false;
    bool validWidth = false;
    std::string length = "";
    std::string width = "";

    while (!validLength) {
        std::cout << std::endl
                  << "Enter the LENGTH of the maze. ODD "
                     "INTEGERS ONLY (MIN = 3, MAX = 301)!"
                  << std::endl;
        validLength = checkDimensions(length);
    }

    len = std::stoi(length);
    this->maze->setXlen(len);

    while (!validWidth) {
        std::cout << std::endl
                  << "Enter the WIDTH of the maze. ODD "
                     "INTEGERS ONLY (MIN = 3, MAX = 301)!"
                  << std::endl;
        validWidth = checkDimensions(width);
    }

    wid = std::stoi(width);
    this->maze->setZlen(wid);
}

// Read the maze from terminal and validate it
char ** Menu::readMazeFromTerminal(int & len, int & wid, mcpp::Coordinate &carpet) {
    std::string maze = "";
    std::string currLine = "";
    bool lineRead = false;
    int exitCounter = 0;
    std::cout << std::endl << "Enter the maze structure: " << std::endl;

    for (int i = 0; i < wid; ++i) {
        // Catch if the user inputs the eof command
        std::cin.clear();
        std::getline(std::cin, currLine);
        ifEndOfFile();

        if (currLine.length() == (unsigned int)len) {
            for (int j = 0; j < len; ++j) {

                if (currLine.at(j) == 'x' || currLine.at(j) == '.') {
                    lineRead = true;

                    // EXIT TRACKER
                    // Track how many exists exist in the maze
                    if ((((i == 0 || i == wid - 1) && j != 0 && j != len - 1) || ((j == 0 || j == len - 1) && i != 0 && i != wid - 1)) && currLine.at(j) == '.') {
                        ++exitCounter;
                        carpet = mcpp::Coordinate(j, 0, i);
                    }

                    // CORNERS CANNOT BE EXITS
                    else if ((((i == 0 || i == wid - 1) && (j == 0 || j == len - 1)) || ((j == 0 || j == len - 1) && (i == 0 || i == wid - 1))) && currLine.at(j) == '.') {
                        currLine = "";
                        lineRead = false;
                        i = -1;
                        j = len - 1;
                        exitCounter = 0;
                        maze.clear();
                        carpet = mcpp::Coordinate(0, 0, 0);
                        std::cout <<
                            std::endl <<
                            "THE MAZE'S CORNERS CANNOT BE EXITS!" << std::endl << "MAZE RESET, PLEASE START AGAIN!" << std::endl;
                        redoMazeInput(maze, len);

                    }

                    // CHECKING IF THERE IS AT LEAST ONE VALID EXIT
                    if (i == wid - 1 && j == len - 1 && exitCounter == 0) {
                        currLine = "";
                        lineRead = false;
                        --i;
                        j = len - 1;
                        std::cout <<
                            std::endl <<
                            "THE MAZE MUST HAVE A VALID EXIT (ONLY 1 AIR BLOCK IN THE OUTERMOST EDGES OF THE MAZE). CORNERS CANNOT BE EXITS!" << std::endl;
                        redoMazeInput(maze, len);
                    }

                    if (exitCounter > 1) {
                        currLine = "";
                        lineRead = false;
                        i = -1;
                        j = len - 1;
                        exitCounter = 0;
                        maze.clear();
                        carpet = mcpp::Coordinate(0, 0, 0);
                        std::cout << std::endl << "THE MAZE CAN ONLY HAVE 1 EXIT (ONLY 1 AIR BLOCK IN THE OUTERMOST EDGES OF THE MAZE). CORNERS CANNOT BE EXITS!" << std::endl << "MAZE RESET, PLEASE START AGAIN!" << std::endl;
                        redoMazeInput(maze, len);
                    }
                }
                else {
                //incase the user inputs something other than an 'x' or a '.'
                    currLine = "";
                    lineRead = false;
                    --i;
                    j = len - 1;
                    std::cout <<
                        std::endl <<
                        "INCORRECT INPUT, USE 'x' TO DENOTE WALLS AND '.' FOR AIR!" <<
                        std::endl;
                    redoMazeInput(maze, len);
                }
            }
        }
        else {
            // incase the user's maze input is not the same
            // length as what is was set to be
            currLine = "";
            lineRead = false;
            --i;
            std::cout << std::endl <<
                "LENGTH OF A SINGLE ROW MUST BE " << len <<
                " AS SPECIFIED BY YOU!" << std::endl;
            redoMazeInput(maze, len);
        }

        if (lineRead) {
            maze += currLine;
        }
    }

    char ** finalMaze = new char * [wid];
    int counter = 0;

// Convert each line of the maze to a complete character array
    for (int i = 0; i < wid; ++i) {
        finalMaze[i] = new char[len];
        for (int j = 0; j < len; ++j) {
            finalMaze[i][j] = maze.at(counter);
            ++counter;
        }
    }

    this->maze->setExit(carpet);
    return finalMaze;
}

// Retake any bad lines for the maze
void Menu::redoMazeInput(std::string &maze, int len)
{
    // change name to redoMazeInputLine()
    if (!maze.empty()) {
        std::cout << std::endl
                  << "Enter the remaining maze structure: " << std::endl;
        for (unsigned int i = 1; i <= maze.length(); ++i) {
            std::cout << maze.at(i - 1);
            if (i % len == 0) {
                std::cout << std::endl;
            }
        }
    } else {
        std::cout << std::endl << "Enter the maze structure: " << std::endl;
    }
}

// Validate the user's maze (extension task 3)
void Menu::validateUserMaze(char** &maze, int &len, int &wid, mcpp::Coordinate &carpet) {
    Coord exit = {carpet.z, carpet.x};
    MazeValidation validation(maze, len, wid, exit);
    if (!validation.checkFixes(maze)) {
        bool validInput = false;
        while (!validInput) {
            std::cout << "\nTHE ENTERED MAZE IS NOT PERFECT (THERE ARE LOOPS OR ISOLATIONS)!" << std::endl;
            std::cout << "DO YOU WANT THE ALGORITHM TO REMOVE ANY AND ALL IMPERFECTIONS? THIS IS AN EXPERIMENTAL FEATURE! (y/n):" << std::endl;
            std::string input = "";
            std::getline(std::cin, input);
            clean(input);
            if (input == "y") {
                maze = validation.getPerfectMaze();
                validInput = true;
            }
            else if (input == "n") {
                std::cout << "\nPROCEEDING WITH IMPERFECT MAZE, THIS COULD LEAD TO AN UNSOLVABLE MAZE!" << std::endl;
                validInput = true;
            }
            else {
                std::cout << "\nINVALID INPUT!" << std::endl;
                validInput = false;
            }
        }
    }
}
