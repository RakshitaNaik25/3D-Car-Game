@echo off
echo Compiling GBPS Car Race (New Version)...
gcc -o NewGame.exe *.c "libs\SOIL\Simple-OpenGL-Image-Library-master\src\*.c" -I. -I"libs\freeglut\include" -I"libs\SOIL\Simple-OpenGL-Image-Library-master\src" -L. -lfreeglut -lopengl32 -lglu32 -lwinmm -DGLUT_DISABLE_ATEXIT_HACK
if %errorlevel% neq 0 (
    echo Compilation Failed!
    exit /b %errorlevel%
)
echo Compilation Successful!
echo Running Game...
start NewGame.exe
