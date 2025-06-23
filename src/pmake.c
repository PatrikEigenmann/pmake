/* ****************************************************************************************************
 * Our custom "pmake" program is designed to empower developers by streamlining the build process with
 * simplicity and efficiency. Tailored specifically for flexibility, it reads configuration files,
 * interprets instructions, and executes commands to compile and build projects seamlessly. By offering
 * an intuitive and robust solution, our Make program not only enhances productivity but also ensures
 * consistency across various development environments. This tool is an essential asset for any development
 * team, enabling faster turnaround times and improved project management.
 * 
 * Compile information:
 * To avoid unnecessary dependencies to libraries all functions and methods are integraded into this
 * codefile. There for a simple call of the compiler to create an executable without any dependencies
 * is probably the easiest way to create your own pmake tool. If you already have a working pmake, you
 * can use it to compile this code. 
 * clang/gcc pmake.c -o pmake
 * pmake pmake.makefile
 * ----------------------------------------------------------------------------------------------------
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
#include <stdarg.h>
#include <ctype.h>

#ifdef _WIN32
    
    // Include Windows relevant libraries
    #include <io.h>
    
    #define _home() getenv("USERPROFILE")

    /* ----------------------------------------------------------------------------------------------------
     * Windows version:
     * By encapsulating the file existence check within this method, we ensure a seamless and efficient way
     * to verify the presence of files. This method not only enhances the maintainability and readability of
     * your codebase, but also guarantees that file existence checks are performed consistently and reliably.
     * 
     * Adopting the doesFileExist method will streamline your file handling processes, fostering better
     * organization and error handling, and ultimately contributing to a more robust and user-friendly product.
     * 
     * @param char *filename    - The name of the file to check for existence.
     * @param int major         - The major # of the verison number.
     * @param int minor         - The minor # of the version number.
     * @return int              - Returns 1 if the file exists, and 0 if it does not.
     * ---------------------------------------------------------------------------------------------------- */
    int doesFileExist(char *filename, int major, int minor) {

        // First check if the file even exist
        int i = (_access(filename, 0) != -1);

        // If it does,
        if(i > 0) {

            FILE *file;
            char *buffer;
            char version[100] = "\0";
            long file_size;

            // Open the file for reading
            file = fopen(filename, "r");
                        
            // Determine the file size
            fseek(file, 0, SEEK_END);
            file_size = ftell(file);
            rewind(file);
            
            // Allocate memory for the buffer
            buffer = (char *)malloc(sizeof(char) * (file_size + 1));
            
            if (buffer == NULL) {
                printf("Memory allocation failed.\n");
                fclose(file);
                return 1;
            }
            
            // Read the file content into the buffer
            fread(buffer, sizeof(char), file_size, file);
            
            // Null-terminate the string
            buffer[file_size] = '\0';
            
            
            // Close the file
            fclose(file);

            sprintf(version, "%02d.%02d", major, minor);

            i &= (strstr(buffer, version) != NULL);
        }

        return i;
    }
    
    // The command I use under Windows is more. More is the equivalent
    // of the UNIX less command.
    char command[255] = "more ";

    char *PATH = "\\AppData\\Local\\";
#else
    // Include Unix relevant libraries
    #include <unistd.h>

    #define _home() getenv("HOME")

    /* -------------------------------------------------------------------------------------------------
     * MacOS version:
     * By encapsulating the file existence check within this method, we ensure a seamless and efficient
     * way to verify the presence of files. This method not only enhances the maintainability and
     * readability of your codebase, but also guarantees that file existence checks are performed
     * consistently and reliably.
     * 
     * Adopting the doesFileExist method will streamline your file handling processes, fostering better
     * organization and error handling, and ultimately contributing to a more robust and user-friendly product.
     * 
     * @param char *filename    - The name of the file to check for existence.
     * @param int major         - The major # of the verison number.
     * @param int minor         - The minor # of the version number.
     * @return int              - Returns 1 if the file exists, and 0 if it does not.
     * ------------------------------------------------------------------------------------------------- */
    int doesFileExist(char *filename, int major, int minor) {

        int i = (access(filename, F_OK) != -1);

        if(i > 0) {
            FILE *file;
            char *buffer;
            char version[100] = "\0";
            long file_size;

            // Open the file for reading
            file = fopen(filename, "r");
                        
            // Determine the file size
            fseek(file, 0, SEEK_END);
            file_size = ftell(file);
            rewind(file);
            
            // Allocate memory for the buffer
            buffer = (char *)malloc(sizeof(char) * (file_size + 1));
            
            if (buffer == NULL) {
                printf("Memory allocation failed.\n");
                fclose(file);
                return 1;
            }
            
            // Read the file content into the buffer
            fread(buffer, sizeof(char), file_size, file);
            
            // Null-terminate the string
            buffer[file_size] = '\0';
            
            
            // Close the file
            fclose(file);

            sprintf(version, "%02d.%02d", major, minor);

            i &= (strstr(buffer, version) != NULL);
        }

        return i; 
    }

    // The command I use under MacOS/Unix is less. Less is the equivalent
    // of the Windows more command.
    char command[255] = "less ";

    char *PATH = "/.local/share/";
