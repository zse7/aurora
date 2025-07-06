// SPDX-FileCopyrightText: 2023 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include "createqrcodepagecontroller.h"
#include <QBuffer>
#include <QDir>
#include <QStandardPaths>
#include <QDateTime>

CreateQrCodePageController::CreateQrCodePageController(QObject *parent) : QObject(parent) { }

/*!
 * \brief Applyes new colors to the QR-code and saves it to a file.
 */
void CreateQrCodePageController::saveQrCodeToFile(QString qrCodeInBase64, QString chosenMainColor,
                                                  QString chosenBackgroundColor)
{
    QStringList stringList = qrCodeInBase64.split(',');
    QByteArray imageData = stringList.at(1).toUtf8();
    imageData = QByteArray::fromBase64(imageData);
    QImage image = QImage::fromData(imageData);
    QString path = QString("%1/%2").arg(
            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), generateFileName());
    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap = changeMainAndBackgroundColors(pixmap, chosenMainColor, chosenBackgroundColor);
    pixmap.save(path, "png");
    emit imageSaved(cutPath(path + ".png"), imageToBase64(pixmap.toImage()));
}

/*!
 * \brief Converts the image in the base64 format.
 */
QString CreateQrCodePageController::imageToBase64(const QImage &image)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "png", -1);
    QString base64 = QString::fromUtf8(byteArray.toBase64());
    return QString("data:image/png;base64,") + base64;
}

/*!
 * \brief Replases the default main and background colors with the specified ones.
 */
QPixmap CreateQrCodePageController::changeMainAndBackgroundColors(QPixmap &pixmap,
                                                                  QColor chosenMainColor,
                                                                  QColor chosenBackgroundColor)
{
    QImage temp = pixmap.toImage();
    for (int y = 0; y < temp.height(); ++y) {
        QRgb *s = reinterpret_cast<QRgb *>(temp.scanLine(y));
        for (int x = 0; x < temp.width(); ++x, ++s) {
            QColor color = QColor::fromRgba(*s);
            uint8_t alpha = color.alpha();
            chosenMainColor.setAlpha(alpha);
            chosenBackgroundColor.setAlpha(alpha);
            if (color == QColor(0, 0, 0))
                *s = chosenMainColor.rgba();
            else if (color == QColor(255, 255, 255))
                *s = chosenBackgroundColor.rgba();
        }
    }
    return QPixmap::fromImage(temp);
}

/*!
 * \brief Generates a result file name taking into the current date.
 */
QString CreateQrCodePageController::generateFileName()
{
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("yyyyMMdd_hhmmss");
    return "qr_code_" + formattedTime;
}

/*!
 * \brief Converts the absolute path to a relative one.
 */
QString CreateQrCodePageController::cutPath(QString path) const
{
    return path.replace(QStandardPaths::writableLocation(QStandardPaths::HomeLocation), "~");
}
