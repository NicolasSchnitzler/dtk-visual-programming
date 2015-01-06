/* main.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Aug  3 17:37:15 2009 (+0200)
 * Version: $Id: 52537c458ad30e9cd14529a9777c2bfab4268515 $
 * Last-Updated: ven. oct.  5 17:16:04 2012 (+0200)
 *           By: Nicolas Niclausse
 *     Update #: 92
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include <dtkLog/dtkLog.h>

#include <dtkCore/dtkCorePluginManager.h>

#include "dtkVisualProgrammingMainWindow.h"

#include <QtWidgets>

int main(int argc, char **argv)
{

#if defined (Q_OS_UNIX)
    // unset QT_PLUGIN_PATH, otherwise, kde plugins can change the qmetatype ids
    setenv("QT_PLUGIN_PATH", "", 1);
#endif
    QApplication application(argc, argv);
    application.setApplicationName("dtkVisualProgramming");
    application.setOrganizationName("inria");
    application.setOrganizationDomain("fr");
    application.setApplicationVersion("0.9.9");

    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");

    if (settings.contains("log_level"))
        dtkLogger::instance().setLevel(settings.value("log_level").toString());
    else
        dtkLogger::instance().setLevel(dtkLog::Trace);

    dtkLogger::instance().attachFile(dtkLogPath(&application));

    // dtkCorePluginManager::instance()->initialize();

    dtkVisualProgrammingMainWindow mainwindow;
    mainwindow.show();
    mainwindow.raise();

    int status = application.exec();

    // dtkCorePluginManager::instance()->uninitialize();

    return status;
}