#endif

/* ***************************************START INTEGRATION*********************************************** */

/* *******************************************************************************************************
 * cVersion - This code was copied from cVersion so I have a more easier way to compile.
 * ******************************************************************************************************* */

// -------------------------------------------------------------------------------------------------------
// This structure as a small box labeled “Version”. Inside this box, there are two compartments. One
// compartment is labeled “major”, and the other is labeled “minor”.
//
// These compartments can hold numbers. The number in the “major” compartment represents big changes or
// updates, like a new design or a significant feature. The number in the “minor” compartment represents
// small changes or updates, like a bug fix or a minor improvement.
//
// So, this “Version” box helps us keep track of both big and small updates in an organized way.
// -------------------------------------------------------------------------------------------------------
typedef struct {
    int major;  // Major built
    int minor;  // Minor built
} Version;

// --------------------------------------------------------------------------------------------------------
// The create_version function is a set of instructions that takes two numbers as input. These numbers
// represent the major and minor parts of a version number, respectively. The function then constructs a
// Version object using these two numbers. The major and minor numbers are stored within this Version object.
// Once this Version object is created, it is then returned by the function. This Version object can be used
// to represent and keep track of the version number of a software or a product. The major part of the version
// usually indicates significant changes or updates, while the minor part indicates smaller updates or bug
// fixes.
//
// @param int major - Major build number.
// @param int minor - Minor build number.
// --------------------------------------------------------------------------------------------------------
Version create_version(int major, int minor) {
    Version v;
    v.major = major;
    v.minor = minor;
    return v;
}

// --------------------------------------------------------------------------------------------------------
// The to_string function is like a translator for the Version object. When you call to_string(v, buffer)
// where v is a Version object and buffer is a space in memory to store a string, you’re essentially asking,
// “Can you translate your version number into a string format?”
//
// In response to this request, the Version object takes its major and minor version numbers, formats them
// into a string that looks like “MM.mm” (where MM is the major version number and mm is the minor version
// number), and then stores this string in the provided buffer.
//
// So, in simple terms, to_string is a way to convert the version number of a Version object into a string
// format.
// --------------------------------------------------------------------------------------------------------
void to_string(Version v, char* buffer) {
    sprintf(buffer, "%02d.%02d", v.major, v.minor);
}

/* ********************************************************************************************************
 * cManPage - This code was copied from cManPage so I have a more easier way to compile.
 * ******************************************************************************************************** */

/* --------------------------------------------------------------------------------------------------------
 * The ManPage struct is a crucial data structure designed to streamline and enhance the management of
 * manual pages within your applications. It encapsulates all the necessary details of a manual page,
 * including the filename and its corresponding content, into a single, organized unit. This structured
 * approach not only simplifies the creation and modification of documentation but also ensures that your
 * manual pages are consistently and efficiently handled across different platforms.
 * 
 * By leveraging the ManPage struct, your development team can achieve greater efficiency, maintainability,
 * and clarity in managing application documentation, ultimately contributing to a more robust and
 * user-friendly software experience.
 * -------------------------------------------------------------------------------------------------------- */
typedef struct {
    char *filename;
    char *manual;
} ManPage;

const char *FILE_EXTENTION = ".man";

