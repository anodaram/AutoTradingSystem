//////////////////////////////////////////////////////////////////////////////////
//*******************************************************************************
//---
//---	author: Wu Chang Sheng
//---
//---	CreateTime:	2016/12/12
//---
//---	VerifyTime:	2020/03/26
//---
//*******************************************************************************
//////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
#include <set>
#include <map>
#include <thread>
#include "cwProductTradeTime.h"
#include "cwTradeCommonDefine.h"
#include "cwStrategyLog.h"

class cwBasicStrategy
{
public:
	enum cwOpenCloseMode :int
	{
		CloseTodayThenYd = 0,//��ƽ����ƽ��,�ɿ�������ƽ�����Ʒ��
		OpenOnly = 1,//ֻ��
		CloseOnly = 2,//ֻƽ
		CloseYdThenOpen = 3,//��ƽ�򣬺󿪲֣���ƽ������ƽ��ܹ��Ʒ�֣��ײ���Ҫ��ȫ��ƽ���ٿ���
		CloseYdOrOpen = 4,//����ƽ�򣬿ɿ��֣����ֺ���ƽ�֣�����ƽ��ܹ��Ʒ�֣��ֲ����󿪲֣��ײ�����һ����ֿ���ûƽ
		CloseYdThenToday = 5//��ƽ����ƽ��,�ɿ�������ƽ����ˣ�ƽ��Ϳ��ֲ���Ʒ��
	};
	const char * GetOpenCloseModeString(cwOpenCloseMode openclose);

public:
	cwBasicStrategy();
	~cwBasicStrategy();

	//��ʾϵͳ�Ƿ��ʼ����ɣ������������б���������
	bool	m_bStrategyReady;

	//��ʾ��������
	virtual std::string  GetStrategyName() { return "BasicStrategy"; }

	/*
	PriceUpdate��OnRtnTrade��OnOrderCanceled��OnRspOrderInsert��OnRspOrderCancel�⼸��������ϵͳ�ص������������Ե�ͬ���麯��
	������Ӧ������±�ϵͳ���ã���ִ������ʵ�ֲ��Թ��ܡ�
	*/
	///MarketData SPI
	//�������
	virtual void PriceUpdate(cwMarketDataPtr pPriceData) = 0;
	
	///Trade SPI
	//�ɽ��ر�
	virtual void OnRtnTrade(cwTradePtr pTrade) = 0;
	//�����ر�, pOrderΪ���±�����pOriginOrderΪ��һ�θ��±����ṹ�壬�п���ΪNULL
	virtual void OnRtnOrder(cwOrderPtr pOrder, cwOrderPtr pOriginOrder = cwOrderPtr()) = 0;
	//�����ɹ�
	virtual void OnOrderCanceled(cwOrderPtr pOrder) = 0;
	//����¼��������Ӧ
	virtual void OnRspOrderInsert(cwOrderPtr pOrder, cwFtdcRspInfoField * pRspInfo) {};
	//��������������Ӧ
	virtual void OnRspOrderCancel(cwOrderPtr pOrder, cwFtdcRspInfoField * pRspInfo) {};

	///Strategy Timer
	virtual void OnStrategyTimer(int iTimerId) {};

	///Special For Simulation 
	///These functions will NOT be called in normal mode
	//�زⲿ�ֽ�����ҹ�̽��������̽����������ã�
	virtual void OnSimulationPartEnd() {};
	//�����ز������������
	virtual void OnSimulationFinished() {};

	virtual void InitialStrategy(const char * pConfigFilePath);
	std::string			m_strConfigFileFullPath;

	//��Trade SPI׼������ǰ��������Ҫ�õ���Լ��Ϣ���������øú����ȴ��ļ��л�ȡ��Լ��Ϣ������ΪNULLʱ��Ĭ�Ϻͳ�����ͬһ·��
	virtual void InitalInstrumentData(const char * pInstrumentDataFilePath = NULL);

	///Action  Function
	//��ȡ���µ�����
	cwMarketDataPtr	GetLastestMarketData(std::string InstrumentID);
	//��ȡ�˻���Ϣ
	cwAccountPtr GetAccount();
	//��ȡ�ҵ��б�����map���ڷ�����Ϣ�����ر������(OrderRef)ΪKey
	bool GetActiveOrders(std::map<std::string, cwOrderPtr>& ActiveOrders);		///key OrderRef
	//��ȡ�ҵ��б������Լ��map���ڷ�����Ϣ�����ر������(OrderRef)ΪKey
	bool GetActiveOrders(std::string InstrumentID, std::map<std::string, cwOrderPtr>& ActiveOrders);		///key OrderRef
	//��ȡ��ͷ�ҵ������������Լ
	int GetActiveOrdersLong(std::string InstrumentID);		///key OrderRef
	//��ȡ��ͷ�ҵ������������Լ
	int GetActiveOrdersShort(std::string InstrumentID);		///key OrderRef
	//��ȡ���б����б�����map���ڷ�����Ϣ���������������(sysOrderID)ΪKey
	bool GetAllOrders(std::map<std::string, cwOrderPtr>& Orders);				///Key OrderSysID
	//��ȡ�ֲ��б�����map���ڷ�����Ϣ����ԼIDΪKey
	bool GetPositions(std::map<std::string, cwPositionPtr>& PositionMap);		///Key InstrumentID
	//��ȡ�ֲֺ͹ҵ��б�
	bool GetPositionsAndActiveOrders(std::map<std::string, cwPositionPtr>& PositionMap,
		std::map<std::string, cwOrderPtr>& ActiveOrders);
	bool GetPositionsAndActiveOrders(std::string InstrumentID, cwPositionPtr& pPosition, std::map<std::string, cwOrderPtr>& ActiveOrders);
	//��ȡ��Լ��Ϣ
	cwInstrumentDataPtr GetInstrumentData(std::string InstrumentID);

