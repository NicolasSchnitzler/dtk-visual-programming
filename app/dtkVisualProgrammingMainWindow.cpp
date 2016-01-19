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
#include "dtkVisualProgrammingMainWindow_p.h"

#include <dtkComposer/dtkComposer.h>
#include <dtkComposer/dtkComposerNode.h>
#include <dtkComposer/dtkComposerWidget.h>
#include <dtkComposer/dtkComposerCompass.h>
#include <dtkComposer/dtkComposerEvaluator.h>
#include <dtkComposer/dtkComposerFactory.h>
#include <dtkComposer/dtkComposerNodeFactoryView.h>
#include <dtkComposer/dtkComposerGraph.h>
#include <dtkComposer/dtkComposerGraphView.h>
#include <dtkComposer/dtkComposerScene.h>
#include <dtkComposer/dtkComposerSceneModel.h>
#include <dtkComposer/dtkComposerSceneNodeEditor.h>
#include <dtkComposer/dtkComposerSceneView.h>
#include <dtkComposer/dtkComposerStack.h>
#include <dtkComposer/dtkComposerStackView.h>
#include <dtkComposer/dtkComposerView.h>
#include <dtkComposer/dtkComposerViewManager.h>
#include <dtkComposer/dtkComposerViewController.h>
#include <dtkComposer/dtkComposerNodeBoolean.h>
#include <dtkComposer/dtkComposerNodeInteger.h>

#include <dtkMonitoring/dtkMonitoringController.h>
#include <dtkMonitoring/dtkMonitoringList.h>
#include <dtkMonitoring/dtkMonitoringScene.h>
#include <dtkMonitoring/dtkMonitoringView.h>
#include <dtkMonitoring/dtkMonitoringModel.h>
#include <dtkMonitoring/dtkMonitoringFactory.h>

#include <dtkGuiSupport/dtkScreenMenu.h>
#include <dtkGuiSupport/dtkRecentFilesMenu.h>
#include <dtkGuiSupport/dtkSpacer.h>
#include <dtkGuiSupport/dtkSplitter.h>

#include <dtkCore/dtkCore.h>
#include <dtkCore/dtkCorePluginManager.h>

#if defined(DTK_BUILD_SUPPORT_PLOT)
#include <dtkPlotSupport/dtkPlotView.h>
#include <dtkPlotSupport/dtkPlotViewSettings.h>
#endif

#include <dtkLog/dtkLog.h>

#include <dtkWidgets/dtkWidgetsLogView.h>
#include <dtkWidgets/dtkNotification.h>
#include <dtkWidgets/dtkNotificationDisplay.h>

#include <dtkLinearAlgebraSparse>
#include <dtkLinearAlgebraSparseComposerFactoryExtension.h>

#include <dtkGeometry>

#include <QtCore>
#include <QtWidgets>

// /////////////////////////////////////////////////////////////////
// dtkVisualProgrammingMainWindowPrivate
// /////////////////////////////////////////////////////////////////

