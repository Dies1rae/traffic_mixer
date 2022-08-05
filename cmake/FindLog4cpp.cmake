if(UNIX)
	FIND_PATH(LOG4CPP_INCLUDE_DIR log4cpp/FileAppender.hh
		"./log4cpp/include/"
	)
	FIND_LIBRARY(LOG4CPP_LIBRARIES
	  NAMES liblog4cpp.so
	  PATHS "./log4cpp/lib"
	)
elseif(WIN32)
	FIND_PATH(LOG4CPP_INCLUDE_DIR 
		NAMES log4cpp/FileAppender.hh
	)
	FIND_LIBRARY(LOG4CPP_LIBRARIES
	  NAMES log4cpp.lib
	)
endif()

# handle the QUIETLY and REQUIRED arguments and set JSONCPP_FOUND to TRUE
# if all listed variables are TRUE, hide their existence from configuration view
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(log4cpp DEFAULT_MSG LOG4CPP_INCLUDE_DIR LOG4CPP_LIBRARIES)
MARK_AS_ADVANCED(
  LOG4CPP_INCLUDE_DIR
  LOG4CPP_LIBRARIES
  )
