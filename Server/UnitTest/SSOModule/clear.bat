

ssodaemon.exe -UnregServer
ssosvrdaemon.exe -UnregServer
regsvr32 ssoagent.dll /u /s
regsvr32 ssosvragent.dll /u /s
regsvr32 ssoipc.dll /u /s

del %windir%\\system32\\ssodaemon.exe /q /f
del %windir%\\system32\\ssoagent.dll /q /f
del %windir%\\system32\\ssoipc.dll /q /f
del %windir%\\system32\\ssosvrdaemon.exe /q /f
del %windir%\\system32\\ssosvragent.dll /q /f
del %windir%\\system32\\CryptoHelper.dll /q /f
