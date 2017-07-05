# Fred's Http Server

This is an implementation of an http server made in c++ using only standard libraries. 

## Build

NOTE: This project uses a non-standard build approach where every file is "combined" into one big file. For this reason, it does not compile properly using IDE's such as visual studio. However, it should work fine with any compiller from the command line. 

The main reason for doing this is to make the compillation process as simple as possible using a .bat file since the aim of this project is to use as few tools and libraries as possible outside of a c++ compiler and a simple text editor.

Keeping the files as seperate compillation units would be both slower(Since no incremental compillation is used) and require editing the build.bat for any new file. 


### Guide

CL.exe must be properly configured as an envrionement variable.
Once that is done, run build.bat to create the FHS.exe


## Structure

### Socket Layer
The socket layer manages everything related to the WinSock api. Ideally, any layer above it should not know about Window specific socket features and should be able to also support other platforms. 

### HttpServer layer
The HttpServer layer is responsible for anything to do with the http protocol. It uses the socket api for communication. This is the last layer where os-spcecific code should be required (mainly for file loading)

### User layer
The user layer is user code, which makes use of an HttpServer. This layer should be as simple as possible and not have to worry about the http protocol or os-specific code. 