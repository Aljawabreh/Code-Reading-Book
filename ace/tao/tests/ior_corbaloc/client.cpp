// client.cpp,v 1.5 2001/05/28 03:17:33 crodrigu Exp
//

// ============================================================================
//
// = LIBRARY
//    TAO/tests/ior_corbaloc/
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//     This implements a simple CORBA client for the
//     corbaloc: style IOR parser
//
// = AUTHOR
//     Priyanka Gontla <pgontla@ece.uci.edu>
//
//
// ============================================================================

#include "ior_corbaloc_client_i.h"

int main (int argc, char *argv [])
{

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {

      IOR_corbaloc_Client_i client;

      int init_result;
      init_result = client.init (argc, argv, ACE_TRY_ENV);
        ACE_TRY_CHECK;

        if (init_result == 0)
          {
            client.run (ACE_TRY_ENV);
            ACE_TRY_CHECK;
          }
        return 0;
    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      //
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