bool dtkVisualProgrammingMainWindowPrivate::maySave(void)
{
    if(this->closing)
        return true;

    if (q->isWindowModified()) {
        QMessageBox::StandardButton ret = QMessageBox::warning(q,
            q->tr("VisualProgramming"),
            q->tr("The composition has been modified.\n Do you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Save);

        if (ret == QMessageBox::Save)
            return q->compositionSave();
        else
            if(ret == QMessageBox::Cancel)
                return false;
    }

    return true;
}

void dtkVisualProgrammingMainWindowPrivate::setCurrentFile(const QString &file)
{
     this->current_composition = file;

     q->setWindowModified(false);

     QString shownName = this->current_composition;
     if (shownName.isEmpty())
         shownName = "untitled.dtk";

     q->setWindowFilePath(shownName);
}

void dtkVisualProgrammingMainWindowPrivate::setModified(bool modified)
{
    q->setWindowModified(modified);
}

// /////////////////////////////////////////////////////////////////
// dtkVisualProgrammingMainWindow
// /////////////////////////////////////////////////////////////////

dtkVisualProgrammingMainWindow::dtkVisualProgrammingMainWindow(QWidget *parent) : QMainWindow(parent), d(new dtkVisualProgrammingMainWindowPrivate)
{
    d->q = this;

    this->readSettings();

// /////////////////////////////////////////////////////////////////
// Elements
// /////////////////////////////////////////////////////////////////

    d->composer = new dtkComposerWidget;
    d->composer->view()->setBackgroundBrush(QBrush(QPixmap(":dtkVisualProgramming/pixmaps/dtkComposerScene-bg.png")));
    d->composer->view()->setCacheMode(QGraphicsView::CacheBackground);

    QScopedPointer<dtkComposerNodeFactoryExtension> extension(new dtkLinearAlgebraSparseComposerFactoryExtension);
    d->composer->factory()->extend(extension.data());

    QScopedPointer<dtkComposerNodeFactoryExtension> geo_ext(new dtkGeometryComposerFactoryExtension);
    d->composer->factory()->extend(geo_ext.data());

    d->editor = new dtkComposerSceneNodeEditor(this);
    d->editor->setScene(d->composer->scene());
    d->editor->setStack(d->composer->stack());
    d->editor->setGraph(d->composer->graph());

    d->model = new dtkComposerSceneModel(this);
    d->model->setScene(d->composer->scene());

    d->scene = new dtkComposerSceneView(this);
    d->scene->setScene(d->composer->scene());
    d->scene->setModel(d->model);

    d->stack = new dtkComposerStackView(this);
    d->stack->setStack(d->composer->stack());

    d->nodes = new dtkComposerNodeFactoryView(this);
    d->nodes->setFactory(d->composer->factory());

    d->graph = new dtkComposerGraphView(this);
    d->graph->setScene(d->composer->graph());
    d->graph->setBackgroundBrush(QBrush(QPixmap(":dtkVisualProgramming/pixmaps/dtkComposerGraphView-bg.png")));

    d->log_view = new dtkWidgetsLogView(this);

    d->view_manager = new dtkComposerViewManager(this);

    d->closing = false;

// /////////////////////////////////////////////////////////////////
// Composer Widget
// /////////////////////////////////////////////////////////////////

    dtkSplitter *left = new dtkSplitter(this);
    left->setOrientation(Qt::Vertical);
    left->addWidget(d->nodes);
    left->addWidget(d->view_manager);
    left->setFixedWidth(300);

    dtkSplitter *right = new dtkSplitter(this);
    right->setOrientation(Qt::Vertical);
    right->addWidget(d->scene);
    right->addWidget(d->editor);
    right->addWidget(d->stack);
    right->addWidget(d->composer->compass());
    right->setSizes(QList<int>()
                    << this->size().height()/4
                    << this->size().height()/4
                    << this->size().height()/4
                    << this->size().height()/4);
    right->setFixedWidth(300);

    QHBoxLayout *composer_layout = new QHBoxLayout;
    composer_layout->setContentsMargins(0, 0, 0, 0);
    composer_layout->setSpacing(0);
    composer_layout->addWidget(left);
    composer_layout->addWidget(d->composer);
    composer_layout->addWidget(right);

    d->composer_widget = new QWidget(this);
    d->composer_widget->setLayout(composer_layout);

// /////////////////////////////////////////////////////////////////
// Monitoring widget
// /////////////////////////////////////////////////////////////////

    d->monitoring_model = new dtkMonitoringModel;

    d->monitoring_list = new dtkMonitoringList(this);
    d->monitoring_list->setFixedWidth(300);
    d->monitoring_list->setDragEnabled(true);
    d->monitoring_list->setModel(d->monitoring_model);
    dtkMonitoringController::instance()->registerModel(d->monitoring_model);

    d->monitoring_factory = new dtkMonitoringFactory;
    d->monitoring_factory->initialize();

    d->monitoring_scene = new dtkMonitoringScene(this);
    d->monitoring_scene->setFactory(d->monitoring_factory);

    d->monitoring_view = new dtkMonitoringView(this);
    d->monitoring_view->setScene(d->monitoring_scene);

    QHBoxLayout *monitoring_layout = new QHBoxLayout;
    monitoring_layout->setSpacing(0);
    monitoring_layout->setContentsMargins(0, 0, 0, 0);
    monitoring_layout->addWidget(d->monitoring_list);
    monitoring_layout->addWidget(d->monitoring_view);

    d->monitoring_widget = new QWidget(this);
    d->monitoring_widget->setLayout(monitoring_layout);

    connect(d->composer->scene(), SIGNAL(monitoringChanged(dtkComposerNode *, bool)), dtkMonitoringController::instance(), SLOT(onMonitoringChanged(dtkComposerNode *, bool)));

// /////////////////////////////////////////////////////////////////
// Distribution widget
// /////////////////////////////////////////////////////////////////

    QHBoxLayout *distribution_layout = new QHBoxLayout;
    distribution_layout->setContentsMargins(0, 0, 0, 0);
    distribution_layout->setSpacing(0);

    d->distribution_widget = new QWidget(this);
    d->distribution_widget->setLayout(distribution_layout);

// /////////////////////////////////////////////////////////////////
// Debug widget
// /////////////////////////////////////////////////////////////////

    QVBoxLayout *debug_layout = new QVBoxLayout;
    debug_layout->setContentsMargins(0, 0, 0, 0);
    debug_layout->setSpacing(0);
    debug_layout->addWidget(d->graph);
    debug_layout->addWidget(d->log_view);

    d->debug_widget = new QWidget(this);
    d->debug_widget->setLayout(debug_layout);

// /////////////////////////////////////////////////////////////////
// View Widget
// /////////////////////////////////////////////////////////////////

    QHBoxLayout *view_layout = new QHBoxLayout;
    view_layout->setContentsMargins(0, 0, 0, 0);
    view_layout->setSpacing(0);
    view_layout->addWidget(d->view_manager);

    d->view_widget = new QWidget(this);
    d->view_widget->setLayout(view_layout);

// /////////////////////////////////////////////////////////////////
// Stack Widget
// /////////////////////////////////////////////////////////////////

    QStackedWidget *stack = new QStackedWidget(this);
    stack->addWidget(d->composer_widget);
    stack->addWidget(d->monitoring_widget);
    stack->addWidget(d->distribution_widget);
    stack->addWidget(d->debug_widget);
    stack->addWidget(d->view_widget);
    stack->setCurrentWidget(d->composer_widget);

// /////////////////////////////////////////////////////////////////
// Actions
// /////////////////////////////////////////////////////////////////

    d->composition_open_action = new QAction("Open", this);
    d->composition_open_action->setShortcut(QKeySequence::Open);

    d->composition_save_action = new QAction("Save", this);
    d->composition_save_action->setShortcut(QKeySequence::Save);

    d->composition_saveas_action = new QAction("Save As...", this);
    d->composition_saveas_action->setShortcut(QKeySequence::SaveAs);

    d->composition_insert_action = new QAction("Insert", this);
    d->composition_insert_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_I);

    d->composition_quit_action = new QAction("Quit", this);
    d->composition_quit_action->setShortcut(QKeySequence::Quit);

    d->undo_action = d->composer->stack()->createUndoAction(this);
    d->undo_action->setShortcut(QKeySequence::Undo);

    d->redo_action = d->composer->stack()->createRedoAction(this);
    d->redo_action->setShortcut(QKeySequence::Redo);

// /////////////////////////////////////////////////////////////////
// Toolbar
// /////////////////////////////////////////////////////////////////

    QToolBar *mainToolBar = this->addToolBar(tr("Main"));
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mainToolBar->setIconSize(QSize(32, 32));

    QAction *run_action = mainToolBar->addAction(QIcon(":dtkVisualProgramming/pixmaps/dtkCreatorToolbarButton_Run_Active.png"), "Run");
    run_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_R);

    QAction *step_action = mainToolBar->addAction(QIcon(":dtkVisualProgramming/pixmaps/dtkCreatorToolbarButton_Step_Active.png"), "Step");
    step_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_N);

    QAction *continue_action = mainToolBar->addAction(QIcon(":dtkVisualProgramming/pixmaps/dtkCreatorToolbarButton_Continue_Active.png"), "Cont");
    continue_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_C);

    QAction *next_action = mainToolBar->addAction(QIcon(":dtkVisualProgramming/pixmaps/dtkCreatorToolbarButton_Continue_Active.png"), "Next");
    next_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_T);

    QAction *stop_action = mainToolBar->addAction(QIcon(":dtkVisualProgramming/pixmaps/dtkCreatorToolbarButton_Stop_Active.png"), "Stop");
    stop_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_Period);

    QAction *reset_action = mainToolBar->addAction(QIcon(":dtkVisualProgramming/pixmaps/dtkCreatorToolbarButton_Reset_Active.png"), "Reset");
    reset_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_D);

    QFrame *buttons = new QFrame(this);
    buttons->setObjectName("dtkVisualProgrammingMainWindowSegmentedButtons");

    d->compo_button = new QPushButton("Composition", buttons);
    d->compo_button->setObjectName("dtkVisualProgrammingMainWindowSegmentedButtonLeft");
    d->compo_button->setFixedSize(75, 25);
    d->compo_button->setCheckable(true);
    d->compo_button->setChecked(true);

    d->montr_button = new QPushButton("Monitoring", buttons);
    d->montr_button->setObjectName("dtkVisualProgrammingMainWindowSegmentedButtonMiddle");
    d->montr_button->setFixedSize(75, 25);
    d->montr_button->setCheckable(true);
    d->montr_button->setChecked(false);

    d->distr_button = new QPushButton("Distribution", buttons);
    d->distr_button->setObjectName("dtkVisualProgrammingMainWindowSegmentedButtonMiddle");
    d->distr_button->setFixedSize(75, 25);
    d->distr_button->setCheckable(true);

    d->debug_button = new QPushButton("Debug", buttons);
    d->debug_button->setObjectName("dtkVisualProgrammingMainWindowSegmentedButtonMiddle");
    d->debug_button->setFixedSize(75, 25);
    d->debug_button->setCheckable(true);

    d->view_button = new QPushButton("View", buttons);
    d->view_button->setObjectName("dtkVisualProgrammingMainWindowSegmentedButtonRight");
    d->view_button->setFixedSize(75, 25);
    d->view_button->setCheckable(true);

    QButtonGroup *button_group = new QButtonGroup(this);
    button_group->setExclusive(true);
    button_group->addButton(d->compo_button, 0);
    button_group->addButton(d->montr_button, 1);
    button_group->addButton(d->distr_button, 2);
    button_group->addButton(d->debug_button, 3);
    button_group->addButton(d->view_button, 4);

    QHBoxLayout *buttons_layout = new QHBoxLayout(buttons);
    buttons_layout->setMargin(0);
    buttons_layout->setSpacing(0);
    buttons_layout->addWidget(d->compo_button);
    buttons_layout->addWidget(d->montr_button);
    buttons_layout->addWidget(d->distr_button);
    buttons_layout->addWidget(d->debug_button);
    buttons_layout->addWidget(d->view_button);

    mainToolBar->addWidget(new dtkSpacer(this));
    mainToolBar->addWidget(new dtkNotificationDisplay(this));
    mainToolBar->addWidget(new dtkSpacer(this));
    mainToolBar->addWidget(buttons);

