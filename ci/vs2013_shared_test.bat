call .\ci\setup.bat
cd build
set "base=%cd%"
set conda=%base%\conda
git clone git@code.ornl.gov:warroom/miniconda.git
%base%\miniconda\Miniconda3-latest-Windows-x86_64.exe /S /D=%conda%
set "PATH=%conda%\Scripts;%conda%;%conda%\Library\bin;%PATH%"
rem %conda%\Scripts\pip.exe install -r %base%\miniconda\windows_requirements.txt --prefix=%conda%
where python
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86_amd64


cmake -DBUILD_SHARED_LIBS=ON ^
      -DBUILDNAME="Windows-CL-18-Shared-Release-%CI_BUILD_REF_NAME%" ^
      -DCMAKE_BUILD_TYPE:STRING=RELEASE ^
      -Dwasp_ENABLE_TESTS:BOOL=ON ^
      -Dwasp_ENABLE_ALL_PACKAGES:BOOL=ON ^
      -G "Visual Studio 12 2013 Win64" ..
ctest --output-on-failure ^
      -D ExperimentalStart ^
      -D ExperimentalBuild ^
      -D ExperimentalTest ^
      -D ExperimentalSubmit
