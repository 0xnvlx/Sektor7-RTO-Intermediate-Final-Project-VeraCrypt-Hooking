@ECHO OFF

cl.exe /nologo /W0 vcsniff.c /MT /link /DLL detours\lib.X64\detours.lib user32.lib /OUT:vcsniff.dll

rem cl.exe /nologo /Ox /MT /W0 /GS- /DNDEBUG /Tp hookme.cpp /link /OUT:hookme.exe /SUBSYSTEM:CONSOLE
del *.obj *.lib *.exp