/* --------------------------------------------------------------------------------------------------------
 * By encapsulating the process of appending formatted content within this method, we ensure a seamless and
 * efficient way to dynamically build strings. This method not only enhances the maintainability and
 * readability of your codebase, but also guarantees that formatted content is appended consistently and
 * effectively.
 * 
 * Adopting the append_format method will streamline your string manipulation tasks, fostering better
 * organization and flexibility, and ultimately contributing to a more polished and efficient product.
 * 
 * @param char **dest - The destination string to which formatted content will be appended.
 * @param const char *format - The format string.
 * @param ... - Additional arguments to format and append to the destination string.
 * -------------------------------------------------------------------------------------------------------- */
void append_format(char **dest, const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    va_list args_copy;
    va_copy(args_copy, args);
    int size = vsnprintf(NULL, 0, format, args_copy) + 1;
    va_end(args_copy);

    char *temp = malloc(size);
    if (temp == NULL) {
        perror("malloc failed");
        va_end(args);
        return;
    }
    
    vsprintf(temp, format, args);
    va_end(args);

    if (*dest == NULL) {
        *dest = malloc(size);
        if (*dest == NULL) {
            perror("malloc failed");
            free(temp);
            return;
        }
        strcpy(*dest, temp);
    } else {
        *dest = realloc(*dest, strlen(*dest) + size);
        if (*dest == NULL) {
            perror("realloc failed");
            free(temp);
            return;
        }
        strcat(*dest, temp);
    }
    
    free(temp);
}

/* -----------------------------------------------------------------------------------------------------
 * By encapsulating the creation of manual pages within this method, we ensure a seamless and efficient
 * process for generating documentation. This not only enhances the maintainability and readability of
 * your codebase, but also guarantees that manual pages are consistently formatted and easily accessible.
 * 
 * Adopting the create_manpage method will streamline your documentation process, fostering better
 * organization and accessibility, and ultimately contributing to a more polished and user-friendly product.
 * 
 * @param char *filenameIn - The filename to the ManPage text file.
 * @param char *manualIn - The content of the ManPage text.
 * ----------------------------------------------------------------------------------------------------- */
void create_manpage(char *filenameIn, char *manualIn, int major, int minor) {

    ManPage mp;

    mp.filename = NULL;
    append_format(&mp.filename, _home());
    append_format(&mp.filename, PATH);
    append_format(&mp.filename, filenameIn);
    append_format(&mp.filename, FILE_EXTENTION);

    mp.manual = NULL;
    append_format(&mp.manual, manualIn);

    if(!doesFileExist(mp.filename, major, minor)) {
        
        FILE *file = fopen(mp.filename, "w");
    
        if (file == NULL) {
            printf("Error opening file!\n");
            return;
        }

        fprintf(file, "%s", mp.manual);

        // Now we can close the file.
        fclose(file);

    }

    strcat(command, mp.filename);
    system(command);
}

/* ----------------------------------------------------------------------------------------------------
 * By encapsulating the detection of help command triggers within this method, we ensure a seamless and
 * efficient way to handle user requests for help or usage information. This method not only enhances
 * the maintainability and readability of your codebase but also guarantees that help commands are
 * consistently and accurately recognized.
 * 
 * Adopting the isHelpTriggered method will streamline your application's user interaction processes,
 * fostering better user experience and accessibility, and ultimately contributing to a more
 * user-friendly product.
 * 
 * @param int argcIn - The number of command-line arguments.
 * @param char *argvIn - The first string of the command-line arguments array.
 * @return int - Returns 1 if the help command is triggered, and 0 otherwise.
 * ---------------------------------------------------------------------------------------------------- */
int isHelpTriggered(int argcIn, char *argvIn) {
    
    /* ------------------------------------------------------- 
     * Help / Manpage is trigger if program call are like:
     *  - <program> or
     *  - <program> -h or
     *  - <program> -H or
     *  - <program> -help or
     *  - <program> -Help
     * ------------------------------------------------------- */
    return (argcIn == 1 ||
        strcmp(argvIn, "-h") == 0 ||
        strcmp(argvIn, "-H") == 0 ||
        strcmp(argvIn, "-help") == 0 ||
        strcmp(argvIn, "-Help") == 0);
}

/* ***************************************END INTEGRATION************************************************* */

#define MAX_LINE_LENGTH 256

