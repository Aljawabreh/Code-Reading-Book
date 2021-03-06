// client.h,v 1.4 2001/03/26 21:17:13 coryan Exp

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Xt_Stopwatch/Stopwatch_client
//
// = FILENAME
//   client.h
//
// = AUTHOR
//   Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef _QT_CLIENT_H
#define _QT_CLIENT_H

#include "testC.h"

#if defined (ACE_HAS_QT)

#include "tao/qt_resource.h"
#include <qapplication.h>
#include <qvbox.h>
#include <qslider.h>
#include <qpushbutton.h>

class Client : public QObject
{
  Q_OBJECT
public:

  Client (CORBA::ORB_ptr orb,
          QApplication &app);
  // ctor

  ~Client (void);
  //Dtor..

  void create_widgets (CORBA::Environment &);
  // Adds the callbacks to the GUI underneath.....

  void show (void);

  void parse_args (int argc, char *argv[],
                   CORBA::Environment &ACE_TRY_ENV);

  QVBox box_;
  // A box widget..

 public slots:
   void remote_call (int val);
   void shutdown_call (void);
   // Two slot handlers for the two widgets that we have

private:

  QPushButton *push_button_;
  // A push button

  QSlider *slider_;
  // A slider widget

  CORBA::ORB_var orb_;
  // The ORB

  LCD_Display_var server_;

  QApplication *qapp_;
};

#endif /*ACE_HAS_QT*/

#endif /* _QT_CLIENT_H */
