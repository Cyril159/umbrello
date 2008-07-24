/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2008                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header file
#include "usecasewidget.h"

// app includes
#include "textitemgroup.h"
#include "textitem.h"
#include "umlview.h"
#include "usecase.h"

// qt includes
#include <QtGui/QPainter>

/**
 *  Creates a UseCase widget.
 *
 *  @param  o The UMLObject to represent.
 */
UseCaseWidget::UseCaseWidget(UMLUseCase *o) : NewUMLRectWidget(o)
{
    m_baseType = Uml::wt_UseCase;
    createTextItemGroup();
}

/**
 *  destructor
 */
UseCaseWidget::~UseCaseWidget()
{
}

void UseCaseWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *o, QWidget *)
{
    painter->setPen(QPen(lineColor(), lineWidth()));
    painter->setBrush(brush());

    painter->drawEllipse(rect());
}

/**
 *   Saves this UseCase to file.
 */
void UseCaseWidget::saveToXMI( QDomDocument & qDoc, QDomElement & qElement )
{
    QDomElement usecaseElement = qDoc.createElement( "usecasewidget" );
    NewUMLRectWidget::saveToXMI( qDoc, usecaseElement );
    qElement.appendChild( usecaseElement );
}

void UseCaseWidget::updateGeometry()
{
	TextItemGroup *grp = textItemGroupAt(GroupIndex);
	setMinimumSize(grp->minimumSize());
	NewUMLRectWidget::updateGeometry();
}

void UseCaseWidget::updateTextItemGroups()
{
    if(umlObject()) {
		TextItemGroup *grp = textItemGroupAt(GroupIndex);
		grp->setTextItemCount(UseCaseWidget::TextItemCount);

        TextItem *nameItem = grp->textItemAt(UseCaseWidget::NameItemIndex);
        nameItem->setText(name());
        nameItem->setItalic(umlObject()->getAbstract());
    }
    NewUMLRectWidget::updateTextItemGroups();
}

QVariant UseCaseWidget::attributeChange(WidgetAttributeChange change, const QVariant& oldValue)
{
	if(change == SizeHasChanged) {
		TextItemGroup *grp = textItemGroupAt(GroupIndex);
		const qreal m = margin();
		grp->setGroupGeometry(rect().adjusted(+m, +m, -m, -m));
	}
	return NewUMLRectWidget::attributeChange(change, oldValue);
}
