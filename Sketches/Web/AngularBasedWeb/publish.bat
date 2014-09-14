@ECHO OFF

rem optimize with require.js optimizer
node r.js -o app.build.js

rem remove unnecessary files
rmdir /S /Q Publish\App_Data
del /Q Publish\Web.Debug.Config

pause
