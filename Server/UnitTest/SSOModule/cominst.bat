

regsvr32 ssoipc.dll /s

ssodaemon.exe -RegServer
regsvr32 ssoagent.dll /s

ssosvrdaemon.exe -RegServer
regsvr32 ssosvragent.dll /s

