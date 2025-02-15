/*
author          Oliver Blaser
date            15.02.2025
copyright       GPL-3.0 - Copyright (c) 2025 Oliver Blaser
*/

#ifndef IG_PROJECT_H
#define IG_PROJECT_H

// #include <omw/defs.h>
// #include <omw/version.h>


namespace prj {

const char* const appDirName_windows = "rdiff-backup-log-check";
const char* const appDirName_unix = "rdiff-backup-log-check";

#ifdef _WIN32 // OMW_PLAT_WIN
const char* const appDirName = appDirName_windows;
#else
const char* const appDirName = appDirName_unix;
#endif

const char* const appName = "rdiff-backup-log-check";
const char* const exeName = "rdiff-backup-log-check"; // eq to the linker setting

const char* const website = "https://github.com/oblaser/rdiff-backup-log-check";

// const omw::Version version(0, 1, 0, "alpha");
const char* const version = "0.1.0-alpha";

} // namespace prj


#if defined(DEBUG) || defined(_DEBUG)
#define PRJ_DEBUG (1)
#else
#undef PRJ_DEBUG
#endif


#endif // IG_PROJECT_H
