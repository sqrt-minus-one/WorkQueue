@echo off

set CodeDir=%cd%
set Warnings= /W4 /WX /wd4201 /wd4100 /wd4127 /wd4505 /wd4533 /wd4189
set CompileFlags=/FC /Z7 /GR- /EHa- /nologo %Warnings% /I "%CodeDir%/code/"

if not exist build mkdir build
pushd build

cl %CompileFlags% %CodeDir%/code/main.c

popd
