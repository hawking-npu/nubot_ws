#include <QIcon>
#include <QApplication>
#include "image_subscribe.h"
#include "calibration_dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString myDir=QCoreApplication::applicationDirPath();
    QDir::setCurrent(myDir);
    a.setWindowIcon(QIcon("../../src/calibration_util/source/hawking.jpeg"));//改变应用程序图标

    ros::init(argc, argv,"calibration_omni");
    ros::NodeHandle node;

    image_subscribe this_image_subscribe(node);
    Dialog w(this_image_subscribe);
    w.show();

    this_image_subscribe.start();
    return a.exec();
}
