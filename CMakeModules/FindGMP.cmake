if(NOT GMP_FOUND)
   find_path(GMP_INCLUDES 
      NAMES gmp.h 
      HINTS ${GMP_ROOT_DIR} 
      PATH_SUFFIXES include)
   find_library(GMP_LIBRARIES 
      gmp 
      HINTS ${GMP_ROOT_DIR} 
      PATH_SUFFIXES lib)
   
   include(FindPackageHandleStandardArgs)
   find_package_handle_standard_args(GMP DEGAULT_MSG 
      GMP_INCLUDES GMP_LIBRARIES)
   mark_as_advanced(GMP_INCLUDES GMP_LIBRARIES)
endif(NOT GMP_FOUND)