/* -------------------------------------------------------------------------------------------------------
 * The print_help function is our top-notch guidance feature, crafted to provide users with clear,
 * intuitive instructions for leveraging our command-line utility within the Windows Command Prompt
 * environment. Think of it as your personal guide, always ready to offer step-by-step explanations for
 * each option available in the tool.
 *
 * When users invoke this function, they receive a straightforward and well-structured breakdown of all
 * possible commands, ensuring they can navigate directory listings with ease and efficiency. With familiar
 * flags like -l and -a, the experience feels intuitive, mirroring the user-friendly nature of Unix systems,
 * but optimized for the Windows Command Prompt.
 *
 * In essence, show_help embodies our commitment to user empowerment, making sure every feature is accessible
 * and easily understood. This minimizes learning curves and maximizes productivity. This function is the
 * cornerstone of our user-centric approach, providing instant, reliable support whenever needed. Welcome to
 * a new era of intuitive, efficient command-line interaction.
 * ------------------------------------------------------------------------------------------------------- */
void print_help() {

    // Version control implemented
    Version v = create_version(0, 18);
    
    // The buffer is needed to write
    // the correct formated version number.
    char buffer[6];
    
    // Format the buffer with the
    // correct version number.
    to_string(v, buffer);

    char *manpage = NULL;

    // Write the ManPage style help file.
    append_format(&manpage, "NAME\n");
    append_format(&manpage, "       pmake Version: %s\n", buffer);
    append_format(&manpage, "       Our custom \"pmake\" program is designed to empower developers by\n");
    append_format(&manpage, "       streamlining the build process with simplicity and efficiency.\n");
    append_format(&manpage, "       Tailored specifically for flexibility, it reads configuration files,\n");
    append_format(&manpage, "       interprets instructions, and executes commands to compile and build\n");
    append_format(&manpage, "       projects seamlessly. By offering an intuitive and robust solution,\n");
    append_format(&manpage, "       our pmake program not only enhances productivity but also ensures\n");
    append_format(&manpage, "       consistency across various development environments. This tool is\n");
    append_format(&manpage, "       an essential asset for any development team, enabling faster\n");
    append_format(&manpage, "       turnaround times and improved project management.\n");
    append_format(&manpage, "\n");
    append_format(&manpage, "SYNOPSIS\n");
    append_format(&manpage, "       pmake <makefile>\n");
    append_format(&manpage, "       pmake <-h\\-help\\-H\\-Help>\n");
    append_format(&manpage, "\n");
    append_format(&manpage, "DESCRIPTION\n");
    append_format(&manpage, "       <makefile> The name of the makefile with the build instructions\n");
    append_format(&manpage, "       to be processed.\n");
    append_format(&manpage, "\n");
    append_format(&manpage, "           Example Makefile myproject.makefile:\n");
    append_format(&manpage, "           ---------------------------------------\n");
    append_format(&manpage, "           # Define the compiler and flags\n");
    append_format(&manpage, "           comp=gcc\n");
    append_format(&manpage, "           cflags=-Wall -Wextra -std=c11 (optional)\n");
    append_format(&manpage, "\n");
    append_format(&manpage, "           # Define the target executable or object or shared.\n");
    append_format(&manpage, "           target=exec or\n");
    append_format(&manpage, "           target=shared\n");
    append_format(&manpage, "           target=obj\n");
    append_format(&manpage, "\n");
    append_format(&manpage, "           # Define the source files\n");
    append_format(&manpage, "           src=main.c (optional)\n");
    append_format(&manpage, "\n");
    append_format(&manpage, "           # Define the project name\n");
    append_format(&manpage, "           project=myproject\n");
    append_format(&manpage, "\n");
    append_format(&manpage, "           # Define the library files\n");
    append_format(&manpage, "           libs=../mylibs/lib1.o ../mylibs/lib2.o\n");
    append_format(&manpage, "           ---------------------------------------\n");
    append_format(&manpage, "\n");
    append_format(&manpage, "       -h, -help -H -Help\n");
    append_format(&manpage, "              Display this help and exit.\n");
    append_format(&manpage, "\n");
    append_format(&manpage, "AUTHOR\n");
    append_format(&manpage, "       Patrik Eigenmann (p.eigenmann@gmx.net)\n");
    append_format(&manpage, "\n");
    append_format(&manpage, "COPYRIGHT\n");
    append_format(&manpage, "      Copyright 2024 Free Software Foundation, Inc. License GPLv3+:\n");
    append_format(&manpage, "      GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n");
    append_format(&manpage, "      This is free software: you are free to change and redistribute it.\n");
    append_format(&manpage, "      There is NO WARRANTY, to the extent permitted by law.\n");

    // Create the manpage in the file
    create_manpage("pmake", manpage, v.major, v.minor);

    // Free up the memory.
    free(manpage);
}

