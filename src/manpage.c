/* ****************************************************************************************************
 * Description
 * ----------------------------------------------------------------------------------------------------
 * Author:      Patrik Eigenmann
 * eMail:       p.eigenmann@gmx.net
 * GitHub:      www.github.com/PatrikEigenmann/pmake
 * ----------------------------------------------------------------------------------------------------
 * Change Log:
 * Sun 2025-06-23 File created.                                                         Version: 00.01
 * **************************************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "manpage.h"
#include "version.h"

#define MAX_LINE_LENGTH 256

const char *FILE_EXTENTION = ".man";

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
    static int doesFileExist(char *filename, int major, int minor) {

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
    static int doesFileExist(char *filename, int major, int minor) {

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
static void append_format(char **dest, const char *format, ...) {
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

    // printf("append_format received: format=\"%s\" → result=\"%s\"\n", format, temp);


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
void print_help(Version v) {

    // Version control implemented
    // Version v = create_version(0, 19);
    
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
    append_format(&manpage, "       pmake <projectname>\n");
    append_format(&manpage, "       pmake <{empty}\\-h\\-help\\-H\\-Help>\n");
    append_format(&manpage, "       pmake --version\n"); 
    append_format(&manpage, "\n");
    append_format(&manpage, "DESCRIPTION\n");
    append_format(&manpage, "       <pmake> The name of the makefile with the build instructions\n");
    append_format(&manpage, "       to be processed.\n");
    append_format(&manpage, "\n");
    append_format(&manpage, "           Example Makefile myproject.pmake:\n");
    append_format(&manpage, "           ---------------------------------------\n");
    append_format(&manpage, "           # Define the compiler and flags\n");
    append_format(&manpage, "           comp=gcc\n");
    append_format(&manpage, "           cflags=-Wall -Wextra -std=c11 (optional)\n");
    append_format(&manpage, "\n");
    append_format(&manpage, "           # Define the target executable or object or shared.\n");
    append_format(&manpage, "           target=exec or\n");
    append_format(&manpage, "           target=shared or\n");
    append_format(&manpage, "           target=obj\n");
    append_format(&manpage, "\n");
    append_format(&manpage, "           # Define the folder for the binaries.\n");
    append_format(&manpage, "           bin=./bin or\n");
    append_format(&manpage, "\n");
    append_format(&manpage, "           # Define the source files\n");
    append_format(&manpage, "           src=./src/main.c (optional)\n");
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
    append_format(&manpage, "       --version\n");
    append_format(&manpage, "              Display the version number and exit.\n");
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