/* -*- C++ -*- */
// AbstractInterfaceDef_i.cpp,v 1.2 2001/04/04 18:46:49 parsons Exp

#include "AbstractInterfaceDef_i.h"
#include "Repository_i.h"

ACE_RCSID(IFR_Service, AbstractInterfaceDef_i, "AbstractInterfaceDef_i.cpp,v 1.2 2001/04/04 18:46:49 parsons Exp")

TAO_AbstractInterfaceDef_i::TAO_AbstractInterfaceDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_Container_i (repo, section_key),
    TAO_Contained_i (repo, section_key),
    TAO_IDLType_i (repo, section_key),
    TAO_InterfaceDef_i (repo, section_key)
{
}

TAO_AbstractInterfaceDef_i::~TAO_AbstractInterfaceDef_i (void)
{
}

CORBA::DefinitionKind
TAO_AbstractInterfaceDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_AbstractInterface;
}

CORBA::Boolean 
TAO_AbstractInterfaceDef_i::is_a (const char *interface_id,
                                  CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->is_a_i (interface_id,
                       ACE_TRY_ENV);
}

CORBA::Boolean 
TAO_AbstractInterfaceDef_i::is_a_i (const char *interface_id,
                                    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (ACE_OS::strcmp (interface_id, "IDL:omg.org/CORBA/AbstractBase:1.0") == 0)
    {
      return 1;
    }

  return this->TAO_InterfaceDef_i::is_a_i (interface_id,
                                           ACE_TRY_ENV);
}

