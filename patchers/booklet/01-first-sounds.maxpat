{
 "patcher": {
  "fileversion": 1,
  "appversion": {
   "major": 8,
   "minor": 0,
   "revision": 0,
   "architecture": "x64",
   "modernui": 1
  },
  "classnamespace": "box",
  "rect": [
   60.0,
   80.0,
   960.0,
   640.0
  ],
  "bglocked": 0,
  "openinpresentation": 0,
  "default_fontsize": 12.0,
  "default_fontface": 0,
  "default_fontname": "Arial",
  "gridonopen": 1,
  "gridsize": [
   15.0,
   15.0
  ],
  "gridsnaponopen": 1,
  "objectsnaponopen": 1,
  "statusbarvisible": 2,
  "toolbarvisible": 1,
  "boxes": [
   {
    "box": {
     "id": "obj-1",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      20.0,
      12.0,
      620.0,
      24.0
     ],
     "text": "Hearing in Three Dimensions — 01 · First sounds",
     "fontsize": 16.0
    }
   },
   {
    "box": {
     "id": "obj-2",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      20.0,
      40.0,
      900.0,
      20.0
     ],
     "text": "A mono source orbits your head. Headphones on, volume low, click the speaker icon. (Book: Part I, chapter 3 — angles are radians; the expr boxes convert from degrees.)"
    }
   },
   {
    "box": {
     "id": "obj-3",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      110.0,
      50.0,
      22.0
     ],
     "text": "pink~",
     "outlettype": [
      "signal"
     ]
    }
   },
   {
    "box": {
     "id": "obj-4",
     "maxclass": "newobj",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      146.0,
      60.0,
      22.0
     ],
     "text": "*~ 0.25",
     "outlettype": [
      "signal"
     ]
    }
   },
   {
    "box": {
     "id": "obj-5",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      280.0,
      140.0,
      22.0
     ],
     "text": "ambitap.encode~ 3",
     "outlettype": [
      "multichannelsignal"
     ]
    }
   },
   {
    "box": {
     "id": "obj-6",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      40.0,
      400.0,
      145.0,
      22.0
     ],
     "text": "ambitap.binaural~ 3",
     "outlettype": [
      "signal",
      "signal"
     ]
    }
   },
   {
    "box": {
     "id": "obj-7",
     "maxclass": "ezdac~",
     "numinlets": 2,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      460.0,
      45.0,
      45.0
     ]
    }
   },
   {
    "box": {
     "id": "obj-8",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      230.0,
      330.0,
      180.0,
      100.0
     ],
     "text": "mc.scope~",
     "outlettype": []
    }
   },
   {
    "box": {
     "id": "obj-9",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      230.0,
      434.0,
      260.0,
      20.0
     ],
     "text": "the HOA bus: 16 channels in one cord (order 3)"
    }
   },
   {
    "box": {
     "id": "obj-10",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      460.0,
      86.0,
      220.0,
      20.0
     ],
     "text": "1 — steer it by hand"
    }
   },
   {
    "box": {
     "id": "obj-11",
     "maxclass": "dial",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      460.0,
      110.0,
      50.0,
      50.0
     ],
     "size": 361,
     "floatoutput": 1,
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-12",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      516.0,
      124.0,
      200.0,
      20.0
     ],
     "text": "azimuth, degrees (90 = left, 180 = behind)"
    }
   },
   {
    "box": {
     "id": "obj-13",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      460.0,
      170.0,
      170.0,
      22.0
     ],
     "text": "expr $f1 * 3.14159265 / 180.",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-14",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      460.0,
      204.0,
      90.0,
      22.0
     ],
     "text": "azimuth $1",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-15",
     "maxclass": "flonum",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      700.0,
      110.0,
      60.0,
      22.0
     ],
     "outlettype": [
      "",
      "bang"
     ]
    }
   },
   {
    "box": {
     "id": "obj-16",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      764.0,
      113.0,
      180.0,
      20.0
     ],
     "text": "elevation, degrees (90 = zenith)"
    }
   },
   {
    "box": {
     "id": "obj-17",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      700.0,
      170.0,
      170.0,
      22.0
     ],
     "text": "expr $f1 * 3.14159265 / 180.",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-18",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      700.0,
      204.0,
      95.0,
      22.0
     ],
     "text": "elevation $1",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-19",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      460.0,
      250.0,
      300.0,
      20.0
     ],
     "text": "2 — or let it orbit (one revolution every 20 s)"
    }
   },
   {
    "box": {
     "id": "obj-20",
     "maxclass": "toggle",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      460.0,
      276.0,
      24.0,
      24.0
     ],
     "outlettype": [
      "int"
     ]
    }
   },
   {
    "box": {
     "id": "obj-21",
     "maxclass": "newobj",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      530.0,
      276.0,
      90.0,
      22.0
     ],
     "text": "phasor~ 0.05",
     "outlettype": [
      "signal"
     ]
    }
   },
   {
    "box": {
     "id": "obj-22",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      530.0,
      312.0,
      90.0,
      22.0
     ],
     "text": "snapshot~ 30",
     "outlettype": [
      "float"
     ]
    }
   },
   {
    "box": {
     "id": "obj-23",
     "maxclass": "newobj",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      460.0,
      348.0,
      50.0,
      22.0
     ],
     "text": "gate",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-24",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      460.0,
      384.0,
      140.0,
      22.0
     ],
     "text": "expr $f1 * 6.2831853",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-25",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      460.0,
      420.0,
      90.0,
      22.0
     ],
     "text": "azimuth $1",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-26",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      86.0,
      200.0,
      20.0
     ],
     "text": "mono source"
    }
   },
   {
    "box": {
     "id": "obj-27",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      530.0,
      880.0,
      33.0
     ],
     "text": "3 — listen with your eyes closed. Behind and above are places stereo cannot reach (book, chapter 1). If elevation feels vague, that is normal — the rendering uses a standard mannequin's ears, not yours (chapter 13)."
    }
   }
  ],
  "lines": [
   {
    "patchline": {
     "destination": [
      "obj-4",
      0
     ],
     "source": [
      "obj-3",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-5",
      0
     ],
     "source": [
      "obj-4",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-6",
      0
     ],
     "source": [
      "obj-5",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-8",
      0
     ],
     "source": [
      "obj-5",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-7",
      0
     ],
     "source": [
      "obj-6",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-7",
      1
     ],
     "source": [
      "obj-6",
      1
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-13",
      0
     ],
     "source": [
      "obj-11",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-14",
      0
     ],
     "source": [
      "obj-13",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-5",
      0
     ],
     "source": [
      "obj-14",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-17",
      0
     ],
     "source": [
      "obj-15",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-18",
      0
     ],
     "source": [
      "obj-17",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-5",
      0
     ],
     "source": [
      "obj-18",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-23",
      0
     ],
     "source": [
      "obj-20",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-22",
      0
     ],
     "source": [
      "obj-21",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-23",
      1
     ],
     "source": [
      "obj-22",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-24",
      0
     ],
     "source": [
      "obj-23",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-25",
      0
     ],
     "source": [
      "obj-24",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-5",
      0
     ],
     "source": [
      "obj-25",
      0
     ]
    }
   }
  ]
 }
}
