/*
author          Oliver Blaser
date            15.02.2025
copyright       GPL-3.0 - Copyright (c) 2025 Oliver Blaser
*/

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "project.h"



// ANSI escape codes
#define SGR_FG_BLACK    "\033[30m"
#define SGR_FG_RED      "\033[31m"
#define SGR_FG_GREEN    "\033[32m"
#define SGR_FG_YELLOW   "\033[33m"
#define SGR_FG_BLUE     "\033[34m"
#define SGR_FG_MAGENTA  "\033[35m"
#define SGR_FG_CYAN     "\033[36m"
#define SGR_FG_WHITE    "\033[37m"
#define SGR_FG_DEFAULT  "\033[39m"
#define SGR_FG_BBLACK   "\033[90m"
#define SGR_FG_BRED     "\033[91m"
#define SGR_FG_BGREEN   "\033[92m"
#define SGR_FG_BYELLOW  "\033[93m"
#define SGR_FG_BBLUE    "\033[94m"
#define SGR_FG_BMAGENTA "\033[95m"
#define SGR_FG_BCYAN    "\033[96m"
#define SGR_FG_BWHITE   "\033[97m"



using std::cout;
using std::endl;


namespace fs = std::filesystem;

namespace {

const std::string usageString = std::string(prj::exeName) + " /path/to/backup-dir/rdiff-backup-data/backup.log";

void printUsageAndTryHelp()
{
    cout << "Usage: " << usageString << "\n\n";
    // cout << "Try '" << prj::exeName << " --help' for more options." << endl;
}

void printVersion()
{
    cout << prj::appName << "   ";

#ifdef IG_OMW_VERSION_H

    const omw::Version& v = prj::version;
    if (v.isPreRelease()) cout << omw::fgBrightMagenta;
    cout << v.toString();
    if (v.isPreRelease()) cout << omw::defaultForeColor;
#if PRJ_DEBUG
    cout << "   " << omw::fgBrightRed << "DEBUG" << omw::defaultForeColor << "   " << __DATE__ << " " << __TIME__;
#endif // PRJ_DEBUG

#else // IG_OMW_VERSION_H

    const bool isPreRelease = (std::string(prj::version).find('-') != std::string::npos);
    if (isPreRelease) cout << SGR_FG_BMAGENTA;
    cout << prj::version;
    if (isPreRelease) cout << SGR_FG_DEFAULT;
#if PRJ_DEBUG
    cout << "   " << SGR_FG_BRED << "DEBUG" << SGR_FG_DEFAULT << "   " << __DATE__ << " " << __TIME__;
#endif // PRJ_DEBUG

#endif // IG_OMW_VERSION_H

    cout << endl;

    cout << endl;
    cout << "project page: " << prj::website << endl;
    cout << endl;
    cout << "Copyright (c) 2025 Oliver Blaser." << endl;
    cout << "License: GNU GPLv3 <http://gnu.org/licenses/>." << endl;
    cout << "This is free software. There is NO WARRANTY." << endl;
}

} // namespace



static int process(std::ifstream& ifs);
static bool isLineOfInterest(const std::string& str);
static std::string iostateToString(std::ios& ios);



int main(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i)
    {
        if (std::strcmp(argv[i], "--help") == 0)
        {
            ::printUsageAndTryHelp();
            return 0;
        }
        else if (std::strcmp(argv[i], "--version") == 0)
        {
            ::printVersion();
            return 0;
        }
    }



    int r = 1;

    if (argc == 2)
    {
        const std::string argFile = argv[1];
        fs::path filePath = fs::weakly_canonical(argFile);

        if (fs::is_directory(filePath)) { filePath /= "rdiff-backup-data/backup.log"; }

        if (fs::exists(filePath))
        {
            std::ifstream ifs;

            try
            {
                ifs.exceptions(std::ios::badbit | std::ios::failbit);
                ifs.open(filePath, std::ios::in);

                ifs.exceptions(std::ios::badbit);
                r = process(ifs);
            }
            catch (const std::exception& ex)
            {
                cout << "fatal error: " << ex.what() << iostateToString(ifs) << endl;
                r = 1;
            }
            catch (...)
            {
                cout << "unknown fatal error" << iostateToString(ifs) << endl;
                r = 1;
            }

            if (ifs.is_open()) { ifs.close(); }
        }
        else
        {
            cout << "log file " << filePath << " not found" << endl;
            r = 1;
        }
    }
    else
    {
        printUsageAndTryHelp();
        r = 1;
    }

    return r;
}



int process(std::ifstream& ifs)
{
    size_t line = 0;
    bool isConsecutiveLoi = false;

    while (ifs.good())
    {
        std::string str;
        std::getline(ifs, str);

        if (ifs.fail())
        {
#if PRJ_DEBUG
            cout << SGR_FG_BMAGENTA << "break due to fail()" << SGR_FG_DEFAULT << endl;
#endif // PRJ_DEBUG

            break;
        }

        if (isLineOfInterest(str))
        {
            if (!isConsecutiveLoi && (line > 0)) { cout << endl << SGR_FG_BBLACK << "#" << line << SGR_FG_DEFAULT << endl; }

            cout << str << endl;

            isConsecutiveLoi = true;
        }
        else
        {
            // if (isConsecutiveLoi) { cout << endl; }

            isConsecutiveLoi = false;
        }

        ++line;
    }

    return 0;
}

bool isLineOfInterest(const std::string& str)
{
    // clang-format off
    const std::array<std::string, 5> okLineBegin = {
        "* Processing file ",
        "* Processing changed file ",
        "* Incrementing mirror file ",
        "* Regressing file ",
        "* Cleaning up"
    };
    // clang-format on

    bool isLoi = true;

    for (const auto& s : okLineBegin)
    {
        if (std::strncmp(str.c_str(), s.c_str(), s.length()) == 0)
        {
            isLoi = false;
            break;
        }
    }

    return (isLoi && !str.empty());
}

std::string iostateToString(std::ios& ios)
{
    std::string str = "";

    if (ios.eof()) { str += " EOF"; }
    if (ios.fail()) { str += " FAIL"; }
    if (ios.bad()) { str += " BAD"; }

    return str;
}
