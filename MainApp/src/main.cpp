#include "BRSystem.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QStyleFactory>
#include "define.h"
#include "UiLogin.h"
#include "CStyleManager.h"
#include "UiFrostedLayer.h"
#include "PopupDialogContainer.h"
#include "CApplication.h"
#include <QDebug>
//#define TEST

using namespace std;

int removeDuplicates(vector<int>& nums) {
	if (nums.size() <= 1)
		return nums.size();
	bool flag = false;
	int newIndex = 1;
	for (int i = 1; i < nums.size(); i++)
	{
		if (nums[i] == nums[i - 1])
		{
			flag = true;
		}
		else
		{
			nums[newIndex] = nums[i];
			flag = false;
			newIndex++;
		}
	}
	return newIndex;
}

int removeDuplicates1(vector<int>& nums) {
	int res = 1, offset = 0, len = nums.size();
	if (len < 1) return 0;
	int last = nums[len - 1];
	for (int i = 0; i < len - 1 && nums[i] != last; ++i) {
		//需要向前移动的次数
		int n = i;
		for (; n < len - 1 && nums[n] != last; ++n) {
			if (nums[n] == nums[n + 1]) {
				offset++;
			}
			else {
				break;
			}
		}
		
		qDebug() << offset;
		res++;
		if (offset > 0) {
			//移动数组
			for (int j = n + 1; j < len; ++j) {
				nums[j - offset] = nums[j];
			}
			offset = 0;
		}
	}
	return res;
}

int myAtoi(string str) {
	if (str.empty()) return 0;
	char *p = &str[0];
	int flag = 1;
	long res = 0;

	/*去掉前面的空格*/
	while (*p == ' ') {
		p++;
	}
		
	if (*p == '-') {
		flag = -1;
		p++;
	}
	else if (*p == '+')
		p++;

	while (p) {
		if (*p <= '9' && *p >= '0') {
			res = res * 10 + (*p - '0');
			if (1 == flag && res > INT_MAX) return INT_MAX;
			if (-1 == flag && res > INT_MAX) return INT_MIN;
			p++;
		}
		else
			return res * flag;
	}
	return (int)res * flag;
}

int main(int argc, char *argv[])
{
#ifdef TEST
	typedef char *pstring;
	qDebug() <<  myAtoi("-42");

	vector<int> nums{ 0,0,0,0,1,1,2,3,3,3,3,3,3,3,3,3,3,3,4,4,4,5,5,5,6};
	int res = removeDuplicates(nums);
	qDebug() << res << nums;

	double var = 3.14;
	int value = 5;
	const int &num = var;
	int &num = value;

	int *const p = &value;
	int * p2 = p;

	//const pstring cstr = 0; 
	const char *const cstr = 0;
	const pstring *pstr;

#else
	CApplication a(argc, argv);

	a.setWindowIcon(QIcon("images/app.ico"));
	a.setApplicationName(TOCH("汇声科技生产专用借还系统"));

	StyleStruct style = CStyleManager::getInstance().getCurrentStyleStruct();
	StyleHelper::loadAppStyle(style.cssFile);

	/*登录界面*/
	UiLogin login;
	if (QDialog::Accepted != login.fadeIn()) return 0;

	/*主界面*/
	BRSystem w;
	a.setMainWidget(&w);
	PopupDialogContainer::showMainWidgetFadeIn(&w);
	return a.exec();
#endif
}
