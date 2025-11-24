@ECHO OFF

rem cl.exe /nologo /Ox /MT /W0 /GS- /DNDEBUG /Tp *.c /link /OUT:vcpersist.exe /SUBSYSTEM:CONSOLE /MACHINE:x64
cl.exe /nologo /Ox /MT /W0 /GS- /DNDEBUG /Tp *.c /link /OUT:vcpersist.exe /SUBSYSTEM:WINDOWS /MACHINE:x64
del *.obj