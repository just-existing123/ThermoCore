@echo off
echo Building Heat Exchanger Program...
echo Compiling source files...
g++ -std=c++17 -Wall -Wextra -c main.cpp
if %errorlevel% neq 0 goto buildfailed
g++ -std=c++17 -Wall -Wextra -c fluid_properties.cpp
if %errorlevel% neq 0 goto buildfailed
g++ -std=c++17 -Wall -Wextra -c dimensionless_numbers.cpp
if %errorlevel% neq 0 goto buildfailed
g++ -std=c++17 -Wall -Wextra -c heat_transfer_correlations.cpp
if %errorlevel% neq 0 goto buildfailed
g++ -std=c++17 -Wall -Wextra -c heat_exchanger_geometry.cpp
if %errorlevel% neq 0 goto buildfailed
g++ -std=c++17 -Wall -Wextra -c thermal_calculations.cpp
if %errorlevel% neq 0 goto buildfailed
g++ -std=c++17 -Wall -Wextra -c numerical_solver.cpp
if %errorlevel% neq 0 goto buildfailed
echo Linking executable...
g++ -std=c++17 -Wall -Wextra -o heat_exchanger.exe *.o
if %errorlevel% neq 0 goto buildfailed
echo Build successful!
echo.
echo Running Heat Exchanger Program...
echo.
heat_exchanger.exe
pause
goto end

:buildfailed
echo Build failed!
pause
exit /b 1

:end
