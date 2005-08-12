/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.				   *
 *                                                                         *
 *  copyright (C) 2005                                                     *
 *  Umbrello UML Modeller Authors <uml-devel@ uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "idlimport.h"

#include <stdio.h>
// qt/kde includes
// #include <qprocess.h>  //should use this instead of popen()
#include <qstringlist.h>
#include <qregexp.h>
#include <klocale.h>
#include <kdebug.h>
// app includes
#include "import_utils.h"
#include "uml.h"
#include "umldoc.h"
#include "umlpackagelist.h"
#include "package.h"
#include "classifier.h"
#include "enum.h"
#include "operation.h"
#include "attribute.h"

IDLImport::IDLImport() : NativeImportBase("//") {
    m_isAbstract = m_isOneway = m_isReadonly = m_isAttribute = false;
    m_inComment = false;
}

IDLImport::~IDLImport() {
}

/// Check for split type names (e.g. unsigned long long)
QString IDLImport::joinTypename() {
    QString typeName = m_source[m_srcIndex];
    if (m_source[m_srcIndex] == "unsigned")
        typeName += ' ' + advance();
    if (m_source[m_srcIndex] == "long" &&
            (m_source[m_srcIndex + 1] == "long" || m_source[m_srcIndex + 1] == "double"))
        typeName += ' ' + advance();
    return typeName;
}

bool IDLImport::preprocess(QString& line) {
    // Ignore C preprocessor generated lines.
    if (line.startsWith("#"))
        return true;  // done
    // Check for end of multi line comment.
    if (m_inComment) {
        int pos = line.find("*/");
        if (pos == -1) {
            m_comment += line + "\n";
            return true;  // done
        }
        if (pos > 0) {
            QString text = line.mid(0, pos - 1);
            m_comment += text.stripWhiteSpace();
        }
        m_source.append(m_singleLineCommentIntro + m_comment);  // denotes comments in `m_source'
        m_comment = "";
        m_inComment = false;
        pos++;  // pos now points at the slash in the "*/"
        if (pos == (int)line.length() - 1)
            return true;  // done
        line = line.mid(pos + 1);
    }
    // If we get here then m_inComment is false.
    // Check for start of multi line comment.
    int pos = line.find("/*");
    if (pos != -1) {
        int endpos = line.find("*/");
        if (endpos == -1) {
            m_inComment = true;
            if (pos + 1 < (int)line.length() - 1) {
                QString cmnt = line.mid(pos + 2);
                m_comment += cmnt.stripWhiteSpace() + "\n";
            }
            if (pos == 0)
                return true;  // done
            line = line.left(pos);
        } else {   // It's a multiline comment on a single line.
            if (endpos > pos + 2)  {
                QString cmnt = line.mid(pos + 2, endpos - pos - 2);
                cmnt = cmnt.stripWhiteSpace();
                if (!cmnt.isEmpty())
                    m_source.append(m_singleLineCommentIntro + cmnt);
            }
            endpos++;  // endpos now points at the slash of "*/"
            QString pre;
            if (pos > 0)
                pre = line.left(pos);
            QString post;
            if (endpos < (int)line.length() - 1)
                post = line.mid(endpos + 1);
            line = pre + post;
        }
    }
    return false;  // The input was not completely consumed by preprocessing.
}

void IDLImport::fillSource(QString word) {
    QString lexeme;
    const uint len = word.length();
    for (uint i = 0; i < len; i++) {
        QChar c = word[i];
        if (c.isLetterOrNumber() || c == '_') {
            lexeme += c;
        } else if (c == ':' && word[i + 1] == ':') {
            // compress scoped name into lexeme
            lexeme += "::";
            i++;
        } else if (c == '<') {
            // compress sequence or bounded string into lexeme
            do {
                lexeme += word[i];
            } while (word[i] != '>' && ++i < len);
        } else {
            if (!lexeme.isEmpty()) {
                m_source.append(lexeme);
                lexeme = QString::null;
            }
            m_source.append(c);
        }
    }
    if (!lexeme.isEmpty())
        m_source.append(lexeme);
}

