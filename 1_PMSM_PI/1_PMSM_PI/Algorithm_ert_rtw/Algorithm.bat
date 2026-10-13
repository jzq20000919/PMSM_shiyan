
set MATLAB=D:\Matlab\R2023a

cd .

if "%1"=="" ("D:\Matlab\R2023a\bin\win64\gmake"  -f Algorithm.mk all) else ("D:\Matlab\R2023a\bin\win64\gmake"  -f Algorithm.mk %1)
@if errorlevel 1 goto error_exit

exit /B 0

:error_exit
echo The make command returned an error of %errorlevel%
exit /B 1