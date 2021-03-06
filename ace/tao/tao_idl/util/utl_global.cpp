// utl_global.cpp,v 1.69 2001/05/25 18:57:21 cleeland Exp

/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

 */

// utl_global.cc - Implementation of class IDL_GlobalData
//

#include        "idl.h"
#include        "idl_extern.h"
#include        "ace/OS.h"

ACE_RCSID(util, utl_global, "utl_global.cpp,v 1.69 2001/05/25 18:57:21 cleeland Exp")

// Define an increment for the size of the array used to store names of
// included files
#undef          INCREMENT
#define         INCREMENT       64

IDL_GlobalData::IDL_GlobalData (void)
    : pd_scopes (0),
      pd_root (0),
      pd_gen (0),
      pd_err (0),
      pd_err_count (0),
      pd_lineno (0),
      pd_filename (0),
      pd_main_filename (0),
      pd_real_filename (0),
      pd_stripped_filename (0),
      pd_import (I_FALSE),
      pd_in_main_file (I_FALSE),
      pd_prog_name (0),
      pd_cpp_location (0),
      pd_compile_flags (0),
      pd_be (0),
      pd_local_escapes (0),
      pd_indent (0),
      pd_pragmas (0),
      pd_read_from_stdin (I_FALSE),
      pd_include_file_names (0),
      pd_n_include_file_names (0),
      pd_n_alloced_file_names (0),
      included_idl_files_ (0),
      n_included_idl_files_ (0),
      n_allocated_idl_files_ (0),
      pd_parse_state (PS_NoState),
      pd_idl_src_file (0),
      gperf_path_ (0),
      temp_dir_ (0),
      ident_string_ (0),
      obv_support_ (I_FALSE),
      case_diff_error_ (I_TRUE),
      idl_flags_ ("")
 {
  // Path for the perfect hash generator(gperf) program.
  // Default is $ACE_ROOT/bin/gperf unless ACE_GPERF is defined.
  // Use ACE_GPERF if $ACE_ROOT hasn't been set or won't be set
  // in the environment.
  // Form the absolute pathname.
  char* ace_root = ACE_OS::getenv ("ACE_ROOT");
  if (ace_root == 0)
    // This may not cause any problem if -g option is used to specify
    // the correct path for the  gperf program. Let us ignore this
    // error here. It will be caught when we check the existence of
    // the perfect hasher and at that time, we can switch over to some
    // other scheme.
    {
#if defined (ACE_GPERF)
      // The actual gperf program must be included in the definition of
      // ACE_GPERF, not just the directory in which it is located.
      const char ace_gperf[] = ACE_GPERF;
      ACE_NEW (this->gperf_path_,
               char [ACE_OS::strlen (ace_gperf) + 1]);
      ACE_OS::sprintf (this->gperf_path_,
                       "%s",
                       ace_gperf);
#else
      this->gperf_path_ = 0;
#endif
    }
  else
    {
      // Set it to the default value.
      ACE_NEW (this->gperf_path_,
               char [ACE_OS::strlen (ace_root) + ACE_OS::strlen ("/bin/gperf") + 1]);
#if defined (ACE_WIN32)
      ACE_OS::sprintf (this->gperf_path_,
                       "%s\\bin\\gperf",
                       ace_root);
#else /* Not ACE_WIN32 */
      ACE_OS::sprintf (this->gperf_path_,
                       "%s/bin/gperf",
                       ace_root);
#endif /* ACE_WIN32 */
    }
}

IDL_GlobalData::~IDL_GlobalData (void)
{  
}

// Get or set scopes stack
UTL_ScopeStack *
IDL_GlobalData::scopes (void)
{
  return this->pd_scopes;
}

void
IDL_GlobalData::set_scopes (UTL_ScopeStack *s)
{
  this->pd_scopes = s;
}

// Get or set root of AST
AST_Root *
IDL_GlobalData::root (void)
{
  return this->pd_root;
}

void
IDL_GlobalData::set_root (AST_Root *r)
{
  this->pd_root = r;
}