// /////////////////////////////////////////////////////////////////
// Menus
// /////////////////////////////////////////////////////////////////

    QMenuBar *menu_bar = this->menuBar();

    d->recent_compositions_menu = new dtkRecentFilesMenu("Open recent...", this);

    d->composition_menu = menu_bar->addMenu("Composition");
    d->composition_menu->addAction(d->composition_open_action);
    d->composition_menu->addMenu(d->recent_compositions_menu);
    d->composition_menu->addAction(d->composition_save_action);
    d->composition_menu->addAction(d->composition_saveas_action);
    d->composition_menu->addSeparator();
    d->composition_menu->addAction(d->composition_insert_action);
    d->composition_menu->addSeparator();
    d->composition_menu->addAction(d->composition_quit_action);

    d->edit_menu = menu_bar->addMenu("Edit");
    d->edit_menu->addAction(d->composer->view()->searchAction());
    d->edit_menu->addSeparator();
    d->edit_menu->addAction(d->undo_action);
    d->edit_menu->addAction(d->redo_action);
    d->edit_menu->addSeparator();
    d->edit_menu->addAction(d->composer->scene()->flagAsBlueAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsGrayAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsGreenAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsOrangeAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsPinkAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsRedAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsYellowAction());
    d->edit_menu->addSeparator();
    d->edit_menu->addAction(d->composer->scene()->maskEdgesAction());
    d->edit_menu->addAction(d->composer->scene()->unmaskEdgesAction());

    dtkScreenMenu *screen_menu = new dtkScreenMenu("Screen",this);
    menu_bar->addMenu(screen_menu);

    QMenu *debug_menu = menu_bar->addMenu("Debug");
    debug_menu->addAction(run_action);
    debug_menu->addAction(step_action);
    debug_menu->addAction(continue_action);
    debug_menu->addAction(next_action);
    debug_menu->addAction(stop_action);
    debug_menu->addAction(reset_action);

