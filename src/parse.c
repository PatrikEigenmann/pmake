/* ****************************************************************************************************
 * parse.c — Configuration file parser and build command generator. Implements a minimal parser for
 * simple key-value build configuration files. Converts parsed data into a structured Makefile object
 * and assembles platform-aware compiler commands based on the extracted values. Also includes utilities
 * for filename normalization and memory cleanup.
 *
 * Intended for use in standalone CLI tools or as part of lightweight build systems. Does not depend on
 * external libraries or parsing frameworks.
 * ----------------------------------------------------------------------------------------------------
 * Author:      Patrik Eigenmann
 * eMail:       p.eigenmann@gmx.net
 * GitHub:      www.github.com/PatrikEigenmann/pmake
 * ----------------------------------------------------------------------------------------------------
 * Change Log:
 * Sun 2025-06-22 File created.                                                         Version: 00.01
 * **************************************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parse.h"
#include "debug.h"

// --------------------------------------------------------------------------------
// Allocate a duplicate of the given C-string. If the input is NULL, returns NULL
// immediately. Otherwise, dynamically allocates memory and copies the contents
// using strdup(). This function simplifies parsing logic by removing the need for
// repetitive NULL checks around optional fields.
//
// It favors clarity over cleverness — safe, predictable, and in step with the rest
// of the minimalist toolkit you're crafting.
//
// @param s     Pointer to the source string (may be NULL)
// @return      Newly allocated copy of the string, or NULL if input was NULL
// --------------------------------------------------------------------------------
static char *dupstr(const char *s) {
    return s ? strdup(s) : NULL;
}

// --------------------------------------------------------------------------------
// Assign a default value to a field if it is currently NULL. This utility helps
// populate optional configuration fields with fallback values, avoiding scattered
// conditional checks and maintaining cleaner parsing logic. If the provided field
// already points to a value, it is left unchanged. Otherwise, it is dynamically
// allocated using strdup(fallback).
//
// This pattern keeps config loading predictable and reduces coupling between
// user-provided and internally assumed values.
//
// @param field     Pointer to the target field (char**) that may be NULL
// @param fallback  Default string to assign if *field is NULL
// --------------------------------------------------------------------------------
static void set_default_if_null(char **field, const char *fallback) {
    if (*field == NULL) *field = strdup(fallback);
}

// --------------------------------------------------------------------------------
// Parse a build configuration file and return a populated Makefile struct. Opens
// the given file and reads key-value pairs line by line, skipping empty lines and
// comments. Recognized keys include comp, flags (or cflags), target, project, bin,
// src, and libs. If optional fields like comp, bin, or src are not provided, they
// are set to sensible defaults. Unknown keys are ignored silently.
//
// The returned Makefile struct reflects the parsed configuration and can be used to construct
// compiler commands or introspect project metadata. Callers are responsible for freeing the struct
// using free_makefile().
//
// If parsing fails — due to I/O errors or missing required fields (project or target) — the function
// returns NULL and sets errmsg to a heap-allocated message describing the problem.
//
// @param filename  Path to the configuration file
// @param errmsg    Pointer to store an error message if parsing fails (set to NULL on success)
// @return          Pointer to a populated Makefile, or NULL on failure
// --------------------------------------------------------------------------------
Makefile *parse(const char *filename, char **errmsg) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        size_t len = snprintf(NULL, 0, "Could not open file: %s", filename) + 1;
        *errmsg = malloc(len);
        if (*errmsg) snprintf(*errmsg, len, "Could not open file: %s", filename);
        return NULL;
    }

    Makefile *mf = calloc(1, sizeof(Makefile));
    if (!mf) {
        fclose(fp);
        *errmsg = strdup("Memory allocation failed for Makefile structure.");
        return NULL;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        line[strcspn(line, "\r")] = 0;
        debug(">>> LINE: '%s'\n", line);
        if (line[0] == '\0' || line[0] == '#') continue;

        if (strncmp(line, "comp=", 5) == 0) {
            mf->comp    = dupstr(line + 5);
            debug("Parsed compiler directive as: '%s'\n", mf->comp);
        }
        else if (strncmp(line, "flags=", 6) == 0) mf->flags = dupstr(line + 6);
        else if (strncmp(line, "cflags=", 7) == 0) mf->flags = dupstr(line + 7);
        else if (strncmp(line, "target=", 7) == 0) mf->target  = dupstr(line + 7);
        else if (strncmp(line, "project=", 8) == 0)mf->project = dupstr(line + 8);
        else if (strncmp(line, "bin=", 4) == 0)    mf->bin     = dupstr(line + 4);
        else if (strncmp(line, "src=", 4) == 0)    mf->src     = dupstr(line + 4);
        else if (strncmp(line, "libs=", 5) == 0)   mf->libs    = dupstr(line + 5);
    }

    fclose(fp);

    set_default_if_null(&mf->comp, "gcc");
    set_default_if_null(&mf->bin, "./bin");
    set_default_if_null(&mf->src, "./src/main.c");

    if (!mf->project || !mf->target) {
        *errmsg = strdup("Missing required field(s): project or target.");
        free_makefile(mf);
        return NULL;
    }

    return mf;
}

// --------------------------------------------------------------------------------
// Construct and execute a build command using the given Makefile configuration. This
// function assembles a full compiler command string by evaluating the fields in the
// Makefile: compiler (comp), flags, target type, source files, libraries, output
// path, and project name. It applies platform-specific behavior to choose
// appropriate output file extensions based on the target type (e.g., .so, .dll,
// .obj, etc.).
//
// Once constructed, the command is executed using system(). If execution fails,
// errmsg is set to a descriptive message allocated on the heap. On success, errmsg
// remains NULL.
//
// This approach favors predictability and portability over shell scripting — it
// gives the build logic a clear C representation, decoupling it from external
// Makefiles or tooling.
//
// @param mf       Pointer to a fully populated Makefile configuration
// @param errmsg   Output parameter to store an error string if the build fails (NULL on success)
// --------------------------------------------------------------------------------
void run(const Makefile *mf, char **errmsg) {
    char *cmd = malloc(1024);
    if (!cmd) {
        *errmsg = strdup("Memory allocation failed for build command.");
        return;
    }

    cmd[0] = '\0';
    snprintf(cmd, 1024, "%s ", mf->comp);

    if (mf->flags) {
        strncat(cmd, mf->flags, 1023 - strlen(cmd));
        strncat(cmd, " ", 1023 - strlen(cmd));
    }

    if (strcmp(mf->target, "lib") == 0) {
        strncat(cmd, "-shared ", 1023 - strlen(cmd));
    }

    strncat(cmd, mf->src, 1023 - strlen(cmd));
    strncat(cmd, " ", 1023 - strlen(cmd));

    if (mf->libs) {
        strncat(cmd, mf->libs, 1023 - strlen(cmd));
        strncat(cmd, " ", 1023 - strlen(cmd));
    }

    strncat(cmd, "-o ", 1023 - strlen(cmd));
    strncat(cmd, mf->bin, 1023 - strlen(cmd));
    strncat(cmd, "/", 1023 - strlen(cmd));
    strncat(cmd, mf->project, 1023 - strlen(cmd));

#ifdef _WIN32
    if (strcmp(mf->target, "lib") == 0)       strncat(cmd, ".dll", 1023 - strlen(cmd));
    else if (strcmp(mf->target, "obj") == 0)  strncat(cmd, ".obj", 1023 - strlen(cmd));
    else                                      strncat(cmd, ".exe", 1023 - strlen(cmd));
#else
    if (strcmp(mf->target, "lib") == 0)       strncat(cmd, ".so", 1023 - strlen(cmd));
    else if (strcmp(mf->target, "obj") == 0)  strncat(cmd, ".o", 1023 - strlen(cmd));
#endif

    printf("Compiling:\n%s\n", cmd);

    int result = system(cmd);
    if (result != 0) {
        *errmsg = strdup("Build command failed.");
    }

    free(cmd);
}

// --------------------------------------------------------------------------------
// Free all memory associated with a Makefile struct. Releases each dynamically
// allocated field within the struct, followed by the struct itself. This function
// is NULL-safe — if mf is NULL, it returns immediately without error. Intended to
// pair with parse(), ensuring proper cleanup after build configuration usage.
//
// @param mf  Pointer to a Makefile previously returned by parse()
// --------------------------------------------------------------------------------
void free_makefile(Makefile *mf) {
    if (!mf) return;
    free(mf->comp);
    free(mf->flags);
    free(mf->target);
    free(mf->project);
    free(mf->bin);
    free(mf->src);
    free(mf->libs);
    free(mf);
}

// --------------------------------------------------------------------------------
// Normalize a filename by removing its extension and appending ".pmake". This
// utility ensures consistent naming for configuration files by stripping any existing
// extension from the input and replacing it with the standard ".pmake" suffix. If no
// extension is present, the suffix is simply appended.
//
// The returned string is heap-allocated and must be freed by the caller. If allocation
// fails, NULL is returned.
//
// Examples:
//   "build.conf"   → "build.pmake"
//   "project"      → "project.pmake"
//
// @param input   Original filename (must be a valid, null-terminated string)
// @return        Newly allocated normalized string, or NULL on allocation failure
// --------------------------------------------------------------------------------
char *normalize_filename(const char *input) {
    const char *ext = strrchr(input, '.');  // Find last '.'
    size_t len = ext ? (size_t)(ext - input) : strlen(input);

    char *normalized = malloc(len + strlen(".pmake") + 1);
    if (!normalized) return NULL;

    strncpy(normalized, input, len);
    normalized[len] = '\0';
    strcat(normalized, ".pmake");

    return normalized;
}