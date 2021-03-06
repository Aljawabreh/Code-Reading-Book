
//=============================================================================
/**
 *  @file    Remote_Object_Proxy_Broker.h
 *
 *  Remote_Object_Proxy_Broker.h,v 1.2 2000/12/15 20:06:49 coryan Exp
 *
 *  This files contains the Remote Proxy Broker implementation
 *  for the CORBA Object.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_REMOTE_OBJECT_PROXY_BROKER_H_
#define TAO_REMOTE_OBJECT_PROXY_BROKER_H_

// -- ACE Include --
#include "ace/pre.h"

// -- TAO Include --
#include "tao/corbafwd.h"
#include "tao/Object_Proxy_Broker.h"
#include "tao/Remote_Object_Proxy_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export TAO_Remote_Object_Proxy_Broker : public TAO_Object_Proxy_Broker
{
public:
  // -- Ctor/Dtor --
  TAO_Remote_Object_Proxy_Broker (void);
  ~TAO_Remote_Object_Proxy_Broker (void);

  virtual TAO_Object_Proxy_Impl &select_proxy (CORBA::Object_ptr object,
                                               CORBA::Environment &ACE_TRY_ENV);

private:
  TAO_Remote_Object_Proxy_Impl remote_proxy_impl_;
};

TAO_Remote_Object_Proxy_Broker *the_tao_remote_object_proxy_broker (void);

#include "ace/post.h"

#endif /* TAO_REMOTE_Object_PROXY_BROKER_H_ */
