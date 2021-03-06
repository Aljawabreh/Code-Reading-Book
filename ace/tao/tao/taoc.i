// -*- C++ -*-
//
// TAOC.i,v 1.10 2001/06/12 18:58:15 fhunleth Exp TAOC.i,v 1.8 2001/01/17 02:59:45 parsons Exp $ */

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// *************************************************************
// Inline operations for class TAO::PrioritySpecification_var
// *************************************************************

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

// *************************************************************
// Inline operations for class TAO::BufferingConstraint_var
// *************************************************************

ACE_INLINE
TAO::BufferingConstraint_var::BufferingConstraint_var (void) // default constructor
  : ptr_ (0)
{}

ACE_INLINE
TAO::BufferingConstraint_var::BufferingConstraint_var (BufferingConstraint *p)
  : ptr_ (p)
{}

ACE_INLINE
TAO::BufferingConstraint_var::BufferingConstraint_var (const ::TAO::BufferingConstraint_var &p) // copy constructor
{
  if (p.ptr_)
    ACE_NEW (this->ptr_, ::TAO::BufferingConstraint (*p.ptr_));
  else
    this->ptr_ = 0;
}

// fixed-size types only
ACE_INLINE
TAO::BufferingConstraint_var::BufferingConstraint_var (const ::TAO::BufferingConstraint &p)
{
  ACE_NEW (this->ptr_, ::TAO::BufferingConstraint (p));
}

ACE_INLINE
TAO::BufferingConstraint_var::~BufferingConstraint_var (void) // destructor
{
  delete this->ptr_;
}

ACE_INLINE TAO::BufferingConstraint_var &
TAO::BufferingConstraint_var::operator= (BufferingConstraint *p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

ACE_INLINE ::TAO::BufferingConstraint_var &
TAO::BufferingConstraint_var::operator= (const ::TAO::BufferingConstraint_var &p)
{
  if (this != &p)
    {
      if (p.ptr_ == 0)
        {
          delete this->ptr_;
          this->ptr_ = 0;
        }
      else
        {
          TAO::BufferingConstraint *deep_copy =
            new TAO::BufferingConstraint (*p.ptr_);

          if (deep_copy != 0)
            {
              TAO::BufferingConstraint *tmp = deep_copy;
              deep_copy = this->ptr_;
              this->ptr_ = tmp;
              delete deep_copy;
            }
        }
    }

  return *this;
}

// fixed-size types only
ACE_INLINE TAO::BufferingConstraint_var &
TAO::BufferingConstraint_var::operator= (const ::TAO::BufferingConstraint &p)
{
  if (this->ptr_ != &p)
  {
    delete this->ptr_;
    ACE_NEW_RETURN (this->ptr_, ::TAO::BufferingConstraint (p), *this);
  }
  return *this;
}

ACE_INLINE const ::TAO::BufferingConstraint *
TAO::BufferingConstraint_var::operator-> (void) const
{
  return this->ptr_;
}

ACE_INLINE ::TAO::BufferingConstraint *
TAO::BufferingConstraint_var::operator-> (void)
{
  return this->ptr_;
}

ACE_INLINE
TAO::BufferingConstraint_var::operator const ::TAO::BufferingConstraint &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE
TAO::BufferingConstraint_var::operator ::TAO::BufferingConstraint &() // cast
{
  return *this->ptr_;
}

ACE_INLINE
TAO::BufferingConstraint_var::operator ::TAO::BufferingConstraint &() const // cast
{
  return *this->ptr_;
}

ACE_INLINE const ::TAO::BufferingConstraint &
TAO::BufferingConstraint_var::in (void) const
{
  return *this->ptr_;
}

ACE_INLINE ::TAO::BufferingConstraint &
TAO::BufferingConstraint_var::inout (void)
{
  return *this->ptr_;
}

// mapping for fixed size
ACE_INLINE ::TAO::BufferingConstraint &
TAO::BufferingConstraint_var::out (void)
{
  return *this->ptr_;
}

ACE_INLINE ::TAO::BufferingConstraint
TAO::BufferingConstraint_var::_retn (void)
{
  return *this->ptr_;
}

ACE_INLINE ::TAO::BufferingConstraint *
TAO::BufferingConstraint_var::ptr (void) const
{
  return this->ptr_;
}

#if !defined (_TAO_BUFFERINGCONSTRAINTPOLICY___CI_)
#define _TAO_BUFFERINGCONSTRAINTPOLICY___CI_

ACE_INLINE TAO::BufferingConstraintPolicy_ptr
tao_TAO_BufferingConstraintPolicy_duplicate (
    TAO::BufferingConstraintPolicy_ptr p
  )
{
  return TAO::BufferingConstraintPolicy::_duplicate (p);
}

ACE_INLINE void
tao_TAO_BufferingConstraintPolicy_release (
    TAO::BufferingConstraintPolicy_ptr p
  )
{
  CORBA::release (p);
}

ACE_INLINE TAO::BufferingConstraintPolicy_ptr
tao_TAO_BufferingConstraintPolicy_nil (
    void
  )
{
  return TAO::BufferingConstraintPolicy::_nil ();
}

ACE_INLINE TAO::BufferingConstraintPolicy_ptr
tao_TAO_BufferingConstraintPolicy_narrow (
    CORBA::Object *p,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  return TAO::BufferingConstraintPolicy::_narrow (p, ACE_TRY_ENV);
}

ACE_INLINE CORBA::Object *
tao_TAO_BufferingConstraintPolicy_upcast (
    void *src
  )
{
  TAO::BufferingConstraintPolicy **tmp =
    ACE_static_cast (TAO::BufferingConstraintPolicy **, src);
  return *tmp;
}


#endif /* end #if !defined */

ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, const TAO::BufferingConstraint &_tao_aggregate)
{
  if (
    (strm << _tao_aggregate.mode) &&
    (strm << _tao_aggregate.timeout) &&
    (strm << _tao_aggregate.message_count) &&
    (strm << _tao_aggregate.message_bytes)
  )
    return 1;
  else
    return 0;

}

ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, TAO::BufferingConstraint &_tao_aggregate)
{
  if (
    (strm >> _tao_aggregate.mode) &&
    (strm >> _tao_aggregate.timeout) &&
    (strm >> _tao_aggregate.message_count) &&
    (strm >> _tao_aggregate.message_bytes)
  )
    return 1;
  else
    return 0;

}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
