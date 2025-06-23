@echo off
echo Marking local files as assumed-unchanged...

git update-index --assume-unchanged Makefile
git update-index --assume-unchanged CMakeLists.txt
git update-index --assume-unchanged pmake.pmake
git update-index --assume-unchanged bin/pmake
git update-index --assume-unchanged build/
git update-index --assume-unchanged README.md
git update-index --assume-unchanged LICENSE
git update-index --assume-unchanged scripts/set_workspace.sh
git update-index --assume-unchanged scripts/set_workspace.cmd
git update-index --assume-unchanged scripts/install.sh
git update-index --assume-unchanged scripts/install.cmd

echo Done. Git will stop tracking changes to these files in this working copy.