/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2009                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#include "cmd_set_txt.h"

// kde includes
#include <klocale.h>
#include <kdebug.h>

namespace Uml
{

    CmdSetTxt::CmdSetTxt(FloatingTextWidget* _ftw, const QString& txt)
      : ftw(_ftw), newstring(txt)
    {
        oldstring = _ftw->text();
        uDebug() << "oldstring: "<< oldstring << ", newstring: "<< newstring;
    }

    CmdSetTxt::~CmdSetTxt()
    {
    }

    void CmdSetTxt::redo()
    {
        ftw->setText(newstring);
        uDebug() << "string after redo" << ftw->text()<< ", oldstring: "<< oldstring << ", newstring: "<< newstring;
    }

    void CmdSetTxt::undo()
    {
        ftw->setName("balbalbalbalbla");
        ftw->setText(oldstring);
        uDebug() << "string after undo: " << ftw->text()<< "oldstring: "<< oldstring << "newstring: "<< newstring;
    }

}
