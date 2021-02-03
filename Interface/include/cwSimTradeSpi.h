#pragma once
#include "cwBasicTradeSpi.h"
#include "cwBasicSimulator.h"
#include "cwBasicCout.h"

class cwSimTradeSpi
	: public cwBasicTradeSpi
{
public:
	cwSimTradeSpi();
	~cwSimTradeSpi();

	virtual void RegisterBasicStrategy(cwBasicStrategy * pBasicStrategy, void * pSpi = NULL);

	///��¼������Ӧ
	virtual void OnRspQryInstrument(std::map<std::string, cwInstrumentDataPtr>& cwInstrumentDataPtr);

	//User Trader Method
	//�������
	virtual void PriceUpdate(cwMarketDataPtr pPriceData);
	
	///����֪ͨ
	void OnRtnOrder(cwOrderPtr pOrder);

	///�ɽ�֪ͨ
	virtual void OnRtnTrade(cwTradePtr pTrade);

	///�˻�֪ͨ
	virtual void OnRtnAccount(cwAccountPtr pAccount);

	///����¼�����ر�
	virtual void OnErrRtnOrderInsert(cwOrderPtr pInputOrder, cwRspInfoPtr pRspInfo);

	///��������������Ӧ
	virtual void OnRspOrderAction(cwOrderPtr pInputOrderAction, cwRspInfoPtr pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnSimulationPartEnd();
	virtual void OnSimulationFinished();

	virtual	cwOrderPtr InputLimitOrder(const char * szInstrumentID, cwFtdcDirectionType direction,
		cwOpenClose openclose, int volume, double price);
	virtual cwOrderPtr InputFAKOrder(const char * szInstrumentID, cwFtdcDirectionType direction,
		cwOpenClose openclose, int volume, double price);
	virtual cwOrderPtr InputFOKOrder(const char * szInstrumentID, cwFtdcDirectionType direction,
		cwOpenClose openclose, int volume, double price);
	virtual void CancelOrder(char * szLocalOrderID);
	virtual void CancelOrder(cwOrderPtr pOrder);

	cwOrderPtr GetcwOrderPtr(const char * szInstrumentID, cwFtdcDirectionType direction,
		cwOpenClose openclose, int volume, double price);

	void Connect(char * pszFrontAddress);
	void Connect(cwBasicSimulator * pBaiscSimulator);
	void DisConnect();

	void WaitForFinish();

	void SetUserLoginField(char * szBrokerID, char * szUserID, char * szPassword);

private:
	cwBasicSimulator *			m_pMarketDataUserApi;

	//User Config Data
	char						m_szMDFrount[1024];

	//
	int							m_iRequestId;

	std::map<std::string, cwMarketDataPtr>		m_LastestPriceDataMap;

#ifdef CWCOUTINFO
	cwBasicCout					m_cwShow;
#endif
};

