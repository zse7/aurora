// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QtCore/QDir>
#include <QtCore/QUrl>
#include <QtCore/QFile>
#include <QtGui/QDesktopServices>

#include "qrcodehandler_p.h"
#include "qrcodehandler.h"

static const QString emailStart = QStringLiteral("mailto:");
static const QString linkV0Start = QStringLiteral("http://");
static const QString linkV1Start = QStringLiteral("https://");
static const QString phoneStart = QStringLiteral("tel:");
static const QString smsStart = QStringLiteral("smsto:");
static const QString calendarStart = QStringLiteral("begin:vcalendar");
static const QString calendarEnd = QStringLiteral("end:vcalendar");
static const QString vcardStart = QStringLiteral("begin:vcard");
static const QString vcardEnd = QStringLiteral("end:vcard");
static const QString geoStart = QStringLiteral("geo:");
static const QString wifiStart = QStringLiteral("WIFI:");
static const QString eventV1Start = QStringLiteral("BEGIN:VEVENT");
static const QString eventV0Start = QStringLiteral("BEGIN:VCALENDAR");
static const QString meCardStart = QStringLiteral("MECARD:");

static const QMap<QRCodeType, QString> typeNames = {
    { QRCodeType::Text, QStringLiteral("Text") },
    { QRCodeType::Link, QStringLiteral("Link") },
    { QRCodeType::Phone, QStringLiteral("Phone") },
    { QRCodeType::EMail, QStringLiteral("E-Mail") },
    { QRCodeType::VCard, QStringLiteral("V-Card") },
    { QRCodeType::Sms, QStringLiteral("SMS") },
    { QRCodeType::Geo, QStringLiteral("Geo") },
    { QRCodeType::WiFi, QStringLiteral("WiFi") },
    { QRCodeType::MECard, QStringLiteral("MECard") },
    { QRCodeType::Event, QStringLiteral("Event") },
};

QRCodeHandlerPrivate::QRCodeHandlerPrivate(QObject *parent) : QObject(parent) { }

void QRCodeHandlerPrivate::setSourceText(const QRCodeText &sourceText)
{
    if (m_sourceText == sourceText)
        return;

    m_sourceText = sourceText;
    emit sourceTextChanged(sourceText);

    QRCodeType type = _determineType(sourceText);
    if (m_type != type) {
        m_type = type;
        emit typeChanged(type);
    }

    QRCodeFields fields = _parseSourceText(type, sourceText);
    if (m_fields != fields) {
        m_fields = fields;
        emit fieldsChanged(fields);
    }

    QRCodeText formatedText = _makeFormatedText(type, fields);
    if (m_formatedText != formatedText) {
        m_formatedText = formatedText;
        emit formatedTextChanged(formatedText);
    }

    bool executable = _executableCheck(type, fields);
    if (m_executable != executable) {
        m_executable = executable;
        emit executableChanged(executable);
    }
}

QRCodeType QRCodeHandlerPrivate::type() const
{
    return m_type;
}

QRCodeText QRCodeHandlerPrivate::sourceText() const
{
    return m_sourceText;
}

QRCodeText QRCodeHandlerPrivate::formatedText() const
{
    return m_formatedText;
}

QRCodeFields QRCodeHandlerPrivate::fields() const
{
    return m_fields;
}

bool QRCodeHandlerPrivate::executable() const
{
    return m_executable;
}

QRCodeType QRCodeHandlerPrivate::_determineType(const QRCodeText &sourceText)
{
    auto startsWith = [sourceText](const QString &text) {
        return sourceText.startsWith(text, Qt::CaseInsensitive);
    };

    auto endsWith = [sourceText](const QString &text) {
        return sourceText.endsWith(text, Qt::CaseInsensitive);
    };

    if (startsWith(emailStart))
        return QRCodeType::EMail;
    else if (startsWith(linkV0Start) || startsWith(linkV1Start))
        return QRCodeType::Link;
    else if (startsWith(phoneStart))
        return QRCodeType::Phone;
    else if (startsWith(smsStart))
        return QRCodeType::Sms;
    else if (startsWith(vcardStart) && endsWith(vcardEnd))
        return QRCodeType::VCard;
    else if (startsWith(geoStart))
        return QRCodeType::Geo;
    else if (startsWith(wifiStart))
        return QRCodeType::WiFi;
    else if (startsWith(eventV0Start) || startsWith(eventV1Start))
        return QRCodeType::Event;
    else if (startsWith(meCardStart))
        return QRCodeType::MECard;
    else
        return QRCodeType::Text;
}

