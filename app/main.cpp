// Version: $Id$
// 
// 

// Commentary: 
// 
// 

// Change Log:
// 
// 

// Code:


#include <dtkLog/dtkLog.h>

#include <dtkCore/dtkCorePluginManager.h>
#include <dtkLinearAlgebraSparse>

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
    dtkLinearAlgebraSparseSettings linear_algebra_sparse_settings;
    linear_algebra_sparse_settings.beginGroup("linear-algebra-sparse");
    dtkLinearAlgebraSparse::pluginManager::initialize(linear_algebra_sparse_settings.value("plugins").toString());
    linear_algebra_sparse_settings.endGroup();

    dtkVisualProgrammingMainWindow mainwindow;
    mainwindow.show();
    mainwindow.raise();

    int status = application.exec();

    // dtkCorePluginManager::instance()->uninitialize();

    return status;
}

// 
// main.cpp ends here
