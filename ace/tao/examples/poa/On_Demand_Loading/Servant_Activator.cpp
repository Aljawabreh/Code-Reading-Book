// Servant_Activator.cpp,v 1.21 2000/07/14 12:07:44 irfan Exp

// ============================================================================
//
// = LIBRARY
//     TAO/examples/POA/On_Demand_Loading
//
// = FILENAME
//     Servant_Activator.cpp
//
// = DESCRIPTION
//     Implementation of <ServantActivator_i>, which is used by a POA
//     with a RETAIN policy.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "Servant_Activator.h"

ACE_RCSID(On_Demand_Loading, Servant_Activator, "Servant_Activator.cpp,v 1.21 2000/07/14 12:07:44 irfan Exp")

// Initialization.

ServantActivator_i::ServantActivator_i (CORBA::ORB_ptr orb)
  : servant_manager_ (orb)
{
}

// This method associates an servant with the ObjectID.

PortableServer::Servant
ServantActivator_i::incarnate (const PortableServer::ObjectId &oid,
                               PortableServer::POA_ptr poa
                               TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;
  // Convert ObjectId to String.
   CORBA::String_var s =
     PortableServer::ObjectId_to_string (oid);

  // Activate and return the servant else exception.
  PortableServer::Servant servant =
    this->servant_manager_.obtain_servant (s.in (),
                                           poa,
                                           27);
  if (servant != 0)
    return servant;
  else
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      0);
}

// This is the method invoked when the object is deactivated or the
// entire POA is is deactivated or destroyed.

void
ServantActivator_i::etherealize (const PortableServer::ObjectId &oid,
                                 PortableServer::POA_ptr,
                                 PortableServer::Servant servant,
                                 CORBA::Boolean,
                                 CORBA::Boolean remaining_activations
                                 TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // If there are no remaining activations i.e ObjectIds associated
  // with MyFooServant object, deactivate it.  Etheralization happens
  // on POA::destroy() and/or Object::deactivate().

  if (remaining_activations == 0)
    this->servant_manager_.destroy_servant (servant, oid);
}

// This method returns an ObjectId when given an dll name and the
// factory function to be invoked in the dll.  The format of the
// ObjectId is <dllname:factory_function>.

PortableServer::ObjectId_var
ServantActivator_i::create_dll_object_id (const char *dllname,
                                          const char *factory_function)
{
  return this->servant_manager_.create_dll_object_id (dllname,
                                                      factory_function);
}