void IDLImport::parseFile(QString filename) {
    QStringList includePaths = Import_Utils::includePathList();
    //QProcess command("cpp", UMLAp::app());
    QString command("cpp -C");   // -C means "preserve comments"
    for (QStringList::Iterator pathIt = includePaths.begin();
            pathIt != includePaths.end(); ++pathIt) {
        QString path = (*pathIt);
        //command.addArgument(" -I" + path);
        command += " -I" + path;
    }
    command += " " + filename;
    kdDebug() << "importIDL: " << command << endl;
    FILE *fp = popen(command.ascii(), "r");
    if (fp == NULL) {
        kdError() << "IDLImport::parseFile: cannot popen(" << command << ")" << endl;
        return;
    }
    // Scan the input file into the QStringList m_source.
    m_source.clear();
    char buf[256];
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        int len = strlen(buf);
        if (buf[len - 1] == '\n')
            buf[--len] = '\0';
        NativeImportBase::scan( QString(buf) );
    }
    // Parse the QStringList m_source.
    m_scopeIndex = 0;
    m_scope[0] = NULL;
    const uint srcLength = m_source.count();
    for (m_srcIndex = 0; m_srcIndex < srcLength; m_srcIndex++) {
        const QString& keyword = m_source[m_srcIndex];
        //kdDebug() << '"' << keyword << '"' << endl;
        if (keyword.startsWith(m_singleLineCommentIntro)) {
            m_comment = keyword.mid(2);
            continue;
        }
        if (keyword == "module") {
            const QString& name = advance();
            UMLObject *ns = Import_Utils::createUMLObject(Uml::ot_Package,
                            name, m_scope[m_scopeIndex], m_comment);
            m_scope[++m_scopeIndex] = static_cast<UMLPackage*>(ns);
            m_scope[m_scopeIndex]->setStereotype("CORBAModule");
            if (advance() != "{") {
                kdError() << "importIDL: unexpected: " << m_source[m_srcIndex] << endl;
                skipStmt("{");
            }
            m_comment = QString::null;
            continue;
        }
        if (keyword == "interface") {
            const QString& name = advance();
            UMLObject *ns = Import_Utils::createUMLObject(Uml::ot_Class,
                            name, m_scope[m_scopeIndex], m_comment);
            m_scope[++m_scopeIndex] = m_klass = static_cast<UMLClassifier*>(ns);
            m_klass->setStereotype("CORBAInterface");
            m_klass->setAbstract(m_isAbstract);
            m_isAbstract = false;
            m_comment = QString::null;
            if (advance() == ";")   // forward declaration
                continue;
            if (m_source[m_srcIndex] == ":") {
                while (++m_srcIndex < srcLength && m_source[m_srcIndex] != "{") {
                    const QString& baseName = m_source[m_srcIndex];
                    Import_Utils::createGeneralization(m_klass, baseName);
                    if (advance() != ",")
                        break;
                }
            }
            if (m_source[m_srcIndex] != "{") {
                kdError() << "importIDL: ignoring excess chars at "
                << name << endl;
                skipStmt("{");
            }
            continue;
        }
        if (keyword == "struct" || keyword == "exception") {
            const QString& name = advance();
            UMLObject *ns = Import_Utils::createUMLObject(Uml::ot_Class,
                            name, m_scope[m_scopeIndex], m_comment);
            m_scope[++m_scopeIndex] = m_klass = static_cast<UMLClassifier*>(ns);
            if (keyword == "struct")
                m_klass->setStereotype("CORBAStruct");
            else
                m_klass->setStereotype("CORBAException");
            if (advance() != "{") {
                kdError() << "importIDL: expecting '{' at " << name << endl;
                skipStmt("{");
            }
            m_comment = QString::null;
            continue;
        }
        if (keyword == "union") {
            // TBD. <gulp>
            skipStmt("}");
            m_srcIndex++;  // advance to ';'
            continue;
        }
        if (keyword == "enum") {
            const QString& name = advance();
            UMLObject *ns = Import_Utils::createUMLObject(Uml::ot_Enum,
                            name, m_scope[m_scopeIndex], m_comment);
            UMLEnum *enumType = static_cast<UMLEnum*>(ns);
            m_srcIndex++;  // skip name
            while (++m_srcIndex < srcLength && m_source[m_srcIndex] != "}") {
                Import_Utils::addEnumLiteral(enumType, m_source[m_srcIndex]);
                if (advance() != ",")
                    break;
            }
            skipStmt();
            m_comment = QString::null;
            continue;
        }
        if (keyword == "typedef") {
            const QString& existingType = advance();
            const QString& newType = advance();
            Import_Utils::createUMLObject(Uml::ot_Class, newType, m_scope[m_scopeIndex],
                                         m_comment, "CORBATypedef" /* stereotype */);
            // @todo How do we convey the existingType ?
            skipStmt();
            continue;
        }
        if (keyword == "const") {
            skipStmt();
            continue;
        }
        if (keyword == "custom") {
            continue;
        }
        if (keyword == "abstract") {
            m_isAbstract = true;
            continue;
        }
        if (keyword == "valuetype") {
            const QString& name = advance();
            UMLObject *ns = Import_Utils::createUMLObject(Uml::ot_Class,
                            name, m_scope[m_scopeIndex], m_comment);
            m_scope[++m_scopeIndex] = m_klass = static_cast<UMLClassifier*>(ns);
            m_klass->setAbstract(m_isAbstract);
            m_isAbstract = false;
            if (advance() == ";")   // forward declaration
                continue;
            if (m_source[m_srcIndex] == ":") {
                if (advance() == "truncatable")
                    m_srcIndex++;
                while (m_srcIndex < srcLength && m_source[m_srcIndex] != "{") {
                    const QString& baseName = m_source[m_srcIndex];
                    Import_Utils::createGeneralization(m_klass, baseName);
                    if (advance() != ",")
                        break;
                    m_srcIndex++;
                }
            }
            if (m_source[m_srcIndex] != "{") {
                kdError() << "importIDL: ignoring excess chars at "
                << name << endl;
                skipStmt("{");
            }
            m_comment = QString::null;
            continue;
        }
        if (keyword == "public") {
            continue;
        }
        if (keyword == "private") {
            m_currentAccess = Uml::Private;
            continue;
        }
        if (keyword == "readonly") {
            m_isReadonly = true;
            continue;
        }
        if (keyword == "attribute") {
            m_isAttribute = true;
            continue;
        }
        if (keyword == "oneway") {
            m_isOneway = true;
            continue;
        }
        if (keyword == "}") {
            if (m_scopeIndex)
                m_klass = dynamic_cast<UMLClassifier*>(m_scope[--m_scopeIndex]);
            else
                kdError() << "importIDL: too many }" << endl;
            m_srcIndex++;  // skip ';'
            continue;
        }
        if (keyword == ";")
            continue;
        // At this point, we expect `keyword' to be a type name
        // (of a member of struct or valuetype, or return type
        // of an operation.) Up next is the name of the attribute
        // or operation.
        if (! keyword.contains( QRegExp("^\\w") )) {
            kdError() << "importIDL: ignoring " << keyword << endl;
            skipStmt();
            continue;
        }
        QString typeName = joinTypename();
        QString name = advance();
        if (name.contains( QRegExp("\\W") )) {
            kdError() << "importIDL: expecting name in " << name << endl;
            skipStmt();
            continue;
        }
        // At this point we most definitely need a class.
        if (m_klass == NULL) {
            kdError() << "importIDL: no class set for " << name << endl;
            continue;
        }
        if (advance() == "(") {
            // operation
            UMLOperation *op = Import_Utils::makeOperation(m_klass, name);
            m_srcIndex++;
            while (m_srcIndex < srcLength && m_source[m_srcIndex] != ")") {
                const QString &direction = m_source[m_srcIndex++];
                QString typeName = joinTypename();
                const QString &parName = advance();
                UMLAttribute *att = Import_Utils::addMethodParameter(op, typeName, parName);
                Uml::Parameter_Direction dir;
                if (Model_Utils::stringToDirection(direction, dir))
                    att->setParmKind(dir);
                else
                    kdError() << "importIDL: expecting parameter direction at "
                    << direction << endl;
                if (advance() != ",")
                    break;
                m_srcIndex++;
            }
            Import_Utils::insertMethod(m_klass, op, Uml::Public, typeName,
                                      false, false, false, false, m_comment);
            if (m_isOneway) {
                op->setStereotype("oneway");
                m_isOneway = false;
            }
            skipStmt();  // skip possible "raises" clause
            m_comment = QString::null;
            continue;
        }
        // At this point we know it's some kind of attribute declaration.
        UMLObject *o = Import_Utils::insertAttribute(m_klass, m_currentAccess, name, typeName, m_comment);
        UMLAttribute *attr = static_cast<UMLAttribute*>(o);
        if (m_isReadonly) {
            attr->setStereotype("readonly");
            m_isReadonly = false;
        }
        m_currentAccess = Uml::Public;
        if (m_source[m_srcIndex] != ";") {
            kdError() << "importIDL: ignoring trailing items at " << name << endl;
            skipStmt();
        }
        m_comment = QString::null;
    }
    pclose(fp);
}


