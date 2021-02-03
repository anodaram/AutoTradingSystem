#pragma once
#include <thread>

#include "cwBasicSimulator.h"
#include "cwTickTradeManager.h"
#include "cwProductTradeTime.h"
#include "cwSettlement.h"

#include "tinyxml.h"

#include "cwBasicCout.h"

class cwPegasusSimulator :
	public cwBasicSimulator
{

public:
	cwPegasusSimulator();
	~cwPegasusSimulator();

	virtual void InitialSimulator(const char * pConfigFilePath);

	virtual int ReqUserMdLogin();
	virtual int ReqQryInstrument();

	virtual int ReqOrderInsert(cwOrderPtr pOrder);
	virtual int CancelOrder(cwOrderPtr pOrder);

	bool		SimulationStart();
	bool		ReadXmlConfigFile();

	cwOrderPtr	GetOrder(cwOrderPtr pOrder);
	cwTradePtr	GetTrade(cwOrderPtr pOrder, double dTradePrice, int iTradeCnt = 1);

	cwFtdcDateType								m_CurrentTradingDay;
	cwFtdcTimeType								m_CurrentSimulationTime;

	volatile bool								m_bSimulationFinished;
	cwSettlement								m_cwSettlement;
private:
	enum SIMTYPE:int
	{
		type_CSV_file = 0,
		type_BIN_file,
		type_CSV_List_file,
		type_BIN_List_file,
		type_DB,
		type_REAL_Time_Quote
	};

	SIMTYPE				m_SimType;
	int					m_iInterval;

	std::string			m_strFrontAddr;
	std::string			m_strInstrumentFile;

	std::thread			m_SimulatorProcessorThread;
	volatile bool		m_bMarketDataUpdateThreadRun;
	void				SimulatorProcessor();

	std::thread			m_MarketDataUpdateThread;
	void				CsvMarketDataUpdate();
	void				BinMarketDataUpdate();
	void				RealTimeMarketDataUpdate();

	std::map<std::string, std::string>			m_MarketDataFileMap;

	std::map<std::string, cwInstrumentDataPtr>	m_InstrumentMap;

	int					m_iSysOrderID;
	int					m_iSysTradeID;

	cwTickTradeManager	m_cwTickManager;


	std::map<std::string, cwMarketDataPtr>								m_LastestMarketDataMap;

	std::map<std::string, cwOrderPtr>									m_TotalOrderMap;
	std::map<std::string, std::map<int64_t, std::deque<cwOrderPtr>>>	m_TotalLongOrderMap;
	std::map<std::string, std::map<int64_t, std::deque<cwOrderPtr>>>	m_TotalShortOrderMap;

	std::map<std::string, std::map<int64_t, std::deque<cwOrderPtr>>>	m_LongWaitOrderListMap;
	std::map<std::string, std::map<int64_t, std::deque<cwOrderPtr>>>	m_ShortWaitOrderListMap;

	enum UserActionType :int
	{
		UAT_IO = 0,
		UAT_CO
	};
	struct cwSimulationUserAction
	{
		UserActionType	Actiontype;
		cwOrderPtr		pOrder;
	};

	cwProductTradeTime							m_ProductTradeTime;

	std::deque<cwSimulationUserAction>			m_UndealOrderDeque;
	
	std::deque<cwTradePtr>						m_TradeDeque;

	cwBasicCout									m_cwShow;

	cwMUTEX										m_ProcessMutex;

	std::deque<cwMarketDataPtr>					m_MDCasheDeque;
	cwMUTEX										m_MDCasheMutex;
	volatile bool								m_bMDCasheMutexReady;
	volatile bool								m_bSimulationPartEnd;

	cwAccountPtr								m_pAccount;

	double										m_dDeposit;

	//Result 
	bool													m_bSaveAccountResult;
	int														m_iAccountResultInterval;
	std::map<std::string, double>							m_dTimeBalanceMap;

	std::map<std::string, bool>								m_bSaveInsResultMap;
	std::map<std::string, int>								m_iInsResultInterval;
	std::map<std::string, std::map<std::string, double>>	m_dInsTimeBalanceMap;
};

