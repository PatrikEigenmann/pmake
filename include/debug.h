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
#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define DEBUG_MODE 1
#else
#define DEBUG_MODE 0
#endif

#define debug(...) \
    do { if (DEBUG_MODE) printf(__VA_ARGS__); } while (0)

#endif
