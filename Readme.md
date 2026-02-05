# Rend Raytracer in DOD Style

### Final Image

#### Work In Progress

![Final Render](https://github.com/madoodia/Rend/blob/main/output/FinalRender.bmp)

---

#### Multi-Thread More rays and bounces

    Samples: 16
    Ray Bounces: 128
    Configurations: 16 Cores with 144 of 80x80 (25k) tiles.
    Render Time: 8677ms
    Total Bounces: 481630421
    Performance: 0.000018ms/bounces

---

#### Multi-Thread

    Samples: 16
    Ray Bounces: 32
    Configurations: 16 Cores with 144 of 80x80 (25k) tiles.
    Render Time: 532ms
    Total Bounces: 29698988
    Performance: 0.000018ms/bounces

---

#### Single-Thread

    Samples: 16
    Ray Bounces: 32
    Configurations: 8 Cores with 40 of 160x160 (100k) tiles.
    Render Time: 4524ms
    Total Bounces: 29704752
    Performance: 0.000152ms/bounces

---