// Get or set generator object
AST_Generator *
IDL_GlobalData::gen (void)
{
  return this->pd_gen;
}

void
IDL_GlobalData::set_gen (AST_Generator *g)
{
  this->pd_gen = g;
}

// Get or set error object
UTL_Error *
IDL_GlobalData::err (void)
{
  return this->pd_err;
}

void
IDL_GlobalData::set_err (UTL_Error *e)
{
  this->pd_err = e;
}

// Get or set error count
long
IDL_GlobalData::err_count (void)
{
  return this->pd_err_count;
}

void
IDL_GlobalData::set_err_count (long c)
{
  this->pd_err_count = c;
}

// Get or set line number
long
IDL_GlobalData::lineno (void)
{
  return this->pd_lineno;
}

void
IDL_GlobalData::set_lineno (long n)
{
  this->pd_lineno = n;
}

// Get or set file name being read now
UTL_String *
IDL_GlobalData::filename (void)
{
  return this->pd_filename;
}

void
IDL_GlobalData::set_filename (UTL_String *s)
{
  if (this->pd_filename != 0)
    {
      this->pd_filename->destroy ();
      delete this->pd_filename;
      this->pd_filename = 0;
    }

  this->pd_filename = s;
}

// Get or set main file name
UTL_String *
IDL_GlobalData::main_filename (void)
{
  return this->pd_main_filename;
}

void
IDL_GlobalData::set_main_filename (UTL_String *n)
{
  if (this->pd_main_filename != 0)
    {
      this->pd_main_filename->destroy ();
      delete this->pd_main_filename;
      this->pd_main_filename = 0;
    }

  this->pd_main_filename = n;
}

// Get or set real file name
UTL_String *
IDL_GlobalData::real_filename (void)
{
  return this->pd_real_filename;
}

void
IDL_GlobalData::set_real_filename (UTL_String *n)
{
  if (this->pd_real_filename != 0)
    {
      this->pd_real_filename->destroy ();
      delete this->pd_real_filename;
      this->pd_real_filename = 0;
    }

  this->pd_real_filename = n;
}

// Get or set indicator whether import is on
idl_bool
IDL_GlobalData::imported (void)
{
  return this->pd_in_main_file ? I_FALSE : pd_import;
}

idl_bool
IDL_GlobalData::import (void)
{
  return this->pd_import;
}

void
IDL_GlobalData::set_import (idl_bool is_in)
{
  this->pd_import = is_in;
}

// Get or set indicator whether we're reading the main file now
idl_bool
IDL_GlobalData::in_main_file (void)
{
  return this->pd_in_main_file;
}

void
IDL_GlobalData::set_in_main_file (idl_bool is_in)
{
  this->pd_in_main_file = is_in;
}

// Get or set stripped file name
UTL_String *
IDL_GlobalData::stripped_filename (void)
{
  return this->pd_stripped_filename;
}

void
IDL_GlobalData::set_stripped_filename (UTL_String *nm)
{
  if (this->pd_stripped_filename != 0)
    delete this->pd_stripped_filename;

  this->pd_stripped_filename = nm;
}

// Get or set cache value for argv[0]
const char *
IDL_GlobalData::prog_name (void)
{
  return this->pd_prog_name;
}

void
IDL_GlobalData::set_prog_name (const char *pn)
{
  this->pd_prog_name = pn;
}

// Get or set location to find C preprocessor
const char *
IDL_GlobalData::cpp_location (void)
{
  return this->pd_cpp_location;
}

void
IDL_GlobalData::set_cpp_location (const char *l)
{
  this->pd_cpp_location = l;
}

// Get or set IDL compiler flags
long
IDL_GlobalData::compile_flags (void)
{
  return this->pd_compile_flags;
}

void
IDL_GlobalData::set_compile_flags (long cf)
{
  this->pd_compile_flags = cf;
}

// Get or set BE to be used
const char *
IDL_GlobalData::be (void)
{
  return this->pd_be;
}

void
IDL_GlobalData::set_be (const char *nbe)
{
  this->pd_be = nbe;
}

