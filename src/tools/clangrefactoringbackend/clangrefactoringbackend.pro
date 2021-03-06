QTC_LIB_DEPENDS += \
    clangbackendipc

include(../../qtcreatortool.pri)
include(../../shared/clang/clang_installation.pri)
include(source/clangrefactoringbackend-source.pri)

QT += core network
QT -= gui

LIBS += $$LIBTOOLING_LIBS
INCLUDEPATH += $$LLVM_INCLUDEPATH

SOURCES += \
    clangrefactoringbackendmain.cpp

unix {
    !osx: QMAKE_LFLAGS += -Wl,-z,origin
    !disable_external_rpath: QMAKE_LFLAGS += -Wl,-rpath,$$shell_quote($${LLVM_LIBDIR})
}
