CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
PROJECT( nodefrontend )

FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(OpenGL COMPONENTS OpenGL REQUIRED)
FIND_PACKAGE(Freetype REQUIRED)
if (UNIX)
FIND_PACKAGE(benchmark REQUIRED)
else(UNIX)
include(FetchContent)
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main)
FetchContent_Declare(googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main) # need main for benchmark::benchmark

FetchContent_MakeAvailable(
        googletest
        googlebenchmark)
endif (UNIX)
FIND_PACKAGE(Lua 5.4 REQUIRED)
SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

SET( ALL_PUBLIC_HEADERS include/util/errod.h include/util/ErrorHandler.h include/util/dbc.h include/core/LuaInterface.h include/core/Circle.h include/core/Container.h include/core/Rectangle.h include/core/Shape.h include/core/Widget.h
        include/core/ConfigurationElement.h
        include/util/Completion.h
        include/util/CompletionTrie.h
        include/util/TrieNode.h
        include/util/CompletionSubstring.h
        include/core/CompositeShape.h
        include/core/Window.h
        include/util/enums.h
        include/core/ValidatorInt.h
        include/gfx/Image.h
        include/gfx/ImageSource.h
        include/gfx/FontImageSource.h
        include/gfx/TextureAtlas.h
        include/gfx/ImageDef.h
        include/gfx/GlyphImageDef.h
        include/gfx/BinImageDef.h
        include/gfx/PackingStrategy.h
        include/gfx/ShelfPackingStrategy.h
        include/core/SpaceTree.h
        src/util/Searchable.h
)
SET( NODEFRONTEND_SRC src/util/errod.cpp src/util/ErrorHandler.cpp src/core/LuaInterface.cpp src/core/Circle.cpp src/core/Container.cpp src/core/Rectangle.cpp src/core/Shape.cpp src/core/Widget.cpp ${ALL_PUBLIC_HEADERS}
        src/core/ConfigurationElement.cpp
        src/util/CompletionTrie.cpp
        src/util/CompletionSubstring.cpp
        src/core/CompositeShape.cpp
        src/core/Window.cpp
        src/gfx/Image.cpp
        src/gfx/ImageSource.cpp
        src/gfx/FontImageSource.cpp
        src/gfx/TextureAtlas.cpp
        src/gfx/ImageDef.cpp
        src/gfx/GlyphImageDef.cpp
        src/gfx/BinImageDef.cpp
        src/gfx/PackingStrategy.cpp
        src/gfx/ShelfPackingStrategy.cpp
        src/core/SpaceTree.cpp
)

SET(HEADERS_GROUP "Header Files")

SOURCE_GROUP(
        ${HEADERS_GROUP}
        FILES ${ALL_PUBLIC_HEADERS}
)

SET(GENERATED_HEADERS_GROUP "Generated header files")
SOURCE_GROUP(
        ${GENERATED_HEADERS_GROUP}
        FILES ${GENERATED_HEADERS}
)

SET( DEP_ROOT CACHE PATH "Dependency root" )

