//////////////////////////////////////////////////////////////////////////////////
//*******************************************************************************
//---
//---	author: Wu Chang Sheng
//---
//---	CreateTime:	2016/12/12
//---
//---	VerifyTime:	2016/12/12
//---
//*******************************************************************************
//////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "cwTradeCommonDefine.h"
#include "cwMutex.h"
#include "cwBasicStrategy.h"
#include "cwOrderReference.h"

#define CWRISK
#define TRADELOG
//#define UPDATE_ORDERRANKED
#define	NoCancelTooMuchPerTick

#ifdef UPDATE_ORDERRANKED
#define NO_TRADEINFO_LOG
#include "cwTickTradeManager.h"
#endif // UPDATE_ORDERRANKED

#ifdef TRADELOG
#include "cwTradeLog.h"
#endif

class cwBasicTradeSpi
{
public:
	friend class cwBasicStrategy;

	enum TradeServerStatus
	{
		Status_UnConnected = 0
		, Status_Connected
		, Status_Logined
		, Status_Initial
		, Status_Normal
	};

public:
	cwBasicTradeSpi(cwTradeAPIType apiType);
	cwBasicTradeSpi(cwTradeAPIType apiType, const char * pLogFileName);
	cwBasicTradeSpi(cwTradeAPIType apiType, const char * pLogFileName, const char * pFolder);
	~cwBasicTradeSpi();

	inline TradeServerStatus GetCurrentStatus()
	{
		return m_CurrentStatus;
	}

	inline const char * GetCurrentStatusString()
	{
		switch (m_CurrentStatus)
		{
		case cwBasicTradeSpi::Status_UnConnected:
			return " UnConnected ";
			break;
		case cwBasicTradeSpi::Status_Connected:
			return " Connecting ";
			break;
		case cwBasicTradeSpi::Status_Logined:
			return " Logined ";
			break;
		case cwBasicTradeSpi::Status_Initial:
			return " Initialing ";
			break;
		case cwBasicTradeSpi::Status_Normal:
			return " Working ";
			break;
		default:
			break;
		}
		return " UnConnected ";
	}

	virtual void RegisterBasicStrategy(cwBasicStrategy * pBasicStrategy, void * pSpi = NULL) = 0;

	void		SetTradeInfo(const char * pszInfo);

	inline cwAccountPtr GetAccount()
	{
		return m_pAccount;
	}

	inline std::map<std::string, cwPositionPtr> GetPosition(bool ClearChangedFlag = true)
	{
		if (ClearChangedFlag)
		{
			m_bHasPositionChanged = false;
		}
		cwAUTOMUTEX mt(m_TradeSpiMutex, true);
		return m_PositionMap;
	}
	inline std::map<std::string, cwOrderPtr> GetOrders(bool ClearChangedFlag = true)
	{
		if (ClearChangedFlag)
		{
			m_bHasOrdersChanged = false;
		}
		cwAUTOMUTEX mt(m_TradeSpiMutex, true);
		return m_OrdersMap;
	}
	inline std::map<std::string, cwOrderPtr> GetActiveOrders(bool ClearChangedFlag = true)
	{
		if (ClearChangedFlag)
		{
			m_bHasActiveOrdersChanged = false;
		}
		cwAUTOMUTEX mt(m_TradeSpiMutex, true);
		return m_ActiveOrdersMap;
	}
	inline std::map<std::string, cwTradePtr> GetTrades(bool ClearChangedFlag = true)
	{
		if (ClearChangedFlag)
		{
			m_bHasTradesChanged = false;
		}
		return m_TradeMap;
	}
	bool		IsWaitOrder(cwOrderPtr pOrder);
	bool		IsIOCTypeOrder(cwOrderPtr pOrder);

	cwInsertOrderType GetInsertOrderType(cwOrderPtr pOrder);
	cwInsertOrderType GetInsertOrderType(cwFtdcOrderPriceType OrderPriceType,
		cwFtdcContingentConditionType ContingentCondition,
		cwFtdcTimeConditionType TimeCondition,
		cwFtdcVolumeConditionType VolumeCondition);

