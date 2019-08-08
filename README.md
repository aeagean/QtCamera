<!-- ---
title: Qt摄像头热插拔实现
date: 2019-08-07 22:41:222
tags: Qt
comments: true
--- -->

> 基于Qt的Camera模块实现摄像头的热插拔。当只有一个摄像头时，被拔开后再插入能自动恢复相机状态。当有多个摄像头时，拔开当前摄像头会自动设置另外一个摄像头。

## 基于定时查询的方法
* **定时查询**设备列表变化；
```cpp
connect(&m_checkDeviceListTimer, SIGNAL(timeout()), 
         this, SLOT(checkDeviceList()));
```
* 如果当前设备列表**对比**上一次缓存的设备列表有变化则**发送**设备列表变化**事件**(信号)`deviceListChanged()`；
```cpp
void QtCamera::checkDeviceList()
{
    QList<QCameraInfo> curCameraInfoList = QCameraInfo::availableCameras();
    if ( m_preCameraInfoList.count() != curCameraInfoList.count() ) {
        emit deviceListChanged();
    }

    m_preCameraInfoList = curCameraInfoList;
}
```

## 单设备重连机制
* 先前`autoRestore()`槽函数**绑定**设备列表变化信号。
```cpp
connect(this, SIGNAL(deviceListChanged()), this, SLOT(autoRestore()));
```
* 触发自动重连机制。
&emsp;&emsp;当设备**存在**则会重新启动`start()`;  
&emsp;&emsp;当设备**不存**在则关闭该设备。  
```cpp
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
```

## 多设备自动切换
* `autoSelectDevice()`槽函数连接设备列表变化信号。
```cpp
connect(this, SIGNAL(deviceListChanged()), this, SLOT(autoSelectDevice()));
```

* 当设备已存在再次刷新**当前的**设备；
* 当设备被**断开**，自动切换到第一个设备。
```cpp
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
```

## 关于更多
* 源码地址：
```
https://github.com/aeagean/QtCamera
```
* 文章首发公众号：**Qt君**  
<p align="center">
  <img src="http://www.qtbig.com/about/index/my_qrcode.jpg" alt="微信公众号:Qt君">
  <p align="center"><em>Qt君</em></p>
</p>

* **后续**更新摄像头参数选择算法(自动匹配用户设置摄像头参数，**分辨率**，**帧率**，**格式**)。