ADD_LIBRARY( nodefrontend SHARED ${NODEFRONTEND_SRC} )
#INCLUDE_DIRECTORIES(PUBLIC ${CMAKE_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${LUA_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})
TARGET_INCLUDE_DIRECTORIES( nodefrontend PUBLIC ${CMAKE_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${LUA_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})

set_target_properties( nodefrontend PROPERTIES DEFINE_SYMBOL NODEFRONTEND_LIBRARY )
TARGET_LINK_LIBRARIES( nodefrontend PRIVATE ${LUA_LIBRARY} GLUT::GLUT OpenGL::GL OpenGL::GLU Freetype::Freetype )
add_subdirectory(src/apps/NodeFrontendTest)
add_subdirectory(src/apps/BenchmarkNodeFrontend)
add_subdirectory(src/demos/demoText)

#ADD_EXECUTABLE( BenchmarkNodeFrontend src/BenchmarkNodeFrontend.cpp include/Node.h include/Transfer.h include/SignalPathDef.h)
#TARGET_INCLUDE_DIRECTORIES( BenchmarkNodeFrontend PUBLIC ${CMAKE_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${LUA_INCLUDE_DIR} ${OSGDB_INCLUDE_DIR} ${ANTLR_Reptol_OUTPUT_DIR})


INCLUDE( CheckIncludeFile )
INCLUDE( CheckIncludeFileCXX )

CHECK_INCLUDE_FILE( windows.h HAVE_WINDOWS_H PATHS DOC "Whether we have the windows.h header, hopefully only on Windows!")
CHECK_INCLUDE_FILE( winsock2.h HAVE_WINSOCK2_H PATHS DOC "")
CHECK_INCLUDE_FILE( float.h HAVE_FLOAT_H  DOC "")
CHECK_INCLUDE_FILE( unistd.h HAVE_UNISTD_H  DOC "")
CHECK_INCLUDE_FILE( sys/types.h HAVE_SYS_TYPES_H  DOC "")
CHECK_INCLUDE_FILE( sys/socket.h HAVE_SYS_SOCKET_H  DOC "")
CHECK_INCLUDE_FILE( sys/time.h HAVE_SYS_TIME_H  DOC "")
CHECK_INCLUDE_FILE( sys/ioctl.h HAVE_SYS_IOCTL_H  DOC "")
CHECK_INCLUDE_FILE( sys/errno.h HAVE_SYS_ERRNO_H  DOC "")
CHECK_INCLUDE_FILE( sys/sysmp.h HAVE_SYS_SYSMP_H  DOC "")
CHECK_INCLUDE_FILE( sys/mman.h HAVE_SYS_MMAN_H  DOC "")
CHECK_INCLUDE_FILE( sys/stat.h HAVE_SYS_STAT_H  DOC "")
CHECK_INCLUDE_FILE( errno.h HAVE_ERRNO_H  DOC "")
CHECK_INCLUDE_FILE( netdb.h HAVE_NETDB_H  DOC "")
CHECK_INCLUDE_FILE( netinet/tcp.h HAVE_NETINET_TCP_H  DOC "")
CHECK_INCLUDE_FILE( netinet/in.h HAVE_NETINET_IN_H  DOC "")
CHECK_INCLUDE_FILE( arpa/inet.h HAVE_ARPA_INET_H  DOC "")
CHECK_INCLUDE_FILE( getopt.h HAVE_GETOPT_H  DOC "")
CHECK_INCLUDE_FILE( fcntl.h HAVE_FCNTL_H  DOC "")
CHECK_INCLUDE_FILE( stdint.h HAVE_STDINT_H  DOC "")
CHECK_INCLUDE_FILE( sys/timerfd.h HAVE_SYS_TIMERFD_H DOC "Whether we have the sys/timerfd header - only present on modern Linux distros, not on Control")
CHECK_INCLUDE_FILE( dirent.h HAVE_DIRENT_H DOC "Whether we have the dirent header, Linux and hopefully macOS X")
CHECK_INCLUDE_FILE( dlfcn.h HAVE_DLFCN_H DOC "Whether we have the dlfcn header for dynnamic libraries, Linux and hopefully macOS X")

CHECK_INCLUDE_FILE_CXX( iostream HAVE_STD_IOSTREAM DOC "")
CHECK_INCLUDE_FILE_CXX( fstream HAVE_STD_FSTREAM DOC "")
CHECK_INCLUDE_FILE_CXX( iosfwd HAVE_STD_IOSFWD DOC "")
CHECK_INCLUDE_FILE_CXX( cassert HAVE_STD_CASSERT DOC "")
CHECK_INCLUDE_FILE_CXX( sstream HAVE_STD_SSTREAM DOC "")
CHECK_INCLUDE_FILE_CXX( stdexcept HAVE_STD_EXCEPT )

CONFIGURE_FILE( include/config/config.h.in ${PROJECT_BINARY_DIR}/include/config/config.h )
IF ( UNIX AND NOT APPLE )
    CONFIGURE_FILE( etc/setup.sh.in ${PROJECT_BINARY_DIR}/etc/setup.sh )
ELSEIF ( APPLE )
    CONFIGURE_FILE( etc/setup_macos.sh.in ${PROJECT_BINARY_DIR}/etc/setup.sh )
ELSEIF ( WIN32 )
    CONFIGURE_FILE( etc/setup.bat.in ${PROJECT_BINARY_DIR}/etc/setup.bat )
ENDIF ( UNIX AND NOT APPLE )
IF ( UNIX )
    INSTALL( FILES ${PROJECT_BINARY_DIR}/etc/setup.sh DESTINATION . )
ELSEIF ( WIN32 )
    INSTALL( FILES ${PROJECT_BINARY_DIR}/etc/setup.bat DESTINATION . )
ENDIF( UNIX )

INSTALL( TARGETS nodefrontend
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
INSTALL(DIRECTORY data DESTINATION .)
