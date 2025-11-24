@ECHO OFF

rem cl.exe /nologo /Ox /MT /W0 /GS- /DNDEBUG /Tp *.c /link /OUT:vcmigrate.exe /SUBSYSTEM:CONSOLE

cl.exe /nologo /W0 vcmigrate.c /MT /link /DLL /OUT:vcmigrate.dll


del *.obj