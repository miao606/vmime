//
// VMime library (http://vmime.sourceforge.net)
// Copyright (C) 2002-2004 Vincent Richard <vincent@vincent-richard.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef VMIME_UTILITY_FILE_HPP_INCLUDED
#define VMIME_UTILITY_FILE_HPP_INCLUDED


#include "path.hpp"
#include "../config.hpp"


#if VMIME_HAVE_FILESYSTEM_FEATURES


namespace vmime {
namespace utility {


class file;


/** File list iterator (see file::getFiles).
  */

class fileIterator
{
public:

	virtual ~fileIterator() { }

	/** Check whether the cursor has reach the end of the list.
	  *
	  * @return true if you can call nextElement(), or false
	  * if no more file is available
	  */
	virtual const bool hasMoreElements() const = 0;

	/** Return the next file in the list.
	  *
	  * @return next file or NULL
	  */
	virtual file* nextElement() = 0;
};


// TODO: fileWriter

class fileWriter
{
public:

	virtual ~fileWriter() { }

	virtual utility::outputStream* getOutputStream() = 0;
};


// TODO: fileReader

class fileReader
{
public:

	virtual ~fileReader() { }

	virtual utility::inputStream* getInputStream() = 0;
};


/** Abstract representation of a file or directory.
  */

class file
{
public:

	typedef utility::path path;
	typedef long length_type;


	virtual ~file() { }


	/** Create the file pointed by this file object.
	  */
	virtual void createFile() = 0;

	/** Create the directory pointed by this file object.
	  *
	  * @param createAll if set to true, recursively create all
	  * parent directories if they do not exist
	  */
	virtual void createDirectory(const bool createAll = false) = 0;

	/** Test whether this is a file.
	  *
	  * @return true if this is a file, false otherwise
	  */
	virtual const bool isFile() const = 0;

	/** Test whether this is a directory.
	  *
	  * @return true if this is a directory, false otherwise
	  */
	virtual const bool isDirectory() const = 0;

	/** Test whether this file is readible.
	  *
	  * @return true if we can read this file, false otherwise
	  */
	virtual const bool canRead() const = 0;

	/** Test whether this file is writeable.
	  *
	  * @return true if we can write to this file, false otherwise
	  */
	virtual const bool canWrite() const = 0;

	/** Return the length of this file.
	  *
	  * @return file size (in bytes)
	  */
	virtual const length_type length() = 0;

	/** Return the full path of this file/directory.
	  *
	  * @return full path of the file
	  */
	virtual const path& fullPath() const = 0;

	/** Test whether this file/directory exists.
	  *
	  * @return true if the file exists, false otherwise
	  */
	virtual const bool exists() const = 0;

	/** Return the parent directory of this file/directory.
	  *
	  * @return parent directory (or NULL if root)
	  */
	virtual const file* getParent() const = 0;

	/** Rename the file/directory.
	  *
	  * @param newName full path of the new file
	  */
	virtual void rename(const path& newName) = 0;

	/** Deletes this file/directory.
	  */
	virtual void remove() = 0;


	// TODO     virtual fileWriter* getFileWriter() = 0;
	// TODO     virtual fileReader* getFileReader() = 0;

	/** Enumerate files contained in this directory.
	  *
	  * @return file iterator to enumerate files
	  * @throw exceptions::not_a_directory if this is not a directory
	  */
	virtual fileIterator* getFiles() const;
};


class fileSystemFactory
{
public:

	virtual ~fileSystemFactory() { }

	/** Create a new file object from the specified path.
	  *
	  * @param path full path (absolute) of the file
	  * @return new file object for the path
	  */
	virtual file* create(const file::path& path) = 0;

	/** Parse a path contained in a string.
	  *
	  * @param str string containing a path in a system-dependant representation
	  * @return path object (abstract representation)
	  */
	virtual file::path stringToPath(const string& str) = 0;

	/** Return the system-dependant string representation for the specified path.
	  *
	  * @param path abstract representation of the path
	  * @return string representation of the path
	  */
	virtual string pathToString(const file::path& path) = 0;
};


} // utility
} // vmime


#endif // VMIME_HAVE_FILESYSTEM_FEATURES


#endif // VMIME_UTILITY_FILE_HPP_INCLUDED