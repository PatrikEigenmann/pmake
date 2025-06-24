// ****************************************************************************************************
// Our custom "pmake" program is designed to empower developers by streamlining the build process with
// simplicity and efficiency. Tailored specifically for flexibility, it reads configuration files,
// interprets instructions, and executes commands to compile and build projects seamlessly. By offering
// an intuitive and robust solution, our Make program not only enhances productivity but also ensures
// consistency across various development environments. This tool is an essential asset for any development
// team, enabling faster turnaround times and improved project management.
// 
// To buil `pmake` you can use any of the following methods:
// =========================================================
// Direct compiler invocation:
//     gcc     -Wall -Wextra -std=c99 -Iinclude src/*.c -o bin/pmake
//     clang   -Wall -Wextra -std=c99 -Iinclude src/*.c -o bin/pmake
//
// Using an older version of pmake:
//     pmake pmake.pmake
//
// Using GNU Make (Makefile already integrated):
//     make
//
// Using CMake (CMakeLists.txt already integrated):
//     mkdir build && cd build
//     cmake ..
//
// To install `pmake` into a system-wide `bin/` directory:
//     On Unix-like systems:
//         chmod +x ./scripts/install.sh
//         ./scripts/install.sh
//     On Windows:
//         .\scripts\install.cmd
// ----------------------------------------------------------------------------------------------------
// Author:  Patrik Eigenmann
// eMail:   p.eigenmann@gmx.net
// GitHub:  www.github.com/PatrikEigenmann/pmake
// ----------------------------------------------------------------------------------------------------
// Fri 2024-11-15 File created.                                                             Version: 00.01
// Sat 2024-11-16 Rearranged the whole program and renamed it. Fixed some bug too.          Version: 00.02
// Sat 2024-11-16 Bug Fix in putting the compiler command togheter.                         Version: 00.03
// Sat 2024-11-16 Rearranged process_makefile() for a clearer, top-down structure.          Version: 00.04
// Sat 2024-11-16 Fixed -c flag handling in process_makefile().                             Version: 00.05
// Sat 2024-11-16 Fixed .o suffix handling with -c flag in process_makefile().              Version: 00.06
// Tue 2024-11-19 Merged cVersion/cManPage into pmake.c for simplicity.                     Version: 00.07
// Thu 2024-11-21 Updated doesFileExist() to validate file and version.                     Version: 00.08
// Wed 2025-01-22 Updated header comment with GitHub URL updated.                           Version: 00.09
// Sun 2025-03-23 Target handles now obj, exec or shared.                                   Version: 00.10
// Sun 2025-03-23 Fixed a bug while handling "shared".                                      Version: 00.11
// Tue 2025-03-25 Changed the way the compiler command is put together.                     Version: 00.12
// Tue 2025-03-25 Shared Library under Windows is now a .dll and under Unix a .so.          Version: 00.13
// Sun 2025-04-06 Increased the library string to 10000 characters.                         Version: 00.14
// Sun 2025-04-06 New approach with a large library string. Using a isLibs flag.            Version: 00.15
// Sun 2025-04-06 BugFix in with the library string. strcpy wasn't working.                 Version: 00.16
// Sun 2025-04-06 BugFix in the library string. Switched to append_format.                  Version: 00.17
// Sun 2025-04-06 Making sure that the bug fix doesn't influence the make_process.          Version: 00.18
// Sun 2025-06-22 Loading now the file {project}.pmake instead of {project}.makefile.       Version: 00.19
// Sun 2025-06-22 Complete overhaul of this tool, because of an unfixable bug.              Version: 00.20
// Tue 2025-06-24 Renewed the manpage style help text because of its new functionality.     Version: 00.21
// Tue 2025-06-25 Updated the manpage style help text.                                      Version: 00.22 
// -----------------------------------------------------------------------------------------------------
// To Do's:
// - Take cVersion.h & cVersion.c appart and integrate it directly into this code base.             Done.                             Done.
// - Take cManPage.h & cManPage.c appart and integrate it directly into this code base.             Done.                             Done.
// - Check file exists before version in doesFileExists().                                          Done.
// - Make sure if the library string isn't in the end, the program doesn't crash.                   Done.
// - Implement linker flags directive in the pmake file.
// *****************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "debug.h"
#include "version.h"
#include "manpage.h"

// -----------------------------------------------------------------------------------------------------
// main - The main function is the starting point of a C or C++ program, where the execution of the program
// begins. This version of the main function allows the program to take command-line arguments when it runs.
// The function typically returns an numbered value to the operating system, often zero to signify successful
// execution.
//
// @param argc  The number of command-line arguments.
// @param argv  The array of command-line arguments.
// @return      EXIT_SUCCESS on successful completion, EXIT_FAILURE on error.
// -----------------------------------------------------------------------------------------------------
int main(int argc, char **argv) {

    Version v = create_version(0, 22);
    
    if (argc < 2 || isHelpTriggered(argc, argv[1])) {
        print_help(v);
        return EXIT_SUCCESS;
    }

    if (argc > 1 && strcmp(argv[1], "--version") == 0) {
        
        char buf[8];
        to_string(v, buf);
        printf("pmake version %s\n", buf);
        return EXIT_SUCCESS;
    }

    char *errmsg = NULL;
    char *filename = normalize_filename(argv[1]);
    Makefile *mf = parse(filename, &errmsg);
    debug("comp    = '%s'\n", mf->comp);
    debug("project = '%s'\n", mf->project);

    if (errmsg) {
        printf("Error: %s\n", errmsg);
        free(errmsg);
        return EXIT_FAILURE;
    }

    run(mf, &errmsg);

    if (errmsg) {
        printf("Error: %s\n", errmsg);
        free(errmsg);
        free_makefile(mf);
        return EXIT_FAILURE;
    }

    free_makefile(mf);
    return EXIT_SUCCESS;
}