// /////////////////////////////////////////////////////////////////
// Connections
// /////////////////////////////////////////////////////////////////

    connect(button_group, SIGNAL(buttonClicked(int)), stack, SLOT(setCurrentIndex(int)));

    connect(run_action, SIGNAL(triggered()), d->composer, SLOT(run()));
    connect(step_action, SIGNAL(triggered()), d->composer, SLOT(step()));
    connect(continue_action, SIGNAL(triggered()), d->composer, SLOT(cont()));
    connect(next_action, SIGNAL(triggered()), d->composer, SLOT(next()));
    connect(stop_action, SIGNAL(triggered()), d->composer, SLOT(stop()));
    connect(reset_action, SIGNAL(triggered()), d->composer, SLOT(reset()));

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(close()));

    connect(d->composer, SIGNAL(modified(bool)), d, SLOT(setModified(bool)));

    connect(d->composition_open_action, SIGNAL(triggered()), this, SLOT(compositionOpen()));
    connect(d->composition_save_action, SIGNAL(triggered()), this, SLOT(compositionSave()));
    connect(d->composition_saveas_action, SIGNAL(triggered()), this, SLOT(compositionSaveAs()));
    connect(d->composition_insert_action, SIGNAL(triggered()), this, SLOT(compositionInsert()));
    connect(d->composition_quit_action, SIGNAL(triggered()), qApp, SLOT(quit()));

    connect(d->recent_compositions_menu, SIGNAL(recentFileTriggered(const QString&)), this, SLOT(compositionOpen(const QString&)));
    connect(d->composer->scene(),SIGNAL(selectedNode(dtkComposerSceneNode*)), d->nodes, SLOT(onShowNodeDocumentation(dtkComposerSceneNode*)));

