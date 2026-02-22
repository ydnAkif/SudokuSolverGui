set(CTEST_PROJECT_NAME "SudokuSolverGui")
set(CTEST_NIGHTLY_START_TIME "00:00:00 UTC")

# Dummy CDash config to allow CTest to generate DartConfiguration.tcl
set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "localhost")
set(CTEST_DROP_LOCATION "/submit.php?project=SudokuSolverGui")
set(CTEST_DROP_SITE_CDASH TRUE)
