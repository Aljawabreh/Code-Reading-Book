// pipe_proc_test.cpp,v 1.2 1998/10/15 04:50:37 nanbor Exp

#define  ACE_BUILD_SVC_DLL
#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"

ACE_RCSID(Synch_Benchmarks, pipe_proc_test, "pipe_proc_test.cpp,v 1.2 1998/10/15 04:50:37 nanbor Exp")

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Pipe_Proc_Test : public Benchmark_Performance
{
public:
  int init (int, char **);
  virtual int svc (void);

private:
  ACE_HANDLE pipe_handles[2];

  void reader (ACE_HANDLE handle);
};

int
Pipe_Proc_Test::init (int, char **)
{
  synch_count = 1;

  if (ACE_OS::pipe (this->pipe_handles) == -1)
    ACE_OS::perror ("pipe"), ACE_OS::exit (1);

  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_OS::perror ("fork"), ACE_OS::exit (1);
    case 0:
      this->reader (pipe_handles[0]);
      /* NOTREACHED */
      break;
    default:
      break;
    }
  return 1;
}

void
Pipe_Proc_Test::reader (ACE_HANDLE handle)
{
  int  ni = this->thr_id ();
  int length = performance_test_options.msg_size ();
  char *to;

  ACE_NEW (to, char[length]);

  while (ACE_OS::read (handle, to, length) > 0)
    performance_test_options.thr_work_count[ni]++;
}


int
Pipe_Proc_Test::svc (void)
{
  ssize_t length = performance_test_options.msg_size ();
  int ni = this->thr_id ();
  ACE_HANDLE handle = this->pipe_handles[1];
  char *from;

  ACE_NEW_RETURN (from, char[length], -1);

  while (!this->done ())
    if (ACE_OS::write (handle, from, length) == length)
      performance_test_options.thr_work_count[ni]++;
    else
      ACE_OS::perror ("write");

  ACE_OS::close (this->pipe_handles[0]);
  ACE_OS::close (this->pipe_handles[1]);
  return 0;
}

ACE_SVC_FACTORY_DECLARE (Pipe_Proc_Test)
ACE_SVC_FACTORY_DEFINE  (Pipe_Proc_Test)

// ACE_Service_Object_Type ppt (&pipe_proc_test, "Pipe_Proc_Test");
#endif /* ACE_HAS_THREADS */