QRCodeText QRCodeHandlerPrivate::_makeFormatedText(QRCodeType type, const QRCodeFields &fields)
{
    QStringList lines;
    lines.prepend(QStringLiteral("[ Type: %1 ]\n").arg(typeNames.value(type)));
    QMapIterator<QString, QVariant> it(fields);
    while (it.hasNext()) {
        it.next();
        lines.append(QStringLiteral("%1: %2").arg(it.key(), it.value().value<QString>()));
    }

    return lines.join(QStringLiteral("\n"));
}

QRCodeFields QRCodeHandlerPrivate::_parseSourceText(QRCodeType type, const QRCodeText &sourceText)
{
    if (type == QRCodeType::Text)
        return _parseText(sourceText);
    else if (type == QRCodeType::EMail)
        return _parseEMail(sourceText);
    else if (type == QRCodeType::Link)
        return _parseLink(sourceText);
    else if (type == QRCodeType::Phone)
        return _parsePhone(sourceText);
    else if (type == QRCodeType::Sms)
        return _parseSms(sourceText);
    else if (type == QRCodeType::VCard)
        return _parseVCard(sourceText);
    else if (type == QRCodeType::Geo)
        return _parseGeo(sourceText);
    else if (type == QRCodeType::WiFi)
        return _parseWiFi(sourceText);
    else if (type == QRCodeType::Event)
        return _parseEvent(sourceText);
    else if (type == QRCodeType::MECard)
        return _parseMECard(sourceText);
    else
        return {};
}

QRCodeFields QRCodeHandlerPrivate::_parseText(const QRCodeText &sourceText)
{
    // "Here can be everything"

    QRCodeFields fields;
    fields.insert(QStringLiteral("data"), sourceText);

    return fields;
}

QRCodeFields QRCodeHandlerPrivate::_parseEMail(const QRCodeText &sourceText)
{
    // "mailto:email@example.com?subject=Тема&body=Сообщение"

    static const QString subjectStart = QStringLiteral("subject=");
    static const QString bodyStart = QStringLiteral("body=");

    QString copyText = sourceText;
    copyText = copyText.remove(0, emailStart.length());

    QRCodeFields fields;
    QStringList dataList = copyText.split(QStringLiteral("?"));
    if (dataList.size() == 2) {
        fields.insert(QStringLiteral("address"), dataList.first());
        dataList = dataList.last().split(QStringLiteral("&"));

        for (auto &&dataItem : dataList) {
            if (dataItem.startsWith(subjectStart, Qt::CaseInsensitive))
                fields.insert(QStringLiteral("subject"), dataItem.remove(0, subjectStart.length()));
            else if (dataItem.startsWith(bodyStart, Qt::CaseInsensitive))
                fields.insert(QStringLiteral("body"), dataItem.remove(0, bodyStart.length()));
        }
    }

    return fields;
}

QRCodeFields QRCodeHandlerPrivate::_parseLink(const QRCodeText &sourceText)
{
    // "https://twitter.com/elonmusk"

    QRCodeFields fields;
    fields.insert(QStringLiteral("link"), sourceText);

    return fields;
}

QRCodeFields QRCodeHandlerPrivate::_parsePhone(const QRCodeText &sourceText)
{
    // "tel:+7 (495) 000-00-00"

    QString copyText = sourceText;
    copyText = copyText.remove(QRegExp(QStringLiteral("[^0-9]")));

    QRCodeFields fields;
    fields.insert(QStringLiteral("number"), copyText);

    return fields;
}

QRCodeFields QRCodeHandlerPrivate::_parseSms(const QRCodeText &sourceText)
{
    // "smsto:+7 (495) 000-00-00:Тестовое сообщение."

    QString copyText = sourceText;
    copyText = copyText.remove(0, smsStart.length());

    QRCodeFields fields;
    QStringList dataList = copyText.split(QStringLiteral(":"));
    if (dataList.size() == 2) {
        fields.insert(QStringLiteral("number"),
                      dataList.first().remove(QRegExp(QStringLiteral("[^0-9]"))));
        fields.insert(QStringLiteral("message"), dataList.last());
    }

    return fields;
}

