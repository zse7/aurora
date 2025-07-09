# SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
# SPDX-License-Identifier: BSD-3-Clause

TARGET = ru.auroraos.QrCodeReader

QT += dbus positioning

message(SDK VERSION: $${AURORA_SDK_VERSION})
message(SDK MAJOR VERSION: $${AURORA_SDK_MAJOR_VERSION})
message(SDK MINOR VERSION: $${AURORA_SDK_MINOR_VERSION})
message(SDK BUILD VERSION: $${AURORA_SDK_BUILD_VERSION})
message(SDK REVSION VERSION: $${AURORA_SDK_REVISION_VERSION})

# For old version sdk(os) have to use sailfishapp and added define USING_SAILFISHAPP
lessThan(AURORA_SDK_VERSION, 4.0.2.172) {
    USING_SAILFISHAPP=ON
    DEFINES+=USING_SAILFISHAPP
}

equals(USING_SAILFISHAPP, "ON") {
    CONFIG += \
        sailfishapp \
        sailfishapp_i18n \
} else {
    CONFIG += \
        auroraapp \
        auroraapp_i18n \
}

TRANSLATIONS += \
    translations/ru.auroraos.QrCodeReader.ts \
    translations/ru.auroraos.QrCodeReader-ru.ts \

HEADERS += \
    src/createqrcodepagecontroller.h \
    src/types/qrcodetypes.h \
    src/handler/qrcodehandler_p.h \
    src/handler/qrcodehandler.h \

SOURCES += \
    src/createqrcodepagecontroller.cpp \
    src/handler/qrcodehandler.cpp \
    src/main.cpp \

DISTFILES += \
    rpm/ru.auroraos.QrCodeReader.spec \
    translations/*.ts \
    ru.auroraos.QrCodeReader.desktop \

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172
