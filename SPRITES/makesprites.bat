for /f %%a IN ('dir /b "spriteconfigs\*.spritecfg"') do call spriteconvert.exe spriteconfigs\%%a
move *.sprite compiled