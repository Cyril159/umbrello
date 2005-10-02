
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Mon Jun 23 2003
 */



#ifndef JAVACLASSIFIERCODEDOCUMENT_H
#define JAVACLASSIFIERCODEDOCUMENT_H

#include <qstring.h>
#include "../codeclassfieldlist.h"
#include "../classifiercodedocument.h"
#include "../classifier.h"
#include "../hierarchicalcodeblock.h"
#include "classifierinfo.h"
#include "javacodeclassfield.h"
#include "javacodeoperation.h"

class JavaCodeGenerator;
class JavaClassDeclarationBlock;
class JavaCodeGenerationPolicy;

/**
  * class JavaClassifierCodeDocument
  * A Java UMLClassifier Code Document.
  */

class JavaClassifierCodeDocument : public ClassifierCodeDocument
{
    Q_OBJECT
public:

    // Constructors/Destructors
    //


    /**
     * Constructor
     */
    JavaClassifierCodeDocument (UMLClassifier * classifier , JavaCodeGenerator * parent);

    /**
     * Empty Destructor
     */
    virtual ~JavaClassifierCodeDocument ( );

    /**
     * Get the dialog widget which allows user interaction with the object parameters.
     * @return  CodeDocumentDialog
     */
    //CodeDocumentDialog getDialog ( );

    QString scopeToJavaDecl(Uml::Visibility scope);
    QString capitalizeFirstLetter(const QString &string);

    // Make it easier on ourselves
    JavaCodeGenerationPolicy * getJavaPolicy();

    QString getJavaClassName (const QString &name);

    QString getPath();

    /** add a code operation to this java classifier code document.
     *  @return bool which is true IF the code operation was added successfully
     */
    bool addCodeOperation (CodeOperation * op );

    /**
            * create a new CodeClassField declaration block object belonging to this CodeDocument.
     */
    virtual CodeClassFieldDeclarationBlock * newDeclarationCodeBlock (CodeClassField * cf);

    /**
      * create a new CodeAccesorMethod object belonging to this CodeDocument.
      * @return      CodeAccessorMethod
      */
    virtual CodeAccessorMethod * newCodeAccessorMethod( CodeClassField *cf, CodeAccessorMethod::AccessorType type );

    /**
      * create a new CodeOperation object belonging to this CodeDocument.
      * @return      CodeOperation
      */
    virtual CodeOperation * newCodeOperation( UMLOperation * op );

    /**
     * create a new code comment. IN this case it is a JavaCodeComment.
     */
    virtual CodeComment * newCodeComment ( );

protected:

    // reset/clear our inventory of textblocks in this document
    void resetTextBlocks();

    /**
      * need to overwrite this for java since we need to pick up the 
    * java class declaration block.
      */
    virtual void loadChildTextBlocksFromNode ( QDomElement & root);

    /** create new code classfield for this document.
     */
    virtual CodeClassField * newCodeClassField( UMLAttribute *at);
    virtual CodeClassField * newCodeClassField( UMLRole *role);

    void addOrUpdateCodeClassFieldMethodsInCodeBlock(CodeClassFieldList &list, JavaClassDeclarationBlock * codeBlock);

    bool forceDoc ();

    void updateContent();

private:

    JavaClassDeclarationBlock * classDeclCodeBlock;
    HierarchicalCodeBlock * constructorBlock;
    HierarchicalCodeBlock * operationsBlock;

    ClassifierInfo * info;
    QString fileName; // Just for our convience in creating code
    QString endLine; // characters for ending line. Just for our convience in creating code
    QString JavaClassName;

    void init ( );
    JavaClassDeclarationBlock * getClassDecl();


};

#endif // JAVACLASSIFIERCODEDOCUMENT_H
