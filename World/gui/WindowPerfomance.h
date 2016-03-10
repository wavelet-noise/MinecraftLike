#pragma once
#include <GLFW\glfw3.h>
#include "WindowBase.h"
#include <array>
#include <string>
#include <mutex>

class WindowPerfomance : public WindowBase
{
public:
  void Draw() override;
  void DtUpdate(float dt, int);
  void TesselatorDt(float dt);
  void GeneratorDt(float dt);
  static WindowPerfomance &Get()
  {
    static WindowPerfomance w;
    return w;
  }

private:
  std::mutex lock;

  std::array<float, 100> fps_sec, fps_subsec;
  int fps_sec_cur, fps_subsec_cur;
  float fps_sec_timer, fps_subsec_timer, dt;

  struct PerfHelper
  {
  public:
    PerfHelper(const std::string &s);
    void Draw();
    void Update(float dt);
    void Set(float tes_dt);
    float Get();

  private:
    float tes_dt{}, tes_dt_shown{}, tes_dt_mean{}, tes_dt_max{};
    int tes_last{}, tes_cur{};
    std::string label;

  } tess_perf{ "tess dt" }, gen_perf{ "gen dt" };

  std::string mems;
};