// Get or set local escapes string. This provides additional mechanism
// to pass information to a BE.
char *
IDL_GlobalData::local_escapes (void)
{
  return this->pd_local_escapes;
}

void
IDL_GlobalData::set_local_escapes (const char *e)
{
  if (this->pd_local_escapes != 0)
    {
      ACE_OS::free (this->pd_local_escapes);
    }

  this->pd_local_escapes = ACE_OS::strdup (e);
}

// Get or set indent object
UTL_Indenter *
IDL_GlobalData::indent (void)
{
  return this->pd_indent;
}

void
IDL_GlobalData::set_indent (UTL_Indenter *i)
{
  this->pd_indent = i;
}

// Get or set list of pragmas being parsed
UTL_StrList *
IDL_GlobalData::pragmas (void)
{
  UTL_StrList *p = this->pd_pragmas;

  this->pd_pragmas = 0;
  return p;
}

void
IDL_GlobalData::set_pragmas (UTL_StrList *p)
{
  this->pd_pragmas = p;
}

// Get or set indicator whether we're reading from stdin.
idl_bool
IDL_GlobalData::read_from_stdin (void)
{
  return this->pd_read_from_stdin;
}

void
IDL_GlobalData::set_read_from_stdin (idl_bool r)
{
  this->pd_read_from_stdin = r;
}

// Have we seen this include file name before?
long
IDL_GlobalData::seen_include_file_before (UTL_String *n)
{
  unsigned long i;

  for (i = 0; i < this->pd_n_include_file_names; i++)
    if (n->compare (this->pd_include_file_names[i]))
      return I_TRUE;
  return I_FALSE;
}

// Store a name of an #include file
void
IDL_GlobalData::store_include_file_name (UTL_String *n)
{
  UTL_String        **o_include_file_names;
  unsigned long o_n_alloced_file_names, i;

  /*
   * Check if we need to store it at all or whether we've seen it already
   */
  if (seen_include_file_before (n))
    return;
  /*
   * OK, need to store. Make sure there's space for one more string
   */
  if (this->pd_n_include_file_names == this->pd_n_alloced_file_names)
    {
      // Allocating more space.

      if (this->pd_n_alloced_file_names == 0)
        {
          this->pd_n_alloced_file_names = INCREMENT;
          this->pd_include_file_names = new UTL_String *[this->pd_n_alloced_file_names];
        }
      else
        {
          o_include_file_names = this->pd_include_file_names;
          o_n_alloced_file_names = this->pd_n_alloced_file_names;
          this->pd_n_alloced_file_names += INCREMENT;
          this->pd_include_file_names = new UTL_String *[this->pd_n_alloced_file_names];
          for (i = 0; i < o_n_alloced_file_names; i++)
            this->pd_include_file_names[i] = o_include_file_names[i];
          delete [] o_include_file_names;
        }
    }

  // Store it.
  this->pd_include_file_names[this->pd_n_include_file_names++] = n;
}

void
IDL_GlobalData::set_include_file_names (UTL_String **ns)
{
  this->pd_include_file_names = ns;
}

UTL_String **
IDL_GlobalData::include_file_names (void)
{
  return this->pd_include_file_names;
}

void
IDL_GlobalData::set_n_include_file_names (unsigned long n)
{
  pd_n_include_file_names = n;
}

unsigned long
IDL_GlobalData::n_include_file_names (void)
{
  return pd_n_include_file_names;
}

// Access methods to deal with other IDL files included in the main
// IDL file.

