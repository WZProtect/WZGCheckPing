






BOOL WZGCheckPing()
{
	//beta test
	if(InternetCheckConnection("http://www.google.com",FLAG_ICC_FORCE_CONNECTION,0))
	{
		return TRUE;
	}else{
		return FALSE;
	}


	if (ipaddr == INADDR_NONE)
	{
		if (hp){
			memcpy(&ipaddr, hp->h_addr, hp->h_length);
		}
		else{
			return FALSE;
		}
	}
	if (hIcmpFile == INVALID_HANDLE_VALUE){
		return FALSE;
	}
	if (ReplyBuffer == NULL) 
	{
		return FALSE;
	}

	memset(ReplyBuffer, 0, ReplySize);
	pIcmpEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
	dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);

	int iCount = 0;//beer
	if (dwRetVal != 0)
	{
		switch (pIcmpEchoReply->Status) {
		case IP_DEST_HOST_UNREACHABLE:
			return FALSE;
			break;
		case IP_DEST_NET_UNREACHABLE:
			return FALSE;
			break;
		case IP_REQ_TIMED_OUT:
			return FALSE;
			break;
		default:
			return TRUE;
			break;
		}
	}
	else
	{
		dwError = GetLastError();
		switch (dwError) {
		case IP_BUF_TOO_SMALL:
			return TRUE;
			break;
		case IP_REQ_TIMED_OUT:
			{
			iCount++;

			if (iCount >=3){
				return FALSE;
			}
			
			return TRUE;
			break;
			}
		default:
			return TRUE;
			break;
		}
	}
}

