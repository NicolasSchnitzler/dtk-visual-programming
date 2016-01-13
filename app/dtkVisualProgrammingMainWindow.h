/* dtkVisualProgrammingMainWindow.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Aug  3 17:38:47 2009 (+0200)
 * Version: $Id: 736dba4f5968cf2d506d986ea3c6cc7431d3c49f $
 * Last-Updated: Tue Jun 25 09:44:56 2013 (+0200)
 *           By: Selim Kraria
 *     Update #: 70
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef DTKVISUALPROGRAMMINGMAINWINDOW_H
#define DTKVISUALPROGRAMMINGMAINWINDOW_H

#include <dtkComposer/dtkComposerWriter.h>

#include <QtWidgets>

class dtkComposerSceneNode;

class dtkVisualProgrammingMainWindowPrivate;

class dtkVisualProgrammingMainWindow : public QMainWindow
{
    Q_OBJECT

public:
     dtkVisualProgrammingMainWindow(QWidget *parent = 0);
    ~dtkVisualProgrammingMainWindow(void);

    void readSettings(void);
    void writeSettings(void);

public slots:
    bool compositionOpen(void);
    bool compositionOpen(const QString& file);
    bool compositionSave(void);
    bool compositionSaveAs(void);
    bool compositionSaveAs(const QString& file, dtkComposerWriter::Type type = dtkComposerWriter::Ascii);
    bool compositionInsert(void);
    bool compositionInsert(const QString& file);

protected slots:
    void switchToCompo(void);
    void switchToMontr(void);
    void switchToDstrb(void);
    void switchToDebug(void);
    void switchToView(void);

protected slots:
    void showControls(void);

protected slots:
    void onComposerNodeFlagged(dtkComposerSceneNode *);

protected:
    void closeEvent(QCloseEvent *event);

private:
    dtkVisualProgrammingMainWindowPrivate *d;
};

#endif