void
IDL_GlobalData::add_to_included_idl_files (char* file_name)
{
  // Is there enough space there to store one more file.
  if (this->n_included_idl_files_ == this->n_allocated_idl_files_)
    {
      // Allocating more space.
      if (this->n_allocated_idl_files_ == 0)
        {
          // First time creation.
          this->n_allocated_idl_files_ = INCREMENT;
          ACE_NEW (this->included_idl_files_,
                   char *[this->n_allocated_idl_files_]);
        }
      else
        {
          // Adding more storage.

          char** old_included_idl_files =
            this->included_idl_files_;
          size_t n_old_allocated_idl_files =
            this->n_allocated_idl_files_;
          this->n_allocated_idl_files_ += INCREMENT;
          ACE_NEW (this->included_idl_files_,
                   char *[this->n_allocated_idl_files_]);
          for (size_t i = 0; i < n_old_allocated_idl_files; i++)
            this->included_idl_files_ [i] = old_included_idl_files [i];
          delete [] old_included_idl_files;
        }
    }

  // Store it.
  this->included_idl_files_ [this->n_included_idl_files_++] = file_name;
}

char**
IDL_GlobalData::included_idl_files (void)
{
  return this->included_idl_files_;
}

size_t
IDL_GlobalData::n_included_idl_files (void)
{
  return this->n_included_idl_files_;
}

// Set the number of included_idl_files. Use this carefully. This
// method is used when we validate all the #included idl files,
// against the ones that we get after preprocessing.
void
IDL_GlobalData::n_included_idl_files (size_t n)
{
  this->n_included_idl_files_ = n;
}

// Validate the included idl files, somefiles might have been
// ignored by the preprocessor.
void
IDL_GlobalData::validate_included_idl_files (void)
{
  // Flag to make sure we dont repeat things.
  static int already_done = 0;

  if (already_done == 1)
    return;

  already_done = 1;

  // New number of included_idl_files.
  size_t newj = 0;

  for (size_t j = 0;
       j < idl_global->n_included_idl_files ();
       j++)
    {
      // Get the base part.
      char *base_part = ACE_OS::strrchr (idl_global->included_idl_files ()[j],
                                         '/');

      // If no / then take the whole name. We dont need the /
      // anyway.
      if (base_part == 0)
        base_part = idl_global->included_idl_files ()[j];
      else
        base_part++;

      // Check this name with the names list that we got from the
      // preprocessor.
      size_t valid_file = 0;
      for (size_t ni = 0;
           ni < idl_global->n_include_file_names ();
           ni++)
        {
          char *file_name = idl_global->include_file_names ()[ni]->get_string ();

          if (ACE_OS::strstr (file_name, base_part) != 0)
            {
              // This file name is valid.
              valid_file = 1;
              break;
            }
        }

      // Remove the file, if it is not valid.
      if (valid_file == 0)
        {
          delete idl_global->included_idl_files ()[j];
          idl_global->included_idl_files ()[j] = 0;
        }
      else
        {
          // File is valid.

          // Move it to new index if necessary.
          if (j != newj)
            {
              // Move to the new index position.
              idl_global->included_idl_files ()[newj] =
                idl_global->included_idl_files ()[j];

              // Make old position 0.
              idl_global->included_idl_files ()[j] = 0;
            }

          // Increment the new index.
          newj++;
        }
    }

  // Now adjust the count on the included_idl_files.
  idl_global->n_included_idl_files (newj);
}

void
IDL_GlobalData::set_parse_state(ParseState ps)
{
  pd_parse_state = ps;
}

IDL_GlobalData::ParseState
IDL_GlobalData::parse_state()
{
  return pd_parse_state;
}

/*
 * Convert a PredefinedType to an ExprType
 */
AST_Expression::ExprType
IDL_GlobalData::PredefinedTypeToExprType(AST_PredefinedType::PredefinedType pt)
{
  switch (pt) {
  case AST_PredefinedType::PT_long:
    return AST_Expression::EV_long;
  case AST_PredefinedType::PT_ulong:
    return AST_Expression::EV_ulong;
  case AST_PredefinedType::PT_short:
    return AST_Expression::EV_short;
  case AST_PredefinedType::PT_ushort:
    return AST_Expression::EV_ushort;
  case AST_PredefinedType::PT_longlong:
    return AST_Expression::EV_longlong;
  case AST_PredefinedType::PT_ulonglong:
    return AST_Expression::EV_ulonglong;
  case AST_PredefinedType::PT_float:
    return AST_Expression::EV_float;
  case AST_PredefinedType::PT_double:
    return AST_Expression::EV_double;
  case AST_PredefinedType::PT_longdouble:
    return AST_Expression::EV_longdouble;
  case AST_PredefinedType::PT_char:
    return AST_Expression::EV_char;
  case AST_PredefinedType::PT_wchar:
    return AST_Expression::EV_wchar;
  case AST_PredefinedType::PT_octet:
    return AST_Expression::EV_octet;
  case AST_PredefinedType::PT_boolean:
    return AST_Expression::EV_bool;
  case AST_PredefinedType::PT_void:
    return AST_Expression::EV_void;
  default:
    return AST_Expression::EV_any;
  }
}

