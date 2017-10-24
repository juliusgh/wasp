. ci/setup.sh
cd build
module load valgrind
cmake -DBUILDNAME="$(uname -s)-GCC-4.8.5-Release-${CI_BUILD_REF_NAME}" \
      -DCMAKE_BUILD_TYPE=RELEASE \
      -Dwasp_ENABLE_TESTS=ON \
      -DBUILD_SHARED_LIBS:BOOL=ON \
      -Dwasp_ENABLE_ALL_PACKAGES=ON \
      ..

ctest -D ExperimentalStart \
      -D ExperimentalBuild -j 8\
      -D ExperimentalSubmit \
      -D ExperimentalTest -j 8 \
      -D ExperimentalSubmit

# clean up prior config for the next bundle config
rm -rf CMake*
module avail
module load gcc-4.8.5-static
cmake -DBUILDNAME="$(uname -s)-GCC-4.8.5-Bundle-${CI_BUILD_REF_NAME}" \
      -DCPACK_PACKAGE_NAME=WASP \
      -DBUILD_SHARED_LIBS:BOOL=ON \
       -DCMAKE_BUILD_TYPE=RELEASE \
       -Dwasp_ENABLE_ALL_PACKAGES=ON \
	  -Dwasp_ENABLE_TESTS:BOOL=OFF \
	  -Dwasp_ENABLE_ALL_PACKAGES:BOOL=ON \
	  -Dwasp_ENABLE_testframework:BOOL=OFF \
	  -Dwasp_ENABLE_googletest:BOOL=OFF \
      -Dwasp_ENABLE_wasppy:BOOL=OFF \
	  -Dwasp_ENABLE_INSTALL_CMAKE_CONFIG_FILES:BOOL=ON \
	  -Dwasp_GENERATE_EXPORT_FILE_DEPENDENCIES:BOOL=ON \
	  -Dwasp_ENABLE_CPACK_PACKAGING:BOOL=ON \
      -DCMAKE_EXE_LINKER_FLAGS=-static \
      ..

make -j 8 package
ls -l ./
# Copy bundle parts up to parent directory to avoid artifact
# having build directory
cp WASP-*-Linux.sh ../
cp waspConfig_install.cmake ../
