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

#ifndef VMIME_MESSAGING_POP3MESSAGE_HPP_INCLUDED
#define VMIME_MESSAGING_POP3MESSAGE_HPP_INCLUDED


#include "message.hpp"
#include "folder.hpp"
#include "../config.hpp"


namespace vmime {
namespace messaging {


/** POP3 message implementation.
  */

class POP3Message : public message
{
protected:

	friend class POP3Folder;

	POP3Message(POP3Folder* folder, const int num);
	POP3Message(const POP3Message&) : message() { }

	~POP3Message();

public:

	const int number() const;

	const uid uniqueId() const;

	const int size() const;

	const bool isExpunged() const;

	const class structure& structure() const;
	class structure& structure();

	const class header& header() const;

	const int flags() const;
	void setFlags(const int flags, const int mode = FLAG_MODE_SET);

	void extract(utility::outputStream& os, progressionListener* progress = NULL, const int start = 0, const int length = -1) const;
	void extractPart(const part& p, utility::outputStream& os, progressionListener* progress = NULL, const int start = 0, const int length = -1) const;

	void fetchPartHeader(part& p);

private:

	void fetch(POP3Folder* folder, const int options);

	void onFolderClosed();

	POP3Folder* m_folder;
	int m_num;
	uid m_uid;

	class header* m_header;
};


} // messaging
} // vmime


#endif // VMIME_MESSAGING_POP3MESSAGE_HPP_INCLUDED