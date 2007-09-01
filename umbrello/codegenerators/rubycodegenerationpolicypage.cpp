/***************************************************************************
                          rubycodegenerationpolicypage.cpp
                          Derived from the Java code generator by thomas

    begin                : Thur Jul 21 2005
    author               : Richard Dale
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2006-2007                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "rubycodegenerationpolicypage.h"
// qt/kde includes
#include <qlabel.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <kdebug.h>
#include <klocale.h>
// app includes
#include "../codegenerationpolicy.h"
#include "../uml.h"

RubyCodeGenerationPolicyPage::RubyCodeGenerationPolicyPage( QWidget *parent, const char *name, RubyCodeGenerationPolicy * policy )
  : CodeGenerationPolicyPage(parent, name, policy)
{
    CodeGenerationPolicy *common = UMLApp::app()->getCommonPolicy();
    form.setupUi(this);
    form.m_SelectCommentStyle->setCurrentItem((int)(common->getCommentStyle()));
    form.m_generateConstructors->setChecked(common->getAutoGenerateConstructors());
    form.m_generateAttribAccessors->setChecked(policy->getAutoGenerateAttribAccessors());
    form.m_generateAssocAccessors->setChecked(policy->getAutoGenerateAssocAccessors());
    form.m_accessorScopeCB->setCurrentItem(common->getAttributeAccessorScope());
    form.m_assocFieldScopeCB->setCurrentItem(common->getAssociationFieldScope());
}

RubyCodeGenerationPolicyPage::~RubyCodeGenerationPolicyPage()
{
}

void RubyCodeGenerationPolicyPage::apply()
{
    CodeGenerationPolicy *common = UMLApp::app()->getCommonPolicy();

    // now do our ruby-specific configs
    RubyCodeGenerationPolicy * parent = (RubyCodeGenerationPolicy*) m_parentPolicy;

    // block signals so we don't cause too many update content calls to code documents
    parent->blockSignals(true);

    common->setCommentStyle((CodeGenerationPolicy::CommentStyle) form.m_SelectCommentStyle->currentItem());
    common->setAttributeAccessorScope((Uml::Visibility::Value) form.m_accessorScopeCB->currentItem());
    common->setAssociationFieldScope((Uml::Visibility::Value) form.m_assocFieldScopeCB->currentItem());
    common->setAutoGenerateConstructors(form.m_generateConstructors->isChecked());
    parent->setAutoGenerateAttribAccessors(form.m_generateAttribAccessors->isChecked());
    parent->setAutoGenerateAssocAccessors(form.m_generateAssocAccessors->isChecked());

    parent->blockSignals(false);

    // now send out modified code content signal
    common->emitModifiedCodeContentSig();

}


#include "rubycodegenerationpolicypage.moc"
