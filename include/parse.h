/* ****************************************************************************************************
 * parse.h - This header defines a minimal interface for parsing build configuration files into
 * structured data. It includes a Makefile struct to store parsed values and functions to normalize
 * filenames, execute builds, and release associated resources. Designed for modular use in small CLI
 * tools.
 * ----------------------------------------------------------------------------------------------------
 * Author:      Patrik Eigenmann
 * eMail:       p.eigenmann@gmx.net
 * GitHub:      www.github.com/PatrikEigenmann/pmake
 * ----------------------------------------------------------------------------------------------------
 * Change Log:
 * Sun 2025-06-22 File created.                                                         Version: 00.01
 * **************************************************************************************************** */
#ifndef PARSE_H
#define PARSE_H

// This struct represents the parsed contents of a simple build configuration file. Each field stores
// a relevant directive: the compiler to use, flags to pass, target type (e.g., executable, shared
// library), and paths for source, output, and optional libraries. Designed for clarity and direct use
// in small CLI tools.
typedef struct {
    char *comp;
    char *flags;
    char *target;
    char *project;
    char *bin;
    char *src;
    char *libs;
} Makefile;

// --------------------------------------------------------------------------------
// Parse a build configuration file and return a populated Makefile struct.
// If parsing fails, errmsg will point to a dynamically allocated error message.
//
// @param filename  Path to the config file to parse
// @param errmsg    Pointer to store error message (set to NULL on success)
// @return          Allocated Makefile* on success, NULL on failure
// --------------------------------------------------------------------------------
Makefile *parse(const char *filename, char **errmsg);

// --------------------------------------------------------------------------------
// Execute the build process based on the provided Makefile configuration.
// Translates the struct fields into a compiler command and invokes it.
//
// On failure, errmsg will point to an allocated string describing the issue.
// Caller is responsible for freeing errmsg if set.
//
// @param mf      Parsed build configuration
// @param errmsg  Output pointer for error messages (set to NULL on success)
// --------------------------------------------------------------------------------
void run(const Makefile *mf, char **errmsg);

// --------------------------------------------------------------------------------
// Free all dynamically allocated memory associated with a Makefile struct.
// Safely deallocates each field and then the struct itself.
//
// @param mf  Pointer to a Makefile previously returned by parse()
//            (or NULL, in which case nothing happens)
// --------------------------------------------------------------------------------
void free_makefile(Makefile *mf);

// --------------------------------------------------------------------------------
// Convert a user-supplied filename into a normalized format.
// This may add or adjust extensions, strip leading paths, or enforce consistency
// in naming conventions. Useful for resolving shorthand or ambiguous filenames.
//
// @param input  Raw filename or command-line argument
// @return       Dynamically allocated, normalized filename string (caller frees)
// --------------------------------------------------------------------------------
char *normalize_filename(const char *input);
#endif
