@echo off

FOR %%d IN (lol4\*.user) DO (
   
    if not %%~nxd==lol4.default.user (
    echo Overwriting %%d...
    copy "lol4\lol4.default.user" "%%d"
    )  
)

FOR %%d IN (lol4edit\*.user) DO (
   
    if not %%~nxd==lol4edit.default.user (
    echo Overwriting %%d...
    copy "lol4edit\lol4edit.default.user" "%%d"
    )  
)