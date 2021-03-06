//
// Server_Task.cpp,v 1.1 2001/05/09 22:38:20 coryan Exp
//
#include "Server_Task.h"

ACE_RCSID(AMI_Latency, Server_Task, "Server_Task.cpp,v 1.1 2001/05/09 22:38:20 coryan Exp")

Server_Task::Server_Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Server_Task::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY {} ACE_ENDTRY;
  return 0;
}