QRCodeFields QRCodeHandlerPrivate::_parseVCard(const QRCodeText &sourceText)
{
    // "BEGIN:VCARD\r\nVERSION:2.1\r\nN:Ivanov Ivan
    // Ivanovich\r\nTEL;HOME;VOICE:0043-7252-72720\r\nTEL;WORK;VOICE:0043-7252-72720\r\nEMAIL:email@example.com\r\nORG:Google\r\nTITLE:Developer\r\nBDAY:20210920\r\nADR:Mountain
    // View\r\nURL:https://www.google.ru\r\nEND:VCARD"

    static const QString versionStart = QStringLiteral("version:");
    static const QString nameStart = QStringLiteral("n:");
    static const QString telHomeVoiceStart = QStringLiteral("TEL;HOME;VOICE:");
    static const QString telWorkVoiceStart = QStringLiteral("TEL;WORK;VOICE:");
    static const QString emailStart = QStringLiteral("email:");
    static const QString orgStart = QStringLiteral("org:");
    static const QString titleStart = QStringLiteral("title:");
    static const QString bdayStart = QStringLiteral("bday:");
    static const QString adrStart = QStringLiteral("adr:");
    static const QString urlStart = QStringLiteral("url:");

    QString copyText = sourceText;
    copyText = copyText.remove(0, vcardStart.length());
    copyText = copyText.remove(copyText.lastIndexOf(vcardEnd), vcardEnd.length());

    QRCodeFields fields;
    QStringList dataList = copyText.split(QStringLiteral("\n"));
    for (auto &&dataItem : dataList) {
        dataItem = dataItem.simplified();
        if (dataItem.startsWith(versionStart, Qt::CaseInsensitive))
            fields.insert(QStringLiteral("version"), dataItem.remove(0, versionStart.length()));
        else if (dataItem.startsWith(nameStart, Qt::CaseInsensitive))
            fields.insert(QStringLiteral("name"), dataItem.remove(0, nameStart.length()));
        else if (dataItem.startsWith(telHomeVoiceStart, Qt::CaseInsensitive))
            fields.insert(QStringLiteral("number (home)"),
                          dataItem.remove(0, telHomeVoiceStart.length()));
        else if (dataItem.startsWith(telWorkVoiceStart, Qt::CaseInsensitive))
            fields.insert(QStringLiteral("number (work)"),
                          dataItem.remove(0, telWorkVoiceStart.length()));
        else if (dataItem.startsWith(emailStart, Qt::CaseInsensitive))
            fields.insert(QStringLiteral("email"), dataItem.remove(0, emailStart.length()));
        else if (orgStart.startsWith(orgStart, Qt::CaseInsensitive))
            fields.insert(QStringLiteral("organization"), dataItem.remove(0, orgStart.length()));
        else if (dataItem.startsWith(titleStart, Qt::CaseInsensitive))
            fields.insert(QStringLiteral("position"), dataItem.remove(0, titleStart.length()));
        else if (dataItem.startsWith(bdayStart, Qt::CaseInsensitive))
            fields.insert(QStringLiteral("birthday"), dataItem.remove(0, bdayStart.length()));
        else if (dataItem.startsWith(adrStart, Qt::CaseInsensitive))
            fields.insert(QStringLiteral("address"), dataItem.remove(0, adrStart.length()));
        else if (dataItem.startsWith(urlStart, Qt::CaseInsensitive))
            fields.insert(QStringLiteral("url"), dataItem.remove(0, urlStart.length()));
    }

    return fields;
}

QRCodeFields QRCodeHandlerPrivate::_parseGeo(const QRCodeText &sourceText)
{
    // geo:40.742081,-74.004480

    QString copyText = sourceText;
    copyText = copyText.remove(0, geoStart.length());

    QRCodeFields fields;
    QStringList dataList = copyText.split(QStringLiteral(","));
    if (dataList.size() == 2) {
        fields.insert(QStringLiteral("latitude"), dataList.first());
        fields.insert(QStringLiteral("longitude"), dataList.last());
    }

    return fields;
}

