@echo off
setlocal

for /l %%i in (1, 1, 400) do (
    echo Running iteration %%i
    b.exe 1000 10000 2 0.5 0.25 0.25
)

endlocal