/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// own header
#include "template.h"

// qt/kde includes
#include <qregexp.h>
#include <kdebug.h>

// app includes
#include "uml.h"
#include "umldoc.h"
#include "dialogs/umltemplatedialog.h"

UMLTemplate::UMLTemplate(const UMLObject *parent, QString name, int id, QString type)
  : UMLClassifierListItem( parent, name, id ) {
	setTypeName( type );
	m_BaseType = Uml::ot_Template;
}

UMLTemplate::UMLTemplate(const UMLObject *parent)
  : UMLClassifierListItem( parent ) {
	m_BaseType = Uml::ot_Template;
}

UMLTemplate::~UMLTemplate() {}

QString UMLTemplate::toString(Uml::Signature_Type /*sig = st_NoSig*/) {
	if (m_pSecondary == NULL || m_pSecondary->getName() == "class") {
		return getName();
	} else {
		return getName() + " : " + m_pSecondary->getName();
	}
}

bool UMLTemplate::operator==(UMLTemplate &rhs) {
	if (this == &rhs) {
		return true;
	}
	if ( !UMLObject::operator==( rhs ) ) {
		return false;
	}
	if (m_pSecondary != rhs.m_pSecondary) {
		return false;
	}
	return true;
}

void UMLTemplate::copyInto(UMLTemplate *rhs) const
{
	UMLClassifierListItem::copyInto(rhs);
}

UMLObject* UMLTemplate::clone() const
{
	UMLTemplate *clone = new UMLTemplate( (UMLTemplate*) parent());
	copyInto(clone);

	return clone;
}


void UMLTemplate::saveToXMI(QDomDocument& qDoc, QDomElement& qElement) {
	//FIXME: uml13.dtd compliance
	QDomElement attributeElement = UMLObject::save("template", qDoc);
	if (m_pSecondary)
		attributeElement.setAttribute("type", m_pSecondary->getID());
	qElement.appendChild(attributeElement);
}

bool UMLTemplate::load(QDomElement& element) {
	m_SecondaryId = element.attribute("type", "");
	if (m_SecondaryId.contains( QRegExp("^\\d+$") )) {
		UMLDoc *pDoc = UMLApp::app()->getDocument();
		m_pSecondary = pDoc->findUMLObject( m_SecondaryId.toInt() );
		if (m_pSecondary)
			m_SecondaryId = "";
		else
			kdDebug() << "UMLTemplate::load: Cannot find UML object"
				  << " for type " << m_SecondaryId << endl;
	}
	return true;
}

bool UMLTemplate::showPropertiesDialogue(QWidget* parent) {
	UMLTemplateDialog dialogue(parent, this);
	return dialogue.exec();
}
