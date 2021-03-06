
// barrier.cpp,v 1.5 1999/09/22 03:13:56 jcej Exp

#include "Barrier_i.h"
#include "ace/Task.h"

/* We'll use a simple Task<> derivative to test our new Barrier
   object.
*/
class Test : public ACE_Task<ACE_NULL_SYNCH>
{
public:

        // Construct the object with a desired thread count
    Test(int _threads);

        // Open/begin the test.  As usual, we have to match the
        // ACE_Task signature.
    int open(void * _unused = 0);

        // Change the threads_ value for the next invocation of open()
    void threads(int _threads);

        // Get the current threads_ value.
    int threads(void);

        // Perform the test
    int svc(void);

protected:
        // How many threads the barrier will test.
    int threads_;

        // The Barrier object we'll use in our tests below
    Barrier barrier_;
};

/* Construct the object & initialize the threads value for open() to
   use.
*/
Test::Test(int _threads)
        : threads_(_threads)
{
}

/* As usual, our open() will create one or more threads where we'll do
   the interesting work.
*/
int Test::open(void * _unused)
{
    ACE_UNUSED_ARG(_unused);

        // One thing about the barrier:  You have to tell it how many
        // threads it will be synching.  The threads() mutator on my
        // Barrier class lets you do that and hides the implementation
        // details at the same time.
    barrier_.threads(threads_);

        // Activate the tasks as usual...  Like the other cases where
        // we're joining (or waiting for) our threads, we can't use
        // THR_DETACHED.
    return this->activate(THR_NEW_LWP, threads_);
}

void Test::threads(int _threads)
{
    threads_ = _threads;
}

int Test::threads(void)
{
    return threads_;
}

/* svc() will execute in each thread & do a few things with the
   Barrier we have.
 */
int Test::svc(void)
{
    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() Entry\n"));

        // Initialize the random number generator.  We'll use this to
        // create sleep() times in each thread.  This will help us see
        // if the barrier synch is working.
    ACE_Time_Value now(ACE_OS::gettimeofday());
    ACE_RANDR_TYPE seed = now.usec();
    ACE_OS::srand(seed);
    int delay;

        // After saying hello above, sleep for a random amount of time
        // from 1 to 6 seconds.  That will cause the next message
        // "Entering wait()" to be staggered on the output as each
        // thread's sleep() returns.
    delay = ACE_OS::rand_r(seed)%5;
    ACE_OS::sleep(abs(delay)+1);

        // When executing the app you should see these messages
        // staggered in an at-most 6 second window.  That is, you
        // won't likely see them all at once.
    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() Entering wait()\n"));

        // All of the threads will now wait at this point.  As each
        // thread finishes the sleep() above it will join the waiters.
    if( barrier_.wait() == -1 )
    {
        ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tbarrier_.wait() failed!\n"));
        return 0;
    }

        // When all threads have reached wait() they will give us this
        // message.  If you execute this, you should see all of the
        // "Everybody together" messages at about the same time.
    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() Everybody together?\n"));

        // Now we do the sleep() cycle again...
    delay = ACE_OS::rand_r(seed)%5;
    ACE_OS::sleep(abs(delay)+1);

        // As before, these will trickle in over a few seconds.
    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() Entering done()\n"));

        // This time we call done() instead of wait().  done()
        // actually invokes wait() but before returning here, it will
        // clean up a few resources.  The goal is to prevent carrying
        // around objects you don't need.
    if( barrier_.done() == -1 )
    {
        ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tbarrier_.done() failed!\n"));
        return 0;
    }

        // Since done() invokes wait() internally, we'll see this
        // message from each thread simultaneously
    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() Is everyone still here?\n"));

        // A final sleep()
    delay = ACE_OS::rand_r(seed)%5;
    ACE_OS::sleep(abs(delay)+1);

        // These should be randomly spaced like all of the other
        // post-sleep messages.
    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() Chaos and anarchy for all!\n"));

    return(0);
}

/* Our test application...
 */
int main(int, char**)
{
        // Create the test object with 10 threads
    Test test(10);

        // and open it to test the barrier.
    test.open();
        // Now wait for them all to exit.
    test.wait();

        // Re-open the Test object with just 5 threads
    test.threads(5);
    test.open();
        // and wait for them to complete also.
    test.wait();

    return(0);
}
