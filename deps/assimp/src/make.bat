@ECHO OFF

PUSHD C:\Program Files (x86)\Microsoft Visual Studio\2019\Community
CALL VC\Auxiliary\Build\vcvars64.bat
POPD

SET OUTDIR=out
IF NOT EXIST %OUTDIR% (MKDIR %OUTDIR%)
PUSHD %OUTDIR%
cmake -G Ninja ^
    -DCMAKE_INSTALL_PREFIX=%~dp0..\ ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DBUILD_SHARED_LIBS=OFF ^
    -DASSIMP_BUILD_TESTS=OFF ^
    -DASSIMP_INSTALL_PDB=OFF ^
    ..\assimp-5.0.1
IF %ERRORLEVEL% == 0 (ninja && ninja install)
POPD
