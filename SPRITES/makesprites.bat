for /f %%a IN ('dir /b "spriteconfigs\*.spritecfg"') do call imgconvert.exe spriteconfigs\%%a
move *.sprite compiled