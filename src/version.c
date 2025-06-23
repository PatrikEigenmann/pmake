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
#include "version.h"

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