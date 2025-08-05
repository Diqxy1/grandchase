

ssodaemon.exe -UnregServer
ssosvrdaemon.exe -UnregServer
regsvr32 ssoagent.dll /u /s
regsvr32 ssosvragent.dll /u /s
regsvr32 ssoipc.dll /u /s

