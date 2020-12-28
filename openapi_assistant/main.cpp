#include "openapi_assistant.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    openapi_assistant w;
    w.show();
    return a.exec();
}
