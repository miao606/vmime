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

#ifndef VMIME_PLAINTEXTPART_HPP_INCLUDED
#define VMIME_PLAINTEXTPART_HPP_INCLUDED


#include "textPart.hpp"


namespace vmime
{


class plainTextPart : public textPart
{
public:

	const mediaType type() const;

	const class charset& charset() const { return (m_charset); }
	class charset& charset() { return (m_charset); }

	const contentHandler& text() const { return (m_text); }
	contentHandler& text() { return (m_text); }

protected:

	contentHandler m_text;
	class charset m_charset;

	const int getPartCount() const;

	void generateIn(bodyPart& message, bodyPart& parent) const;
	void parse(const bodyPart& message, const bodyPart& parent, const bodyPart& textPart);
};


} // vmime


#endif // VMIME_PLAINTEXTPART_HPP_INCLUDED