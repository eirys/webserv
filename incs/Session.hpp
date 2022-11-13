#ifndef SESSION_HPP
# define SESSION_HPP

# define SESSION_ID_LENGTH 16
# define SESSION_TIMEOUT 1000
# define SESSION_ID	"sessionId"

# include "utils.hpp"
# include "Cookie.hpp"
# include "Purchase.hpp"

# include <vector>
# include <cstdio>

/******************************************************************************/
/*                               CLASS SESSION                                */
/******************************************************************************/

class Session
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef std::vector<class Cookie>			listOfCookies;
		typedef std::vector<class Purchase>			listOfPurchases;
		//typedef std::vector<std::string>			listOfPath;

		typedef std::pair<std::string, std::string>	pairRootImg;
		typedef std::vector<pairRootImg>			listOfPath;



	private:
	/**********************     MEMBER VARIABLES     ********************/
		std::string				_id;
		listOfCookies			_cookies;
		listOfPurchases			_order;
		Purchase				_purchase;
		size_t					_time;
		listOfPath				_gallery;

	public:
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------     Main    -------*/
		Session();
		Session(const std::string& id);
		Session(const Session& other);
		Session&				operator=(const Session& other);
		~Session();

						/*--------    Getter   -------*/
		std::string				getId() const;
		listOfCookies&			getCookies();
		const listOfCookies&	getCookies() const;
		listOfPurchases&		getOrder();
		const listOfPurchases&	getOrder() const;
		listOfPath&				getGallery();
		const listOfPath&		getGallery() const;
		size_t					getTime() const;

						/*-------     Time   -------*/
		bool					sessionIsAlive();
		void					updateTime();

						/*-------   Cookies  -------*/
		std::string				getCookieHeader();
		void					fillCookies(const listOfCookies& other);
		void					displayCookies() const;

						/*--------  Purchase  -------*/
		void					completePurchase(const std::string& name, const std::string& content, std::string* msg);
		bool					deletePurchase(const std::string& id, std::string* msg);

						/*--------   Gallery  ------*/
		void					addImage(const std::string& path, const std::string& root);
		bool					imageExist(const std::string& path);
		void					deleteImage(listOfPath::iterator ite, std::string* msg);

	private:
						/*-------   Cookies  -------*/
		void					_addCookie(const std::string& name, const std::string& value);
		bool					_cookieAlreadySet(const std::string& name, const std::string& value);

						/*--------  Purchase  -------*/
		void					_initPurchase();
		std::string				_generatePurchaseId();
		void					_addPurchaseInOrder(std::string* msg);
		bool					_idIsUnique(const std::string& id);

		void					_destroyGallery();
};

/******************************************************************************/
/*                           CLASS SESSION HANDLER                            */
/******************************************************************************/

class SessionHandler
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef Session::listOfCookies		listOfCookies;
		typedef std::vector<Session*>		listOfSessions;

	private:
	/**********************     MEMBER VARIABLES     ********************/
		listOfSessions						_sessions;
		listOfSessions::iterator			_ite;

	public:
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

		SessionHandler();
		~SessionHandler();

								/*-------     Lookup   -------*/
		Session*							lookupSession(const listOfCookies& requestCookies);

								/*-------     Getter   -------*/
		listOfSessions&						getSessions();
		const listOfSessions&				getSessions() const;

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

								/*--------     Setup   ------*/
		Session*							_newSession();
		std::string							_generateSessionId();

								/*--------     find   ------*/
		// Session*							_findSession(const std::string& sessionId);
		Session*							_findSession(const listOfCookies& cookies);
		bool								_matchSession(const std::string& sessionId);
		listOfSessions::iterator			_findSessionIte(const std::string& sessionId);
		std::string							_getCookieSID(const listOfCookies& cookies);

								/*--------   Cleanup  ------*/
		void								_deleteSessions();
		void								_deleteSession(listOfSessions::iterator ite);
};

#endif
