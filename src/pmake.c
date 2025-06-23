// ****************************************************************************************************
// Our custom "pmake" program is designed to empower developers by streamlining the build process with
// simplicity and efficiency. Tailored specifically for flexibility, it reads configuration files,
// interprets instructions, and executes commands to compile and build projects seamlessly. By offering
// an intuitive and robust solution, our Make program not only enhances productivity but also ensures
// consistency across various development environments. This tool is an essential asset for any development
// team, enabling faster turnaround times and improved project management.
// 
// Compile information:
// To avoid unnecessary dependencies to libraries all functions and methods are integraded into this
// codefile. There for a simple call of the compiler to create an executable without any dependencies
// is probably the easiest way to create your own pmake tool. If you already have a working pmake, you
// can use it to compile this code.
// gcc -Wall -Wextra -g -Iinclude src/*.c -o bin/pmake
// clang -Wall -Wextra -g -Iinclude src/*.c -o bin/pmake 
/* ----------------------------------------------------------------------------------------------------
 * Author:  Patrik Eigenmann
 * eMail:   p.eigenmann@gmx.net
 * GitHub:  www.github.com/PatrikEigenmann/cpp
 * ----------------------------------------------------------------------------------------------------
 * Fri 2024-11-15 File created.                                                         Version: 00.01
 * Sat 2024-11-16 Rearranged the whole program and renamed it. Fixed some bug too.      Version: 00.02
 * Sat 2024-11-16 Bug Fix in putting the compiler command togheter.                     Version: 00.03
 * Sat 2024-11-16 Rearranged the function process_makefile to a more concice top down   Version: 00.04
 *                way.
 * Sat 2024-11-16 BugFix - in function process_makefile compiling with the -c flad      Version: 00.05
 *                didn't work.
 * Sat 2024-11-16 Another BugFix - in the function process_makefile, adding .o ending   Version: 00.06
 *                when compiling with the -c flag.
 * Tue 2024-11-19 For ease of compiling this project, I have included the               Version: 00.07
 *                functionallity from cVersion & cManPage directly into the code file
 *                pmake.c.
 * Thu 2024-11-21 Updated the method doesFileExist(), first check if file exists,       Version: 00.08
 *                second check if it is the same version.
 * Wed 2025-01-22 Header comment GitHub URL updated.                                    Version: 00.09
 * Sun 2025-03-23 Target handles now obj, exec or shared.                               Version: 00.10
 * Sun 2025-03-23 Fixed a bug while handling "shared".                                  Version: 00.11
 * Tue 2025-03-25 Changed the way the compiler command is put together.                 Version: 00.12
 * Tue 2025-03-25 Shared Library under Windows is now a .dll and under Unix a .so.      Version: 00.13
 * Sun 2025-04-06 Increased the library string to 10000 characters.                     Version: 00.14
 * Sun 2025-04-06 New approach with a large library string. Using a isLibs flag.        Version: 00.15
 * Sun 2025-04-06 BugFix in with the library string. strcpy wasn't working.             Version: 00.16
 * Sun 2025-04-06 BugFix in the library string. Switched to append_format.              Version: 00.17
 * Sun 2025-04-06 Making sure that the bug fix doesn't influence the make_process.      Version: 00.18
 * Sun 2025-06-22 Loading now the file {project}.pmake instead of {project}.makefile.   Version: 00.19
 * Sun 2025-06-22 Complete overhaul of this tool, because of an unfixable bug.          Version: 00.20 
 * -----------------------------------------------------------------------------------------------------
 * To Do's:
 * - Take cVersion.h & cVersion.c appart and integrate it directly into this code base.             Done.                             Done.
 * - Take cManPage.h & cManPage.c appart and integrate it directly into this code base.             Done.                             Done.
 * - Updated the method doesFileExists, checking first if the file really exist, then if it is      Done.
 *   the same version.
 * - Make sure if the library string isn't in the end, the program doesn't crash.                   Done.
 * *****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "debug.h"
#include "version.h"
#include "manpage.h"

// ---------------------------------------------------------------------------------------------------
// Our main function serves as the command center of your network diagnostics tool, orchestrating the
// seamless execution of all key operations. It handles user input, dynamically configures the scanning
// range, initiates progress tracking, and processes the results with utmost efficiency. Designed for
// top-down clarity, this function ensures that every step, from initialization to output, is executed
// with precision.
//
// By leveraging the main function, developers can effortlessly manage complex network diagnostics tasks,
// ensuring a streamlined and intuitive user experience. It integrates all components harmoniously,
// reflecting the highest standards of coding excellence.
//
// @param argc  The number of command-line arguments.
// @param argv  The array of command-line arguments.
// @return      0 on successful completion, 1 on error.
// ----------------------------------------------------------------------------------------------------
int main(int argc, char **argv) {

    Version v = create_version(0, 20);
    
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