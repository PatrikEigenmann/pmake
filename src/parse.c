/* ****************************************************************************************************
 * Description
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

static char *dupstr(const char *s) {
    return s ? strdup(s) : NULL;
}

static void set_default_if_null(char **field, const char *fallback) {
    if (*field == NULL) *field = strdup(fallback);
}

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
