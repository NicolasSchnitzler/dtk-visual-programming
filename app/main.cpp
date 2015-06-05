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
    dtkApplication *application = dtkApplication::create(argc, argv);
    application->setApplicationName("dtkVisualProgramming");
    application->setOrganizationName("inria");
    application->setOrganizationDomain("fr");
    application->setApplicationVersion("0.9.9");

    QCommandLineParser *parser = application->parser();
    parser->setApplicationDescription("DTK visual programming.");

    application->initialize();

    // dtkCorePluginManager::instance()->initialize();
    dtkLinearAlgebraSparseSettings linear_algebra_sparse_settings;
    linear_algebra_sparse_settings.beginGroup("linear-algebra-sparse");
    dtkLinearAlgebraSparse::pluginManager::initialize(linear_algebra_sparse_settings.value("plugins").toString());
    linear_algebra_sparse_settings.endGroup();

    dtkVisualProgrammingMainWindow mainwindow;
    mainwindow.show();
    mainwindow.raise();

    int status = application->exec();

    // dtkCorePluginManager::instance()->uninitialize();

    return status;
}

// 
// main.cpp ends here
