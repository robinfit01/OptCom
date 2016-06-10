#include "stdafx.h"

void SerialCom::SetComPort(int no){
	comNo = GetComNoOfList(no);
}

int SerialCom::GetComNo()
{
	return comNo;
}

int SerialCom::GetBaud()
{
	return Baud;
}

void SerialCom::SetBaud(int baud){
	Baud = baud; 
}

bool SerialCom::InitCom(){
	return InitPort(comNo,Baud);
}



int SerialCom::UpdateComList()
{
	HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;
	SP_DEVINFO_DATA spdata = {0};
	GUID guid = GUID_DEVINTERFACE_COMPORT;
	CString buff;

	comList.RemoveAll();
	hDevInfo = SetupDiGetClassDevs(&guid, 0, 0, DIGCF_PRESENT|DIGCF_DEVICEINTERFACE);
	if(hDevInfo == INVALID_HANDLE_VALUE){
		std::cout << "error" << std::endl;
	}

	spdata.cbSize = sizeof(spdata);
	for(int i=0; SetupDiEnumDeviceInfo(hDevInfo, i, &spdata); i++){
		if(SetupDiGetDeviceRegistryProperty(hDevInfo, &spdata, SPDRP_FRIENDLYNAME, NULL, 
			PBYTE(buff.GetBuffer(1024)),1024, NULL))
		{
			// Prolific com port (COMxx)

			buff.ReleaseBuffer();
			comList.Add(buff);
		}
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);

	return GetComListSize();
}

CString SerialCom::GetComList(int index)
{
	if (index >= comList.GetSize())
		return CString("none");
	return comList[index];
}

int SerialCom::GetComListSize()
{
	return comList.GetSize();
}

int SerialCom::GetComNoOfList(int index)
{
	return SerialCom::GetComList(index).GetAt(GetComList(0).GetLength() - 2) - '0';
}

CString SerialCom::GetComName(int index)
{
	return GetComList(index).Left(GetComList(index).Find(L"(COM"));
}

SerialCom::SerialCom(){}
SerialCom::~SerialCom(){}
