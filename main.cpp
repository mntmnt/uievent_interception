#include <QApplication>

#include "input_catcher.h"

void getInfoXXX(QLabel & label);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MyLabel label;
    label.resize(500, 500);
    label.show();

    registerID((HWND)label.winId());

    getInfoXXX(label);

    return a.exec();
}


QString getInfo(HANDLE h);
void getInfoXXX(QLabel & label) {
    auto devices = getDevices();

    QString res;
       for(int ind = 0; ind < devices.size(); ++ind) {
          RAWINPUTDEVICELIST & i = devices[ind];
          qDebug() << "******* " << ind << "*********";

          QString type = devTypeStr(i.dwType) + "0x" + QString::number(i.dwType, 16);
            qDebug() << "type: " << type << "---" << QString::number((unsigned int)i.hDevice, 16);
            res += type;
            res += "<br />";

            auto inf = getInfo(i.hDevice);
            res += inf + "<br />";
            qDebug() << inf;
                   }
       label.setText(res);

}
/*A handle to the raw input device. This comes from the lParam of the WM_INPUT message, from the hDevice member of RAWINPUTHEADER, or from GetRawInputDeviceList. It can also be NULL if an application inserts input data, for example, by using SendInput.*/
QString getInfo(HANDLE h) {
   QString res;
   res += getDeviceName(h);
   res += getDeviceInfo(h);

  // RID_DEVICE_INFO dev_inf;

  // GetRawInputDeviceInfoA(h, RIDI_DEVICEINFO, &dev_inf, sizeof(RID_DEVICE_INFO));

   return res;
}



