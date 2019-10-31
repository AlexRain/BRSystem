#pragma 
#include <QString>
#include <QMetaType>
#include "database_global.h"

typedef struct _UserData
{
	QString userId;
	QString userName;
	bool    isAdmin;
	QString departmentId;
	QString departmentName;
	QString password;
}UserData;

Q_DECLARE_METATYPE(UserData)

class DATABASE_EXPORT UserSession
{
	
private:
	UserSession();
	~UserSession();

public:
	static UserSession & getInstance();
	void setUserData(const UserData &data);
	const UserData & userData() const;

private:
	UserData _userData;


};
