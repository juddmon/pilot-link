
#ifndef _PILOT_SYSPKT_H
#define _PILOT_SYSPKT_H

struct RPC_param {
  int byRef;
  int size;
  int invert;
  void * data;
};

extern int syspkt_tx(struct pi_socket *ps, unsigned char *msg, int length);
                                    
extern int syspkt_rx(struct pi_socket *ps, unsigned char *buf, int len);
                                    

extern int sys_RemoteEvent(int sd, int penDown, int x, int y, int keypressed, 
                       int keymod, int keyasc, int keycode);

extern int sys_RPC(int sd, int dlp, int trap, long * D0, long * A0, int params, struct RPC_param * param, int rep);

#define RPC_Short(data) (-2),((unsigned int)htons((data)))
#define RPC_Long(data) (-4),((unsigned int)htonl((data)))
#define RPC_Ptr(data,len) (len),((void*)(data)),0
#define RPC_LongPtr(ptr) (4),((void*)(ptr)),1
#define RPC_ShortPtr(ptr) (2),((void*)(ptr)),1
#define RPC_LongRef(ref) (4),((void*)(&(ref))),1
#define RPC_ShortRef(ref) (2),((void*)(&(ref))),1
#define RPC_End 0

extern int RPC(int sd, int trap, int ret, ...);

extern int dlp_ProcessRPC(int sd, int trap, int ret, ...);

extern int RPC_Int_Void(int sd, int trap);
extern int RPC_Ptr_Void(int sd, int trap);

#endif /*_PILOT_SYSPKT_H_*/