// ///////////////////////////////////////////////////////////////

    this->setCentralWidget(stack);
    this->setStyleSheet(dtkReadFile(":dtkVisualProgramming/dtkVisualProgramming.qss"));
    this->setUnifiedTitleAndToolBarOnMac(true);

    d->setCurrentFile("");
}

dtkVisualProgrammingMainWindow::~dtkVisualProgrammingMainWindow(void)
{
    delete d;
}

void dtkVisualProgrammingMainWindow::readSettings(void)
{
    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(600, 400)).toSize();
    move(pos);
    resize(size);
    settings.endGroup();
}

void dtkVisualProgrammingMainWindow::writeSettings(void)
{
    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.endGroup();
}

bool dtkVisualProgrammingMainWindow::compositionOpen(void)
{
    if(!d->maySave())
        return true;

    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");
    QString path = settings.value("last_open_dir", QDir::homePath()).toString();
    settings.endGroup();

    QFileDialog *dialog = new QFileDialog(this, tr("Open composition"), path, QString("dtk composition (*.dtk)"));
    dialog->setStyleSheet("background-color: none ; color: none;");
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->open(this, SLOT(compositionOpen(const QString&)));

    return true;
}

bool dtkVisualProgrammingMainWindow::compositionOpen(const QString& file)
{
    if(sender() == d->recent_compositions_menu && !d->maySave())
        return true;

    bool status = d->composer->open(file);

    if(status) {
        d->recent_compositions_menu->addRecentFile(file);
        d->setCurrentFile(file);
    }

    QFileInfo info(file);

    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");
    settings.setValue("last_open_dir", info.absolutePath());
    settings.endGroup();

    if(status)
        dtkNotify(QString("<div style=\"color: #006600\">Opened %1</div>").arg(info.baseName()), 3000);

    return status;
}

