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

public:
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

protected:
    void closeEvent(QCloseEvent *event);

private:
    dtkVisualProgrammingMainWindowPrivate *d;
};

//
// dtkVisualProgrammingMainWindow.h ends here
