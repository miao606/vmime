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

#include <iostream>
#include <ostream>

#include "../../src/vmime"
#include "../../examples/common.inc"


int main(int argc, char* argv[])
{
	// VMime initialization
	vmime::platformDependant::setHandler<my_handler>();


	const vmime::string encoding(argv[1]);
	const vmime::string mode(argv[2]);

	vmime::encoder* p = vmime::encoderFactory::getInstance()->create(encoding);

	p->properties()["maxlinelength"] = 76;

	vmime::inputStreamAdapter in(std::cin);
	vmime::outputStreamAdapter out(std::cout);

	if (mode == "e")
		p->encode(in, out);
	else
		p->decode(in, out);

	delete (p);
}