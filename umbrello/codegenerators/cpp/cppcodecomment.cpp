/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003      Brian Thomas <thomas@mail630.gsfc.nasa.gov>   *
 *   copyright (C) 2004-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

// own header
#include "cppcodecomment.h"

// qt includes
#include <QRegExp>


CPPCodeComment::CPPCodeComment (CodeDocument * doc, const QString & text)
        : CodeComment (doc, text)
{
}

CPPCodeComment::~CPPCodeComment ()
{
}

void CPPCodeComment::saveToXMI1 (QDomDocument & doc, QDomElement & root)
{
    QDomElement blockElement = doc.createElement(QLatin1String("cppcodecomment"));
    setAttributesOnNode(doc, blockElement); // as we added no additional fields to this class we may
    // just use parent TextBlock method
    root.appendChild(blockElement);
}

QString CPPCodeComment::toString () const
{
    QString output;

    // simple output method
    if(getWriteOutText())
    {
        QString indent = getIndentationString();
        QString endLine = getNewLineEndingChars();
        output.append(formatMultiLineText (getText() + endLine, indent + QLatin1String("// "), endLine));
    }

    return output;
}

QString CPPCodeComment::getNewEditorLine (int amount)
{
    QString line = getIndentationString(amount) + QLatin1String("// ");
    return line;
}

QString CPPCodeComment::unformatText (const QString & text, const QString & indent)
{
    // remove leading or trailing comment stuff
    QString mytext = TextBlock::unformatText(text, indent);

    // now leading slashes
    mytext.remove(QRegExp(QLatin1String("^\\/\\/\\s*")));
    return mytext;
}
