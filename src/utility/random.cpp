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

#include "random.hpp"
#include "platformDependant.hpp"

#include <ctime>


namespace vmime {
namespace utility {


unsigned int random::m_next(static_cast<unsigned int>(::std::time(NULL)));


const unsigned int random::next()
{
	// Park and Miller's minimal standard generator:
	// xn+1 = (a * xn + b) mod c
	// xn+1 = (16807 * xn) mod (2^31 - 1)
	static const unsigned long a = 16807;
	static const unsigned long c = (1 << ((sizeof(int) << 3) - 1));

	m_next = static_cast<unsigned int>((a * m_next) % c);
	return (m_next);
}


const unsigned int random::time()
{
	return (platformDependant::getHandler()->getUnixTime());
}


const unsigned int random::process()
{
	return (platformDependant::getHandler()->getProcessId());
}


} // utility
} // vmime