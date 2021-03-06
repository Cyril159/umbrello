/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003-2020                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#ifndef STEREOTYPE_H
#define STEREOTYPE_H

#include "umlobject.h"

/**
 * This class is used to set up information for a stereotype.
 * Stereotypes are used essentially as properties of
 * attributes and operations etc.
 *
 * Each stereotype object is reference counted, i.e. client code
 * manages it such that it comes into existence as soon as there is
 * at least one user, and ceases existing when the number of users
 * drops to 0.
 * m_refCount reflects the number of users. It is externally managed,
 * i.e. client code must take care to call incrRefCount() and
 * decrRefCount() as appropriate.
 *
 * The one and only owner of all stereotypes is the UMLDoc, and the
 * ownership is specially managed (umlPackage() returns NULL for a
 * UMLStereotype.) The reason for this special treatment is that
 * class UMLDoc does not inherit from class UMLPackage, and therefore
 * setUMLPackage() cannot be used for stereotypes.
 *
 * @short Sets up stereotype information.
 * @author Jonathan Riddell
 * @author Oliver Kellogg
 * Bugs and comments to umbrello-devel@kde.org or https://bugs.kde.org
 */
class UMLStereotype : public UMLObject
{
    Q_OBJECT
public:
    explicit UMLStereotype(const QString &name, Uml::ID::Type id = Uml::ID::None);
    UMLStereotype();

    virtual ~UMLStereotype();

    bool operator==(const UMLStereotype &rhs) const;

    virtual void copyInto(UMLObject *lhs) const;

    virtual UMLObject* clone() const;

    void incrRefCount();
    void decrRefCount();

    int refCount() const;

    QString name(bool includeAdornments=false) const;

    void saveToXMI1(QXmlStreamWriter& writer);

    virtual bool showPropertiesDialog(QWidget* parent);

protected:
    int m_refCount;

};

#endif
