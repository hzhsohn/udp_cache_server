#pragma once

template<typename T>
class UDPMagrDelegate 
{
	typedef void (T::*RecvfromCB)(char*ip,int port,char* data,int len);

private:
    T* m_pT;
	RecvfromCB recvf_cb;
	
public:
	
    void set_recv_cb(T* pT, RecvfromCB pf)
    {
        m_pT=pT;
        recvf_cb=pf;
    } 

    void callback_recv_cb(char*ip,int port,char* data,int len) 
	{
        (m_pT->*recvf_cb)(ip,port,data,len);
    }

};
