#pragma once
#include "cwBasicMdSpi.h"
#include "cwBasicSimulator.h"

class cwSimMdSpi 
	: public cwBasicMdSpi
{
public:
	cwSimMdSpi();
	~cwSimMdSpi();

		///��¼������Ӧ
	virtual void OnRspUserLogin();

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(cwMarketDataPtr pDepthMarketData);


	virtual void RegisterStrategy(cwBasicStrategy * pBasicStrategy);

	void Connect(char * pszFrontAddress);
	void Connect(cwBasicSimulator * pBaiscSimulator);
	void DisConnect();

	void WaitForFinish();

	void SetUserLoginField(char * szBrokerID, char * szUserID, char * szPassword);

	virtual void SubscribeMarketData(std::vector<std::string>& SubscribeInstrument);
	virtual void UnSubscribeMarketData(std::vector<std::string>& SubscribeInstrument);

	size_t		 GetDataDequeLength() { return m_DepthMarketDataDeque.size(); }
private:
	cwBasicSimulator *			m_pMarketDataUserApi;

	//User Config Data
	char						m_szMDFrount[1024];

	//
	int							m_iRequestId;

	std::map<std::string, bool>	m_SubscribeInstrumentMap;
};

