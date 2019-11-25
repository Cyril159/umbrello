/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

// own header file
#include "usecasewidget.h"

// app includes
#include "debug_utils.h"
#include "usecase.h"
#include "umlview.h"

DEBUG_REGISTER_DISABLED(UseCaseWidget)

/**
 *  Creates a UseCase widget.
 *  @param  scene  The parent of the widget.
 *  @param  o      The UMLUseCase to represent.
 */
UseCaseWidget::UseCaseWidget(UMLScene * scene, UMLUseCase *o)
  : UMLWidget(scene, WidgetBase::wt_UseCase, o)
{
}

/**
 * Destructor.
 */
UseCaseWidget::~UseCaseWidget()
{
}

/**
 * Overrides the standard paint event.
 */
void UseCaseWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    setPenFromSettings(painter);
    if (UMLWidget::useFillColor())
        painter->setBrush(UMLWidget::fillColor());
    QFont font = UMLWidget::font();
    font.setUnderline(false);
    font.setBold(false);
    font.setItalic(m_umlObject->isAbstract());
    painter->setFont(font);
    const QFontMetrics &fm = getFontMetrics(FT_NORMAL);
    const int fontHeight  = fm.lineSpacing();
    const int w = width();
    const int h = height();
    //int middleX = w / 2;
    bool drawStereotype = umlObject() && !umlObject()->stereotype().isEmpty();
    const int textStartY = (h / 2) - (drawStereotype ? fontHeight / 4 : fontHeight / 2);

    painter->drawEllipse(0, 0, w, h);
    painter->setPen(textColor());
    if (drawStereotype)
        painter->drawText(UC_MARGIN, textStartY-fontHeight, w - UC_MARGIN * 2, fontHeight, Qt::AlignCenter, umlObject()->stereotype(true));

    painter->drawText(UC_MARGIN, textStartY, w - UC_MARGIN * 2, fontHeight, Qt::AlignCenter, name());
    setPenFromSettings(painter);

    UMLWidget::paint(painter, option, widget);
}

/**
 * Saves this UseCase to file.
 */
void UseCaseWidget::saveToXMI1(QDomDocument & qDoc, QDomElement & qElement)
{
    QDomElement usecaseElement = qDoc.createElement(QLatin1String("usecasewidget"));
    UMLWidget::saveToXMI1(qDoc, usecaseElement);
    qElement.appendChild(usecaseElement);
}

/**
 * Overrides method from UMLWidget
 */
QSizeF UseCaseWidget::minimumSize() const
{
    const UMLWidget::FontType ft = (m_umlObject->isAbstract() ? FT_BOLD_ITALIC : FT_BOLD);
    const QFontMetrics &fm = UMLWidget::getFontMetrics(ft);
    const int fontHeight = fm.lineSpacing();
    const int textWidth = fm.width(name());
    bool drawStereotype = umlObject() && !umlObject()->stereotype().isEmpty();
    int width = textWidth > UC_WIDTH?textWidth:UC_WIDTH;
    int height = UC_HEIGHT + (drawStereotype ? 2 * fontHeight : fontHeight) + UC_MARGIN;

    width += UC_MARGIN * 2;

    return QSizeF(width, height);
}
