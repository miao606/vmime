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

#include "charset.hpp"
#include "exception.hpp"
#include "platformDependant.hpp"


extern "C"
{
	#include <iconv.h>

	// HACK: prototypes may differ depending on the compiler and/or system (the
	// second parameter may or may not be 'const'). This redeclaration is a hack
	// to have a common prototype "iconv_cast".
	typedef size_t (*iconv_const_hack)(iconv_t cd, const char* * inbuf,
		size_t *inbytesleft, char* * outbuf, size_t *outbytesleft);

	#define iconv_const ((iconv_const_hack) iconv)
}


namespace vmime
{


charset::charset()
	: m_name(charsets::US_ASCII)
{
}


charset::charset(const string& name)
	: m_name(name)
{
}


void charset::parse(const string& buffer, const string::size_type position,
	const string::size_type end, string::size_type* newPosition)
{
	m_name = string(buffer.begin() + position, buffer.begin() + end);

	if (newPosition)
		*newPosition = end;
}


void charset::generate(utility::outputStream& os, const string::size_type /* maxLineLength */,
	const string::size_type curLinePos, string::size_type* newLinePos) const
{
	os << m_name;

	if (newLinePos)
		*newLinePos = curLinePos + m_name.length();
}


/** Convert the contents of an input stream in a specified charset
  * to another charset and write the result to an output stream.
  *
  * @param in input stream to read data from
  * @param out output stream to write the converted data
  * @param source input charset
  * @param dest output charset
  */

void charset::convert(utility::inputStream& in, utility::outputStream& out,
	const charset& source, const charset& dest)
{
	// Get an iconv descriptor
	const iconv_t cd = iconv_open(dest.name().c_str(), source.name().c_str());

	if (cd != (iconv_t) -1)
	{
		char inBuffer[5];
		char outBuffer[32768];
		size_t inPos = 0;

		bool prevIsInvalid = false;

		while (true)
		{
			// Fullfill the buffer
			size_t inLength = (size_t) in.read(inBuffer + inPos, sizeof(inBuffer) - inPos) + inPos;
			size_t outLength = sizeof(outBuffer);

			const char* inPtr = inBuffer;
			char* outPtr = outBuffer;

			// Convert input bytes
			if (iconv_const(cd, &inPtr, &inLength, &outPtr, &outLength) == (size_t) -1)
			{
				// Illegal input sequence or input sequence has no equivalent
				// sequence in the destination charset.
				if (prevIsInvalid)
				{
					// Write successfully converted bytes
					out.write(outBuffer, sizeof(outBuffer) - outLength);

					// Output a special character to indicate we don't known how to
					// convert the sequence at this position
					out.write("?", 1);

					// Skip a byte and leave unconverted bytes in the input buffer
					std::copy((char*) inPtr + 1, inBuffer + sizeof(inBuffer), inBuffer);
					inPos = inLength - 1;
				}
				else
				{
					// Write successfully converted bytes
					out.write(outBuffer, sizeof(outBuffer) - outLength);

					// Leave unconverted bytes in the input buffer
					std::copy((char*) inPtr, inBuffer + sizeof(inBuffer), inBuffer);
					inPos = inLength;

					prevIsInvalid = true;
				}
			}
			else
			{
				// Write successfully converted bytes
				out.write(outBuffer, sizeof(outBuffer) - outLength);

				inPos = 0;
				prevIsInvalid = false;
			}

			// Check for end of data
			if (in.eof() && inPos == 0)
				break;
		}

		// Close iconv handle
		iconv_close(cd);
	}
	else
	{
		throw exceptions::charset_conv_error();
	}
}


/** Convert a string buffer in a specified charset to a string
  * buffer in another charset.
  *
  * @param in input buffer
  * @param out output buffer
  * @param from input charset
  * @param to output charset
  */

template <class STRINGF, class STRINGT>
void charset::iconvert(const STRINGF& in, STRINGT& out, const charset& from, const charset& to)
{
	// Get an iconv descriptor
	const iconv_t cd = iconv_open(to.name().c_str(), from.name().c_str());

	typedef typename STRINGF::value_type ivt;
	typedef typename STRINGT::value_type ovt;

	if (cd != (iconv_t) -1)
	{
		out.clear();

		char buffer[65536];

		const char* inBuffer = (const char*) in.data();
		size_t inBytesLeft = in.length();

		for ( ; inBytesLeft > 0 ; )
		{
			size_t outBytesLeft = sizeof(buffer);
			char* outBuffer = buffer;

			if (iconv_const(cd, &inBuffer, &inBytesLeft,
			                &outBuffer, &outBytesLeft) == (size_t) -1)
			{
				out += STRINGT((ovt*) buffer, sizeof(buffer) - outBytesLeft);

				// Ignore this "blocking" character and continue
				out += '?';
				++inBuffer;
				--inBytesLeft;
			}
			else
			{
				out += STRINGT((ovt*) buffer, sizeof(buffer) - outBytesLeft);
			}
		}

		// Close iconv handle
		iconv_close(cd);
	}
	else
	{
		throw exceptions::charset_conv_error();
	}
}


#if VMIME_WIDE_CHAR_SUPPORT

/** Convert a string buffer in the specified charset to a wide-char
  * string buffer.
  *
  * @param in input buffer
  * @param out output buffer
  * @param ch input charset
  */

void charset::decode(const string& in, wstring& out, const charset& ch)
{
	iconvert(in, out, ch, charset("WCHAR_T"));
}


/** Convert a wide-char string buffer to a string buffer in the
  * specified charset.
  *
  * @param in input buffer
  * @param out output buffer
  * @param ch output charset
  */

void charset::encode(const wstring& in, string& out, const charset& ch)
{
	iconvert(in, out, charset("WCHAR_T"), ch);
}

#endif


/** Convert a string buffer from one charset to another charset.
  *
  * @param in input buffer
  * @param out output buffer
  * @param source input charset
  * @param dest output charset
  */

void charset::convert(const string& in, string& out, const charset& source, const charset& dest)
{
	iconvert(in, out, source, dest);
}


/** Returns the default charset used on the system.
  *
  * This function simply calls <code>platformDependantHandler::getLocaleCharset()</code>
  * and is provided for convenience.
  *
  * @return system default charset
  */

const charset charset::getLocaleCharset()
{
	return (platformDependant::getHandler()->getLocaleCharset());
}


charset& charset::operator=(const charset& source)
{
	m_name = source.m_name;
	return (*this);
}


charset& charset::operator=(const string& name)
{
	parse(name);
	return (*this);
}


const bool charset::operator==(const charset& value) const
{
	return (isStringEqualNoCase(m_name, value.m_name));
}


const bool charset::operator!=(const charset& value) const
{
	return !(*this == value);
}


} // vmime