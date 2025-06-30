/* ****************************************************************************************************
 * debug.h — A minimal toggle for internal printf-style debugging.
 *
 * If DEBUG is defined at compile time, debug(...) expands to printf(...).
 * If not, it compiles to nothing — zero overhead, zero output.
 * It’s how I talk to myself during development without cluttering release builds.
 *
 * Define with -DDEBUG to activate.
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

// Toggle debug mode based on whether DEBUG is defined at compile time.
// This keeps the debug() macro lean — either fully active or completely silent.
// One flag, no runtime cost, no surprises.
#ifdef DEBUG
#define DEBUG_MODE 1
#else
#define DEBUG_MODE 0
#endif

// Conditionally print debug messages if DEBUG_MODE is active.
// Wraps printf(...) in a macro so it disappears completely in release builds.
// Usage stays clean — just call debug(...) like you would printf(...).
#define debug(...) \
    do { if (DEBUG_MODE) printf(__VA_ARGS__); } while (0)
#endif