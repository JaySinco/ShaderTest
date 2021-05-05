@ECHO OFF

PUSHD glm-0.9.9.8
IF NOT EXIST ..\..\include (XCOPY /E/Y/I/F glm ..\..\include\glm)
POPD
