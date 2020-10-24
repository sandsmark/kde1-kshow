cmake_minimum_required (VERSION 3.0)

project (kshow VERSION 0.2.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

find_package(KDE1 REQUIRED)
find_package(X11 REQUIRED)
find_package(GIF REQUIRED)
find_package(JPEG REQUIRED)

include_directories (
    ${KDE1_INCLUDE_DIR}
)


include(CMakePackageConfigHelpers)
include(Qt1Macros)
include(KDE1Macros)
include(KDE1InstallDirs)

option(ENABLE_SANITIZERS "Enable runtime sanitizing (for development)")
if (ENABLE_SANITIZERS)
    message("Enabling asan and ubsan")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-write-strings")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -std=c++98 -Wno-write-strings")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
endif()

qt1_wrap_moc(MOC_FILES
    kshow.moc QwViewport.moc pixview.moc configdlg.moc
    infodlg.moc listdlg.moc progdlg.moc album.moc
)

add_executable(kshow
    jpeg.cpp netpbm.cpp formats.cpp kshow.cpp
    QwViewport.cpp pixview.cpp configdlg.cpp infodlg.cpp
    listdlg.cpp progdlg.cpp album.cpp main.cpp
    ${MOC_FILES}
)

target_compile_definitions(kshow PRIVATE VERSION=\"${PROJECT_VERSION}\")

target_link_libraries(kshow
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
    ${KDE1_KFILE}
    Qt::Qt1
    X11::X11
    GIF::GIF
    JPEG::JPEG
)

install(TARGETS kshow RUNTIME DESTINATION ${KDE1_BINDIR})

install_icon(kshow.xpm)
install(FILES kshow.kdelnk DESTINATION ${KDE1_APPSDIR}/Graphics/)

install(FILES
    pics/manni.xpm
    pics/back.xpm
    pics/diashow.xpm
    pics/editcopy.xpm
    pics/thumexit.xpm
    pics/filenew.xpm
    pics/fileopen.xpm
    pics/thumdialog.xpm
    pics/thumforward.xpm
    pics/forward.xpm
    pics/help.xpm
    pics/thummove.xpm
    pics/newwin.xpm
    pics/next.xpm
    pics/thumback.xpm
    pics/stop.xpm

    DESTINATION ${KDE1_DATADIR}/kshow/pics/
)

install(FILES
    pics/diashow.xpm pics/thumdialog.xpm
    DESTINATION ${KDE1_TOOLBARDIR}
)

# TODO: doc, it's just german so meh
