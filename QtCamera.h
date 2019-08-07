/**********************************************************
Author: Qt君
微信公众号: Qt君(首发)
QQ群: 732271126
Email: 2088201923@qq.com
LICENSE: MIT
**********************************************************/
#ifndef QT_CAMERA_H
#define QT_CAMERA_H

#include <QCamera>
#include <QCameraInfo>
#include <QVideoWidget>
#include <QTimer>

class QtCamera : public QObject
{
    Q_OBJECT
public:
    QtCamera(QVideoWidget *widget);
    ~QtCamera();

    bool start();
    bool stop();
    bool isStarted();

    bool selectDevice(const QCameraInfo &cameraInfo);

signals:
    void deviceListChanged();
    void deviceStatusChanged();

private slots:
    void autoRestore();
    void autoSelectDevice();
    void checkDeviceList();

private:
    bool deviceExist(const QCameraInfo &cameraInfo) const;

private:
    bool               m_isStarted;
    QTimer             m_checkDeviceListTimer;

    QVideoWidget      *m_videoWidget;
    QCamera           *m_camera;
    QCameraInfo        m_curCameraInfo;
    QList<QCameraInfo> m_curCameraInfoList;
    QList<QCameraInfo> m_preCameraInfoList;
};

#endif // QT_CAMERA_H