	//��������--�޼۵�
	cwOrderPtr InputLimitOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);
	//��������--FAK����Filled And Kill �����ɽ�ʣ���Զ�����ָ�
	cwOrderPtr InputFAKOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);
	//��������--FOK��(FOK Filled Or Kill ����ȫ���ɽ������Զ�����ָ��)
	cwOrderPtr InputFOKOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);

	//�򻯱��������� volume����ʾ�򣬸���ʾ�����Զ���ƽ���гֲ־�ƽ�֣�û�оͿ���
	cwOrderPtr EasyInputOrder(const char * szInstrumentID, int volume, double price,
		cwOpenCloseMode openclosemode = cwOpenCloseMode::CloseTodayThenYd,
		cwInsertOrderType insertordertype = cwInsertOrderType::cwInsertLimitOrder);

	//��������
	bool CancelOrder(cwOrderPtr pOrder);

	//���ĺ�Լ
	void	   SubScribePrice(std::vector<std::string>& SubscribeInstrument);
	//ȡ�����ĺ�Լ
	void	   UnSubScribePrice(std::vector<std::string>& UnSubscribeInstrument);

	//��Լ��Ϣ�б���Ҫ��С�䶯����Լ��������Ϣ����ͨ�����map��ȡ����ԼIDΪKey
	std::map<std::string, cwInstrumentDataPtr> m_InstrumentMap;
	//��ȡ��Լ��С�䶯�������ȡʧ�ܷ���-1
	double    GetTickSize(const char * szInstrumentID);
	//��ȡ��Լ�����������ȡʧ�ܷ���-1
	double	  GetMultiplier(const char * szInstrumentID);
	//��ȡ��ƷID 
	char *    GetProductID(const char * szInstrumentID);
	//��ȡ����ʱ��Σ��࿪�̶�����;����̶�����
	//��������Լ��������ʱ�䣨102835->10:28:35),���׽׶Σ� ��ý���ʱ�ο��̶����룬�����̶�����
	bool	  GetTradeTimeSpace(const char * szInstrumentID, const char * updatetime,
		cwProductTradeTime::cwTradeTimeSpace& iTradeIndex, int& iOpen, int& iClose);
	//��ȡǰһ������ʱ�ε���ǰ����ʱ�ο���ʱ����
	int		  GetPreTimeSpaceInterval(const char * szInstrumentID, cwProductTradeTime::cwTradeTimeSpace iTradeIndex);

	//��ȡ��Լ��ǰ��������
	int		  GetInstrumentCancelCount(std::string InstrumentID);
	//��ȡ��Լ�Ƿ��Ƕ���
	bool	  IsThisStrategySubScribed(std::string InstrumentID);
	//��ȡ��ǰ״̬�Ƿ�Ϊ�ز�ģ�����
	inline bool GetIsSimulation() { return m_bIsSimulation; }

	//���ö�ʱ�� iTimerId��ʱ��id����OnTimer�ص����ݴ�id�ж����ĸ���ʱ������, iElapse ������������룩
	//Ŀǰ��֧��10����ʱ������ʱ���ڻص��������������߼������ж�ʱ���ص�����һ���̡߳�
	//ͬ��id�£�����������ᱻ����
	bool	  SetTimer(int iTimerId, int iElapse);
	void	  RemoveTimer(int iTimerId);

	///������ظú�������ȷ��������ĺ����ܹ������õ���
	virtual void	   SetStrategyReady();
	///ϵͳ���ýӿ���Ϣ����
	void					SetMdSpi(cwMDAPIType apiType, void * pSpi);
	void					SetTradeSpi(cwTradeAPIType apiType, void *pSpi);
	void					SetIsSimulation(bool IsSimulation = false) { m_bIsSimulation = IsSimulation; };
private:
	///ϵͳ���ýӿ���Ϣ����
	bool					m_bIsSimulation;

	void *					m_pTradeSpi;
	cwTradeAPIType			m_TradeApiType;

	void *					m_pMdSpi;
	cwMDAPIType				m_MdApiType;


	std::set<std::string>	m_SubscribeInstrumentSet;
	cwProductTradeTime		m_ProductTradeTime;

	cwStrategyLog			m_BasicStrategyLog;

	//Timer	key:TimerID, value:Elapse in ms
	std::map<int, int>		m_cwTimerElapseMap;

	std::thread				m_StrategyTimerWorkingThread;
	volatile bool			m_bStrategyTimerWorkingThreadRun;
	void					StrategyTimerWorkingThread();
};
