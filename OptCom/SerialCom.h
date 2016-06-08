#ifndef SERIALCOM_H_
#define SERIALCOM_H_

#pragma once

#include "stdafx.h"

class SerialCom: public CSerialPort
{
private:
	int comNo = -1;
	int Baud = 9600;
	CStringArray comList;

public:
	int UpdateComList();
	CString GetComList(int index);
	int GetComListSize();
	int GetComNoOfList(int index);
	CString GetComName(int index);

	void SetComPort(int no);
	int GetComNo();
	int GetBaud();
	void SetBaud(int baud);
	bool InitCom();

	SerialCom();
	~SerialCom();
};
#endif //SERIALPORT_H_