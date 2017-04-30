@echo off

IF NOT EXIST build mkdir build

pushd build

set CompilerFlags=-FC -Zi /W0

set Files=..\code\FHS.cpp

set Libs=ws2_32.lib

cl %Files% %CompilerFlags% %libs% /EHsc /I..\INCLUDE /W0 /nologo


popd


