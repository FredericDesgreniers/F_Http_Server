# Fred's Http Server

This is an implementation of an http server made in c++ using only standard libraries. 

## Build
CL.exe must be properly configured as an envrionement variable.
Once that is done, run build.bat to create the FHS.exe


## Structure

### Socket Layer
The socket layer manages everything related to the WinSock api. Ideally, any layer above it should not know about Window specific socket features and should be able to also support other platforms. 

### HttpServer layer
The HttpServer layer is responsible for anything to do with the http protocol. It uses the socket api for communication. This is the last layer where os-spcecific code should be required (mainly for file loading)

### User layer
The user layer is user code, which makes use of an HttpServer. This layer should be as simple as possible and not have to worry about the http protocol or os-specific code. 