#ifndef __DataHoldingListWidgetItem
#define __DataHoldingListWidgetItem

#include <QDialog>
#include <QListWidget>

class DataHoldingListWidgetItem: public QListWidgetItem
{
public:
	DataHoldingListWidgetItem ( QListWidget * parent = 0, int type = Type ):
		QListWidgetItem(parent,type)
	{
		customData = NULL;
		customType = 0;
	}

	DataHoldingListWidgetItem ( const QString & text, QListWidget * parent = 0, int type = Type ):
		QListWidgetItem(text,parent,type)
	{
		customData = NULL;
		customType = 0;
	}

	void setCustomData(void *data)
	{
		customData = data;
	}

	void setCustomType(int type)
	{
		customType = type;
	}

	void *getCustomData()
	{
		return customData;
	}

	int getCustomType()
	{
		return customType;
	}
private:
	void *customData;
	int customType;
};

#endif