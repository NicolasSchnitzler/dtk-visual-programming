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

#pragma once

#include <QtCore>
#include <QtWidgets>

class dtkComposerWidget;
class dtkComposerNodeFactoryView;
class dtkComposerGraphView;
class dtkComposerSceneModel;
class dtkComposerSceneNodeEditor;
class dtkComposerSceneView;
class dtkComposerStackView;
class dtkComposerViewManager;

class dtkMonitoringList;
class dtkMonitoringScene;
class dtkMonitoringView;
class dtkMonitoringModel;

class dtkRecentFilesMenu;

class dtkSplitter;

class dtkWidgetsLogView;

class dtkVisualProgrammingMainWindow;

class dtkVisualProgrammingMainWindowPrivate : public QObject
{
    Q_OBJECT

public:
    bool maySave(void);

public:
    void setCurrentFile(const QString& file);

public slots:
    void setModified(bool modified);

public:
    dtkComposerWidget *composer;
    dtkComposerNodeFactoryView *nodes;
    dtkComposerGraphView *graph;
    dtkComposerSceneModel *model;
    dtkComposerSceneNodeEditor *editor;
    dtkComposerSceneView *scene;
    dtkComposerStackView *stack;

public:
    dtkMonitoringList *monitoring_list;
    dtkMonitoringModel *monitoring_model;
    dtkMonitoringScene *monitoring_scene;
    dtkMonitoringView *monitoring_view;

public:
    dtkComposerViewManager *view_manager;

public:
    QWidget     *composer_widget;
    QWidget   *monitoring_widget;
    QWidget *distribution_widget;
    QWidget        *debug_widget;
    QWidget         *view_widget;

public:
    dtkWidgetsLogView *log_view;

public:
    dtkSplitter *inner;

public:
    QMenu *composition_menu;
    QAction *composition_open_action;
    QAction *composition_save_action;
    QAction *composition_saveas_action;
    QAction *composition_insert_action;
    QAction *composition_quit_action;

public:
    QMenu *edit_menu;
    QAction *undo_action;
    QAction *redo_action;

public:
    dtkRecentFilesMenu *recent_compositions_menu;

public:
    QPushButton *compo_button;
    QPushButton *montr_button;
    QPushButton *distr_button;
    QPushButton *debug_button;
    QPushButton *view_button;

public:
    bool closing;

public:
    QString current_composition;

public:
    dtkVisualProgrammingMainWindow *q;
};

//
// dtkVisualProgrammingMainWindow_p.h ends here
