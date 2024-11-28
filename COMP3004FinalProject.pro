QT       += core gui widgets charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    app.cpp \
    battery.cpp \
    device.cpp \
    main.cpp \
    mainwindow.cpp \
    scan.cpp \
    user.cpp

HEADERS += \
    app.h \
    battery.h \
    device.h \
    mainwindow.h \
    scan.h \
    user.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    resources/JPEGimages/Capture1.jpg \
    resources/JPEGimages/Capture10.jpg \
    resources/JPEGimages/Capture11.jpg \
    resources/JPEGimages/Capture12.jpg \
    resources/JPEGimages/Capture13.jpg \
    resources/JPEGimages/Capture14.jpg \
    resources/JPEGimages/Capture15.jpg \
    resources/JPEGimages/Capture16.jpg \
    resources/JPEGimages/Capture17.jpg \
    resources/JPEGimages/Capture18.jpg \
    resources/JPEGimages/Capture19.jpg \
    resources/JPEGimages/Capture2.jpg \
    resources/JPEGimages/Capture20.jpg \
    resources/JPEGimages/Capture21.jpg \
    resources/JPEGimages/Capture22.jpg \
    resources/JPEGimages/Capture23.jpg \
    resources/JPEGimages/Capture24.jpg \
    resources/JPEGimages/Capture3.jpg \
    resources/JPEGimages/Capture4.jpg \
    resources/JPEGimages/Capture5.jpg \
    resources/JPEGimages/Capture6.jpg \
    resources/JPEGimages/Capture7.jpg \
    resources/JPEGimages/Capture8.jpg \
    resources/JPEGimages/Capture9.jpg \
    resources/compressedImages/Capture1-min.jpg \
    resources/compressedImages/Capture10-min.jpg \
    resources/compressedImages/Capture11-min.jpg \
    resources/compressedImages/Capture12-min.jpg \
    resources/compressedImages/Capture13-min.jpg \
    resources/compressedImages/Capture14-min.jpg \
    resources/compressedImages/Capture15-min.jpg \
    resources/compressedImages/Capture16-min.jpg \
    resources/compressedImages/Capture17-min.jpg \
    resources/compressedImages/Capture18-min.jpg \
    resources/compressedImages/Capture19-min.jpg \
    resources/compressedImages/Capture2-min.jpg \
    resources/compressedImages/Capture20-min.jpg \
    resources/compressedImages/Capture21-min.jpg \
    resources/compressedImages/Capture22-min.jpg \
    resources/compressedImages/Capture23-min.jpg \
    resources/compressedImages/Capture24-min.jpg \
    resources/compressedImages/Capture3-min.jpg \
    resources/compressedImages/Capture4-min.jpg \
    resources/compressedImages/Capture5-min.jpg \
    resources/compressedImages/Capture6-min.jpg \
    resources/compressedImages/Capture7-min.jpg \
    resources/compressedImages/Capture8-min.jpg \
    resources/compressedImages/Capture9-min.jpg \
    resources/images/Capture1.PNG \
    resources/images/Capture10.PNG \
    resources/images/Capture11.PNG \
    resources/images/Capture12.PNG \
    resources/images/Capture13.PNG \
    resources/images/Capture14.PNG \
    resources/images/Capture15.PNG \
    resources/images/Capture16.PNG \
    resources/images/Capture17.PNG \
    resources/images/Capture18.PNG \
    resources/images/Capture19.PNG \
    resources/images/Capture2.PNG \
    resources/images/Capture20.PNG \
    resources/images/Capture21.PNG \
    resources/images/Capture22.PNG \
    resources/images/Capture23.PNG \
    resources/images/Capture3.PNG \
    resources/images/Capture4.PNG \
    resources/images/Capture5.PNG \
    resources/images/Capture6.PNG

RESOURCES += \
    imageResources.qrc