	bool		GetPosition(std::string InstrumentID, cwFtdcDirectionType direction,
		int& TotalPositon, int& TodayPosition);
	bool		GetPositionAndActiveOrders(std::string InstrumentID, cwFtdcDirectionType direction,
		int& TotalPositon, int& TodayPosition, int& FrozenTdPosition, int& FrozenYdPosition);

	//CWRISK
#ifdef CWRISK
	int			GetOrderCancelCount(std::string InstrumentID);
#endif

	//User Trader Method
	//�������
	virtual void PriceUpdate(cwMarketDataPtr pPriceData) = 0;
	virtual	cwOrderPtr InputLimitOrder(const char * szInstrumentID, cwFtdcDirectionType direction,
		cwOpenClose openclose, int volume, double price) = 0;
	virtual cwOrderPtr InputFAKOrder(const char * szInstrumentID, cwFtdcDirectionType direction,
		cwOpenClose openclose, int volume, double price) = 0;
	virtual cwOrderPtr InputFOKOrder(const char * szInstrumentID, cwFtdcDirectionType direction,
		cwOpenClose openclose, int volume, double price) = 0;
	virtual void CancelOrder(char * szLocalOrderID) = 0;
	virtual void CancelOrder(cwOrderPtr pOrder) = 0;

	void	SetDisConnectExit(bool bDisConnectExit = true) { m_bDisConnectExit = bDisConnectExit; }

	std::map<std::string, cwInstrumentDataPtr>	m_InstrumentMap;

	std::string									m_strInstrumentDataFileName;
	void	SetSaveInstrumentDataToFile(bool bSave) { m_bSaveInstrumentDataToFile = bSave; }
	void	SetInstrumentDataFileName(const char * fileName);
	void	GetInstrumentDataFromFile();
	bool	GenerateInstrumentDataToFile();

	const cwTradeAPIType		m_cwTradeAPIType;
	char						m_szTradeInfo[128];

	bool						m_bHasPositionChanged;
	bool						m_bHasOrdersChanged;
	bool						m_bHasActiveOrdersChanged;
	bool						m_bHasTradesChanged;

protected:
	TradeServerStatus			m_CurrentStatus;
	cwFtdcTimeType				m_cwTradeLoginTime;

	cwBasicStrategy	*			m_pBasicStrategy;

	//Trade info
	cwOrderReference			m_cwOrderRef;
	cwAccountPtr				m_pAccount;

	cwMUTEX						m_TradeSpiMutex;

	bool						m_bIsQryingPosition;
	std::map<std::string, cwPositionPtr> m_PositionMap;
	std::map<std::string, cwPositionPtr> m_PositionTempMap;

	//
	bool						m_bHasGetPosition;
	bool						m_bHasGetOrders;
	bool						m_bHasGetTrades;
	bool						m_bOrderRankedUpdate;

	std::map<std::string, cwOrderPtr>	m_OrdersMap;		//Key OrderSysID
	std::map<std::string, cwOrderPtr>	m_ActiveOrdersMap;	//Key OrderRef

	std::map<std::string, cwTradePtr>	m_TradeMap;			//key TradeID

	std::map<std::string, cwFtdcInstrumentStatusType>		m_ExchangeStatus;


	void						Reset();

	//UPDATE_ORDERRANKED
#ifdef  UPDATE_ORDERRANKED
	cwTickTradeManager			m_TickTradeManger;
#endif //  UPDATE_ORDERRANKED

#ifdef TRADELOG
	cwTradeLog					m_TradeLog;
#endif // TRADELOG

	//CWRISK
#ifdef CWRISK
	const int					m_iMaxCancelLimitNum;			//��󳷵�����
	std::map<std::string, int>	m_iCancelLimitMap;				//��������ͳ�ƣ�key:InstrumentID
	//���ر��� Ref�Ǽǣ� �����������س�������������׼ȷͳ��
	//key Isntrument, value : OrderRefSet;
	std::map<std::string, std::set<std::string>>	m_MayCancelOrderRefSetMap;		
#endif // CWRISK

	bool						m_bDisConnectExit;

	static	int					m_iTradeApiCount;
	bool						m_bSaveInstrumentDataToFile;
};

