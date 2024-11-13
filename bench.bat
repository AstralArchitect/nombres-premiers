@echo off

rem Set the program to measure
set program_to_measure=.\builddir\main.exe 1000000 true

rem Start the timer
set start_time=%time%

rem Run the program
%program_to_measure%

rem Stop the timer
set end_time=%time%

rem Calculate the elapsed time in milliseconds
set /a start_time_ms=%start_time:~0,2%*60*60*1000+%start_time:~3,2%*60*1000+%start_time:~6,2%*1000+%start_time:~9,2%
set /a end_time_ms=%end_time:~0,2%*60*60*1000+%end_time:~3,2%*60*1000+%end_time:~6,2%*1000+%end_time:~9,2%
set /a elapsed_time_ms=%end_time_ms%-%start_time_ms%

echo Execution time: %elapsed_time_ms% milliseconds