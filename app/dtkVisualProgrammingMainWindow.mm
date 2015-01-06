/* dtkCreatorMainWindow.mm --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008-2011 - Julien Wintz, Inria.
 * Created: Wed Feb  1 12:29:47 2012 (+0100)
 * Version: $Id$
 * Last-Updated: Mon Mar 19 22:43:45 2012 (+0100)
 *           By: Julien Wintz
 *     Update #: 18
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include "dtkVisualProgrammingMainWindow.h"
#include "dtkVisualProgrammingMainWindow_p.h"

#import <Cocoa/Cocoa.h>

#if defined(Q_OS_MAC) && (MAC_OS_X_VERSION_MAX_ALLOWED > MAC_OS_X_VERSION_10_6)

void dtkVisualProgrammingMainWindowPrivate::enableFullScreenSupport(void)
{
    NSView *nsview = (NSView *)(q->winId());
    NSWindow *nswindow = [nsview window];
    [nswindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
}

void dtkVisualProgrammingMainWindowPrivate::showFullScreen(void)
{
    NSView *nsview = (NSView *)(q->winId());
    NSWindow *nswindow = [nsview window];
    [nswindow toggleFullScreen:nil];
}

#endif
