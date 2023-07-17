@ECHO OFF
mkdir %1
cd %1
mkdir output
xcopy /E ..\hello_world\test-framework test-framework\
xcopy /E ..\hello_world\.vscode .vscode\
xcopy ..\hello_world\build.bat .
echo. 2>main.c
echo. 2>%1.c
echo. 2>%1.h
cd ..
Code %1