QRCodeFields QRCodeHandlerPrivate::_parseWiFi(const QRCodeText &sourceText)
{
    // WIFI:S:Keenetic-0198_LVD5;T:WPA;P:14877090;;

    static const QString cipherStart = QStringLiteral("T:");
    static const QString nameStart = QStringLiteral("S:");
    static const QString passowrdStart = QStringLiteral("P:");
    static const QString hiddenStart = QStringLiteral("H:");

    QRCodeFields fields;
    QString copyText = sourceText;
    copyText = copyText.replace(wifiStart, "");

    QStringList dataList = copyText.split(QStringLiteral(";"));
    for (QString dataItem : dataList) {
        if (dataItem.startsWith(cipherStart))
            fields.insert(QStringLiteral("Cipher"), dataItem.replace(cipherStart, ""));
        if (dataItem.startsWith(nameStart))
            fields.insert(QStringLiteral("Name"), dataItem.replace(nameStart, ""));
        if (dataItem.startsWith(passowrdStart))
            fields.insert(QStringLiteral("Password"), dataItem.replace(passowrdStart, ""));
        if (dataItem.startsWith(hiddenStart))
            fields.insert(QStringLiteral("Hidden"), dataItem.replace(hiddenStart, ""));
    }

    return fields;
}

QRCodeFields QRCodeHandlerPrivate::_parseEvent(const QRCodeText &sourceText)
{
    // BEGIN:VEVENT\nSUMMARY:Название\nURL:https://vkqr.ru/event\nDTSTART:20240410T220500Z\nDTEND:20240424T210000Z\nEND:VEVENT
    // BEGIN:VCALENDAR\nVERSION:2.0\nBEGIN:VEVENT\nSUMMARY: Заголовок
    // события;\nDESCRIPTION:<p>Описание
    // события</p>;\nDTSTART:2011-11-11T11:11\nDTEND:2022-02-22T11:01\nEND:VEVENT\nEND:VCALENDAR

    static const QString urlStart = QStringLiteral("URL:");
    static const QString summaryStart = QStringLiteral("SUMMARY:");
    static const QString dateStartingStart = QStringLiteral("DTSTART:");
    static const QString dateEndingStart = QStringLiteral("DTEND:");
    static const QString descriptionStart = QStringLiteral("DESCRIPTION:");
    static const QString locationStart = QStringLiteral("LOCATION:");

    QRCodeFields fields;
    QString copyText = sourceText;
    copyText = copyText.replace(eventV0Start, "").replace(eventV1Start, "");
    QStringList dataList = copyText.split(QStringLiteral("\n"));
    for (QString dataItem : dataList) {
        dataItem = dataItem.trimmed();
        if (dataItem.startsWith(urlStart) && !dataItem.replace(urlStart, "").isEmpty())
            fields.insert(QStringLiteral("URL"), dataItem.replace(urlStart, ""));
        if (dataItem.startsWith(summaryStart) && !dataItem.replace(summaryStart, "").isEmpty())
            fields.insert(QStringLiteral("Summary"), dataItem.replace(summaryStart, ""));
        if (dataItem.startsWith(dateStartingStart)
            && !dataItem.replace(dateStartingStart, "").isEmpty())
            fields.insert(QStringLiteral("Date start"), dataItem.replace(dateStartingStart, ""));
        if (dataItem.startsWith(dateEndingStart)
            && !dataItem.replace(dateEndingStart, "").isEmpty())
            fields.insert(QStringLiteral("Date end"), dataItem.replace(dateEndingStart, ""));
        if (dataItem.startsWith(descriptionStart)
            && !dataItem.replace(descriptionStart, "").isEmpty())
            fields.insert(QStringLiteral("Description"), dataItem.replace(descriptionStart, ""));
        if (dataItem.startsWith(locationStart) && !dataItem.replace(locationStart, "").isEmpty())
            fields.insert(QStringLiteral("locationStart"), dataItem.replace(locationStart, ""));
    }

    return fields;
}

