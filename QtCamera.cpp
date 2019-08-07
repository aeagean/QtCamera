/**********************************************************
Author: Qt君
微信公众号: Qt君(首发)
QQ群: 732271126
Email: 2088201923@qq.com
LICENSE: MIT
**********************************************************/
#include "QtCamera.h"
#include "QtCamera.h"
#include <QDebug>

QtCamera::QtCamera(QVideoWidget *widget) :
    m_videoWidget(widget),
    m_isStarted(false),
    m_camera(NULL)
{
    selectDevice(QCameraInfo::defaultCamera());
    start();

    connect(&m_checkDeviceListTimer, SIGNAL(timeout()), this, SLOT(checkDeviceList()));
    connect(this, SIGNAL(deviceListChanged()), this, SLOT(autoRestore()));
    connect(this, SIGNAL(deviceListChanged()), this, SLOT(autoSelectDevice()));

    m_checkDeviceListTimer.start(3000);

    QList<QCameraInfo> curCameraInfoList = QCameraInfo::availableCameras();
    for (int i = 0; i < curCameraInfoList.count(); i++) {
        qDebug() << curCameraInfoList.at(i).deviceName();
    }
}

QtCamera::~QtCamera()
{

}

bool QtCamera::start()
{
    if (! m_isStarted) {
        m_isStarted = true;

        m_camera->start();
    }

    return true;
}

bool QtCamera::stop()
{
    if (m_isStarted) {
        m_isStarted = false;

        m_camera->stop();
    }

    return true;
}

bool QtCamera::isStarted()
{
    return m_isStarted;
}

bool QtCamera::selectDevice(const QCameraInfo &cameraInfo)
{
    bool restart = isStarted();
    if (isStarted()) {
        stop();
    }

    if (m_camera) {
        delete m_camera;
    }

    m_curCameraInfo = cameraInfo;
    m_camera = new QCamera(m_curCameraInfo);
    m_camera->setViewfinder(m_videoWidget);

    if (restart)
        start();
}

void QtCamera::autoRestore()
{
    if (! m_isStarted) {
        return;
    }

    if (deviceExist(m_curCameraInfo)) {
        if (m_camera->status() != QCamera::ActiveStatus) {
            QTimer::singleShot(500, m_camera, SLOT(start()));
        }
    }
    else {
        if (m_camera->status() == QCamera::ActiveStatus) {
            m_camera->stop();
            m_camera->unload();
        }
    }
}

void QtCamera::autoSelectDevice()
{
    QList<QCameraInfo> curCameraInfoList = QCameraInfo::availableCameras();
    if (curCameraInfoList.isEmpty())
        return;

    if (curCameraInfoList.contains(m_curCameraInfo)) {
        selectDevice(m_curCameraInfo);
        return;
    }

    selectDevice(curCameraInfoList.first());
}

void QtCamera::checkDeviceList()
{
    QList<QCameraInfo> curCameraInfoList = QCameraInfo::availableCameras();
    if ( m_preCameraInfoList.count() != curCameraInfoList.count() ) {
        emit deviceListChanged();
    }

    m_preCameraInfoList = curCameraInfoList;
}

bool QtCamera::deviceExist(const QCameraInfo &cameraInfo) const
{
    QList<QCameraInfo> cameraInfoList = QCameraInfo::availableCameras();
    for (int i = 0; i < cameraInfoList.count(); i++) {
        if (cameraInfo.deviceName() == cameraInfoList.at(i).deviceName())
            return true;
    }

    return false;
}
