for /f %%a IN ('dir /b "bitmaps\*.bmp"') do call imgconvert.exe bitmaps\%%a
move bitmaps\*.osimg compiled