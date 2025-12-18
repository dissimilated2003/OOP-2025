@echo off

SET MyProgram="..\x64\Debug\invert.exe"

REM No path argument security
if %MyProgram%=="" (
        echo Please specify path to program
        exit /B 1
)

REM Test 1: Empty file
echo Test 1: Empty file
%MyProgram% empty_matrix.txt | findstr /C:"Invalid matrix format"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 1 passed


REM Test 2: Zero determinant (with extra blanks in lines end)
echo Test 2: Matrix with determinant 0
%MyProgram% zero_det.txt | findstr /C:"Matrix is degenerate! Non-invertible"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 2 passed

REM Test 3: Reading not exist file
echo Test 3: File not found
%MyProgram% not_found.txt | findstr /C:"Failed to open input file!"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 3 passed

REM Test 4: Spacing matrix between like: 1 2 -3, no tabs
echo Test 4: Invalid matrix form
%MyProgram% blank_matrix.txt | findstr /C:"Invalid matrix format"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 4 passed

REM Test 5: "Na" in matrix, vinegret
echo Test 5: Invalid matrix form
%MyProgram% vinegret_matrix.txt | findstr /C:"Invalid matrix format"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 5 passed

REM Test 6: "a" in matrix
echo Test 6: Total invalid matrix
%MyProgram% sym_matrix.txt | findstr /C:"Invalid matrix format"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 6 passed

REM Test 7: Lower than three strings in file
echo Test 7: Invalid matrix form
%MyProgram% lower_lines_matrix.txt | findstr /C:"Invalid matrix format"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 7 passed

REM Test 8: Upper than three strings in file
echo Test 8: Invalid matrix form
%MyProgram% upper_lines_matrix.txt | findstr /C:"Invalid matrix format"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 8 passed

REM Test 9: Lower than three columns in file
echo Test 9: Invalid matrix form
%MyProgram% lower_columns_matrix.txt | findstr /C:"Invalid matrix format"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 9 passed

REM Test 10: Upper than three columns in file
echo Test 10: Invalid matrix form
%MyProgram% upper_columns_matrix.txt | findstr /C:"Invalid matrix format"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 10 passed

REM Test 11: Param -h
echo Test 11: Helper
%MyProgram% -h | findstr /C:"Usage:"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 11 passed

REM Test 12: integer numbers matrix inversion
echo Test 12: Correct matrix inversion
%MyProgram% matrix1.txt | findstr /s /C:"0.483" /C:"-0.526" /C:"0.242" /C:"0.061" /C:"-0.033" /C:"0.008" /C:"-0.195" /C:"0.207" /C:"-0.065"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 12 passed

REM Test 13: float numbers matrix inversion
echo Test 13: Correct matrix inversion
%MyProgram% matrix2.txt | findstr /s /C:"0.010" /C:"0.119" /C:"0.002" /C:"0.007" /C:"0.071" /C:"-0.082" /C:"-0.011" /C:"0.004" /C:"-0.003"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 13 passed

REM Test 14: float & integer nums -> negative & positive + blanks
echo Test 14: Correct matrix inversion
%MyProgram% matrix3.txt | findstr /s /C:"0.024" /C:"0.127" /C:"-0.016" /C:"0.090" /C:"-0.113" /C:"0.014" /C:"0.113" /C:"-0.153" /C:"0.034"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 14 passed

REM Test 15: Spaces in strings (beg & end)
echo Test 15: Correct matrix inversion
%MyProgram% matrix4.txt | findstr /s /C:"0.346" /C:"0.015" /C:"-0.087" /C:"-0.112" /C:"-0.011" /C:"0.049" /C:"-0.041" /C:"0.011" /C:"0.010"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 15 passed

REM Test 16: many tabs
echo Test 16: Correct matrix inversion
%MyProgram% many_tabs.txt | findstr /C:"Invalid matrix format"
if %ERRORLEVEL% NEQ 0 (
    goto err
)
echo Test 16 passed

echo All tests passed successfully
exit /B 0

:err
echo Test failed
exit /B 1