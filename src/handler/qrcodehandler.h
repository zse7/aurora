// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QRCODEHANDLER_H
#define QRCODEHANDLER_H

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

#include "../types/qrcodetypes.h"

class QRCodeHandlerPrivate;

class QRCodeHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QRCodeType type READ type NOTIFY typeChanged)
    Q_PROPERTY(QRCodeText sourceText READ sourceText WRITE setSourceText NOTIFY sourceTextChanged)
    Q_PROPERTY(QRCodeText formatedText READ formatedText NOTIFY formatedTextChanged)
    Q_PROPERTY(QRCodeFields fields READ fields NOTIFY fieldsChanged)
    Q_PROPERTY(bool executable READ executable NOTIFY executableChanged)

public:
    explicit QRCodeHandler(QObject *parent = nullptr);

    void setSourceText(const QRCodeText &sourceText);

    QRCodeType type() const;
    QRCodeText sourceText() const;
    QRCodeText formatedText() const;
    QRCodeFields fields() const;
    bool executable() const;

    Q_INVOKABLE bool execute();

signals:
    void typeChanged(QRCodeType type);
    void sourceTextChanged(const QRCodeText &sourceText);
    void formatedTextChanged(const QRCodeText &formatedText);
    void fieldsChanged(const QRCodeFields &fields);
    void executableChanged(bool executable);

private:
    QSharedPointer<QRCodeHandlerPrivate> m_data{ nullptr };
};

#endif // QRCODEHANDLER_H
