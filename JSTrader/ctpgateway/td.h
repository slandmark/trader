#ifndef CTPTD_H
#define CTPTD_H
#ifdef  CTPGATEWAY_EXPORTS
#define CTPGATEWAY_API __declspec(dllexport)
#else
#define CTPGATEWAY_API __declspec(dllimport)
#endif
#include<string>
#include<map>
#include<sstream>
#include<stdlib.h>
#include<direct.h>
#include<algorithm>
#include"CTPAPI/ThostFtdcUserApiDataType.h"
#include"CTPGateway.h"
#include"structs.h"
#include"utils.hpp"
class CTPGateway;
class CTPGATEWAY_API  CTPTD :public CThostFtdcTraderSpi
{
public:
	CTPTD(CTPGateway *CTPGateway, std::string gatewayname);
	~CTPTD();
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);
	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///投资者结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询资金账户响应
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓响应
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单录入请求响应
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单操作请求响应
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///报单通知
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);
	///成交通知
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
	///报单录入错误回报
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
	// 是否收到成功的响应

	///报单操作错误回报
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

	bool IsFlowControl(int iResult);

	void connect(std::string userID, std::string password, std::string brokerID, std::string address);

	void login();

	void logout();

	void qryAccount();

	void qryPosition();

	std::string sendOrder(OrderReq & req);

	void cancelOrder(CancelOrderReq & req);

	void close();
private:
	CThostFtdcTraderApi* m_tdapi;
	CTPGateway *m_ctpgateway;
	std::string m_gatewayname;
	std::atomic_int m_reqID;  //请求编号
	int m_orderRef; std::mutex m_orderRefmtx;//报单编号
	std::atomic_bool m_connectionStatus;
	std::atomic_bool m_loginStatus;
	std::string m_userID;//账号密码 经纪商地址例如9999 1080 服务器地址tcp://xxx.xxx.xxx.xxx
	std::string m_password;
	std::string m_brokerID;
	std::string m_address;

	int m_frontID;//前置机编号
	int m_sessionID;//会话编号

	//
	std::map<std::string, std::shared_ptr<Event_Position>>m_posBufferMap;		std::mutex m_positionbuffermtx;//缓存持仓
	std::map<std::string, std::string>m_symbolExchangeMap;													   //只读不写，不需要加锁
	std::map<std::string, int>m_symbolSizeMap;
};
#endif