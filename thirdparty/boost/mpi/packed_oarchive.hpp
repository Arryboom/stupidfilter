// (C) Copyright 2005 Matthias Troyer
// (C) Copyright 2006 Douglas Gregor <doug.gregor -at- gmail.com>

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer
//           Douglas Gregor

/** @file packed_oarchive.hpp
 *
 *  This header provides the facilities for unpacking Serializable
 *  data types from a buffer using @c MPI_Unpack. The buffers are
 *  typically received via MPI and have been packed either by via the
 *  facilities in @c packed_iarchive.hpp or @c MPI_Pack.
 */
#ifndef BOOST_MPI_PACKED_OARCHIVE_HPP
#define BOOST_MPI_PACKED_OARCHIVE_HPP

#include <boost/mpi/datatype.hpp>
#include <boost/archive/detail/auto_link_archive.hpp>
#include <boost/archive/basic_binary_oarchive.hpp>
#include <boost/mpi/detail/packed_oprimitive.hpp>

namespace boost { namespace mpi {

/** @brief An archive that unpacks binary data from an MPI buffer.
 *
 *  The @c packed_oarchive class is an Archiver (as in the
 *  Boost.Serialization library) that unpacks binary data from a
 *  buffer received via MPI. It can operate on any Serializable data
 *  type and will use the @c MPI_Unpack function of the underlying MPI
 *  implementation to perform deserialization.
 */
class BOOST_MPI_DECL packed_oarchive
  : public packed_oprimitive,
    public archive::basic_binary_oarchive<packed_oarchive>
{
public:
  /**
   *  Construct a @c packed_oarchive to receive data over the given
   *  MPI communicator and with an initial buffer.
   *
   *  @param comm The communicator over which this archive will be
   *  received.
   *
   *  @param b A user-defined buffer that contains the binary
   *  representation of serialized objects.
   *
   *  @param flags Control the serialization of the data types. Refer
   *  to the Boost.Serialization documentation before changing the
   *  default flags.
   */
  packed_oarchive( MPI_Comm const & comm, buffer_type & b, unsigned int flags = boost::archive::no_header)
         : packed_oprimitive(b,comm),
           archive::basic_binary_oarchive<packed_oarchive>(flags)
        {}

  /**
   *  Construct a @c packed_oarchive to receive data over the given
   *  MPI communicator.
   *
   *  @param comm The communicator over which this archive will be
   *  received.
   *
   *  @param flags Control the serialization of the data types. Refer
   *  to the Boost.Serialization documentation before changing the
   *  default flags.
   */
  packed_oarchive ( MPI_Comm const & comm, unsigned int flags =  boost::archive::no_header)
         : packed_oprimitive(internal_buffer_,comm),
           archive::basic_binary_oarchive<packed_oarchive>(flags)
        {}


private:
  /// An internal buffer to be used when the user does not supply his
  /// own buffer.
  buffer_type internal_buffer_;
};

} } // end namespace boost::mpi

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::mpi::packed_oarchive)


#endif // BOOST_MPI_PACKED_OARCHIVE_HPP
