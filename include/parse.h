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
#ifndef PARSE_H
#define PARSE_H

typedef struct {
    char *comp;
    char *flags;
    char *target;
    char *project;
    char *bin;
    char *src;
    char *libs;
} Makefile;

Makefile *parse(const char *filename, char **errmsg);
void run(const Makefile *mf, char **errmsg);
void free_makefile(Makefile *mf);
char *normalize_filename(const char *input);
#endif
