#pragma once
class IClima {
  public:
    virtual ~IClima() {}
    virtual int probLluviaDia() = 0; // 0..100
    virtual void tick() = 0;
};