QRCodeFields QRCodeHandlerPrivate::_parseMECard(const QRCodeText &sourceText)
{
    // MECARD:N:Doe,John;TEL:1(303)555-1212;EMAIL:john.doe@example.com;ADR:105-32 Farrell St,
    // Dartmouth NS  B3A 4B2,Canada;NICKNAME:Johnny;BDAY:1983-06-15;URL:https://example.com;NOTE:No
    // building at listed address;;

    static const QString urlStart = QStringLiteral("URL:");
    static const QString phoneStart = QStringLiteral("TEL:");
    static const QString birthDayStart = QStringLiteral("BDAY:");
    static const QString nameStart = QStringLiteral("N:");
    static const QString organizationStart = QStringLiteral("ORG:");
    static const QString emailStart = QStringLiteral("EMAIL:");
    static const QString noteStart = QStringLiteral("NOTE:");
    static const QString nicknameStart = QStringLiteral("NICKNAME:");
    static const QString namePronunciationStart = QStringLiteral("SOUND:");
    static const QString telAvStart = QStringLiteral("TEL-AV:");
    static const QString addressStart = QStringLiteral("ADR:");

    QRCodeFields fields;
    QString copyText = sourceText;
    copyText = copyText.replace(meCardStart, "");
    QStringList dataList = copyText.split(QStringLiteral(";"));
    for (QString dataItem : dataList) {
        if (dataItem.startsWith(urlStart) && !dataItem.replace(urlStart, "").isEmpty())
            fields.insert(QStringLiteral("URL"), dataItem.replace(urlStart, ""));
        if (dataItem.startsWith(phoneStart) && !dataItem.replace(phoneStart, "").isEmpty())
            fields.insert(QStringLiteral("Phone"), dataItem.replace(phoneStart, ""));
        if (dataItem.startsWith(birthDayStart) && !dataItem.replace(birthDayStart, "").isEmpty())
            fields.insert(QStringLiteral("Birth day"), dataItem.replace(birthDayStart, ""));
        if (dataItem.startsWith(nameStart) && !dataItem.replace(nameStart, "").isEmpty())
            fields.insert(QStringLiteral("Name"), dataItem.replace(nameStart, ""));
        if (dataItem.startsWith(organizationStart)
            && !dataItem.replace(organizationStart, "").isEmpty())
            fields.insert(QStringLiteral("Organization"), dataItem.replace(organizationStart, ""));
        if (dataItem.startsWith(noteStart) && !dataItem.replace(noteStart, "").isEmpty())
            fields.insert(QStringLiteral("Note"), dataItem.replace(noteStart, ""));
        if (dataItem.startsWith(nicknameStart) && !dataItem.replace(nicknameStart, "").isEmpty())
            fields.insert(QStringLiteral("Nickname"), dataItem.replace(nicknameStart, ""));
        if (dataItem.startsWith(namePronunciationStart)
            && !dataItem.replace(namePronunciationStart, "").isEmpty())
            fields.insert(QStringLiteral("Name pronunciation"),
                          dataItem.replace(namePronunciationStart, ""));
        if (dataItem.startsWith(telAvStart) && !dataItem.replace(telAvStart, "").isEmpty())
            fields.insert(QStringLiteral("Videophone number"), dataItem.replace(telAvStart, ""));
        if (dataItem.startsWith(emailStart) && !dataItem.replace(emailStart, "").isEmpty())
            fields.insert(QStringLiteral("Email"), dataItem.replace(emailStart, ""));
        if (dataItem.startsWith(addressStart) && !dataItem.replace(addressStart, "").isEmpty())
            fields.insert(QStringLiteral("Address"), dataItem.replace(addressStart, ""));
    }
    return fields;
}

bool QRCodeHandlerPrivate::_executableCheck(QRCodeType type, const QRCodeFields &fields)
{
    if (type == QRCodeType::Text)
        return false;
    else if (type == QRCodeType::EMail)
        return !fields.value(QStringLiteral("address")).isNull();
    else if (type == QRCodeType::Link)
        return !fields.value(QStringLiteral("link")).isNull();
    else if (type == QRCodeType::Phone)
        return !fields.value(QStringLiteral("number")).isNull();
    else if (type == QRCodeType::Sms)
        return !fields.value(QStringLiteral("number")).isNull();
    else if (type == QRCodeType::VCard)
        return !fields.value(QStringLiteral("name")).isNull();
    else if (type == QRCodeType::Geo)
        return false;
    else
        return false;
}

