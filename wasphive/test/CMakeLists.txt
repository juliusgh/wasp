INCLUDE(GoogleTest)
include(SetDefault)
SET_DEFAULT(HIVE_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}" )
configure_file("Paths.h.in" "${CMAKE_CURRENT_BINARY_DIR}/Paths.h" @ONLY)
ADD_GOOGLE_TEST(tstHIVE.cpp NP 1)
ADD_GOOGLE_TEST(tstAutoDoc.cpp NP 1)
ADD_GOOGLE_TEST(tstInput2JSON.cpp NP 1)
ADD_GOOGLE_TEST(tstDefinition.cpp NP 1)
