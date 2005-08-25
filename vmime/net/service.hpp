//
// VMime library (http://www.vmime.org)
// Copyright (C) 2002-2005 Vincent Richard <vincent@vincent-richard.net>
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

#ifndef VMIME_NET_SERVICE_HPP_INCLUDED
#define VMIME_NET_SERVICE_HPP_INCLUDED


#include "vmime/types.hpp"

#include "vmime/net/session.hpp"
#include "vmime/net/authenticator.hpp"

#include "vmime/net/serviceFactory.hpp"
#include "vmime/net/serviceInfos.hpp"

#include "vmime/utility/progressionListener.hpp"


namespace vmime {
namespace net {


/** Base class for messaging services.
  */

class service : public object
{
protected:

	service(ref <session> sess, const serviceInfos& infos, ref <authenticator> auth);

public:

	virtual ~service();

	// Possible service types
	enum Type
	{
		TYPE_STORE = 0,    /**< The service is a message store. */
		TYPE_TRANSPORT     /**< The service sends messages. */
	};

	/** Return the type of service.
	  *
	  * @return type of service
	  */
	virtual const Type getType() const = 0;

	/** Return the protocol name of this service.
	  *
	  * @return protocol name
	  */
	virtual const string getProtocolName() const = 0;

	/** Return the session object associated with this service instance.
	  *
	  * @return session object
	  */
	ref <const session> getSession() const;

	/** Return the session object associated with this service instance.
	  *
	  * @return session object
	  */
	ref <session> getSession();

	/** Return information about this service.
	  *
	  * @return information about the service
	  */
	virtual const serviceInfos& getInfos() const = 0;

	/** Connect to service.
	  */
	virtual void connect() = 0;

	/** Disconnect from service.
	  */
	virtual void disconnect() = 0;

	/** Test whether this service is connected.
	  *
	  * @return true if the service is connected, false otherwise
	  */
	virtual const bool isConnected() const = 0;

	/** Do nothing but ensure the server do not disconnect (for
	  * example, this can reset the auto-logout timer on the
	  * server, if one exists).
	  */
	virtual void noop() = 0;

	/** Return the authenticator object used with this service instance.
	  *
	  * @return authenticator object
	  */
	ref <const authenticator> getAuthenticator() const;

	/** Return the authenticator object used with this service instance.
	  *
	  * @return authenticator object
	  */
	ref <authenticator> getAuthenticator();

	/** Set a property for this service (service prefix is added automatically).
	  *
	  * WARNING: this sets the property on the session object, so all service
	  * instances created with the session object will inherit the property.
	  *
	  * @param name property name
	  * @param value property value
	  */
	template <typename TYPE>
	void setProperty(const string& name, const TYPE& value)
	{
		m_session->getProperties()[getInfos().getPropertyPrefix() + name] = value;
	}

#ifndef VMIME_BUILDING_DOC
	// Basic service registerer
	template <class S>
	class initializer
	{
	public:

		initializer(const string& protocol)
		{
			serviceFactory::getInstance()->
				template registerServiceByProtocol <S>(protocol);
		}
	};
#endif // VMIME_BUILDING_DOC

private:

	ref <session> m_session;
	ref <authenticator> m_auth;
};


} // net
} // vmime


#endif // VMIME_NET_SERVICE_HPP_INCLUDED