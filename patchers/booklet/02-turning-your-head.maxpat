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
   980.0,
   660.0
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
      640.0,
      24.0
     ],
     "text": "Hearing in Three Dimensions — 02 · Turning your head",
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
      930.0,
      20.0
     ],
     "text": "Two sources on one bus: noise ahead, a tone behind. One rotator turns the whole world; the renderer's own yaw turns your head. (Book: Part I, chapter 4.)"
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
      100.0,
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
      136.0,
      60.0,
      22.0
     ],
     "text": "*~ 0.2",
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
      210.0,
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
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      236.0,
      160.0,
      20.0
     ],
     "text": "noise, dead ahead"
    }
   },
   {
    "box": {
     "id": "obj-7",
     "maxclass": "newobj",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      230.0,
      100.0,
      80.0,
      22.0
     ],
     "text": "cycle~ 220",
     "outlettype": [
      "signal"
     ]
    }
   },
   {
    "box": {
     "id": "obj-8",
     "maxclass": "newobj",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      230.0,
      136.0,
      60.0,
      22.0
     ],
     "text": "*~ 0.1",
     "outlettype": [
      "signal"
     ]
    }
   },
   {
    "box": {
     "id": "obj-9",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      310.0,
      172.0,
      130.0,
      22.0
     ],
     "text": "azimuth 3.14159265",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-10",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      230.0,
      210.0,
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
     "id": "obj-11",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      230.0,
      236.0,
      210.0,
      20.0
     ],
     "text": "tone, behind you (click the message!)"
    }
   },
   {
    "box": {
     "id": "obj-12",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      40.0,
      300.0,
      135.0,
      22.0
     ],
     "text": "ambitap.rotate~ 3",
     "outlettype": [
      "multichannelsignal"
     ]
    }
   },
   {
    "box": {
     "id": "obj-13",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      185.0,
      303.0,
      170.0,
      20.0
     ],
     "text": "← turns the WORLD"
    }
   },
   {
    "box": {
     "id": "obj-14",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 2,
     "patching_rect": [
      40.0,
      390.0,
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
     "id": "obj-15",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      190.0,
      393.0,
      170.0,
      20.0
     ],
     "text": "← turns YOUR HEAD"
    }
   },
   {
    "box": {
     "id": "obj-16",
     "maxclass": "ezdac~",
     "numinlets": 2,
     "numoutlets": 0,
     "patching_rect": [
      40.0,
      450.0,
      45.0,
      45.0
     ]
    }
   },
   {
    "box": {
     "id": "obj-17",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      500.0,
      86.0,
      260.0,
      20.0
     ],
     "text": "scene yaw (degrees) → ambitap.rotate~"
    }
   },
   {
    "box": {
     "id": "obj-18",
     "maxclass": "dial",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      500.0,
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
     "id": "obj-19",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      500.0,
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
     "id": "obj-20",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      500.0,
      204.0,
      60.0,
      22.0
     ],
     "text": "yaw $1",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-21",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      720.0,
      86.0,
      250.0,
      20.0
     ],
     "text": "head yaw (degrees) → ambitap.binaural~"
    }
   },
   {
    "box": {
     "id": "obj-22",
     "maxclass": "dial",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      720.0,
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
     "id": "obj-23",
     "maxclass": "newobj",
     "numinlets": 1,
     "numoutlets": 1,
     "patching_rect": [
      720.0,
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
     "id": "obj-24",
     "maxclass": "message",
     "numinlets": 2,
     "numoutlets": 1,
     "patching_rect": [
      720.0,
      204.0,
      60.0,
      22.0
     ],
     "text": "yaw $1",
     "outlettype": [
      ""
     ]
    }
   },
   {
    "box": {
     "id": "obj-25",
     "maxclass": "comment",
     "numinlets": 1,
     "numoutlets": 0,
     "patching_rect": [
      500.0,
      260.0,
      440.0,
      47.0
     ],
     "text": "Experiments: (1) sweep scene yaw — both sources turn rigidly together, keeping their 180° separation. (2) sweep head yaw instead — positive turns your head left, so the front source lands at your right ear. (3) set BOTH dials to the same angle: world and head cancel, nothing moves."
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
      530.0,
      900.0,
      33.0
     ],
     "text": "Joining two mc patch cords into one inlet sums the buses — that IS mixing in ambisonics. The rotator multiplies 16 channels by one matrix: the cost is identical for two sources or two hundred. This is why VR audio runs on Ambisonics (book, chapter 4)."
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
      "obj-8",
      0
     ],
     "source": [
      "obj-7",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-10",
      0
     ],
     "source": [
      "obj-8",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-10",
      0
     ],
     "source": [
      "obj-9",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-12",
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
      "obj-12",
      0
     ],
     "source": [
      "obj-10",
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
      "obj-12",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-16",
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
      "obj-16",
      1
     ],
     "source": [
      "obj-14",
      1
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-19",
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
      "obj-20",
      0
     ],
     "source": [
      "obj-19",
      0
     ]
    }
   },
   {
    "patchline": {
     "destination": [
      "obj-12",
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
      "obj-23",
      0
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
      "obj-14",
      0
     ],
     "source": [
      "obj-24",
      0
     ]
    }
   }
  ]
 }
}
