@echo off
set PROTOC_PATH=E:\Lenis0\WorkSpace\Tools\vcpkg\installed\x64-windows\tools\protobuf\protoc.exe
set GRPC_PLUGIN_PATH=E:\Lenis0\WorkSpace\Tools\vcpkg\installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe
set PROTO_FILE=message.proto

echo Generating gRPC code...
%PROTOC_PATH% -I="." --grpc_out="." --plugin=protoc-gen-grpc="%GRPC_PLUGIN_PATH%" "%PROTO_FILE%"

echo Generating C++ code...
%PROTOC_PATH% --cpp_out=. "%PROTO_FILE%"

pause

echo Done.