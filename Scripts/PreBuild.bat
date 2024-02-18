@echo off
setlocal

:: Define directory paths
set "currentDirectory=%~dp0"
set "zipFile=%currentDirectory%..\Libraries\libraries_used.zip"
set "destinationFolder=%currentDirectory%..\Libraries\libraries_used"
set "SDL2_Library_Folder=%destinationFolder%\SDL2\lib\x64"
set "GLEW_Debug_Library_Folder=%destinationFolder%\GLEW\glew-2.2.0\bin\Debug\x64"
set "GLEW_Release_Library_Folder=%destinationFolder%\GLEW\glew-2.2.0\bin\Release\x64"
set "FMOD_Library_Folder=%destinationFolder%\FMOD\lib"
set "Release_Build_Directory=%currentDirectory%..\Bin\Release\x64"
set "Debug_Build_Directory=%currentDirectory%..\Bin\Debug\x64"
powershell write-host -fore cyan "Current Directory \"%currentDirectory%\""
powershell write-host -fore white "Extracting files from \"%zipFile%\""
echo.

:: Create destination folder if it doesn't exist
if not exist "%destinationFolder%" (
    mkdir "%destinationFolder%" && echo.Libraries destination folder created.
)

:: Check if zip file exists
if not exist "%zipFile%" (
    echo.
    echo ERROR: The zip file "%zipFile%" does not exist.
    echo There was a problem.
    exit /b 1
)

:: Remove any existing files and folders in the destination folder
del /s /q "%destinationFolder%"

:: Extract the zip file
powershell.exe -command "Expand-Archive -Path \"%zipFile%\" -DestinationPath \"%destinationFolder%\""

:: Check extraction result
if errorlevel 1 (
    echo.
    echo There was a problem extracting the files.
    exit /b 1
) else (
    echo.
    powershell write-host -fore cyan "Succefully extracted files to \"%destinationFolder%\""
)

:: Create Release_Build_Directory if it doesn't exist
if not exist "%Release_Build_Directory%" (
    mkdir "%Release_Build_Directory%" && echo.Release_Build_Directory created.
)

:: Create Debug_Build_Directory if it doesn't exist
if not exist "%Debug_Build_Directory%" (
    mkdir "%Debug_Build_Directory%" && echo.Debug_Build_Directory created.
)

:: Copy SDL2 DLL fileS to build directories
if exist "%SDL2_Library_Folder%\*.dll" (
    echo Copying SDL2 DLL files to Bin\Release\x64...
    copy "%SDL2_Library_Folder%\*.dll" "%Release_Build_Directory%" >nul
	echo Copying SDL2 DLL files to Bin\Debug\x64...
    copy "%SDL2_Library_Folder%\*.dll" "%Debug_Build_Directory%" >nul
    powershell write-host -fore cyan "SDL2 DLL files copied successfully to build directory"
) else (
    powershell write-host -fore yellow No SDL2 DLL files found to ^copy
)

:: Copy GLEW DLL fileS to build directories
if exist "%GLEW_Debug_Library_Folder%\*.dll" (
    echo Copying GLEW DLL files to Bin\Release\x64...
    copy "%GLEW_Release_Library_Folder%\*.dll" "%Release_Build_Directory%" >nul
    copy "%GLEW_Release_Library_Folder%\glew32.pdb" "%Release_Build_Directory%" >nul
	echo Copying GLEW DLL files to Bin\Debug\x64...
    copy "%GLEW_Debug_Library_Folder%\*.dll" "%Debug_Build_Directory%" >nul
    copy "%GLEW_Debug_Library_Folder%\glew32d.pdb" "%Debug_Build_Directory%" >nul
    powershell write-host -fore cyan "GLEW DLL files copied successfully to build directory"
) else (
    powershell write-host -fore yellow No GLEW DLL files found to ^copy
)

:: Copy FMOD DLL fileS to build directories
if exist "%FMOD_Library_Folder%\*.dll" (
    echo Copying FMOD DLL files to Bin\Release\x64...
    copy "%FMOD_Library_Folder%\fmod.dll" "%Release_Build_Directory%" >nul
    copy "%FMOD_Library_Folder%\fmod64.dll" "%Release_Build_Directory%" >nul
	echo Copying FMOD DLL files to Bin\Debug\x64...
    copy "%FMOD_Library_Folder%\fmodL.dll" "%Debug_Build_Directory%" >nul
    copy "%FMOD_Library_Folder%\fmodL64.dll" "%Debug_Build_Directory%" >nul
    powershell write-host -fore cyan "FMOD DLL files copied successfully to build directory"
) else (
    powershell write-host -fore yellow No FMOD DLL files found to ^copy
)

:: Wait for user input to keep the window open
powershell write-host -fore White Finished! Press Any Key To ^Exit
pause >nul
