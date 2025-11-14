
IF EXIST regen_build_number.bat (

call regen_build_number.bat
)

IF NOT EXIST build_number.h (

echo #define GENERATED_BUILD_NUMBER 0> build_number.h
)

IF NOT EXIST makefile_dep (
touch makefile_dep
)


echo "Make depends ..."
@echo off
bash -c "sde-make depend"

@echo on
echo "Rebuilding ..."
bash -c "sde-make clean"

bash -c "sde-make image %1"
pause