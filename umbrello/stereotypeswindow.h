/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2015-2020                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#ifndef STEREOTYPESWINDOW_H
#define STEREOTYPESWINDOW_H

#include <QDockWidget>
#include <QModelIndex>

class QTableView;

class StereotypesWindow : public QDockWidget
{
    Q_OBJECT
public:
    explicit StereotypesWindow(const QString &title, QWidget *parent = 0);
    ~StereotypesWindow();

signals:

public slots:
    void modified();

protected slots:
    void slotStereotypesDoubleClicked(QModelIndex index);

protected:
    QTableView *m_stereotypesTree;
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // STEREOTYPESWINDOW_H
