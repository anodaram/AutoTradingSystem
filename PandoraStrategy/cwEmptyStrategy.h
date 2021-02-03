#pragma once
#include <fstream>
#include "cwBasicStrategy.h"
#include <iostream>

class cwEmptyStrategy :
	public cwBasicStrategy
{
public:
	cwEmptyStrategy();
	~cwEmptyStrategy();

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

	bool		m_bShowPosition;
private:

};

