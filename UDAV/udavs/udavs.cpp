#include "udavs.h"
#include "server_application.h"

ServerApplication* serverApp;

int main(int argc, char *argv[])
{
    ServerApplication app(argc, argv);

    int res = app.exec();
    return res;
}