bool QRCodeHandlerPrivate::execute()
{
    if (m_type == QRCodeType::Text)
        return _executeText();
    else if (m_type == QRCodeType::EMail)
        return _executeEMail();
    else if (m_type == QRCodeType::Link)
        return _executeLink();
    else if (m_type == QRCodeType::Phone)
        return _executePhone();
    else if (m_type == QRCodeType::Sms)
        return _executeSms();
    else if (m_type == QRCodeType::VCard)
        return _executeVCard();
    else if (m_type == QRCodeType::Geo)
        return _executeGeo();
    else if (m_type == QRCodeType::WiFi)
        return _executeWiFi();
    else if (m_type == QRCodeType::Event)
        return _executeEvent();
    else if (m_type == QRCodeType::MECard)
        return _executeMECard();
    else
        return false;
}

bool QRCodeHandlerPrivate::_executeText()
{
    return false;
}

bool QRCodeHandlerPrivate::_executeWiFi()
{
    return false;
}

bool QRCodeHandlerPrivate::_executeEvent()
{
    return false;
}

bool QRCodeHandlerPrivate::_executeMECard()
{
    return false;
}

bool QRCodeHandlerPrivate::_executeEMail()
{
    return QDesktopServices::openUrl(QUrl(m_sourceText));
}

bool QRCodeHandlerPrivate::_executeLink()
{
    return QDesktopServices::openUrl(QUrl(m_sourceText));
}

bool QRCodeHandlerPrivate::_executePhone()
{
    return QDesktopServices::openUrl(QUrl(m_sourceText));
}

bool QRCodeHandlerPrivate::_executeSms()
{
    const QString number = m_fields.value(QStringLiteral("number")).value<QString>();
    const QString message = m_fields.value(QStringLiteral("message")).value<QString>();
    const QString smsUrl = QStringLiteral("sms:%1?body=%2").arg(number, message);

    return QDesktopServices::openUrl(QUrl(smsUrl));
}

bool QRCodeHandlerPrivate::_executeVCard()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    if (!dir.exists())
        dir.mkpath(dir.absolutePath());

    QFile file(dir.absoluteFilePath(QStringLiteral("profile.vcard")));
    if (file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
        file.write(m_sourceText.toUtf8());
        file.close();
    }

    return QDesktopServices::openUrl(QUrl::fromLocalFile(file.fileName()));
}

bool QRCodeHandlerPrivate::_executeGeo()
{
    return false;
}

QRCodeHandler::QRCodeHandler(QObject *parent)
    : QObject(parent), m_data(new QRCodeHandlerPrivate(this))
{
    connect(m_data.data(), &QRCodeHandlerPrivate::typeChanged, this, &QRCodeHandler::typeChanged);
    connect(m_data.data(), &QRCodeHandlerPrivate::sourceTextChanged, this,
            &QRCodeHandler::sourceTextChanged);
    connect(m_data.data(), &QRCodeHandlerPrivate::formatedTextChanged, this,
            &QRCodeHandler::formatedTextChanged);
    connect(m_data.data(), &QRCodeHandlerPrivate::fieldsChanged, this,
            &QRCodeHandler::fieldsChanged);
    connect(m_data.data(), &QRCodeHandlerPrivate::executableChanged, this,
            &QRCodeHandler::executableChanged);
}

void QRCodeHandler::setSourceText(const QRCodeText &sourceText)
{
    m_data->setSourceText(sourceText);
}

QRCodeType QRCodeHandler::type() const
{
    return m_data->type();
}

QRCodeText QRCodeHandler::sourceText() const
{
    return m_data->sourceText();
}

QRCodeText QRCodeHandler::formatedText() const
{
    return m_data->formatedText();
}

QRCodeFields QRCodeHandler::fields() const
{
    return m_data->fields();
}

bool QRCodeHandler::executable() const
{
    return m_data->executable();
}

bool QRCodeHandler::execute()
{
    return m_data->execute();
}
