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

#include "dtkVisualProgrammingMainWindow.h"

#include <dtkLog>
#include <dtkCore>
#include <dtkDistributed>
#include <dtkLinearAlgebraSparse>
#include <dtkGeometry>

#include <QtWidgets>

int main(int argc, char **argv)
{
#if defined (Q_OS_UNIX)
    setenv("QT_PLUGIN_PATH", "", 1);
#endif

    dtkApplication *application = dtkApplication::create(argc, argv);
    application->setApplicationName("dtkVisualProgramming");
    application->setOrganizationName("inria");
    application->setOrganizationDomain("fr");
    application->setApplicationVersion("0.9.9");
    application->initialize();

    dtkDistributedSettings settings;
    settings.beginGroup("slave");
    settings.setValue("path", "dtkComposerEvaluatorSparse");
    settings.endGroup();

    dtkGeometrySettings geo_settings;
    geo_settings.beginGroup("geometry");
    dtkGeometry::mesher::initialize(geo_settings.value("plugins").toString());
    dtkGeometry::meshStatistics::initialize(geo_settings.value("plugins").toString());
    geo_settings.endGroup();

    dtkLinearAlgebraSparse::pluginManager::initialize();

    dtkVisualProgrammingMainWindow mainwindow;
    mainwindow.show();
    mainwindow.raise();

    int status = application->exec();

    return status;
}

//
// main.cpp ends here
