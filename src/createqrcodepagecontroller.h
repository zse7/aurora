// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef CREATEQRCODEPAGECONTROLLER_H
#define CREATEQRCODEPAGECONTROLLER_H

#include <QObject>
#include <QImage>
#include <QPixmap>

class CreateQrCodePageController : public QObject
{
    Q_OBJECT
public:
    explicit CreateQrCodePageController(QObject *parent = nullptr);

    Q_INVOKABLE void saveQrCodeToFile(QString qrCodeInBase64, QString chosenMainColor,
                                      QString chosenBackgroundColor);
signals:
    void imageSaved(QString imagePath, QString image);
    void imageColorized(QImage image);
private:
    QString generateFileName();
    QString cutPath(QString path) const;
    QPixmap changeMainAndBackgroundColors(QPixmap &pixmap, QColor to, QColor from);
    QString imageToBase64(const QImage &image);
};

#endif // CREATEQRCODEPAGECONTROLLER_H
