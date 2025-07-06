// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QRCODEHANDLER_P_H
#define QRCODEHANDLER_P_H

#include <QtCore/QObject>

#include "../types/qrcodetypes.h"

class QRCodeHandlerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit QRCodeHandlerPrivate(QObject *parent = nullptr);

    void setSourceText(const QRCodeText &sourceText);

    QRCodeType type() const;
    QRCodeText sourceText() const;
    QRCodeText formatedText() const;
    QRCodeFields fields() const;
    bool executable() const;

    bool execute();

signals:
    void typeChanged(QRCodeType type);
    void sourceTextChanged(const QRCodeText &sourceText);
    void formatedTextChanged(const QRCodeText &formatedText);
    void fieldsChanged(const QRCodeFields &fields);
    void executableChanged(bool executable);

private:
    QRCodeType _determineType(const QRCodeText &sourceText);
    QRCodeText _makeFormatedText(QRCodeType type, const QRCodeFields &fields);
    QRCodeFields _parseSourceText(QRCodeType type, const QRCodeText &sourceText);
    QRCodeFields _parseText(const QRCodeText &sourceText);
    QRCodeFields _parseEMail(const QRCodeText &sourceText);
    QRCodeFields _parseLink(const QRCodeText &sourceText);
    QRCodeFields _parsePhone(const QRCodeText &sourceText);
    QRCodeFields _parseSms(const QRCodeText &sourceText);
    QRCodeFields _parseVCard(const QRCodeText &sourceText);
    QRCodeFields _parseGeo(const QRCodeText &sourceText);
    QRCodeFields _parseWiFi(const QRCodeText &sourceText);
    QRCodeFields _parseEvent(const QRCodeText &sourceText);
    QRCodeFields _parseMECard(const QRCodeText &sourceText);
    bool _executableCheck(QRCodeType type, const QRCodeFields &fields);
    bool _executeText();
    bool _executeEMail();
    bool _executeLink();
    bool _executePhone();
    bool _executeSms();
    bool _executeVCard();
    bool _executeGeo();
    bool _executeWiFi();
    bool _executeEvent();
    bool _executeMECard();

private:
    QRCodeType m_type{ QRCodeType::Text };
    QRCodeText m_sourceText{};
    QRCodeText m_formatedText{};
    QRCodeFields m_fields{};
    bool m_executable{ false };
};

#endif // QRCODEHANDLER_P_H