/* ------------------------------------------------------------------------------------------------
 * The process_makefile function is a pivotal component of our custom "Make" program, designed to
 * streamline the build process by reading and executing commands from a specified makefile. This
 * function ensures efficient parsing and execution of build instructions, enhancing productivity
 * and simplifying project management for developers.
 *
 * @param filename  The name of the makefile to be processed. This file contains the build instructions
 *                  to be executed.
 * ------------------------------------------------------------------------------------------------- */
void process_makefile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    char line[MAX_LINE_LENGTH];
    char comp[50] = "gcc";
    char cflags[100] = "";
    char target[10] = "exec";
    char project[50] = "";
    char src[500] = "";
    char* libs = NULL;
    
    int isLibs = -1;

    while (fgets(line, sizeof(line), file)) {
        
        // Trim newline character
        line[strcspn(line, "\n")] = 0;
        
        // Skip empty lines or comments
        if (line[0] == '\0' || line[0] == '#') continue;
        
        // Parse the makefile variables
        if (strncmp(line, "comp=", 5) == 0) {
            isLibs = -1;
            strcpy(comp, line + 5);
        } else if (strncmp(line, "cflags=", 7) == 0) {
            isLibs = -1;
            strcpy(cflags, line + 7);
        } else if (strncmp(line, "target=", 7) == 0) {
            isLibs = -1;
            strcpy(target, line + 7);
        } else if (strncmp(line, "project=", 8) == 0) {
            isLibs = -1;
            strcpy(project, line + 8);
        } else if (strncmp(line, "src=", 4) == 0) {
            isLibs = -1;
            strcpy(src, line + 4);
        } else if (strncmp(line, "libs=", 5) == 0 || isLibs) {
            isLibs = 1;
            if(strncmp(line, "libs=", 5) == 0) 
                append_format(&libs, "%s ", line + 5);
            else
                append_format(&libs, "%s ", line);
        }
    }
    
    fclose(file);

    char *command = NULL;

    // Step 1: Start with the compiler in the command.
    append_format(&command, "%s ", comp);

    // Step 2: Set the flags if there are any.
    if(cflags[0] != '\0')
        append_format(&command, "%s ", cflags);

    // Step 3: Check the target, is it object, exec or shared.
    if(target[0] == 's')
        append_format(&command, "-shared ");
    else if(target[0] == 'o')
        append_format(&command, "-c ");

    if(src[0] != '\0')
        append_format(&command, "%s ", src);
    else
        append_format(&command, "%s.c ", project);

    if(libs[0] != '\0')
        append_format(&command, "%s ", libs);


    #ifdef _WIN32
        // Windows version
        if(target[0] == 's')
            append_format(&command, "-o %s.dll", project);
        else if(target[0] == 'o')
            append_format(&command, "-o %s.o", project);
        else
            append_format(&command, "-o %s", project);
    #else
        // Unix version
        if(target[0] == 's')
            append_format(&command, "-o %s.so", project);
        else if(target[0] == 'o')
            append_format(&command, "-o %s.o", project);
        else
            append_format(&command, "-o %s", project);

    #endif

    printf("Compiling command:\n");
    printf("%s\n", command);

    // Execute the command
    int result = system(command);

    free(libs);
    free(command);

    if (result != 0) {
        fprintf(stderr, "Command failed: %s\n", command);
        exit(EXIT_FAILURE);
    }
}

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
int main (int argc, char **argv) {

    // Check if the help is triggered.
    if(isHelpTriggered(argc, argv[1])) {
        print_help();
        return 1;
    }

    process_makefile(argv[1]);
    return EXIT_SUCCESS;
}