bool dtkVisualProgrammingMainWindow::compositionSave(void)
{
    bool status;

    if(d->current_composition.isEmpty() || d->current_composition == "untitled.dtk")
        status = this->compositionSaveAs();
    else
        status = d->composer->save();

    if(status)
        this->setWindowModified(false);

    if(status)
        dtkNotify(QString("<div style=\"color: #006600\">Saved %1</div>").arg(d->current_composition), 3000);

    return status;
}

bool dtkVisualProgrammingMainWindow::compositionSaveAs(void)
{
    bool status = false;

    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");
    QString path = settings.value("last_open_dir", QDir::homePath()).toString();
    settings.endGroup();

    QStringList nameFilters;
    nameFilters <<  "Ascii composition (*.dtk)";
    nameFilters << "Binary composition (*.dtk)";

    QFileDialog dialog(this, "Save composition as ...", path, QString("dtk composition (*.dtk)"));
    dialog.setStyleSheet("background-color: none ; color: none;");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setConfirmOverwrite(true);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilters(nameFilters);
    dialog.setDefaultSuffix("dtk");

    if(dialog.exec()) {

        if(dialog.selectedNameFilter() == nameFilters.at(0))
            status = this->compositionSaveAs(dialog.selectedFiles().first(), dtkComposerWriter::Ascii);
        else
            status = this->compositionSaveAs(dialog.selectedFiles().first(), dtkComposerWriter::Binary);
    }

    return status;
}

bool dtkVisualProgrammingMainWindow::compositionSaveAs(const QString& file, dtkComposerWriter::Type type)
{
    bool status = false;

    if(file.isEmpty())
        return status;

    status = d->composer->save(file, type);

    if(status) {
        d->setCurrentFile(file);
        this->setWindowModified(false);
    }

    QFileInfo info(file);

    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");
    settings.setValue("last_open_dir", info.absolutePath());
    settings.endGroup();

    if(status)
        dtkNotify(QString("<div style=\"color: #006600\">Saved as %1</div>").arg(info.baseName()), 3000);

    return status;
}

bool dtkVisualProgrammingMainWindow::compositionInsert(void)
{
    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");
    QString path = settings.value("last_open_dir", QDir::homePath()).toString();
    settings.endGroup();

    QFileDialog *dialog = new QFileDialog(this, tr("Insert composition"), path, QString("dtk composition (*.dtk)"));
    dialog->setStyleSheet("background-color: none ; color: none;");
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->open(this, SLOT(compositionInsert(const QString&)));

    return true;
}

bool dtkVisualProgrammingMainWindow::compositionInsert(const QString& file)
{
    bool status = d->composer->insert(file);

    if(status)
        this->setWindowModified(true);

    QFileInfo info(file);

    QSettings settings("inria", "dtk");
    settings.beginGroup("VisualProgramming");
    settings.setValue("last_open_dir", info.absolutePath());
    settings.endGroup();

    return status;
}

void dtkVisualProgrammingMainWindow::closeEvent(QCloseEvent *event)
{
    if (d->maySave()) {
         writeSettings();
         d->closing = true;
         event->accept();
     } else {
         event->ignore();
     }
}

//
// dtkVisualProgrammingMainWindow.cpp ends here
