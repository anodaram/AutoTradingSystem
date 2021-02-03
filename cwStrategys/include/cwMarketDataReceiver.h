#pragma once
#include <fstream>
#include "cwBasicStrategy.h"
#include "cwStrategyLog.h"
#include <iostream>
#include "tinyxml.h"

class cwMarketDataReceiver :
	public cwBasicStrategy
{
public:
	cwMarketDataReceiver();
	~cwMarketDataReceiver();

	std::string  GetStrategyName();

	//MarketData SPI
	///�������
	virtual void PriceUpdate(cwMarketDataPtr pPriceData);

	//Trade SPI
	///�ɽ��ر�
	virtual void OnRtnTrade(cwTradePtr pTrade) {};
	///�����ر�
	virtual void OnRtnOrder(cwOrderPtr pOrder, cwOrderPtr pOriginOrder = cwOrderPtr()) {};
	///�����ɹ�
	virtual void OnOrderCanceled(cwOrderPtr pOrder) {};

	virtual void SetStrategyReady();

	std::string	m_strCurrentUpdateTime;	


	void InitialStrategy(const char * pConfigFilePath);

	///strategy parameter
	//�������д���
	std::string m_strStrategyName;		
	//�����Ƿ�����
	bool		m_bStrategyRun;					


private:
	bool							m_bNightMode;
	std::string						m_strAppStartDay;
	std::string						m_strAppStartNextDay;
	std::string						m_strAppStartTime;

	cwStrategyLog					m_StrategyLog;

	std::map<std::string, uint64_t>	m_TotalVolume;
	std::map<std::string, double>	m_TotalTurnOver;

	std::map<std::string, bool>		m_bHasFirstQuotes;
};

