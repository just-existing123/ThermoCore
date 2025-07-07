@echo off
echo ========================================
echo  HEAT EXCHANGER PROGRAM DEMO
echo ========================================
echo.
echo This demo will test the heat exchanger program
echo with sample water-to-water data.
echo.
echo Building the program...
call build_and_run.bat
echo.
echo Build complete!
echo.
echo Instructions for manual testing:
echo 1. Run heat_exchanger.exe
echo 2. Choose option 1 (Custom input)
echo 3. Use the values from sample_test_data.txt
echo.
echo Expected outputs:
echo - temperature_profile.csv (temperature distribution)
echo - heat_transfer_summary.txt (analysis summary)  
echo - convergence_study.csv (if convergence study is run)
echo.
echo Press any key to exit...
pause > nul
