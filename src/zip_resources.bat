@echo off

REM SETLOCAL assures environment variables created in a batch file are not exported to its calling environment
setlocal

set BUILD_RESDIR=build28\share\emblocks

if not exist build28 md build28\
if not exist build28\share md build28\share\
if not exist %BUILD_RESDIR% md %BUILD_RESDIR%\
if not exist %BUILD_RESDIR%\lexers md %BUILD_RESDIR%\lexers\
if not exist %BUILD_RESDIR%\images md %BUILD_RESDIR%\images\
if not exist %BUILD_RESDIR%\images\settings md %BUILD_RESDIR%\images\settings\
if not exist %BUILD_RESDIR%\images\16x16 md %BUILD_RESDIR%\images\16x16\
if not exist %BUILD_RESDIR%\images\codecompletion md %BUILD_RESDIR%\images\codecompletion\
if not exist %BUILD_RESDIR%\plugins md %BUILD_RESDIR%\plugins\
if not exist %BUILD_RESDIR%\templates md %BUILD_RESDIR%\templates\
if not exist %BUILD_RESDIR%\templates\wizard md %BUILD_RESDIR%\templates\wizard\
if not exist %BUILD_RESDIR%\scripts md %BUILD_RESDIR%\scripts\

set ZIPCMD=zip

echo Compressing core UI resources
%ZIPCMD% -jqu9 %BUILD_RESDIR%\resources.zip src\resources\*.xrc > nul
%ZIPCMD% -jqu9 %BUILD_RESDIR%\manager_resources.zip sdk\resources\*.xrc sdk\resources\images\*.png > nul
%ZIPCMD% -jqu9 %BUILD_RESDIR%\start_here.zip src\resources\start_here\*.html src\resources\start_here\*.png > nul
echo Compressing plugins UI resources
%ZIPCMD% -jqu9 %BUILD_RESDIR%\astyle.zip plugins\astyle\resources\manifest.xml plugins\astyle\resources\*.xrc > nul
%ZIPCMD% -jqu9 %BUILD_RESDIR%\autosave.zip plugins\autosave\manifest.xml plugins\autosave\*.xrc > nul
%ZIPCMD% -jqu9 %BUILD_RESDIR%\classwizard.zip plugins\classwizard\resources\manifest.xml plugins\classwizard\resources\*.xrc > nul
%ZIPCMD% -jqu9 %BUILD_RESDIR%\codecompletion.zip plugins\codecompletion\resources\manifest.xml plugins\codecompletion\resources\*.xrc > nul
%ZIPCMD% -jqu9 %BUILD_RESDIR%\compiler.zip plugins\compilergcc\resources\manifest.xml plugins\compilergcc\resources\*.xrc > nul
%ZIPCMD% -jqu9 %BUILD_RESDIR%\debugger.zip plugins\debuggergdb\resources\manifest.xml plugins\debuggergdb\resources\*.xrc > nul
%ZIPCMD% -jqu9 %BUILD_RESDIR%\defaultmimehandler.zip plugins\defaultmimehandler\resources\manifest.xml plugins\defaultmimehandler\resources\*.xrc > nul
%ZIPCMD% -jqu9 %BUILD_RESDIR%\openfileslist.zip plugins\openfileslist\manifest.xml > nul
%ZIPCMD% -jqu9 %BUILD_RESDIR%\projectsimporter.zip plugins\projectsimporter\resources\manifest.xml plugins\projectsimporter\resources\*.xrc > nul
%ZIPCMD% -jqu9 %BUILD_RESDIR%\scriptedwizard.zip plugins\scriptedwizard\resources\manifest.xml > nul
%ZIPCMD% -jqu9 %BUILD_RESDIR%\todo.zip plugins\todo\resources\manifest.xml plugins\todo\resources\*.xrc > nul
%ZIPCMD% -jqu9 %BUILD_RESDIR%\xpmanifest.zip plugins\xpmanifest\manifest.xml > nul
%ZIPCMD% -jqu9 %BUILD_RESDIR%\abbreviations.zip plugins\abbreviations\resources\manifest.xml plugins\abbreviations\resources\*.xrc > nul
echo Packing core UI bitmaps
cd src\resources
%ZIPCMD% -0 -qu ..\..\%BUILD_RESDIR%\resources.zip images\*.png images\16x16\*.png > nul
cd ..\..\sdk\resources
%ZIPCMD% -0 -qu ..\..\%BUILD_RESDIR%\manager_resources.zip images\*.png images\16x16\*.png > nul
echo Packing plugins UI bitmaps
cd ..\..\plugins\compilergcc\resources
%ZIPCMD% -0 -qu ..\..\..\%BUILD_RESDIR%\compiler.zip images\*.png images\16x16\*.png > nul
cd ..\..\..\plugins\debuggergdb\resources
%ZIPCMD% -0 -qu ..\..\..\%BUILD_RESDIR%\debugger.zip images\*.png images\16x16\*.png > nul
cd ..\..\..

echo Copying files
xcopy /D /y sdk\resources\lexers\lexer_* %BUILD_RESDIR%\lexers > nul
xcopy /D /y src\resources\images\*.png %BUILD_RESDIR%\images > nul
xcopy /D /y src\resources\images\settings\*.png %BUILD_RESDIR%\images\settings > nul
xcopy /D /y src\resources\images\16x16\*.png %BUILD_RESDIR%\images\16x16 > nul
xcopy /D /y plugins\codecompletion\resources\images\*.png %BUILD_RESDIR%\images\codecompletion > nul
echo Makefile.am > excludes.txt
echo Makefile.in >> excludes.txt
echo \.svn\ >> excludes.txt
echo *.gdb >> excludes.txt
xcopy /D /y /s plugins\scriptedwizard\resources\* %BUILD_RESDIR%\templates\wizard /EXCLUDE:excludes.txt >nul
xcopy /D /y scripts\* %BUILD_RESDIR%\scripts /EXCLUDE:excludes.txt > nul
del excludes.txt
xcopy /D /y tips.txt %BUILD_RESDIR% > nul
