//Я ХЗ ЧЕ ЭТО
// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QRCODETYPES_H
#define QRCODETYPES_H

#include <QtCore/QObject>
#include <QtCore/QVariantMap>

class QRCodeTypeClass
{
    Q_GADGET

public:
    enum Value {
        Text,
        EMail,
        Link,
        Phone,
        Sms,
        VCard,
        Geo,
        WiFi,
        MECard,
        Event,
     };

    Q_ENUM(Value)

private:
    explicit QRCodeTypeClass() = delete;
};

using QRCodeType = QRCodeTypeClass::Value;
using QRCodeText = QString;
using QRCodeFields = QVariantMap;

Q_DECLARE_METATYPE(QRCodeType)
Q_DECLARE_METATYPE(QRCodeText)
Q_DECLARE_METATYPE(QRCodeFields)

#endif // QRCODETYPES_H
