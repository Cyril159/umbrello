//own header
#include "instanceattribute.h"

//local includes
#include "debug_utils.h"
#include "umldoc.h"
#include "uml.h"
#include "umlinstanceattributedialog.h"
#include "object_factory.h"

//kde includes
#include <KLocalizedString>
#include <KMessageBox>

UMLInstanceAttribute::UMLInstanceAttribute(UMLObject *parent, const QString &name, Uml::ID::Type id, Uml::Visibility::Enum s, UMLObject *type, const QString &value)
    : UMLAttribute(parent, name, id, s, type, value)
{
    init();
    if(m_pSecondary){
        m_pSecondary->setBaseType(UMLObject::ot_Instance);
    }
}

UMLInstanceAttribute::UMLInstanceAttribute(UMLObject *parent)
    : UMLAttribute(parent)
{
    init();
}

/**
 * @brief UMLInstanceAttribute::init
 * Initialize members of this class
 */
void UMLInstanceAttribute::init()
{
    m_BaseType = UMLObject::ot_InstanceAttribute;
}

/**
 * Sets the UMLInstanceAttribute's attributes property.
 * @param attributes  The new value for the attributes property.
 */
void UMLInstanceAttribute::setAttributes(const QString &attributes)
{
    m_attributes = attributes;
}

/**
 * Returns the value of the UMLInstanceAttribute's attributes property.
 * @return  The value of the UMLInstanceAttribute's attributes property.
 */
QString UMLInstanceAttribute::getAttributes() const
{
    return m_attributes;
}

/**
 * Creates the <UML:InstanceAttribute> XMI element.
 */
void UMLInstanceAttribute::saveToXMI(QDomDocument &qDoc, QDomElement &qElement)
{
    QDomElement instanceAttributeElement = UMLObject::save(QLatin1String("UML:InstanceAttribute"), qDoc);
    if (m_pSecondary == NULL) {
        uDebug() << name() << ": m_pSecondary is NULL, using local name " << m_SecondaryId;
        instanceAttributeElement.setAttribute(QLatin1String("type"), m_SecondaryId);
    } else {
        instanceAttributeElement.setAttribute(QLatin1String("type"), Uml::ID::toString(m_pSecondary->id()));
    }
    instanceAttributeElement.setAttribute(QLatin1String("initialValue"), m_InitialValue);
    instanceAttributeElement.setAttribute(QLatin1String("attributes"), m_attributes);
    qElement.appendChild(instanceAttributeElement);
}

/**
 * Loads the <UML:InstanceAttribute> XMI element.
 */
bool UMLInstanceAttribute::load(QDomElement & element)
{
    if (! UMLAttribute::load(element))
        return false;
    m_attributes = element.attribute(QLatin1String("attributes"));
    m_InitialValue = element.attribute(QLatin1String("initialValue"));
    return true;
}

/**
 * Display the properties configuration dialog for the instanceattribute.
 */
bool UMLInstanceAttribute::showPropertiesDialog(QWidget* parent)
{
    UMLInstanceAttributeDialog dialog(parent, this);
    return dialog.exec();
}
