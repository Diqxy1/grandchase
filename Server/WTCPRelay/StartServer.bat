@echo off
:Loop
svn update
WTRServer.exe
goto Loop
