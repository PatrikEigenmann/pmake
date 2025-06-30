/* ****************************************************************************************************
 * manpage.h - This header provides basic facilities for generating and displaying manual/help text
 * within a command-line application. It includes data structures for storing help content and functions
 * to print documentation or detect common help flags.
 *
 * Designed for portability and minimal dependencies — suitable for lightweight tools or
 * utilities that need self-contained usage guidance.
 * ----------------------------------------------------------------------------------------------------
 * Author:      Patrik Eigenmann
 * eMail:       p.eigenmann@gmx.net
 * GitHub:      www.github.com/PatrikEigenmann/pmake
 * ----------------------------------------------------------------------------------------------------
 * Change Log:
 * Sun 2025-06-23 File created.                                                         Version: 00.01
 * **************************************************************************************************** */
#ifndef MANPAGE_H
#define MANPAGE_H

#include "version.h"

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
void create_manpage(char *filenameIn, char *manualIn, int major, int minor);

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
int isHelpTriggered(int argcIn, char *argvIn);

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
 * 
 * @param Version v - The version of the program to be displayed in the help.
 * ------------------------------------------------------------------------------------------------------- */
void print_help(Version v);

#endif