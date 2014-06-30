//  [6/28/2014 Administrator]
// Queue

#ifndef _QUEUE_
#define _QUEUE_

#include <list>

class MsgData
{
public:
	enum EMsgType
	{
		EMsgType_Null = 0,
		EMsgType_Connect,
		EMsgType_Closed,
		EMsgType_Message,
	};

	MsgData()
	{
		eMsgType = EMsgType_Null;
		nSockId = 0;
		wLength = 0;
	}
	
public:
	EMsgType eMsgType;
	int nSockId;
	char szData[0x2000];
	unsigned short wLength;
};

class CQueue
{
public:
	CQueue()
	{
		m_pMsgDataListPush = &m_listMsgDataA;
		m_pMsgDataListPop = &m_listMsgDataB;
	}
	~CQueue()
	{

	}

public:
	bool PushMsgData(MsgData msgData)
	{
		m_mtx.lock();
		m_pMsgDataListPush->push_back(msgData);
		m_mtx.unlock();

		return true;
	}
	bool PrepareMsgData(MsgData &msgData)
	{
		if (0 == m_pMsgDataListPop->size())
		{
			m_mtx.lock();
			MsgDataList *pTempMsgDataList = m_pMsgDataListPush;
			m_pMsgDataListPush = m_pMsgDataListPop;
			m_pMsgDataListPop = pTempMsgDataList;
			m_mtx.unlock();
		}

		if (m_pMsgDataListPop->size() > 0)
		{
			msgData = m_pMsgDataListPop->front();
			return true;
		}
		else
		{
			return false;
		}
	}
	void RemoveFrontMsgData()
	{
		if (m_pMsgDataListPop->size() > 0)
		{
			m_pMsgDataListPop->pop_front();
		}
	}

private:
	typedef std::list<MsgData> MsgDataList;
	MsgDataList m_listMsgDataA;
	MsgDataList	m_listMsgDataB;

	MsgDataList *m_pMsgDataListPush;
	MsgDataList *m_pMsgDataListPop;

	mutex m_mtx;

};

#endif