# ------------------------------------------------------------------------------------
# makefile for the tool pmake. pmake is a tool that reads instructions you give it
# about how to use a compiler of your choice and then automatically runs the compiler
# to build your software projects. Imagine it as a smart assistant that knows exactly
# how to compile and prepare all the pieces of your software, ensuring everything fits
# together correctly without you having to manually run each step. This automation makes
# the development process faster and reduces the chance of errors.
# ------------------------------------------------------------------------------------
# Author: Patrik Eigenmann (p.eigenmann@gmx.net)
# ------------------------------------------------------------------------------------
# Change Log:
# Sat 2024-11-16 File created.                                          Version: 00.01
# Sun 2024-11-17 Changed the configuration to fit the project.          Version: 00.02
# ------------------------------------------------------------------------------------

# The compiler used to compile the project. Common compilers used in both Windows and
# MacOS are gcc or clang.
# comp=clang
comp=gcc

# If the project needs to be compile with specific compiler flags (optional).
cflags=

# Targed binary of the project, either an executive or an object/library file.
# target=obj
target=exec

# Project name, if there is no src file defined, pmake assumes that the .c
# file has the same name as the project name.
project=pmake

# Source file: Use it if the source filename is different than the project name.
# As example main.c but project is cal.exe.
src=pmake.c

# Libraries to add, or already compiled .o files. Make sure they are properly included.
libs=