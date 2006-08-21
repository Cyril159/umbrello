/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2006                                                    *
 *   Umbrello UML Modeller Authors <uml-devel@ uml.sf.net>                 *
 ***************************************************************************/

#ifndef UMLFOLDER_H
#define UMLFOLDER_H

#include "package.h"
#include "umlviewlist.h"

/**
 * This class manages the UMLObjects and UMLViews of a Folder.
 * This class inherits from UMLPackage which contains most
 * of the information.
 *
 * The UMLDoc class allocates a fixed instance of this class for
 * each of the predefined Logical, UseCase, Component, Deployment, and
 * Entity-Relationship folders.  Further instances are created on demand
 * for user folders.
 *
 * @short Non-graphical management of objects and diagrams of a Folder
 * @author Oliver Kellogg
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */

class UMLFolder : public UMLPackage {
    Q_OBJECT
public:
    /**
     * Sets up a Folder.
     *
     * @param name    The name of the Folder.
     * @param id      The unique id of the Folder. A new ID will be generated
     *                if this argument is left away.
     */
    UMLFolder(const QString & name = "", Uml::IDType id = Uml::id_None);

    /**
     * Empty deconstructor.
     */
    virtual ~UMLFolder();

    /**
     * Initializes key variables of the class.
     */
    virtual void init();

    /**
     * Make a clone of this object.
     */
    virtual UMLObject* clone() const;

    /**
     * Mark this folder as being for Umbrello special use.
     * This is set for the predefined root views (Logical,
     * UseCase, Component, Deployment, EntityRelationship,
     * and the Datatypes folder inside the Logical View.)
     */
    void markPredefined();

    /**
     * Return whther this folder is one of the Umbrello special
     * folders.
     */
    bool isPredefined();

    /**
     * Add a view to the diagram list.
     */
    void addView(UMLView *view);

    /**
     * Set the folder file name for a separate submodel.
     */
    void setFolderFile(QString fileName);

    /**
     * Get the folder file name for a separate submodel.
     */
    QString getFolderFile();

    /**
     * Creates a UML:Model or UML:Package element:
     * UML:Model is created for the predefined fixed folders,
     * UML:Package with stereotype <<folder>> is created for all else.
     */
    void saveToXMI( QDomDocument & qDoc, QDomElement & qElement );

protected:
    /**
     * Loads the UML:Component element.
     */
    bool load(QDomElement & element);

private:
    bool m_bPredefined;
    QString m_folderFile;
    UMLViewList m_diagrams;
};

#endif
