
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
 *      Date   : Thu Jun 19 2003
 */

#include <kdebug.h>

#include "xmlschemacodegenerator.h"
#include "xmlschemaclassifiercodedocument.h"
#include "xmlschemacodeclassfield.h"

// Constructors/Destructors
//

XMLSchemaCodeGenerator::XMLSchemaCodeGenerator ( UMLDoc * parentDoc , const char * name)
    : CodeGenerator( parentDoc, name ) {

	//m_parentDoc = parentDoc; // this should be done by the call to the parent constructor?
kdDebug()<<"XMLSchemaCodeGenerator "<<this<<" created with parent document:"<<parentDoc<<endl;

	initFields();
}

XMLSchemaCodeGenerator::~XMLSchemaCodeGenerator ( ) {
kdDebug()<<"XMLSchemaCodeGenerator "<<this<<" destroyed"<<endl;
}

//
// Methods
//

QString XMLSchemaCodeGenerator::getLanguage() {
        return "XMLSchema";
}

bool XMLSchemaCodeGenerator::isType (QString & type)
{
   if(type == "XMLSchemaCodeGenerator")
	return true;
   return false;
}


// Accessor methods
//


// Other methods
//

/**
 * @return	ClassifierCodeDocument
 * @param	classifier
 * @param	this This package generator object.
 */
CodeDocument * XMLSchemaCodeGenerator::newClassifierCodeDocument (UMLClassifier * classifier)
{
kdDebug()<<"Create New XMLSCHEMA Classifier doc"<<endl;
	return new XMLSchemaClassifierCodeDocument(classifier,this);
}

void XMLSchemaCodeGenerator::initFields ( ) {

        // setPolicy ( new JavaCodeGenerationPolicy(this) );

        // load Classifier documents from parent document
        initFromParentDocument();

}

// override parent method
void XMLSchemaCodeGenerator::initFromParentDocument( ) {

        // Walk through the document converting classifiers into
        // classifier code documents as needed (e.g only if doesnt exist)
        UMLClassifierList concepts = getDocument()->getClassesAndInterfaces();
        for (UMLClassifier *c = concepts.first(); c; c = concepts.next())
        {

                // Doesnt exist? Then build one.
                CodeDocument * codeDoc = findCodeDocumentByClassifier(c);
                if (!codeDoc)
                {
                        codeDoc = (CodeDocument*) newClassifierCodeDocument(c);
                        addCodeDocument(codeDoc); // this will also add a unique tag to the code document
                }
        }
}


#include "xmlschemacodegenerator.moc"
