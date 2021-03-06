#pragma once
// #include "utils.h"
// #include "HAL\interrupt.h"
// #include "HAL\uart_port.h"
// #include "Generic\TypeList.h"
#include "HAL/interrupt.h"
#include "HAL/uart_port.h"

namespace HAL
{
  template <class Handler, typename Tag>
  class Uart
  {
  public:
    typedef Int2Type<0> XmtInterruptTag;
    typedef Int2Type<1> RcvInterruptTag;
    typedef Int2Type<2> StatusInterruptTag;
    typedef Uart<Handler, Tag> ThisType;

    void InterruptHandler(XmtInterruptTag);
    void InterruptHandler(RcvInterruptTag);
    void InterruptHandler(StatusInterruptTag);

    enum
    {
      TimeScale = 525000, // ms
      DeadTime = 5000     // sec
    };

    template <int Port, class Pins>
    Uart(Int2Type<Port>, Type2Type<Pins>) : port_(Int2Type<Port>()),
                                            XmtInterrupt_(Detail::UartPortSelect<Port>::XmtInterruptID),
                                            RcvInterrupt_(Detail::UartPortSelect<Port>::RcvInterruptID),
                                            StatusInterrupt_(Detail::UartPortSelect<Port>::StatusInterruptID)
    {
      // FIXME:
      // MPL::foreach<Pins, Detail::PortInitPins>::Process();                // Detail::PortInitPins  -- ?
      InterruptSource<Detail::UartPortSelect<Port>::XmtInterruptID, XmtInterruptTag, ThisType>::SetHandler(this);
      InterruptSource<Detail::UartPortSelect<Port>::RcvInterruptID, RcvInterruptTag, ThisType>::SetHandler(this);
      InterruptSource<Detail::UartPortSelect<Port>::StatusInterruptID, StatusInterruptTag, ThisType>::SetHandler(this);
    }

    void Init(int baud);
    void SetHandler(Handler *h)
    {
      handler_ = h;
    }

    void SetBaudrate(int baud);

    void InitialRcv();
    void Xmt(void *ptr, int size);
    void Rcv(void *ptr, int size);

    inline unsigned char GetChar()
    {
      return port_.Reg()->RcvBuff;
    }

    inline void PutChar(unsigned char c)
    {
      port_.Reg()->XmtHoldReg = c;
    }

    inline void PutCharWait(unsigned char c)
    {
      while (!IsXmtEmpty())
      {
      }
      PutChar(c);
    }

    inline unsigned char GetCharWait()
    {
      while (!IsRcvDataReady())
      {
      }
      return GetChar();
    }

    int GetCharTimeout(int millisec);

    void Print(const unsigned char *str)
    {
      while (*str != 0)
        PutCharWait(*str++);
    }

    inline bool IsRcvRun()
    {
      return port_.RcvDma()->Config & 0x01; // 0x01 - Check if DMA is Enabled ?
    }
    inline bool IsXmtRun()
    {
      return port_.XmtDma()->Config & 0x01; // 0x01 - Check if DMA is Enabled ?
    }

    inline bool IsRcvDataReady()
    {
      return port_.Reg()->Status & 0x01; // 0x01(DR) - Check if Data is Ready ?
    }
    inline bool IsXmtFinished()
    {
      return port_.Reg()->Status & 0x80; // 0x80(TEMT) - Check if TSR/THR is Empty ?
    }
    inline bool IsXmtEmpty()
    {
      return port_.Reg()->Status & 0x20; // 0x20(THRE) - Check if Transmit Hold Register is Empty ?
    }

    inline bool IsAlive()
    {
      unsigned int time = clock() / TimeScale;
      return (time - xmtTime_) < DeadTime;
    }

    inline void ResetTimeout()
    {
      rcvTime_ = clock() / TimeScale;
      xmtTime_ = clock() / TimeScale;
    }

  private:
    const Detail::UartPort port_;
    const InterruptControl XmtInterrupt_;
    const InterruptControl RcvInterrupt_;
    const InterruptControl StatusInterrupt_;
    Handler *handler_;

    unsigned int xmtTime_;
    unsigned int rcvTime_;
  };
} // namespace HAL