// returns the IDL source file being copiled
UTL_String* IDL_GlobalData::idl_src_file()
{
  return this->pd_idl_src_file;
}

// set the source IDL file that is being parsed
void IDL_GlobalData::idl_src_file(UTL_String *s)
{
  this->pd_idl_src_file = s;
}

void
IDL_GlobalData::temp_dir (const char *s)
{
  // Delete the old pointer.
  delete [] this->temp_dir_;

  // Allocate memory, 1 for the end of string.
  ACE_NEW (this->temp_dir_,
           char [ACE_OS::strlen (s) +
                ACE_OS::strlen (ACE_DIRECTORY_SEPARATOR_STR) +
                1]);

  // Copy the strings.
  ACE_OS::sprintf (this->temp_dir_,
                   "%s%s",
                   s,
                   ACE_DIRECTORY_SEPARATOR_STR);
}

const char *
IDL_GlobalData::temp_dir (void) const
{
  return this->temp_dir_;
}

void
IDL_GlobalData::gperf_path (const char *s)
{
  delete [] this->gperf_path_;
  this->gperf_path_ = ACE::strnew (s);
}

const char *
IDL_GlobalData::gperf_path (void) const
{
  return this->gperf_path_;
}

void
IDL_GlobalData::ident_string (const char *s)
{
  delete [] this->ident_string_;
  this->ident_string_ = ACE::strnew (s);
}

const char *
IDL_GlobalData::ident_string (void) const
{
  return this->ident_string_;
}

#ifdef IDL_HAS_VALUETYPE
void
IDL_GlobalData::obv_support (idl_bool val)
{
  this->obv_support_ = val;
}
#endif /* IDL_HAS_VALUETYPE */

idl_bool
IDL_GlobalData::obv_support (void)
{
  return this->obv_support_;
}

void
IDL_GlobalData::case_diff_error (idl_bool val)
{
  this->case_diff_error_ = val;
}

idl_bool
IDL_GlobalData::case_diff_error (void)
{
  return this->case_diff_error_;
}

void
IDL_GlobalData::destroy (void)
{
  if (this->pd_filename != 0)
    {
      this->pd_filename->destroy ();
      delete this->pd_filename;
      this->pd_filename = 0;
    }

  if (this->pd_main_filename != 0)
    {
      this->pd_main_filename->destroy ();
      delete this->pd_main_filename;
      this->pd_main_filename = 0;
    }

  if (this->pd_real_filename != 0)
    {
      this->pd_real_filename->destroy ();
      delete this->pd_real_filename;
      this->pd_real_filename = 0;
    }

  if (this->pd_stripped_filename != 0)
    {
      this->pd_stripped_filename->destroy ();
      delete this->pd_stripped_filename;
      this->pd_stripped_filename = 0;
    }

  delete [] this->ident_string_;
  this->ident_string_ = 0;

  // Should do pragmas here.
}

void
IDL_GlobalData::append_idl_flag (const char *s)
{
  idl_flags_ += " " + ACE_CString (s);     
}

const char *
IDL_GlobalData::idl_flags (void) const
{
  return idl_flags_.c_str ();
}

ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex> &
IDL_GlobalData::idl_keywords (void)
{
  return this->idl_keywords_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<ACE_CString, int>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Equal_To<ACE_CString>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash<ACE_CString>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, int>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Equal_To<ACE_CString>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash<ACE_CString>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, int, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
